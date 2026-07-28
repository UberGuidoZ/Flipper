#!/usr/bin/env python3
"""flippercheck - a static validator for Flipper Zero data files.

Checks .sub (Sub-GHz), .ir (Infrared), RTTTL music, and Sub-GHz playlist files
for the structural mistakes that make them fail (or silently misbehave) on a real
Flipper, without needing a Flipper plugged in. It reads files, it never transmits.

Rules come from the Flipper firmware source and the CC1101 datasheet, checked
against real files in the wild. Where the firmware tolerates something the docs
forbid (RAW captures that start on a negative/space value, for example) the tool
follows the firmware, so it doesn't flag the thousands of good files that do it.

Usage:
    flippercheck PATH [PATH ...]        files or folders (folders are walked)
    flippercheck --type sub file        force a type instead of guessing
    flippercheck --firmware ofw PATH    strict: only stock-firmware protocols/presets
    flippercheck --fix PATH             fix trailing spaces / CRLF / final newline
    flippercheck --json PATH            machine-readable output
    flippercheck --quiet PATH           only print problems, not clean files
    flippercheck --strict PATH          treat warnings as failures too

Exit codes: 0 = clean, 1 = warnings only, 2 = at least one error.
"""

import argparse
import json
import os
import re
import sys

__version__ = "1.0.0"

# ---------------------------------------------------------------------------
# Ground-truth tables (firmware dev branch, mid-2026; CC1101 datasheet SWRS061)
# ---------------------------------------------------------------------------

# furi_hal_subghz_is_frequency_valid() - the three bands the CC1101 synthesizer
# can actually lock. Anything outside these could not have come off real hardware.
SUB_FREQ_BANDS = (
    (299_999_755, 348_000_335),
    (386_999_938, 464_000_000),
    (778_999_847, 928_000_000),
)

# The only six Preset: strings any shipped Flipper parser resolves. The C enum has
# nine, but MSK99_97Kb / GFSK9_99Kb / IDLE are internal-only and never valid in a file.
SUB_PRESETS = frozenset({
    "FuriHalSubGhzPresetOok270Async",
    "FuriHalSubGhzPresetOok650Async",
    "FuriHalSubGhzPreset2FSKDev238Async",
    "FuriHalSubGhzPreset2FSKDev12KAsync",
    "FuriHalSubGhzPreset2FSKDev476Async",
    "FuriHalSubGhzPresetCustom",
})
SUB_PRESETS_INTERNAL_ONLY = frozenset({
    "FuriHalSubGhzPresetIDLE",
    "FuriHalSubGhzPresetMSK99_97KbAsync",
    "FuriHalSubGhzPresetGFSK9_99KbAsync",
})

# Registered Protocol: names in stock OFW (lib/subghz/protocols/protocol_items.c).
# Forks (Unleashed/Momentum/RogueMaster) add many more, so an unknown name is only
# an error under --firmware ofw; otherwise it's a note.
SUB_PROTOCOLS_OFW = frozenset({
    "RAW", "BinRAW", "Princeton", "KeeLoq", "Star Line", "CAME", "CAME TWEE",
    "CAME Atomo", "Nice FLO", "Nice FloR-S", "GateTX", "Faac SLH", "Nero Sketch",
    "Nero Radio", "iDo 117/111", "KIA Seed", "Hormann HSM", "Somfy Telis",
    "Somfy Keytis", "Scher-Khan", "Linear", "LinearDelta3", "Security+ 1.0",
    "Security+ 2.0", "MegaCode", "Holtek", "Holtek_HT12X", "Cham_Code",
    "Power Smart", "Marantec", "Marantec24", "BETT", "Doitrand", "Phoenix_V2",
    "Honeywell", "Magellan", "Intertechno_V3", "Clemsa", "Ansonic", "SMC5326",
    "Dooya", "Alutech AT-4N", "KingGates Stylo4k", "Mastercode", "Legrand",
    "Dickert_MAHS", "GangQi", "Hollarm", "Hay21", "Revers_RB2", "Feron",
    "Roger", "Elplast",
})
# Protocols that require an extra field beyond the generic Bit+Key.
SUB_PROTO_NEEDS_MANUFACTURE = frozenset({"KeeLoq", "Star Line"})
SUB_PROTO_NEEDS_TE = frozenset({"Princeton"})

