import pandas as pd
split = 1000  # split files according to the keys count (each 1000 in one file)
case = 0

for x in range(0, 4096):  # 12bit = 4096 possibilities  
    binary = "{0:012b}".format(x)  # with leading zeros
    cmd = ['-15078 ', '321 ']
    for char in binary:
        if char == "0":
            cmd.append('-334 ')
            cmd.append('667 ')
        if char == "1":
            cmd.append('-664 ')
            cmd.append('343 ')
    joined = "".join(cmd)
    Multijoined = joined * 5  # number of repetition 
    command = 'RAW_Data: ' + Multijoined
    padding = "RAW_Data: -50000 50000 "
       
# split files according to the keys count (each 1000 in one file)

    if (x % split) == 0:  
        case += 1
        filecase = f'output{case}.sub'
        with open(filecase, 'w') as f:
            f.write("Filetype: Flipper SubGhz RAW File\nVersion: 1\nFrequency: 433920000\nPreset: FuriHalSubGhzPresetOok650Async\nProtocol: RAW\n")

# write keys to sub file
    def writing(raw, filename, pad):
      with open(filename, 'a') as f:
        f.write(raw)
        f.write('\n')
        f.write(pad)
        f.write('\n')

    writing(command, filecase, padding)
