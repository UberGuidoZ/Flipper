sub file for a Westek pair of Wireless Plug-in Receiver (Model 805) and Wall Switch Transmitter (RFA108), marked with a Channel Code B sticker.

I had difficulty getting a clear reading of this signal.  

FCC documentation, page 14 of 16 indicates it's operating frequency is 305Mhz.
https://apps.fcc.gov/eas/GetApplicationAttachment.html?id=1031148

The Flipper was able to get several reads at different frequencies (and annoyingly, the Frequency Analyzer really only detects anything at 915Mhz, which must be a harmonic).  

So, here was the way to get this to work.  Get a successful reading on any frequency that it picks up on. You're looking for a Holtek_HT12X 12b (not CAME).  Save it as a sub.  This switch has two buttons, but they send out the same button/key code, so you only need one sample.

Open the sub on your computer and change the frequency to Frequency: 305000000

Save the sub and place back on the Flipper. This worked immediately.

The sub attached is from the procedure above. Note that this pair operates on Channel Code B. I do not have any other pairs to test.
