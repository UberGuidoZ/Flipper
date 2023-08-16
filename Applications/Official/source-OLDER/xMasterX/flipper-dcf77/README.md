# Flipper DCF77
Sends the DCF77 time signal (badly) on the 125khz LFRFID antenna. This should allow you to update [radio clocks](https://en.wikipedia.org/wiki/Radio_clock). You can send it at the original 77.5kHz, but I've had better range using 155kHz. The baseband signal is output on C3.

Building a portable antenna for the LF/VLF band is left as an exercise for the reader.

# technical details

* It works on every clock I own _eventually_. DCF77 is slow, it sends a complete update once per minute. Sometimes it works on the first try, sometimes I have to wait more than 5 attempts
* Implementation is using a software kernel timer (`furi_timer_start`) to generate the signal
* Implementation is wonky and although FZ is FreeRTOS, this software timer is hit and miss. 30Hz seems to work best for generating the baseband signal. More interrupts seem to overlap and are significantly delayed. Less interrupts are sometimes (still!) delayed or missed, but less interrupts means the baseband signal gets distorted more during a miss
* The baseband signal encodes 0 as an 800ms mark, 1 as a 900ms mark and second 59 of each minute is marked with the carrier being on continuously - this is the ASK modulation. There's also a PSK modulation which FZ is not doing. 
* The transmitter is not off between marks, but is still transmitting at reduced power. This is rarely visible outside Germany
* The antenna is highly mistuned for this purpose (sending 77500Hz on a 125000Hz aerial is about 33% off). You can try the 155k harmonic, which is only 20% off. I have seen working attempts using speaker generated EMF at 15500 Hz.
* Combine the slow signal, wonky software timer and very mistuned antenna and YMMV

# todo

* menus! make it more flipper!
* configurable simulated data (encoding the time `25:69` is possible)
* simulate it just as a timezone offset (for changing clocks around your house according to your country's choice of DST madness)
* RBU, ALS162, WWVB, other time signals

