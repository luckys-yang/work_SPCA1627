@ECHO OFF & setlocal EnableDelayedExpansion

title Front driver install

REM **********************************************************************
REM Sensor driver description file
REM
REM New front driver must be registered to this batch file.
REM You may modify the segment marked "Customized",
REM Others marked "Absolute" should be kept!
REM
REM front_install.bat [sensor type] [hw platform] [FDP] [customer prj]
REM
REM                                                                By Phil
REM **********************************************************************

REM **********************************************************************
REM Front driver customization (Customized)
REM
REM eg: "CUSTOM_DRIVER=OV_OV3630_RAW"
REM
REM If CUSTOM_DRIVER have be defined, The front driver defined will be
REM installed, or we'll use the parameter passed to this batch file
REM
REM **********************************************************************

REM set "CUSTOM_DRIVER=GC_GC1034_RAW"
set "CUSTOM_DRIVER=OV_OV5647_RAW"
REM set "CUSTOM_DRIVER=GC_GC0307_YUV"
REM set "CUSTOM_DRIVER=GC_GC1064_RAW"

REM set "CUSTOM_DRIVER=GC_GC2023_RAW"


REM **********************************************************************
REM Sensor List
REM
REM ========== Galaxy Core ==========
REM  GC_GC0316_RAW
REM  GC_GC1303_RAW
REM  GC_GC0307_YUV
REM ========== Micron ==========
REM  MI_MI1320_RAW
REM  MI_MI1600_RAW
REM  MI_MI5100_RAW
REM  MI_MI5130_RAW
REM  MI_MI5130_RAW_REV3
REM  MI_MI5130_RAW_REV8
REM  MI_MI5131E_RAW
REM  AP_SOC1040_RAW
REM  AP_SOC1040_YUV
REM ========== Omni Vision ==========
REM  OV_OV3630_RAW
REM  OV_OV3640_YUV
REM  OV_OV5610_RAW
REM  OV_OV5620_FRAME_RAW
REM  OV_OV5620_RAW
REM  OV_OV5633_RAW
REM  OV_OV5633_FRAME_RAW
REM  OV_OV5653_RAW
REM  OV_OV5653_RAW_INV
REM  OV_OV5653_RAW_INV_24
REM  OV_OV9712_RAW
REM  OV_OV7710_YUV
REM  OV_OV7725_YUV
REM  OV_OV9655_YUV
REM ========== PixArt Imaging ==========
REM  PA_PAS6371LT_RAW
REM ========== Silicon Optronics ==========
REM  SO_SOI768_RAW
REM ========== Philips_SAA7113 =========
REM  PH_PHSST3F7_RAW
REM ========== Himax Imaging=========
REM  HM_HM1355_RAW
REM
REM **********************************************************************

REM **********************************************************************
REM Environment configuration (Absolute)
REM **********************************************************************

set HW_SOLUTION=_HW_EVB_
set DRIVER_PATH=.\drivers\front
set DOWNLOAD=.\download\A
set TAG_DRIVER=drv_sensor.c
set DRV_PRJ=drv_front
set STR_MDF=.\script\STRModify.cmd

set "FRONT_PARAM="
set "FRONT_PARAM=%FRONT_PARAM% cdsppara.bin"
set "FRONT_PARAM=%FRONT_PARAM% cdsphc.bin"
set "FRONT_PARAM=%FRONT_PARAM% pvcdsp.bin"
set "FRONT_PARAM=%FRONT_PARAM% edgecoring.bin"
set "FRONT_PARAM=%FRONT_PARAM% stillcdsp.bin"
set "FRONT_PARAM=%FRONT_PARAM% stillcdsp_s.bin"
set "FRONT_PARAM=%FRONT_PARAM% stillcdsp_m.bin"
set "FRONT_PARAM=%FRONT_PARAM% stillcdsp_w.bin"
set "FRONT_PARAM=%FRONT_PARAM% lenslut.bin"
set "FRONT_PARAM=%FRONT_PARAM% gamma.bin"
set "FRONT_PARAM=%FRONT_PARAM% wdrtbl.bin"
set "FRONT_PARAM=%FRONT_PARAM% awbctt.bin"
set "FRONT_PARAM=%FRONT_PARAM% cm_a.bin"
set "FRONT_PARAM=%FRONT_PARAM% callen.bin"

REM **********************************************************************
REM Set the HUE parameter source name	(AUTO)
REM
REM **********************************************************************
set "FRONT_PARAM=%FRONT_PARAM% CDSPPARA_HUE_AUTO_X.BIN"
set "FRONT_PARAM=%FRONT_PARAM% CDSPPARA_HUE_AUTO_Y.BIN"
set "FRONT_PARAM=%FRONT_PARAM% CDSPPARA_HUE_AUTO_Z.BIN"

