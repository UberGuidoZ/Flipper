This is an implementation of the classic Asteroids game for the [Flipper Zero](https://flipperzero.one/). Inside you will find a simple 2D engine that can be reused to implement other games. Note: This one is SimplyMinimal's fork of Antirez's version with several modifications.

# What's New
* Auto rapid fire (less wear on the buttons this way too)
* Up button applies thrusters
* Haptic feedback and LED effects
* High Score system
* Automatic save and load of high score
* Some modifications to certain game play elements

## What's coming next
* Settings screen
* Enabling sound effects (configurable on/off option)
* Power Ups

---

This is a screenshot, but the game looks a lot better in the device itself:

![Asteroids for Flipper Zero screenshot](/images/Asteroids.jpg)

# Controls:
* Left/Right: rotate ship in the two directions.
* Ok, short press: Short burst bullets
* Ok, long press: Auto-fire bullets
* Up: Accelerate
* Down: Decelerate
* Back (Long Press): Exit game. It will automatically save the high scoore too.

Your high scores will automatically be saved. Go forth and compete!

---
## Installing the binary file (no build needed)

Go to the releases and drop the `asteroids.fap` file into the
following Flipper Zero location:

    /ext/apps/Games

The `ext` part means that we are in the SD card. So if you don't want
to use the Android (or other) application to upload the file,
you can just take out the SD card, insert it in your computer,
copy the fine into `apps/Games`, and that's it.

## Installing the app from source

* Download the Flipper Zero dev kit and build it:
```
mkdir -p ~/flipperZero/official/
cd ~/flipperZero/official/
git clone --recursive  https://github.com/flipperdevices/flipperzero-firmware.git  ./
./fbt
```
* Copy this application folder in `official/application_user`.
* Connect your Flipper via USB.
* Build and install with: `./fbt launch_app APPSRC=asteroids`.

## License

BSD licensed. Enjoy.
