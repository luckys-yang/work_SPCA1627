@echo off

set DOWNLOAD=.\download

set PATH_CHK=%DOWNLOAD%\resource\
set FILE_CHK=res.fdk

set FILE_BAT_VER=.\script\ver_gen.bat
set FILE_INPUT=.\kernel\ver_svn.h
set FILE_OUTPUT=.\kernel\ver_svn.h
set DIR_TAG=.\

if "%1" neq "" (
	set FILE_CHK=res.%1
)

if exist %PATH_CHK%\%FILE_CHK% (

	echo Resource files rebuilding...
	pushd .\script\
	call .\menu.bat %1 >NUL
	popd

	echo %FILE_OUTPUT% generating...
	call %FILE_BAT_VER% %DIR_TAG% %FILE_INPUT% %FILE_OUTPUT%
		
	del /f /q %PATH_CHK%\%FILE_CHK% >NUL
)

echo Install progress completed.
:end