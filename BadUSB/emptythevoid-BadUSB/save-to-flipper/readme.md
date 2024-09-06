# Save data from your BadUSB scripts directly to the Flipper's SD Card!

Inspired by this thread: https://forum.flipperzero.one/t/anyway-to-save-files-back-to-the-flipper-using-badusb/2372

Credit: Major credit to @LupusE for taking my original proof-of-concept, running with it, and making a proper example payload!

Changes since original proof-of-concept:

- Increased baud rate from 115200 to 230400 (thanks for the suggestion Caribou!)

- Add capslock flashing to indicate when it's time to exit BadUSB mode

- Starts powershell hidden by default

- Bypasses Windows Terminal and invokes powershell directly

- Fast STRING and slower (but more compatible) ALTSTRING versions

Targets Windows 10, 11

BadUSB script that uses inline Powershell to enumerate which COM port the Flipper is connected to,
and then use Powershell's serial module to interface with Flipper's CLI and write text data to a specified file in Flipper's SD Card. It will automatically detect when the Flipper has exited BadUSB mode before writing the data to the SD card.

## What makes this method unique?
There are plenty of examples of using Ducky Script to exfiltrate data. Exfiltration by webhook/discord can be detected/blocked by IDS and firewall rules. Exfiltration by copying data to a USB flash drive (mass storage driver) can be thwarted by rules disallowing access to USB mass storage devices.  The "Save to Flipper" POC makes use of Windows' COM ports and the Powershell serial module.  This traffic is much more problematic to block, as many legitimate things can use the COM ports, and it is not affected by blocking access to mass storage devices. Further, you can tweak the device ID to further bypass any restrictions.

## Limitations:
Using serial to interface to the Flipper's CLI in order to save a file is *slow.*  This is not a huge problem for small amounts of text data, but could be problematic for a larger exfiltration.  

There also appears to be some kind of buffer size limit that will cause the exfiltrated data to fail to save. This is being investigated.

## Usage:
Attach Flipper to computer by USB cable and run the BadUSB script.  Watch the victim's keyboard for the caps lock light to blink a few times. Once it stops blinking, hit Back enough times to get to the Flipper application list (otherwise it won't save the data) and wait a moment and then remove the Flipper. (how long you need to wait depends on how large an exfil you're doing - it's slow)

## Variables:
$d is the command who's output you want to exfiltrate to the Flipper's storage. Make sure to include |Out-String  as the last part of the command.

$BHID and $BPID allow you to specify the Device ID parameters of your Flipper.

$SPATH is the location on the SD card to store your exfiltrated data. By default, it saves to /ext/apps_data/exfil_data

## Example
This example runs the Get-ComputerInfo command and saves it to the Flipper's SD card under /ext/apps_data/exfil_data

## Note about early disconnection
If the Flipper is disconnected from the host computer *while* it is receiving the exfiltrated data, the Flipper may remain stuck with it's serial CLI open, which will cause it to have an error when connecting it to qFlipper. Simply reboot the Flipper to get it working normally again.

## STRING vs ALTSTRING
There are two versions of this payload. One uses STRING, which is faster but depends on the keyboard layout. ALTSTRING is able to work with different keyboard layouts, but is much slower to deploy
