from rflib import *

def calculate_crc( pre, sink_word, rest_id, station_id, pager_n, alert_type ):

    l = re.findall('..', pre + sink_word + rest_id + station_id +  pager_n + '0000000000' + alert_type  )

    bin_array = []
    for c in l:
         bin_array.append ( (format( int(c, 16) , '08b')))

    sum=0
    for b in bin_array:
         sum +=  int(b , 2)

    foo='{0}{1}{2}{3}{4}{5}{6}{7}'.format( pre, sink_word, rest_id, station_id, pager_n, '0000000000', alert_type, format( ( sum % 255), '02x' ))
    print("d.RFxmit(bytes.fromhex('"+foo+"'))")
    return foo


d = RfCat()
d.setMdmModulation(MOD_2FSK)
d.setFreq(467750000)
d.setMdmSyncMode(0)
d.setMdmDeviatn(5000)
d.setMdmDRate(625)
d.setMaxPower()
d.setModeIDLE()
d.setEnableMdmManchester(1)
for rest_id in range(0,255):
    crc_out = ( calculate_crc( format(11184810, '06x') , format( 64557,'04x'), format(rest_id, '02x'), '0', format( 0  ,'03x' ), format(1, '02x') ) )

    d.RFxmit(bytes.fromhex(crc_out))
    d.setModeIDLE()
