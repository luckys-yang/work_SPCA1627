/**************************************************************************
 *
 *      Copyright (c) 2005-2008 by Sunplus mMedia Technology Co., Ltd.
 *
 *  This software is copyrighted by and is the property of Sunplus mMedia
 *  Technology Co., Ltd. All rights are reserved by Sunplus mMedia Technology
 *  Co., Ltd. This software may only be used in accordance with the
 *  corresponding license agreement. Any unauthorized use, duplication,
 *  distribution, or disclosure of this software is expressly forbidden.
 *
 *  This Copyright notice MUST not be removed or modified without prior
 *  written consent of Sunplus mMedia Technology Co., Ltd.
 *
 *  Sunplus mMedia Technology Co., Ltd. reserves the right to modify this
 *  software without notice.
 *
 *  Sunplus mMedia Technology Co., Ltd.
 *  19, Innovation First Road, Science-Based Industrial Park,
 *  Hsin-Chu, Taiwan.
 *
 *
 **************************************************************************/

/**
 * @file		app_ui_para.h
 * @brief		App for UI setting parameter definition
 * @author	Phil Lin
 * @since		2008-02-20
 * @date		2008-02-20
 */
#ifndef __APP_UI_PARA_H__
#define __APP_UI_PARA_H__
 //=============================================================================
//Header file
//=============================================================================
#include "app_menu_api.h"
#include "host_func_opt.h"
//=============================================================================
//Symbol
//=============================================================================
//-----------------------------------------------------------------------------
//Constant
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//Type definition
//-----------------------------------------------------------------------------

