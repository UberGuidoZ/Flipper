import os
import math


# Protocol settings: https://phreakerclub.com/447
class Protocol:
    """
    Generic OOK communication protocol:
        - name: the protocol's name
        - n_bits: number of bits composing a key
        - transposition_table: how 1s and 0s are translated to .sub format
        - pilot_period: preamble PREPENDED to each key in sub format, empty by default
        - stop_bit: APPENDED to each key in sub format, empty by default
        - frequency: protocol's frequency in Hz, 433920000 by default
        - repetition: number of times to repeat each key, 3 by default
        - key_range: range of keys to generate, None by default
    """

    def __init__(
        self,
        name,
        n_bits,
        transposition_table,
        pilot_period="",
        stop_bit="",
        frequency=433920000,
        repetition=3,
        key_range=None,
    ):
        self.name = name
        self.n_bits = n_bits
        self.transposition_table = transposition_table
        self.pilot_period = pilot_period
        self.stop_bit = stop_bit
        self.repetition = repetition
        self.key_range = key_range
        self.file_header = (
            "Filetype: Flipper SubGhz RAW File\n"
            + "Version: 1\n"
            + f"Frequency: {frequency}\n"
            + "Preset: FuriHalSubGhzPresetOok650Async\n"
            + "Protocol: RAW\n"
        )

    def key_to_sub(self, key):
        bin_str = f"{key:0{self.n_bits}b}"
        return self.key_bin_str_to_sub(bin_str)

    def key_bin_str_to_sub(self, bin_str, debruijn=False):
        sub = ""
        if not debruijn:
            sub = self.pilot_period
        line_len = 0  # keep lines under 2500 chars
        for bit in bin_str:
            if line_len > 2500:
                sub += "\nRAW_Data: "
                line_len = 0
            sub += self.transposition_table[bit]
            line_len += len(self.transposition_table[bit])
        if not debruijn:
            sub += self.stop_bit
        return sub

    def de_bruijn(self):
        """
        de Bruijn binary sequence
        credit: https://www.rosettacode.org/wiki/De_Bruijn_sequences#Python
        """
        alphabet = "01"
        k = 2
        a = [0] * k * (self.n_bits if self.pilot_period == "" else self.n_bits + 1)
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
        return self.key_bin_str_to_sub(db_seq, True)

    def generate_sub_files(self, n_folders=6):
        """
        Generate sub files grouped by split nuber of keys
        Directory structure:
        - sub_files
            - protocol_name
                - split_factor
                    - split_factor_id.sub

        n_folder: number of folders to create,
                  folder [1..n_folders] will contain [2^1..2^n_folders] files,
                  each file containing [2^n_bits/2^1..2^n_bits/2^n_folders] keys.
        """
        if (
            self.n_bits > 12 and self.key_range is None
        ):  # take up too much space for github
            print(f"Skipping {self.name}, takes up too much space for github")
            return
        base_dir = f"sub_files/{self.name}"
        os.makedirs(base_dir, exist_ok=True)
        # If key_range is defined, generate those keys only
        if self.key_range is not None:
            filename = f"{base_dir}/bf_{self.key_range[0]}-{self.key_range[-1]}.sub"
            with open(filename, "w") as f:
                f.write(self.file_header)
                for key in self.key_range:
                    f.write(
                        "RAW_Data: " + self.key_to_sub(key) * self.repetition + "\n"
                    )
            return
        # Create debruijn.sub
        filename = f"{base_dir}/debruijn.sub"
        with open(filename, "w") as f:
            f.write(self.file_header)
            f.write("RAW_Data: " + self.de_bruijn() + "\n")
        # Generate sets of 2^0, 2^1, .., 2^n_folders .sub files
        splits = [
            int(pow(2, self.n_bits) / _) for _ in [pow(2, _) for _ in range(n_folders)]
        ]
        [os.makedirs(f"{base_dir}/{split}", exist_ok=True) for split in splits]
        split_files = [None] * len(splits)  # current file per split
        for key in range(pow(2, self.n_bits)):
            for idx, split in enumerate(splits):
                if key % split == 0:
                    # close previous file if open
                    if split_files[idx] is not None:
                        split_files[idx].close()
                    filename = f"{base_dir}/{split}/{math.floor(key/(split*2)):03d}_{key/split:03.0f}.sub"
                    split_files[idx] = open(filename, "w")
                    split_files[idx].write(self.file_header)
                split_files[idx].write(
                    "RAW_Data: " + self.key_to_sub(key) * self.repetition + "\n"
                )
        # close all files
        [f.close() for f in split_files if f is not None]