# 2^30-1: the max single RAW value the 30-bit LevelDuration field can carry on TX.
RAW_MAX_DURATION = 1_073_741_823
RAW_LINE_MAX_VALUES = 512            # writer chunk size (SUBGHZ_DOWNLOAD_MAX_SIZE)
RAW_LIVE_CAPTURE_MAX = 1_000_000     # live capture drops anything >= 1s
RAW_LIVE_CAPTURE_MIN = 50            # live capture drops transitions <= 50us

# Infrared: parsed protocol names shipped in OFW (InfraredFileFormats.md).
IR_PROTOCOLS_OFW = frozenset({
    "NEC", "NECext", "NEC42", "NEC42ext", "Samsung32", "RC6", "RC5", "RC5X",
    "SIRC", "SIRC15", "SIRC20", "Kaseikyo", "RCA",
})
IR_DATA_MAX = 1024

SUB_FILETYPES = frozenset({"Flipper SubGhz Key File", "Flipper SubGhz RAW File"})
# Firmware writes "IR signals file"; older community libraries use "IR library file".
IR_FILETYPES = frozenset({"IR signals file", "IR library file"})

# ---------------------------------------------------------------------------
# Diagnostics
# ---------------------------------------------------------------------------

ERROR, WARN, INFO = "error", "warning", "info"
_LEVEL_RANK = {INFO: 0, WARN: 1, ERROR: 2}


class Finding:
    __slots__ = ("path", "line", "level", "code", "msg")

    def __init__(self, path, line, level, code, msg):
        self.path = path
        self.line = line
        self.level = level
        self.code = code
        self.msg = msg

    def as_dict(self):
        return {"file": self.path, "line": self.line, "level": self.level,
                "code": self.code, "message": self.msg}


class Report:
    def __init__(self, path):
        self.path = path
        self.findings = []
        self.checked_as = None

    def add(self, line, level, code, msg):
        self.findings.append(Finding(self.path, line, level, code, msg))

    def error(self, line, code, msg): self.add(line, ERROR, code, msg)
    def warn(self, line, code, msg): self.add(line, WARN, code, msg)
    def info(self, line, code, msg): self.add(line, INFO, code, msg)

    def worst(self):
        return max((_LEVEL_RANK[f.level] for f in self.findings), default=-1)


# ---------------------------------------------------------------------------
# Reading / parsing helpers
# ---------------------------------------------------------------------------

def read_text(path):
    """Return (lines, meta). lines are without their line endings. meta records the
    line-ending style and whether the file ended with a newline, for --fix."""
    with open(path, "rb") as fh:
        raw = fh.read()
    crlf = raw.count(b"\r\n")
    lf = raw.count(b"\n") - crlf
    meta = {
        "crlf": crlf,
        "lf": lf,
        "final_newline": raw.endswith(b"\n"),
        "size": len(raw),
    }
    text = raw.decode("utf-8", errors="replace")
    lines = text.splitlines()
    return lines, meta


def split_field(line):
    """'Key: value' -> ('Key', 'value'). Returns (None, None) if not a field line."""
    if ":" not in line:
        return None, None
    key, _, val = line.partition(":")
    if not key or " " in key.strip() and not key.strip().replace(" ", "").isalnum():
        # keys are single tokens; a stray colon in prose isn't a field
        pass
    return key.strip(), val.strip()


HEX_TOKEN = re.compile(r"^[0-9A-Fa-f]{2}$")


def is_hex_bytes(value, count=None):
    toks = value.split()
    if count is not None and len(toks) != count:
        return False
    return bool(toks) and all(HEX_TOKEN.match(t) for t in toks)


def trailing_ws_lines(lines):
    return [i + 1 for i, ln in enumerate(lines) if ln != ln.rstrip()]


# ---------------------------------------------------------------------------
# .sub linter
# ---------------------------------------------------------------------------

