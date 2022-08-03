# Script settings:
split = 100  # number of keys per file
file_header = """
Filetype: Flipper SubGhz RAW File
Version: 1
Frequency: 433920000
Preset: FuriHalSubGhzPresetOok650Async
Protocol: RAW
"""

# Protocol settings: https://phreakerclub.com/447
n_bits = 12
combos = range(0, pow(2, n_bits))
repetition = 3
transposition_table = {
    "0": "-320 640 ",
    "1": "-640 320 ",
}
pilot_period = "-11520 320 "

for key_dec in combos:
    key_bin = f"{key_dec:012b}"  # format as 12 digits bin
    key_str = pilot_period
    for bit in key_bin:
        key_str += transposition_table[bit]
    joined = "".join(key_str)
    key_str = key_str * repetition

    if (key_dec % split) == 0:
        filename = f"CAME_{split}_{int(key_dec / split)}.sub"
        with open(filename, "w") as f:
            f.write(file_header)

    with open(filename, "a") as f:
        f.write("RAW_Data: " + key_str + "\n")
