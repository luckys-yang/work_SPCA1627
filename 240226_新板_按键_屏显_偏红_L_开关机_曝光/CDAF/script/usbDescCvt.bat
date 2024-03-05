@ECHO OFF

set BIN_C51=C:\Keil\C51\BIN\C51.EXE
set BIN_PARSER=..\bin\cFileParser.exe
set FILE_SRC_C=..\kernel\src\middleware\libUsb\common\stdreq.c
set FILE_SRC_I=..\lst\stdreq.i
set FILE_PRE_RESULT=..\lst\stdreq.result
set FILE_USB_RES=..\download\A\RES_DAT\stddesc.res

%BIN_C51% "%FILE_SRC_C%" LARGE OMF2 OPTIMIZE (9,SIZE) BROWSE NOAREGS ^
INCDIR(..\kernel\inc;.\kernel\inc\hal;..\kernel\inc\middleware;..\kernel\inc\os;..\kernel\inc\sp1k;^
..\FPGA\inc;..\host\inc\app;..\host\inc\customization) ^
DEFINE (CVT_TOOL) DEBUG NOPRINT PREPRINT(%FILE_SRC_I%) >%FILE_PRE_RESULT%

if exist %FILE_SRC_I% (
	%PARSER% %BIN_PARSER% %FILE_SRC_I% %FILE_USB_RES%
)
