import os

template1 = """Filetype: Flipper NFC device
Version: 2
# Nfc device type can be UID, Mifare Ultralight, Bank card
Device type: NTAG215
# UID, ATQA and SAK are common for all formats"""
template2 ="""ATQA: 44 00
SAK: 00
# Mifare Ultralight specific data
Signature: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Mifare version: 00 04 04 02 01 00 11 03
Counter 0: 0
Tearing 0: 00
Counter 1: 0
Tearing 1: 00
Counter 2: 0
Tearing 2: 00"""

def amiiDump(f):
    pagelist = []
    uidlist = []
    try:
        amiibin = open(f, "rb")
        pagenumber = 0
        while amiibin:
            # Read the bin 4 bytes at a time
            chunk = amiibin.read(4)
            if not chunk:
                break
            # Convert binary to non-ASCII hex and add Flipper Zero required formatting
            dirtypage = chunk.hex()
            cleanpage = ' '.join(dirtypage[i:i+2] for i in range(0, len(dirtypage), 2))
            completepage = "Page {0}: {1}".format(pagenumber,cleanpage.upper())
            # Place UID (first 8 bytes) into the uidlist
            if pagenumber <= 1:
                uidlist.append(cleanpage.upper())
            # Append the completed page to the pagelist
            pagelist.append(completepage)
            pagenumber += 1 
        amiibin.close()
        totalpages = "Pages total: {}".format(pagenumber)
        # UID is 7 bytes, remove last 3 characters from the string to match
        cleanuid = ' '.join(uidlist)[:-3]
        return(totalpages,pagelist,cleanuid)
    except IOError:
        print("Can't open file.")

def amiiCombine(totalpages,pagelist,cleanuid,f):
    #Now bring it all together and write to a Flipper Zero .nfc file
    newf = f[:-3] + "nfc"
    nfcfile = open(newf, "a+")
    nfcfile.write(template1)
    nfcfile.write("\n")
    nfcfile.write("UID: {}".format(cleanuid))
    nfcfile.write("\n")
    nfcfile.write(template2)
    nfcfile.write("\n")
    nfcfile.write(totalpages)
    nfcfile.write("\n")
    for page in pagelist:
        nfcfile.write(page)
        nfcfile.write("\n")
    nfcfile.close()

files = [f for f in os.listdir('.') if os.path.isfile(f)]
for f in files:
    if f.endswith(".bin"):
        totalpages,pagelist,cleanuid = amiiDump(f)
        amiiCombine(totalpages,pagelist,cleanuid,f)