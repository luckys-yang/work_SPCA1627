%TOOL%\ICON2SFN %RES%\Icon\Icon2bit\16 %RES%\Icon\Icon8bit\16 %DOWNLOAD%\A\MENU\icon.sfn %INC%\app_osd_icon.h

%TOOL%\IconColorConvert.exe %RES%\font\ascii\16\ %RES%\font\ascii 0x800000 0x000000 0xc0c0c0 0x808080 W=AUTO
%TOOL%\FONT2SFN %RES%\Font\ascii %DOWNLOAD%\A\MENU\FONT_ASCII.sfn 0x0000 0x7f
%TOOL%\IconColorConvert.exe %RES%\font\english\16\ %RES%\font\english 0x800000 0x000000 0xc0c0c0 0x808080 W=AUTO
%TOOL%\FONT2SFN %RES%\Font\english %DOWNLOAD%\A\MENU\FONT00.sfn 0x0000 0x0000
%TOOL%\IconColorConvert.exe %RES%\font\schinese\16\ %RES%\font\schinese 0x800000 0x000000 0xc0c0c0 0x808080
%TOOL%\FONT2SFN %RES%\Font\schinese %DOWNLOAD%\A\MENU\FONT01.sfn 0x0000 0x0106
%TOOL%\IconColorConvert.exe %RES%\font\tchinese\16\ %RES%\font\tchinese 0x800000 0x000000 0xc0c0c0 0x808080
%TOOL%\FONT2SFN %RES%\Font\tchinese %DOWNLOAD%\A\MENU\FONT02.sfn 0x0000 0x0109
%TOOL%\IconColorConvert.exe %RES%\font\french\16\ %RES%\font\french 0x800000 0x000000 0xc0c0c0 0x808080 W=AUTO
%TOOL%\FONT2SFN %RES%\Font\french %DOWNLOAD%\A\MENU\FONT03.sfn 0x0000 0x0007
%TOOL%\IconColorConvert.exe %RES%\font\spanish\16\ %RES%\font\spanish 0x800000 0x000000 0xc0c0c0 0x808080 W=AUTO
%TOOL%\FONT2SFN %RES%\Font\spanish %DOWNLOAD%\A\MENU\FONT04.sfn 0x0000 0x000a
%TOOL%\IconColorConvert.exe %RES%\font\german\16\ %RES%\font\german 0x800000 0x000000 0xc0c0c0 0x808080 W=AUTO
%TOOL%\FONT2SFN %RES%\Font\german %DOWNLOAD%\A\MENU\FONT05.sfn 0x0000 0x0005
%TOOL%\IconColorConvert.exe %RES%\font\italian\16\ %RES%\font\italian 0x800000 0x000000 0xc0c0c0 0x808080 W=AUTO
%TOOL%\FONT2SFN %RES%\Font\italian %DOWNLOAD%\A\MENU\FONT06.sfn 0x0000 0x0004
%TOOL%\IconColorConvert.exe %RES%\font\portuguese\16\ %RES%\font\portuguese 0x800000 0x000000 0xc0c0c0 0x808080 W=AUTO
%TOOL%\FONT2SFN %RES%\Font\portuguese %DOWNLOAD%\A\MENU\FONT07.sfn 0x0000 0x000c
%TOOL%\IconColorConvert.exe %RES%\font\dutch\16\ %RES%\font\dutch 0x800000 0x000000 0xc0c0c0 0x808080 W=AUTO
%TOOL%\FONT2SFN %RES%\Font\dutch %DOWNLOAD%\A\MENU\FONT08.sfn 0x0000 0x0003
%TOOL%\IconColorConvert.exe %RES%\font\japanese\16\ %RES%\font\japanese 0x800000 0x000000 0xc0c0c0 0x808080
%TOOL%\FONT2SFN %RES%\Font\japanese %DOWNLOAD%\A\MENU\FONT09.sfn 0x0000 0x00ed
%TOOL%\IconColorConvert.exe %RES%\font\russian\16\ %RES%\font\russian 0x800000 0x000000 0xc0c0c0 0x808080 W=AUTO
%TOOL%\FONT2SFN %RES%\Font\russian %DOWNLOAD%\A\MENU\FONT10.sfn 0x0000 0x0036
%TOOL%\IconColorConvert.exe %RES%\font\arabic\16\ %RES%\font\arabic 0x800000 0x000000 0xc0c0c0 0x808080 W=AUTO
%TOOL%\FONT2SFN %RES%\Font\arabic %DOWNLOAD%\A\MENU\FONT11.sfn 0x0000 0x008c

%TOOL%\RES2MACRO 1 %DOWNLOAD%\A\MENU\ %INC%\Res_Macro.h

