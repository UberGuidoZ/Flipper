# Quick and easy flasher/updater for [Marauder](https://github.com/justcallmekoko/ESP32Marauder) (Devboard/WROOM)!

Looking for a Linux/Mac version? [Check out SkeletonMan's Python edition!](https://github.com/SkeletonMan03/FZEasyMarauderFlash) (WIP for Windows too including full automated downloads.)

Looking for a quick video walkthrough on how to use this flasher? [Thanks to Lab401.com, you're in luck](https://www.youtube.com/watch?v=um_acrDaBK4)!<br>
Or if you'd rather have a quick rundown of using the Flipper Application, check out [this summary from haxez.org](https://haxez.org/2022/11/marauding-wi-fi-networks-with-the-flipper-zero/).

![Flasher_SS](https://github.com/UberGuidoZ/Flipper/assets/57457139/b6069d3b-856a-4d64-928e-82810d402803)

## Now it's as easy as 1, 2, 3 to install or update Marauder on Windows.

1. Download and extract [the ZIP file](https://github.com/UberGuidoZ/Flipper/raw/main/Wifi_DevBoard/FZ_Marauder_Flasher/FZ_Marauder_v2.3.zip) above to the same directory.<br>
2. Hold `BOOT` on the devboard and plug it into your PC directly via USB.<br>
3. Double-click `flash.bat` from the extracted files then choose `Flash` or `Update`.

* If you choose `Flash Marauder` the script will locate your board and flash Marauder automatically!<br>
* If you choose `Update Marauder` you'll be taken to the Marauder download location to grab a new version.<br>
(Simply download the Flipper BIN file, stick it in the Marauder subfolder by the batch file, and delete the old BIN.)<br>
* If you choose `Save Flipper Blackmagic WiFi settings` your current Blackmagic configuration will be saved.<br>
* If you choose `Flash Flipper Blackmagic` the script will flash the original Blackmagic firmware it shipped with.

Current Marauder version included in the ZIP: v0.10.7.20230609 (current release as of June 9, 2023)

**Once the install has completed, [head over here](https://github.com/UberGuidoZ/Flipper/tree/main/Wifi_DevBoard#marauder-install-information) to learn more about what Marauder can do!**

The [batch file](https://github.com/UberGuidoZ/Flipper/blob/main/Wifi_DevBoard/FZ_Marauder_Flasher/Flash-v2.3.bat) is also included above for review and to track coming changes.

Acknowledgements:<br>
* [justcallmekoko](https://github.com/justcallmekoko/ESP32Marauder) for the AWESOME work in developing Marauder and porting it to the Flipper.
* [0xchocolate](https://github.com/0xchocolate) for the Marauder companion plugin (now in [Unleashed](https://github.com/Eng1n33r/flipperzero-firmware) and [RogueMaster](https://github.com/RogueMaster/flipperzero-firmware-wPlugins).)
* [Frog](https://github.com/FroggMaster) For initial scripting under the [Wifi Pentest Tool](https://github.com/FroggMaster/ESP32-Wi-Fi-Penetration-Tool) and inspiring the idea.<br>
* [ImprovingRigmarole](https://github.com/Improving-Rigmarole) Initial (and continued) scripting of this flasher and lots of  testing.<br>
* [UberGuidoZ](https://github.com/UberGuidoZ) Tweaking/Automating Frog's original, continued scripting, development, and testing.

-----

## Donation Information

Nothing is ever expected for the hoarding of digital files, creations I have made, or the people I may have helped.

## Ordering from Lab401? [USE THIS LINK FOR 5% OFF!](https://lab401.com/r?id=vsmgoc) (Or code `UberGuidoZ` at checkout.)

I've had so many asking for me to add this.<br>
![Flipper_Blush](https://user-images.githubusercontent.com/57457139/183561666-4424a3cc-679b-4016-a368-24f7e7ad0a88.jpg) ![Flipper_Love](https://user-images.githubusercontent.com/57457139/183561692-381d37bd-264f-4c88-8877-e58d60d9be6e.jpg)

**BTC**: `3AWgaL3FxquakP15ZVDxr8q8xVTc5Q75dS`<br>
**BCH**: `17nWCvf2YPMZ3F3H1seX8T149Z9E3BMKXk`<br>
**ETH**: `0x0f0003fCB0bD9355Ad7B124c30b9F3D860D5E191`<br>
**LTC**: `M8Ujk52U27bkm1ksiWUyteL8b3rRQVMke2`<br>
**PayPal**: `uberguidoz@gmail.com`

So, here it is. All donations of *any* size are humbly appreciated.<br>
![Flipper_Clap](https://user-images.githubusercontent.com/57457139/183561789-2e853ede-8ef7-41e8-a67c-716225177e5d.jpg) ![Flipper_OMG](https://user-images.githubusercontent.com/57457139/183561787-e21bdc1e-b316-4e67-b327-5129503d0313.jpg)

Donations will be used for hardware (and maybe caffeine) to further testing!<br>
![UberGuidoZ](https://cdn.discordapp.com/emojis/1000632669622767686.gif)
