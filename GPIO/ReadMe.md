Starting to document adventures with the GPIO. Official [GPIO layout](https://miro.com/app/board/uXjVO_LaYYI=/) created by [Aleksandr Kutuzov](https://discord.com/channels/740930220399525928/765282833744265246/993107060172542012).

# Sentry Safe plugin pinout<br>
(Flipper) 8/GND -> Black wire (Safe)<br>
(Flipper) 15/C1 -> Green wire (Safe)

# Mousejack / NRF24 pinout

2/A7 on FZ goes to MOSI/6 on nrf24l01<br>
3/A6 on FZ goes to MISO/7 on nrf24l01<br>
4/A4 on FZ goes to CSN/4 on nrf24l01<br>
5/B3 on FZ goes to SCK/5 on nrf24l01<br>
6/B2 on FZ goes to CE/3 on nrf24l01<br>
8/GND on FZ goes to GND/1 on nrf24l01<br>
9/3V3 on FZ goes to VCC/2 on nrf24l01<br>
IRQ/8 is left disconnected on nrf24l01<br>

![NRF_Pins](https://user-images.githubusercontent.com/57457139/178093717-39effd5c-ebe2-4253-b13c-70517d7902f9.png)
