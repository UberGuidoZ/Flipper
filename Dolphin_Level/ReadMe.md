If your flipper level is moving too slow for you, there are ways you can fix that! Are you are the tinkering kind? See [DroomOne's](https://github.com/DroomOne/FlipperScripts) Python script. (All the files below were made with this script. I've notated the numbers used as _DO#_ later.)

If you would rather just "git'r'done" then grab one of the following files depending on your need.

`dolphin.state.ZERO` - Starting from scratch, just a n00b... _(DO#: 0)_<br>
`dolphin.state.TOP1` - Next action you do upgrades to LVL 2 _(DO#: 734)_<br>
`dolphin.state.LVL2a` - Instant LVL 2 animation on reboot _(DO#: 735)_<br>
`dolphin.state.TOP2` - Next action you do upgrades to LVL 3 _(DO#: 2939)_<br>
`dolphin.state.LVL3a` - Instant LVL 3 animation on reboot _(DO#: 2940)_<br>
`dolphin.state.FIN3` - You're the king, nothing left... _(DO#: 2941 or greater)_

Now that you have downloaded the dolphin.state file you want to a location you know, RENAME IT to just `dolphin.state`<br>
Next, open up qFlipper and head to the file browser section to transfer your new file:

![File_browser](https://user-images.githubusercontent.com/57457139/169634442-38acca0a-94e0-4038-aa54-dd33ebdffa29.png)

You should see the internal and external (SD) there - double-click on `Internal Flash`:

![Int_Flash](https://user-images.githubusercontent.com/57457139/169634459-a9e87dac-d180-4e09-b047-86dc7cad49f9.png)

There's not a lot here, but the important file is! You should see a dolphin.state file already present:

![Dolphin_State](https://user-images.githubusercontent.com/57457139/169634517-232ec48d-e7ec-44d0-a456-c2cad9adbf65.png)

Right-click on it and select "Rename..."

![Rename_Dolphin_State](https://user-images.githubusercontent.com/57457139/169634563-05313a4f-85d0-4a94-a298-a75e7b059fec.png)

Give it a name that you'll know what it is still and can go back. I picked `dolphin.state.mine`:

![Renamed_Dolphin_State](https://user-images.githubusercontent.com/57457139/169634598-920eeb36-7b1f-40fb-8ad2-bfda8c6a6620.png)

Great! Now drag and drop the `dolphin.state` file you downloaded from this repo right into the same spot:

![Copy_New_File](https://user-images.githubusercontent.com/57457139/169634632-3c8195ca-af54-43da-a636-7231ff36a988.png)

That's it! Now [restart Flipper](https://docs.flipperzero.one/basics/reboot) and enjoy your new level!

![L337_Level](https://user-images.githubusercontent.com/57457139/169634673-889e823f-4757-4911-ac34-5dd962e7f907.png)

If you find yourself longing for your original friend as it was, it's easy to go back. Repeat the process above, but now DELETE<br>
the `dolphin.state` file, then rename your `dolphin.state.mine` (or whatever you picked) back to `dolphin.state`.<br>
Finally, retart Flipper again and you should be back to where you started! (This can be done as many times as you like.)
