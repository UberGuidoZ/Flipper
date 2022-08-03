import os

# Script settings:
REPETITION = 4  # number of tx per key


# Protocol settings: https://phreakerclub.com/447
class Protocol:
    def __init__(
        self, name, n_bits, transposition_table, pilot_period=None, frequency=433920000
    ):
        self.name = name
        self.n_bits = n_bits
        self.transposition_table = transposition_table
        self.pilot_period = pilot_period
        self.file_header = (
            "Filetype: Flipper SubGhz RAW File\n"
            + "Version: 1\n"
            + f"Frequency: {frequency}\n"
            + "Preset: FuriHalSubGhzPresetOok650Async\n"
            + "Protocol: RAW\n"
        )

    def key_dec_to_str(self, key_dec):
        key_bin = f"{key_dec:012b}"  # format as 12 digits bin
        return self.key_bin_to_str(key_bin)

    def key_bin_to_str(self, key_bin):
        key_str = self.pilot_period if self.pilot_period else ""
        line_len = 0  # keep lines under 2500 chars
        for bit in key_bin:
            if line_len > 2500:
                key_str += "\nRAW_Data: "
                line_len = 0
            key_str += self.transposition_table[bit]
            line_len += len(self.transposition_table[bit])
        return key_str

    def de_bruijn(self):
        """
        de Bruijn binary sequence
        credit: https://www.rosettacode.org/wiki/De_Bruijn_sequences#Python
        """
        alphabet = "01"
        k = 2
        a = [0] * k * self.n_bits
        sequence = []

        def db(t, p):
            if t > self.n_bits:
                if self.n_bits % p == 0:
                    sequence.extend(a[1 : p + 1])
            else:
                a[t] = a[t - p]
                db(t + 1, p)
                for j in range(a[t - p] + 1, k):
                    a[t] = j
                    db(t + 1, t)

        db(1, 1)
        db_seq = "".join(alphabet[i] for i in sequence)
        return self.key_bin_to_str(db_seq)

    def generate_sub_files(self):
        """
        Generate sub files grouped by split nuber of keys
        Directory structure:
        - sub_files
            - protocol_name
                - split_factor
                    - split_factor_id.sub
        """
        base_dir = f"sub_files/{self.name}"
        os.makedirs(base_dir, exist_ok=True)
        # Create debruijn.sub
        filename = f"{base_dir}/debruijn.sub"
        with open(filename, "w") as f:
            f.write(self.file_header)
        with open(filename, "a") as f:
            f.write("RAW_Data: " + self.de_bruijn() + "\n")
        if self.n_bits > 12:  # take up too much space, try de bruijn instead
            return
        # Generate sets of 1, 2, 4, 8, 16, 32 .sub files
        splits = [int(pow(2, self.n_bits) / _) for _ in [pow(2, _) for _ in range(6)]]
        for split in splits:
            split_dir = f"{base_dir}/{split}"
            os.makedirs(split_dir, exist_ok=True)

            for key_dec in range(pow(2, self.n_bits)):
                key_str = self.key_dec_to_str(key_dec) * REPETITION

                if (key_dec % split) == 0:
                    filename = f"{split_dir}/{key_dec / split:03.0f}.sub"
                    with open(filename, "w") as f:
                        f.write(self.file_header)

                with open(filename, "a") as f:
                    f.write("RAW_Data: " + key_str + "\n")


protocols = [
    Protocol("CAME", 12, {"0": "-320 640 ", "1": "-640 320 "}, "-11520 320 "),
    Protocol("NICE", 12, {"0": "-700 1400 ", "1": "-1400 700 "}, "-25200 700 "),
    # 24 bits take up too much space to upload to github as over 100MB
    Protocol("PT-2240", 24, {"0": "450 -1350 ", "1": "1350 -450 "}, "450 -13950 "),
    Protocol("PT-2262", 24, {"0": "450 -1350 ", "1": "1350 -450 "}, "450 -13950 "),
    Protocol("8bit", 8, {"0": "200 -400 ", "1": "400 -200 "}),  # generic 8 bit protocol
]

for p in protocols:
    p.generate_sub_files()
    print(f"{p.name} done")
