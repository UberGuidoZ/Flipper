Combined the stock Unleased firmware [mf_classic_dict.nfc](https://github.com/Eng1n33r/flipperzero-firmware/blob/dev/assets/resources/nfc/assets/mf_classic_dict.nfc) and all NFC keys from [Proxmark3's Iceman](https://github.com/RfidResearchGroup/proxmark3/tree/master/client/dictionaries) then removed dupes.

This has almost tripled the amount of verified keys and has been very successful in capturing all 32 under multiple tests during reads.

To use, replace the existing file of the same name under SD -> nfc -> assets. You can rename the existing as a backup or [download it again](https://github.com/Eng1n33r/flipperzero-firmware/blob/dev/assets/resources/nfc/assets/mf_classic_dict.nfc) if ever needed.

NOTE: [This has been merged into the official Unleased firmware](https://github.com/Eng1n33r/flipperzero-firmware/commit/9e9b33445f0524d6d6ef4641cfc275731d0936c4).
