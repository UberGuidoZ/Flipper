let usbdisk = require("usbdisk");
let image = "/ext/apps_assets/mass_storage/test.img";
// print("Creating image...");
// usbdisk.createImage("/ext/apps_data/mass_storage/128MB.img", 128 * 1024 * 1024);
print("Starting UsbDisk...");
usbdisk.start(image);
print("Started, waiting until ejected...");
while (!usbdisk.wasEjected()) {
    delay(1000);
}
print("Ejected, stopping UsbDisk...");
usbdisk.stop();
print("Done");