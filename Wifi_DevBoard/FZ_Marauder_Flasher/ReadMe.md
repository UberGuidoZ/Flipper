# Quick and easy flasher/updater for [Marauder](https://github.com/justcallmekoko/ESP32Marauder) on the Wifi Devboard!

Looking for a Linux/OS X version? [Check out SkeletonMan's Python edition!](https://github.com/SkeletonMan03/FZEasyMarauderFlash) (WIP for Windows too including full automated downloads.)

Looking for a quick video walkthrough on how to use this flasher? [Thanks to Lab401.com, you're in luck](https://www.youtube.com/watch?v=um_acrDaBK4)!

![Flasher_SS](https://user-images.githubusercontent.com/57457139/195889709-cd8086f2-dbd3-4ca2-9965-75be64fab4da.png)

## Now it's as easy as 1, 2, 3 to install or update Marauder on Windows.

1. Download and extract [the ZIP file](https://github.com/UberGuidoZ/Flipper/raw/main/Wifi_DevBoard/FZ_Marauder_Flasher/FZ_Marauder_v1.5.zip) above to the same directory.<br>
2. Hold `BOOT` on the devboard and plug it into your PC directly via USB.<br>
3. Double-click `flash.bat` from the extracted files then choose `Flash` or `Update`.

* If you choose `Flash Marauder` the script will locate your board and flash Marauder automatically!<br>
* If you choose `Update Marauder` you'll be taken to the Marauder download location to grab a new version.<br>
(Simply download the BIN file as stated and stick it in the Marauder subfolder of the batch file.)<br>
* If you choose `Save Flipper Blackmagic WiFi settings` your current Blackmagic configuration will be saved.<br>
* If you choose `Flash Flipper Blackmagic` the script will flash the original Blackmagic firmware it shipped with.

Current Marauder version included in the ZIP: v0.9.16.20221013 (current release as of Oct 14, 2022)

**Once the install has completed, [head over here](https://github.com/UberGuidoZ/Flipper/tree/main/Wifi_DevBoard#marauder-install-information) to learn more about what Marauder can do!**

The [batch file](https://github.com/UberGuidoZ/Flipper/blob/main/Wifi_DevBoard/FZ_Marauder_Flasher/Flash-v1.5.bat) is also included above for review and to track coming changes.

Acknowledgements:<br>
* [justcallmekoko](https://github.com/justcallmekoko/ESP32Marauder) for the AWESOME work in developing Marauder and porting it to the Flipper.
* [0xchocolate](https://github.com/0xchocolate) for the Marauder companion plugin (now in [Unleashed](https://github.com/Eng1n33r/flipperzero-firmware) and [RogueMaster](https://github.com/RogueMaster/flipperzero-firmware-wPlugins).)
* [Frog](https://github.com/FroggMaster) For initial scripting under the [Wifi Pentest Tool](https://github.com/FroggMaster/ESP32-Wi-Fi-Penetration-Tool) and inspiring the idea.<br>
* [ImprovingRigmarole](https://github.com/Improving-Rigmarole) Initial (and continued) scripting of this flasher and lots of  testing.<br>
* [UberGuidoZ](https://github.com/UberGuidoZ) Tweaking/Automating Frog's original, continued scripting, development, and testing.
