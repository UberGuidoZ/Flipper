@echo off
setlocal enabledelayedexpansion
cls
echo.
echo #########################################
echo #    Marauder WROOM Flasher v1.10       #
echo #    By UberGuidoZ and SkeletonMan      #
echo #########################################
echo. 

:: Basic error checks
IF NOT EXIST esptool.exe GOTO ESPERROR

set BR=921600

for /f "tokens=1" %%A in ('wmic path Win32_SerialPort get DeviceID^,PNPDeviceID^|findstr /i VID_10C4') do set "_com=%%A"
if not [!_com!]==[] echo Attempting to use serial port: !_com! & GOTO CHOOSE

echo Marauder v4.x not found! Make sure drivers are installed and USB cable is good.
echo.
echo Drivers: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
GOTO ERREXIT

:CHOOSE
echo.
echo Which action would you like to perform?
echo.
echo 1. Flash Marauder WROOM (including older v4 OEM).
echo 2. Update Marauder (v0.10.0 included).
echo 3. Download USB UART Drivers (Silicon Labs).
echo.
set choice=
set /p choice= Type choice and hit enter: 
if '%choice%'=='1' GOTO MARAUDER
if '%choice%'=='2' GOTO UPDATE
if '%choice%'=='3' GOTO DRIVERS
echo Please choose 1, 2, or 3!
ping 127.0.0.1 -n 5
cls
GOTO CHOOSE

:MARAUDER
cls
echo.
echo #########################################
echo #    Marauder WROOM Flasher v1.10       #
echo #    By UberGuidoZ and SkeletonMan      #
echo #########################################
echo. 
set last_firmware=
for /f "tokens=1" %%F in ('dir WROOM\esp32_marauder*_old_hardware.bin /b /o-n') do set last_firmware=%%F
IF [!last_firmware!]==[] echo Please get and copy the last firmware from ESP32Marauder's Github Releases & GOTO ERREXIT
esptool.exe -p !_com! -b %BR% -c esp32 --before default_reset -a no_reset erase_region 0x9000 0x6000
echo Firmware Erased, preparing write...
ping 127.0.0.1 -n 3 > NUL
esptool.exe -p !_com! -b %BR% --before default_reset -a hard_reset -c esp32 write_flash --flash_mode dio --flash_freq 80m --flash_size 2MB 0x1000 WROOM\bootloader.bin 0x8000 WROOM\partitions.bin 0x10000 WROOM\!last_firmware! 0xE000 WROOM\boot_app.bin
GOTO DONE

:UPDATE
cls
echo.
echo #########################################
echo #    Marauder WROOM Flasher v1.10       #
echo #    By UberGuidoZ and SkeletonMan      #
echo #########################################
echo.
echo Please download OLD_HARDWARE BIN file to Marauder folder and rerun Flasher.
echo.
echo Press any key to open Marauder download location in default browser...
pause>NUL
start https://github.com/justcallmekoko/ESP32Marauder/releases/latest
GOTO ERREXIT

:DRIVERS
cls
echo.
echo #########################################
echo #    Marauder WROOM Flasher v1.10       #
echo #    By UberGuidoZ and SkeletonMan      #
echo #########################################
echo.
echo Please download and install the correct drivers and rerun Flasher.
echo.
echo Press any key to open driver download location in default browser...
pause>NUL
start https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
GOTO ERREXIT

:DONE
echo.
echo --------------------------------------------------------------------------
echo Process has completed. Marauder should have rebooted to finish the update.
echo --------------------------------------------------------------------------
echo.
echo (You may now close this window or press any key to exit.)
pause>nul
exit

:ESPERROR
echo esptool.exe is missing. Please download and extract the full package.
GOTO ERREXIT

:ERREXIT
echo.
echo (You may now close this window or press any key to exit.)
echo.
pause>nul
exit