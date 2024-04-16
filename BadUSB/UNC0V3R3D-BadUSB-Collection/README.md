
# The Ultimate Flipper Zero Badusb Collection

NOTE: This is an archived version with some dead links/pics removed.

# Before you start!

In order to begin, you ``must read and accept`` the usage agreement. **This project is for educational purposes only**! Please seek permission before running any of the scripts provided by me. I cannot be held responsible for any damage that may occur as a result of your use of these scripts.
If you want to purchase a completely unique and personalized script contact me on discord.
![](header.png)

## Installation

Windows:

```sh
1. git clone the repo

2. Use the qFlipper app to drag the files into the folder onto your Flipper.
```

## Usage example

To begin using the scripts, ``please carefully read`` the "readme.md" file provided with each script. This file contains ``important information`` on how to use the script safely. Keep in mind that some scripts may ``potentially harm your system``, so be cautious and do not run unfamiliar scripts on your personal computer. To test scripts, it is recommended to ``use a virtual machine`` for safety.

## Release History

* v1.0
    * Released all the files
* v1.1
    * CHANGE: More options on storing the grabbed Wifi Passwords (WifiPassStealer.txt)
    * Added ``MEMZ.exe script``
* v1.2
    * Added ``NoMoreSound.txt`` Script
* v1.3
    * A few delay_time errors ``fixed``
* v1.4
    * Added a lot of new files to repo
* v1.5
    * Added ``DeleteMicrosoftStore.txt`` and fixed link to MEMZ.exe
* v1.6
    * Added a bunch of new files
* v1.7 (BIG UPDATE)
    * Added ``a lot`` of new files, fixed a few files
* v1.7.1 
    * Few fixes due to delay errors
* v1.8 
    * Added ``ChangeWinUsername.txt`` and ``setWinPass.txt``
* v1.9 
    * Added a bunch of new files
* v2.0 
    * Added ``DownLoadASCII`` and seperated ``Selfwriting`` Ascii
    * Instant-download Ascii is about ``20x faster``
* v2.1
    * Added readme files for every script
    * Readme files contain ``important info`` about the script
* v2.2
    * Added the ``GoodUSB`` folder
* v2.5
    * Skipped a few releases
    * Added a few files
    * Fixed ``SAM exfiltration``
      
## Usage Agreement

By downloading and using the scripts provided by UNC0V3R3D, you are automatically agreeing to the following usage agreement. If you do not agree to the terms of this agreement, you are not permitted to download or use the scripts.

1. You acknowledge that UNC0V3R3D ``is not responsible`` for your actions or any damage you may cause as a result of using the scripts.
2. You are ``permitted`` to share all of the files.
3. You are ``allowed`` to modify the files, but are still responsible for your own actions.
4. If you are using my scripts in your own repo, please consider giving credits.
5. You are allowed to do everything the license says...

# Instructions for newbies
In this quick instruction, you are going to learn how to set up the scripts and use them properly.
If you have any questions after reading this instruction, just DM me on Discord (UNC0V3R3D#8662).

## Installation

``` sh
1. Clone/Download the files directly from the repo or download the latest release.
2. Extract the files anywhere you like
3. If you are using a phone, just install the Flipper Zero mobile app.
4. If you are using a PC, just install the qFlipper app: https://flipperzero.one/update
5. Connect your Flipper via Bluetooth if you are using a phone, or connect it via usb if you are on PC.
6. Open qFlipper --> SD Card --> badusb --> Move the files here.
7. Now you are done with moving the files to your Flipper.
 ```

## Explanation of the files

Before using the files on your Flipper, it is important to thoroughly understand them. While the majority of the files in my BadUsb repository are relatively harmless, there are some that may potentially cause serious damage. It is important to exercise caution when using these files.

<h3> Understanding DuckyScript </h3>

* All BadUsb-Scripts are written in the ``DuckyScript 1.0`` language.
* The language is kind of ``easy`` to understand and to learn.
* If you really want to look further into this please refer to this [Documentation].
* The kind of ``hard part`` are the PowerShell scripts.
* We use PowerShell or PowerShell scripts in ``97%`` of all DuckyScripts.
* PowerShell or PowerShell scripts give us ``full power`` over the machine.
* If you want to learn how to write PowerShell scripts after learning the DuckyScript basics please refer to this [PowerShell-Guide].
* In BadUsb scripts, you will most likely find comments every few lines, that start with the command ``REM`` at the beginning.
* Those comments often ``explain the whole process`` and help you a lot.
* That's how you understand certain BadUsb scripts, but you can also often already identify the script by its file name.

<h3> Using the scripts properly </h3>

* So now that we have prepared everything, we can start to test our first script!
* We are going to run the first script on our ``own`` PC.
* Of course, you don't want to cause damage to your PC, so we are going to choose a harmless script.
* Let's choose a script that will draw something in the Notepad.
* I chose a [script] from my ASCII repository.
* If you want to open the text file on your phone or pc to see what is inside feel free to do so.
* To start you are going to start the Flipper and find the ``BadUSB`` category.
* There you will see all the scripts you have imported from your PC or phone.
* Now choose a harmless script and wait until the Flipper tells you to connect to a PC via the USB cable.
* Just press the middle button to start the script.
* Now the script should open Notepad and write a simple sentence "Hacked by UNC0V3R3D".
* If you succeeded congrats! You have just run your very first script.
* If something went wrong then please scroll further down to see the ``Troubleshooting section`` and follow the steps.

<h3> Troubleshooting Problems </h3>

First of all, you have to identify the problem. Then you can look at the list below and maybe you will recognize your problem.

* ``1.0`` <b> The script open random things and typed the text somewhere, where it shouldn't be. </b>
  * <em> So this is often caused by ``too short delays`` between the commands. In the BadUsb script file, you should see some commands, that start with ``DELAY``
       and then there is a number behind it. ``Example: DELAY 500``. The number stands for ``milliseconds``. Changing the delay to a ``higher number`` than the current number should solve the problem ``(DELAY 500 --> DELAY 700)`` </em>
       
* ``1.1`` <b> The Flipper shows an error like this: ``ERROR: line 5`` </b>
  * <em> If the Flipper prints random errors like this you should check the ``text file``. The most common thing causing this error is apparently a ``random blank line``
  between the commands. Otherwise, make sure there is no line containing the ``"LOCALE .."`` command. It doesn't properly work on the Flipper, yeah I do not know why that is. If there is still an error, look at the line where the error is coming from and make sure there is no ``space`` at the beginning of the line. </em>
  
* I hope that you find a a solution for your problem. If you need help feel free to always contact me via Discord or Email.

[PowerShell-Guide]: https://www.youtube.com/watch?v=IABNJEl2ZWk
[Documentation]: http://github.com/hak5darren/USB-Rubber-Ducky/wiki/Duckyscript
[script]: https://github.com/UberGuidoZ/Flipper/blob/main/BadUSB/UNC0V3R3D-BadUSB-Collection/Windows_Badusb/ASCII/Selfwriting/SimpleTroll.txt
[qFlipper]: https://flipperzero.one/update

## Meta

Distributed under the ``Attribution-NonCommercial-ShareAlike 4.0 International`` license. See ``LICENSE.md`` for more information. 

## Credits

* [UberGuidoZ] , [FalsePhilosopher] and [I-am-Jakoby]
* Make sure to check them out! They are the reason this repository exists. When I started learning how to create badusb scripts i learnt from them.

[UberGuidoZ]: https://github.com/UberGuidoZ
[FalsePhilosopher]: https://github.com/FalsePhilosopher
[I-am-Jakoby]: https://github.com/I-Am-Jakoby
