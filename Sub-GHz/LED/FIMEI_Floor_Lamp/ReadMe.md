# FIMEI floor lamp (3 independently controlled lights, 3000K-6000K)

FIMEI LED floor lamp with a main light, rotating lights around it and a small reading light, sold on Amazon as
"FIMEI Lámpara de Pie, con 3 Luces Control Independientes" (ASIN B0C9ZBT1YQ). Its remote is 433.92 MHz OOK, not infrared.

- Protocol: Princeton, 24 bit, TE ≈ 263 µs (long pulse 3×TE, gap ≈ 31×TE). Remote address `0x67AE`, last byte = button.
- The lamp remembers the last selected light; brightness and colour temperature buttons only affect that light.
- Each brightness/temperature press changes very little (dimming is continuous); send several in a row.

| File | Code | Button |
|---|---|---|
| `Power.sub` | `0x67AE02` | Power on/off (toggle) |
| `Brightness_Up.sub` | `0x67AE06` | Brightness + (selected light) |
| `Brightness_Down.sub` | `0x67AE03` | Brightness - (selected light) |
| `Warmer.sub` | `0x67AE04` | Colour temperature warmer (selected light) |
| `Cooler.sub` | `0x67AE0E` | Colour temperature cooler (selected light) |
| `Max_3000K.sub` | `0x67AE01` | Full brightness at 3000K |
| `Max_4000K.sub` | `0x67AE08` | Full brightness at 4000K (centre button) |
| `Max_6000K.sub` | `0x67AE0F` | Full brightness at 6000K |
| `Select_Main_Light.sub` | `0x67AE09` | Select the main (big) light |
| `Select_Rotating_Lights.sub` | `0x67AE05` | Select the rotating lights around the main light |
| `Select_Reading_Light.sub` | `0x67AE0A` | Select the small reading/night light |
| `Timer_1H.sub` | `0x67AE0C` | 1 hour off timer (not tested) |

Captured with a Broadlink RM4 pro and decoded by hand. All buttons except `Timer_1H` were tested on the real lamp by replaying them
from the Broadlink; the `.sub` files were generated from the decoded codes. Re-synthesising the Princeton frames from these values (TE 263 µs) and sending them from the Broadlink also worked (checked with Warmer/Cooler); not yet replayed from an actual Flipper.