def lint_sub(lines, meta, rep, firmware):
    rep.checked_as = "sub"
    fields, order = {}, []
    for idx, ln in enumerate(lines, 1):
        s = ln.strip()
        if not s or s.startswith("#"):
            continue
        key, val = split_field(ln)
        if key is None:
            rep.warn(idx, "sub.stray_line", "line is not a 'Field: value' pair")
            continue
        order.append((key, val, idx))
        fields.setdefault(key, (val, idx))

    def line_of(k):
        return fields[k][1] if k in fields else 0

    # Header
    if "Filetype" not in fields:
        rep.error(1, "sub.no_filetype", "missing 'Filetype:' header")
    else:
        ft = fields["Filetype"][0]
        if ft not in SUB_FILETYPES:
            rep.warn(line_of("Filetype"), "sub.filetype",
                     "unusual Filetype %r (expected one of %s)"
                     % (ft, ", ".join(sorted(SUB_FILETYPES))))
    if "Version" not in fields:
        rep.error(1, "sub.no_version", "missing 'Version:' header")
    elif not fields["Version"][0].isdigit():
        rep.error(line_of("Version"), "sub.version", "Version must be an integer")

    # Frequency
    if "Frequency" not in fields:
        rep.error(1, "sub.no_frequency", "missing 'Frequency:' header")
    else:
        fv, fline = fields["Frequency"]
        if not fv.isdigit():
            rep.error(fline, "sub.freq_nan", "Frequency must be an integer in Hz")
        else:
            hz = int(fv)
            if not any(lo <= hz <= hi for lo, hi in SUB_FREQ_BANDS):
                rep.error(fline, "sub.freq_band",
                          "%d Hz is outside the CC1101 bands "
                          "(300-348 / 387-464 / 779-928 MHz); a real Flipper cannot "
                          "tune here" % hz)

    # Preset
    preset = fields.get("Preset", (None, 0))[0]
    if preset is None:
        rep.error(1, "sub.no_preset", "missing 'Preset:' header")
    elif preset in SUB_PRESETS_INTERNAL_ONLY:
        rep.error(line_of("Preset"), "sub.preset_internal",
                  "%s is an internal firmware state, never valid in a file" % preset)
    elif preset not in SUB_PRESETS:
        rep.error(line_of("Preset"), "sub.preset_unknown",
                  "unknown Preset %r (no shipped parser resolves it)" % preset)
    elif preset == "FuriHalSubGhzPresetCustom":
        _check_custom_preset(fields, rep)

    # Protocol / payload
    proto = fields.get("Protocol", (None, 0))[0]
    if proto is None:
        rep.error(1, "sub.no_protocol", "missing 'Protocol:' field")
        return
    if proto == "RAW":
        _check_raw(lines, rep)
    elif proto == "BinRAW":
        _check_binraw(fields, rep)
    else:
        _check_parsed_sub(proto, fields, rep, firmware)

    _check_preset_protocol_consistency(preset, proto, fields, rep)


def _check_custom_preset(fields, rep):
    mod = fields.get("Custom_preset_module")
    data = fields.get("Custom_preset_data")
    if mod is None:
        rep.error(fields["Preset"][1], "sub.custom_no_module",
                  "Custom preset needs 'Custom_preset_module: CC1101'")
    elif mod[0] != "CC1101":
        rep.warn(mod[1], "sub.custom_module",
                 "Custom_preset_module is %r; Flipper Zero only has CC1101" % mod[0])
    if data is None:
        rep.error(fields["Preset"][1], "sub.custom_no_data",
                  "Custom preset needs a 'Custom_preset_data:' register block")
        return
    val, dline = data
    toks = val.split()
    if not all(HEX_TOKEN.match(t) for t in toks):
        rep.error(dline, "sub.custom_hex",
                  "Custom_preset_data must be space-separated hex byte pairs")
        return
    bytes_ = [int(t, 16) for t in toks]
    # structure: XX YY ... 00 00 <8 PA bytes>. The loader stops at the first 0x00
    # address, so a 0x00 register address mid-block silently truncates the preset.
    term = None
    i = 0
    while i + 1 < len(bytes_):
        if bytes_[i] == 0x00:
            term = i
            break
        if not (0x01 <= bytes_[i] <= 0x2E):
            rep.warn(dline, "sub.custom_reg_range",
                     "register address 0x%02X is outside CC1101's config range "
                     "0x01-0x2E" % bytes_[i])
        i += 2
    if term is None:
        rep.error(dline, "sub.custom_no_terminator",
                  "Custom_preset_data has no '00 00' end-of-registers marker")
        return
    if (len(bytes_) - term) != 10:
        rep.error(dline, "sub.custom_pa_table",
                  "expected exactly '00 00' + 8 power-table bytes after the "
                  "registers; found %d trailing bytes" % (len(bytes_) - term))
    if term % 2 != 0:
        rep.error(dline, "sub.custom_reg_pairs",
                  "register section has an odd byte count (must be address/value pairs)")


