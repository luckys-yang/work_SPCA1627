@ECHO off

REM ***********************************
REM Make the project
REM 
REM Double click this batch file or use command as below:
REM
REM     make.bat [clean]|[H/W solution] [usbboot]
REM
REM such as:
REM     make.bat clean
REM     make.bat 1628
REM     make.bat 1628 ub
REM 
REM                             By Phil
REM ***********************************

REM ***********************************
REM Set default target
REM ***********************************

set TAG="1628"

REM ***********************************
REM Environment configure
REM ***********************************

set WORK=.\
set OBJ=obj
set LST=lst
set DOWNLOAD=download
set C51=C:\Keil\UV3\
set USBBOOT=C:\Progra~1\Sunplus\UsbBoot
set PROJECT=sp1kSrc.uv2
set RESULT=lst\result.build

pushd %WORK%

REM ***********************************
REM Input param parsing
REM ***********************************

if "%1"=="clean" (
	del %WORK%\%OBJ%\*.* /f /s /q
	del %WORK%\%LST%\*.* /f /s /q
	del %WORK%\%DOWNLOAD%\*.bin /f /q
	cls
	ECHO clean finished!
	goto END
)

REM ***********************************
REM Start build target
REM ***********************************

REM cls
ECHO start to build...

start /MIN /WAIT %C51%\uv3.exe -b %WORK%\%PROJECT% -t %TAG% -o %WORK%\%RESULT%

REM PAUSE
cls
type %WORK%\%RESULT%

popd

REM ***********************************
REM Finish build
REM ***********************************

if "%3"=="ub" %USBBOOT%\usbboot.exe 0 %WORK%\%DOWNLOAD%\fw.bin
if "%1"=="" pause 

REM ECHO =====================================
REM ECHO             build finished

:END