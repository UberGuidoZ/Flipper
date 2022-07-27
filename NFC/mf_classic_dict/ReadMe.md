# Big changes coming!

Official firmware has introduced a user dict file option which will allow you to update firmware and not lose any added NFC keys! This is great, but requires a bit of work on my side to change things up. I'm going to remove all the NFC keys that are in the original 'mf_classic_dict.nfc' provided by Official firmware, then convert what's left over to the new `mf_classic_dict_user.nfc` file.

Look for updates soon. Until then... carry on with the below.

## Combined multiple mf_classic_dict.nfc files:
- [Official firmware](https://github.com/flipperdevices/flipperzero-firmware/blob/dev/assets/resources/nfc/assets/mf_classic_dict.nfc)
- [Unleased firmware](https://github.com/Eng1n33r/flipperzero-firmware/blob/dev/assets/resources/nfc/assets/mf_classic_dict.nfc)
- [MuddledBox firmware](https://github.com/MuddledBox/flipperzero-firmware/blob/dev/assets/resources/nfc/assets/mf_classic_dict.nfc)
 
Also included all NFC keys from [Proxmark3 Iceman's dictionaries](https://github.com/RfidResearchGroup/proxmark3/tree/master/client/dictionaries) and [RFIDresearchgroup](https://github.com/RfidResearchGroup), as well as random others online, then removed all dupes. This has almost **tripled** the amount of verified keys and been much more successful in capturing all keys in multiple tests during reads.

To use, replace the existing file `mf_classic_dict.nfc` under SD Card -> nfc -> assets.<br>
You can rename the existing as a backup or download it again from the links above if ever needed.

![File_location](https://user-images.githubusercontent.com/57457139/170588136-dbe99587-a04f-4dfd-8032-5e84b929cf58.png)

NOTE: This has been merged into [Unleased firmware](https://github.com/Eng1n33r/flipperzero-firmware/commit/9e9b33445f0524d6d6ef4641cfc275731d0936c4) as well as [RogueMaster's firmware](https://github.com/RogueMaster/flipperzero-firmware-wPlugins). Also important...

![Bank_Cards](https://user-images.githubusercontent.com/57457139/170762420-3e496885-d167-401a-ba4e-517a08a75214.png)
