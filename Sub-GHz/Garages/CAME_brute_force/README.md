# Flipper Zero Sub Files To Brute-Force CAME 12bit Gate.

## About the Project

**433.92Mhz/CAMEbruteforcer433.py**: will generate sub files which have all the possible keys combination for CAME gate (12bit code/433.92 Mhz), the code will generate multiple files splitted by user choice (500 keys in a file, 1000... etc).

**868.35Mhz/CAMEbruteforcer868.py**: will generate sub files which have all the possible keys combination for CAME gate (12bit code/868.35 Mhz), the code will generate multiple files splitted by user choice (500 keys in a file, 1000... etc).

**CAMEall.sub**: This contains all possible keys (4096), it needs less than 5 minutes to run completely (287 seconds), using this file will open the gate eventually, if you feel the need to know the exact key or shorten this process check the instructions below. I also attached the whole set of files splitted according to the number of keys.

## Changelog 

**12-Aug-2022** : -Time reduced! the whole script now complete in ~4 minutes.
			-Folder created automatically

## Installation

Tested for **Python 3.10.0+**

```bash
$ git clone https://github.com/BitcoinRaven/CAMEbruteforcer.git
$ cd CAMEbruteforcer
```

## Usage

Just run the script and it will generate all the required files.
```bash
$ python CAMEbruteforcer.py
```

## Practical Guide for Brute-Force attack using sub files.

This process is the best I can get with the current flipper firmware.

1. Start with the 1000 keys file, each file will take around 1 minute. (for example the gate opened on file "1000-1999.sub")
2. Go to the next set of files with 500 keys files, you need to run two files now (in our example we need to run  "1000-1499.sub" and "1500-1999.sub"), lets assume    it opened with "1500-1999.sub" file.
3. Go to the next set of files with 100 keys files, you need to run five files now, each file will take 5 seconds to run.
4. if you feel you really need to know the exact code, generate less files with the script (50, 10...) and continue the same process.


# Folder system and Timing

| 2000 | From | To | Time |
| --- | :---: | :---: | :---: |
| 0 | 0 | 1999 | ~2 min |
| 1 | 2000 | 3999 | ~2 min |
| 2 | 4000 | 4096 | ~5 sec |

| 1000 | From | To | Time |
| --- | :---: | :---: | :---: |
| 0 | 0 | 999 | ~1 min |
| 1 | 1000 | 1999 | ~1 min |
| 2 | 2000 | 1999 | ~1 min |
| 3 | 3000 | 3999 | ~1 min |
| 4 | 4000 | 4096 | ~5 sec |

| 500 | From | To | Time |
| --- | :---: | :---: | :---: |
| 0 | 0 | 499 | ~27 sec |
| 1 | 500 | 999 | ~27 sec |
| 2 | 1000 | 1499 | ~27 sec |
| 3 | 1500 | 1999 | ~27 sec |
| 4 | 2000 | 2499 | ~27 sec |
| 5 | 2500 | 2999 | ~27 sec |
| 6 | 3000 | 3499 | ~27 sec |
| 7 | 3500 | 3999 | ~27 sec |
| 8 | 4000 | 4096 | ~5 sec |

# Thanks
[@tobiabocchi](https://github.com/tobiabocchi/flipperzero-bruteforce#optimization) for the updated code and great ideas! check his codes for more protocols and explanation for the recent optimization

