# Harbor Breeze A25-FA0158

FCC ID: [A25-FA0158](https://fccid.io/A25-FA0158/User-Manual/UM-5229516)<br>
IC: 10924A-FA0158

Fan manufacturer: Harbor Breeze<br>
Remote manufacturer: DawnSun Electronic Technology

Tested unit: [Better Homes & Gardens 52" Pearce Black Indoor Ceiling Fan with Light](https://www.walmart.com/ip/Better-Homes-Gardens-52-Pearce-Black-Indoor-Ceiling-Fan-with-Light-Adjustable-Color-Temperature-Reverse-Airflow/1924936716)

Captured on the default DIP switches. Captures were manually cleaned to align with the codes.

Each frame is a 400µs sync pulse followed by 32 bits (MSB first), then a ~14.2ms gap:

| Bit | Off | On |
|-----|-----|----|
| `1` | 400µs | 800µs |
| `0` | 800µs | 400µs |

## Codes

The 32-bit code is a 24-bit remote ID (`C4D803`) followed by an 8-bit button code.

| File | Code | Button |
|------|------|--------|
| `Light_delay.sub` | `C4D80381` | `81` |
| `Fan_off.sub` | `C4D80385` | `85` |
| `Fan_low.sub` | `C4D80386` | `86` |
| `Fan_mid.sub` | `C4D80387` | `87` |
| `Fan_high.sub` | `C4D80388` | `88` |
| `Fan_random.sub` | `C4D8038C` | `8C` |
| `Light.sub` | `C4D8038E` | `8E` |
| `Light_random.sub` | `C4D80392` | `92` |