def _check_raw(lines, rep):
    proto_idx = None
    for i, ln in enumerate(lines):
        s = ln.strip()
        if s.startswith("Protocol:") and s.split(":", 1)[1].strip() == "RAW":
            proto_idx = i
            break
    if proto_idx is None:
        return
    have_data = False
    stray_line = 0            # first non-RAW_Data content line after Protocol: RAW
    data_after_stray = 0      # RAW_Data that resumes after such a line (real data loss)
    prev_sign = 0
    nonalt_count = 0
    nonalt_line = 0
    for i in range(proto_idx + 1, len(lines)):
        lineno = i + 1
        s = lines[i].strip()
        if not s or s.startswith("#"):
            continue
        if not s.startswith("RAW_Data:"):
            # A trailing field after all RAW_Data (a Repeat: line, say) is harmless -
            # the data was already read. Only RAW_Data that RESUMES after such a line
            # is actually dropped on transmit, so that is the case worth erroring on.
            if stray_line == 0:
                stray_line = lineno
            continue
        if stray_line and not data_after_stray:
            data_after_stray = lineno
        have_data = True
        vals = s.split(":", 1)[1].split()
        if len(vals) > RAW_LINE_MAX_VALUES:
            rep.info(lineno, "sub.raw_line_len",
                     "%d values on one RAW_Data line (firmware writes at most %d)"
                     % (len(vals), RAW_LINE_MAX_VALUES))
        for tok in vals:
            try:
                v = int(tok)
            except ValueError:
                rep.error(lineno, "sub.raw_nan",
                          "RAW_Data value %r is not an integer" % tok)
                continue
            if v == 0:
                rep.error(lineno, "sub.raw_zero",
                          "RAW_Data value 0 is a stop sentinel; it truncates transmission")
                continue
            # Sign should alternate on a clean capture, but plenty of real
            # (tool-generated or noisy) files carry consecutive same-sign values and
            # the firmware just plays them back-to-back. A note, not an error -
            # erroring here would reject ~14% of the real files in the wild.
            sign = 1 if v > 0 else -1
            if prev_sign != 0 and sign == prev_sign:
                nonalt_count += 1
                if not nonalt_line:
                    nonalt_line = lineno
            prev_sign = sign
            mag = abs(v)
            if mag > RAW_MAX_DURATION:
                rep.error(lineno, "sub.raw_overflow",
                          "|%d| exceeds the 30-bit transmit limit (%d us); it is "
                          "silently corrupted on air" % (v, RAW_MAX_DURATION))
            elif mag >= RAW_LIVE_CAPTURE_MAX:
                rep.warn(lineno, "sub.raw_long",
                         "very long single pulse (%d us); only plausible as a "
                         "deliberate delay, not a captured signal" % v)
    if data_after_stray:
        rep.error(data_after_stray, "sub.raw_interleave",
                  "RAW_Data resumes after a non-RAW_Data line; on transmit the values "
                  "above that line are dropped")
    if nonalt_count:
        rep.info(nonalt_line, "sub.raw_nonalternating",
                 "%d consecutive same-sign value(s) in RAW_Data; tolerated by firmware "
                 "but atypical of a clean capture" % nonalt_count)
    if not have_data:
        rep.error(proto_idx + 1, "sub.raw_no_data", "Protocol: RAW but no RAW_Data lines")


