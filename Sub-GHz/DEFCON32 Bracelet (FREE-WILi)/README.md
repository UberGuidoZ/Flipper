# bracelet_subfiles
.sub files for the LED bracelets distributed at DEFCON ([Original source here](https://github.com/freewili/bracelet_subfiles))

# Adding SUB files to Flipper Zero

* Copy folder to SD card like any other SUB file(S) then execute via SubGHz menu or any SubGHz apps.
  
# Uploading the .sub files to FREE-WILi

* Use Python 3.11+
* Install the freewili library with pip: `pip install freewili`
* Find out which device is the FREE-WILi `Main` processor: `fwi-serial --list`

```
(python311) PS E:\downloads> fwi-serial --list
Found 2 FreeWili(s)
        1. Main v29 COM23 @ 1-5.1:x.0
        2. Display v28 COM22 @ 1-5.2:x.0
```

* In the above example, `Main` is device 1.  So we will use `--index 1` in the following commands.
* Upload the .sub file to the FREE-WILi: `fwi-serial --send_file yellow.sub -fn /radio/yellow.sub --index 1` (be sure to use the correct index!)
* You should see the .sub files in the `Radios/Transmit` menu now