protocols = [
    Protocol(
        name="CAME-12bit-433",
        n_bits=12,
        transposition_table={"0": "-320 640 ", "1": "-640 320 "},
        pilot_period="-11520 320 ",
    ),
    Protocol(
        name="CAME-12bit-433-fast",
        n_bits=12,
        transposition_table={"0": "-250 500 ", "1": "-500 250 "},
        pilot_period="-9000 250 ",
    ),
    Protocol(
        name="CAME-12bit-868",
        n_bits=12,
        transposition_table={"0": "-320 640 ", "1": "-640 320 "},
        pilot_period="-11520 320 ",
        frequency=868350000,
    ),
    Protocol(
        name="CAME-12bit-868-fast",
        n_bits=12,
        transposition_table={"0": "-250 500 ", "1": "-500 250 "},
        pilot_period="-9000 250 ",
        frequency=868350000,
    ),
    Protocol(
        name="Chamberlain-9bit-315",
        n_bits=10,
        transposition_table={"0": "-1000 3000 ", "1": "-2000 2000 "},
        pilot_period="-39000 1000 ",
        stop_bit="-3000 1000 ",
        frequency=315000000,
    ),
    Protocol(
        name="Chamberlain-9bit-390",
        n_bits=10,
        transposition_table={"0": "-1000 3000 ", "1": "-2000 2000 "},
        pilot_period="-39000 1000 ",
        stop_bit="-3000 1000 ",
        frequency=390000000,
    ),
    Protocol(
        name="Linear-10bit-300",
        n_bits=10,
        transposition_table={"0": "500 -1500 ", "1": "1500 -500 "},
        stop_bit="1 -21500 ",
        frequency=300000000,
        repetition=5,
    ),
    Protocol(
        name="Linear-10bit-310",
        n_bits=10,
        transposition_table={"0": "500 -1500 ", "1": "1500 -500 "},
        stop_bit="1 -21500 ",
        frequency=310000000,
        repetition=5,
    ),
    Protocol(
        name="NICE-12bit-433",
        n_bits=12,
        transposition_table={"0": "-700 1400 ", "1": "-1400 700 "},
        pilot_period="-25200 700 ",
    ),
    Protocol(
        name="NICE-12bit-868",
        n_bits=12,
        transposition_table={"0": "-700 1400 ", "1": "-1400 700 "},
        pilot_period="-25200 700 ",
        frequency=868350000,
    ),
    Protocol(
        name="PT-2240-433",
        n_bits=24,
        transposition_table={"0": "450 -1350 ", "1": "1350 -450 "},
        pilot_period="450 -13950 ",
    ),
    Protocol(
        name="Spacca_pager-433",
        n_bits=24,
        transposition_table={"0": "320 -960 ", "1": "960 -320 "},
        pilot_period="320 -9920 ",
        frequency=433650000,
        key_range=range(0x11A01C, 0x11A0E4),  # 300 keys
    ),
    Protocol(
        name="Ansonic-434",
        n_bits=12,
        transposition_table={"0": "-1111 555 ", "1": "-555 1111 "},
        frequency=434075000,
        pilot_period="-19425 555 ",
    ),
    Protocol(
        name="Holtek-315",
        n_bits=12,
        transposition_table={"0": "-870 430 ", "1": "-430 870 "},
        frequency=315000000,
        pilot_period="-15480 430 ",
    ),
    Protocol(
        name="Holtek-433",
        n_bits=12,
        transposition_table={"0": "-870 430 ", "1": "-430 870 "},
        pilot_period="-15480 430 ",
    ),
    Protocol(
        name="Holtek-868",
        n_bits=12,
        transposition_table={"0": "-870 430 ", "1": "-430 870 "},
        frequency=868350000,
        pilot_period="-15480 430 ",
    ),
    Protocol(
        name="Holtek-915",
        n_bits=12,
        transposition_table={"0": "-870 430 ", "1": "-430 870 "},
        frequency=915000000,
        pilot_period="-15480 430 ",
    ),
]

for p in protocols:
    # TODO multithread this
    p.generate_sub_files()
    print(f"{p.name} done")
