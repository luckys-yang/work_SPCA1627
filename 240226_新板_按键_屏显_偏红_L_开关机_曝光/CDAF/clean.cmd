@ECHO OFF & setlocal EnableDelayedExpansion

REM **********************************************************************
REM Items to be cleared selection
REM **********************************************************************

set CLR_tmp=1
set CLR_bak=1
set CLR_bin=1

REM **********************************************************************
REM Environment configure
REM **********************************************************************

set WORK=.\
set LIB=lib
set OBJ=obj
set LST=lst
set DOWNLOAD=download
set SRC=src
set SRC4CUS=lib_src

REM **********************************************************************
REM Clean specified files
REM **********************************************************************

pushd %WORK%

if "%CLR_tmp%" == "1" (
	del .\%OBJ%\*.* /f /s /q >NUL 2>NUL
	ECHO clean *.obj done!
	del .\%LST%\*.* /f /s /q >NUL 2>NUL
	ECHO clean *.lst done!
	
	pushd .\drivers
	
	del drv*.c /f /s /q 1>NUL 2>NUL
	del drv*.lib /f /s /q 1>NUL 2>NUL
	del drv*.obj /f /s /q 1>NUL 2>NUL
	del drv*.bak /f /s /q 1>NUL 2>NUL
	del drv_front.uv2 /f /s /q 1>NUL 2>NUL
	del drv_front.opt /f /s /q 1>NUL 2>NUL
	
	FOR /r %%f IN (obj, lst) DO (
		if exist "%%f" (
			RD /s /q "%%f" 1>NUL
		)
	)
	
	popd
	
	ECHO clean temp files in driver folder done!
)
if "%CLR_bak%" == "1" (
	del .\*.bak /f /s /q >NUL 2>NUL
	ECHO clean *.bak done!
)
if "%CLR_bin%" == "1" (
	del .\%DOWNLOAD%\*.bin /f /q >NUL 2>NUL
	ECHO clean *.bin done!
)

popd

:end