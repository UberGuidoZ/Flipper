# Speaker

A JavaScript that plays tones using the `ffi` commands.  The advantage of the `ffi` commands is that they can run on firmware without requiring a particular module `.fal` file to exist on the Flipper.  Thanks to [Freehuntx](https://github.com/Freehuntx) for the various examples of using `ffi`.

## How to use
Copy the `speaker.js` file and `speaker_api.js` file to your Flipper Zero, typically the `SD Card/app/Scrips` folder. You can then run the script (on your Flipper press `OK` then choose `Apps`, `Scripts`, `speaker.js`).

NOTE: The script uses the `__dirpath` variable to locate the `speaker_api.js` file.  Some frameworks don't support this variable yet, so you may need to adjust the path to the `speaker_api.js` file.  For example change the first line of speaker.js to the following:
```js
let Speaker = load("/ext/apps/Scripts/speaker_api.js");
```

For an `ffi` version of script that obtains the same data as `__dirpath` see [this Discord post](
https://discord.com/channels/1211622338198765599/1220056596638597327) by @Freehuntx.