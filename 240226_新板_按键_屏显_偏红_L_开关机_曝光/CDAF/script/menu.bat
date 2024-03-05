@ECHO OFF

REM **********************************************************************
REM rebuild menu struct, convert osd reource
REM 
REM Double click this batch file or use command as below:
REM
REM     menu.bat
REM 
REM                                                               By Joe
REM **********************************************************************

set DOWNLOAD=..\download
if "%1" == "host.Mod.A" (
	set DOWNLOAD=..\download.Mod.A
)
if "%1" == "host.Mod.B" (
	set DOWNLOAD=..\download.Mod.B
)

set RES=%DOWNLOAD%\resource
set INC=..\host\inc\app
set TOOL=..\bin
set FOLDER_HOST=host

if "%1" neq "" (
	set INC=..\%1\inc\app
	set FOLDER_HOST=%1
)


REM **********************************************************************
REM english schinese tchinese french dutch spanish italian portuguese german czech hungarian vietnamese polish turkish russian swedish greek romanian croation malay japanese korean thai
REM **********************************************************************
%TOOL%\Menu_1628.exe %INC%\iCatchTek.xls ..\%FOLDER_HOST%\app_status\sts_menu\app_menu_tab.c english schinese tchinese french spanish german italian portuguese dutch japanese russian arabic

call .\menu_clr.bat
call .\string.bat
call .\font.bat

call .\font_28.bat
REM call .\font_22.bat
REM call .\font_16.bat
REM call .\font_28_22.bat
REM call .\font_28_16.bat

%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\28\iconPv.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\28
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\28\iconVid.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\28
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\28\iconAud.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\28
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\28\iconPb.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\28
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\28\iconDps.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\28

%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\28\iconPv_TV.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\28
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\28\iconVid_TV.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\28
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\28\iconAud_TV.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\28
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\28\iconPb_TV.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\28
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\28\iconDps_TV.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\28

%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\22\iconPv.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\22
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\22\iconVid.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\22
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\22\iconAud.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\22
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\22\iconPb.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\22
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\22\iconDps.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\22

%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\22\iconPv_TV.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\22
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\22\iconVid_TV.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\22
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\22\iconAud_TV.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\22
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\22\iconPb_TV.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\22
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\22\iconDps_TV.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\22

%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\16\iconPv.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\16
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\16\iconVid.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\16
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\16\iconAud.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\16
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\16\iconPb.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\16
%TOOL%\iconlayout2bin.exe 0 %RES%\IconLayout\16\iconDps.ini %INC%\app_osd_icon.h %DOWNLOAD%\A\ICON_POS\16


REM /*add for UIConfig tool*/
echo #include "app_init.h" 							>> ..\%FOLDER_HOST%\app_status\sts_menu\app_menu_tab.c
echo sfr BankPage   = 0x9A; 							>> ..\%FOLDER_HOST%\app_status\sts_menu\app_menu_tab.c
echo void menuBankGet(UIToolInfo*info)						>> ..\%FOLDER_HOST%\app_status\sts_menu\app_menu_tab.c
echo {										>> ..\%FOLDER_HOST%\app_status\sts_menu\app_menu_tab.c
echo	info-^>currentBank = (UINT32)BankPage;					>> ..\%FOLDER_HOST%\app_status\sts_menu\app_menu_tab.c
echo	info-^>strSize = (UINT32)(MenuPtr_english-0x8000);			>> ..\%FOLDER_HOST%\app_status\sts_menu\app_menu_tab.c
echo	info-^>paraAddr=(UINT32)MenuParaTab;					>> ..\%FOLDER_HOST%\app_status\sts_menu\app_menu_tab.c
echo	info-^>paraSize = (UINT32)sizeof(MenuParaTab);				>> ..\%FOLDER_HOST%\app_status\sts_menu\app_menu_tab.c
echo	info-^>strIdxAddr = (UINT32)(MenuPtr_english-0x8000);			>> ..\%FOLDER_HOST%\app_status\sts_menu\app_menu_tab.c
echo	info-^>strIdxSize = (UINT32)MenuStrPtr_english - MenuPtr_english;	>> ..\%FOLDER_HOST%\app_status\sts_menu\app_menu_tab.c
echo }										>> ..\%FOLDER_HOST%\app_status\sts_menu\app_menu_tab.c

