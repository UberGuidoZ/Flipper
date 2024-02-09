echo off
@title Flipper SD Cleaner
cls
cd /D "%~dp0"
echo.
echo  Continuing will DELETE FILES from %~dp0 and all subdirectories.
echo  All .git, .mp4, .jp*, .png, .md, and README files will be removed!
echo.
echo  If you're good with that, press any key to continue. Otherwise, close out.
pause > NUL
echo.
echo  Deleting files, please wait...
attrib *.* -h -s -r /s
del *.git /s /q
del *.mp4 /s /q
del *.jp* /s /q
del *.png /s /q
del *.md /s /q
del README /s /q
echo.
echo  Done! Press any key to exit. (Scroll up for errors if desired.)
pause > NUL
exit