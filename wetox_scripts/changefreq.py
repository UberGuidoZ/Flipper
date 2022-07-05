# script to change the frequency of the Flipper SubGhz file

# sample: python changefreq.py -f tesla.sub -o tesla_new.sub -r 433.92

import os
import argparse

# parse input options:
parser = argparse.ArgumentParser(description="Change the frequency of the SubGhz FFF file.")
parser.add_argument("-f", "--file", help="input file")
parser.add_argument("-o", "--outfile", help="output file")
parser.add_argument("-r", "--newfreq", help="new frequency, can be decimal or whole: 433.92, 315, etc.")
args = parser.parse_args()

# check if all arguments present
if args.file is None or args.outfile is None or args.newfreq is None:
    parser.print_help()
    exit()

# check if input file exists
if not os.path.isfile(args.file):
    print("File not found:", args.file)
    exit()

# convert the fractional frequency to the whole number
# example: 433.92 becomes 433920000
if "." in args.newfreq:
  freqs = args.newfreq.split(".")
  newfreq = freqs[0] + freqs[1] + ("0" * (6 - len(freqs[1])))
else:
  newfreq = args.newfreq + ("0" * 6)

# open file
f = open(args.file, 'r')

# read file and copy it to the new one
# if the line contains the frequency, replace it with the correct one
fout = open(args.outfile, 'w')

for line in f:
    if 'Frequency' in line:
        fout.write('Frequency: %s\n' % newfreq)
    else:
        fout.write(line)

# close files
f.close()
fout.close()

print("Successfully wrote file", args.outfile)
