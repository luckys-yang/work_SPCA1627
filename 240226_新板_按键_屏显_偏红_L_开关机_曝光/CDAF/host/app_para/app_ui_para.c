/***************************************************************************
 * File: xxx.c
 * Author: Yang
 * Date: 2024-03-07 09:11:44
 * description: 
 -----------------------------------
用户界面参数设置
 -----------------------------------
****************************************************************************/
#include "general.h"
#include "host_func_opt.h"
#include "initio.h"
#include "sp1k_rsvblk_api.h"
#include "app_init.h"
#include "app_ui_para.h"
#include "app_menu_micro.h"
#include "sp1k_util_api.h"
#include "app_sts_pwroff.h"
#include "hal_global.h"
#include "sp1k_hal_api.h"

/*不要修改该值！*/
#define UI_PARAM_SIZE 512

/*static uiPara_t xdata sPara _at_ 0x7000;*/

static UINT8 xdata uiPara[UI_PARAM_SIZE] _at_ 0x7000;   // 存储 UI 参数，存储在 xdata 区域的地址 0x7000 处
static uiPara_t xdata *xdata psPara;    // 存储在 xdata 区域，用于操作 UI 参数数组
UINT8 itemModify;   // 用于记录项目修改信息
static UINT8 xdata itemSts[] = {0}; // 用于存储项目状态信息

extern UINT8 appUDFLoad(void);
extern UINT8 xdata preVideoFrameRate;
extern xdata UINT8 frameRateIsL; /* 0:60    1:30 */
extern xdata dateStc_t DATEINFO;


//#define appUiParaInit_Print	printf
#ifndef appUiParaInit_Print
	#include "dbg_def.h"
	#define appUiParaInit_Print	/_printf
#endif

/* Public function prototypes=========================================================*/

void appUiParaInit(uiParaInitMode_t mode);
uiPara_t* appUiParaGet(void);
void appUiItemOrderCfg(void);
UINT8 appUiItemStsSet(UINT8 idx, UINT8 val);
UINT8 appUiItemStsGet(UINT8 idx);
UINT8 appUiItemOrderGet(UINT8 idx, UINT8 *pTable, UINT8 *length);

