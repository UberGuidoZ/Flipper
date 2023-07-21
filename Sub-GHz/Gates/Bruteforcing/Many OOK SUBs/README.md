# Everything in here copied directly from [tobiabocchi](https://github.com/tobiabocchi/flipperzero-bruteforce)!

<h1 align="center">
  Brute Force OOK using <a href="https://flipperzero.one">Flipper Zero</a>
</h1>

<p align="center">
  <img src="https://user-images.githubusercontent.com/29007647/182851959-afaa1367-9f4d-46c8-92af-aa5ff70fca64.png" />
</p>

Brute force subghz fixed code protocols using flipper zero, initially inspired by [CAMEbruteforcer](https://github.com/BitcoinRaven/CAMEbruteforcer)

This repo aims to collect as many brute force files/protocols as possible, so if you can or want to contribute you are more than welcome to do so!

## How it works

Using `flipperzero-bruteforce.py` you can generate bruteforce `.sub` files for subghz protocols that use fixed OOK codes. Inside the script it is also possible to specify your own protocol in case it's not present.

To generate all the files simply run:

```bash
python3 flipperzero-bruteforce.py
```

It will generate bruteforce files for all the specified protocols organized in many folders with the following structure:

```
sub_files/
└── PROTOCOL_NAME
    ├── SPLIT_FACTOR
    │   ├── <parent_file>_000.sub
    │   ├── <parent_file>_001.sub
    │   ├── ...
    │   └── <parent_file>_NNN.sub
    └── debruijn.sub
```

For each protocol there are 6 sub folders, containing 1, 2, 4, 8, 16 and 32 files, `SPLIT_FACTOR` (the directory's name) indicates the number of keys per `.sub` file. `<parent_file>` simply indicates the parent file of the current `.sub` file, for example, inside folder `64` we have `003_006.sub`, its parent file is `128/<parent_file>_003` and its children will be `32/006_<file_id>`. This is useful when trying to get a close guess to the key.

For example, let's say you are trying to bruteforce a gate using CAME 12 bit protocol:

1. Play the single file (`4096/` folder) to make sure the attack works
2. Play the two files inside `2048/` folder, to see which half contains the correct key (suppose the second one works, `000_001.sub` containing keys from 2048 4095)
3. Play its children (`001_002.sub`, `001_003.sub`) inside the `1024/` folder to narrow the search
4. Keep doing this until you reach the last files inside the `128/` folder, these files take less than 10 seconds to send, almost the same as having the actual remote.

If you wanted to narrow the search even more you could modify the script to generate your own files containing less keys.

The `debruijn.sub` files are there just for reference I have not really been able to use them succesfully, they try to simulate a debruijn sequence for the protocol, although this is quite unfeasible because most protocols have some sort of stop bit or pilot bit appended or prepended to the actual key that differs from a normal bit. Also I have not yet found a gate using a bitshift register.

## Currently supported protocols

| Protocol ℹ️ | Frequency 📡 | Bits | Timing ⏱  | Testing 🧪 |
| ----------- | ------------ | ---- | --------- | ---------- |
| CAME        | 433.92       | 12   | 287 sec   | Tested     |
| CAME        | 868.35       | 12   | 287 sec   | Untested   |
| CAME-fast   | 433.92       | 12   | 224 sec   | Tested     |
| CAME-fast   | 868.35       | 12   | 224 sec   | Untested   |
| Chamberlain | 300          | 9    | 123 sec   | Untested   |
| Chamberlain | 315          | 9    | 123 sec   | Untested   |
| Linear      | 300          | 10   | 212 sec   | Tested     |
| Linear      | 310          | 10   | 212 sec   | Untested   |
| NICE        | 433.92       | 12   | 628 sec   | Untested   |
| NICE        | 868.35       | 12   | 628 sec   | Untested   |
| PT-2240     | 433.92       | 24   | 800 hours | Untested   |
| Ansonic     | 434.075      | 12   | 276 sec   | Untested   |
| Holtek      | 315          | 12   | 387 sec   | Untested   |
| Holtek      | 433.92       | 12   | 387 sec   | Untested   |
| Holtek      | 868.35       | 12   | 387 sec   | Untested   |
| Holtek      | 915          | 12   | 387 sec   | Untested   |

### Currently supported Pagers

Pagers for the Italian restaurant "Spacca", listed as _Spacca_pager_

### Adding a protocol

Adding a protocol is very straight forward, inside the script protocols are defined at the bottom, inside the protocol list:

```python
protocols = [
    Protocol("CAME", 12, {"0": "-320 640 ", "1": "-640 320 "}, "-11520 320 "),
    Protocol("NICE", 12, {"0": "-700 1400 ", "1": "-1400 700 "}, "-25200 700 "),
    Protocol("8bit", 8,  {"0": "200 -400 ", "1": "400 -200 "}),  # generic 8 bit protocol
    ...
]
```

A protocol is defined by a few parameters passed to the constructor in the following order:

- name: the name of the protocol
- n_bits: the number of bits for a single key
- transposition_table: how 0s and 1s are translated into flipper subghz `.sub` language
- pilot_period: aka preamble, a recurring pattern at the beginning of each key, inexistant by default
- stop_bit: a recurring pattern at the end of each key, inexistant by default
- frequency: working frequency, defaults to 433.92
- repetition: number of transmissions per key in the bruteforce

### Optimization

After a little testing working with CAME 12 bit (together with [@BitcoinRaven](https://github.com/BitcoinRaven)), it seems that as long as the **ratio** between the bits' lengths is respected (`x`: short pulse, `2x`: long pulse, `36x`: pilot period) the actual duration of x can be lowered from the original 320 microseconds. From testing it seems that 250 microseconds is stable, shortening the bruteforce by a good minute (224 seconds against 287 seconds). From a code point of view:

```python
protocols = [
    # Old CAME
    Protocol("CAME", 12, {"0": "-320 640 ", "1": "-640 320 "}, "-11520 320 "),
    # New CAME
    Protocol("CAME", 12, {"0": "-250 500 ", "1": "-500 250 "}, "-9000 250 "),
]
```

Other attempts we made to shorten the bruteforce were:

- Combining two 12 bit codes &rarr; ❌
- Removing/adding 1Bit from/to the end/beginning &rarr; ❌
- Sending only the first pilot signal &rarr; ❌
- Removing the pilot signal between the repetion &rarr; ❌
- Remove half a bit from the end &rarr; ❌
- Reducing pulse length keeping the same ratio as the original &rarr; ✅

Further testing will be performed for the other protocols.

# Timing

To compute the time it takes to perform a bruteforce attack, we need to sum the time it takes to send each code:

```
(pilot_period + n_bits * bit_period) * repetition * 2^n_bits
```

For example, computing this for CAME turns out to be:

```
[(9000 + 250) + 12 * (250 + 500)] * 3 * 2^12 = 224.256.000 microseconds ~ 224 seconds
```
