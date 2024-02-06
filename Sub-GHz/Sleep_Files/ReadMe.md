# Blank SUB files for pausing playlists

NOTE: It appears these files may have stopped working on some custom firmware(s) recently ([issue here](https://github.com/UberGuidoZ/Flipper/issues/514)). There is a workaround discovered but it creates much larger files. Since larger files, that risk crashing the FDlipper in certain circumstances, is better than no working files at all, I'd like to provide a ZIP file (thanks [martineliascz](https://github.com/martineliascz)!) of the new files that appear to work correctly. [The temp/workaround files can be downloaded here](https://github.com/UberGuidoZ/Flipper/files/14022645/flipper_sleep_files.zip). Hopefully there will be an update soon and we'll be able to revert back to something a bit more managable.

While using the [SubGHz Playlist](https://github.com/darmiel/flipper-playlist) included in most custom firmware, it's been a common question as to how to pause the playlist to wait for something to finish before continuing. While you could record raw data for the needed time, you might pick up other signals that you don't wish to replay. Plus, then you're sitting there recording a long RAW or chaining together a bunch of short ones.

Thanks to [darmiel](https://github.com/darmiel) (creator of SubGHz Playlist), the issue is solved! I've confirmed via a pulse plotter and a HackRF these files send "nothing" when played back. They are manually generated, not recorded, and are designed to "pause" playlists by filling the space with nothing. I've sorted the files into `Seconds`, `Minutes`, and `Hours` for ease of finding what you need.

If you'd like to make a custom time, you can do so easily. You'll only need to change one number to the appropriate amount of time needed.
```
Filetype: Flipper SubGhz RAW File
Version: 1
Frequency: 433920000
Preset: FuriHalSubGhzPresetOok650Async
Protocol: RAW
RAW_Data: 1000 -9999000
```
The very last number, or `-9999000` in the above example, is the amount of time in microseconds. This should be just under the time you want by 1/1000 of a second as the first `1000` counts as one. In the above example, [this is 9.999 seconds](https://www.google.com/search?q=9999000+microseconds+to+seconds), which is 10 seconds when you add the first 1000 pulse.

So, to set your own, it's the same as adding 6 zeros to the amount of time in seconds, then subtracting 1/1000. For example, 45 seconds would be `-45000000` minus 1/1000 which is `-44999000` and the full file would be:
```
Filetype: Flipper SubGhz RAW File
Version: 1
Frequency: 433920000
Preset: FuriHalSubGhzPresetOok650Async
Protocol: RAW
RAW_Data: 1000 -44999000
```
To use these, simply put the SUB file in the spot of your playlist where you need it to pause. The Flipper will still appear to be sending something, but in fact it's sending a blank signal for the specified time.

-----

## Donation Information

Nothing is ever expected for the hoarding of digital files, creations I have made, or the people I may have helped.

## Ordering from Lab401? [USE THIS LINK FOR 5% OFF!](https://lab401.com/r?id=vsmgoc) (Or code `UberGuidoZ` at checkout.)

I've had so many asking for me to add this.<br>
![Flipper_Blush](https://user-images.githubusercontent.com/57457139/183561666-4424a3cc-679b-4016-a368-24f7e7ad0a88.jpg) ![Flipper_Love](https://user-images.githubusercontent.com/57457139/183561692-381d37bd-264f-4c88-8877-e58d60d9be6e.jpg)

**BTC**: `3AWgaL3FxquakP15ZVDxr8q8xVTc5Q75dS`<br>
**BCH**: `17nWCvf2YPMZ3F3H1seX8T149Z9E3BMKXk`<br>
**ETH**: `0x0f0003fCB0bD9355Ad7B124c30b9F3D860D5E191`<br>
**LTC**: `M8Ujk52U27bkm1ksiWUyteL8b3rRQVMke2`<br>
**PayPal**: `uberguidoz@gmail.com`

So, here it is. All donations of *any* size are humbly appreciated.<br>
![Flipper_Clap](https://user-images.githubusercontent.com/57457139/183561789-2e853ede-8ef7-41e8-a67c-716225177e5d.jpg) ![Flipper_OMG](https://user-images.githubusercontent.com/57457139/183561787-e21bdc1e-b316-4e67-b327-5129503d0313.jpg)

Donations will be used for hardware (and maybe caffeine) to further testing!<br>
![UberGuidoZ](https://cdn.discordapp.com/emojis/1000632669622767686.gif)
