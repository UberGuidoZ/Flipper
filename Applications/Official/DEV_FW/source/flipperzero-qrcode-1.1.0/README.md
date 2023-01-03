# flipperzero-qrcode
Display qrcodes on the [Flipper Zero]

## Download
Grab the latest `qrcode.fap` from [Releases].

## Installation
Copy the `qrcode.fap` file onto your [Flipper Zero] sd card in the `apps/Tools`
directory. Then create a top level directory called `qrcodes` to store your
qrcode files. This can be done using [qFlipper], for example, by
draging-and-dropping `qrcode.fap` into `apps/Tools` and then navigating back to
the top level (where the directories like `infrared` and `nfc` live), right
click, and create a new folder called `qrcodes`.

## Creating QR Codes
qrcode files are simple text files with the extension `.qrcode`. This app will
expect them to live in a top-level directory on your sd card called `qrcodes`.
They should have the following content:

```
Filetype: QRCode
Version: 0
Message: your content here
```

### Message Format
qrcodes support 4 formats called "modes": numeric, alpha-numeric, binary, and
kanji. Because of the limited screen real-estate on the [Flipper Zero], you'll
want to pick the best mode for the data you are trying to display.

The app will automatically detect the best mode to use, so the only thing you
need to do is make sure the message in your file is formatted to use the best
mode. For example, if your message is entirely numeric, make sure you don't
include any extraneous punctuation in your file. If you're only encoding a
domain name, make sure it's uppercase to take advantage of alpha-numeric mode,
etc.

#### Numeric Mode
Consists of only numbers, nothing else. This mode can encode the most data.

#### Alpha-Numeric Mode
This mode can encode numbers, uppercase letters *only*, spaces, and the
following symbols: `$%*+-./:`. This format _may_ be appropriate for urls, as
long as you're only encoding the domain name and you remember to use uppercase
letters (ex: `HTTP://EXAMPLE.COM`). If your url includes some path after the
domain, you'll likely need to use binary mode because the paths are usually
case-sensitive.

A qrcode in alpha-numeric mode can encode ~40% less data than numeric mode.

#### Binary Mode
This mode is a little bit of a misnomer: binary mode simply means that the
message will be encoded as 8-bit bytes. The qrcode standard stipulates that
text will use ISO-8859-1 (also known as Latin-1) encoding, _not_ utf8 as would
be the standard these days. However, _some_ readers _may_ automatically detect
utf8. To be standard-compliant, that basically means you can only use Latin
letters, numbers, and symbols.

A qrcode in binary mode can encode ~60% less data than numeric mode, and ~30%
less than alpha-numeric.

#### Kanji Mode
This mode is unsupported, so I won't go into detail. A limitation of the
underlying qrcode library that I'm using, unfortunately. If there's interest,
perhaps I'll hack in support sometime.

## Using the App
The app is fairly straightforward. When it first starts, the file browser will
automatically open to the `qrcodes` directory and display any `.qrcode` files.
Select one using the arrow keys and the center button. The qrcode will display.
If you push the right arrow, some stats will display: the qrcode "Version" -
which corresponds to how big it is; the ECC level - which determines the
qrcode's resilience to damage, such as a dirty screen (Low, Medium, Quartile,
and High); and the qrcode Mode (Numeric, Alpha-Numeric, Binary, or Kanji).

While viewing the stats, you can select Version or ECC using the up and down
arrows and the center button. You can then increase or decrease the Version or
ECC using up and down and save your choice using the center buttton. This
feature was mostly added for my own amusement and testing, but, theoretically,
it may help a reader that's having trouble if the default ECC is less than the
highest value ("H"): you can increase the Version by 1 and then set the ECC to
"H". Whether or not this helps depends on the reader.

You can hide the stats by pressing the left arrow.

When you're done viewing the qrcode, press the back button to return to the
file browser. If you push the back button in the file browser, the app will
exit.

I will ask that you temper your expectations: the Flipper Zero screen is small
and many readers may have difficulty reading the qrcodes, especially if they
are encoding a lot of data. However, I have successfully got my iPhone to read
qrcodes encoding phone numbers, wifi info, and a url, all the way up to a
version 11 qrcode (ie, the largest size the screen will fit).

## Example: Wifi QRCodes
Most phones can automatically connect to wifi networks from a qrcode. If you
should like to encode your wifi's connection info into a qrcode, here's how
you'd do it:

```
Filetype: QRCode
Version: 0
Message: WIFI:S:<ssid>;P:<password>;T:<encryption>;
```

Replace `<ssid>` with the name of your wifi, `<password>` with the password.
`<encryption>` would be "WPA" or "WEP". If your wifi is open (no password),
this can be "None" and you can remove `P:<password>;` from the message. If your
wifi is hidden (ie, does not broadcast the ssid), you can add `H:true;` to the
end.

Note that if your ssid or password contain any of these characters: `\";,:`,
you'll need to "escape" it by placing a backslash (`\`) before it.

For example, if my ssid was "wifiball" and not broadcast, and the password was
"pa$$:word" with WPA encryption, the message would be:

```
Message: WIFI:S:wifiball;P:pa$$\:word;T:WPA;H:true;
```

## Building
First, clone the [flipperzero-firmware] repo and then clone this repo in the
`applications_user` directory:

```bash
git clone git@github.com:flipperdevices/flipperzero-firmware.git
cd flipperzero-firmware/applications_user
git clone git@github.com:bmatcuk/flipperzero-qrcode.git
```

Next, in the base of the [flipperzero-firmware] directory, run fbt:

```bash
cd ..
./fbt fap_qrcode
```

This will automatically install dependencies and build the application. When it
has finished building, the .fap will be in
`build/f7-firmware-D/.extapps/qrcode.fap` (fbt output will tell you where to
find the .fap, should it change in the future).

## qrcode library
This application uses the [QRCode] library by ricmoo. This is the same library
that is in the lib directory of the flipper-firmware repo (which was originally
included for a [now-removed demo app]), but modified slightly to fix some
compiler errors.

[now-removed demo app]: https://github.com/flipperdevices/flipperzero-firmware/pull/160/files
[flipperzero-firmware]: https://github.com/flipperdevices/flipperzero-firmware
[Flipper Zero]: https://flipperzero.one/
[QRCode]: https://github.com/ricmoo/QRCode
[qFlipper]: https://docs.flipperzero.one/qflipper
[Releases]: https://github.com/bmatcuk/flipperzero-qrcode/releases/latest
