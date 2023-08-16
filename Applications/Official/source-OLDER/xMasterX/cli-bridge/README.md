# flipperzero-cli-bridge
Allows CLI control from GUI, giving untethered access to sub-ghz chat, system diagnostics, and more.
# Installation
## Easy way - get a .fap file from the releases page
Swing by the [releases](https://github.com/ranchordo/flipperzero-cli-bridge/releases) page, and download a pre-built .fap file for the latest flipperzero firmware. Use [qFlipper](https://flipperzero.one/update) to copy the .fap file into SD Card/apps/Tools/. MAKE SURE TO UPGRADE FLIPPERZERO FIRMWARE TO NEWEST VERSION BEFORE INSTALLING.
## Hard way - building from source
The following commands will (probably) not work on Windows. If you run Windows, use wsl or a linux vm or something.
```sh
git clone https://github.com/flipperdevices/flipperzero-firmware
cd ./flipperzero-firmware
git clone https://github.com/ranchordo/flipperzero-cli-bridge ./applications_user/flipperzero-cli-bridge/
./fbt fap_dist APPSRC=applications_user/flipperzero-cli-bridge
# If everything went well, the built .fap file can be found in ./dist/f7-D/apps/apps/Tools/cli_gui.fap
```
# Usage
On the flipperzero, you should be able to find a new application (CLI-GUI Bridge) under Applications->Tools. Opening it will result in a text prompt - the prompt for the command line. Enter a suitable command (quickly pressing the back button or holding `_` on the keyboard will input a space) such as `subghz chat [freq in hz, e.g. 310000000]`, etc, then navigate to and press the SAVE key. You should then see the command window. Use Up and Down to scroll, and use Left or Center to get back to the text input prompt. A quick tap of the back key while viewing the console output sends a Ctrl-C to the console, and a long press of the left or right keys during text input will navigate back to the console output without executing.
## Exiting the app
Holding and then releasing the back key for at least a second or so (long press) will exit the app normally, meaning that the inner terminal will send Ctrl-C and close. Any sessions will be disconnected.  
  
  
Holding and then releasing the OK key while focusing on the console output for at least a second or so (long press) will exit the app while keeping the terminal open. Terminal output will be cleared the next time you launch the app, but whatever command or session was running previously will be resumed. This is especially handy with subghz chat - exiting the app while keeping the terminal open will not disconnect you from the chat, and the flipper will still vibrate briefly whenever a new message comes in (even if the app is closed).  
  
  
NOTE: USB functionality (qFlipper, normal USB CLI) may not work after running the app (especially after exiting without closing the terminal), simply restart your flipper and all USB functionality will return to normal.
