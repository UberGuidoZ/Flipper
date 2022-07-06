Some changes I made to the available frequency settings (including unlocking them all). The file is found here:

![Sub-Ghz_Changes](https://user-images.githubusercontent.com/57457139/174948988-f6955976-2318-4e3e-b658-93f0465bb22e.png)

Officially supported frequencies are 300-348 MHz, 387-464 MHz, and 779-928 MHz (from [CC1101 chip docs](https://www.ti.com/product/CC1101))<br>
Unofficially supported frequencies are 281-361 MHz, 378-481 MHz, and 749-962 MHz (from [YARD Stick One](https://greatscottgadgets.com/yardstickone/) docs)

**NOTE: Going outside the supported frequencies may DAMAGE YOUR FLIPPER.<br>
Please understand what you're doing if trying to break out of official frequencies.**

You'll need to edit some code and recompile if you want to break outside the officially supported frequencies. ([Hint here...](https://github.com/flipperdevices/flipperzero-firmware/pull/1287/files))

# CAUTION within 402-408 range! Medical devices can operate here.<br>
This range is purposefully not included in my file above.