REM **********************************************************************
REM Set the HUE parameter source name	(BEACH)
REM
REM **********************************************************************
set "FRONT_PARAM=%FRONT_PARAM% CDSPPARA_HUE_BEACH_X.BIN"
set "FRONT_PARAM=%FRONT_PARAM% CDSPPARA_HUE_BEACH_Y.BIN"
set "FRONT_PARAM=%FRONT_PARAM% CDSPPARA_HUE_BEACH_Z.BIN"

REM **********************************************************************
REM Set the HUE parameter source name	(LANDSCAPE)
REM
REM **********************************************************************
set "FRONT_PARAM=%FRONT_PARAM% CDSPPARA_HUE_LANDSCAPE_X.BIN"
set "FRONT_PARAM=%FRONT_PARAM% CDSPPARA_HUE_LANDSCAPE_Y.BIN"
set "FRONT_PARAM=%FRONT_PARAM% CDSPPARA_HUE_LANDSCAPE_Z.BIN"

REM **********************************************************************
REM Set the HUE parameter source name	(PORTRAIT)
REM
REM **********************************************************************
set "FRONT_PARAM=%FRONT_PARAM% CDSPPARA_HUE_PORTRAIT_X.BIN"
set "FRONT_PARAM=%FRONT_PARAM% CDSPPARA_HUE_PORTRAIT_Y.BIN"
set "FRONT_PARAM=%FRONT_PARAM% CDSPPARA_HUE_PORTRAIT_Z.BIN"

REM **********************************************************************
REM Set the non-blocking digital zoom parameter source name	(LCD)
REM
REM **********************************************************************
set "FRONT_PARAM=%FRONT_PARAM% PV_VGA169_FACTOR.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_HD88_FACTOR.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_HD68_FACTOR.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_D1_FACTOR.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_VGA_FACTOR.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_QVGA_FACTOR.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_FHD_FACTOR.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_VGA_HIGH_FACTOR.BIN"

REM **********************************************************************
REM Set the non-blocking digital zoom parameter source name	(NTSC)
REM
REM **********************************************************************
set "FRONT_PARAM=%FRONT_PARAM% PV_VGA169_FACTOR_NTSC.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_HD88_FACTOR_NTSC.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_HD68_FACTOR_NTSC.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_D1_FACTOR_NTSC.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_VGA_FACTOR_NTSC.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_QVGA_FACTOR_NTSC.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_FHD_FACTOR_NTSC.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_VGA_HIGH_FACTOR_NTSC.BIN"

REM **********************************************************************
REM Set the non-blocking digital zoom parameter source name	(PAL)
REM
REM **********************************************************************
set "FRONT_PARAM=%FRONT_PARAM% PV_VGA169_FACTOR_PAL.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_HD88_FACTOR_PAL.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_HD68_FACTOR_PAL.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_D1_FACTOR_PAL.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_VGA_FACTOR_PAL.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_QVGA_FACTOR_PAL.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_FHD_FACTOR_PAL.BIN"
set "FRONT_PARAM=%FRONT_PARAM% DV_VGA_HIGH_FACTOR_PAL.BIN"

REM **********************************************************************
REM Set the non-blocking digital zoom parameter source name	(HDMI)
REM
REM **********************************************************************

REM **********************************************************************
REM Set the non-blocking digital zoom parameter source name	(PCCAM)
REM
REM **********************************************************************

set "FRONT_PARAM=%FRONT_PARAM% PC_QQVGA_FACTOR.BIN"

REM **********************************************************************
REM Set the Sensor Resource
REM
REM **********************************************************************

set AE_DUMMY_BIN=..\NULL
set FRONT_RES=.\download\A\SENSOR\gainTbl.BIN

set "AE_RES_BIN_LIST="
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% ae15fps50hzTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% ae15fps60hzTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% ae20fps50hzTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% ae20fps60hzTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% ae24fps50hzTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% ae24fps60hzTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% ae30fps50hzTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% ae30fps60hzTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% ae60fps50hzTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% ae60fps60hzTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% front15fpsPrevCmdTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% front20fpsPrevCmdTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% front24fpsPrevCmdTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% front30fpsPrevCmdTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% front60fpsPrevCmdTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% frontInit0CmdTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% frontInit1CmdTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% frontInit2CmdTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% frontInit3CmdTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% frontInit4CmdTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% frontSnapCmdTbl.BIN"
set "AE_RES_BIN_LIST=%AE_RES_BIN_LIST% gainTbl.BIN"

