# Various SUB files I've found or created. Some tested, some not.

Special thanks to [FalsePhilosopher](https://github.com/FalsePhilosopher) for organization help and everyone else that shares files!<br>
Worth a mention: If you have a HackRF, you can find a ton of files and info over at RocketGod's [HackRF Treasure Chest](https://github.com/RocketGod-git/HackRF-Treasure-Chest).

## How are the raw SUB files are formatted? (Adapted from a chat with [ImprovingRigmarole](https://github.com/improving-rigmarole)!)

This is a very quick and dirty explanation of the Flipper "raw" SUB format. Enough to figure out more at least.<br>
Each numerical value is the duration of a pulse in microseconds. Positive is `send` and negative is `pause`.<br>
Example: `3607 -100 1789 -66` means send (positive) during the `3607µs` then pause (negative) for the `100µs`, etc.<br>
You can easily plot any raw SUB (or IR) file at the [Official Pulse Plotter](https://my.flipp.dev/pulse-plotter) and see a graph to help decode patterns.

Another take... positive numbers in the raw .SUB files tell you how long (in microseconds / μs) the signal was ON and the negative numbers tell you how long it was OFF. Usually on means 1 and off means 0. This also makes sense when you remember/realize OOK = `On Off Keying`.

## NOTE: The deBruijn and OpenSesame files have moved to the [Garages subfolder](https://github.com/UberGuidoZ/Flipper/tree/main/Sub-GHz/Garages/deBruijn).

You can generate MANY of your own SUBs, such as MegaCode and Firefly, over at [Flipper Maker](https://flippermaker.github.io/).

Looking to mess with garage doors? You can try the OpenSesame/deBruijn files above.<br>
(Also check out more info at [RemoteDup](https://www.ifoedit.com/RemoteDup.html), [GarageHack](https://www.ifoedit.com/garagehack.html), and [Learn Button coding](https://support.dealer.liftmaster.com/articles/Knowledge/Determine-the-Color-of-the-Learn-Button-on-Your-Garage-Door-Opener).

# Frequency limitations and caveats

Officially supported frequencies: 300-348 MHz, 387-464 MHz, and 779-928 MHz (from [CC1101 chip docs](https://www.ti.com/product/CC1101))<br>
Unofficially supported frequencies: 281-361 MHz, 378-481 MHz, and 749-962 MHz (from [YARD Stick One](https://greatscottgadgets.com/yardstickone/) CC1111 docs)

Official does not allow anything outside of the officially supported CC1101 specs.<br>
RogueMaster & CodeGrabber (Unleashed) allows unofficially supported frequencies with the `extend_range` and `dangerous_settings` files.

**NOTE: Going outside the officially supported frequencies may DAMAGE YOUR FLIPPER AMP.<br>
Please understand what you're doing if trying to break out of official frequencies.**

Here's what the [manufacturer of the chip (CC1101) says](https://e2e.ti.com/support/wireless-connectivity/sub-1-ghz-group/sub-1-ghz/f/sub-1-ghz-forum/1120743/cc1101-operation-outside-datasheet-frequencies/4154832) about going outside the limits:

```
The frequency range of the chip is always tested in the verification tests and there is always some design margin included before
the VCO and/or PLL has problems operating for a specified frequency range. Working outside the frequency range can cause issues
with the VCO and/or PLL and/or divider not operating correctly. If the VCO is operating outside it's standard frequency range,
there are risks of unwanted emissions and no oscillation. The PLL can also fail to lock if operating outside it's standard
frequency range and will still apply power to the antenna.

Risks with antenna mismatch are increased harmonics, reduced output power and increased current consumption. Generally, the antenna
mismatch can be large and the output stage will not be damaged when presented with a large mismatch for short periods of time.
However, if the antenna mismatch is very poor for long periods of time, then this can effect the longevity of the chip especially
if further stressed with maximum voltage and maximum temperature. Recommend keeping VSWR better than 5:1 for worst case scenarios.
```

You'll need to edit some code and recompile if you want to break outside of the officially supported frequencies. [Proceed with caution.](https://github.com/UberGuidoZ/Flipper/blob/main/Sub-GHz/Restaurant_Pagers/SubGHz_changes.md)

Officially supported frequencies according to region (as manufactured and programmed on Official firmware):

`Allowed to transmit (MHz):`<br>
`R01: 433.05-434.79; 868.15-868.55`<br>
`R02: 304.10-321.95; 433.05-434.79; 915.00-928.00`<br>
`R03: 312.00-315.25; 920.50-923.50`<br>

`Shipped to:`<br>
`R01: EU, UK, RU, UA, BY, BA, PF, VA, IS, KZ, LI, MD, MK, NO, RS, CH, TR`<br>
`R02: US, CA, AU, NZ, MX, BR, CL, AR, UM`<br>
`R03: rest of the world`

# Modulation (AM/FM)

AM650 and FM650 modulation tries to cover more range of frequencies at the cost of less pickup distance.<br>
AM270 and FM270 modulation offer a more narrow range, but have a cleaner pickup and slightly better distance.

[Official documentation is an excellent read!](https://docs.flipperzero.one/sub-ghz/read-raw#HUe8f)

-----

I've had so many asking for me to add this.<br>
![Flipper_Blush](https://user-images.githubusercontent.com/57457139/183561666-4424a3cc-679b-4016-a368-24f7e7ad0a88.jpg) ![Flipper_Love](https://user-images.githubusercontent.com/57457139/183561692-381d37bd-264f-4c88-8877-e58d60d9be6e.jpg)

**BTC**: `3AWgaL3FxquakP15ZVDxr8q8xVTc5Q75dS`<br>
**BCH**: `17nWCvf2YPMZ3F3H1seX8T149Z9E3BMKXk`<br>
**ETH**: `0x0f0003fCB0bD9355Ad7B124c30b9F3D860D5E191`<br>
**LTC**: `M8Ujk52U27bkm1ksiWUyteL8b3rRQVMke2`

So, here it is. All donations of *any* size are humbly appreciated.<br>
![Flipper_Clap](https://user-images.githubusercontent.com/57457139/183561789-2e853ede-8ef7-41e8-a67c-716225177e5d.jpg) ![Flipper_OMG](https://user-images.githubusercontent.com/57457139/183561787-e21bdc1e-b316-4e67-b327-5129503d0313.jpg)

Donations will be used for hardware (and maybe caffeine) to further testing!<br>
![UberGuidoZ](https://cdn.discordapp.com/emojis/1000632669622767686.gif)
