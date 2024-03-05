@ECHO OFF

title Device driver install

REM *************************************************************************************
REM Device driver install process file
REM
REM Install device driver before compiling.
REM Sequence: sensor -> display
REM Accepted parameter to batch file (The param could be NULL):
REM
REM dev_install.bat [sensor type] [panel type] [package type] [hdmi type] [hw platform] [FDP] [customer prj]
REM
REM eg:
REM dev_install.bat OV_OV5620_RAW AUO_A025BN01_UPS052 PACKAGE_1528
REM -> completed form for driver install.
REM
REM dev_install.bat NULL AUO_A025BN01_UPS052 NULL
REM -> dummy param passed to front and package driver install file.
REM
REM dev_install.bat
REM -> none parm passed to all driver install files.
REM
REM NOTE: If CUSTOM_DRIVER have been defined in each driver install file, the parameter
REM       passed to the batch file will be aborted by each driver install process.
REM
REM                                                                              By Phil
REM *************************************************************************************

REM *************************************************************************************
REM Environment configure (Absolute)
REM *************************************************************************************

set WORK_PATH=.\script
set FRONT_DRIVER=front_install.bat
set DISP_DRIVER=disp_install.bat
set PACKAGE_DRIVER=package_install.bat
set HDMI_DRIVER=hdmi_install.bat

REM *************************************************************************************
REM Driver install process (Absolute)
REM *************************************************************************************

call %WORK_PATH%\%FRONT_DRIVER% %1 %5 %6 %7
call %WORK_PATH%\%DISP_DRIVER% %2
call %WORK_PATH%\%PACKAGE_DRIVER% %3
call %WORK_PATH%\%HDMI_DRIVER% %4

:end