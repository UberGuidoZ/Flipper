@echo off
@title Flipper SD Cleaner
cls
cd /D "%~dp0"
echo This will DELETE the following file types from %~dp0 and all subdirectories:
echo .git, .mp4, .jp*, .png, .md, .pdf, and README files.
echo Press any key to continue, or close this window to abort.
pause > NUL
echo Deleting files, please wait...
attrib -h -s -r /s /d *.*
del /s /q *.git *.mp4 *.jp* *.png *.md *.pdf README
echo Done! Press any key to exit.
pause > NUL
exit
