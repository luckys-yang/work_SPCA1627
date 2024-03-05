@ECHO OFF & setlocal EnableDelayedExpansion

title HDMI driver install

REM **********************************************************************
REM HDMI driver description file
REM
REM New HDMI driver must be registered to this batch file.
REM You may modify the segment marked "Customized",
REM Others marked "Absolute" should be kept!
REM
REM                                                                By Phil
REM **********************************************************************

REM **********************************************************************
REM HDMI driver customization (Customized)
REM
REM eg: "CUSTOM_DRIVER=ANA_ANX7150"
REM
REM If CUSTOM_DRIVER have be defined, The HDMI driver defined will be
REM installed, or we'll use the parameter passed to this batch file
REM
REM **********************************************************************

set "CUSTOM_DRIVER="

REM **********************************************************************
REM HDMI interface List
REM
REM ========== iCatch ========== 
REM  SPMM_NULL_NONE
REM ========== ANA ==========
REM  ANA_ANX7150
REM ========== EP ==========
REM  EP_EP932
REM  EP_EP952
REM
REM **********************************************************************

REM **********************************************************************
REM Environment configuration (Absolute)
REM **********************************************************************

set KEIL=C:\Keil\uv3\Uv3.exe
set DRIVER_PATH=.\drivers\display
set TAG_BASE_NAME=drv_hdmi
set TAG_DRIVER=%TAG_BASE_NAME%.c
set TAG_OBJ=%TAG_BASE_NAME%.obj
set TAG_LIB=drv_hdmi.lib

set TOOL_MT_CMP=.\bin\fAttrCmp.exe

REM **********************************************************************
REM Set default HDMI driver (Absolute)
REM
REM If CUSTOM_DRIVER haven¡¯t be defined and no parameter passed to this batch file,
REM we'll use the default HDMI driver
REM **********************************************************************

set "HDMI_DRIVER=ANA_ANX7150"
set "SRC_DRIVER=disp_hdmi_ana_anx7150.c"

REM **********************************************************************
REM Target HDMI driver selection (Absolute)
REM **********************************************************************

if "%CUSTOM_DRIVER%" neq "" (
	set HDMI_DRIVER=%CUSTOM_DRIVER%
	echo Load HDMI driver: "!HDMI_DRIVER!"
) else (
	if "%1" neq "" (
		if "%1" neq "NULL" (
			set "HDMI_DRIVER=%1"
			echo Load HDMI driver: "!HDMI_DRIVER!"
		) else (
			echo Load default HDMI driver: "!HDMI_DRIVER!"
		)
	) else (
		echo Load default HDMI driver: "!HDMI_DRIVER!"
	)
)

REM **********************************************************************
REM HDMI driver parsing (Customized)
REM **********************************************************************

set SRC_DRIVER=disp_hdmi_%HDMI_DRIVER%.c

REM **********************************************************************
REM Install specified HDMI driver and check error after that (Absolute)
REM **********************************************************************

if exist %DRIVER_PATH%\%TAG_DRIVER% (
	%TOOL_MT_CMP% %DRIVER_PATH%\tv\HDMI_%HDMI_DRIVER%\%SRC_DRIVER% %DRIVER_PATH%\%TAG_DRIVER%
	if !errorlevel! equ 0 (
		goto end
	)
	del /f /q %DRIVER_PATH%\%TAG_DRIVER% >NUL 2>NUL
)

del /f /q .\obj\%TAG_OBJ% >NUL 2>NUL

copy /y %DRIVER_PATH%\tv\HDMI_%HDMI_DRIVER%\%SRC_DRIVER% %DRIVER_PATH%\%TAG_DRIVER% >NUL
if %errorlevel% gtr 0 (
	echo -----------------------------
	echo  HDMI Driver Install Fail
	echo -----------------------------
)

%TOOL_MT_CMP% %DRIVER_PATH%\tv\HDMI_%HDMI_DRIVER%\src\obj\%TAG_LIB% %DRIVER_PATH%\%TAG_LIB%
if %errorlevel% neq 0 (
REM if not exist %DRIVER_PATH%\tv\HDMI_%HDMI_DRIVER%\src\obj\%TAG_LIB% (
	pushd %DRIVER_PATH%\tv\HDMI_%HDMI_DRIVER%\src\
	start /wait %KEIL% -b .\drv_hdmi.Uv2
	popd
	copy /y %DRIVER_PATH%\tv\HDMI_%HDMI_DRIVER%\src\obj\%TAG_LIB% %DRIVER_PATH%\%TAG_LIB% >NUL
	if !errorlevel! gtr 0 (
		echo -------------------------------------------------------------------------
		echo  Build %DRIVER_PATH%\tv\HDMI_%HDMI_DRIVER%\src\obj\%TAG_LIB% Fail
		echo -------------------------------------------------------------------------
	)
)

:end

