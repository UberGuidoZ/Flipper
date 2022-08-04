# Documenting adventures with the GPIO...

Official [GPIO layout](https://miro.com/app/board/uXjVO_LaYYI=/) created by [Aleksandr Kutuzov](https://discord.com/channels/740930220399525928/765282833744265246/993107060172542012).

# Sentry Safe plugin pinout

Plugin by [H4ckd4ddy](https://github.com/H4ckd4ddy/flipperzero-sentry-safe-plugin)

(Flipper) 8/GND -> Black wire (Safe)<br>
(Flipper) 15/C1 -> Green wire (Safe)

# Mousejack / NRF24 pinout

Plugin by [mothball187](https://github.com/mothball187/flipperzero-nrf24)

More info from [RogueMaster](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/tree/unleashed/applications/mousejacker) and [Unleashed](https://github.com/Eng1n33r/flipperzero-firmware/blob/dev/documentation/NRF24.md). (Both include the plugin in their firmware releases.)

2/A7 on FZ goes to MOSI/6 on nrf24l01<br>
3/A6 on FZ goes to MISO/7 on nrf24l01<br>
4/A4 on FZ goes to CSN/4 on nrf24l01<br>
5/B3 on FZ goes to SCK/5 on nrf24l01<br>
6/B2 on FZ goes to CE/3 on nrf24l01<br>
8/GND on FZ goes to GND/1 on nrf24l01<br>
9/3V3 on FZ goes to VCC/2 on nrf24l01<br>
IRQ/8 is left disconnected on nrf24l01

![NRF_Pins](https://user-images.githubusercontent.com/57457139/178093717-39effd5c-ebe2-4253-b13c-70517d7902f9.png)

If the nRF model is acting a bit flakey, try adding a capacitor to the vcc/gnd lines! I've not tried the Plus model so it may have a bigger need for a cap. Otherwise, I haven't had any major issues. Anything from a 3.3 uF to 10 uF should do. (Watch your positive/negative placement! Negative to ground.) I learned if you wanna get fancy, include a 0.1 uF cap in parallel. The 3.3 uF to 10 uF will respond to slow freq changes while the 0.1 uF will respond to the high freq switching spikes that the larger one cannot. That said, a single 10 uF will likely suffice for the Mousejack attack. ¯\\\_(ツ)_/¯

![NRF_Capacitor](https://user-images.githubusercontent.com/57457139/178169959-d030f9a6-d2ac-46af-af8b-470ff092c8a7.jpg)
![Cap_parrallel](https://user-images.githubusercontent.com/57457139/178372198-84125694-3b7f-4d31-8343-6f892a5e683c.jpg)

# ESP32-WROOM

![ESP32-WROOM](https://user-images.githubusercontent.com/57457139/182290985-54d00e1e-dd3f-4efd-a66f-4d6b8e8c2ed2.jpg)

NOTE: Use `old_hardware.bin` if flashing Marauder onto the ESP32-WROOM (thanks wyldgoat!)

![wyldgoat](https://user-images.githubusercontent.com/57457139/182552144-46abf993-160e-42e2-8dde-466da146b16d.png)<br>
![bobo_dudu](https://user-images.githubusercontent.com/57457139/182552153-9c50f817-8b8c-4d31-8d62-3fe8fca7a48b.png)

# ESP8266

![ESP8266](https://user-images.githubusercontent.com/57457139/182291241-8728052d-aa33-46b0-8e3d-fe44bbda4b6b.jpg)

## ESP32 / ESP32-WROOM / ESP8266 Pinout to Flipper Zero
(Thanks [E_Surge](https://github.com/ESurge) and wyldgoat for guidance!)

ESP32: 3 (RX0) -> FZ: 13 or 15 (these are TX pins)<br>
ESP32: 1 (TX0) -> FZ: 14 or 16 (these are RX pins)<br>
ESP32: GND -> FZ: 8 or 11 or 18 (GND)<br>
ESP32: 3.3V -> FZ: 9 (3.3V) ***OR*** ESP32: 5V -> FZ: 1 (5V)

Switch between UART Pins 13/14 and 15/16 setting in FlipperZero<br>
`(GPIO -> USB-UART Bridge -> LEFT -> UART Pins)`

# ESP32-CAM (Camera)

![ESP32-CAM](https://user-images.githubusercontent.com/57457139/182571081-81df66a8-a536-426f-9ee0-ee277da5ef0a.png)

## ESP32-CAM Pinout to Flipper Zero

CAM32: 1 (RX0) -> FZ: 13 or 15 (these are TX pins)<br>
CAM32: 2 (TX0) -> FZ: 14 or 16 (these are RX pins)<br>
CAM32: 3 (GND) -> FZ: 8 or 11 or 18 (GND)<br>
CAM32: 4 (3.3V) -> FZ: 9 (3.3V) ***OR*** ESP32: 5V -> FZ: 1 (5V)

# LoRa ESP32 Wifi (Wifi + Bluetooth + LoRa 433 MHz)

![WIFI_LoRa_ESP32 to F0](https://user-images.githubusercontent.com/57457139/182568878-aef3ff21-2748-4ba3-9cdd-bc2f5d33b26d.jpg)

## LoRa ESP32 Wifi Pinout to Flipper Zero (PENDING VERIFICATION)
LoRa32: 1 (RX0) -> FZ: 13 or 15 (these are TX pins)<br>
LoRa32: 2 (TX0) -> FZ: 14 or 16 (these are RX pins)<br>
LoRa32: 3 (GND) -> FZ: 8 or 11 or 18 (GND)<br>
LoRa32: 4 (3.3V) -> FZ: 9 (3.3V)

Switch between UART Pins 13/14 and 15/16 setting in FlipperZero<br>
`GPIO -> USB-UART Bridge -> LEFT -> UART Pins`

# Some other accessories to try with Flipper (Code needed!)

## HC-SR04 Ultrasonic Distance Sensor

![HC-SR04](https://user-images.githubusercontent.com/57457139/182779625-f3d43727-c9a6-4c44-8916-3150cccb0fc8.jpg)

```
HC-SR04: 1 (RX0) -> FZ: 13 or 15 (these are TX pins)
HC-SR04: 2 (TX0) -> FZ: 14 or 16 (these are RX pins)
HC-SR04: 3 (GND) -> FZ: 8 or 11 or 18 (GND)
HC-SR04: 4 (5V)  -> FZ: 1 (5V)

Switch between UART Pins 13/14 and 15/16 setting in:
Flipper -> GPIO -> USB-UART Bridge -> LEFT -> UART Pins
```
