# flipperzero-lrs-pagers
FlipperZero Brute force of LRS Pager System from [JimiLinuxGuy](https://github.com/jimilinuxguy/flipperzero-lrs-pagers)

Removed the 433 version as it was confirmed to not work for any pagers. (EU is 467 too!)<br>
For the standard 467 version, you'll need to modify the Flipper firmware. See warning below.

This should brute force all resturaunt IDs and pager ids and alert/beep each one for 30s. Verified that this works at Chilis.

Officially supported frequencies: 300-348 MHz, 387-464 MHz, and 779-928 MHz (from [CC1101 chip docs](https://www.ti.com/product/CC1101))<br>
Unofficially supported frequencies: 281-361 MHz, 378-481 MHz, and 749-962 MHz (from [YARD Stick One](https://greatscottgadgets.com/yardstickone/) CC1111 docs)

Official currently does not allow anything outside of the officially supported CC1101 specs.<br>
RogueMaster & CodeGrabber (Unleashed) allows unofficially supported frequencies with the `extend_range` and `dangerous_settings` files.

**NOTE: Going outside the officially supported frequencies may DAMAGE YOUR FLIPPER AMP.<br>
Please understand what you're doing if trying to break out of official frequencies.**

You'll need to edit some code and recompile if you want to break outside of the officially supported frequencies. [Proceed with caution.](https://github.com/UberGuidoZ/Flipper/blob/main/Sub-GHz/Restaurant_Pagers/SubGHz_changes.md)

Referenced Work:
* [LRS Paging Systems](https://paging-systems.readme.io/v1.2/docs/am-page-modes)
* [Mayhem Portapack Firmware](https://github.com/eried/portapack-mayhem/blob/next/firmware/application/apps/ui_coasterp.cpp)
* [Tony-Tiger - Hacking Restaurant Pagers with HackRF](https://www.youtube.com/watch?v=ycLLb4eVZpI)
