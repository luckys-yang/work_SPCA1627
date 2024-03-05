:start
@ECHO OFF

REM ***********************************
REM For Usb boot
REM 
REM Double click this batch file or use command as below:
REM
REM     download.bat
REM 
REM                             By Phil
REM ***********************************

set USBBOOT=C:\Progra~1\Sunplus\UsbBoot
set DOWNLOAD=.\download\
set TARGET=fw.bin
set EXE=.\exe\
set DRAMPARAM=.\drampara\

%EXE%\setpara.exe 1528 %DRAMPARAM%\Isp.ini 
%USBBOOT%\usbboot.exe 0x0 %DOWNLOAD%\%TARGET%

ECHO Done!
:end