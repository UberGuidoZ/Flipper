# Flipper Zero Sub Files To Brute-Force CAME 12bit Gate.

## About the Project

**CAMEbruteforcer.py**: will generate sub files which have all the possible keys combination for CAME gate (12bit code), the code will generate multiple files splitted by user choice( each 500 keys in a file, 1000 ....etc). 

**CAMEall.sub**: This contains all possible keys (4096), it needs 16 minutes to run completely, using this file will open the gate eventually, if you feel the need to know the exact key or shorten this process check the instructions below. I also attached the whole set of files splitted according to the number of keys. 

## Installation

Tested for **Python 3.10.0+**

```bash
$ git clone https://github.com/BitcoinRaven/CAMEbruteforcer.git
$ cd CAMEbruteforcer
```

## Usage

Change "split = 1000" to whatever number of keys you wish to have in each file. 
```bash
$ python CAMEbruteforcer.py
```

## Practical Guide for Brute-Force attack using sub files. 
This process is the best I can get with the current flipper firmware. 

1. Start with the 1000 keys file, each file will take around 4 minutes each. (for example the gate opened on file "1000-1999.sub")
2. Go to the next set of files with 500 keys files, you need to run two files now (in our example we need to run  "1000-1499.sub" and "1500-1999.sub"), lets assume    it opened with "1500-1999.sub" file. 
3. Go to the next set of files with 100 keys files, you need to run five files now, each file will take 25 seconds to run. 
4. if you feel you really need to know the exact code, generate less files with the script (50, 10...) and continue the same process. 


# Folder system and Timing 
 
    2000    From     To     Time 
    ---------------------------------
    1	      0	  	1999	8 min
	2	     2000	3999	8 min
	3	     4000	4096	25 sec
    ---------------------------------

    1000    From     To     Time 
    ---------------------------------
	1	    0	    	999	4 min
	2	    1000	1999	4 min
	3	    2000	2999	4 min
	4	    3000	3999	4 min
	5	    4000	4096	25 sec
    ---------------------------------			
    
    500    From     To     Time 
    ---------------------------------  
    1	    0	    	499	2 min
	2	    500	    	999	2 min
	3	    1000	1499	2 min
	4	    1500	1999	2 min
	5	    2000	2499	2 min
	6	    2500	2999	2 min
	7	    3000	3499	2 min
	8	    3500	3999	2 min
	9	    4000	4096	25 sec
    ---------------------------------


