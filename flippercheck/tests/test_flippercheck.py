"""Tests for flippercheck. Run with: python3 -m unittest -v  (from this folder)
or: python3 tests/test_flippercheck.py

Pure stdlib, no pytest needed. Fixtures live in tests/fixtures/{good,bad}. The good
set includes real files pulled from the repo; every one must come back clean.
"""

import os
import sys
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(HERE))
import flippercheck as fc  # noqa: E402

GOOD = os.path.join(HERE, "fixtures", "good")
BAD = os.path.join(HERE, "fixtures", "bad")


def codes(path, firmware="extended", ftype=None):
    rep = fc.check_file(path, ftype, firmware, None)
    return [f.code for f in rep.findings]


def levels(path, firmware="extended", ftype=None):
    rep = fc.check_file(path, ftype, firmware, None)
    return {f.code: f.level for f in rep.findings}


def errors(path, firmware="extended", ftype=None):
    rep = fc.check_file(path, ftype, firmware, None)
    return [f.code for f in rep.findings if f.level == fc.ERROR]


class GoodFiles(unittest.TestCase):
    def test_every_good_fixture_has_no_errors(self):
        for name in sorted(os.listdir(GOOD)):
            path = os.path.join(GOOD, name)
            with self.subTest(file=name):
                self.assertEqual(errors(path), [],
                                 "%s should have no errors" % name)

    def test_parsed_sub_clean(self):
        self.assertEqual(errors(os.path.join(GOOD, "Doorbell0.sub")), [])

    def test_raw_sub_starting_negative_is_clean(self):
        # Real repo files start RAW_Data on a negative value; must not be rejected.
        self.assertEqual(errors(os.path.join(GOOD, "raw_negative_start.sub")), [])

    def test_custom_preset_clean(self):
        self.assertEqual(errors(os.path.join(GOOD, "custom_preset.sub")), [])

    def test_ir_library_variant_filetype_accepted(self):
        # ir_library.ir uses "IR library file", not the firmware's "IR signals file",
        # and repeats the name "POWER" - both are fine and must not error.
        self.assertEqual(errors(os.path.join(GOOD, "ir_library.ir")), [])


class SubErrors(unittest.TestCase):
    def test_frequency_out_of_band(self):
        self.assertIn("sub.freq_band", errors(os.path.join(BAD, "freq_out_of_band.sub")))

    def test_internal_only_preset_rejected(self):
        self.assertIn("sub.preset_internal",
                      errors(os.path.join(BAD, "preset_internal.sub")))

    def test_key_wrong_length(self):
        self.assertIn("sub.parsed_key_len",
                      errors(os.path.join(BAD, "key_too_short.sub")))

    def test_raw_zero_value(self):
        self.assertIn("sub.raw_zero", errors(os.path.join(BAD, "raw_zero.sub")))

    def test_raw_overflow(self):
        self.assertIn("sub.raw_overflow", errors(os.path.join(BAD, "raw_overflow.sub")))

    def test_raw_interleave_when_data_resumes(self):
        self.assertIn("sub.raw_interleave",
                      errors(os.path.join(BAD, "raw_interleave.sub")))

    def test_custom_preset_broken_structure(self):
        errs = errors(os.path.join(BAD, "custom_zero_addr.sub"))
        self.assertTrue(any(c.startswith("sub.custom") for c in errs), errs)


class RawAlternationIsSoft(unittest.TestCase):
    def test_same_sign_is_info_not_error(self):
        lvls = levels(os.path.join(BAD, "raw_same_sign.sub"))
        self.assertEqual(lvls.get("sub.raw_nonalternating"), fc.INFO)
        self.assertNotIn("sub.raw_alternate", lvls)


class FirmwareTargets(unittest.TestCase):
    def test_fork_protocol_errors_in_ofw_mode(self):
        self.assertIn("sub.proto_unknown_ofw",
                      errors(os.path.join(BAD, "proto_unknown.sub"), firmware="ofw"))

    def test_fork_protocol_is_note_in_extended_mode(self):
        errs = errors(os.path.join(BAD, "proto_unknown.sub"), firmware="extended")
        self.assertNotIn("sub.proto_unknown_ofw", errs)

    def test_fork_protocol_does_not_require_bit_key(self):
        # A weather-station style protocol without Bit/Key must not error in extended.
        errs = errors(os.path.join(BAD, "proto_unknown.sub"), firmware="extended")
        self.assertNotIn("sub.parsed_no_key", errs)


class IrErrors(unittest.TestCase):
    def test_address_wrong_length(self):
        self.assertIn("ir.address_len", errors(os.path.join(BAD, "ir_addr_short.ir")))

    def test_bad_type(self):
        self.assertIn("ir.type", errors(os.path.join(BAD, "ir_bad_type.ir")))

    def test_raw_negative_timing(self):
        self.assertIn("ir.data_sign", errors(os.path.join(BAD, "ir_raw_negative.ir")))


class PlaylistAndMusic(unittest.TestCase):
    def test_playlist_bad_line(self):
        c = codes(os.path.join(BAD, "bad_playlist.txt"))
        self.assertTrue(any(x.startswith("playlist.") for x in c), c)

    def test_rtttl_bad_notes(self):
        c = codes(os.path.join(BAD, "bad_rtttl.txt"), ftype="rtttl")
        self.assertIn("rtttl.note", c)


class Detection(unittest.TestCase):
    def test_detects_sub_by_extension(self):
        lines, _ = fc.read_text(os.path.join(GOOD, "Doorbell0.sub"))
        self.assertEqual(fc.detect_type("x.sub", lines), "sub")

    def test_detects_playlist_by_content(self):
        lines = ["# c", "sub: /ext/a.sub"]
        self.assertEqual(fc.detect_type("x.txt", lines), "playlist")

    def test_unknown_txt_skipped(self):
        lines = ["just some notes", "nothing structured here"]
        self.assertIsNone(fc.detect_type("readme.txt", lines))


class Fixup(unittest.TestCase):
    def test_fix_strips_trailing_whitespace(self):
        import tempfile
        with tempfile.NamedTemporaryFile("w", suffix=".sub", delete=False) as fh:
            fh.write("Filetype: Flipper SubGhz Key File   \r\nVersion: 1")
            tmp = fh.name
        try:
            self.assertTrue(fc.fix_file(tmp))
            with open(tmp, "rb") as f:
                data = f.read()
            self.assertNotIn(b"   \n", data)
            self.assertNotIn(b"\r\n", data)
            self.assertTrue(data.endswith(b"\n"))
        finally:
            os.unlink(tmp)


if __name__ == "__main__":
    unittest.main(verbosity=2)
