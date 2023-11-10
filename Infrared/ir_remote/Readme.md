# IR Remote (Alternate Remote Application from [Hong5489](https://github.com/Hong5489/ir_remote))

This is an application (FAP) similar to the [UniversalRF Remix](https://github.com/ESurge/flipperzero-firmware-unirfremix) but for infrared (IR) files. The author states `"I do this plugin for convenience, because the main IR app need to navigate for different button abit troublesome (buttons like up,down,left,right,back). I found it useful for TV and TV box."` We agree!

IR Remote supports both short press and long press (hold) input for different IR buttons.<br>
Compiled FAP has been tested on [Unleashed](https://github.com/DarkFlippers/unleashed-firmware) and [RogueMaster](https://github.com/RogueMaster/flipperzero-firmware-wPlugins) firmwares.<br>
FAP file is either included or can be obtained with the "extra apps" pack.

*Unleashed*: Download [Extra Apps Pack](https://download-directory.github.io/?url=https://github.com/xMasterX/unleashed-extra-pack/tree/main/apps) then look in `Misc`<br>
*RogueMaster*: Included under `Tools` folder.

Download all of the IR remote files [with this link](https://uberguidoz.github.io/DownGit/#/home?url=https://github.com/UberGuidoZ/Flipper/tree/main/Infrared/ir_remote/Example_Maps)

## How to use

The path of the actual IR file should be listed after `REMOTE:` at the top.<br>
The rest of the buttons should be defined by the button `name:` listed in the original IR file.

The first section are short press, the last with `HOLD` is long press (hold).

Example of the configuration file:
```
REMOTE: /ext/infrared/IRDB/TVs/Roku/Roku.ir
UP: Up
DOWN: Down
LEFT: Left
RIGHT: Right
OK: Ok
BACK: Back
UPHOLD: V_up
DOWNHOLD: V_down
LEFTHOLD: Play
RIGHTHOLD: Home
OKHOLD: Power
```

Leave a line empty if it will not be used for a button.

1. Make sure the file is saved as a `txt` file with Unix (LF) line ending.
2. Make a new folder in the root of your SD card named `ir_remote`.
3. *If using RogueMaster, place the `ir_remote` folder into the `infrared` folder on the SD.*
4. Place your TXT map file from above into the `ir_remote` folder.
5. Open the app, choose the configuration file, then enjoy!
6. Hold the back button to exit the app. (This cannot be programmed in the map file.)

![IR_Remote](https://user-images.githubusercontent.com/57457139/207985275-e5e5e1cb-28f6-48cc-bec1-5f8db5d95584.gif)

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