/**
* @param    mode:init mode
* @retval   None
* @brief    用户界面参数初始化 --- 2008-02-20
**/
void appUiParaInit(uiParaInitMode_t mode) USING_0
{
    UINT32 freeBuffAddr = K_SDRAM_ImagePlaybackBufAddr; // 从宏定义中获取空闲缓冲区地址
    UINT32 resID;   // 定义资源ID变量
    UINT16 num; // 定义数字变量
    UINT8 *pClean = NULL;   // 定义指向无符号字符类型的空指针

    /*从用户定义文件（UDF）中加载参数进行初始化*/
    if (mode == PARA_INIT_FROM_UDF)
    {
        // 从 nand 储备区块加载 UDF
        psPara = (uiPara_t *)&uiPara[0];    // 将psPara指向uiPara数组的起始地址

        psPara->EmptyPara = 0;  // 设置EmptyPara为0
        if(appUDFLoad() == SUCCESS) // 如果UDF加载成功
        {
            appMENULoad();  // 加载菜单
#if (SUPPORT_AF && SUPPORT_FIXED_AF)    // 【支持AF调焦 & 支持两段式固定焦距 远距/微距】
            psPara->AFMode = Status_0; // AF模式 --- 远焦
#endif
            psPara->SnapAf = Status_0; // 自动对焦 --- 关闭

            psPara->ImageEffect = PRV_EFFECT_NORNAL;    // 特效 --- 正常
            psPara->SceneMode = PRV_SCENE_AUTO; // 场景 --- 自动
            // psPara->WBMode = PRV_WB_AUTO; // 白平衡 --- 自动
            psPara->AEMode = PRV_COMP_0EV;  // 曝光 --- 0.0
            psPara->redEyePb = REDEYEPB_OFF;    // 红眼校正 --- 取消
            psPara->Iso = PRV_ISO_AUTO; // 自动 --- 自动
            psPara->Seamless = 0;   // 暂时没用到
            /* ------ Note: Demo code zone start flag, don't modify or delete this line!!! ------ */
#if FUNC_HOST_PF_PV // 【在预览模式中为照片框架提供功能支持】
            psPara->PhotoFrame = PRV_PHOTO_FRAME_OFF;   // 相框 --- 关闭
#endif
            /* ------ Note: Demo code zone end flag, don't modify or delete this line!!! ------ */
            //psPara->SleepTime = POWER_SAVE_5MIN;	// 自动关机 --- 5分钟

            if (prjDbgMode) // 处理调试模式下的USBMode设置
            {
                psPara->USBMode = USB_PCCAM;    // USB模式 -> 摄像头
            }
            psPara->FlashLed = Status_0;   // 闪光灯 --- 关闭
            psPara->resCreate = 0;  // 暂时没用到
            psPara->tpCorrect = 0;  // 触摸板用到
            preVideoFrameRate = psPara->FrameRate;  // 帧率
            frameRateIsL = psPara->FrameRate;   // 视频帧率
            appUiParaInit_Print("UDF load!\n");
            return;
        }
    }

    if(appADFLoad() == SUCCESS) // 尝试从ADF加载UI配置工具流
    {
        appUiParaInit_Print("adf load success\n");
        appMENULoad();   // 加载菜单
    }
    else    // 在FW流程中
    {
        psPara = (uiPara_t *) &uiPara[0];   // 将psPara指向uiPara数组的起始地址
        appUiParaInit_Print("adf load fail\n");
        pClean = (UINT8 *)psPara;   // 将pClean指向psPara所指向的地址
        for(num = 0; num < sizeof(uiPara_t); num++) // 遍历所有参数
        {
            *pClean = 0;
            pClean++;
        }
        // 设置默认参数
        psPara->EmptyPara = 0;
        psPara->DriverMode = PRV_SNAP_SINGLE;     // 拍摄模式 --- 关闭
        psPara->ImageSize = PRV_SIZE_3M;          // 图像大小 --- 1600x1200
        psPara->ImageQuality = PRV_QUAL_SUPER;    // 图像质量 --- 超好
        psPara->WBMode = PRV_WB_AUTO;             // 白平衡 --- 自动
        psPara->AEMetering = PRV_METER_MULTI;     // 测光模式 ---  多点测光
        psPara->FDMetering = FD_AE_OFF;           // 人脸测光 --- 关闭
        psPara->AEMode = PRV_COMP_0EV;            // 曝光 --- 0.0
        psPara->Saturation = PRV_SATURATION_HIGH; // 饱和度 --- 高
        psPara->Sharpness = PRV_EDGE_NORMAL;      // 锐度 --- 正常
        psPara->Contrast = PRV_CONTRAST_HIGH;     // 对比度 --- 高

        /* ------ Note: Demo code zone start flag, don't modify or delete this line!!! ------ */
#if FUNC_HOST_PF_PV // 【在预览模式中为照片框架提供功能支持】
        psPara->PhotoFrame = PRV_PHOTO_FRAME_OFF;
#endif
        /* ------ Note: Demo code zone end flag, don't modify or delete this line!!! ------ */
        psPara->ImageEffect = PRV_EFFECT_NORNAL;    // 摄像头效果 --- 正常
        psPara->SceneMode = PRV_SCENE_AUTO; // 场景 --- 自动
        psPara->VideoSize = MOV_SIZE_1080P; // 视频格式 --- 1080P
        psPara->FrameRate = MOV_FRM_LOW;    // 帧率 --- 低
        psPara->SpeedyCap = SPEEDY_CAPTURE_OFF; // 快速捕获枚举 --- 关闭
        psPara->SpeedyCapRec = SPEEDY_CAPTURE_REC_OFF;
        psPara->Iso = PRV_ISO_AUTO; // ISO --- 自动
        psPara->AntiShake = PRV_ANTISHAKE_ON;// 防手震 --- 开启

        psPara->Seamless = 0;   // 暂时没用到
        psPara->StampMode = PRV_STAMP_OFF;  // 日戳 --- 关闭
        psPara->StampVideoMode = PRV_STAMP_OFF; // 日戳 --- 关闭

        if (prjDbgMode)
        {
            psPara->USBMode = USB_PCCAM;    // USB模式 -> 摄像头
        }
        else 
        {
            psPara->USBMode = USB_MASS; // USB模式 --- 移动硬盘
        }
        psPara->resCreate = 1;  // 暂时没用到
        //psPara->SleepTime = POWER_SAVE_3MIN;  // 自动关机 --- 3分钟
#if FUNC_HOST_TOUCH_PANEL   // 【支持触摸面板的功能】
        psPara->tpCorrect = 1;  // 触摸板用到
#else
        psPara->tpCorrect = 0;  // 触摸板用到
#endif
        psPara->Repeat = Status_1;  // 循环 --- 否

#if (FUNC_HOST_DPS == 1)    // 【为 DPS 提供功能支持】

        psPara->DPSQuality = 0x00;   // 图像质量
        psPara->DPSLayout = 0x00;    // 排列
        psPara->DPSPageSize = 0x00;  // 纸张大小
        psPara->DPSPageType = 0x00;  // 纸张类型
        psPara->DPSDate = 0x00;      // 打印日期
        psPara->DPSFileName = 0x00;  // 文件序号
        psPara->DPSImageMode = 0x00; // 文件设定
        psPara->DPSCopies = 0x00;    // 打印份数

#endif
        // 更新RTC时间
        psPara->rtcYear = 22;   
        psPara->rtcMonth = 1 - 1;
        psPara->rtcDay = 1 - 1;
        psPara->rtcMinute = 0;
        psPara->rtcHour = 0;

        psPara->DateStyle = Status_2;  // 日期格式 --- YYYY/MM/DD


        psPara->SpeakSound = Status_1; // 声音 --- 开启
        psPara->FlashLed = Status_0;   // 闪光灯 --- 关闭

        psPara->TVMode = TVMODE_NTSC;   // 电视模式 --- NTSC
        psPara->BeepSound = BEEP_ON;    // 按键声音 --- 开启 
        psPara->ShutterSound = SHUTTER_ON;  // 快门 --- 开启
        psPara->Language = S_CHINESE;   // 语言 --- 中文简体
        psPara->Volume = VOLUME_10;      // 音量 --- 10
        psPara->BlinkMode = BLINKMODE_OFF;  // 眨眼检测 --- 关闭
        psPara->SmileMode = SMILEMODE_OFF;  // 微笑拍照 --- 关闭
        psPara->redEyePb = REDEYEPB_OFF;    // 红眼校正 --- 关闭
        /*psPara->WDRMode=WDRMODE_OFF; */
        psPara->FDMode = FDMODE_OFF;    // 脸部检测 --- 关闭
        psPara->DisMode = DISMODE_OFF;  // DIS模式 --- 关闭
        psPara->AdvanceDenoise = REC_ADVANCEDENOISE_OFF;    // 高级降噪 --- 关闭
        psPara->StartupSound = STARTSOUND_1;    // 开机声音 --- 声音1
        psPara->LightFreq = PRV_FREQ_60; // 电源频率 --- 60Hz
        psPara->OpenPic = PB_OPENPIC_DEFAULT;   // 开机画面 --- 系统
        psPara->AutoReviewTime = QUICKVIEW_1S;  // 自动回放 --- 1S
        psPara->SleepTime = POWER_SAVE_OFF; // 自动关机 --- 关闭
// 【支持屏保】        
#if SUPPORT_PANEL_PROTECT
        psPara->ProtTime = PANEL_TIME_OFF;  // 屏保时间 --- 关闭
#endif
        frameRateIsL = psPara->FrameRate;
        psPara->SunplusFlag[0] = 'S';
        psPara->SunplusFlag[1] = 'U';
        psPara->SunplusFlag[2] = 'N';
        psPara->SunplusFlag[3] = 'P';

        psPara->SnapAf = Status_0; // 自动对焦 --- 关闭

        if(!psPara->AdjustMacro)
        {
            psPara->AdjustMacro = 431;//480;//496;
// 【支持AF调焦】 && 【支持两段式固定焦距】            
#if (SUPPORT_AF && SUPPORT_FIXED_AF)
            psPara->AFMode = Status_0; // AF模式 --- 远焦
#endif
        }
    }

    DATEINFO.Day = psPara->rtcDay + 1;
    DATEINFO.Hour = psPara->rtcHour;
    DATEINFO.Minute = psPara->rtcMinute;
    DATEINFO.Month = psPara->rtcMonth + 1;
    DATEINFO.Year = psPara->rtcYear;
    sp1kHalWriteRTC(&DATEINFO);

    resID = 0x0d; //.\a\open.jpg
    sp1kNandRsvRead(resID, freeBuffAddr << 1);
    // resID = 0x800; //.\b\open.jpg
    // sp1kNandRsvWrite(resID, freeBuffAddr << 1);
}

