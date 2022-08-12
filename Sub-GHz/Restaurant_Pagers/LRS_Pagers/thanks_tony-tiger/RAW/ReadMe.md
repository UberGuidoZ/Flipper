

# Basic command for HackRF transmit LRS pagers.
# Avaiable raw binary pager files will ring all pagers on designated restaurant ID setting.

# Example ring all pagers on restaurant ID 1

/usr/bin/hackrf_transfer -s 2000000 -t pager_restid_1.raw -f 467750000 -x 40

