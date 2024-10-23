@echo off
@title Flipper SD Cleaner
cls
cd /D "%~dp0"
echo.
echo  This will DELETE the following file types from %~dp0 and all subdirectories:
echo.
echo  .git, .mp4, .jp*, .png, .md, .pdf, and README files.
echo.
echo  If you're good with that, press any key to continue. Otherwise, X out to abort.
pause > NUL
echo.
echo  Deleting files, please wait...
attrib -h -s -r /s /d *.*
del /s /q *.git *.mp4 *.jp* *.png *.md *.pdf README
echo.
echo  Done! Press any key to exit. (Scroll up for errors if desired.)
pause > NUL
exit
