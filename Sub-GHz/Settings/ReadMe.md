# Adding additional frequencies (TX/RX)

Official firmware now uses a different method for region locks. Per Aleksandr Kutuzov (skotopes):
```
Mobile app uses Phone/SIM region with fallback to GeoIP.
Desktop uses GeoIP and fallback to System.
```
This data is stored in the `.region_data` (hidden) file within internal storage and is set when connected to Desktop/Mobile per above.<br>
If you're having issues with regional retrictions that do not match your region, you can use the `region_data_null` file here.<br>
Simply download and rename it to `.region_data` then replace the one currently in your internal storage.

NOTE: This _will_ be overwritten each time you sync to the mobile app or qFlipper! **Use strictly for testing/verification purposes.**

## Note: You must be on the latest release!

Some changes I made to the available frequency settings (including unlocking them all). The file is found here:

![Sub-Ghz_Changes](https://user-images.githubusercontent.com/57457139/174948988-f6955976-2318-4e3e-b658-93f0465bb22e.png)

Officially supported frequencies: 300-348 MHz, 387-464 MHz, and 779-928 MHz (from [CC1101 chip docs](https://www.ti.com/product/CC1101))<br>
Unofficially supported frequencies: 281-361 MHz, 378-481 MHz, and 749-962 MHz (from [YARD Stick One](https://greatscottgadgets.com/yardstickone/) CC1111 docs)

Official & Unleashed currently do not allow anything outside of the officially supported CC1101 specs.<br>
RogueMaster allows unofficially supported frequencies with the `extend_range` file.

**NOTE: Going outside the supported frequencies may DAMAGE YOUR FLIPPER AMP.<br>
Please understand what you're doing if trying to break out of official frequencies.**

You'll need to edit some code and recompile if you want to break outside of the officially supported frequencies. [Proceed with caution.](https://github.com/UberGuidoZ/Flipper/blob/main/Sub-GHz/Restaurant_Pagers/SubGHz_changes.md)

# CAUTION within 402-408 range!<br>Medical devices can operate here.

## This range is purposefully not included in my file above.

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