set path=%path%;.\bin

set KEIL=C:\Keil\UV3\Uv3.exe
set CP_OPT=-b
set PRJ_OPT=src
set DRV_LIST=.\lst\drv_front.i
set PRE_RESULT=.\obj\drv_front.result

set DRV_TOOL_AE=.\bin\Tbl2Res.exe
set TOOL_MT_CMP=.\bin\fAttrCmp.exe

REM **********************************************************************
REM Set the parameter source folder (Absolute)
REM If there's NO "FDP" defined, the default folder is "PARAM"
REM
REM eg: "FDP_1" means select the folder "PARAM_1" which in front driver folder
REM
REM FDP: Front Driver Parameter
REM
REM **********************************************************************

set PARAM_SRC_EXT=
if "%3" neq "NULL" (
	for /f "tokens=1,* delims=_" %%i in ("%3") do set cmd_opt_name=%%i&set cmd_opt_val=%%j
	if "!cmd_opt_name!" == "FDP" (
		REM echo PARAM_SRC_EXT=!cmd_opt_val!
		set PARAM_SRC_EXT=_!cmd_opt_val!
	)
)

if "%4" neq "NULL" (
	set PRJ_OPT=prj
	echo Build front driver for customer project
) else (
	echo Build front driver for fdk project
)

REM **********************************************************************
REM Set default front driver (Absolute)
REM
REM If CUSTOM_DRIVER haven't be defined and no parameter passed to this batch file,
REM we'll use the default front driver
REM **********************************************************************

set "FRONT_DRIVER=OV_OV5620_RAW"
set "SRC_DRIVER=front_ov_ov5620_raw.c"

REM **********************************************************************
REM Target front driver selection (Absolute)
REM **********************************************************************

if "%CUSTOM_DRIVER%" neq "" (
	set FRONT_DRIVER=%CUSTOM_DRIVER%
	echo Load custom front driver: "!FRONT_DRIVER!"
	
) else (
	if "%1" neq "" (
		if "%1" neq "NULL" (
			set "FRONT_DRIVER=%1"
			echo Load front driver: "!FRONT_DRIVER!"
		) else (
			echo Load default front driver: "!FRONT_DRIVER!"
		)
	) else (
		echo Load default front driver: "!FRONT_DRIVER!"
	)
)

REM **********************************************************************
REM Sensor driver parsing (Customized)
REM **********************************************************************

set SRC_DRIVER=%FRONT_DRIVER%\front_%FRONT_DRIVER%.c
set CDSP_PARAM_PATH_SRC=%DRIVER_PATH%\%FRONT_DRIVER%\param
set CDSP_PARAM_PATH_SRC=%CDSP_PARAM_PATH_SRC%%PARAM_SRC_EXT%
set CDSP_PARAM_PATH_DST=%DOWNLOAD%\SENSOR

REM **********************************************************************
REM Install specified front driver and check error after that (Absolute)
REM **********************************************************************

set PROC_DRV_INSTALL=0

REM **********************************************************************
REM Check Keil IDE
REM **********************************************************************

if not exist %KEIL% (
	echo -------------------------------------------------------------------------
	echo  Keil IDE not found.
	echo  Pls check "front_install.bat" for "set KEIL=C:\Keil\UV3\Uv3.exe"
	echo -------------------------------------------------------------------------
	goto end
)

REM **********************************************************************
REM Check keil project
REM **********************************************************************

if not exist %DRIVER_PATH%\%FRONT_DRIVER%\%DRV_PRJ%.uv2 (
	set PROC_DRV_INSTALL=1
	copy /y %DRIVER_PATH%\_%DRV_PRJ%.uv2 %DRIVER_PATH%\%FRONT_DRIVER%\%DRV_PRJ%.uv2 >NUL
	if !errorlevel! gtr 0 goto FRONT_INSTALL_FAIL
	copy /y %DRIVER_PATH%\_%DRV_PRJ%.opt %DRIVER_PATH%\%FRONT_DRIVER%\%DRV_PRJ%.opt >NUL
	if !errorlevel! gtr 0 goto FRONT_INSTALL_FAIL
	
	call %STR_MDF% %DRIVER_PATH%\%FRONT_DRIVER%\%DRV_PRJ%.uv2 null front_%FRONT_DRIVER% 0
	call %STR_MDF% %DRIVER_PATH%\%FRONT_DRIVER%\%DRV_PRJ%.opt null front_%FRONT_DRIVER% 0
)