def _check_binraw(fields, rep):
    if "Bit" not in fields:
        rep.error(0, "sub.binraw_no_bit", "BinRAW needs a 'Bit:' field")
    else:
        bv, bl = fields["Bit"]
        if not bv.isdigit():
            rep.error(bl, "sub.binraw_bit_nan", "Bit must be an integer")
        elif int(bv) > 4096:
            rep.error(bl, "sub.binraw_bit_max", "BinRAW Bit exceeds the 4096 maximum")
    if "TE" not in fields:
        rep.warn(0, "sub.binraw_no_te", "BinRAW usually needs a 'TE:' field")
    if "Data_RAW" not in fields:
        rep.error(0, "sub.binraw_no_data", "BinRAW needs 'Bit_RAW:'/'Data_RAW:' blocks")
    elif not is_hex_bytes(fields["Data_RAW"][0]):
        rep.error(fields["Data_RAW"][1], "sub.binraw_hex",
                  "Data_RAW must be space-separated hex bytes")
    if "RAW_Data" in fields:
        rep.error(fields["RAW_Data"][1], "sub.binraw_rawdata",
                  "BinRAW files use Data_RAW, not RAW_Data")


def _check_parsed_sub(proto, fields, rep, firmware):
    known = proto in SUB_PROTOCOLS_OFW
    # A Key field, if present, is always exactly 8 bytes on any protocol that uses one.
    if "Key" in fields:
        kv, kl = fields["Key"]
        if not is_hex_bytes(kv, count=8):
            rep.error(kl, "sub.parsed_key_len",
                      "Key must be exactly 8 hex bytes (got %r)" % kv)
    if not known:
        # Fork/extra protocols (weather, TPMS, pagers...) have their own schemas and
        # frequently don't use the generic Bit+Key pair, so don't invent required
        # fields for them - only the universal checks (frequency, preset, syntax) apply.
        if firmware == "ofw":
            rep.error(fields["Protocol"][1], "sub.proto_unknown_ofw",
                      "%r is not a stock-firmware protocol (needs Unleashed/Momentum)"
                      % proto)
        else:
            rep.info(fields["Protocol"][1], "sub.proto_fork",
                     "%r is a fork protocol (not in stock OFW); structural checks only"
                     % proto)
        return
    if "Bit" not in fields:
        rep.error(0, "sub.parsed_no_bit", "protocol %r needs a 'Bit:' field" % proto)
    elif not fields["Bit"][0].isdigit():
        rep.error(fields["Bit"][1], "sub.parsed_bit_nan", "Bit must be an integer")
    if "Key" not in fields:
        rep.error(0, "sub.parsed_no_key", "protocol %r needs a 'Key:' field" % proto)
    if proto in SUB_PROTO_NEEDS_TE and "TE" not in fields:
        rep.warn(0, "sub.parsed_no_te", "protocol %r usually needs a 'TE:' field" % proto)
    if proto in SUB_PROTO_NEEDS_MANUFACTURE and "Manufacture" not in fields:
        rep.warn(0, "sub.parsed_no_manufacture",
                 "protocol %r needs a 'Manufacture:' field to decode" % proto)
    if proto == "Security+ 2.0" and "Secplus_packet_1" not in fields:
        rep.warn(0, "sub.parsed_no_secplus", "Security+ 2.0 needs 'Secplus_packet_1:'")


def _check_preset_protocol_consistency(preset, proto, fields, rep):
    ook_protocols = {"Princeton", "CAME", "KeeLoq", "Nice FLO", "Holtek", "Ansonic",
                     "Linear", "SMC5326", "Holtek_HT12X", "Princeton"}
    if proto in ook_protocols and preset and preset.startswith(
            "FuriHalSubGhzPreset2FSK"):
        rep.info(fields.get("Preset", ("", 0))[1], "sub.preset_mismatch",
                 "%s is an OOK protocol but the preset is 2-FSK; unusual pairing"
                 % proto)


# ---------------------------------------------------------------------------
# .ir linter
# ---------------------------------------------------------------------------

def lint_ir(lines, meta, rep, firmware):
    rep.checked_as = "ir"
    header = {}
    body_start = 0
    for idx, ln in enumerate(lines, 1):
        s = ln.strip()
        if not s or s.startswith("#"):
            body_start = idx
            break
        key, val = split_field(ln)
        if key:
            header[key] = (val, idx)
        body_start = idx

    if "Filetype" not in header:
        rep.error(1, "ir.no_filetype", "missing 'Filetype:' header")
    elif header["Filetype"][0] not in IR_FILETYPES:
        rep.warn(header["Filetype"][1], "ir.filetype",
                 "unusual Filetype %r" % header["Filetype"][0])
    if "Version" not in header:
        rep.error(1, "ir.no_version", "missing 'Version:' header")

    # split remaining lines into '#'-delimited signal blocks
    blocks = []
    cur = []
    cur_start = None
    for idx, ln in enumerate(lines, 1):
        s = ln.strip()
        if s.startswith("Filetype:") or s.startswith("Version:"):
            continue
        if s == "#" or (s.startswith("#") and cur):
            if cur:
                blocks.append((cur_start, cur))
            cur, cur_start = [], None
            continue
        if s.startswith("#") or not s:
            continue
        if cur_start is None:
            cur_start = idx
        cur.append((idx, s))
    if cur:
        blocks.append((cur_start, cur))

    for start, block in blocks:
        _check_ir_block(start, block, rep, firmware)


