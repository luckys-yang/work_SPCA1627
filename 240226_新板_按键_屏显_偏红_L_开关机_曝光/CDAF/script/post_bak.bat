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
set HEX2BIN=HEX2BIN_bak.exe

set FW_SIZE_RECORD=0x7FF0

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

%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B0:0 - B0:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H00) >NUL
%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B1:0 - B1:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H01) OFFSET (-0x10000) >NUL
%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B2:0 - B2:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H02) OFFSET (-0x20000) >NUL
%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B3:0 - B3:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H03) OFFSET (-0x30000) >NUL
%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B4:0 - B4:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H04) OFFSET (-0x40000) >NUL
%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B5:0 - B5:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H05) OFFSET (-0x50000) >NUL
%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B6:0 - B6:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H06) OFFSET (-0x60000) >NUL
%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B7:0 - B7:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H07) OFFSET (-0x70000) >NUL
%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B8:0 - B8:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H08) OFFSET (-0x80000) >NUL
%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B9:0 - B9:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H09) OFFSET (-0x90000) >NUL
%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B10:0 - B10:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H10) OFFSET (-0xA0000) >NUL
%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B11:0 - B11:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H11) OFFSET (-0xB0000) >NUL
%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B12:0 - B12:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H12) OFFSET (-0xC0000) >NUL
%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B13:0 - B13:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H13) OFFSET (-0xD0000) >NUL
%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B14:0 - B14:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H14) OFFSET (-0xE0000) >NUL

%TOOL%\HEX2BIN -e B00 %OBJ%\%SOURCE%.H00 >NUL
%TOOL%\HEX2BIN -e B01 %OBJ%\%SOURCE%.H01 >NUL
%TOOL%\HEX2BIN -e B02 %OBJ%\%SOURCE%.H02 >NUL
%TOOL%\HEX2BIN -e B03 %OBJ%\%SOURCE%.H03 >NUL
%TOOL%\HEX2BIN -e B04 %OBJ%\%SOURCE%.H04 >NUL
%TOOL%\HEX2BIN -e B05 %OBJ%\%SOURCE%.H05 >NUL
%TOOL%\HEX2BIN -e B06 %OBJ%\%SOURCE%.H06 >NUL
%TOOL%\HEX2BIN -e B07 %OBJ%\%SOURCE%.H07 >NUL
%TOOL%\HEX2BIN -e B08 %OBJ%\%SOURCE%.H08 >NUL
%TOOL%\HEX2BIN -e B09 %OBJ%\%SOURCE%.H09 >NUL
%TOOL%\HEX2BIN -e B10 %OBJ%\%SOURCE%.H10 >NUL
%TOOL%\HEX2BIN -e B11 %OBJ%\%SOURCE%.H11 >NUL
%TOOL%\HEX2BIN -e B12 %OBJ%\%SOURCE%.H12 >NUL
%TOOL%\HEX2BIN -e B13 %OBJ%\%SOURCE%.H13 >NUL
%TOOL%\HEX2BIN -e B14 %OBJ%\%SOURCE%.H14 >NUL


REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B15:0 - B15:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H15) OFFSET (-0xF0000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B16:0 - B16:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H16) OFFSET (-0x100000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B17:0 - B17:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H17) OFFSET (-0x110000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B18:0 - B18:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H18) OFFSET (-0x120000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B19:0 - B19:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H19) OFFSET (-0x130000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B20:0 - B20:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H20) OFFSET (-0x140000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B21:0 - B21:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H21) OFFSET (-0x150000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B22:0 - B22:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H22) OFFSET (-0x160000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B23:0 - B23:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H23) OFFSET (-0x170000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B24:0 - B24:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H24) OFFSET (-0x180000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B25:0 - B25:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H25) OFFSET (-0x190000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B26:0 - B26:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H26) OFFSET (-0x1A0000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B27:0 - B27:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H27) OFFSET (-0x1B0000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B28:0 - B28:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H28) OFFSET (-0x1C0000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B29:0 - B29:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H29) OFFSET (-0x1D0000) >NUL
REM	%KEIL%\OHX51 %OBJ%\%SOURCE% RANGE (B30:0 - B30:0xFFFF) HEXFILE (%OBJ%\%SOURCE%.H30) OFFSET (-0x1E0000) >NUL
	
REM	%TOOL%\HEX2BIN -e B15 %OBJ%\%SOURCE%.H15 >NUL
REM	%TOOL%\HEX2BIN -e B16 %OBJ%\%SOURCE%.H16 >NUL
REM	%TOOL%\HEX2BIN -e B17 %OBJ%\%SOURCE%.H17 >NUL
REM	%TOOL%\HEX2BIN -e B18 %OBJ%\%SOURCE%.H18 >NUL
REM	%TOOL%\HEX2BIN -e B19 %OBJ%\%SOURCE%.H19 >NUL
REM	%TOOL%\HEX2BIN -e B20 %OBJ%\%SOURCE%.H20 >NUL
REM	%TOOL%\HEX2BIN -e B21 %OBJ%\%SOURCE%.H21 >NUL
REM	%TOOL%\HEX2BIN -e B22 %OBJ%\%SOURCE%.H22 >NUL
REM	%TOOL%\HEX2BIN -e B23 %OBJ%\%SOURCE%.H23 >NUL
REM	%TOOL%\HEX2BIN -e B24 %OBJ%\%SOURCE%.H24 >NUL
REM	%TOOL%\HEX2BIN -e B25 %OBJ%\%SOURCE%.H25 >NUL
REM	%TOOL%\HEX2BIN -e B26 %OBJ%\%SOURCE%.H26 >NUL
REM	%TOOL%\HEX2BIN -e B27 %OBJ%\%SOURCE%.H27 >NUL
REM	%TOOL%\HEX2BIN -e B28 %OBJ%\%SOURCE%.H28 >NUL
REM	%TOOL%\HEX2BIN -e B29 %OBJ%\%SOURCE%.H29 >NUL
REM	%TOOL%\HEX2BIN -e B30 %OBJ%\%SOURCE%.H30 >NUL

%TOOL%\BANKLINK 15 %OBJ%\%SOURCE% %OBJ%\%SOURCE%.BIN >NUL
REM	%TOOL%\BANKLINK 31 %OBJ%\%SOURCE% %OBJ%\%SOURCE%.BIN >NUL

REM **********************************************************************
REM Merge to target file
REM **********************************************************************

COPY %OBJ%\%SOURCE%.BIN %DOWNLOAD%\%MIDDLE% >NUL

pushd %DOWNLOAD%
.%TOOL%\RESMERGER %TARGET% .\RES_CFG.INI 1528
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
