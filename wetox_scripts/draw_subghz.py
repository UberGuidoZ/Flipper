# Script that imports a FFF subghz file and draws a timing graph

# Example line:
# RAW_Data: 379 -798 807 -404 377 -798 805 -802 405 -396 393 -396 397 -394
# Where the numbers are the timings between changes from 0 to 1 and vice versa
# Positive numbers are rising edges, negative numbers are falling edges

# Setup dependencies
import matplotlib.pyplot as plt

# Setup arguments
import argparse
parser = argparse.ArgumentParser(description='Draw a subghz file')
parser.add_argument('-f', '--file', help='Subghz file to draw', required=True)
args = parser.parse_args()

# Read in the file
with open(args.file, 'r') as f:
    lines = f.readlines()

# Trim the lines
data = []
for line in lines:
  if "RAW_Data:" in line:
    data.append(line.split("RAW_Data: ")[1].strip())

# Split the data into a list of individual timings
timings = []
for line in data:
  timings.extend((int(x) for x in line.split()))

# Convert timings to a stream of 0s and 1s
stream = []
for timing in timings:
  if timing > 0:
    for i in range(timing):
      stream.append(1)
  else:
    for i in range(abs(timing)):
      stream.append(0)

# Draw the graph
plt.plot(stream)

#Show the graph
plt.show()

# End of script




