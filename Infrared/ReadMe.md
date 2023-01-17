Keeping all the IR files in my fork of the [Flipper-IRDB](https://github.com/UberGuidoZ/Flipper-IRDB)! (I'm now able to maintain the entire branch.)

Did you know you can quickly generate your own IR files on the go at [Flipper Maker](https://flippermaker.github.io/)?

 Looking for an updated Universal Remote files? Then you're in the right place! [Unleashed](https://github.com/Eng1n33r/flipperzero-firmware) and [RogueMaster](https://github.com/RogueMaster/flipperzero-firmware-wPlugins) both include these.<br>
 ***Now with ALL buttons too!***  All files developed with support/testing from [UberGuidoZ](https://github.com/UberGuidoZ). HUGE thank you to [amec0e](https://github.com/amec0e) for maintenance!
 
[TV universal remote](https://github.com/UberGuidoZ/Flipper/blob/main/Infrared/tv.ir) compiled by [Hyper_Mash](https://discord.com/channels/740930220399525928/954422774141710366/994121751023853668) and maintained by [amec0e](https://github.com/amec0e)<br>
[AC universal remote](https://github.com/UberGuidoZ/Flipper/blob/main/Infrared/ac.ir) added by [JEREMYNO](https://github.com/jaroslavmraz) and maintained by [amec0e](https://github.com/amec0e).<br>
[Soundbar/Speakers universal remote](https://github.com/UberGuidoZ/Flipper/blob/main/Infrared/audio.ir) added and maintained by [amec0e](https://github.com/amec0e).<br>
[Projector universal remote](https://github.com/UberGuidoZ/Flipper/blob/main/Infrared/projectors.ir) added by [xMasterx](https://github.com/Eng1n33r/flipperzero-firmware/commit/029f82dc822e6d3515b39b839a2f70500dc9bb86) and maintained by [amec0e](https://github.com/amec0e).<br>
[Fans universal remote](https://github.com/UberGuidoZ/Flipper/blob/main/Infrared/fans.ir) compiled by Edhel90 and maintained by [amec0e](https://github.com/amec0e)

-----

[Unleashed firmware](https://github.com/Eng1n33r/flipperzero-firmware/releases/latest) was the first to add all of the Universal Remotes. Highly advise checking out their work. (Thanks [xMasterx](https://github.com/xMasterx)!)

-----

Huge thank you to [ChaoticDynamic aka c-nagy](https://github.com/c-nagy) for some duplicate signal and file cleanup ([details here](https://github.com/UberGuidoZ/Flipper/pull/16).)<br>

**You may have manifest file issues if you do not compile your own firmware, or use one of the listed firmwares above!**<br>
*Both unlocked firmwares have been tested successfully. Official has stated they will merge it at some point.*

**NOTE: Long pauses are normal, including 10-15 seconds of no apparent activity. It's just skipping through the "other" buttons.**

Click on the file of your choice and save it with a right-click from the `Raw` button here:

![Download_File](https://user-images.githubusercontent.com/57457139/174234554-555503d2-019f-4dbe-b129-29b3a0a9f1e6.png)

Then, use qFlipper to transfer it to your Flipper and replace the existing one (or add a new one) here:

![Replace_This](https://user-images.githubusercontent.com/57457139/174234726-e39c1917-0d21-4b60-88c9-70fd60ee069f.png)

***Again, you may have manifest file issues if you do not compile your own or use a tested firmware above!***

## Protocol info
This info was gathered from the respecctive locations in the firmwares comments. <br>
Ie. lib/infrared/encoder_decoder/`*`/infrared_protocol_`*`.h<br>
Some of the info doesn't show up well so view thew raw readme or [this commit](https://github.com/flipperdevices/flipperzero-firmware/commit/75e9de12b065bd5e572b8b9232c8b9670c8e6f91).

### Kaseikyo protocol description
https://github.com/Arduino-IRremote/Arduino-IRremote/blob/master/src/ir_Kaseikyo.hpp <br>
*     Preamble   Preamble      Pulse Distance/Width          Pause       Preamble   Preamble
*       mark      space            Modulation             up to period    repeat     repeat
*                                                                          mark       space
*
*        3360      1665               48 bit              ...130000        3456       1728
`     __________          _ _ _ _  _  _  _ _ _  _  _ _ _                ___________`<br>
` ____          __________ _ _ _ __ __ __ _ _ __ __ _ _ ________________           ___________`

### NEC protocol description
https://radioparty.ru/manuals/encyclopedia/213-ircontrol?start=1 <br>
*     Preamble   Preamble      Pulse Distance/Width          Pause       Preamble   Preamble  Stop
*       mark      space            Modulation             up to period    repeat     repeat    bit
*                                                                          mark       space
*
*        9000      4500         32 bit + stop bit         ...110000         9000       2250
`     __________          _ _ _ _  _  _  _ _ _  _  _ _ _                ___________            _`<br>
` ____          __________ _ _ _ __ __ __ _ _ __ __ _ _ ________________           ____________ ___`

### RC5 protocol description
https://www.mikrocontroller.net/articles/IRMP_-_english#RC5_.2B_RC5X
*                                       Manchester/biphase
*                                           Modulation
*
*                              888/1776 - bit (x2 for toggle bit)
*
`                           __  ____    __  __  __  __  __  __  __  __`<br>
`                         __  __    ____  __  __  __  __  __  __  __  _`<br>
*                         | 1 | 1 | 0 |      ...      |      ...      |
*                           s  si   T   address (MSB)   command (MSB)
*
*    Note: manchester starts from space timing, so it have to be handled properly
*    s - start bit (always 1)
*    si - RC5: start bit (always 1), RC5X - 7-th bit of address (in our case always 0)
*    T - toggle bit, change it's value every button press
*    address - 5 bit
*    command - 6/7 bit

### RC6 protocol description
https://www.mikrocontroller.net/articles/IRMP_-_english#RC6_.2B_RC6A
*      Preamble                       Manchester/biphase                       Silence
*     mark/space                          Modulation
*
*    2666     889        444/888 - bit (x2 for toggle bit)                       2666
*
`  ________         __    __  __  __    ____  __  __  __  __  __  __  __  __`<br>
` _        _________  ____  __  __  ____    __  __  __  __  __  __  __  __  _______________`<br>
*                   | 1 | 0 | 0 | 0 |   0   |      ...      |      ...      |             |
*                     s  m2  m1  m0     T     address (MSB)   command (MSB)
*
*    s - start bit (always 1)
*    m0-2 - mode (000 for RC6)
*    T - toggle bit, twice longer
*    address - 8 bit
*    command - 8 bit

### SAMSUNG32 protocol description
https://www.mikrocontroller.net/articles/IRMP_-_english#SAMSUNG <br>
*  Preamble   Preamble     Pulse Distance/Width        Pause       Preamble   Preamble  Bit1  Stop
*    mark      space           Modulation                           repeat     repeat          bit
*                                                                    mark       space
*
*     4500      4500        32 bit + stop bit       40000/100000     4500       4500
`  __________          _  _ _  _  _  _ _ _  _  _ _                ___________            _    _`<br>
` _          __________ __ _ __ __ __ _ _ __ __ _ ________________           ____________ ____ ___`

### Sony SIRC protocol description
https://www.sbprojects.net/knowledge/ir/sirc.php <br>
http://picprojects.org.uk/
*      Preamble  Preamble     Pulse Width Modulation           Pause             Entirely repeat
*        mark     space                                     up to period             message..
*
*        2400      600      12/15/20 bits (600,1200)         ...45000          2400      600
`     __________          _ _ _ _  _  _  _ _ _  _  _ _ _                    __________          _ _`<br>
` ____          __________ _ _ _ __ __ __ _ _ __ __ _ _ ____________________          __________ _`
*                        |    command    |   address    |
*                 SIRC   |     7b LSB    |    5b LSB    |
*                 SIRC15 |     7b LSB    |    8b LSB    |
*                 SIRC20 |     7b LSB    |    13b LSB   |
*
* No way to determine either next message is repeat or not,
* so recognize only fact message received. Sony remotes always send at least 3 messages.
* Assume 8 last extended bits for SIRC20 are address bits.

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
