
# The Ultimate Flipper Zero Badusb Collection
<img src="https://github.com/UNC0V3R3D/ressources/blob/main/badusbpicnew.png" height="380" width="1050" >

![GitHub all releases](https://img.shields.io/github/downloads/UNC0V3R3D/Flipper_Zero-BadUsb/total?logo=GitHub) ![GitHub commit activity](https://img.shields.io/github/commit-activity/w/UNC0V3R3D/Flipper_Zero-BadUsb) ![GitHub repo size](https://img.shields.io/github/repo-size/UNC0V3R3D/Flipper_Zero-BadUsb) ![GitHub release (release name instead of tag name)](https://img.shields.io/github/v/release/UNC0V3R3D/Flipper_Zero-BadUsb?include_prereleases)
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=jo.112.nas@gmail.com&lc=US&no_note=0&item_name=Thank+you+for+supporting+UNC0V3R3D's+Github+Project.&cn=&curency_code=EUR&bn=PP-DonationsBF:btn_donateCC_LG.gif:NonHosted)


# Before you start!

Join the Discord-Community: https://discord.gg/WWQETvS8Vv <br>
Flipper Related Forum: ----

In order to begin, you ``must read and accept`` the usage agreement. **This project is for educational purposes only**! Please seek permission before running any of the scripts provided by me. I cannot be held responsible for any damage that may occur as a result of your use of these scripts.
If you want to purchase a completely unique and personalized script contact me on discord.
![](header.png)

## Installation

Windows:

```sh
1. git clone https://github.com/UNC0V3R3D/Flipper_Zero-BadUsb.git

2. Use the qFlipper app to drag the files into the "BadUsb" folder on your Flipper.
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
[Documentation]: https://web.archive.org/web/20220816200129/http://github.com/hak5darren/USB-Rubber-Ducky/wiki/Duckyscript
[script]: https://github.com/UNC0V3R3D/Flipper_Zero-BadUsb/blob/main/BadUsb-Collection/ASCII/Selfwriting/SimpleTroll.txt
[qFlipper]: https://flipperzero.one/update
[Patreon]: https://patreon.com/user?u=33918929&utm_medium=clipboard_copy&utm_source=copyLink&utm_campaign=creatorshare_creator&utm_content=join_link

## Sponsoring

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/Q5Q5HIDDD)

## Meta

If you have any idea on how to make this Instruction to BadUsb scripts better, feel free to open an Issue or contact me via Discord. :)

UNC0V3R3D – [@GitHub](https://github.com/UNC0V3R3D) – unc0v3r3d@proton.me

Distributed under the ``Attribution-NonCommercial-ShareAlike 4.0 International`` license. See ``LICENSE.md`` for more information. 

[https://github.com/UNC0V3R3D/Flipper_Zero-BadUsb](https://github.com/UNC0V3R3D/)


## Credits

* [UberGuidoZ] , [FalsePhilosopher] and [I-am-Jakoby]
* Make sure to check them out! They are the reason this repository exists. When I started learning how to create badusb scripts i learnt from them.


[release]: https://github.com/UNC0V3R3D/Flipper_Zero-BadUsb/releases
[UberGuidoZ]: https://github.com/UberGuidoZ
[FalsePhilosopher]: https://github.com/FalsePhilosopher
[I-am-Jakoby]: https://github.com/I-Am-Jakoby
