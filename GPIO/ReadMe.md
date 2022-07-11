Starting to document adventures with the GPIO. Official [GPIO layout](https://miro.com/app/board/uXjVO_LaYYI=/) created by [Aleksandr Kutuzov](https://discord.com/channels/740930220399525928/765282833744265246/993107060172542012).

# Sentry Safe plugin pinout<br>
(Flipper) 8/GND -> Black wire (Safe)<br>
(Flipper) 15/C1 -> Green wire (Safe)

# Mousejack / NRF24 pinout

[More info on RogueMaster's repo](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/tree/unleashed/applications/mousejacker). (Currently the only pre-compiled DFU with this included!)

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
