@echo off
setlocal enabledelayedexpansion
cls
echo.
echo #########################################
echo #    Marauder Flasher Script v2.30      #
echo #    By Frog, tweaked by UberGuidoZ     #
echo #      and by ImprovingRigmarole        #
echo #    WROOM inspired by SkeletonMan      #
echo #########################################
echo. 

:: Basic error checks
IF NOT EXIST esptool.exe GOTO ESPERROR

set BR=921600

for /f "tokens=1" %%A in ('wmic path Win32_SerialPort get DeviceID^,PNPDeviceID^|findstr /i VID_303A') do set "_com=%%A"
if not [!_com!]==[] echo Attempting to use serial port: !_com! & GOTO CHOOSE_FW
for /f "tokens=1" %%A in ('wmic path Win32_SerialPort get DeviceID^,PNPDeviceID^|findstr /i VID_10C4') do set "_com=%%A"
if not [!_com!]==[] echo Attempting to use serial port: !_com! & GOTO WROOM

echo Cannot find ESP32-S2 DevBoard or WROOM (Marauder)!
echo For Devboard, make sure to hold BOOT when plugging in USB.
echo For WROOM, try continuing to hold the boot button through the process.
echo.
echo Otherwise, make sure drivers are installed and USB cable is good.
echo.
echo Drivers: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
GOTO ERREXIT

:CHOOSE_FW
echo.
echo Which action would you like to perform?
echo.
echo 1. Flash Marauder to Devboard
echo 2. Update Marauder BIN file (v0.10.7 included)
echo 3. Save Flipper Blackmagic WiFi settings
echo 4. Flash Flipper Blackmagic
echo 5. Download USB UART Drivers (Silicon Labs).
echo.
set choice_fw=
set /p choice_fw= Type choice and hit enter: 
if '%choice_fw%'=='1' GOTO MARAUDER
if '%choice_fw%'=='2' GOTO UPDATE
if '%choice_fw%'=='3' GOTO BACKUP
if '%choice_fw%'=='4' GOTO FLIPPERBM
if '%choice_fw%'=='5' GOTO DRIVERS
echo Please choose 1, or 2!
ping 127.0.0.1 -n 5
cls
GOTO CHOOSE_FW

:MARAUDER
cls
echo.
echo #########################################
echo #    Marauder Flasher Script v2.30      #
echo #    By Frog, tweaked by UberGuidoZ     #
echo #      and by ImprovingRigmarole        #
echo #    WROOM inspired by SkeletonMan      #
echo #########################################
echo. 
set last_firmware=
for /f "tokens=1" %%F in ('dir Marauder\esp32_marauder*flipper.bin /b /o-n') do set last_firmware=%%F
IF [!last_firmware!]==[] echo Please get and copy the last firmware from ESP32Marauder's Github Releases & GOTO ERREXIT
esptool.exe -p !_com! -b %BR% -c esp32s2 --before default_reset -a no_reset erase_region 0x9000 0x6000
echo Firmware Erased, preparing write...
ping 127.0.0.1 -n 5 > NUL
esptool.exe -p !_com! -b %BR% -c esp32s2 --before default_reset -a no_reset write_flash --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 Marauder\bootloader.bin 0x8000 Marauder\partitions.bin 0x10000 Marauder\!last_firmware!
GOTO DONE

:UPDATE
cls
echo.
echo #########################################
echo #    Marauder Flasher Script v2.30      #
echo #    By Frog, tweaked by UberGuidoZ     #
echo #      and by ImprovingRigmarole        #
echo #    WROOM inspired by SkeletonMan      #
echo #########################################
echo.
echo Please download flipper BIN file to Marauder folder, delete any other BIN files, and rerun Flasher..
echo.
echo Press any key to open Marauder download location in default browser...
pause>NUL
start https://github.com/justcallmekoko/ESP32Marauder/releases/latest
GOTO ERREXIT

:BACKUP
cls
echo.
echo #########################################
echo #    Marauder Flasher Script v2.30      #
echo #    By Frog, tweaked by UberGuidoZ     #
echo #      and by ImprovingRigmarole        #
echo #    WROOM inspired by SkeletonMan      #
echo #########################################
echo.
echo Saving Flipper Blackmagic WiFi Settings to "FlipperBlackmagic\nvs.bin"
esptool.exe -p !_com! -b %BR% -c esp32s2 -a no_reset read_flash 0x9000 0x6000 FlipperBlackmagic\nvs.bin
GOTO DONE

