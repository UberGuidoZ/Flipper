# IR Remote (Alternate Remote Application from [Hong5489](https://github.com/Hong5489/ir_remote))

This is an application (FAP) similar to the [UniversalRF Remix](https://github.com/ESurge/flipperzero-firmware-unirfremix) but for infrared (IR) files. The author states `"I do this plugin for convenience, because the main IR app need to navigate for different button abit troublesome (buttons like up,down,left,right,back). I found it useful for TV and TV box."` We agree!

IR Remote supports both short press and long press (hold) input for different IR buttons.<br>
Compiled FAP has been tested on [Unleashed](https://github.com/DarkFlippers/unleashed-firmware) and [RogueMaster](https://github.com/RogueMaster/flipperzero-firmware-wPlugins) firmwares.<br>
FAP file is either included or can be obtained with the "extra apps" pack.

*Unleashed*: Download [Extra Apps Pack](https://download-directory.github.io/?url=https://github.com/xMasterX/unleashed-extra-pack/tree/main/apps) then look in `Misc`<br>
*RogueMaster*: Included under `Tools` folder.

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
3. Place your TXT map file from above into the `ir_remote` folder.
4. Open the app, choose the configuration file, then enjoy!
5. Hold the back button to exit the app. (This cannot be programmed in the map file.)

![IR_Remote](https://user-images.githubusercontent.com/57457139/207985275-e5e5e1cb-28f6-48cc-bec1-5f8db5d95584.gif)

NOTE: If using RogueMaster, place the `ir_remote` folder into the `infrared` folder on the SD.
