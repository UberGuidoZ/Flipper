$port = New-Object System.IO.Ports.SerialPort
$port.PortName = "COM10"
$port.BaudRate = "115200"
$port.Parity = "None"
$port.DataBits = 8
$port.StopBits = 1
$port.ReadTimeout = 9000 # 9 seconds
$port.DTREnable = $True
$port.open() #opens serial connection

$SPATH="/ext/apps_data/testfile.txt"
$OPATH="C:\users\$env:username\Desktop\outfile.txt"

echo "attempting to read file"
Start-Sleep -m 500  # wait for CLI

$port.DiscardInBuffer()

$port.Write("storage read $SPATH `r`n") 

Start-Sleep -m 500

$outfile = $port.ReadExisting()

# formatting output file

# Removes first two leading blank lines, trailing garbage at the end
$outfileformat = $outfile.split([string[]]"`r`n", [StringSplitOptions]::None)
$outfileformat[2..($outfileformat.length -3)] | Out-File $OPATH

echo "done"

$port.close()

