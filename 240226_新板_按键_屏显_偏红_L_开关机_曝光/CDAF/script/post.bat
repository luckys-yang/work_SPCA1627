@ECHO OFF

title Post building process

REM **********************************************************************
REM Convert Hex file to Binary file and merge to target
REM 
REM Double click this batch file or use command as below:
REM
REM     post.bat [download path] [usboot || romter]
REM
REM such as:
REM     post.bat 
REM     post.bat .\download\ usboot
REM     post.bat .\download\ romter
REM 
REM                                                                By Phil
REM **********************************************************************

REM **********************************************************************
REM Environment configure
REM **********************************************************************

set path=%path%;.\bin

set WORK=.\
set OBJ=.\obj
set KEIL=C:\Keil\C51\BIN
set USBBOOT=C:\Progra~1\Sunplus\UsbBoot
set TOOL=.\bin
set DOWNLOAD=.\download
set RESOURCE=resource
set SOURCE=1628
set MIDDLE=FW.BIN
set TARGET=1628.bin
set MAP=.\lst\%SOURCE%.map

set FW_SIZE_RECORD=0x7FF0
set /a FW_SIZE_ALIGN=0

if "%1" neq "" (
	set DOWNLOAD=%1
)

REM **********************************************************************
REM Check code/data overlap
REM **********************************************************************

findstr /n /c:"*** OVERLAP ***" %MAP%
if %errorlevel% equ 0 (
	goto ERR_OVERLAP
)

REM **********************************************************************
REM IDE Environment check
REM **********************************************************************

if not exist %KEIL%\ohx51.exe (
	goto ERR_IDE_NOT_FOUND
)

REM **********************************************************************
REM Convert to Binary File
REM **********************************************************************

if %FW_SIZE_ALIGN% equ 0 (
	%TOOL%\HEX2BIN.EXE %OBJ%\%SOURCE%.hex
) else (
	%TOOL%\HEX2BIN_align.EXE %OBJ%\%SOURCE%.hex
)
%TOOL%\sizeRec.exe %OBJ%\%SOURCE%.BIN %FW_SIZE_RECORD% 1

REM **********************************************************************
REM Merge to target file
REM **********************************************************************

COPY %OBJ%\%SOURCE%.BIN %DOWNLOAD%\%MIDDLE% >NUL

pushd %DOWNLOAD%
.%TOOL%\RESMERGER %TARGET% .\RES_CFG.INI 1628
REM .%TOOL%\RESMERGER %TARGET% .\RLE_RES_CFG.INI 1528
REM .%TOOL%\RESMERGER %TARGET% .\RES_CFG.INI 1628
popd

REM **********************************************************************
REM Download
REM **********************************************************************

if "%2" == "usboot" (
	call .\script\usboot.bat
)

if "%2" == "romter" (
	call .\script\romter.bat
)

REM **********************************************************************
REM Finish Process
REM **********************************************************************

goto end
exit

REM **********************************************************************
REM Error handle
REM **********************************************************************

:ERR_OVERLAP
ECHO -------------------------------------------------------
ECHO  Build target fail!
ECHO  Code/Data overlap, please check %MAP%
ECHO -------------------------------------------------------
pause
exit

:ERR_IDE_NOT_FOUND
ECHO -------------------------------------------------------
ECHO  Build target fail!
ECHO  Couldn't find Keil IDE tools in %KEIL%\
ECHO  Please configure IDE environment in ".\post.bat"
ECHO -------------------------------------------------------
pause
exit

:end
