A complete (I think) capture of an ATI Wonder II remote. These are raw captures.

This device works over SubGhz to a receiver station.  

I have tested what I could using the Linux kernel driver on a regular desktop.  A lot will depend on your setup and what input is assigned to which action. It has not been tested on Windows.  Not all keys could be tested.

I had issues nailing down the exact frequency, so I've done my best. They may need to be adjusted.

# Remotes
I have included some convenient remote maps, if you're able to sue the Sub-GHz Remote app.  Copy the .ir files into /etc/subghz/ATI_Wonder_II/ and then place the remote .txt files into /etc/subhgz/remote

Ati_media:
Controls Vol Up/Down, Play/Pause, Rew, FF.  No room for a Mute button.

Ati_media_alt: (More useful for Youtube on desktop)
Fullscreen toggle, Play, Left, Right

Ati_mouse: 
This remote is almost useless, since it's hard to get the speed captured right for the mouse movement.  The remote layout also doesn't allow a right-click.

Ati_arrows:
Just a mapping for just the directional arrows and OK button.
