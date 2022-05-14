Some files that are fun to use. Starting to get a better idea on the structure (Flipper format) of NFC files and payloads.

Example using the [RickRoll.nfc](https://github.com/UberGuidoZ/Flipper/blob/main/NFC/Fun_Files/RickRoll.nfc) file. Leave the data in pages 1 through 6 alone (though not always true, but it is for YouTube links.)

Page 7: 79 6F 75 74 <br>
Page 8: 75 2E 62 65 <br>
Page 9: 2F 64 51 77 <br>
Page 10: 34 77 39 57 <br>
Page 11: 67 58 63 51 <br>

This is simply the YouTube "share" link encoded into HEX and split into 4 byte chunks. See for yourself...

HEX from above: 79 6F 75 74 75 2E 62 65 2F 64 51 77 34 77 39 57 67 58 63 51 <br>
[Converted](https://www.binaryhexconverter.com/hex-to-ascii-text-converter): youtu.be/dQw4w9WgXcQ

The last byte in page 6 (0x04) defines what type of encoding ([data sheet](https://www.nxp.com/docs/en/data-sheet/NTAG213_215_216.pdf) and [URI identifier codes](https://learn.adafruit.com/adafruit-pn532-rfid-nfc/ndef)). To convert your link to HEX, use anything such as an [online tool](https://onlinehextools.com/convert-ascii-to-hex).

One limitation is the URL will be truncated if it goes beyond page 11! If your URL is less than exact, pad it with 00, making sure page 12 stays as "FE 00 00 00". Note that [TinyURL](https://tinyurl.com/app) links appear to fit well and conversion/use is free.
