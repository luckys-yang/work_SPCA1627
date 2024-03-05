:start
@ECHO OFF
title Boot from romter
REM **********************************************************************
REM  For system boot from romter
REM 
REM                                                               By Phil
REM **********************************************************************

REM **********************************************************************
REM  Romter type configure
REM **********************************************************************

set ROMTER_USB=1
set ROMTER_COM=1

REM **********************************************************************
REM  Environment configure
REM **********************************************************************

set FILE_TARGET=fw.bin
set PATH_BIN=.\bin
set FILE_LOG=.\log.tmp
set ERR_DELAY=1

if "%ROMTER_USB%"=="1" (
	set TOOL_DOWN=down.exe
	set FILE_TARGET=.\download\%FILE_TARGET%
	echo Download to USB romter...
) else (
	set TOOL_DOWN=ep4.exe
	echo Download to COM romter...
)

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

if "%ROMTER_USB%" neq "1" (
	set FILE_TARGET=%CD%\download\%FILE_TARGET%
)

REM **********************************************************************
REM  Download the bin file to romter
REM **********************************************************************

%PATH_BIN%\%TOOL_DOWN% %FILE_TARGET% >%FILE_LOG%

REM **********************************************************************
REM  Check error and show the message if there's any error
REM **********************************************************************

findstr /c:"not" %FILE_LOG% >NUL 2>NUL || findstr /c:"error" %FILE_LOG% >NUL 2>NUL
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