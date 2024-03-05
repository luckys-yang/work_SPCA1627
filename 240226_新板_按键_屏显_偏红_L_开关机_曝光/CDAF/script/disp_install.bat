@ECHO OFF & setlocal EnableDelayedExpansion

title Display driver install

REM **********************************************************************
REM Display driver description file
REM
REM New display driver must be registered to this batch file.
REM You may modify the segment marked "Customized",
REM Others marked "Absolute" should be kept!
REM
REM                                                                By Phil
REM **********************************************************************

REM **********************************************************************
REM Display driver customization (Customized)
REM
REM eg: "CUSTOM_DRIVER=AUO_A025BN01_UPS052"
REM
REM If CUSTOM_DRIVER have be defined, The display driver defined will be
REM installed, or we'll use the parameter passed to this batch file
REM
REM **********************************************************************

REM set "CUSTOM_DRIVER=st7789v_HSDIPS_RT20QV034E_lcm"

set "CUSTOM_DRIVER=hsd2.4ips_st7789v"

REM set "CUSTOM_DRIVER=JD9851_BOE2.4_IPS_240x320_1dot_G2.5"

REM set "CUSTOM_DRIVER=ili9340x_BOE2.4_IPS"


REM set "CUSTOM_DRIVER=ili9341_IPS_RGH_lcm"


REM **********************************************************************
REM Panel List
REM
REM ========== AUO ==========
REM  txd_txdt180a_lcm
REM  AUO_A015AN02_UPS052
REM  AUO_A015AN04_UPS051
REM  AUO_A015AN04_UPS052
REM  AUO_A015BL01_UPS052
REM  AUO_A025BN01_CCIR
REM  AUO_A025BN01_UPS051
REM  AUO_A025BN01_UPS052
REM  AUO_A025DL01_UPS052
REM  AUO_A030DL01_UPS051
REM  AUO_A030DL01_UPS052
REM ========== Giant Plus ==========
REM  GP_54124A_LCM
REM  GP_GP015SQCT4B_UPS052
REM  GP_GPG1212011QS1_LCM
REM  GP_GPG12129QS1_LCM
REM  GP_GPG1216011QS1_LCM
REM  GP_GPM779A0_LCM
REM  GP_GPM815A0_LCM
REM ========== Samsung ==========
REM  SS_LTV160Q1_UPS051
REM  SS_LTV160Q1_UPS052
REM ========== iCatch ==========
REM  ICATCH_NULL_NONE
REM ========== Toppoly ==========
REM  TP_TD015THEB2_UPS052
REM  TP_TD020THEG1_UPS052
REM  TP_TD025THEEA_UPS052
REM ========== Tongxingda ==========
REM  TXD_TXDC190C1_LCM
REM  TXD_TXDT180A_LCM
REM	 TXD_TXDT020_LCD
REM  TXDT240TQT-76V3_ili9341_LCM
REM **********************************************************************
REM ========== DJ ==========
REM
REM	  DJNO300_ILI8960_UPS052
REM
REM **********************************************************************
REM **********************************************************************
REM Environment configuration (Absolute)
REM **********************************************************************

set DRIVER_PATH=.\drivers\display
set TAG_BASE_NAME=drv_panel
set TAG_DRIVER=%TAG_BASE_NAME%.c
set TAG_OBJ=%TAG_BASE_NAME%.obj
set PANEL_TYPE=

set TOOL_MT_CMP=.\bin\fAttrCmp.exe

REM **********************************************************************
REM Set default display driver (Absolute)
REM
REM If CUSTOM_DRIVER haven¡¯t be defined and no parameter passed to this batch file,
REM we'll use the default display driver
REM **********************************************************************

set "DISP_DRIVER=AUO_A025BN01_UPS052"
set "SRC_DRIVER=disp_auo_a025bn01_ups052.c"

REM **********************************************************************
REM Target display driver selection (Absolute)
REM **********************************************************************

if "%CUSTOM_DRIVER%" neq "" (
	set DISP_DRIVER=%CUSTOM_DRIVER%
	echo Load custom display driver: "!DISP_DRIVER!"
) else (
	if "%1" neq "" (
		if "%1" neq "NULL" (
			set "DISP_DRIVER=%1"
			echo Load display driver: "!DISP_DRIVER!"
		) else (
			echo Load default display driver: "!DISP_DRIVER!"
		)
	) else (
		echo Load default display driver: "!DISP_DRIVER!"
	)
)

REM **********************************************************************
REM Display driver parsing (Customized)
REM **********************************************************************

set SRC_DRIVER=disp_%DISP_DRIVER%.c

REM **********************************************************************
REM Install specified display driver and check error after that (Absolute)
REM **********************************************************************

if exist %DRIVER_PATH%\lcd\%SRC_DRIVER% (
	set PANEL_TYPE=lcd
) else (
	if exist %DRIVER_PATH%\lcm\%SRC_DRIVER% (
		set PANEL_TYPE=lcm
	)
)

if exist %DRIVER_PATH%\%TAG_DRIVER% (
	%TOOL_MT_CMP% %DRIVER_PATH%\%PANEL_TYPE%\%SRC_DRIVER% %DRIVER_PATH%\%TAG_DRIVER%
	if !errorlevel! equ 0 (
		goto end
	)
	del /f /q %DRIVER_PATH%\%TAG_DRIVER% >NUL 2>NUL
)

del /f /q .\obj\%TAG_OBJ% >NUL 2>NUL

copy /y %DRIVER_PATH%\%PANEL_TYPE%\%SRC_DRIVER% %DRIVER_PATH%\%TAG_DRIVER% >NUL

if %errorlevel% gtr 0 (
	echo -----------------------------
	echo  Display Driver Install Fail
	echo -----------------------------
)

:end

