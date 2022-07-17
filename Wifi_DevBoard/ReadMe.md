# Official info

The Wifi devboard ships with [Blackmagic firmware](https://github.com/flipperdevices/blackmagic-esp32-s2) installed. The Flipper documentation [is here](https://docs.flipperzero.one/development/hardware/wifi-debugger-module), and Blackmagic is [over here](https://black-magic.org/).

Quick start: Connect to the SSID `blackmagic` using the password `iamwitcher` or plug the wifi devboard in via USB.

# ESP32 Wi-Fi Pentest Tool

Check out [Frog's write-up](https://github.com/FroggMaster/ESP32-Wi-Fi-Penetration-Tool) and build for quick and easy flashing! Seriously, it's basically just a double-click.

# Some quick steps to get [Marauder](https://github.com/justcallmekoko/ESP32Marauder) installed<br>
(Direct from the dev WillStunForFood on Discord - check out [his video walkthrough](https://www.youtube.com/watch?v=_YLTpNo5xa0) too!):

- Plug the WiFi dev board directly into your PC
- Upload the MarauderOTA firmware from source via Arduino IDE
- Use the MarauderOTA firmware to flash the Marauder Flipper bin over WiFi

The first step in the wiki documentation [starts here](https://github.com/justcallmekoko/ESP32Marauder/wiki/flipper-zerowhile).<br>
(Then you should have the necessary links to the follow on documentation to get the firmware installed.)

ESP32-S2 is the correct board if you are installing on the Flipper WiFi Dev Board. <br>
If you are using the Marauder OTA method, you shouldn't have to install any libraries. <br>
The only other thing you should have to install is the boards for the ESP32 in the Arduino IDE and the drivers for the ESP32-S2.

Commands `channel`, `scanap`, `sniffbeacon`, `sniffdeauth`, `sniffpmkid`, `stopscan`, `clearap`

------------------------------------------------------------------------------

Example Attack Profile ([from Discord](https://discord.com/channels/740930220399525928/967843558520418384/997185157175988264)):

Use command `scanap` stop with `stopscan` when done.

List all found Beacons from previous steps via `list -a`

Note the enumeration of your target Beacon...

Use `select -a x` command to select your target. (x being your target # from previous step)

Execute chosen attack `attack -t deauth`

Use `stopscan` when done.

----------------------------------------------------------------------------------- 

Connecting to the devboard with a Mac ([from Discord](https://discord.com/channels/740930220399525928/967843558520418384/998043936977330276))

Open Terminal

Enter ls /dev/tty.*

You will be provided with several USB directories. Select one that has your flippers name in it example: /dev/tty.usbmodemflip_XXXXX3

Add "screen" in the prefix and the baud rate as the suffix to the command after copy pasting.....

screen /dev/tty.usbmodemflip_XXXXX3 115200

Hit reset on the flipper board and you'll see it populate. If it doesn't, simply try the other flipper directory name.

-----------------------------------------------------------------------------------

# Quick steps from Rabid Root...

![Quick_info](https://user-images.githubusercontent.com/57457139/171563068-4997e28d-ac75-4c22-96b3-9e21fb0cdb18.jpg)<br>
![Flipper_Settings](https://user-images.githubusercontent.com/57457139/176063404-86cfaa96-4cb5-4a94-a388-f935f59ac7b2.png)

# AND a great step by step from E_Surge!

![More_Steps](https://user-images.githubusercontent.com/57457139/176063439-f23620ae-7985-46eb-a11e-d85d1bcc62b7.png)

Also from E_Surge: "Flashed esp32marauder directly to the esp32-s2 using the esptool command -- wasn't working until a PC restart and boom. But it took about three hours of different methods, attempts, and finally restarting of devices etc."

`esptool -p PORT -b 460800 --before default_reset --after hard_reset --chip esp32s2  write_flash --flash_mode dio --flash_freq 80m --flash_size 4MB 0x10000 esp32_marauder_v0_9_9_20220628_flipper.bin`

# If serial connection looks scrambled... (thanks Frog!)
![Scrambled](https://user-images.githubusercontent.com/57457139/178117119-01907ac3-e0db-4ab2-b248-1c38e5926693.png)

![Frog_Fix](https://user-images.githubusercontent.com/57457139/178117083-fb209d3e-660a-4cef-b80e-f2e25ed2060e.png)

Frog also noted that it's wise to reflash the Flipper firmware if such issues are persisting.<br>
Start with the Official firmware, test, then move to a unlocked one if desired.
