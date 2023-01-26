VB Lab Migration Assistant for Flipper Zero
===========================================

This app is designed to make transferring your characters from VB Lab and VBC
Lab more convenient.

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/C0C81P4PX)

Background
----------
The Vital Bracelet Arena app is going to be the new companion app for the Vital
Bracelet series of fitness bracelet toys, however the app does not support
account linking from VB Lab and VBC Lab, and requires you to transfer characters
one by one from those apps through your Vital Bracelet. Because the Vital
Bracelet can only hold at most two characters at a time and requires inserting
Dim/VBM to complete the transfer, transferring all of your characters may take a
significant amount of time. The VB Lab Migration Assistant is designed to make
this process faster by allowing you to store an unlimited amount of characters
on the Flipper and to bypass the Dim loading process, therefore allowing you to
transfer your characters without having to flip back and forth between the apps
or wasting time loading data that will never be used.

Usage
-----

### 1. Register Vital Bracelet

Registering your Vital Bracelet to the Flipper creates template data used for
emulating the VB using the Flipper.

- Select "Register Vital Bracelet" from Migration Assistant's main menu.
- If you have an active character on your Vital Bracelet, **transfer it to VB
  Lab/Arena**. The active character slot on the VB has to be empty to avoid
  issues when transferring character from VB Lab/Arena to the Flipper. Press the
  right key on Flipper.

If using VB Lab:
- If there is an active character in VB Lab, put it in storage.
- On VB Lab's "Scan" page, tap "Vital Bracelet -> App". Press the right key on
  Flipper.

If using VB Arena:
- Select the device type matching your VB and any franchise.
- Tap the "Send" button. Press the right key on Flipper.

- Tap the Flipper to your Vital Bracelet. You do not have to put the VB in scan
   mode. When Flipper finishes reading, it will beep.
- On VB Lab, tap "Send", or on VB Arena, tap "Transfer Data", and tap the
  Flipper to your phone. VB Lab/Arena will give you an error message. This is
  normal, and you can dismiss it. The Flipper will beep. Wait for it to switch
  to the next screen.
- Tap the Flipper to the original Vital Bracelet again. The Flipper will beep
  when it's finished reading.
- Name the Vital Bracelet and select "Save".

The Vital Bracelet info will show after saving.

### 2. Select Vital Bracelet, Vital Bracelet Info

Use the "Select Vital Bracelet" menu to load a previously registered VB. After
loading, you will see information about this Vital Bracelet:

- Registered name
- Vital Bracelet type
- Number of characters captured

Press the right key to see a menu of options you can use with the registered VB.

### Spoof Version

Vital Bracelet Arena splits Vital Bracelet and Vital Hero Digimon into
mutually-exclusive categories, and currently transferring VBM characters from
VBDM has not been tested, so if you want to transfer between categories, you can
select the Vital Bracelet version to spoof to change the VB type and force the
app to perform transfers between categories.

Note that spoofing a Vital Bracelet Characters from a non-VBC registration and
vice versa will not work because different encryption keys are used.

### Unlink Account

This option unsets the account flag in the NFC data so the character is not
bound to any particular account, and you can transfer it to a different Vital
Bracelet that is using a different account when using VB Arena.

If enabled when transferring from app, the flag will be unset on the capture and
will still be unset when transferring to app. If enabled when transferring to
app, the flag will be unset on all transfers, regardless whether the flag was
set when captured.

Note that VB Lab may still complain because it also uses character ID embedded
in character data to check whether the character belongs to the current account.

### 3. Transfer App > Flipper

Use this mode to transfer characters from VB Lab to Flipper.

- An instructions screen is displayed. Press right key to continue.

If using VB Lab:
- On your VB Lab app, wake up the character you want to send (if you have not
  already).
- Go to the "Scan" screen, and tap "App -> Vital Bracelet". Select "Check Dim
  Card". If prompted to send Special Missions or items, tap "No".

If using VB Arena:
- Select the device and franchise for the character you want to transfer.
- Tap the "Assign" button.
- Press and hold on the character you want to transfer.
- Tap the "Send" button.
- Tap "OK".
- Tap "Check Card".

- Tap the Flipper to your phone. Release when VB Lab indicates to do so, and
  wait for Flipper to beep.
- Tap "Check Dim/Card Installation" on VB Lab/Arena, and tap Flipper to your
  phone. Flipper will beep.
- Tap "Send" on VB Lab, or "Transfer Data" on VB Arena, and tap Flipper to your
  phone. Hold it there until VB Lab/Arena shows the transfer animation. Flipper
  will beep after it has received the character.
- Tap "OK" on VB Lab/Arena. Flipper will have automatically reloaded the
  template so you can repeat the above again to transfer more characters.
- When you are done transferring characters, press the left key on Flipper to
  exit the transfer mode.

Note if you have previously transferred characters to Flipper, the new
characters you transfer will be put at the end of the sequence of characters
when transferring from Flipper to app.

### 4. Transfer Flipper > App

Use this mode to transfer characters from Flipper to VB Lab.

- An instructions screen is displayed. Press right key to continue.

If using VB Lab:
- If there is an active character in VB Lab, put it in storage.
- On VB Lab's "Scan" page, tap "Vital Bracelet -> App".
- Tap "Send".

If using VB Arena:
- Select the device type matching your VB and any franchise.
- Tap the "Send" button.
- Tap the "Transfer Data" button.

- Tap your Flipper to your phone, and hold until VB Lab/Arena shows the
- transfer animation. Flipper will beep when it detected the transfer and will
- load the next character.
- Repeat the above until all of your characters have been transferred.
- If you want to skip a certain character, press the right key to skip and load
- the next character.
- You will see the "Transfers complete" screen on Flipper when all characters
- have been transferred. Press the right key to return to the menu.

### 5. Clear Captures

If you are doing transfers in batches, you can clear the currently captured
characters so you do not have to skip them when transferring to app. Check the
details, and press the right key to confirm deletion.

### 6. Delete Vital Bracelet

If you want to unregister the Vital Bracelet, you can use this option to delete
its captures and remove it from the "Select Vital Bracelet" list. Check the
details, and press the right key to confirm deletion.

Credits
-------

Graphics by [Aderek](https://twitter.com/AderekArt)
