# flipperzero-lrs-pagers
FlipperZero Brute force of LRS Pager System from [JimiLinuxGuy](https://github.com/jimilinuxguy/flipperzero-lrs-pagers)

Only the 433 version works in all released firmwares (though it may not work for most/any pagers.)<br>
For the standard 467 version, you'll need to modify the Flipper firmware. See warning below.

This should brute force all resturaunt IDs and pager ids and alert/beep each one for 30s. Verified that this works at Chilis.

Officially supported frequencies are 300-348 MHz, 387-464 MHz, and 779-928 MHz (from [CC1101 chip docs](https://www.ti.com/product/CC1101))<br>
Unofficially supported frequencies are 281-361 MHz, 378-481 MHz, and 749-962 MHz (from [YARD Stick One](https://greatscottgadgets.com/yardstickone/) docs)

**NOTE: Going outside the supported frequencies may DAMAGE YOUR FLIPPER.<br>
Please understand what you're doing if trying to break out of official frequencies.**

You'll need to edit some code and recompile if you want to break outside the officially supported frequencies. ([Hint here...](https://github.com/flipperdevices/flipperzero-firmware/pull/1287/files))

Referenced Work:
* [LRS Paging Systems](https://paging-systems.readme.io/v1.2/docs/am-page-modes)
* [Mayhem Portapack Firmware](https://github.com/eried/portapack-mayhem/blob/next/firmware/application/apps/ui_coasterp.cpp)
* [Tony-Tiger - Hacking Restaurant Pagers with HackRF](https://www.youtube.com/watch?v=ycLLb4eVZpI)
