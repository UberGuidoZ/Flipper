let vgm = require("vgm");
let __dirpath = "/ext/apps/Scripts";
let speaker = load(__dirpath + "/speaker_api.js");

let min_delta_angle = 29.98;
let max_wait_ms = 10000;
let freq_hz = 440;

//Uncomment below to test for various test cases...
//print(vgm.deltaYaw("30.312", 1000)); // Invalid arg (angle).
//print(vgm.deltaYaw()); // Invalid args. Pass (angle, [timeout]). Got 0 args.
//print(vgm.deltaYaw(1,2,3)); // Invalid args. Pass (angle, [timeout]). Got 3 args.
//print(vgm.deltaYaw(29.98, 1000)); // This should work fine.

// Show the pitch, roll, and yaw values for 5 seconds
for (let i = 0; i < 5; i++) {
    print("P", vgm.getPitch(), "R", vgm.getRoll(), "Y", vgm.getYaw());
    delay(1000);
}

// Play a sound that changes pitch based on yaw
for (let i = 0; i < 500; i++) {
    print("Freq", freq_hz, "Hz");
    speaker.play(freq_hz, 1.0, 250);

    let delta = vgm.deltaYaw(min_delta_angle, max_wait_ms);
    if (delta === undefined) {
        print("No VGM detected. Exiting...");
        break;
    }
    delta /= (min_delta_angle / 1.02);

    if (delta > 0) {
        freq_hz *= delta;
    } else if (delta < 0) {
        freq_hz /= -delta;
    }
}