/**
* @param    None
* @retval   None
* @brief    获取UI参数指针
**/
uiPara_t* appUiParaGet(void)
{
	return psPara; // 返回UI参数指针
}

/**
* @param    None
* @retval   None
* @brief    配置UI项目顺序
**/
void appUiItemOrderCfg(void)
{
	UINT8 idx;
	idx = uiPara[0x100]; // 从UI参数数组中获取索引值

	//printf("@@idx--->0x%bx\n",idx); // 打印调试信息

	if (idx >= ITEM_FIRST && idx != ITEM_NULL) // 如果索引值大于等于第一个项并且不等于空项
	{
		itemModify = 1; // 设置项修改标志为1
	}
	else
	{
		itemModify = 0; // 否则设置项修改标志为0
	}
}

/**
* @param    idx: 项目索引
* @param    val: 状态值
* @retval   None
* @brief    设置UI项目的状态值
**/
UINT8 appUiItemStsSet(UINT8 idx, UINT8 val)
{
	if (!itemModify) // 如果项目未被修改
		val = 0; // 将状态值设为0

	if (idx < ITEM_EMPTY || idx > ITEM_MAX) // 如果项目索引小于空项目索引或大于最大项目索引
	{
		return 0; // 返回失败
	}

	itemSts[idx] = val; // 设置项目状态值
}

