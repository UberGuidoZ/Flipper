let gpio = require("gpio");
let pins = ["PA7", "PA6", "PA4", "PB3", "PB2", "PC3"];
for (let i=0; i<pins.length;i++) {
  gpio.init(pins[i], "outputPushPull", "none");
}

function reset() {
  for (let i=0; i<pins.length;i++) {
    gpio.write(pins[i], false);
  }
}

let delay_ms = 2000;
while (true) {
    // 0b100000 / 0b111111 * 3.3 = 1.65 volts
    print("1.65 volts");
    reset();
    gpio.write(pins[5], true); 
    delay(delay_ms);

    // 0b101010 / 0b111111 * 3.3 = 2.2 volts
    print("2.2 volts");
    reset();
    gpio.write(pins[5], true); 
    gpio.write(pins[3], true); 
    gpio.write(pins[1], true); 
    delay(delay_ms);

    // 0b010011 / 0b111111 * 3.3 = 1.0 volts
    print("1.0 volts");
    reset();
    gpio.write(pins[4], true); 
    gpio.write(pins[1], true); 
    gpio.write(pins[0], true); 
    delay(delay_ms);
}
