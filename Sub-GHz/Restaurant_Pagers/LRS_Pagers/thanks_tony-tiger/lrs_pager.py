#!/usr/bin/python

"""
Written by: Tony Tiger 6/2019

This program generates manchester encoded data packets for LRS pagers and GNU Radio.

Output file name: pager.bin

Watch the YouTube video for more information: https://www.youtube.com/watch?v=ycLLb4eVZpI

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

import sys
import re
import struct


def encode_manchester( bin_list ):

      pre = []  # create extra preambles to wake up the pager
      for x in range(0,50): 
            pre.append('1')
            pre.append('0')

      l = re.findall('.', "".join( pre + bin_list  ) )  # join the preamble and the rest of the packet

      m = []
      print '\n'
      print "".join(str(x) for x in l)  # convert list to string

      for x in l:   # convert to manchaster coding
           if( x == '0'):
               m.append(1)
               m.append(0)

           if( x == '1'):
               m.append(0)
               m.append(1)
      return m


# calculate the crc
def calculate_crc( pre, sink_word, rest_id, station_id, pager_n, alert_type ):

    l = re.findall('..', pre + sink_word + rest_id + station_id +  pager_n + '0000000000' + alert_type  )

    bin_array = []
    for c in l:
         bin_array.append ( (format( int(c, 16) , '08b')))

    sum=0
    for b in bin_array:
         sum +=  int(b , 2)

    print '\n{0} {1} {2} {3} {4} {5} {6} {7}'.format( pre, sink_word, rest_id, station_id, pager_n, '0000000000', alert_type, format( ( sum % 255), '02x' ))
    bin_array.append( format( ( sum % 255), '08b') )
    return bin_array




##########################################
# main program start                     #
##########################################

try:
    rest_id=int(raw_input('\nEnter restaurant id 0-255: '))
except ValueError:
    print "Not a number"

try:
    pagers=(raw_input('Enter one or more pager numbers 0-1023 : '))
except ValueError:
    print "Not a number"

pager_list = []
pager_list = map( int, re.split('\s+',pagers))

print '1 Flash 30 Seconds\n2 Flash 5 Minutes\n3 Flash/Beep 5X5\n4 Beep 3 Times\n5 Beep 5 Minutes\n6 Glow 5 Minutes\n7 Glow/Vib 15 Times\n10 Flash/Vib 1 Second\n68 beep 3 times\n'


try:
    alert_type=int(raw_input('Enter alert type: '))
except ValueError:
    print "Not a number"


handle = open('pager.bin', 'wb')

data = []
for pager_n in pager_list:
    crc_out = ( calculate_crc( format(11184810, '06x') , format( 64557,'04x'), format(rest_id, '02x'), '0', format( pager_n  ,'03x' ), format(alert_type, '02x') ) )

    data = encode_manchester( crc_out )
    [ data.append(0) for x in range(0,100) ]

    print '\n';
    print "".join(str(x) for x in data)
    print '\n'

    for d in data:
        if d == 0:
            handle.write(struct.pack('f', .0001)) 
        elif d == 1:
            handle.write(struct.pack('f', 1)) 
        else:
            print "Error detected in data"
            sys.exit()

handle.close()    

