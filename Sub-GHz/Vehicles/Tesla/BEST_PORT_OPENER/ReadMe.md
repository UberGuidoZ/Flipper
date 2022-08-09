# "Pure" Tesla Charge Port Opener Files

[From the creator (fredilarsen)](https://github.com/fredilarsen/TeslaChargeDoorOpener):

<em>I’ve generated "clean" files. The ones available in every repository are RAW recordings.<br>
Those are generated files without the timing jitter or a recording.</em>

Original project was for an Arduino and an ASK STX882 transmitter. These have been converted for the Flipper!<br>
Generated/Created by the original author, also known as [GringoZobel on Discord](https://discord.com/channels/740930220399525928/954422680969445377/1004501487143096451).

Technical details also direct from the creator:

```
Hex:
aaaaaa8acb32cccccb4d2d4ad34cab4b1596659999969a5a95a69956962b2ccb33332d34b52b4d32ad28

Bits:
101010101010101010101010100010101100101100110010110011001100110011001011010011010010
110101001010110100110100110010101011010010110001010110010110011001011001100110011001
100101101001101001011010100101011010011010011001010101101001011000101011001011001100
101100110011001100110010110100110100101101010010101101001101001100101010110100101
```
```
The symbol rate is 2500 bits/second.
// The signal to send
const uint16_t pulseWidth = 400;                // Microseconds
const uint16_t messageDistance = 23;            // Millis
const uint8_t transmissions = 5;                // Number of repeated transmissions
const uint8_t messageLength = 43;
const uint8_t sequence[messageLength] = { 
  0x02,0xAA,0xAA,0xAA,  // Preamble of 26 bits by repeating 1010
  0x2B,                 // Sync byte
  0x2C,0xCB,0x33,0x33,0x2D,0x34,0xB5,0x2B,0x4D,0x32,0xAD,0x2C,0x56,0x59,0x96,0x66,
  0x66,0x5A,0x69,0x6A,0x56,0x9A,0x65,0x5A,0x58,0xAC,0xB3,0x2C,0xCC,0xCC,0xB4,0xD2,
  0xD4,0xAD,0x34,0xCA,0xB4,0xA0};
```

Analysis of old raw/dirty signals for cleaning (playback from Evil Crow):

![URH_BPO_1](https://user-images.githubusercontent.com/57457139/182748473-c4aefae5-fb07-4a5a-a091-c59a0f7a7faa.jpg)
![URH_BPO_2](https://user-images.githubusercontent.com/57457139/182748483-703fb134-ab64-4b94-9f8b-30d52ab8357d.jpg)