def _check_ir_block(start, block, rep, firmware):
    # Note: repeated signal names are normal and expected in .ir library files
    # (a universal remote lists many "POWER" entries for different brands), so
    # duplicate names are deliberately not flagged.
    fields = {}
    for idx, s in block:
        key, val = split_field(s)
        if key:
            fields[key] = (val, idx)
    name = fields.get("name", (None, start))[0]
    if name is None:
        rep.error(start, "ir.no_name", "signal has no 'name:' field")
    typ = fields.get("type", (None, start))[0]
    if typ is None:
        rep.error(start, "ir.no_type", "signal has no 'type:' field")
        return
    if typ not in ("parsed", "raw"):
        rep.error(fields["type"][1], "ir.type", "type must be 'parsed' or 'raw'")
        return
    if typ == "parsed":
        proto = fields.get("protocol", (None, start))[0]
        if proto is None:
            rep.error(start, "ir.no_protocol", "parsed signal needs 'protocol:'")
        elif proto not in IR_PROTOCOLS_OFW and firmware == "ofw":
            rep.warn(fields["protocol"][1], "ir.protocol",
                     "unknown IR protocol %r" % proto)
        for f in ("address", "command"):
            if f not in fields:
                rep.error(start, "ir.no_" + f, "parsed signal needs '%s:'" % f)
            elif not is_hex_bytes(fields[f][0], count=4):
                rep.error(fields[f][1], "ir." + f + "_len",
                          "%s must be exactly 4 hex bytes (got %r)"
                          % (f, fields[f][0]))
    else:  # raw
        if "frequency" not in fields:
            rep.error(start, "ir.no_frequency", "raw signal needs 'frequency:'")
        elif fields["frequency"][0].isdigit():
            fr = int(fields["frequency"][0])
            if not (20_000 <= fr <= 60_000):
                rep.warn(fields["frequency"][1], "ir.frequency",
                         "carrier %d Hz is unusual for IR (typically ~38000)" % fr)
        if "duty_cycle" in fields:
            try:
                d = float(fields["duty_cycle"][0])
                if not (0.0 < d < 1.0):
                    rep.warn(fields["duty_cycle"][1], "ir.duty",
                             "duty_cycle %s is outside 0..1" % fields["duty_cycle"][0])
            except ValueError:
                rep.error(fields["duty_cycle"][1], "ir.duty_nan",
                          "duty_cycle must be a number")
        data = fields.get("data")
        if data is None:
            rep.error(start, "ir.no_data", "raw signal needs a 'data:' field")
        else:
            toks = data[0].split()
            if not all(t.lstrip("-").isdigit() for t in toks):
                rep.error(data[1], "ir.data_nan", "data must be integers")
            else:
                nums = [int(t) for t in toks]
                if len(nums) > IR_DATA_MAX:
                    rep.warn(data[1], "ir.data_len",
                             "%d timings exceed the %d maximum" % (len(nums), IR_DATA_MAX))
                if any(n <= 0 for n in nums):
                    rep.error(data[1], "ir.data_sign",
                              "raw IR timings must all be positive")


# ---------------------------------------------------------------------------
# RTTTL music linter
# ---------------------------------------------------------------------------

RTTTL_HEADER = re.compile(r"^\s*[^:]{1,40}:\s*(d=\d+\s*,\s*)?o=\d+\s*,\s*b=\d+\s*:", re.I)
RTTTL_NOTE = re.compile(r"^(1|2|4|8|16|32)?([a-gp])(#)?(\.)?([1-8])?(\.)?$", re.I)


