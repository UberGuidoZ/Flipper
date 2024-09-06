Example code that runs powershell via BadUSB that waits for a drive label to mount, and then runs a command. You can use this to read a payload from the Flipper's SD card, such as an executable.

Usage:

- Run BadUSB script.  Powershell will launch and then wait for the specified drive label to mount.

- Back out of BadUSB app and switch to Mass Storage app and select a disk image with the matching disk label.

- Once Windows has mounted the drive, it will run the payload. 
