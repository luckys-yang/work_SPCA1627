:start
@ECHO OFF
title Boot from USB
REM **********************************************************************
REM  For system boot from USB
REM 
REM                                                               By Phil
REM **********************************************************************

REM **********************************************************************
REM  Environment configure
REM **********************************************************************

set PATH_DOWNLOAD=.\download
if "%1" neq "" (
	set PATH_DOWNLOAD=%1
)

set PATH_USBOOT=C:\Progra~1\Sunplus\UsbBoot
set PATH_BIN=.\bin
set PATH_DRAM_PARAM=.\bin\drampara
set FILE_TARGET=%PATH_DOWNLOAD%\fw.bin
set FILE_DRAM_PARAM_INI=isp.ini
set FILE_LOG=.\log.tmp
set TOOL_USBOOT=usbboot.exe
set TOOL_SETPARA=setpara.exe
set ERR_DELAY=1

REM **********************************************************************
REM  Get the current directory
REM **********************************************************************

set "cd_=%cd%"
:loop
set "cd_=%cd_:*\=%"
set "cd_tmp=%cd_:\=%"
if not "%cd_tmp%"=="%cd_%" goto loop

if "%cd_%" == "script" (
	pushd ..\
)

REM **********************************************************************
REM  Download the bin file to romter
REM **********************************************************************

%PATH_BIN%\%TOOL_SETPARA% 1528 %PATH_DRAM_PARAM%\%FILE_DRAM_PARAM_INI%
%PATH_USBOOT%\%TOOL_USBOOT% 0x00 %FILE_TARGET% >%FILE_LOG%

REM **********************************************************************
REM  Check error and show the message if there's any error
REM **********************************************************************

findstr /c:"Can" %FILE_LOG% >NUL 2>NUL || findstr /c:"error" %FILE_LOG% >NUL 2>NUL
if %ERRORLEVEL% equ 0 (
	type %FILE_LOG%
	if "%ERR_DELAY%" == "1" (
		ping 127.0.0.1 >NUL 2>NUL
	)
)
del /f /q %FILE_LOG% >NUL 2>NUL

REM **********************************************************************
REM  Finished
REM **********************************************************************

if "%cd_%" == "script" (
	popd
)

ECHO Done!
:end