def looks_like_rtttl(lines):
    for ln in lines:
        if ln.strip():
            return bool(RTTTL_HEADER.match(ln))
    return False


def lint_rtttl(lines, meta, rep, firmware):
    rep.checked_as = "rtttl"
    text = "\n".join(l for l in lines if l.strip())
    parts = text.split(":")
    if len(parts) < 3:
        rep.error(1, "rtttl.structure", "RTTTL needs 'name:defaults:notes'")
        return
    name, defaults, notes = parts[0], parts[1], ":".join(parts[2:])
    for token in defaults.split(","):
        token = token.strip()
        if not token:
            continue
        m = re.match(r"^([dob])\s*=\s*(\d+)$", token, re.I)
        if not m:
            rep.warn(1, "rtttl.default", "unrecognized default %r" % token)
            continue
        key, num = m.group(1).lower(), int(m.group(2))
        if key == "d" and num not in (1, 2, 4, 8, 16, 32):
            rep.warn(1, "rtttl.duration", "default duration d=%d is unusual" % num)
        if key == "o" and not (1 <= num <= 8):
            rep.warn(1, "rtttl.octave", "default octave o=%d is outside 1..8" % num)
    note_tokens = [n.strip() for n in notes.split(",") if n.strip()]
    if not note_tokens:
        rep.error(1, "rtttl.empty", "no notes after the defaults section")
    bad = [n for n in note_tokens if not RTTTL_NOTE.match(n)]
    for n in bad[:10]:
        rep.warn(1, "rtttl.note", "note %r does not parse" % n)
    if len(bad) > 10:
        rep.info(1, "rtttl.note_more", "... and %d more unparsable notes" % (len(bad) - 10))


# ---------------------------------------------------------------------------
# Playlist linter
# ---------------------------------------------------------------------------

def looks_like_playlist(lines):
    # A playlist is recognised by its first content line being a sub:/ir: entry;
    # individual malformed lines are then reported by the linter rather than
    # causing the whole file to be treated as "not a playlist".
    for ln in lines:
        s = ln.strip()
        if not s or s.startswith("#"):
            continue
        return bool(re.match(r"^(sub|ir):", s))
    return False


def lint_playlist(lines, meta, rep, firmware, root=None):
    rep.checked_as = "playlist"
    count = 0
    for idx, ln in enumerate(lines, 1):
        s = ln.strip()
        if not s or s.startswith("#"):
            continue
        m = re.match(r"^(sub|ir):\s*(\S.*)$", s)
        if not m:
            rep.warn(idx, "playlist.line", "expected 'sub: <path>' or a comment")
            continue
        count += 1
        kind, path = m.group(1), m.group(2).strip()
        ext = ".sub" if kind == "sub" else ".ir"
        if not path.lower().endswith(ext):
            rep.warn(idx, "playlist.ext", "%s entry does not point at a %s file"
                     % (kind, ext))
        if root:
            local = os.path.join(root, path.lstrip("/"))
            if not os.path.exists(local):
                rep.warn(idx, "playlist.missing", "referenced file not found: %s" % path)
    if count == 0:
        rep.warn(0, "playlist.empty", "playlist has no entries")


# ---------------------------------------------------------------------------
# Dispatch
# ---------------------------------------------------------------------------

def detect_type(path, lines):
    ext = os.path.splitext(path)[1].lower()
    if ext == ".sub":
        return "sub"
    if ext == ".ir":
        return "ir"
    first = next((l.strip() for l in lines if l.strip()), "")
    if first.startswith("Filetype:"):
        ft = first.split(":", 1)[1].strip()
        if ft in SUB_FILETYPES:
            return "sub"
        if ft in IR_FILETYPES:
            return "ir"
    if looks_like_playlist(lines):
        return "playlist"
    if looks_like_rtttl(lines):
        return "rtttl"
    return None


LINTERS = {
    "sub": lint_sub,
    "ir": lint_ir,
    "rtttl": lint_rtttl,
    "playlist": lint_playlist,
}


