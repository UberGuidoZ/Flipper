#!/usr/bin/python3

filename = "ZJ-68-pager_{:02X}.sub"

template = """Filetype: Flipper SubGhz Key File
Version: 1
Frequency: 433920000
Preset: FuriHalSubGhzPresetOok650Async
Protocol: Princeton
Bit: 24
Key: 00 00 00 00 00 E9 C4 {:02X}
TE: 325"""

playlist = "sub: /ext/subghz/Restaurant_Pagers/iBells_ZJ-68/ZJ-68-pager_{:02X}.sub\n"
playlist_file = open("ibells-pager_zj-68_playlist.txt", "w+")

for i in range(0, 256):
    playlist_file.write(playlist.format(i))
    sub_file = open(filename.format(i), "w+")
    sub_file.write(template.format(i))
    sub_file.close()

playlist_file.close()
