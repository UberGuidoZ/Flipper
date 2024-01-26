# Documenting adventures with the GPIO...

Official [GPIO layout](https://miro.com/app/board/uXjVO_LaYYI=/) created by [Aleksandr Kutuzov](https://discord.com/channels/740930220399525928/765282833744265246/993107060172542012). Official documentation [is here](https://docs.flipperzero.one/gpio-and-modules)!<br>

# Connect an external CC1101 to your Flipper

Check out [this excellent guide](https://github.com/quen0n/flipperzero-ext-cc1101) from quen0n on the hardware and configurations needed to increase your SubGHz range!

# Sentry Safe plugin pinout

Plugin by [H4ckd4ddy](https://github.com/H4ckd4ddy/flipperzero-sentry-safe-plugin)

(Flipper) 8/GND -> Black wire (Safe)<br>
(Flipper) 15/C1 -> Green wire (Safe)

# NRF24L01+ / Mousejack pinout - 8-pin

Plugin by [mothball187](https://github.com/mothball187/flipperzero-nrf24)  |  Affected devices from [Bastille research](https://www.bastille.net/research/vulnerabilities/mousejack/affected-devices)

More info from [RogueMaster](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/tree/unleashed/applications/mousejacker) and [Unleashed](https://github.com/Eng1n33r/flipperzero-firmware/blob/dev/documentation/NRF24.md). (Both include the plugin in their firmware releases.)

2/A7 on FZ goes to MOSI/6 on NRF24<br>
3/A6 on FZ goes to MISO/7 on NRF24<br>
4/A4 on FZ goes to CSN/4 on NRF24<br>
5/B3 on FZ goes to SCK/5 on NRF24<br>
6/B2 on FZ goes to CE/3 on NRF24<br>
8/GND on FZ goes to GND/1 on NRF24<br>
9/3V3 on FZ goes to VCC/2 on NRF24<br>
IRQ/8 is left disconnected on NRF24

![NRF_Pins](https://user-images.githubusercontent.com/57457139/178093717-39effd5c-ebe2-4253-b13c-70517d7902f9.png)

If the nRF model is acting a bit flakey, try adding a capacitor to the vcc/gnd lines! I've not tried the Plus model so it may have a bigger need for a cap. Otherwise, I haven't had any major issues. Anything from a 3.3 uF to 10 uF should do. (Watch your positive/negative placement! Negative to ground.) I learned if you wanna get fancy, include a 0.1 uF cap in parallel. The 3.3 uF to 10 uF will respond to slow freq changes while the 0.1 uF will respond to the high freq switching spikes that the larger one cannot. That said, a single 10 uF will likely suffice for the Mousejack attack. ¯\\\_(ツ)_/¯

![NRF_Capacitor](https://user-images.githubusercontent.com/57457139/178169959-d030f9a6-d2ac-46af-af8b-470ff092c8a7.jpg)
![Cap_parrallel](https://user-images.githubusercontent.com/57457139/178372198-84125694-3b7f-4d31-8343-6f892a5e683c.jpg)

# NRF24L01+ (B) - 10-pin

![NRF24L01+_B](https://user-images.githubusercontent.com/57457139/184263193-4f63a352-4e52-400d-8c45-ec4dc6d2ac8b.jpg)

Thank you to [Amec0e](https://github.com/amec0e) for the information on the newer NRF24L01 (B)!

NRF24: 1 (CE)  to FZ 6 (B2)<br>
NRF24: 2 (SCK) to FZ 5 (B3)<br>
NRF24: 3 (CSN) to FZ 4 (A4)<br>
NRF24: 4 (MOSI) to FZ 2 (A7)<br>
NRF24: 6 (MISO) to FZ 3 (A6)<br>
NRF24: 7 or 8 (GND) to FZ GND (8 or 11 or 18)<br>
NRF24: 9 or 10 (VCC) to FZ 9 (3v3)

# ESP32-WROOM

![ESP32-WROOM](https://user-images.githubusercontent.com/57457139/182290985-54d00e1e-dd3f-4efd-a66f-4d6b8e8c2ed2.jpg)

NOTE: Use `old_hardware.bin` if flashing Marauder onto the ESP32-WROOM (thanks wyldgoat!)<br>
[Easy Flasher (Windows / Linux / Mac) available here](https://github.com/UberGuidoZ/Flipper/tree/main/Wifi_DevBoard/FZ_Marauder_Flasher/ESP32-WROOM).

![wyldgoat](https://user-images.githubusercontent.com/57457139/182552144-46abf993-160e-42e2-8dde-466da146b16d.png)<br>
![bobo_dudu](https://user-images.githubusercontent.com/57457139/182552153-9c50f817-8b8c-4d31-8d62-3fe8fca7a48b.png)

## ESP32 / ESP32-WROOM Pinout to Flipper Zero
(Thanks [E_Surge](https://github.com/ESurge) and wyldgoat for guidance!)<br>
NOTE: Make sure you're using at least an ESP32 v2 or above for full functionality! [Good read](https://blog.spacehuhn.com/esp32-deauther).<br>
Running an ESP32 single-core and want Marauder? You've gotta head over to [V0lk3n's write-up](https://github.com/V0lk3n/Flipper-ESP32-Solo-Marauder)!

ESP32: 3 (RX0) -> FZ: 13 or 15 (these are TX pins)<br>
ESP32: 1 (TX0) -> FZ: 14 or 16 (these are RX pins)<br>
ESP32: GND -> FZ: 8 or 11 or 18 (GND)<br>
ESP32: 5V -> FZ: 1 (5V) ***OR*** ESP32: 3.3V -> FZ: 9 (3.3V)<br>
*NOTE: Make sure to enable 5v under GPIO on the Flipper*

Switch between UART Pins 13/14 and 15/16 setting in FlipperZero<br>
`(GPIO -> USB-UART Bridge -> LEFT -> UART Pins)`

# ESP8266

![ESP8266](https://user-images.githubusercontent.com/57457139/182291241-8728052d-aa33-46b0-8e3d-fe44bbda4b6b.jpg)

If you have the ESP8266 and are looking to use a wifi deauth, check out [speexvocon's](https://github.com/Speexvocon/FlipperZeroESPexpansion) easy [web flasher](https://github.com/Speexvocon/FlipperZeroESPexpansion#programming-the-module)!<br>
If you're looking to give another deauther a try, [check out HEX0DAYS repo over here](https://github.com/HEX0DAYS/FlipperZero-PWNDTOOLS).

## ESP8266 Pinout to Flipper Zero
(Thanks PixelHerstel for additional guidance!)

ESP8266: 1 (TX0) -> FZ: 14 or 16 (these are RX pins)<br>
ESP8266: 3 (RX0) -> FZ: 13 or 15 (these are TX pins)<br>
ESP8266: 5 -> FZ: 7<br>
ESP8266: 6 -> FZ: 6<br>
ESP8266: 7 -> FZ: 5<br>
ESP8266: 8 -> FZ: 4<br>
ESP8266: VIN -> FZ: 1<br>
ESP8266: GND -> FZ: 8 or 11 (GND)<br>
FZ: 16  -> FZ: 18 (yes, FZ to FZ)<br>
ESP8266: 5V -> FZ: 1 (5V)<br>
*NOTE: Make sure to enable 5V under GPIO on the Flipper*<br>
***OR*** ESP32: 3.3V -> FZ: 9 (3.3V) though this can have issues...

Switch between UART Pins 13/14 and 15/16 setting in FlipperZero<br>
`(GPIO -> USB-UART Bridge -> LEFT -> UART Pins)`

A visual pinout can be seen in [this PDF](https://github.com/UberGuidoZ/Flipper/blob/main/GPIO/ESP8266_WIFI_Schematic.pdf), too.

# ESP32-S2 WROVER

![ESP32-S2WROVER](https://user-images.githubusercontent.com/57457139/201228294-d5565fff-99ae-44ed-8076-36b0fc8d39c9.jpg)

## ESP32-S2 WROVER Pinout (thanks max_fpv_austria!)

ESP32: 44 (RX0) -> FZ: 13 or 15 (these are TX pins)<br>
ESP32: 43 (TX0) -> FZ: 14 or 16 (these are RX pins)<br>
ESP32: GND -> FZ: 8 or 11 or 18 (GND)<br>
ESP32: 3.3V -> FZ: 9 (3.3V)<br>

Switch between UART Pins 13/14 and 15/16 setting in FlipperZero<br>
`(GPIO -> USB-UART Bridge -> LEFT -> UART Pins)`

Picture includes wiring pinout for the SD card mod too!

# ESP32-CAM (Camera) - [FAP FILE HERE](https://github.com/Z4urce/flipperzero-camera/)!

![ESP32-CAM](https://user-images.githubusercontent.com/57457139/182571081-81df66a8-a536-426f-9ee0-ee277da5ef0a.png)

## ESP32-CAM Pinout to Flipper Zero

CAM32: 1 (RX0) -> FZ: 13 or 15 (these are TX pins)<br>
CAM32: 2 (TX0) -> FZ: 14 or 16 (these are RX pins)<br>
CAM32: 3 (GND) -> FZ: 8 or 11 or 18 (GND)<br>
CAM32: 4 (3.3V) -> FZ: 9 (3.3V) ***OR*** ESP32: 5V -> FZ: 1 (5V)

# LoRa ESP32 Wifi (Wifi + Bluetooth + LoRa 433 MHz)

![WIFI_LoRa_ESP32 to F0](https://user-images.githubusercontent.com/57457139/182568878-aef3ff21-2748-4ba3-9cdd-bc2f5d33b26d.jpg)

## LoRa ESP32 Wifi Pinout to Flipper Zero
LoRa32: 1 (RX0) -> FZ: 13 or 15 (these are TX pins)<br>
LoRa32: 2 (TX0) -> FZ: 14 or 16 (these are RX pins)<br>
LoRa32: 3 (GND) -> FZ: 8 or 11 or 18 (GND)<br>
LoRa32: 4 (3.3V) -> FZ: 9 (3.3V)

Switch between UART Pins 13/14 and 15/16 setting in FlipperZero<br>
`GPIO -> USB-UART Bridge -> LEFT -> UART Pins`

# Raspberry Pi Zero UART

![Rpi_Zero_GPIO](https://user-images.githubusercontent.com/22322762/183289015-45329e9c-83e0-4483-96a0-1b68552bfa3f.png)

## Raspberry Pi Zero : Prerequisite

Mount your SD Card to your computer and add the following line at the end of `/boot/config.txt` file.

`enable_uart=1`

Additionally you can look at the baudrate in the `/boot/cmdline.txt` file, which should be `115200` by default.

## Raspberry Pi Zero : Pinout to Flipper Zero

```
PiZero: 4 (5V) -> FZ: 1 (5V)
PiZero: 6 (GND) -> FZ: 11 (GND)
PiZero: 8 (UART0_TXD) -> FZ: 14 (RX)
PiZero: 10 (UART0_RXD) -> FZ: 13 (TX)
```

## HC-SR04 Ultrasonic Distance Sensor

Plugin by Sanqui: https://github.com/Sanqui/flipperzero-firmware/blob/hc_sr04/applications/hc_sr04/hc_sr04.c

![HC-SR04](https://user-images.githubusercontent.com/57457139/182779625-f3d43727-c9a6-4c44-8916-3150cccb0fc8.jpg)

```
HC-SR04: 1 (RX0) -> FZ: 13 or 15 (these are TX pins)
HC-SR04: 2 (TX0) -> FZ: 14 or 16 (these are RX pins)
HC-SR04: 3 (GND) -> FZ: 8 or 11 or 18 (GND)
HC-SR04: 4 (5V)  -> FZ: 1 (5V)

Switch between UART Pins 13/14 and 15/16 setting in:
Flipper -> GPIO -> USB-UART Bridge -> LEFT -> UART Pins
```
![HC-SR04](https://user-images.githubusercontent.com/57457139/208268156-05021579-5410-44bc-a705-99fc97b0d046.png)

-----

## NMEA 0183 GPS
https://github.com/ezod/flipperzero-gps
```
GPIO pins 9 (3.3V), 11 (GND), 13 (TX), and 14 (RX)
```
![GPS NMEA 0183](https://github.com/ezod/flipperzero-gps/raw/main/wiring.png)

-----

## i2c Tools (by NaejEL) 
```
C0 -> SCL / C1 -> SDA / GND -> GND | 3v3 logic levels only!
```
https://github.com/NaejEL/flipperzero-i2ctools

-----

## HTU21D / SI7021 Temperature Sensor
![HTU21D](https://user-images.githubusercontent.com/57457139/208267976-6709577e-f15d-4ec0-928d-dcff9b3389be.png)
```
GPIO pins 9 (3.3V), 11 (GND), 15 (sda), and 16 (scl)
```
https://github.com/Mywk/FlipperTemperatureSensor
![Temperature Sensor - HTU21D / SI7021](https://github.com/Mywk/FlipperTemperatureSensor/raw/master/images/Flipper.png)

-----

## AM2320/AM2321 Temp. Sensor
```
GPIO pins 9 (3.3V), 11 (GND), 15 (sda), and 16 (scl)
```
https://github.com/xMasterX/AM2320_Flipper_Plugin
![Connection](https://user-images.githubusercontent.com/10697207/199586577-5c9cf516-2096-4d70-9e2f-1f9458a68d65.jpg)

-----

## DHT11/22 Temp. Sensor
```
VCC (none, +, VCC, red wire)	1 (5V) or 9 (3V3)
GND (-, GND, black wire)	8, 18 (GND)
DATA (OUT, S, yellow wire)	2-7, 10, 12-17 (to choose from)
```
https://github.com/quen0n/FipperZero-DHT-Monitor
-----

## UHF RFID YRM100 ([FAP / App here](https://github.com/frux-c/uhf_rfid))
```
YRM100: 1 (VCC) -> FZ: 1 (5V)
YRM100: 2 (RXD) -> FZ: 13 or 15 (these are TX pins)
YRM100: 3 (TXD) -> FZ: 14 or 16 (these are RX pins)
YRM100: 4 (EN)  -> FZ: 9 (3.3V)
YRM100: 5 (GND) -> FZ: 8 or 11 or 18 (GND)

Switch between UART Pins 13/14 and 15/16 setting in:
Flipper -> GPIO -> USB-UART Bridge -> LEFT -> UART Pins

Also make sure to enable 5V in Flipper -> GPIO
```
![Pinout](https://github.com/UberGuidoZ/Flipper/assets/57457139/522764fb-db90-4bc9-b46b-9eb0c15f5d75)
![Schematic](https://github.com/UberGuidoZ/Flipper/assets/57457139/0d685f28-7b80-4657-8ccd-af50da553721)
![Front](https://github.com/UberGuidoZ/Flipper/assets/57457139/9e395d8f-92a9-488d-932a-5c7647da30dd)
![Back](https://github.com/UberGuidoZ/Flipper/assets/57457139/44bfff6f-318a-4677-98fd-3acb6b20f722)

Credit: [RekordNadoev](https://github.com/RekordNadoev) / [Issue 515](https://github.com/UberGuidoZ/Flipper/issues/515)
-----

## ServoTester ([Info/App](https://mhasbini.com/blog/servo-tester-flipper-zero.html))
```
VCC (none, +, VCC, red wire) -> Flipper 1 (5V)
GND (-, GND, black/brown wire) -> Flipper 8 OR 18 (GND)
DATA (OUT, S, yellow wire) -> Flipper 2

Make sure to enable 5V in Flipper -> GPIO
```
![Servo](https://github.com/UberGuidoZ/Flipper/assets/57457139/f9cc7ef8-f97e-4489-9c38-f4403ae681a3)

Credit: [RekordNadoev](https://github.com/RekordNadoev) / [Issue 515](https://github.com/UberGuidoZ/Flipper/issues/515)
-----

# Custom PCB's
## Here's some repo's with designs for custom PCB's you can order or build using a prototype board.

[DrB0rk](https://github.com/DrB0rk/Flipper-Zero-Boards)<br>
[AWOK](https://github.com/AWOK559/Flipper_Zero_Boards)<br>
[Speexvocon](https://github.com/Speexvocon/) ([ESP Expansion](https://github.com/Speexvocon/FlipperZeroESPexpansion) and [Header Extender](https://github.com/Speexvocon/FlipperZeroHeaderExtender))<br>
[ushastoe](https://github.com/krolchonok/flipper-board)<br>
[TheOtherLonestar](https://github.com/TheOtherLonestar/RS-232-UART-Flipper)<br>
[Oleksii Kutuzov](https://github.com/oleksiikutuzov/flipperzero-flasher-board)

-----

## Donation Information

Nothing is ever expected for the hoarding of digital files, creations I have made, or the people I may have helped.

## Ordering from Lab401? [USE THIS LINK FOR 5% OFF!](https://lab401.com/r?id=vsmgoc) (Or code `UberGuidoZ` at checkout.)

I've had so many asking for me to add this.<br>
![Flipper_Blush](https://user-images.githubusercontent.com/57457139/183561666-4424a3cc-679b-4016-a368-24f7e7ad0a88.jpg) ![Flipper_Love](https://user-images.githubusercontent.com/57457139/183561692-381d37bd-264f-4c88-8877-e58d60d9be6e.jpg)

**BTC**: `3AWgaL3FxquakP15ZVDxr8q8xVTc5Q75dS`<br>
**BCH**: `17nWCvf2YPMZ3F3H1seX8T149Z9E3BMKXk`<br>
**ETH**: `0x0f0003fCB0bD9355Ad7B124c30b9F3D860D5E191`<br>
**LTC**: `M8Ujk52U27bkm1ksiWUyteL8b3rRQVMke2`<br>
**PayPal**: `uberguidoz@gmail.com`

So, here it is. All donations of *any* size are humbly appreciated.<br>
![Flipper_Clap](https://user-images.githubusercontent.com/57457139/183561789-2e853ede-8ef7-41e8-a67c-716225177e5d.jpg) ![Flipper_OMG](https://user-images.githubusercontent.com/57457139/183561787-e21bdc1e-b316-4e67-b327-5129503d0313.jpg)

Donations will be used for hardware (and maybe caffeine) to further testing!<br>
![UberGuidoZ](https://cdn.discordapp.com/emojis/1000632669622767686.gif)
