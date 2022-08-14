# Quick and easy flasher/updater for [Marauder](https://github.com/justcallmekoko/ESP32Marauder) on the Wifi Devboard!

![Screenshot](https://user-images.githubusercontent.com/57457139/184529067-27ed301f-fd19-4779-a85d-c134dbf93ba6.png)

## Now it's as easy as 1, 2, 3 to install or update Marauder on Windows.

1. Download and extract [the ZIP file](https://github.com/UberGuidoZ/Flipper/raw/main/Wifi_DevBoard/FZ_Marauder_Flasher/FZ_Marauder_v1.0.zip) above to the same directory.<br>
2. Hold `BOOT` on the devboard and plug it into your PC directly via USB.<br>
3. Double-click `flash.bat` from the extracted files then choose `Flash` or `Update`.

If you choose `Flash` the script will locate your board and flash Marauder automatically!<br>
If you choose `Update` you'll be taken to the Marauder download location to grab a new version.<br>
(Simply download the BIN file as stated and stick it in the Marauder subfolder of the batch file.)

**Current Marauder version included in the ZIP: v0.9.13.20220806 (current at release Aug 14, 2022)**

The [batch file](https://github.com/UberGuidoZ/Flipper/blob/main/Wifi_DevBoard/FZ_Marauder_Flasher/flash.bat) is also included above for review and to track coming changes.<br>
*Coming soon: Black Magic (original debug firmware) flashing, firmware backup, firmware restore.*

Acknowledgements:<br>
* [justcallmekoko](https://github.com/justcallmekoko/ESP32Marauder) for the AWESOME work in developing Marauder and porting it to the Flipper.
* [0xchocolatex](https://github.com/0xchocolate) for the Marauder companion plugin (now in [Unleashed](https://github.com/Eng1n33r/flipperzero-firmware) and [RogueMaster](https://github.com/RogueMaster/flipperzero-firmware-wPlugins).
* [Frog](https://github.com/FroggMaster) For initial scripting under the [Wifi Pentest Tool](https://github.com/FroggMaster/ESP32-Wi-Fi-Penetration-Tool) and inspiring the idea.<br>
* [ImprovingRigmarole](https://github.com/Improving-Rigmarole) Initial (and continued) scripting of this flasher and lots of  testing.<br>
* [UberGuidoZ](https://github.com/UberGuidoZ) Tweaking/Automating Frog's original, continued scripting, development, and testing.
