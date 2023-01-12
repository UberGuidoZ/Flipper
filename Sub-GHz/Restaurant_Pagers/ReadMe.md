# Fun with Restaurant Pagers

Officially supported frequencies: 300-348 MHz, 387-464 MHz, and 779-928 MHz (from [CC1101 chip docs](https://www.ti.com/product/CC1101))<br>
Unofficially supported frequencies: 281-361 MHz, 378-481 MHz, and 749-962 MHz (from [YARD Stick One](https://greatscottgadgets.com/yardstickone/) CC1111 docs)

Official currently does not allow anything outside of the officially supported CC1101 specs.<br>
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
If you use [Unleashed](https://github.com/Eng1n33r/flipperzero-firmware/blob/dev/documentation/DangerousSettings.md) or [RogueMaster](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/420/documentation/DangerousSettings.md), both have options (linked) to allow these settings. Otherwise, you'll need to edit some code and recompile if you want to break outside of the officially supported frequencies. [Proceed with caution.](https://github.com/UberGuidoZ/Flipper/blob/main/Sub-GHz/Restaurant_Pagers/SubGHz_changes.md)

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