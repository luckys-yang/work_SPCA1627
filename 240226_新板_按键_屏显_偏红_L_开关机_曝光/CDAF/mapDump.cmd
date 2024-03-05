@echo off & setlocal EnableDelayedExpansion

set /a EN_OUTPUT_FILE=1

set BIN_DUMP=.\bin\codeSizeAnalyzer.exe
set FILE_MAP=.\lst\1628.map
set FILE_RESULT=.\lst\1628.map.dmp

title Map File Dump - %FILE_MAP%

if not exist %BIN_DUMP% (
	echo can't find "%BIN_DUMP%"
	pause
	exit
)

if not exist %FILE_MAP% (
	echo can't find "%FILE_MAP%"
	pause
	exit
)

del /f /q %FILE_RESULT% >NUL 2>NUL

if %EN_OUTPUT_FILE% equ 1 (
	%BIN_DUMP% %FILE_MAP% %FILE_RESULT% %EN_OUTPUT_FILE%
	if !ERRORLEVEL! equ 0 (
		echo Output "%FILE_RESULT%" successful
	)
) else (
	%BIN_DUMP% %FILE_MAP%
)

pause 

:EOF