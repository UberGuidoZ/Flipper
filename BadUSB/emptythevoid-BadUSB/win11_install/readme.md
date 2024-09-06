Windows 11 installer helpers.  You must use these with a fresh Windows 11 ISO installer on a USB boot drive.  

These will likely fail to work as expected when the computer is set up with an OEM install.

These come in two parts:

Part 1- Handles the initial setup of wiping out any existing partitions, setting up the disk, and installing Windows. No customization is needed here.

Part 2- Handles the "setup" after install. You need to modify a few things for your setup:

youruser

yourpassword

security 1

security 2

security 3

It is highly recommended NOT to have your computer connected to Ethernet when you perform the installation, otherwise it will perform updates and try to make you use a Microsoft account.

If, however, Ethernet is plugged in, Part 2 will release the DHCP IP address before setup.  Once the setup is complete, Flipper will PAUSE and wait for a button press.  Perform this after the computer is safely at the desktop. This will renew the IP configuration.

The OOBE bypass scripts are likely not needed and obsolete.
