@ECHO OFF & setlocal EnableDelayedExpansion

title Package driver install

REM **********************************************************************
REM Package driver description file
REM
REM New package driver must be registered to this batch file.
REM You may modify the segment marked "Customized",
REM Others marked "Absolute" should be kept!
REM
REM                                                                By Phil
REM **********************************************************************

REM **********************************************************************
REM Package driver customization (Customized)
REM
REM eg: "CUSTOM_DRIVER=PACKAGE_1628"
REM
REM If CUSTOM_DRIVER have be defined, The package driver defined will be
REM installed, or we'll use the parameter passed to this batch file
REM
REM **********************************************************************

set "CUSTOM_DRIVER=PACKAGE_1627"

REM **********************************************************************
REM Package List
REM
REM ========== ICatch ==========
REM  PACKAGE_1626
REM  PACKAGE_1627
REM  PACKAGE_1628
REM
REM **********************************************************************

REM **********************************************************************
REM Environment configuration (Absolute)
REM **********************************************************************

set DRIVER_PATH=.\drivers\package
set TAG_BASE_NAME=drv_package
set TAG_DRIVER=%TAG_BASE_NAME%.c
set TAG_OBJ=%TAG_BASE_NAME%.obj

set TOOL_MT_CMP=.\bin\fAttrCmp.exe

REM **********************************************************************
REM Set default package driver (Absolute)
REM
REM If CUSTOM_DRIVER haven't be defined and no parameter passed to this batch file,
REM we'll use the default package driver
REM **********************************************************************

set "PACKAGE_DRIVER=PACKAGE_1628"

REM **********************************************************************
REM Target package driver selection (Absolute)
REM **********************************************************************

if "%CUSTOM_DRIVER%" neq "" (
	set PACKAGE_DRIVER=%CUSTOM_DRIVER%
	echo Load custom package driver: "!PACKAGE_DRIVER!"
) else (
	if "%1" neq "" (
		if "%1" neq "NULL" (
			set "PACKAGE_DRIVER=%1"
			echo Load package driver: "!PACKAGE_DRIVER!"
		) else (
			echo Load default package driver: "!PACKAGE_DRIVER!"
		)
	) else (
		echo Load default package driver: "!PACKAGE_DRIVER!"
	)
)

REM **********************************************************************
REM Package driver parsing (Customized)
REM **********************************************************************

set SRC_DRIVER=%PACKAGE_DRIVER%.c

REM **********************************************************************
REM Install specified package driver and check error after that (Absolute)
REM **********************************************************************

if exist %DRIVER_PATH%\%TAG_DRIVER% (
	%TOOL_MT_CMP% %DRIVER_PATH%\%SRC_DRIVER% %DRIVER_PATH%\%TAG_DRIVER%
	if !errorlevel! equ 0 (
		goto end
	)
	del /f /q %DRIVER_PATH%\%TAG_DRIVER% >NUL 2>NUL
)

del /f /q .\obj\%TAG_OBJ% >NUL 2>NUL

copy /y %DRIVER_PATH%\%SRC_DRIVER% %DRIVER_PATH%\%TAG_DRIVER% >NUL

if %errorlevel% gtr 0 (
	echo -----------------------------
	echo  Package Driver Install Fail
	echo -----------------------------
)

:end

