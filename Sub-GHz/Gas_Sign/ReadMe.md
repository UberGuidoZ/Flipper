# Gas Station Price Sign

## Captures by [0day](https://github.com/0dayCTF) (from [Discord](https://discord.com/channels/740930220399525928/954422680969445377/1001983366205284452))

Captured remote is [GL-OIL-RF](https://olympianled.com/product/gas-price-changer-rf-remote-control-gl-oil-rf/). The [PDF manual](https://github.com/UberGuidoZ/Flipper/blob/main/Sub-GHz/Gas_Sign/Manual_GL-OIL-RF.pdf) is also available and contains programming instructions.

More work on this to be done! These captures are locked to the 20110120 code but can be [physically changed](https://olympianled.com/changing-remote-code/):

```
Our remote code is 20110120. As you can see from the pictures below: 

L1 = 2

No solder = 0

1H = 1

The receiver inside of your sign needs to match the remote code, so you’ll have to look at the receiver
inside of the sign that is attached to the GL_GMAN board. Here is a picture of the receiver for reference,
you can see it’s also matching our 20110120 code.
```
Receiver

![Remote_Receiver](https://user-images.githubusercontent.com/57457139/182993141-0f2a725b-036a-4b15-b38b-15c7e4177735.png)

Remote

![Remote_Back](https://user-images.githubusercontent.com/57457139/182993143-c4e261c7-c986-4a11-b17d-ed394d3953ba.png)