def check_file(path, forced_type, firmware, root):
    rep = Report(path)
    try:
        lines, meta = read_text(path)
    except OSError as exc:
        rep.error(0, "io", "cannot read file: %s" % exc)
        return rep
    ftype = forced_type or detect_type(path, lines)
    if ftype is None:
        rep.info(0, "skip", "unrecognized file type (use --type to force)")
        return rep
    tws = trailing_ws_lines(lines)
    if tws:
        rep.info(tws[0], "style.trailing_ws",
                 "%d line(s) have trailing whitespace (fix with --fix)" % len(tws))
    if meta["crlf"] and meta["lf"]:
        rep.warn(0, "style.mixed_eol", "mixed CRLF and LF line endings")
    if lines and not meta["final_newline"]:
        rep.info(0, "style.final_newline", "no newline at end of file")
    linter = LINTERS[ftype]
    if ftype == "playlist":
        linter(lines, meta, rep, firmware, root=root)
    else:
        linter(lines, meta, rep, firmware)
    return rep


# ---------------------------------------------------------------------------
# --fix (trivial, safe rewrites only)
# ---------------------------------------------------------------------------

def fix_file(path):
    with open(path, "rb") as fh:
        raw = fh.read()
    text = raw.decode("utf-8", errors="replace")
    fixed = "\n".join(line.rstrip() for line in text.splitlines())
    if fixed:
        fixed += "\n"
    new = fixed.encode("utf-8")
    if new != raw:
        with open(path, "wb") as fh:
            fh.write(new)
        return True
    return False


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def walk_paths(paths):
    exts = {".sub", ".ir", ".txt"}
    for p in paths:
        if os.path.isdir(p):
            for dirpath, _, names in os.walk(p):
                for n in sorted(names):
                    if os.path.splitext(n)[1].lower() in exts:
                        yield os.path.join(dirpath, n)
        else:
            yield p


def main(argv=None):
    ap = argparse.ArgumentParser(
        prog="flippercheck",
        description="Validate Flipper Zero .sub / .ir / RTTTL / playlist files.")
    ap.add_argument("paths", nargs="+", help="files or folders to check")
    ap.add_argument("--type", choices=sorted(LINTERS), help="force a file type")
    ap.add_argument("--firmware", choices=("ofw", "extended"), default="extended",
                    help="ofw = stock only; extended = allow fork protocols (default)")
    ap.add_argument("--root", help="base path for playlist file-existence checks")
    ap.add_argument("--fix", action="store_true",
                    help="rewrite files to strip trailing spaces / CRLF / fix newline")
    ap.add_argument("--json", action="store_true", help="machine-readable output")
    ap.add_argument("--quiet", action="store_true", help="hide files with no findings")
    ap.add_argument("--strict", action="store_true", help="warnings fail too")
    ap.add_argument("--version", action="version", version="flippercheck " + __version__)
    args = ap.parse_args(argv)

    files = list(walk_paths(args.paths))
    if args.fix:
        changed = [p for p in files if fix_file(p)]
        if not args.json:
            print("fixed %d file(s)" % len(changed))
            for c in changed:
                print("  " + c)
        return 0

    reports = [check_file(p, args.type, args.firmware, args.root) for p in files]
    worst = max((r.worst() for r in reports), default=-1)

    if args.json:
        out = [{"file": r.path, "type": r.checked_as,
                "findings": [f.as_dict() for f in r.findings]} for r in reports]
        print(json.dumps(out, indent=2))
    else:
        _print_human(reports, args.quiet)

    if worst >= _LEVEL_RANK[ERROR]:
        return 2
    if worst >= _LEVEL_RANK[WARN] and args.strict:
        return 1
    return 0


def _print_human(reports, quiet):
    marks = {ERROR: "ERROR", WARN: "warn ", INFO: "info "}
    total = {ERROR: 0, WARN: 0, INFO: 0}
    for r in reports:
        real = [f for f in r.findings if f.level != INFO]
        if quiet and not real:
            continue
        if not r.findings:
            print("ok   %s" % r.path)
            continue
        print("%s  (%s)" % (r.path, r.checked_as or "?"))
        for f in r.findings:
            total[f.level] += 1
            loc = ("line %d" % f.line) if f.line else "-"
            print("  %s %-8s %s: %s" % (marks[f.level], loc, f.code, f.msg))
    if not quiet or any(total.values()):
        print("\n%d error(s), %d warning(s), %d note(s) across %d file(s)"
              % (total[ERROR], total[WARN], total[INFO], len(reports)))


if __name__ == "__main__":
    sys.exit(main())