:FLIPPERBM
cls
echo.
echo #########################################
echo #    Marauder Flasher Script v2.30      #
echo #    By Frog, tweaked by UberGuidoZ     #
echo #      and by ImprovingRigmarole        #
echo #    WROOM inspired by SkeletonMan      #
echo #########################################
echo. 
IF EXIST FlipperBlackmagic\nvs.bin (
    echo Flashing Flipper Blackmagic with WiFi Settings restore
    esptool.exe -p !_com! -b %BR% -c esp32s2 --before default_reset -a no_reset write_flash --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 FlipperBlackmagic\bootloader.bin 0x8000 FlipperBlackmagic\partition-table.bin 0x9000 FlipperBlackmagic\nvs.bin 0x10000 FlipperBlackmagic\blackmagic.bin
) ELSE (
    echo Flashing Flipper Blackmagic without WiFi Settings restore
    esptool.exe -p !_com! -b %BR% -c esp32s2 --before default_reset -a no_reset erase_region 0x9000 0x6000
    echo Firmware Erased, preparing write...
    ping 127.0.0.1 -n 5 > NUL
    esptool.exe -p !_com! -b %BR% -c esp32s2 --before default_reset -a no_reset write_flash --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 FlipperBlackmagic\bootloader.bin 0x8000 FlipperBlackmagic\partition-table.bin 0x10000 FlipperBlackmagic\blackmagic.bin
)
GOTO DONE

:WROOM
echo.
echo Which action would you like to perform?
echo.
echo 1. Flash Marauder WROOM (including older v4 OEM).
echo 2. Update Marauder BIN file (v0.10.7 included).
echo 3. Download USB UART Drivers (Silicon Labs).
echo.
set choice=
set /p choice= Type choice and hit enter: 
if '%choice%'=='1' GOTO WRMARAUDER
if '%choice%'=='2' GOTO WRUPDATE
if '%choice%'=='3' GOTO DRIVERS
echo Please choose 1, 2, or 3!
ping 127.0.0.1 -n 5
cls
GOTO CHOOSE

:WRMARAUDER
cls
echo.
echo #########################################
echo #    Marauder Flasher Script v2.30      #
echo #    By Frog, tweaked by UberGuidoZ     #
echo #      and by ImprovingRigmarole        #
echo #    WROOM inspired by SkeletonMan      #
echo #########################################
echo. 
set last_firmware=
for /f "tokens=1" %%F in ('dir WROOM\esp32_marauder*_old_hardware.bin /b /o-n') do set last_firmware=%%F
IF [!last_firmware!]==[] echo Please get and copy the last firmware from ESP32Marauder's Github Releases & GOTO ERREXIT
esptool.exe -p !_com! -b %BR% -c esp32 --before default_reset -a no_reset erase_region 0x9000 0x6000
echo Firmware Erased, preparing write...
ping 127.0.0.1 -n 3 > NUL
esptool.exe -p !_com! -b %BR% --before default_reset -a hard_reset -c esp32 write_flash --flash_mode dio --flash_freq 80m --flash_size 2MB 0x1000 WROOM\bootloader.bin 0x8000 WROOM\partitions.bin 0x10000 WROOM\!last_firmware! 0xE000 WROOM\boot_app.bin
GOTO WRDONE

:WRUPDATE
cls
echo.
echo #########################################
echo #    Marauder Flasher Script v2.30      #
echo #    By Frog, tweaked by UberGuidoZ     #
echo #      and by ImprovingRigmarole        #
echo #    WROOM inspired by SkeletonMan      #
echo #########################################
echo.
echo Please download OLD_HARDWARE.BIN file to WROOM folder, delete any other BIN files, and rerun Flasher.
echo.
echo Press any key to open Marauder download location in default browser...
pause>NUL
start https://github.com/justcallmekoko/ESP32Marauder/releases/latest
GOTO ERREXIT

:DRIVERS
cls
echo.
echo #########################################
echo #    Marauder Flasher Script v2.30      #
echo #    By Frog, tweaked by UberGuidoZ     #
echo #      and by ImprovingRigmarole        #
echo #    WROOM inspired by SkeletonMan      #
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
echo -------------------------------------------------------------------------------------------
echo Process has completed! Please disconnect your Devboard and connect it to your Flipper Zero.
echo -------------------------------------------------------------------------------------------
echo.
echo ==========================================================================
echo ERRORS ABOVE MAY BE NORMAL - Please ignore them for now and give it a try.
echo ==========================================================================
echo.
echo (You may now close this window or press any key to exit.)
pause>nul
exit

:WRDONE
echo.
echo ---------------------------------------------------------------
echo Process has completed. Device may reboot to finish the update.
echo Otherwise, disconnect the device and plug it into your Flipper!
echo ---------------------------------------------------------------
echo.
echo ==========================================================================
echo ERRORS ABOVE MAY BE NORMAL - Please ignore them for now and give it a try.
echo ==========================================================================
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
pause>nul
exit