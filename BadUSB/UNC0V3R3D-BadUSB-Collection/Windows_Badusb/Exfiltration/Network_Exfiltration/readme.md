
# Exfiltrate Network
This script will exfiltrate the network and uplaod the report to an ftp server.

## How to use?

This script is not plug and play. You need to do the following changes:

- change the two numbers 1-225 into a number from 1-225 ;)
- "STRING $ScanResults = 1-225..1-225 | ForEach-Object {($Ping).Send($IpAddr + $_) } | Where-Object {$_.Status -eq 'Success'} | select Address "
- Would be -->  $ScanResults = 1..169 (for example)


## Features

- open powershell 
- exfiltrate network
- upload report to server



## Feedback

If you have any feedback, please reach out to me via Discord "UNC0V3R3D#8662".






## Support

For support, contact me via  Discord "UNC0V3R3D#8662".


## Meta


- If you want to sponsor me on Patreon, the link is on my profile.


