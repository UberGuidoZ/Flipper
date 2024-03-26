let badusb = require("badusb");
let notify = require("notification");
badusb.setup({ vid: 0xAAAA, pid: 0xBBBB, mfr_name: "Flipper", prod_name: "Zero" });

print("Waiting for connection");
while (!badusb.isConnected()) {
    delay(1000);
}

function OpenSiteWin()
{
    badusb.press("GUI", "r");
    delay(100);
    badusb.println("https://rogue-master.net", 1);
    delay(100);
}
if (badusb.isConnected())
{
    OpenSiteWin();
    print("Executed");
}