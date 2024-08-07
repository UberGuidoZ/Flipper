# BadUSB

[![FlipperZero: BadUSB JavaScript](https://img.youtube.com/vi/DAUQGeG4pc4/0.jpg)](https://youtu.be/DAUQGeG4pc4)

This script was created for the [YouTube video](https://youtu.be/DAUQGeG4pc4) to demonstrate various concepts in JavaScript running on the Flipper Zero, using a real world scenario.

## Overview summary
This script uses features that were introduced in the dev branch of Momentum firmware on April 5, 2024. You need to be sure your firmware is updated or else you will get errors running the script.

This script will copy an MP3 file from the Flipper to the computer, play it on the computer, run commands to get information from the computer (like wifi credentials), copy the output back to the Flipper Zero, and display it and append it on a local file on the Flipper.  It will also automate switching between keyboard, disk, ejecting devices.

Many of these ideas are from the Momentum Discord server's script-sharing channel.  I encourage you to check out the various scripts being shared there.

## Detailed summary
This script will:

- Use `storage` APIs to see if a virtual USB disk image exists. If it does, it will delete it.  It then creates a new virtual USB disk image using the `usbdisk` APIs.  It will then use `storage` APIs to mount the virtual USB disk image and then it will use the `copy` function to copy a file from the SD card onto the virtual USB disk.  It will then unmount the virtual USB disk image.
- Next it will use `badusb` APIs to emulate a keyboard with the specified language layout.
- It will then display a `Waiting for connection` message on the screen to indicate that the Flipper Zero is ready to be connected to a computer.  It uses `badusb` APIs to wait for a connection.
- It will launch PowerShell on the connected computer (`Windows+x` then `i`).
- It creates a temporary directory on the computer and then switches to that directory.
- It then types all of the commands in the script array (e.g. get ips, get wifi creds, etc.) into the PowerShell window.  These commands should save output into the `localFileName`.
- It then types commands to scan for the virtual USB disk to be attached and then set the $DriveLetter variable to the drive letter of the virtual USB disk.
- If copyPayload is true, it will type commands to copy the payload file from the virtual USB disk to the temporary directory on the computer.
- If playPayload is true, it will type commands to play the payload file on the computer.
- If at least one script command is specified, it will type commands to copy the output file from the temporary directory on the computer to the virtual USB disk.
- It will then type commands to eject the drive (signaling that the script is done).
- It will then type commands to hide it's tracks (somewhat) and close the PowerShell window.
- At this point the `badusb` APIs will disconnect the Flipper Zero from the computer.
- The `usbdisk` APIs will then attach the virtual USB disk image.
- At this point the above scripts will detect the virtual USB disk image, and run the above commands, which will eventually finish by ejecting the virtual USB disk image.
- The `usbdisk` APIs will detect the detach the virtual USB disk image.
- The `usbdisk` APIs will then stop the virtual USB disk image.
- If at least one script command is specified, it will then use the `storage` APIs to mount the virtual USB disk image.  It will read the output file from the virtual USB disk image and display it on the screen using the `textbox` APIs.  It will also append the output file to the `loot.txt` file on the SD card.
- Once the back button is pressed, the storage APIs will unmount the image.

## Config
Edit the [badusbdemo.js](./badusbdemo.js) file to customize the script.  The following are the settings that should be changed:

- Make sure `layout` is set to the correct keyboard layout for the computer (like `en-US`, `en-UK`, etc).  See your [/ext/badusb/assets/layouts](https://github.com/Next-Flip/Momentum-Firmware/tree/dev/applications/main/bad_kb/resources/badusb/assets/layouts) for the list of supported latouts.
- `localTempFolder` is the temporary folder that will created on the computer.  You do not need to change this.
- `localFileName` is the name of the output file that will be used by scripts.  You do not need to change this.
- `script` array is the list of scripts to run.
  - If it is empty, then the MP3 will be copied and played, but no scripts will be run.
  - If it has at least one element, then the MP3 will be copied and played, and the scripts will be run.
  - Your commands should save output into the `localFileName`.
  - If you use a quote mark in your command, you should escape it with a backslash (e.g. `\"`).
  - Commands should end with a comma.
- `copyPayload` is `true` to copy the payload file from the Flipper to the computer (otherwise `false`).
- `playPayload` is `true` to play the payload file on the computer (otherwise `false`).
- `payloadName` is the name of the payload file to copy and play.
- `payloadSrcName` is the path to the file.  You do not need to change this.  It it expected that the script will also contains a `payloads` folder with this file.
- `payloadDstName` is the name of the file on virtual USB disk.  You do not need to change this.
- `lootFile` is the name of the file on the SD card to append the output file to.  You do not need to change this.  This will be saved in the same folder as the script.
- `exfilCapacityMb` is the size of the virtual USB disk image in MB.  You do not need to change this unless your payload is >4MB.
- `image` is the name of the virtual USB disk image.  You do not need to change this.
- `flipperStorageName` is the name of the virtual USB disk image when it is attached to the Flipper Zero.  You should not change this (the driver currently hard-codes this name).
- `resultFolder` is the name of the folder on the virtual USB disk image where the output file will be saved.  You do not need to change this.
- `resultFileName` is the name of the output file on the virtual USB disk image.  You do not need to change this.

## Setup

- Copy the [badusbdemo.js](./badusbdemo.js) file to the Flipper Zero.  It should be stored in `SD Card/apps/Scripts/badusbdemo.js`.
- Create a `payloads` folder in the same folder as the script (`SD Card/apps/Scripts/payload`).
- Create a `demo.mp3` file in the `payloads` folder.  This file will be copied to the computer and played.  This file should be less than 4MB (your `exfilCapacityMb`).

## Running

- Make sure you have the latest dev firmware installed on your Flipper.
- On the Flipper click the OK button, then choose `Apps`, then choose `Scripts`, then choose `badusbdemo.js`.
- Connect the Flipper Zero to the computer when prompted with `Waiting for connection`.
- The script will run and eventually disconnect the Flipper Zero from the computer.
- The script will finish and the output will be displayed on the screen and appended to the `loot.txt` file on the SD card in the (`SD Card/apps/Scripts/loot.txt`).

## Troubleshooting
- If you get... `"textbox" module load fail at :23` -- Your firmware is too old.  You need to update to the latest dev branch of the firmware.
- If you get... `"file/dir not exist at :71"` -- You need to put demo.mp3 (all lowercase) in `SD Card/apps/Scripts/payloads/demo.mp3`. The MP3 file I saved in payloads/demo.mp3 for the video was https://www.soundboard.com/track/download/156453
- If you get... `"out of memory"` -- Reboot the Flipper (BACK+LEFT buttons) before running the script to free up memory.
- If you get a different error, **please let me know** (ideally take a picture of the error with your phone) so I can investigate it. Even if you figure it out, it would be great to add to this troubleshooting section.

## Support

If you have any questions, please ask in my [Flipper Zero Discord](https://discord.com/invite/NsjCvqwPAd) server.  There are also giveaways and other fun things happening there.

Support my work: 
- Option 1. [Like, Subscribe and click the Bell (to get notified)](https://youtu.be/DAUQGeG4pc4) 
- Option 2. [https://ko-fi.com/codeallnight](https://ko-fi.com/codeallnight) (donate $3 via PayPal or Venmo) 
- Option 3. Click the "Thanks" button on [YouTube](https://youtu.be/DAUQGeG4pc4). 
- Option 4. Purchase a [FlipBoard](https://github.com/MakeItHackin/FlipBoard) (I get a portion of the sale).
