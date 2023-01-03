# Quick and easy flasher/updater for [Marauder](https://github.com/justcallmekoko/ESP32Marauder) ESP32-WROOM!

## This flasher/updater will work for any ESP32-WROOM chip or the v4 OEM Marauder.

Looking for a Linux/OS X version? [Check out SkeletonMan's Python edition!](https://github.com/SkeletonMan03/FZEasyMarauderFlash)

![Flasher_1](https://user-images.githubusercontent.com/57457139/210445500-c0ef079e-6d5e-4157-b61f-2c4f8bc83820.png)

## Now it's as easy as 1, 2, 3 to install or update Marauder on Windows.

1. Download and extract [the ZIP file](https://github.com/UberGuidoZ/Flipper/raw/main/Wifi_DevBoard/FZ_Marauder_Flasher/ESP32-WROOM/Marauder_WROOM_v1.10.zip) above to the same directory.<br>
2. Plug in your ESP32-WROOM or v4 OEM Marauder directly into your PC directly via USB.<br>
3. Double-click `Flash-WROOM.bat` from the extracted files then choose `Flash` or `Update`.

* If you choose `Flash Marauder` the script will locate your board and flash Marauder automatically!<br>
* If you choose `Update Marauder` you'll be taken to the Marauder download location to grab a new version.<br>
(Simply download the OLD_HARDWARE BIN file, stick it in the WROOM subfolder by the batch file, and delete the old BIN.)<br>
* If you choose `Download USB UART` the script will open the Silicon Labs driver download.

Current Marauder version included in the ZIP: v0.10.0.20221222 (current release as of Dec 22, 2022)

**Once the install has completed, [head over here](https://github.com/justcallmekoko/ESP32Marauder/wiki) to learn more about what Marauder can do!**

The [batch file](https://github.com/UberGuidoZ/Flipper/blob/main/Wifi_DevBoard/FZ_Marauder_Flasher/ESP32-WROOM/Flash-WROOM.bat) is also included above for review and to track changes.

Acknowledgements:<br>
* [justcallmekoko](https://github.com/justcallmekoko/ESP32Marauder) for the AWESOME work in developing Marauder and porting it to the Flipper.
* [Frog](https://github.com/FroggMaster) For initial scripting under the [Wifi Pentest Tool](https://github.com/FroggMaster/ESP32-Wi-Fi-Penetration-Tool) and inspiring the idea.<br>
* [SkeletonMan](https://github.com/SkeletonMan03/FZEasyMarauderFlash) For initial WROOM scripting and inspiration.<br>
* [UberGuidoZ](https://github.com/UberGuidoZ) Creation of this one, plus tweaking/automating Frog's original, continued scripting, development, and testing.