/**
* @param    idx: 项目索引
* @retval   返回值 = 成功 / 失败
* @brief    获取UI项目状态
**/
UINT8 appUiItemStsGet(UINT8 idx)
{
	if (!itemModify) // 如果项目未被修改
		return 0; // 返回0

	if (idx < ITEM_EMPTY || idx > ITEM_MAX) // 如果项目索引小于空项目索引或大于最大项目索引
	{
		return 0; // 返回0
	}

	return itemSts[idx]; // 返回项目状态值
}

/**
* @param    idx: 表示要获取顺序的项目的索引
* @param    *pTable: 是一个指向存储项目顺序的数组的指针
* @param    *length: 是一个指向存储项目数量的变量的指针，函数会修改这个变量的值以返回项目的数量
* @retval   返回值 = 成功 / 失败
* @brief    获取UI项目的顺序
**/
UINT8 appUiItemOrderGet(UINT8 idx, UINT8 *pTable, UINT8 *length)
{
	UINT8 *pDate = &uiPara[0x100]; // 指向UI参数数组的起始地址
	UINT8 num = 0; // 计数器

	if (itemModify) // 如果项目已被修改
	{
		if (idx < ITEM_EMPTY || idx > ITEM_MAX) // 如果项目索引小于空项目索引或大于最大项目索引
		{
			return FALSE; // 返回失败
		}
		if (*pDate == ITEM_NULL) // 如果第一个项目索引为ITEM_NULL
        { 
			return FALSE; // 返回失败
		}
		while (*pDate != idx) // 查找索引值
		{
			pDate++;
			if (*pDate == ITEM_NULL)  // 如果找到末尾
            {
				return FALSE; // 返回失败
			}
		}

		pDate++;
		while (*pDate < ITEM_EMPTY) // 如果索引值小于空项目索引
		{
			*pTable = *pDate; // 将项目索引存入表中
			pTable++;
			pDate++;
			num++;
		}
		*length = num; // 更新项目数量
		return TRUE; // 返回成功
	}
	return FALSE; // 返回失败
}