typedef struct uiPara_s {	/* Can't be larger than 512Bytes */
/* Note: Don't add any struct member at here !!! */
/* ------ Note: Auto create code zone start flag, don't modify or delete this line!!! ------ */
	UINT8 EmptyPara;		/* Auto create code zone, don't modify or delete! */
	UINT8 ImageSize;		/* Auto create code zone, don't modify or delete! */
	UINT8 ImageQuality;		/* Auto create code zone, don't modify or delete! */
	UINT8 AEMetering;		/* Auto create code zone, don't modify or delete! */
	UINT8 FDMetering;		/* Auto create code zone, don't modify or delete! */
	UINT8 Sharpness;		/* Auto create code zone, don't modify or delete! */
	UINT8 Saturation;		/* Auto create code zone, don't modify or delete! */
	UINT8 Contrast;		/* Auto create code zone, don't modify or delete! */
	UINT8 WBMode;		/* Auto create code zone, don't modify or delete! */
	UINT8 AEMode;		/* Auto create code zone, don't modify or delete! */
	UINT8 DriverMode;		/* Auto create code zone, don't modify or delete! */
	UINT8 SpeedyCap;		/* Auto create code zone, don't modify or delete! */
	UINT8 SpeedyCapRec;		/* Auto create code zone, don't modify or delete! */
	UINT8 ImageEffect;		/* Auto create code zone, don't modify or delete! */
	UINT8 SceneMode;		/* Auto create code zone, don't modify or delete! */
	UINT8 PhotoFrame;		/* Auto create code zone, don't modify or delete! */
	UINT8 StampMode;		/* Auto create code zone, don't modify or delete! */
	UINT8 Iso;		/* Auto create code zone, don't modify or delete! */
	UINT8 VideoSize;		/* Auto create code zone, don't modify or delete! */
	UINT8 FrameRate;		/* Auto create code zone, don't modify or delete! */
	UINT8 StampVideoMode;		/* Auto create code zone, don't modify or delete! */
	UINT8 AdvanceDenoise;		/* Auto create code zone, don't modify or delete! */
	UINT8 ImageSizeNoCard;		/* Auto create code zone, don't modify or delete! */
	UINT8 VideoSizeNoCard;		/* Auto create code zone, don't modify or delete! */
	UINT8 AudInAviRec;		/* Auto create code zone, don't modify or delete! */
	UINT8 AudInAviPlay;		/* Auto create code zone, don't modify or delete! */
	UINT8 Interval;		/* Auto create code zone, don't modify or delete! */
	UINT8 Repeat;		/* Auto create code zone, don't modify or delete! */
	UINT8 AntiShake;		/* Auto create code zone, don't modify or delete! */
	UINT8 FDMode;		/* Auto create code zone, don't modify or delete! */
	UINT8 SmileMode;		/* Auto create code zone, don't modify or delete! */
	UINT8 rtcYear;		/* Auto create code zone, don't modify or delete! */
	UINT8 rtcMonth;		/* Auto create code zone, don't modify or delete! */
	UINT8 rtcDay;		/* Auto create code zone, don't modify or delete! */
	UINT8 rtcHour;		/* Auto create code zone, don't modify or delete! */
	UINT8 rtcMinute;		/* Auto create code zone, don't modify or delete! */
	UINT8 DateStyle;		/* Auto create code zone, don't modify or delete! */
	UINT8 Language;		/* Auto create code zone, don't modify or delete! */
	UINT8 SpeakSound;		/* Auto create code zone, don't modify or delete! */
	UINT8 LightFreq;		/* Auto create code zone, don't modify or delete! */
	UINT8 SleepTime;		/* Auto create code zone, don't modify or delete! */
	UINT8 DPSImageMode;		/* Auto create code zone, don't modify or delete! */
	UINT8 DPSCopies;		/* Auto create code zone, don't modify or delete! */
	UINT8 DPSQuality;		/* Auto create code zone, don't modify or delete! */
	UINT8 DPSLayout;		/* Auto create code zone, don't modify or delete! */
	UINT8 DPSPageSize;		/* Auto create code zone, don't modify or delete! */
	UINT8 DPSPageType;		/* Auto create code zone, don't modify or delete! */
	UINT8 DPSDate;		/* Auto create code zone, don't modify or delete! */
	UINT8 DPSFileName;		/* Auto create code zone, don't modify or delete! */
	UINT8 cal_ob;		/* Auto create code zone, don't modify or delete! */
	UINT8 cal_lc;		/* Auto create code zone, don't modify or delete! */
	UINT8 cal_gma;		/* Auto create code zone, don't modify or delete! */
	UINT8 cal_ae;		/* Auto create code zone, don't modify or delete! */
	UINT8 cal_awb;		/* Auto create code zone, don't modify or delete! */
	UINT8 cal_wdr;		/* Auto create code zone, don't modify or delete! */
	UINT8 USBMode;		/* Auto create code zone, don't modify or delete! */
/* ------ Note: Auto create code zone end flag, don't modify or delete this line!!! ------ */
/* Note: You can add struct member after this line !!! */
	UINT8 ImageEdge;
	UINT8 VideoWBMode;
	UINT8 VideoQuality;
	UINT8 resCreate;
	UINT8 DisMode;
	UINT8 BlinkMode;    // 眨眼检测(0:打开 1:关闭)	
	UINT8 redEyePb;	    // 红眼校正(0:设置 1:取消)
	UINT8 tpCorrect;
	UINT8 ShutterSound; // 快门(0:打开 1:关闭)
	UINT8 StartupSound; // 开机声音(0:无 1:声音1 2:声音2 3:声音3)
	UINT8 BeepSound;    // 按键音(0:打开 1:关闭)
	UINT8 Volume;       // 音量(0:音量0 1:音量1 2:音量2 3:音量3)
	UINT8 AutoReviewTime;   // 自动回放(0:关闭 1:1s 2:3s)
	UINT8 TVMode;   // 电视(0:NTSC 1:PAL)
	//UINT8 USBMode;
	UINT8 OpenPic;  // 开机页面(0:关闭 1:系统 2:自定义)
	UINT8 ProtTime;
	UINT8 Seamless;
	UINT8 FlashLed; // 闪光灯(0:关闭 1:自动 2:打开)
	UINT8 SnapAf;   // 自动对焦(0:关闭 1:打开)

	UINT8 AFMode;   //  AF模式(0:远焦 1:近焦 2:自动对焦)
	//UINT8 LightFreq;
	//UINT8 SleepTime;
	//UINT8 FDMode;
	//UINT8 SmileMode;
	//UINT8 AntiShake;
	#if (SUPPORT_AF && SUPPORT_FIXED_AF)
	UINT8 FocalLenght;
	#endif
	UINT16 AdjustMacro;
	//UINT8 SpeakSound;
/* Note: Next is checksum , don't add anything below */
	UINT8 SunplusFlag[4];
	UINT32 Checksum;
} uiPara_t;

typedef enum uiParaInitMode_e
{
    PARA_INIT_WITH_DEF = 0x00,  // 使用默认参数进行初始化
    PARA_INIT_FROM_UDF = 0x01,  // 从用户定义文件（UDF）中加载参数进行初始化
} uiParaInitMode_t;

typedef enum uiItemOrder_e
{
    ITEM_EMPTY = 0x80,  // 空项目
    ITEM_FIRST = 0x81,  // 第一个项目
    ITEM_MAX = 0xfe,    // 最大项目
    ITEM_NULL = 0xff    // 空项目s
} uiItemOrder_t;

//-----------------------------------------------------------------------------
//Variable
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//Function declare
//-----------------------------------------------------------------------------
void appUiParaInit(uiParaInitMode_t mode);
uiPara_t* appUiParaGet(void);
UINT8 appUiItemOrderGet(UINT8 idx,UINT8 *pTable,UINT8 *length);
void appUiItemOrderCfg(void);
UINT8 appUiItemStsSet(UINT8 idx,UINT8 val);
UINT8  appUiItemStsGet(UINT8 idx);

#endif /*__APP_UI_PARA_H__*/
