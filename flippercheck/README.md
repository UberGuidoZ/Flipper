# flippercheck

A static checker for Flipper Zero data files. Point it at a `.sub`, `.ir`, RTTTL
music file, or a Sub-GHz playlist and it tells you what's wrong before you copy it
onto the SD card and wonder why it won't send.

It only reads files. It never opens the radio and never transmits, so you don't need
a Flipper plugged in to run it. Pure Python 3, standard library only, one file.

## Why

This repo has thousands of hand-edited and tool-generated files, and files rot
quietly: a key that's the wrong length, a RAW capture with a stray line in the middle,
a frequency the CC1101 can't actually tune, a pause so long it overflows the transmit
timer. None of that shows up until you try to send it. There was already an IR-only
linter (`darmiel/fff-ir-lint`), but nothing checked `.sub` files at all, and it hasn't
been touched since 2024. This covers `.sub`, `.ir`, RTTTL, and playlists in one tool.

The rules come from the Flipper firmware source and the CC1101 datasheet, then checked
against the real files in this repo. Where the firmware tolerates something the docs
forbid, the checker follows the firmware. For example the docs say a RAW capture must
start on a positive value, but plenty of good files here start negative and play fine,
so that isn't treated as an error. The goal is to catch real breakage without crying
wolf on the files people actually use.

## Use

```sh
python3 flippercheck.py path/to/file.sub          # one file
python3 flippercheck.py Sub-GHz/                   # a whole folder (walks it)
python3 flippercheck.py --quiet Sub-GHz/ Infrared/ # only show files with findings
python3 flippercheck.py --json Sub-GHz/ > out.json # machine-readable
python3 flippercheck.py --fix path/to/file.sub     # tidy trailing spaces / CRLF / newline
```

No install needed. If you want it on your PATH, drop `flippercheck.py` somewhere and
`chmod +x` it.

Findings come at three levels:

- **error** - the file is broken and will misbehave on a real Flipper.
- **warning** - probably wrong, worth a look.
- **note** - style or "unusual but fine" (trailing whitespace, a non-alternating RAW value).

Exit code is `0` when clean, `1` when there are only warnings, `2` when there are errors,
so it drops into a script or a git hook. `--strict` makes warnings fail too.

### Firmware target

Custom firmware (Unleashed, Momentum, RogueMaster) adds protocols and opens up
frequencies that stock firmware doesn't have. By default the checker is permissive and
accepts those. Pass `--firmware ofw` to hold a file to stock firmware only, which is
useful if you're sharing something that has to work on an unmodified Flipper.

## What it checks

**.sub (Sub-GHz)**
- `Filetype` / `Version` / `Frequency` / `Preset` / `Protocol` present and sane.
- Frequency inside one of the CC1101's three bands (300-348 / 387-464 / 779-928 MHz) -
  anything else can't be tuned on real hardware.
- `Preset` is one of the six the firmware actually resolves (the internal-only
  MSK/GFSK/IDLE presets are rejected), and custom preset register blocks are
  structurally valid (register pairs, the `00 00` terminator, the 8-byte power table,
  no `0x00` register address that would truncate the block).
- RAW data: no zero values (they stop transmission), no value past the 30-bit transmit
  limit (~17m54s, which silently corrupts on air), and RAW data must not resume after a
  stray non-RAW line.
- Parsed protocols: `Key` is exactly 8 bytes, `Bit` is present, and the protocol name is
  known. KeeLoq/Star Line want `Manufacture`, Princeton wants `TE`, and so on.

**.ir (Infrared)**
- Header, and every signal has a `name` and a `type` of `parsed` or `raw`.
- Parsed: known protocol, `address`/`command` exactly 4 bytes.
- Raw: sane carrier frequency, `duty_cycle` between 0 and 1, positive timings, at most
  1024 of them.

**RTTTL music and playlists**
- RTTTL header defaults and note tokens parse.
- Playlist lines are `sub:`/`ir:` paths pointing at the right file type (with `--root`
  it can also check the files exist).

## What it found here

Run against the repo's own `.sub` files, it flagged a small number of genuinely broken
ones, including:

- `Sub-GHz/Sleep_Files/` - the sleep files encode a single multi-hour pause as one RAW
  value (around -2,000,000,000 us), which is past the firmware's 30-bit transmit limit
  and gets bit-truncated into a much shorter, wrong delay. That lines up with the
  long-standing "sleep files don't delay" reports.
- A couple of LRS restaurant-pager files set to 467.75 MHz, which is above the CC1101's
  top band - a real Flipper can't tune there.
- A few files with all the spaces stripped out (`Key:0000000000A39AA4`), so the key no
  longer parses as bytes.
- RAW files with a field stranded in the middle of the data.

## Tests

```sh
python3 tests/test_flippercheck.py
```

The fixtures under `tests/fixtures/good` include real files from this repo, and every
one of them has to come back clean - that's the guardrail against the checker getting
too strict and flagging files that are actually fine.

## Notes

The firmware facts (frequency edges, preset names, the 30-bit timer, protocol list) are
from a mid-2026 build of the firmware. They're stable in practice but not guaranteed
across versions, so if something looks off on a much newer firmware, that's the first
place to check.
