// SK9822 RGB LED strip example

let gpio = require("gpio");
gpio.init("PC3", "outputPushPull", "no"); // pin, mode, pull
print("PC3 is clock");
gpio.init("PA7", "outputPushPull", "no"); // pin, mode, pull
print("PA7 is data");

function writeByte(data) {
    for (let i = 0; i < 8; i++) {
        let mask = 1 << (7 - i);
        gpio.write("PA7", (data & mask) === mask);
        gpio.write("PC3", true);
        gpio.write("PC3", false);
    }
}

function writeStart() {
    writeByte(0);
    writeByte(0);
    writeByte(0);
    writeByte(0);
}

function writeStop(b) {
    writeByte(b);
    writeByte(b);
    writeByte(b);
    writeByte(b);
}

function writeColor(r, g, b, i) {
    writeByte(0xE0 | (i & 31));
    writeByte(b);
    writeByte(g);
    writeByte(r);
}


// read value from PC1 and write it to PC3
let maxIntensity = 31;
let intensity = 0;
while (true) {
    writeStart();
    writeColor(255, 0, 0, intensity);
    writeColor(0, 255, 0, intensity);
    writeColor(0, 0, 255, intensity);
    writeColor(255, 255, 255, intensity);
    writeColor(255, 255, 0, intensity);
    writeColor(255, 0, 255, intensity);
    writeColor(0, 255, 255, intensity);
    writeColor(1, 0, 0, intensity);
    writeColor(4, 0, 0, intensity);
    writeColor(16, 0, 0, intensity);
    writeColor(64, 0, 0, intensity);
    writeStop(0);
    delay(20);
    intensity++;
    if (intensity > maxIntensity) {
        intensity = 1;
    }
}