REM **********************************************************************
REM Check the front driver
REM **********************************************************************

if not exist %DRIVER_PATH%\%FRONT_DRIVER%\obj\front_%FRONT_DRIVER%.i (
	set CP_OPT=-r
)

echo Pre-building front_%FRONT_DRIVER%.c...
pushd %DRIVER_PATH%\%FRONT_DRIVER%\
if not exist .\obj md .\obj\>NUL
start /MIN /WAIT %KEIL% %CP_OPT% %DRV_PRJ%.uv2 -t %PRJ_OPT% -o %PRE_RESULT%
if exist %PRE_RESULT% findstr "error ERROR" %PRE_RESULT%
popd
if %errorlevel% equ 0 (
	echo Building fail, pls check front_%FRONT_DRIVER%.c^^!
	goto FRONT_INSTALL_FAIL
)
echo Pre-building sensor driver success

REM **********************************************************************
REM Check the "i" file
REM **********************************************************************

if exist %DRV_LIST% (
	%TOOL_MT_CMP% %DRV_LIST% %DRIVER_PATH%\%FRONT_DRIVER%\obj\front_%FRONT_DRIVER%.i
	if !errorlevel! equ 0 (
		if exist %FRONT_RES% (
			echo No need to copy resource files for sensor driver
			goto DRV_CMP_END
		)
	)
)

if not exist .\lst md .\lst >NUL
copy /y %DRIVER_PATH%\%FRONT_DRIVER%\obj\front_%FRONT_DRIVER%.i %DRV_LIST% >NUL

REM **********************************************************************
REM delete old AE resource files
REM **********************************************************************

echo delete old AE resource files...

REM goto AE_RES_DEL
:RETURN_AE_RES_DEL

pushd %CDSP_PARAM_PATH_DST%
..\..\.%DRV_TOOL_AE% ..\..\.%DRV_LIST% ..\..\.%DRIVER_PATH%\%TAG_DRIVER% %FRONT_DRIVER% >NUL
popd

REM **********************************************************************
REM copy new AE resource files
REM **********************************************************************

echo copy new AE resource files...

goto AE_RES_MOVE
:RETURN_AE_RES_MOVE

:DRV_CMP_END

REM **********************************************************************
REM Copy specified CDSP parameter and check error after that (Absolute)
REM **********************************************************************

for %%a in (%FRONT_PARAM%) do (
	%TOOL_MT_CMP% %CDSP_PARAM_PATH_SRC%\%%a %CDSP_PARAM_PATH_DST%\%%a
	if !errorlevel! neq 0 (
		set PROC_DRV_INSTALL=1
	)
	
	if "!PROC_DRV_INSTALL!"=="1" (
		copy /y %CDSP_PARAM_PATH_SRC%\%%a %CDSP_PARAM_PATH_DST%\%%a >NUL
		if !errorlevel! gtr 0 (
		 	echo copy %CDSP_PARAM_PATH_SRC%\%%a %CDSP_PARAM_PATH_DST%\%%a fail
		) else (
			echo copy %CDSP_PARAM_PATH_SRC%\%%a %CDSP_PARAM_PATH_DST%\%%a
		)
	)
)

goto end

:AE_RES_MOVE

REM **********************************************************************
REM Copy specified Sensor Driver Resource File and check error after that (Absolute)
REM **********************************************************************

pushd %CDSP_PARAM_PATH_DST%
for %%a in (%AE_RES_BIN_LIST%) do (
	if not exist %%a (	
		copy /y %AE_DUMMY_BIN% %%a >NUL
		if !errorlevel! gtr 0 (
		 	echo copy %AE_DUMMY_BIN% %%a fail
		) else (
			echo copy %AE_DUMMY_BIN% %%a
		)
	) else (
		echo convert %%a success
	)
)
popd

goto RETURN_AE_RES_MOVE

:AE_RES_DEL

REM **********************************************************************
REM Del old specified Sensor Driver Resource File and check error after that (Absolute)
REM **********************************************************************

pushd %CDSP_PARAM_PATH_DST%
for %%a in (%AE_RES_BIN_LIST%) do (
	if exist %%a (	
		del /f /q %%a >NUL 2>NUL
		if !errorlevel! gtr 0 (
			echo del old %%a fail
		) else (
			echo del old %%a success
		)
	)
)
popd

goto RETURN_AE_RES_DEL

REM **********************************************************************
REM Error handle
REM **********************************************************************

:FRONT_INSTALL_FAIL
echo ----------------------------
echo  Front Driver Install Fail^^!
echo ----------------------------
goto end

:end