#include "general.h"
#include "initio.h"
#include "solution.h"
#include "app_main.h"
#include "os_msg.h"
#include "app_msg.h"
#include "app_still_view.h"
#include "app_cmd.h"
#include "App_menu_micro.h"
#include "App_ui_osd.h"
#include "App_ui_para.h"
#include "Timer.h"
#include "func_ptr.h"
//#include "exifhdr.h"
#include "exif_def.h"//xl
#include "exiffunc.h"
#include "app_dev_storage.h"
#include "app_dev_usb.h"
#include "app_dev_disp.h"
#include "app_sts_pwroff.h"
#include "app_pwr_mgr.h"
#include "app_battery.h"

#include "app_menu_tab.h"
#include "app_menu_core.h"//6-11
#include "key_scan.h"//jintao.liu 2008-4-7 mask burst tele/wide for demo
#include "sp1k_osd_api.h"
#include "sp1k_cdsp_api.h"
#include "sp1k_pv_api.h"
#include "sp1k_snap_api.h"
#include "sp1k_disp_api.h"
#include "sp1k_ae_api.h"
#include "sp1k_af_api.h"
#include "sp1k_awb_api.h"//wangjg++
#include "sp1k_rsvblk_api.h"
#include "sp1k_hal_api.h"
#include "sp1k_util_api.h"
#include "sp1k_vfs_api.h"
#include "app_osd_icon.h"
#include "sp1k_aud_api.h"//jintao.liu 2008.05.05 for self snap beep
#include "sp1k_pb_api.h"
#include "sp1k_calibration_api.h"
#include "sp1k_exif_api.h"
#include "sp1k_cdsp_api.h"
#include "app_audio_play.h"
#include "uiflow.h"
#include "cardlink.h"//6-11
#include "doslink.h"
#include "cardlink.h"
#include "pb_general.h"
#include "app_still_play.h"
#include "pb_photoframe.h"
#include "sp1k_pb_api.h"
#include "app_menu_event.h"
#include "smcfunc.h"
//#include "dos32.h"
#include "sp1k_dos_api.h"
#include "hal_dram.h"
#include "hal_gprm.h"
//#include "motion_det.h"
#include "sp1k_md_api.h"
#include "sp1k_disk_api.h"


#include "sp1k_pv_api.h"
#include "sp1k_front_api.h"
#include "macro_expand.h"
#include "dbg_def.h"
#include "sp1k_calibration_api.h"
#include "hal_front.h"
#include "sp1k_cdsp_api.h"
#include "app_ui_osd.h"
#include "sp1k_fd_api.h"
#include "osd_api.h"
#include "sp1k_dist_api.h"
#include "app_dev_disp.h"
#include "sp1k_dcf_api.h"
#include "app_video_view.h"
#include "app_init.h"
#include "app_menu.h"
#include "app_cal_api.h"
#include "sp1k_stamp_api.h"
#include "gpio_api.h"
#include "front_logi.h"
#include "rsc_id.h"
#include "sp1k_video_api.h"
#include "video_clip.h"
#include "video_capture.h"
#include "sp1k_video_capture.h"
#include "video_capture.h"
#include "sp1k_panorama_api.h"
#include "sp1k_gpio_api.h"

/*-------------------- USER CODE: Include Begin --------------------*/
#include "User_Config.h"
/*-------------------- USER CODE: Include End --------------------*/

#define __FILE	  __FILE_ID_APP_STS_STILLVIEW__

#define G_HSize_P K_Preview_Width
#define G_VSize_P K_Preview_Height

#define PV_3A_WAIT_CNT_MAX ((UINT16)10000) /* for error handle */

#define AWB_SPEED 2             // wangjg - invalid
#define AWB_SPEED_FAST_ONCE 255 // wangjg - invalid

#define STILL_ZOOM_FACTOR_LOOKUP_TABLE 0

//#define SCENE_SET_PRINT printf
#ifndef SCENE_SET_PRINT
	#define SCENE_SET_PRINT /_printf
#endif

#ifdef AAA_ON
#define SNAP_WAIT_AEAWBREADY_OPTION
#else
#undef SNAP_WAIT_AEAWBREADY_OPTION
#endif
UINT16 stillZFctor=100;
#if (STILL_NEW_ZOOM==1)
UINT16 stillCurrLog=0;
#else
UINT16 stillCurrLog=100;
#endif

UINT32 count = 0;   // 读取的定时器值
UINT32 speedyCapRecTime = 0;
UINT8 affarflag = 0;
UINT8 versionclearflag = 0;

UINT8 afbackfaruiclearflag = 0;
	
extern UINT16 videoZFctor;
extern UINT16 videoCurrLog;

extern UINT8 flag_menu_back_videoview;

extern UINT8 enterMacroadjust;
extern UINT8 enterVersionshow;
extern UINT8 enterPCCamflag;


extern UINT8 cur_idx;
static UINT8 adjust_macro_sta = 0;
UINT8 adjust_macro_to_menu = 0;
UINT8 selfsnapbackfarflag = 0;


//#if (STILL_NEW_ZOOM == 0)
#if (STILL_ZOOM_FACTOR_LOOKUP_TABLE == 1)
appPvZoom_t xdata PvZoomCtrl = 
{
	100,
	0,
	400,//zoom in 4X
	100,
	100,//zoom in 4X
	40,
	100,//zoom in 4X
	2,//4,//3*2,//zoom in 8X
};

//power (0.03333++, 1.6) curve
code UINT16 StillZoomFactor[31] = 
{
	100,
	101,
	103,
	107,
	111,
	117,
	122,
	129,
	136,
	143,
	151,
	160,
	169,
	178,
	188,
	198,
	209,
	220,
	232,
	244,
	256,
	269,
	282,
	296,
	309,
	324,
	338,
	353,
	368,
	384,
	400,
};
/* 8x
code UINT16 StillZoomFactor[61] = {

	100,
	101,
	103,
	105,
	109,
	113,
	117,
	122,
	127,
	133,
	139,
	146,
	153,
	160,
	168,
	176,
	184,
	193,
	201,
	211,
	220,
	230,
	240,
	250,
	261,
	272,
	283,
	295,
	306,
	318,
	330,
	343,
	356,
	368,
	382,
	395,
	409,
	422,
	437,
	451,
	465,
	480,
	495,
	510,
	526,
	541,
	557,
	573,
	589,
	606,
	622,
	639,
	656,
	673,
	691,
	709,
	726,
	744,
	763,
	781,
	800
};
*/
#else
appPvZoom_t xdata PvZoomCtrl = 
{
	100,
	0,
	400,//zoom in 4X
	100,
	100,//zoom in 4X
	25,
	100,//zoom in 4X
	5,//2,//4,//3*2,//zoom in 8X
};
code UINT16 StillZoomFactor[1]={0};
#endif
//#endif


UINT8 userMeterEn;	/* default : 0 -> disable */
UINT8 userDefineMeter[8 * 8] = 
{
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
};

UINT8 aeWindowInformation[8 * 8] = 
{
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
};
static UINT8 S_imgsizeflag;
UINT8 S_imgRatioChg;
UINT8 imgResolutionSet;
//static UINT8 poweron = 0;
extern UINT8 xdata storageReadyflag;
//extern xdata UINT8 OsdMaxX;
//extern xdata UINT8 OsdMaxY;
extern UINT32 RemainPictureNO;
//extern xdata 	UINT32 	G_DOS_FileSize;
//extern xdata UINT8 AdapterCharge; //  1: USB contect power adapter charge   0:USB contect PC
extern UINT8 AutoTestMode;
extern xdata UINT16 GlobleGain;
extern UINT8 ForceAEDo;
#if LIFE_SPAN_TEST
extern UINT8 snapRdy;
#endif
UINT8 isUpdateNum=0;
extern xdata UINT8 G_calibrationMode;
extern xdata UINT8 calibrationAEOn;
static UINT8 xdata pvZoomSts;
UINT16 xdata pvWaitCnt;

UINT8 xdata G_SceneMode = 0;
//UINT8 afdoingflag = 0;
//UINT8 afdoingsnapflag = 0;

#if MSG_AUTO_SEND
extern UINT8
frontAeTest(
	UINT8 id,
	UINT8 userIdx
);
#endif
UINT8 appStillViewAEAWBWait(void);
UINT8 appSnapEnvChk(void);
UINT8 appSnapParaLoad(void);
UINT8 appSnapPreProc(void);
UINT8 appSnapPostProc(UINT16 msg);
UINT8 menuDistortionSet (UINT8 layer, UINT8 item, UINT16 msg);

#if (FUNC_HOST_AF_FIXTURE == 1)
UINT8 appAfWinSet(UINT8 En);
#endif

/* ------ Note: Demo code zone start flag, don't modify or delete this line!!! ------ */
extern xdata UINT8 exifVlcAtOdd;
void appPvPhotoFrameRecover(void);
UINT8 appPvPhotoFrameResLoad(UINT8 idx, UINT16* resW, UINT16* resH);
/* ------ Note: Demo code zone end flag, don't modify or delete this line!!! ------ */

UINT8 showChargingIcon = Flag_false;

UINT8 press_snap_check_flag = Flag_false;   // 快照按键检测标志位

UINT8 key_repeate_check_flag = Flag_false;  // 按键长按检测标志位

UINT8 key_enter_AF_menu_flag = Flag_false;

UINT8 af_timer_start = Flag_false;   // 自动对焦计时器开始标志位
UINT8 af_timer_out = Flag_false; // 自动对焦计时器结束标志位

#if SUPPORT_AWB_ADJ // 【定时检测亮度是否发生变化，如果发生变化则打开AWB，否则关闭AWB】
UINT8 awb_auto_disable_flag = 0;
UINT8 awb_adj_start_flag = 0;
#endif

extern xdata UINT8 AdapterCharge; //  1：USB 连接电源适配器充电 0：USB 连接电脑
extern UINT8 power_up_usb_check_flag;

extern UINT16 LCD_WIDTH;
extern UINT16 LCD_HEIGHT;
extern UINT16 MENU_CHAR_WIDTH;
extern UINT16 MENU_CHAR_HEIGHT;
extern UINT8 cur_idx;

/* Public function prototypes=========================================================*/

extern void app_quick_delete_dialog(UINT8 cur_idx);
extern void appVideoStampSet(void);

void app_adjust_macro_sta_set(UINT8 sta);
UINT8 app_adjust_macro_sta_get(void);
void uiShowOSDSelfSnapCntDown(UINT8 countDown,UINT8 enableShow);
void appSelfSnapMSGSend(void);
void appSelfSnapHandle(void);
void appStillPreviewReset(void);
UINT8 appDriveModeSet(void);
UINT8 appEffectSet(UINT8 effect);
UINT8 appContrastSet(UINT8 ContrastValue);
UINT8 appSaturationSet(UINT8 SaturationValue);
UINT8 appSharpnessSet(UINT8 SharpnessValue);
UINT8 appSceneSet(UINT8 scene);
UINT8 appWBSet(UINT8 WBValue);
UINT8 appSpeedyCaptureConfig(void);
UINT8 appSpeedyCaptureRecConfig(void);
void appStillView(UINT16 msg);
void appStillViewInit(UINT8 InitMode);
UINT8 appStillQualitySet(void);
void appStillQuickViewSet(void);
UINT8 appStillResolutionSet(UINT8 resolution);


typedef struct
{
    UINT8 sts;
    UINT8 ret;  // 存储触发快照流来捕捉图像返回值(0:ok 1:error)
    UINT8 Xoffset;  // 绘图或显示的水平偏移量
    UINT8 stillZoomRptCnt;
} vStillViewParameter_st;

vStillViewParameter_st vStillViewParamater = 
{
    0,
    0,
    0,
    0
};

/**
* @param    msg: 消息枚举类型 ( @ref appMsgList_t @ref btnMsgList_t )
* @retval   None
* @brief    拍照视图
**/
void appStillView(UINT16 msg)
{
#if FUNC_HOST_MD    // 【支持运动检测功能】
    UINT8 mdEn;
#endif

#if FUNC_HOST_IGBT_FLASH    // 【支持 IGBT 功能】
    UINT8 curGid;
#endif

#if FUNC_HOST_PANORAMA  // 【支持全景功能】
    static SINT16 panoramaSumOfGMV[2];
    static SINT16 panoramaGMVByFrame[2];
    static UINT8 panoramaStatus;
#endif

    UINT16 adcVal;
    static UINT8 zoomRptCnt = 0;
    uiPara_t *puiPara;  // UI参数指针

    /*-----------变量初始化---------------*/
    vStillViewParamater.sts = 0;
    vStillViewParamater.ret = 0;
    vStillViewParamater.Xoffset = 0;

	puiPara = appUiParaGet();   // 获取UI指针参数

	switch (msg) 
    {
    case SP1K_MSG_STATE_INIT: // 【初始化消息】
    {
        UINT8 LastMode; // 上一个状态
        UINT8 InitMode; // 初始化模式

        /***************Just for QA test @start***************************************/
        if (AutoTestMode == 0x55)
        {
            appStillOsdInit(1); // 初始化拍照模式下界面
        }
        /***************Just for QA test @end***************************************/

        LastMode = appPreStateGet(0); // 获取前一个状态

        switch (LastMode)
        {
        case APP_STATE_MENU: // 【菜单状态】
        {
            InitMode = SP1K_STILL_VIEW_INIT_SMART;

            /*
            前一个状态是 录像视图 || 音频录制状态 || 有更新数据 || 图像分辨率设置
            */
            if ((appPreStateGet(1) == APP_STATE_VIDEO_VIEW) ||
                (appPreStateGet(1) == APP_STATE_AUDIO_REC) ||
                appPvForceUpdateGet() ||
                (Flag_true == imgResolutionSet))
            {
                imgResolutionSet = Flag_false;
                appPvForceUpdateOn(Flag_false); // 不强制更新UI
                InitMode = SP1K_STILL_VIEW_INIT_FULL;
                __FUNC_TRACK__;
            }
            break;
        }
#if 0
    case APP_STATE_POWER_ON:
    {
        if (1 == bUsbPwr && !prjDbgMode) // 如果热键开机，则切换到校准菜单
        {
            sp1kFdDisable();
            osMsgPost(SP1K_MSG_STATE_TO_MENU);
        }

        if ((appSDPlugStatusGet() == 0) && (sp1kDiskTypeGet() == DEV_TYPE_DISK_SPI) && (sp1kDosRamStsGet() == FALSE))
        {
            InitMode = SP1K_STILL_VIEW_INIT_FULL;
            break;
        }
        if (sp1kSuperPreviewStatusGet(0) || puiPara->ImageSize == PRV_SIZE_2M_16_9 || puiPara->ImageSize == PRV_SIZE_7M_16_9 || puiPara->ImageSize == PRV_SIZE_1M_16_9)
        {
            InitMode = SP1K_STILL_VIEW_INIT_FULL;
            break;
        }
        InitMode = SP1K_STILL_VIEW_INIT_SMART;
        break;
    }
#endif
        default:
        {
            InitMode = SP1K_STILL_VIEW_INIT_FULL;
            break;
        }
        }

        /*
        jintao.liu 2008-2-26
        add for resolve mantis bug 20099
        jintao.liu 2008-3-31 add for electronic freq
        */
        appElecFreqSet(); // 设置电子频率
        // appSceneSet(puiPara->SceneMode); // 场景
        appStillViewInit(InitMode); // 拍照视图初始化
        appAutoOffTimeSet();        // 自动关机时间设置 jintao.liu 2008-3-28 add for mantis 21604
        appStillQualitySet();       // 设置静态图像质量

#if MSG_AUTO_SEND
        {
            UINT8 osdMaxX, osdMaxY;
            static UINT8 ISize = 0;
            ISize++;

            if (PRV_SIZE_MAX == ISize)
            {
                ISize = 0;
            }
            puiPara->ImageSize = ISize;
            sp1kOsdLayoutAttrGet(&osdMaxX, &osdMaxY);
            uiUpdateImgSize(osdMaxX - 9 + vStillViewParamater.Xoffset, 0, 0);
            uiRemainPixInit();   // mantis #26379
            uiUpdateRemainPix(); // mantis #26379
            sp1kHalWait(2000);

            osMsgPost(SP1K_MSG_KEY_PRESS_S2);
        }
#endif

#if FUNC_HOST_TOUCH_PANEL // 【支持触摸面板的功能】
        if ((puiPara->tpCorrect == 1) && (0 == bUsbPwr))
        {
            appCalTouchPanel();
        }
#endif

        showChargingIcon = 1;
#if (SUPPORT_AF && SUPPORT_SNAP_AF)
        if (!puiPara->SnapAf) // 自动对焦 关闭状态则
        {
#if (SUPPORT_AF && SUPPORT_FIXED_AF)
            if (puiPara->AFMode) // 近焦/自动对焦
            {
                AF_Set(431); // puiPara->AdjustMacro);//431);
            }
            else // 远焦
            {
                AF_Set(0);
                appAFCtrl(0);
            }
#else
            AF_Set(0);
            appAFCtrl(0);
#endif
        }
#endif
        break;
    }

    case SP1K_MSG_KEY_PRESS_S1: // 【S1按键按下消息】
    {
        // 设置菜单中的畸变（distortion）参数
        menuDistortionSet(0, 0, 0); // wdd  @只为节省内存
        break;
    }

    case SP1K_MSG_KEY_PRESS_UP: // 【上按键按下消息】
    {
        // 应用程序调整宏获取成功 && 进入应用程序调整宏标志位 为真
        if (app_adjust_macro_sta_get() && (Flag_true == enterMacroadjust))
        {
            cur_idx = !cur_idx;               // 当前选择索引取反
            app_quick_delete_dialog(cur_idx); // 快速删除对话框
            break;
        }

        if (Status_0 == puiPara->FlashLed) // 闪光灯 -> 关闭
        {
            puiPara->FlashLed = Status_1; // 闪光灯 -> 自动
        }
        else if (Status_1 == puiPara->FlashLed) // 闪光灯 -> 自动
        {
            puiPara->FlashLed = Status_0; // 闪光灯 -> 关闭
        }
        appSnapFlashLed(1);                // 快门时闪光灯控制 --- 常开
        uiUpdateFlashLed(LCD_WIDTH, 0, 0); // 根据当前闪光灯模式更新相应的图标显示状态
        break;
    }

    case SP1K_MSG_KEY_PRESS_DOWN: // 【下按键按下消息】
    {
#if 0
    // 特效 != 正常 ||  模式 != 关闭
    if ((puiPara->ImageEffect != PRV_EFFECT_NORNAL) || (puiPara->DriverMode != PRV_SNAP_SINGLE))
    {
        if (puiPara->DriverMode != PRV_SNAP_SINGLE)  // 拍摄模式 != 关闭
        {
            puiPara->DriverMode = PRV_SNAP_SINGLE;   // 拍摄模式 -> 关闭
            uiUpdateSnapMode(LCD_WIDTH, 0, 0);  // 在 PV 模式下刷新快照模式图标
        }
        else if (puiPara->ImageEffect != PRV_EFFECT_NORNAL) // 特效 != 正常
        {
            puiPara->ImageEffect--;  // --

            appEffectSet(puiPara->ImageEffect);  // 设置相应的摄像头效果
            uiUpdateEffect(LCD_WIDTH, 0, 0);    // 在PV模式下刷新快照效果图标

            if (PRV_EFFECT_NORNAL == puiPara->ImageEffect)   // 特效 == 正常
            {
                puiPara->DriverMode = PRV_SNAP_SELF_5S;  // 拍摄模式 -> 自拍5S
                uiUpdateSnapMode(LCD_WIDTH, 0, 0);  // 在 PV 模式下刷新快照模式图标
            }
        }
    }
    else // 特效 == 正常 ||  模式 == 关闭
    {
        puiPara->PhotoFrame++;   // 相框++

        if (PRV_PHOTO_FRAME_MAX == puiPara->PhotoFrame)  // 到达最大
        {
            puiPara->PhotoFrame = PRV_PHOTO_FRAME_OFF;   // 相框状态 -> 关闭
        }

        if (puiPara->PhotoFrame != PRV_PHOTO_FRAME_OFF)  // 相框非关闭
        {
            sp1kOsdClear(0);    // 清除OSD显示区域
        }
        else
        {
            sp1kPvPhotofrmEnable(PV_PhotoFrame_Disable);    // 启用或禁用PV模式下的相框
            appStillOsdInit(1); // 初始化拍照模式下界面
        }
        appPvPhotoFrameSet(puiPara->PhotoFrame); // 设置相框状态
    }
#endif
        break;
    }

    case SP1K_MSG_KEY_PRESS_PB: // 【PB按键按下消息】
    {
        // 应用程序调整宏获取成功 && 进入应用程序调整宏标志位 为真
        // if (app_adjust_macro_sta_get() && enterMacroadjust)
        // {
        //     cur_idx = !cur_idx; // 当前选择索引取反
        //     app_quick_delete_dialog(cur_idx);   // 快速删除对话框
        //     break;
        // }

        // 场景 != 自动
        if (puiPara->SceneMode != PRV_SCENE_AUTO)
        {
            puiPara->SceneMode = PRV_SCENE_AUTO; // 场景 -> 自动
            uiSceneModeUpdate();                 // 更新场景模式图标
        }
        else // 场景 == 自动
        {
            if (PRV_SNAP_BURST == puiPara->DriverMode) // 拍摄模式状态 -> 连拍
            {
                puiPara->DriverMode = PRV_SNAP_SINGLE; // 拍摄模式状态 -> 关闭
            }
            else
            {
                puiPara->DriverMode = PRV_SNAP_BURST; // 拍摄模式状态 -> 连拍
            }
            // appDriveModeSet();  // 根据不同的拍摄模式设置相应的拍摄参数
            uiUpdateSnapMode(LCD_WIDTH, 0, 0); // 刷新PV模式下的拍摄模式图标
        }
        // 拍摄模式状态 != 关闭
        if (puiPara->DriverMode != PRV_SNAP_SINGLE)
        {
            AF_Set(0);                 // AF关闭
            appAFCtrl(AF_CTRL_DISABL); // AF自动对焦电源控制 --- 关闭
        }
#if 0
    if (puiPara->PhotoFrame == PRV_PHOTO_FRAME_OFF)  // 相框状态 -> 关闭
    {
        // 拍摄模式状态 -> 关闭 && 摄像头效果 -> 正常
        if ((puiPara->DriverMode == PRV_SNAP_SINGLE) && (puiPara->ImageEffect == PRV_EFFECT_NORNAL))
        {
            puiPara->DriverMode = PRV_SNAP_SELF_5S;  // 拍摄模式状态 -> 自拍5S
            uiUpdateSnapMode(LCD_WIDTH, 0, 0);  // 刷新PV模式下的拍摄模式图标
        }
        else    // 拍摄模式状态 != 关闭 || 摄像头效果 != 正常
        {
            puiPara->DriverMode = PRV_SNAP_SINGLE;   // 拍摄模式状态 -> 关闭
            puiPara->ImageEffect++;  // 摄像头效果++

            if (puiPara->ImageEffect == PRV_EFFECT_MAX)  // 到达最大值则
            {
                puiPara->ImageEffect = PRV_EFFECT_NORNAL;    // 摄像头效果正常
                puiPara->DriverMode = PRV_SNAP_SINGLE;   // 拍摄模式状态 -> 关闭
            }
            appEffectSet(puiPara->ImageEffect);  // 输入的效果参数设置相应的摄像头效果
            uiUpdateEffect(LCD_WIDTH, 0, 0);    // 在PV模式下刷新快照效果图标
        }
    }
    else    // 相框状态 != 关闭
    {
        puiPara->PhotoFrame--;  // 相框状态--

        // 相框状态 != 关闭
        if (puiPara->PhotoFrame != PRV_PHOTO_FRAME_OFF)
        {
            sp1kOsdClear(0);    // 清除OSD显示区域
        }
        else    // 相框状态 == 关闭
        {
            sp1kPvPhotofrmEnable(PV_PhotoFrame_Disable);    // 启用或禁用PV模式下的相框
            appStillOsdInit(1); // 初始化拍照模式下界面
        }

        appPvPhotoFrameSet(puiPara->PhotoFrame);    // 设置相框状态
    }

    // 相框状态 -> 关闭
    if (puiPara->PhotoFrame == PRV_PHOTO_FRAME_OFF) 
    {
        if (puiPara->DriverMode == PRV_SNAP_SINGLE) // 拍摄模式状态 -> 关闭
        {
            puiPara->DriverMode = PRV_SNAP_SELF_5S; // 拍摄模式状态 -> 自拍5S
        }
        else    // 拍摄模式状态 != 关闭
        {
            puiPara->DriverMode = PRV_SNAP_SINGLE;  // 拍摄模式状态 -> 关闭
        }

        uiUpdateSnapMode(LCD_WIDTH, 0, 0);  // 刷新PV模式下的拍摄模式图标
    }
    else // 相框状态 != 关闭
    {
        puiPara->PhotoFrame--;  // 相框状态--

        if (puiPara->PhotoFrame != PRV_PHOTO_FRAME_OFF) // 相框状态 != 关闭
        {
            sp1kOsdClear(0);    // 清除OSD显示区域
        }
        else // 相框状态 == 关闭
        {
            sp1kPvPhotofrmEnable(PV_PhotoFrame_Disable);    // 启用或禁用PV模式下的相框
            appStillOsdInit(1); // 初始化拍照模式下界面
        }

        appPvPhotoFrameSet(puiPara->PhotoFrame);    // 设置相框状态
    }
#endif
        break;
    }

    case SP1K_MSG_KEY_PRESS_BACK: // 【返回按键按下消息】
    {
        // 应用程序调整宏获取成功 && 进入应用程序调整宏标志位 为真
        if (app_adjust_macro_sta_get() && (Flag_true == enterMacroadjust))
        {
            cur_idx = !cur_idx;               // 当前选择索引取反
            app_quick_delete_dialog(cur_idx); // 快速删除对话框
            break;
        }

        // 场景 != 自动
        if (puiPara->SceneMode != PRV_SCENE_AUTO)
        {
            puiPara->SceneMode = PRV_SCENE_AUTO; // 场景 -> 自动
            uiSceneModeUpdate();                 // 更新场景模式图标
        }

        // 拍摄模式状态 != 连拍
        if (puiPara->DriverMode != PRV_SNAP_BURST)
        {
            puiPara->DriverMode++; // 拍摄模式状态++

            if (PRV_SNAP_BURST == puiPara->DriverMode) // 拍摄模式状态 == 连拍
            {
                puiPara->DriverMode = PRV_SNAP_SINGLE; // 拍摄模式状态 -> 关闭
            }
        }
        // appDriveModeSet();  // 根据不同的拍摄模式设置相应的拍摄参数
        uiUpdateSnapMode(LCD_WIDTH, 0, 0); // 刷新PV模式下的拍摄模式图标
        break;
    }

    case SP1K_MSG_KEY_PRESS_MODE: // 【模式按键按下消息】
    {
#if 0 // 2024.3.17
            afbackfaruiclearflag = 0;
            versionclearflag = 0;

            uiOsdVerDisp(FW_VERSION, 0); // 显示垂直OSD信息 版本信息
#endif

        if (Flag_true == enterMacroadjust) // 进入应用程序调整宏标志位 为真
        {
            enterMacroadjust = Flag_false;
        }

        sp1kDispImgWinEn(0);                                 // 启用预览层 --- 禁用
        sp1kHalWait(200);                                    // 延时
        sp1kPreviewCropSizeSet(1280, 960);                   // 设置预览裁剪尺寸
        sp1kPreviewSet(0, 1280, 720, sp1kPvZoomFctGet(), 1); // 设置预览参数
        sp1kHalWait(100);                                    // 延时

// 【录像EV降低】
#if SUPPORT_VIDEO_EV
        puiPara->AEMode = PRV_COMP_N03EV; // 曝光
        appEVSet(puiPara->AEMode);        // 设置曝光模式
#endif

        osMsgPost(SP1K_MSG_STATE_TO_VIDEO_VIEW); // 发送 录像视图状态消息
        flag_menu_back_videoview = 0;
        break;
    }

    // case SP1K_MSG_KEY_PRESS_TELE: // 【变焦放大按键按下消息】
    case SP1K_MSG_KEY_PRESS_ZOOM_IN: // 【放大按键按下消息】
    {
        // if (puiPara->DriverMode != PRV_SNAP_SINGLE)  // 拍摄模式状态 != 关闭
        // {
        //     break;
        // }

        vStillViewParamater.stillZoomRptCnt = 0;
        appOsdModeSw(0); // 控制 OSD 模式 --- 重置为带 OSD 的 LCD 模式
        pvZoomSts |= 0xf0;

        if (!sp1kSnapParamGet(SP1K_SNAP_PARAM_PHOTO_FRM_OPT)) // 获取快照参数
        {
            sp1kAeSetModeEn(AE_DISABLE); // 启用/禁用 AE 控制 --- 禁用
            sp1kAwbDisable();            // 禁用 awb
        }

        sp1kFdDisable(); // 启用或禁用脸部检测 --- 禁用

        // 快速捕获 -> 打开 ||
        if ((SPEEDY_CAPTURE_ON == puiPara->SpeedyCap) || (SPEEDY_CAPTURE_REC_ON == puiPara->SpeedyCapRec))
        {
#if (VIDEO_NEW_ZOOM == 1)
            sp1kVideoZoom3APreSet();
#endif
        }
        else
        {
#if (STILL_NEW_ZOOM == 1)
            if (!sp1kSnapParamGet(SP1K_SNAP_PARAM_PHOTO_FRM_OPT)) // 获取快照参数
            {
                sp1kPvZoom3APreSet();
            }
#endif
        }
        break;
    }
    // case SP1K_MSG_KEY_RELEASE_TELE: // 【变焦放大按键释放消息】
    case SP1K_MSG_KEY_REPEATE_ZOOM_IN: // 【放大按键长按消息】
    case SP1K_MSG_KEY_RELEASE_ZOOM_IN: // 【放大按键释放消息】
    {
        // if (puiPara->DriverMode != PRV_SNAP_SINGLE)  // 拍摄模式状态 != 关闭
        // {
        //     break;
        // }

        vStillViewParamater.stillZoomRptCnt = 0;
        pvZoomSts &= ~0xf0;

        // 快速捕获 -> 打开 ||
        if ((SPEEDY_CAPTURE_ON == puiPara->SpeedyCap) || (SPEEDY_CAPTURE_REC_ON == puiPara->SpeedyCapRec))
        {
#if (VIDEO_NEW_ZOOM == 1)
            appVideoViewTele(); // 视频键启动过程
            sp1kVideoSmoothZoomSet(0);
#else
            UINT8 ZfStatus;

            appVideoViewTele();                          // 视频键启动过程
            if (sp1kVideoRecStatusGet() == VID_REC_IDLE) // 录像状态获取 -> 空闲
            {
                ZfStatus = sp1kVideoRecZoomFacSet(video_zoomFactor, 0); // 设置当前有效缩放
            }
            else // 录像状态获取 -> 录像中/暂停
            {
                ZfStatus = sp1kVideoRecZoomFacSet(video_zoomFactor, 1); // 当前无效缩放
            }

            if (ZfStatus == FAIL) // 错误
            {
                appVideoViewTeleReply();
            }

            uiUpdateOSDPvZoom(video_zoomFactor, 1); // 在 PV 模式下刷新缩放系数
            sp1kAeSetModeEn(AE_ENABLE);             // 启用/禁用 AE 控制 --- 启用
            sp1kAwbEnable();                        // 启用 awb
#endif
        }
        else
        {
#if (STILL_NEW_ZOOM == 1)
            if (!sp1kSnapParamGet(SP1K_SNAP_PARAM_PHOTO_FRM_OPT))
            {
                appStillViewZoomIn();
                sp1kPvSmoothZoomSet(0);
            }
            else
            {
            }
#else
            if (!sp1kSnapParamGet(SP1K_SNAP_PARAM_PHOTO_FRM_OPT))
            {
                appStillViewZoomIn();
                stillZFctor = PvZoomCtrl.factor;
                stillCurrLog = PvZoomCtrl.curLog;
            }
            sp1kAeSetModeEn(AE_ENABLE); // 启用/禁用 AE 控制 --- 启用
            sp1kAwbEnable();            // 启用 awb
#endif
        }

        if (puiPara->FDMode == FDMODE_ON) // 脸部检测->打开
        {
            sp1kFdEnable(); // 启用或禁用脸部检测 --- 启用
        }
        break;
    }

    case SP1K_MSG_KEY_PRESS_ZOOM_OUT: // 【缩小按键按下消息】
    {
        // if (puiPara->DriverMode != PRV_SNAP_SINGLE)  // 拍摄模式状态 != 关闭
        // {
        //     break;
        // }

        vStillViewParamater.stillZoomRptCnt = 0;

        appOsdModeSw(0); // 控制 OSD 模式 --- 重置为带 OSD 的 LCD 模式
        pvZoomSts |= 0x0f;

        if (!sp1kSnapParamGet(SP1K_SNAP_PARAM_PHOTO_FRM_OPT))
        {
            sp1kAeSetModeEn(AE_ENABLE); // 启用/禁用 AE 控制 --- 启用
            sp1kAwbDisable();           // 禁用 awb
        }

        sp1kFdDisable(); // 启用或禁用脸部检测 --- 禁用

        // 快速捕获 -> 打开 ||
        if ((SPEEDY_CAPTURE_ON == puiPara->SpeedyCap) || (SPEEDY_CAPTURE_REC_ON == puiPara->SpeedyCapRec))
        {
#if (VIDEO_NEW_ZOOM == 1)
            sp1kVideoZoom3APreSet();
#endif
        }
        else
        {
#if (STILL_NEW_ZOOM == 1)
            if (!sp1kSnapParamGet(SP1K_SNAP_PARAM_PHOTO_FRM_OPT))
            {
                sp1kPvZoom3APreSet();
            }
#endif
        }
        break;
    }

    case SP1K_MSG_KEY_REPEATE_ZOOM_OUT: // 【缩小按键长按消息】
    case SP1K_MSG_KEY_RELEASE_ZOOM_OUT: // 【缩小按键释放消息】
    {
        // if (puiPara->DriverMode != PRV_SNAP_SINGLE)  // 拍摄模式状态 != 关闭
        // {
        //     break;
        // }

        vStillViewParamater.stillZoomRptCnt = 0;
        pvZoomSts &= ~0x0f;

        // 快速捕获 -> 打开 ||
        if ((SPEEDY_CAPTURE_ON == puiPara->SpeedyCap) || (SPEEDY_CAPTURE_REC_ON == puiPara->SpeedyCapRec))
        {
#if (VIDEO_NEW_ZOOM == 1)
            appVideoViewWide(); // 关于下降过程的简短视频
            sp1kVideoSmoothZoomSet(0);
            stillZFctor = sp1kVideoZoomFctGet();
            stillCurrLog = sp1kVideoZoomCtrlParaGet(SP1K_VIDEO_ZOOM_CTRL_PARA_CURRENT_LOG);
#else
            appVideoViewWide(); // 关于下降过程的简短视频

            if (sp1kVideoRecStatusGet() == VID_REC_IDLE) // 录像状态获取 -> 空闲
            {
                sp1kVideoRecZoomFacSet(video_zoomFactor, 0);
            }
            else
            {
                sp1kVideoRecZoomFacSet(video_zoomFactor, 1);
            }
            uiUpdateOSDPvZoom(video_zoomFactor, 1); // 在 PV 模式下刷新缩放系数
            sp1kAeSetModeEn(AE_ENABLE);             // 启用/禁用 AE 控制 --- 启用
            sp1kAwbEnable();                        // 启用 awb

#endif
        }
        else
        {
#if (STILL_NEW_ZOOM == 1)
            if (!sp1kSnapParamGet(SP1K_SNAP_PARAM_PHOTO_FRM_OPT))
            {
                appStillViewZoomOut();
                sp1kPvSmoothZoomSet(0);
                stillZFctor = sp1kPvZoomFctGet();
                stillCurrLog = sp1kPvZoomCtrlParaGet(SP1K_PV_ZOOM_CTRL_PARA_CURRENT_LOG);
            }
#else
            appStillViewZoomOut();
            stillZFctor = PvZoomCtrl.factor;
            stillCurrLog = PvZoomCtrl.curLog;
            sp1kAeSetModeEn(AE_ENABLE); // 启用/禁用 AE 控制 --- 启用
            sp1kAwbEnable();            // 启用 awb
#endif
        }

        if (puiPara->FDMode == FDMODE_ON) // 脸部检测->打开
        {
            sp1kFdEnable(); // 启用或禁用脸部检测 --- 启用
        }
        break;
    }

    case SP1K_MSG_KEY_PRESS_AF: // 【自动对焦按键按下消息】
    {
#if 0
    key_repeate_check_flag = Flag_false; // 按键长按检测标志位清除
#endif

#if 0 //(SUPPORT_AF && SUPPORT_FIXED_AF)   // 【支持AF调焦】【支持两段式固定焦距】
    AF_Switch();    // 切换自动对焦功能的开关
    // key_enter_AF_menu_flag = Flag_true; // 进入AF菜单标志位置1
    // osMsgPost(SP1K_MSG_STATE_TO_MENU);  // 发送 菜单状态消息
#endif

        if (!af_timer_out && !af_timer_start)
        {
            af_timer_start = Flag_true; // 开始标志位
            break;
        }
        else if (af_timer_out)
        {
            af_timer_out = Flag_false;
            af_timer_start = Flag_false;
        }

        appStillOsdInit(1); // 初始化拍照模式下界面

// 【支持AF调焦】【支持拍照前自动对焦】
#if (SUPPORT_AF && SUPPORT_SNAP_AF)
// 【支持AF调焦】【支持两段式固定焦距】
#if (SUPPORT_AF && SUPPORT_FIXED_AF)
        puiPara->SnapAf = Status_1;            // 自动对焦 --- 打开
        // puiPara->DriverMode = PRV_SNAP_SINGLE; // 拍摄模式 --- 关闭
#endif
        appAFCtrl(AF_CTRL_ENABLE); // AF自动对焦电源控制 --- 打开
        afbackfaruiclearflag = 0;
        osMsgPost(SP1K_MSG_SYS_AF_START); // 发送 AF开始消息
#endif

#if 0
// 【支持AF调焦】【支持两段式固定焦距】
#if (SUPPORT_AF && SUPPORT_FIXED_AF)
    AF_Switch();    // 切换自动对焦功能的开关
    // key_enter_AF_menu_flag = Flag_true; // 进入AF菜单标志位置1
    // osMsgPost(SP1K_MSG_STATE_TO_MENU);  // 发送 菜单状态消息
#endif
#endif
        break;
    }

// 【---------------支持AF调焦------------------】
#if SUPPORT_AF
    case SP1K_MSG_SYS_AF_START: // 【AF开始消息】
    {
        // afdoingflag = 1;
        affarflag = 0;
        AF_AfCalStatusShow(AF_BOX_COLOR_YELLOW, AF_BOX_DISPLAY); // 对焦显示框

        // AF_Set(512);
        AF_Set(0);       // AF关闭
        sp1kHalWait(50); // 延时
        AF_startCalAF(); // 启动AF自动对焦校准
        break;
    }
    case SP1K_MSG_SYS_AF_CAL: // 【AF校准消息】
    {
        AF_CalAF();
        break;
    }
    case SP1K_MSG_SYS_AF_BACK_FAR: // 【AF退出消息】
    {
        if (afbackfaruiclearflag == 0)
        {
            sp1kOsdClearCharExt(128, 88, 64, 64); // 清除屏幕特定区域内容
        }
        puiPara->SnapAf = Status_0; // 自动对焦 --- 关闭

        AF_Set(0);                 // AF关闭
        appAFCtrl(AF_CTRL_DISABL); // AF自动对焦电源控制 --- 失能
        break;
    }
    case SP1K_MSG_SYS_AF_SUCCESS: // 【AF成功消息】
    {
        AF_AfCalStatusShow(AF_BOX_COLOR_GREEEN, AF_BOX_DISPLAY); // 绿, 显示
        sp1kHalWait(150);                                        // 延时
        AF_AfCalStatusShow(AF_BOX_COLOR_GREEEN, AF_BOX_HIDE);    // 绿, 不显示
        sp1kHalWait(150);                                        // 延时
        AF_AfCalStatusShow(AF_BOX_COLOR_GREEEN, AF_BOX_DISPLAY); // 绿, 显示
        sp1kHalWait(150);                                        // 延时
        // AF_AfCalStatusShow(1, 0);

        if (app_adjust_macro_sta_get()) // 返回 adjust_macro_sta
        {
            puiPara->SnapAf = Status_0;  // 自动对焦 --- 关闭
            app_adjust_macro_sta_set(0); // 赋值adjust_macro_sta
            appAFCtrl(AF_CTRL_DISABL);   // AF自动对焦电源控制 --- 失能
        }
        else
        {
            affarflag = 1;
            // osMsgPost(SP1K_MSG_KEY_PRESS_S2);    // 发送 S2按键按下消息
        }
        // afdoingflag = 0;
        // if (afdoingsnapflag)
        // {
        //     osMsgPost(SP1K_MSG_KEY_PRESS_S2);    // 发送 S2按键按下消息
        //     afdoingsnapflag = 0;
        // }
        ENTER_CRITICAL(vStillViewParamater.sts); // 进入临界区
        sp1kADCSarSel(0x02);                     // 选择 SAR ADC 通道
        sp1kHalWait(50);                         // 延时
        adcVal = sp1kADCValueSnapIntr();         // 捕捉刚刚在中断中使用的 ADC 值
        EXIT_CRITICAL(vStillViewParamater.sts);  // 退出临界区
        // printf("----------adc val : %d\n", adcVal);
        if (adcVal <= (0 + 10))
        {
            osMsgPost(SP1K_MSG_KEY_PRESS_S2); // 发送 S2按键按下消息
        }
        break;
    }
    case SP1K_MSG_SYS_AF_FAIL: // 【AF失败消息】
    {
        AF_AfCalStatusShow(0, 1); // 根据给定的状态和标志来显示或清除一个特定的OSD选择窗口
        sp1kHalWait(150);         // 延时
        AF_AfCalStatusShow(0, 0); // 根据给定的状态和标志来显示或清除一个特定的OSD选择窗口
        sp1kHalWait(150);         // 延时
        AF_AfCalStatusShow(0, 1); // 根据给定的状态和标志来显示或清除一个特定的OSD选择窗口
        sp1kHalWait(150);         // 延时
        // AF_AfCalStatusShow(0, 0);

        if (app_adjust_macro_sta_get()) // 返回 adjust_macro_sta
        {
            puiPara->SnapAf = Status_0;
            app_adjust_macro_sta_set(0); // 赋值adjust_macro_sta
            appAFCtrl(AF_CTRL_DISABL);   // AF自动对焦电源控制 --- 失能
        }
        else
        {
            // osMsgPost(SP1K_MSG_KEY_PRESS_S2);    // 发送 S2按键按下消息
        }
        // afdoingflag = 0;
        break;
    }
#endif
    case SP1K_MSG_KEY_PRESS_SNAP: // 【快照按键按下消息】无break
    {
#if 0
            if (app_adjust_macro_sta_get()) // 应用程序调整宏获取成功则
            {
                if (!cur_idx)
                {
                    puiPara->SnapAf = Status_1;             // 自动对焦 --- 打开
                    puiPara->AFMode = Status_1;             // AF模式 --- 近焦
                    press_snap_check_flag = Flag_true; // 快照按键检测标志位置1

                    cur_idx = 0; // 索引清0

                    // app_adjust_macro_sta_set(0);

                    // osMsgPost(SP1K_MSG_STATE_INIT);  // 发送初始化消息
                    appStillOsdInit(1);            // 初始化拍照模式下界面
                    enterMacroadjust = Flag_false; // 进入应用程序调整宏标志位置0
                }
                else
                {
                    cur_idx = 0;

                    app_adjust_macro_sta_set(0);

                    // osMsgPost(SP1K_MSG_STATE_INIT);  // 发送初始化消息
                    appStillOsdInit(1); // 初始化拍照模式下界面

                    // pbFile.dwIndex = 0xffff; //xian ++, invalid index for #31928
                    // appStillViewInit(SP1K_STILL_VIEW_INIT_SMART);   // 拍照视图初始化

                    enterMacroadjust = Flag_false; // 进入应用程序调整宏标志位置0
                }
                break;
            }

            press_snap_check_flag = Flag_true;   // 快照按键检测标志位置
            key_repeate_check_flag = Flag_false; // 按键长按检查标志位清除
#endif

        // if (afdoingflag)
        // {
        //     afdoingsnapflag = 1;
        //     break;
        // }
        af_timer_start = 0;
    }
    case SP1K_MSG_KEY_PRESS_S2: // 【S2按键按下消息】
    case SP1K_MSG_FD_SMILE:     // 【微笑检测消息】
    case SP1K_MSG_MD_ACTION:    // 【运动检测消息】
    {
        if (sp1kSD_CdGet()) // 获取 SD 卡检测值 --- 无SD卡直接返回
        {
            break;
        }
        sp1kBtnDisableAll(); // 禁用所有按键

        // sp1kSuperPreviewSet(0,0);   // 静态预览模式, 禁用超级预览

        if (PRV_SNAP_SINGLE == puiPara->DriverMode) // 拍摄模式是关闭则
        {
            app_set_led(LED_BUSY_OFF); // 忙指示灯控制 --- 灭

            appSnapFlashAdjustsAe(puiPara->FlashLed); // 根据闪光灯状态进行调整曝光 色彩校正
            appSnapFlashLed(Status_1);                // 闪光灯控制 --- 常开
            sp1kHalWait(500);                         // 延时

            if (puiPara->FlashLed != Status_1) // 闪光灯状态不等于 自动
            {
                appSnapFlashLed(Status_0); // 闪光灯控制 --- 关闭
                sp1kHalWait(500);          // 延时
            }
        }

#if (STILL_NEW_ZOOM == 1)
        if (appStillSmoothZoomMsgProcFlow(msg) == FAIL)
        {
            break;
        }
#endif

// 如果是 主机+传感器+VLC 计时 则
#if (FRONT_SNAP_BREAK_DOWN >= 2)
        gpioByteOutSet(GPIO_BYTE_GEN1, 0x10, 0x10); // GPIO12 输出高电平
#endif

        /***************Just for QA test @start***************************************/
        if (0x55 == AutoTestMode)
        {
            /*
            模拟在自动测试模式下相机的操作流程
            */
            // 声明并初始化局部变量osdMaxX和osdMaxY
            UINT8 osdMaxX, osdMaxY;
            // 静态变量ISize递增
            static UINT8 ISize = 0;
            ISize++;

            // 如果ISize达到最大图像尺寸
            if (PRV_SIZE_MAX == ISize)
            {
                // 重置ISize
                ISize = 0;
            }
            // 更新ui结构体中的ImageSize字段
            puiPara->ImageSize = ISize;
            // 获取OSD布局属性
            sp1kOsdLayoutAttrGet(&osdMaxX, &osdMaxY);
            // 更新图像大小
            uiUpdateImgSize(osdMaxX - 9 + vStillViewParamater.Xoffset, 0, 0);
            // 初始化剩余像素显示
            uiRemainPixInit(); // mantis #26379
            // 更新剩余像素显示
            uiUpdateRemainPix(); // mantis #26379
            // 等待1000毫秒
            sp1kHalWait(1000);
            // 禁用距离测量功能
            sp1kDistDisable();
        }
        /***************Just for QA test @end***************************************/

#if FUNC_HOST_IGBT_FLASH // 【支持 IGBT 功能】
        if ((FlashLightMode == FLASH_AUTO && curGid <= 36) || FlashLightMode == FLASH_FORCE)
        {
            if (ChargeStatusGet() == 1)
            {
                break;
            }
        }
#endif

        /* 检查环境 是否符合拍摄条件 */
        if (appSnapEnvChk() != SUCCESS)
        {
            sp1kBtnEnableAll(); // 启用所有按键
            break;
        }

        /* 发送信息等待 AE/AWB 就绪 */
        pvWaitCnt = 0;
        osMsgPost(SP1K_MSG_3A_WAIT_PROC); // 发送 等待3A处理消息
        sp1kFdDisable();                  // 使能人脸检测功能

        /* 等待 AE/AWB 就绪时禁用所有按钮 */
        // 视频捕获获取 --- 视频捕获模式 != 剪辑捕获
        if (sp1kVideoCapParamGet(SP1K_VD_CAP_MOD) != SP1K_SPEEDCAP_CLIP)
        {
            sp1kBtnDisableAll(); // 禁用所有按键
        }

        if (puiPara->SpeedyCap == SPEEDY_CAPTURE_ON) // 快速捕获状态 -> 打开
        {
            puiPara->StampVideoMode = puiPara->StampMode; // 视频下日戳状态 = 日戳状态
            // puiPara->VideoSize = MOV_SIZE_VGA;    // 视频格式状态 =
            appVideoStampSet(); // 设置视频时间戳(在 osdinit 之后移动)
        }

        if (puiPara->SpeedyCapRec == SPEEDY_CAPTURE_REC_ON) // 速拍录制状态 -> 打开
        {
            puiPara->StampVideoMode = puiPara->StampMode; // 视频下日戳状态 = 日戳状态
            appVideoStampSet();                           // 设置视频时间戳(在 osdinit 之后移动)
        }
        break;
    }
    case SP1K_MSG_KEY_PRESS_OK: // 【确认按键按下消息】
    {
        afbackfaruiclearflag = 0;
        versionclearflag = 0;

        uiOsdVerDisp(FW_VERSION, 0); // 显示垂直OSD信息 版本信息

        if (Flag_true == enterMacroadjust) // 进入应用程序调整宏标志位 为真
        {
            enterMacroadjust = Flag_false;
        }

        // 获取快速拍照状态 -> 快速拍照中
        if (sp1kSpeedyCapStsGet() == SPEEDCAP_STS_RUNNING)
        {
            break;
        }
        sp1kFdDisable();   // 启用或禁用脸部检测 --- 禁用
        sp1kFdSetSmile(0); // Fd 设置微笑功能 --- 禁用

        // 快速捕获 -> 打开 ||
        if (puiPara->SpeedyCap == SPEEDY_CAPTURE_ON || puiPara->SpeedyCapRec == SPEEDY_CAPTURE_REC_ON)
        {
#if (VIDEO_NEW_ZOOM == 1)
            stillZFctor = sp1kVideoZoomFctGet();
            stillCurrLog = sp1kVideoZoomCtrlParaGet(SP1K_VIDEO_ZOOM_CTRL_PARA_CURRENT_LOG);
#else
            stillZFctor = video_zoomFactor;
            stillCurrLog = ZoomCurLog;
#endif
        }
        else
        {
#if (STILL_NEW_ZOOM == 1)
            stillZFctor = sp1kPvZoomFctGet();
            stillCurrLog = sp1kPvZoomCtrlParaGet(SP1K_PV_ZOOM_CTRL_PARA_CURRENT_LOG);
#else
            stillZFctor = PvZoomCtrl.factor;
            stillCurrLog = PvZoomCtrl.curLog;
#endif
        }

        puiPara->FlashLed = Status_0;      // 闪光灯状态 -> 关闭
        appSnapFlashLed(0);                // 快门时闪光灯控制 --- 关闭
        uiUpdateFlashLed(LCD_WIDTH, 0, 0); // 根据当前闪光灯模式更新相应的图标显示状态

        osMsgPost(SP1K_MSG_STATE_TO_MENU); // 发送 菜单状态消息
        break;
    }

    case SP1K_MSG_3A_WAIT_PROC: // 【等待3A处理消息】
    {
/* 检查 3A 准备就绪 */
#if FUNC_HOST_IGBT_FLASH                       // 【支持 IGBT 功能】
        aeStatusGet(SP1K_AE_STS_GID, &curGid); // 获取AE状态
        // printf("curGid = %bu\n",curGid);

        if (FlashLightMode != FLASH_OFF && curGid <= 36) /* flash */
        {
            UINT8 befPreFlashLuma, preFlashLuma;
            UINT32 mainFlashTime;

            // printf("\033[32mF_FORCE\033[0m\n");
            sp1kAeSetModeEn(AE_DISABLE); // 启用/禁用 AE 控制 --- 禁用
            sp1kAwbDisable();            // 禁用 awb

            frontPreflashCfg(SENSOR_PREFLASH_CFG_WIDTH, 190); // 配置预闪类型和持续时间 /*20us*/
            frontPreflashCfg(SENSOR_PREFLASH_CFG_H_POS, 10);
            frontPreflashCfg(SENSOR_PREFLASH_CFG_DELAY_TYPE, SENSOR_FLASH_DELAY_TYPE_H_POS);

            sp1kDispImgWinEn(0);        // 启用预览层 --- 禁用
            sp1kCalibrationAWBSet(0);   // 自动白平衡（AWB）校准功能 --- 禁用
            sp1kCalibrationObSet(0);    // 光学黑电平（OB）校准功能 --- 禁用
            sp1kCalibrationGammaSet(0); // 伽马校准功能 --- 禁用

            frontPreflashAeCfg(333, SENSOR_GAIN_AUTO, 4); // 配置前置预闪光灯的自动曝光设置

            frontPreflashTrig(&befPreFlashLuma, &preFlashLuma); // 预闪光触发

            if (preFlashLuma - befPreFlashLuma < 5 && befPreFlashLuma < 25)
            {
                preFlashLuma = befPreFlashLuma + 8;
            }
            else if (preFlashLuma - befPreFlashLuma < 5 && befPreFlashLuma < 30)
            {
                preFlashLuma = befPreFlashLuma + 5;
            }
            // 计算成像系统中前置摄像头的闪光时间
            mainFlashTime = frontSnapFlashTimeGet(befPreFlashLuma, preFlashLuma, 60, 1);

            frontSnapFlashAeCfg(0, SENSOR_GAIN_AUTO, 1);

            sp1kAwbEnable();            // 启用自动白平衡
            sp1kCalibrationObSet(1);    // 光学黑电平（OB）校准功能 --- 启动
            sp1kCalibrationAWBSet(1);   // 自动白平衡（AWB）校准功能 --- 启动
            sp1kCalibrationGammaSet(1); // 伽马校准功能 --- 启动

            frontSnapFlashCfg(SENSOR_SNAP_FLASH_CFG_DELAY_TYPE, SENSOR_FLASH_DELAY_TYPE_H_POS);
            frontSnapFlashCfg(SENSOR_SNAP_FLASH_CFG_WIDTH, mainFlashTime);

            sp1kSnapParamSet(SP1K_SNAP_PARAM_REDO_PREPROCESS, SP1K_SNAP_POST_WB); // 设置背景水平和垂直尺寸
        }
        else if (FlashLightMode == FLASH_FORCE && curGid > 36) /*dummy flash*/
        {
            if (appStillViewAEAWBWait() != SUCCESS)
            {
                break;
            }
            // printf("\033[32mF_DUMMY\033[0m\n");
            /*sp1kAeSetModeEn(AE_DISABLE);*/

            sp1kDispImgWinEn(0);                              // 启用预览层 --- 禁用
            frontPreflashAeCfg(333, SENSOR_GAIN_AUTO, 0);     // 配置前置预闪光灯的自动曝光设置
            frontPreflashCfg(SENSOR_PREFLASH_CFG_WIDTH, 190); // 配置预闪类型和持续时间/*20us*/
            frontPreflashCfg(SENSOR_PREFLASH_CFG_H_POS, 10);
            frontPreflashCfg(SENSOR_PREFLASH_CFG_DELAY_TYPE, SENSOR_FLASH_DELAY_TYPE_H_POS);

            frontIgbtFlashCalibTrig(190, 0);
            frontSnapFlashCfg(SENSOR_SNAP_FLASH_CFG_DELAY_TYPE, SENSOR_FLASH_DELAY_TYPE_H_POS);
            frontSnapFlashCfg(SENSOR_SNAP_FLASH_CFG_WIDTH, 200);
            frontSnapFlashAeCfg(0, SENSOR_GAIN_AUTO, 0);
        }
        else /* NO flash , check 3A ready */
        {
            if (appStillViewAEAWBWait() != SUCCESS)
            {
                break;
            }
            // printf("\033[32mNO_FLASH\033[0m\n");
        }
#else
        if (appStillViewAEAWBWait() != SUCCESS) // 等待自动曝光和白平衡完成 --- 失败
        {
            break;
        }
#endif

        if (puiPara->SpeedyCap == SPEEDY_CAPTURE_ON) // 快速捕获状态 -> 打开
        {
            sp1kVideoCapStart();                   // 启动速拍功能
            uiOsdDialogDisp(ID_STR_WAITING___, 0); // 显示消息对话框 --- 显示后0等待
            sp1kBtnDisableAll();                   // 禁用所有按键
            break;
        }

        if (puiPara->SpeedyCapRec == SPEEDY_CAPTURE_REC_ON)
        {
            // printf("--speedycapture rec-\n");
            sp1kVideoCapStart();                     // 启动速拍功能
            HAL_GlobalReadGTimer(&speedyCapRecTime); // 读取全局定时器的计数值存储在speedyCapRecTime
            count = speedyCapRecTime;
            break;
        }

#if (FRONT_SNAP_BREAK_DOWN >= 2)
        gpioByteOutSet(GPIO_BYTE_GEN1, 0x10, 0x00); // GPIO12 输出低电平
#endif

#if (FRONT_SNAP_BREAK_DOWN >= 2)
        gpioByteOutSet(GPIO_BYTE_GEN1, 0x10, 0x10); // GPIO12 输出高电平
#endif

/* 当 AE/AWB 准备就绪时，启用所有按钮 */
#if LIFE_SPAN_TEST
        snapRdy = 1;
#endif

        if (puiPara->PhotoFrame == Status_1) // 相框状态 -> 相框A
        {
            appPvPhotoFrameSet(PRV_PHOTO_FRAME_OFF); // 设置相机的照片帧参数 --- 关闭相框
        }

        /* 加载快照参数 不成功则 */
        if (appSnapParaLoad() != SUCCESS)
        {
            sp1kBtnEnableAll(); // 启用所有按键
            break;
        }

        /* 设置自定义印章参数 */
        appCustomizeStampSet();

        // xian ++ 20081216, move from appSnapParaLoad()
        if (appDriveModeSet() != SUCCESS) // 根据不同的拍摄模式设置相应的拍摄参数
        {
            sp1kBtnEnableAll(); // 启用所有按键
            break;
        }
        // xian --

        /* 快照任务预处理 */
        if (appSnapPreProc() != SUCCESS) // 抓拍或捕捉图像相关的预处理
        {
            sp1kBtnEnableAll(); // 启用所有按键
            break;
        }

        sp1kSnapParamSet(SP1K_SNAP_PARAM_REDO_PREPROCESS, SP1K_SNAP_POST_WB); // 设置快照参数

        /* // 启用或禁用PV模式下的相框 */
        sp1kPvPhotofrmEnable(PV_PhotoFrame_Disable); // xian ++ 20090717

#if FUNC_HOST_MD // 【支持运动检测功能】
        mdEn = sp1kMDEnGet();
        sp1kMDEnSet(MD_DISABLE);
#endif

        sp1kOsdClearCharExt(128, 88, 64, 64); // 清除屏幕特定区域内容
        appSnapFlashLed(Status_1);            // 快门时闪光灯控制 --- 常开

        // for (cnt=0;cnt<1;cnt++){
        vStillViewParamater.ret = sp1kSnapTask(); // 触发快照流来捕捉图像
        //}

        if (puiPara->PhotoFrame == Status_1) // 相框状态 -> 相框A
        {
            sp1kSnapParamSet(SP1K_SNAP_PARAM_PHOTO_FRM_OPT, 1); // 设置快照参数
        }

#if FUNC_HOST_MD // 【支持运动检测功能】
        if (mdEn == MD_ENABLE)
        {
            sp1kMDEnSet(MD_ENABLE);
            sp1kMDLearningStart(MD_LEARNING_ENABLE);
        }
#endif

#if (FRONT_SNAP_BREAK_DOWN >= 2)
        gpioByteOutSet(GPIO_BYTE_GEN1, 0x10, 0x10);
#endif

        /* 快照任务后处理 */
        appSnapPostProc(vStillViewParamater.ret);

#if (FRONT_SNAP_BREAK_DOWN >= 2)
        gpioByteOutSet(GPIO_BYTE_GEN1, 0x10, 0x00);
#endif

#if FUNC_HOST_PF_PV
        sp1kPvDlyPhofrmBlendCntSet(1); // mantis 0042795
        appPvPhotoFrameRecover();      // xian ++ 20081216
#endif

        if (puiPara->FDMode == FDMODE_ON) // 脸部检测 -> 打开
        {
            sp1kFdReset();  // FD 复位
            sp1kFdEnable(); // 启用或禁用脸部检测 --- 启用
        }

#if MSG_AUTO_SEND
        osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY); // 发送 拍照回放消息
#endif

        /***************Just for QA test @start***************************************/
        if (AutoTestMode == 0x55)
        {
            osMsgPost(SP1K_MSG_KEY_PRESS_S2); // 发送 S2按键按下消息
        }
        /***************Just for QA test @end***************************************/

        app_set_led(LED_BUSY_ON); // 忙指示灯控制 --- 亮
        if (puiPara->FlashLed != Status_1)
        {
            appSnapFlashLed(Status_0); // 快门时闪光灯控制 --- 关闭
        }
        else
        {
            appSnapFlashLed(Status_1);
        }

        appSnapFlashAdjustsAe(0xff); // 调整拍摄闪光灯并调整曝光 --- 复位

        // sp1kSuperPreviewSet(0,1);   // 使能超级预览模式
        if (sp1kSuperPreviewStatusGet(0)) // 获取 still 下超级预览模式状态 --- 如果是启用
        {
            appStillPreviewReset(); // 根据拍摄模式设置静态拍摄参数
        }

#if (SUPPORT_AF && SUPPORT_SNAP_AF) // 支持AF调焦 && 支持拍照前自动对焦
        if (puiPara->SnapAf)             // 自动对焦已打开
        {
            puiPara->SnapAf = Status_0; // 关闭自动对焦

            AF_Set(0);                 // AF关闭
            appAFCtrl(AF_CTRL_DISABL); // AF自动对焦电源控制  --- 关闭
        }
#endif
        sp1kBtnEnableAll(); // 启用所有按键
        break;
    }
// 【---------------------支持全景功能---------------】         
#if FUNC_HOST_PANORAMA
		case SP1K_MSG_GMV_BY_FRAME_CALCULUS:    // 【帧计算全景消息】
			sp1kPanoramaGMVByFrameCalculus(&panoramaGMVByFrame[0],&panoramaGMVByFrame[1],&panoramaStatus);
			sp1kPanoramaSumOfGMVCalculus(&panoramaSumOfGMV[0], &panoramaSumOfGMV[1]);
			break;
		case SP1K_MSG_SUM_OF_GMV_OSD_DRAW:  // 【GMV和的OSD绘制消息】
			/* GMV osd draw */
			//printf("SumOfGMV(%d,%d)\n",panoramaSumOfGMV[0],panoramaSumOfGMV[1]);
			break;
		case SP1K_MSG_GMV_TOO_FAST_OSD_DRAW:    // 【GMV过快OSD绘制消息】
			/* too fast osd draw */
			//printf("too fast\n");
			break;
		case SP1K_MSG_GMV_TOO_SLOW_OSD_DRAW:    // 【GMV过慢OSD绘制消息】
			/* too slow osd draw */
			//printf("too slow\n");
			break;
		case SP1K_MSG_PANORAMA_FINISH:  // 【全景拍摄完成消息】
			/* too slow osd draw */
			sp1kPanoramaEnSet(PANORAMA_DISABLE);
			sp1kPanoramaStitchFunc();
			sp1kPanoramaDstFileSave("SPCA0001.YUV");
			sp1kSnapSizeSet( 2880, 400 );
			osMsgPost(SP1K_MSG_3A_WAIT_PROC);
			printf("Panorama finish\n");
			break;
#endif
        case SP1K_MSG_EYE_BLINK: // 【眨眼检测消息】
        {
            sp1kSnapParamSet(SP1K_SNAP_PARAM_BLINK, 0xff); // 设置快照参数 --- 获取闪烁信息
            osMsgPost(SP1K_MSG_STATE_TO_MENU);             // 发送 菜单状态消息
            break;
        }
        case SP1K_MSG_KEY_PRESS_MENU: // 【菜单按键按下消息】
        {
            afbackfaruiclearflag = 0;

            if (Flag_true == enterMacroadjust) // 进入应用程序调整宏标志位 为真
            {
                enterMacroadjust = Flag_false;
            }

            // 获取 SD 卡检测值 -> 有 SD卡 && 有文件
            if (!sp1kSD_CdGet() && sp1kPbFileCntGet() != 0)
            {
                osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY); // 发送 拍照回放消息
                osMsgPost(SP1K_MSG_SYS_QUICK_DELETE);    // 发送 快速删除消息
            }
            else if (0 == sp1kPbFileCntGet()) // 无文件
            {
                uiOsdDialogDisp(ID_STR_NO_FILE, 0); // 显示消息对话框 --- 显示后0等待
                sp1kHalWait(500);
                appStillOsdInit(1); // 初始化拍照模式下界面
            }

#if 0
    // 获取快速拍照状态 -> 快速拍照中
    if (sp1kSpeedyCapStsGet() == SPEEDCAP_STS_RUNNING)
    {
        printf("---stop capture\n");
        HAL_GlobalReadGTimer(&speedyCapRecTime);    // 读取全局定时器的计数值存储在speedyCapRecTime
        count = (speedyCapRecTime - count) / 1000;
        sp1kSpeedyCaptureStop();    // 停止快速拍照
        osMsgPost(SP1K_MSG_MEDIA_VIDO_CAP_FINISH);  // 发送 视频拍摄完成消息
    }

    osMsgPost(SP1K_MSG_SYS_QUICK_DELETE);   // 发送 快速删除消息
    sp1kAwbDisable();   // 禁用 awb
    sp1kFdDisable();    // 启用或禁用脸部检测 --- 禁用
    sp1kDistDisable();  // 禁用距离测量功能
#endif
            break;
        }
        case SP1K_MSG_KEY_PRESS_LEFT: // 【左按键按下消息】
        {
            if (PRV_SNAP_SINGLE == puiPara->DriverMode) // 拍摄模式状态 -> 关闭
            {
                puiPara->DriverMode = PRV_SNAP_SELF_5S; // 拍摄模式状态 -> 自拍5S
            }
            else if (PRV_SNAP_SELF_5S == puiPara->DriverMode) // 拍摄模式状态 -> 自拍5S
            {
                puiPara->DriverMode = PRV_SNAP_SINGLE; // 拍摄模式状态 -> 关闭
                puiPara->ImageEffect++;                // 摄像头效果滤镜++
                appEffectSet(puiPara->ImageEffect);    // 输入的效果参数设置相应的摄像头效果
                uiUpdateEffect(LCD_WIDTH, 0, 0);       // 在PV模式下刷新快照效果图标
            }

            uiUpdateSnapMode(LCD_WIDTH, 0, 0);          // 刷新PV模式下的拍摄模式图标
            if (puiPara->DriverMode != PRV_SNAP_SINGLE) // 拍摄模式状态 != 关闭
            {
                AF_Set(0);                 // AF关闭
                appAFCtrl(AF_CTRL_DISABL); // AF自动对焦电源控制 --- 禁用
            }
            break;
        }
        case SP1K_MSG_KEY_PRESS_RIGHT: // 【右按键按下消息】
        {
            if (Flag_true == enterMacroadjust) // 进入应用程序调整宏标志位 为真
            {
                enterMacroadjust = Flag_false;
            }
            // 获取 SD 卡检测值 -> 有 SD卡 && 有文件
            if (!sp1kSD_CdGet() && sp1kPbFileCntGet() != 0)
            {
                osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY); // 发送 拍照回放消息
                osMsgPost(SP1K_MSG_SYS_QUICK_DELETE);    // 发送 快速删除消息
            }
            break;
        }
        case SP1K_MSG_DZOOM_OSD_UPDATE: // 【DZOOM OSD更新消息】
        {
            // 快速捕获 -> 打开 ||
            if ((SPEEDY_CAPTURE_ON == puiPara->SpeedyCap) || (SPEEDY_CAPTURE_REC_ON == puiPara->SpeedyCapRec))
            {
#if (VIDEO_NEW_ZOOM == 1)
                stillZFctor = sp1kVideoZoomFctGet();
                stillCurrLog = sp1kVideoZoomCtrlParaGet(SP1K_VIDEO_ZOOM_CTRL_PARA_CURRENT_LOG);
                uiUpdateOSDPvZoom(stillZFctor, 0);
#endif
            }
            else
            {
#if (STILL_NEW_ZOOM == 1)
                stillZFctor = sp1kPvZoomFctGet();
                stillCurrLog = sp1kPvZoomCtrlParaGet(SP1K_PV_ZOOM_CTRL_PARA_CURRENT_LOG);
                uiUpdateOSDPvZoom(stillZFctor, 0);
#endif
            }
            break;
        }
        case SP1K_MSG_DZOOM_3A_UPDATE: // 【DZOOM 3A更新消息】
        {
            // 快速捕获 -> 打开 ||
            if ((SPEEDY_CAPTURE_ON == puiPara->SpeedyCap) || (SPEEDY_CAPTURE_REC_ON == puiPara->SpeedyCapRec))
            {
#if (VIDEO_NEW_ZOOM == 1)
                sp1kVideoZoom3APostSet();
#endif
            }
            else
            {
#if (STILL_NEW_ZOOM == 1)
                sp1kPvZoom3APostSet();
#endif
            }
            sp1kAeSetModeEn(AE_ENABLE); // 启用/禁用 AE 控制 --- 启用
            sp1kAwbEnable();            // 启用自动白平衡
            break;
        }

        case SP1K_MSG_KEY_REPEATE_TELE: // 【变焦放大按键长按消息】
        {
            // if (puiPara->DriverMode != PRV_SNAP_SINGLE)  // 拍摄模式状态 != 关闭
            // {
            //     break;
            // }

            // 快速捕获 -> 打开 ||
            if ((SPEEDY_CAPTURE_ON == puiPara->SpeedyCap) || (SPEEDY_CAPTURE_REC_ON == puiPara->SpeedyCapRec))
            {
#if (VIDEO_NEW_ZOOM == 1)
                sp1kVideoSmoothZoomSet(1);
                appVideoViewTele(); // 视频键启动过程
#else
            UINT8 ZfStatus; // 存储缩放执行状态

            appVideoViewTele(); // 视频键启动过程

            if (sp1kVideoRecStatusGet() == VID_REC_IDLE) // 录像状态获取 -> 空闲
            {
                ZfStatus = sp1kVideoRecZoomFacSet(video_zoomFactor, 0); // 设置当前有效缩放
            }
            else // 录像状态获取 -> 录像中/暂停
            {
                ZfStatus = sp1kVideoRecZoomFacSet(video_zoomFactor, 1);
            }

            if (ZfStatus == FAIL) // 错误
            {
                appVideoViewTeleReply(); // 视频键向上回复过程
            }
            uiUpdateOSDPvZoom(video_zoomFactor, 1); // 在 PV 模式下刷新缩放系数
#endif
            }
            else
            {
#if (STILL_NEW_ZOOM == 1)
                if (!sp1kSnapParamGet(SP1K_SNAP_PARAM_PHOTO_FRM_OPT)) // 获取快照参数
                {
                    sp1kPvSmoothZoomSet(1);
                    appStillViewZoomIn();
                }
#else
            if (!sp1kSnapParamGet(SP1K_SNAP_PARAM_PHOTO_FRM_OPT))
            {
                appStillViewZoomIn(); // 应用 StillView 缩放流程
            }
#endif
            }

            if (osMsgSeek(SP1K_MSG_KEY_RELEASE_TELE) == TRUE) // 搜索消息缓冲区中是否存在 变焦放大按键释放消息
            {
                osMsgFlush(MSG_CTRL_FLUSH_BTN); // 清空消息队列 --- 刷新按钮驱动器消息
            }
            else
            {
                break;
            }
            break;
        }

        case SP1K_MSG_KEY_PRESS_F1: // 【F1按键按下消息】
        {
#ifdef PREVIEW_RAW_SAVE_MODE
            sp1kCalibrationAWBSet(0); // 自动白平衡（AWB）校准功能 --- 禁用
            sp1kCalibrationPrvRawModeSet(0);
#endif
            break;
        }
        case SP1K_MSG_KEY_PRESS_F2: // 【F2按键按下消息】
        {
#ifdef PREVIEW_RAW_SAVE_MODE
            sp1kCalibrationPrvRawSave(0);
#endif
            break;
        }
        case SP1K_MSG_Version_Show: // 【显示版本消息】
        {
            versionclearflag = 1;
            uiOsdVerDisp(FW_VERSION, 0); // 显示垂直OSD信息 版本信息
            break;
        }
        case SP1K_MSG_Version_Clear: // 【清除版本消息】
        {
            versionclearflag = 0;
            uiOsdVerDisp(FW_VERSION, 0); // 显示垂直OSD信息 版本信息
            break;
        }
        case SP1K_MSG_Adjust_Macro: // 【调整宏消息】
        {
            if (app_adjust_macro_sta_get()) // 应用程序调整宏获取成功
            {
                cur_idx = 0;

                app_adjust_macro_sta_set(0); // 赋值adjust_macro_sta

                // osMsgPost(SP1K_MSG_STATE_INIT);  // 发送 初始化消息
                appStillOsdInit(1); // 初始化拍照模式下界面
                // pbFile.dwIndex = 0xffff; //xian ++, invalid index for #31928
                appStillViewInit(SP1K_STILL_VIEW_INIT_SMART); // 拍照视图初始化
                break;
            }

            adjust_macro_to_menu = 1;

#if 0
    if (dispDevActGet() != 4)   // 获取当前激活的设备ID
    { 
        // HDMI 不支持菜单
        pbZoomCurLog = PLAY_MIN_ZOOM_FACT;
        pbCropDispBuffer2ABBuf();
        osMsgPost(SP1K_MSG_STATE_TO_MENU);  // 发送 菜单状态消息
    }
#else
            app_adjust_macro_sta_set(1);      // 赋值adjust_macro_sta
            app_quick_delete_dialog(cur_idx); // 快速删除对话框
#endif
            break;
        }
        case SP1K_MSG_KEY_REPEATE_POWER: // 【电源按键长按消息】
        {
            appPowerOffProc(); // 关机 --- 其实是发送 关机消息
            break;
        }
        case SP1K_MSG_USB_PLUG_IN: // 【USB插入消息】
        {
            power_up_usb_check_flag = 0;

#if (STILL_NEW_ZOOM == 1)
            appStillSmoothZoomMsgProcFlow(msg);
#endif

            if (AdapterCharge) //  1：USB 连接电源适配器充电
            {
                break;
            }

#if 0
    if (storageReadyflag == SUCCESS)
    {
        if (USB_DBG)
        {
            /*
                uiPara_t* uienv;
                uienv = appUiParaGet();
                if(uienv->USBMode==USB_PCCAM)
                    sp1kHalUSBSwPlugIn();
                else
            */
            sp1kHalUSBSwPlugIn();
        }
#if (FUNC_HOST_DPS == 1)
        else if (puiPara->USBMode == USB_SIDC)
        {
            // printf("mark\n");
            osMsgPost(SP1K_MSG_STATE_TO_DPS);   // 发送 进入 DPS 模式消息
        }
#endif
        else
        {
            // puiPara->USBMode=USB_MASS;
            osMsgPost(SP1K_MSG_STATE_TO_MENU);  // 发送 菜单状态消息
        }
    }
    else
    {
        puiPara->USBMode = USB_PCCAM;
        osMsgPost(SP1K_MSG_STATE_TO_USB);   // 发送 USB模式消息
    }
#endif
            osMsgPost(SP1K_MSG_STATE_TO_MENU); // 发送 菜单状态消息
            break;
        }
        case SP1K_MSG_USB_PLUG_OUT: // 【USB拔出消息】
        {
            if (power_up_usb_check_flag) // USB电源启动检测标志位
            {
                power_up_usb_check_flag = Flag_false; // USB电源启动检测标志位 --- 清零
                break;
            }
            break;
        }
        case SP1K_MSG_POWER_OFF: // 【关机消息】
        {
            appPowerOffProc(); // 关机 --- 其实是发送 关机消息
            break;
        }
        case SP1K_MSG_KEY_PRESS_SELFSNAP: // 【自拍按键按下消息】
        {
            app_set_led(LED_BUSY_OFF); // 忙指示灯控制 --- 关闭
            sp1kPvPhotofrmEnable(1);   // 启用或禁用PV模式下的相框 --- 启用

            appSnapFlashAdjustsAe(puiPara->FlashLed); // 根据闪光灯状态进行调整曝光 色彩校正
            appSnapFlashLed(LED_BUSY_ON);             // 快门时闪光灯控制 --- 常开
            sp1kHalWait(500);                         // 延时

            appSnapFlashLed(0); // 快门时闪光灯控制 --- 关闭
            sp1kHalWait(500);   // 延时

            appSnapFlashLed(1);      // 快门时闪光灯控制 --- 常开
            appSnapParaLoad();       // 加载快照参数
            appStillQuickViewSet();  // 根据当前的 UI 参数中的自动回顾时间设置快速预览参数
            sp1kPvPhotofrmEnable(0); // 启用或禁用PV模式下的相框 --- 禁用
            // sp1kSuperPreviewSet(0,0);   // 使能超级预览模式

            if (sp1kSnapTask() == SP1K_SNAP_MSG_FILE_WRITE_ERROR) // 可触发快照流捕捉图像 --- 失败
            {
                uiUpdateOSDMemFull(); // 更新OSD显示内存已满提示
                isUpdateNum = 1;
                osMsgPost(SP1K_MSG_STATE_INIT); // 发送 初始化消息
            }

            selfsnapbackfarflag = 0;

            // sp1kSuperPreviewSet(0,1);   // 启用/禁用超级预览模式（不支持录音）

            if (sp1kSuperPreviewStatusGet(0)) // 获取 still 下超级预览模式状态 --- 如果是启用
            {
                appStillPreviewReset(); // 根据拍摄模式设置静态拍摄参数
            }

// 【在预览模式中为照片框架提供功能支持】
#if FUNC_HOST_PF_PV
        appPvPhotoFrameRecover(); // 根据当前的拍摄参数和驾驶模式以及用户设置的照片帧模式来决定是否启用照片帧功能
#endif
        appOsdSelfSnapEnableOsd(0); // 禁用自拍OSD
        uiRemainPixShow();          // 在屏幕上显示剩余图片数量
        sp1kBtnEnableAll();         // 启用所有按键

        if (FDMODE_ON == puiPara->FDMode) // 脸部检测 -> 启用
        {
            sp1kFdReset();  // FD 复位
            sp1kFdEnable(); // 启用或禁用脸部检测 --- 启用
        }

        sp1kBtnEnableAll(); // 启用所有按键

        if (((puiPara->DriverMode <= PRV_SNAP_CYC_10S) && (puiPara->DriverMode >= PRV_SNAP_CYC_2S))) // 拍摄模式
        {
            osMsgPost(SP1K_MSG_KEY_PRESS_S2); // 发送 S2按键按下消息
        }
        else
        {
            app_set_led(LED_BUSY_ON);    // 忙指示灯控制 --- 打开
            appSnapFlashLed(0);          // 快门时闪光灯控制 --- 关闭
            appSnapFlashAdjustsAe(0xff); // 调整拍摄闪光灯并调整曝光
        }
        break;
    }
    case SP1K_MSG_BAT_LV_UPDATE: // 【电池电量更新消息】
    {
        // jintao.liu 2008-3-12
        // add for battery detect
        uiUpdateBatteryLevel(); // 刷新电池电量图标
        appBattStsHandle();     // 处理电池状态
        break;
    }
    case SP1K_MSG_BAT_EMPTY: // 【电池电量低消息】
    {
#if (STILL_NEW_ZOOM == 1)
        appStillSmoothZoomMsgProcFlow(msg);
#endif
        uiUpdateOSDBatEmpty();         // 处理 UI 更新和低电量情况下的按钮启用/禁用操作
        osMsgPost(SP1K_MSG_POWER_OFF); // 发送 关机消息
        sp1kBtnDisableAll();           // 准备关闭电源，禁用所有按钮
        break;
    }
    case SP1K_MSG_TV_PLUG_IN: // 【TV插入消息】
    {
#if (STILL_NEW_ZOOM == 1)
        appStillSmoothZoomMsgProcFlow(msg);
#endif
#if (TV_OPTION == TV_PLAY_OPTION)
        osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY);
        osMsgPost(SP1K_MSG_TV_PLUG_IN);
#elif (TV_OPTION == TV_FULL_OPTION)
        appDispSet(); // 面板选择
#endif
        break;
    }
    case SP1K_MSG_TV_PLUG_OUT: // 【TV拔出消息】
    {
#if (STILL_NEW_ZOOM == 1)
        appStillSmoothZoomMsgProcFlow(msg);
#endif
#if (TV_OPTION == TV_PLAY_OPTION)
        osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY);
        osMsgPost(SP1K_MSG_TV_PLUG_OUT);
#elif (TV_OPTION == TV_FULL_OPTION)
        appDispSet(); // 面板选择
#endif
        break;
    }
    case SP1K_MSG_HDMI_PLUG_IN: // 【HDMI插入消息】
    {
#if (STILL_NEW_ZOOM == 1)
        appStillSmoothZoomMsgProcFlow(msg);
#endif
#if 1
        osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY); // 发送 拍照回放消息
        osMsgPost(SP1K_MSG_HDMI_PLUG_IN);        // 发送 HDMI插入消息
                                                 // appDispHdmiOpen();
#else
        // example code for super preview in video video HD mode
        // osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY);
        appDispHdmiOpen();
        osMsgPost(SP1K_MSG_STATE_INIT);
        sp1kHalCtrlScreen(3);

        osMsgPost(SP1K_MSG_STATE_TO_VIDEO_VIEW);
        sp1kSuperPreviewSet(1, 1);
        // osMsgPost(SP1K_MSG_HDMI_PLUG_IN);
        // appDispHdmiOpen();
        break;
#endif
        break;
    }
    case SP1K_MSG_HDMI_PLUG_OUT: // 【HDMI拔出消息】
    {
        appDispSet(); // 面板选择
        break;
    }
    case SP1K_MSG_CARD_PLUG_IN: // 【SD卡插入消息】
    {
        versionclearflag = 0;

        uiOsdVerDisp(FW_VERSION, 0); // 显示垂直OSD信息 --- 版本信息
        appCardErrFmtErrSet(0);

#if (STILL_NEW_ZOOM == 1)
        appStillSmoothZoomMsgProcFlow(msg);
#endif
        appStorageMount(K_MEDIA_SDF); // SD 插卡动作
        uiRemainPixInit();            // 初始化剩余像素显示
        appStillOsdInit(1);           // 初始化拍照模式下界面
        break;
    }
    case SP1K_MSG_CARD_PLUG_OUT: // 【SD卡拔出消息】
    {
// 【启用 LCD 和 OSD 模式开关】
#ifdef _ENABLE_LCD_OFF_  // yp for mantis 43034
        appOsdModeSw(0); // 控制 OSD 模式 --- 重置为带 OSD 的 LCD 模式
#endif                   /*_ENABLE_LCD_OFF_*/

        appCardErrFmtErrSet(0);

// add for mantis #36958 @start
#if (STILL_NEW_ZOOM == 1)
        appStillSmoothZoomMsgProcFlow(msg);
#endif
        // 如果SD卡未插入 && 设备类型 -> 磁盘SPI && 获取 DOS Ram 状态
        if ((appSDPlugStatusGet() == 0) && (sp1kDiskTypeGet() == DEV_TYPE_DISK_SPI) && (sp1kDosRamStsGet() == FALSE))
        {
            osMsgFlush(MSG_CTRL_FLUSH_HOST | MSG_CTRL_FLUSH_BTN);
            uiOsdDialogDisp(ID_STR_NO_CARD_, 0); // 显示消息对话框 --- 显示后0等待

            if (enterVersionshow == 1)
            {
                enterVersionshow = 0;
                osMsgPost(SP1K_MSG_Version_Show); // 发送 显示版本消息
            }

// 【支持在预览界面显示当前时间】
#if SUPPORT_RTC_OSD_UPDATE
            appCurrentTimeShow(1); // 显示当前时间
#endif

            if (power_up_usb_check_flag) // USB电源启动检测标志位
            {
                power_up_usb_check_flag = Flag_false;

                if (sp1kUSB_PlugGet()) // 获取USB 插头检测值
                {
                    osMsgPost(SP1K_MSG_USB_PLUG_IN); // 发送 USB插入消息
                }
            }

            /*
            if(enterPCCamflag && !AdapterCharge){
                enterPCCamflag = 0;
                puiPara->USBMode=USB_PCCAM;
                osMsgPost(SP1K_MSG_STATE_TO_USB);
                break;
            }
            */

            break;
        }
        // add for mantis #36958 @end

        sp1kBtnEnableAll();             // 启用所有按键
        appStorageMount(K_MEDIA_NANDF); // SD 插卡动作

        uiRemainPixInit();           // 初始化剩余像素显示
        appStillOsdInit(1);          // 初始化拍照模式下界面
        app_set_led(LED_BUSY_ON);    // 忙指示灯控制 --- 打开
        appSnapFlashLed(0);          // 快门时闪光灯控制 --- 关闭
        appSnapFlashAdjustsAe(0xff); // 调整拍摄闪光灯并调整曝光
        break;
    }
    case SP1K_MSG_NAND_ERROR: // 【NAND错误消息】
    {
#if (STILL_NEW_ZOOM == 1)
        appStillSmoothZoomMsgProcFlow(msg);
#endif
        sp1kBtnDisableAll(); // 禁用所有按键

        if (osMsgSeek(SP1K_MSG_KEY_PRESS_POWER) == TRUE) // 搜索消息缓冲区中是否存在 电源按键按下消息
        {
            osMsgFlush(MSG_CTRL_FLUSH_HOST | MSG_CTRL_FLUSH_BTN);
            osMsgPost(SP1K_MSG_POWER_OFF); // 发送 关机消息
        }
        else
        {
            osMsgFlush(MSG_CTRL_FLUSH_HOST | MSG_CTRL_FLUSH_BTN);
        }
        uiOsdDialogDisp(ID_STR_FORMATTING___, 0); // 显示消息对话框 --- 显示后0等待

        sp1kVfsFmt(0xC0); // format
        vStillViewParamater.sts = sp1kVfsInit(1);

        if (vStillViewParamater.sts == TRUE)
        {
#if TAG_TODO                     // by xian ???no use
            G_USR_FileIndex = 1; // DCF Init
#endif
            appStillOsdInit(1);  // 初始化拍照模式下界面
            uiRemainPixInit();   // 初始化剩余像素显示
            uiUpdateRemainPix(); // 更新剩余像素显示
        }
        else
        {
            uiOsdDialogDisp(ID_STR_CARD_ERROR, 0); // 显示消息对话框 --- 显示后0等待
        }
        // appUiBusyExit();
        sp1kBtnEnableAll(); // 启用所有按键
        break;
    }
    case SP1K_MSG_CARD_ERROR: // 【SD卡错误消息】
    {
#if (STILL_NEW_ZOOM == 1)
        appStillSmoothZoomMsgProcFlow(msg);
#endif
        if (sp1kDiskErrorCodeGet() == DISK_ERR_DETECT)
        {
            uiOsdDialogDisp(ID_STR_PLEASE_PLUG_OUT_CARD, 1000); // 显示消息对话框 --- 显示后1000等待
        }
        else
        {
            uiOsdDialogDisp(ID_STR_CARD_ERROR, 1000); // 显示消息对话框 --- 显示后1000等待
            appCardErrFmtSta(1);                      // 1: 发送 菜单状态消息
        }
        break;
    }
    case SP1K_MSG_CARD_NOT_SUPPORT: // 【SD卡不支持消息】
    {
#if (STILL_NEW_ZOOM == 1)
        appStillSmoothZoomMsgProcFlow(msg);
#endif
        uiOsdDialogDisp(ID_STR_CARD_ERROR, 1000); // 显示消息对话框 --- 显示后1000等待
        sp1kBtnDisableAll();                      // 禁用所有按键
        break;
    }
// 【---------------------支持在预览界面显示当前时间---------------】
#if SUPPORT_RTC_OSD_UPDATE
    case SP1K_MSG_UPDATE_RTCTIME: // 【更新RTC时间消息】
    {
        appCurrentTimeShow(0); // 显示当前时间
        break;
    }
#endif
    case SP1K_MSG_SELFSNAP_BEEP: // 【自拍蜂鸣器消息】
    {
        appSelfSnapHandle(); // 自拍处理函数
        break;
    }
    case SP1K_MSG_SELFSNAP_COUNT: // 【自拍计数消息】
    {
        selfsnapbackfarflag = 1;
        sp1kBtnDisableAll();  // 禁用所有按键
        appSelfSnapMSGSend(); // 倒计时并在时间到达后发送自拍消息给快照
        break;
    }
    case SP1K_MSG_TP_CAL_COMPLETE: // 【触摸校准完成消息】
    {
        appStillOsdInit(1); // 初始化拍照模式下界面
        puiPara->tpCorrect = 0;
        break;
    }
    case SP1K_MSG_MEDIA_VIDO_CAP_FINISH: // 【视频拍摄完成消息】
    {
        if (puiPara->SpeedyCap == SPEEDY_CAPTURE_ON) // 快速捕获 -> 打开
        {
            RemainPictureNO -= 6;
        }
        else if (puiPara->SpeedyCapRec == SPEEDY_CAPTURE_REC_ON)
        {
            // printf("count = %lu\n", count);
            if (count >= (RemainPictureNO / 3))
            {
                RemainPictureNO = 0;
            }
            else
            {
                RemainPictureNO -= 3 * count;
            }
        }
        isUpdateNum = 1;
        appStillOsdInit(1); // 初始化拍照模式下界面
        sp1kBtnEnableAll(); // 启用所有按键
        break;
    }
    case SP1K_MSG_MEDIA_REC_DISK_FULL: // 【磁盘已满消息】
    {
        HAL_GlobalReadGTimer(&speedyCapRecTime);
        count = (speedyCapRecTime - count) / 1000;
        sp1kSpeedyCaptureStop();                   // 停止快速拍照
        osMsgPost(SP1K_MSG_MEDIA_VIDO_CAP_FINISH); // 发送 视频拍摄完成消息
        uiUpdateOSDMemFull();                      // 更新OSD显示内存已满提示
        isUpdateNum = 1;
        appStillOsdInit(1); // 初始化拍照模式下界面
        sp1kBtnEnableAll(); // 启用所有按键
        break;
    }
    case SP1K_MSG_MEDIA_VIDO_CAP_ERROR: // 【视频拍摄错误消息】
    {
        uiOsdDialogDisp(ID_STR_FILE_ERROR_, 1000); // 显示消息对话框 --- 显示后1000等待
        appStillOsdInit(1);                        // 初始化拍照模式下界面
        sp1kBtnEnableAll();                        // 启用所有按键
        break;
    }
// 【--------------支持屏保-----------------】
#if SUPPORT_PANEL_PROTECT
    case SP1K_MSG_PANEL_PROTECT: // 【面板保护消息】
    {
        appPanelAutoProt(); // 自动保护面板
        break;
    }
    case SP1K_MSG_SYS_CLOSE_LCD_BACKLIGHT: // 【关闭屏幕背光消息】
    {
        app_set_lcd_backlight_status(LCD_LED_OFF); // 设置 屏幕LCD 背光的状态 --- 关闭
        break;
    }
    case SP1K_MSG_SYS_OPEN_LCD_BACKLIGHT: // 【打开屏幕背光消息】
    {
        app_set_lcd_backlight_status(LCD_LED_ON); // 设置 屏幕LCD 背光的状态 --- 打开
        break;
    }
#endif

// 【-------------定时检测亮度是否发生变化---------------】
#if SUPPORT_AWB_ADJ
    case SP1K_MSG_AWB_OFF: // 【关闭AWB消息】
    {
        sp1kAwbDisable(); // 禁用 awb
        break;
    }
    case SP1K_MSG_AWB_CHECK: // 【AWB检查消息】
    {
        static UINT8 prev_luma = 0;
        xdata UINT8 curr_luma = 0, diff_luma = 0;

        sp1kAeStatusGet(SP1K_AE_STS_CURRENT_LUMA, &curr_luma); // 获取 AE 状态 存储在 curr_luma

        if (curr_luma > prev_luma)
        {
            diff_luma = curr_luma - prev_luma;
        }
        else
        {
            diff_luma = prev_luma - curr_luma;
        }

        // printf("prev luma : %d\n", (UINT16)prev_luma);
        // printf("curr luma : %d\n", (UINT16)curr_luma);
        // printf("diff luma : %d\n", (UINT16)diff_luma);

        prev_luma = curr_luma;

        if (diff_luma > 50)
        {
            sp1kAwbEnable(); // 启用 awb

#if SUPPORT_AWB_ADJ // 【定时检测亮度是否发生变化，如果发生变化则打开AWB，否则关闭AWB】
            awb_auto_disable_flag = 1;
#endif
        }
        else
        {
            sp1kAwbDisable(); // 禁用AWB
        }
        break;
    }
#endif
    default:
    {
        // printf("Unknown message:0x%bx\n", msg);
        break;
    }
    }
}

/**
* @param    InitMode: 初始状态
* @retval   None
* @brief    拍照视图初始化
**/
void appStillViewInit(UINT8 InitMode)
{
    /* power control -- preview */
    // appModePowerCtrl(MODE_PREVIEW); // 控制电源，进入预览模式

    UINT16 frontHSize, frontVSize; // 前置摄像头水平尺寸和垂直尺寸
    UINT16 cropVSize;              // 裁剪垂直尺寸
    UINT16 cropHsize;              // 裁剪水平尺寸
    uiPara_t *puiPara;             // UI参数指针
    UINT16 pvHSize, pvVSize;       // 预览水平尺寸和垂直尺寸
    UINT16 snapXSize, snapYSize;   // 快照水平尺寸和垂直尺寸
    UINT8 ret;                     // 返回值

    // sp1kSuperPreviewSet(0,1);

    ret = appSpeedyCaptureConfig(); // 配置快速拍摄
    if (ret == IGNORE_DEFAULT)
    {
        return;
    }

    ret = appSpeedyCaptureRecConfig(); // 配置快速拍摄记录
    if (ret == IGNORE_DEFAULT)
    {
        return;
    }

    if (appPvForceUpdateGet()) // 是否需要强制更新预览
    {
        appPvForceUpdateOn(0);                // 关闭强制更新预览
        InitMode = SP1K_STILL_VIEW_INIT_FULL; // 设置初始化模式为完整静止预览
    }

    puiPara = appUiParaGet(); // 获取UI参数
    appSnapParaInit();        // 初始化快照参数

#if FUNC_HOST_IGBT_FLASH
    frontStrobeInit(); // 4 //fan.xiao modified 2011-09-21 , 17:25:43 // 初始化闪光灯
#endif

    if (InitMode == SP1K_STILL_VIEW_INIT_FULL)
    {                               // 若初始化模式为完整静止预览
        appPreviewCdspDispResSet(); // 设置预览CDSP显示分辨率
        if (appPreStateGet(0) != APP_STATE_POWER_ON)
        {                        // 如果之前的状态不是开机状态
            sp1kDispImgWinEn(0); // 显示图像窗口禁用
        }

        sp1kCdspPvDynamicParaFullLoad(0x3c); // CDSP预览动态参数全加载

        if (appPreStateGet(0) != APP_STATE_STILL_VIEW)
        {                                   // 如果之前的状态不是静止预览状态
            appIconPsTabLoad(ICON_MODE_PV); // 加载图标PS表格
        }

        sp1kAeSetModeEn(AE_DISABLE);                          // 禁用AE
        sp1kAwbDisable();                                     // 禁用AWB
        sp1kFdDisable();                                      // 禁用人脸检测
        sp1kDistDisable();                                    // 禁用镜头失真矫正
        sp1kFrontPreviewSizeGet(0, &frontHSize, &frontVSize); // 获取前置摄像头预览尺寸

        sp1kSnapSizeGet(&snapXSize, &snapYSize); // 获取快照尺寸

        pvHSize = K_Preview_Width;
        pvVSize = K_Preview_Height;

#if SUPPORT_QVGA_SENSOR
        if (frontHSize * 3 > frontVSize * 4)
        { /*sensor size 16:9*/
            cropHsize = frontHSize;
            cropVSize = frontVSize;
        }
        else
        {
            cropHsize = frontHSize;
            cropVSize = frontVSize;
        }

        if (snapXSize * 3 > snapYSize * 4)
        {
            /* snap size 16:9*/
            pvVSize = pvVSize * 3 / 4;

            // if(frontHSize*3 > frontVSize*4){/*sensor is 16:9*/
            //	cropHsize = frontHSize;
            //	cropVSize = frontVSize;
            // }
            // else{
            cropVSize = cropVSize * 3 / 4;
            //}
        }
#else
        /* sensor ratio */
        if (frontHSize * 3 > frontVSize * 4)
        {
            /*sensor size 16:9*/
            cropHsize = frontVSize * 4 / 3;
            cropVSize = frontVSize;
        }
        else
        {
            /*sensor size 4:3*/
            cropHsize = frontHSize;
            cropVSize = frontVSize;
        }
        if (snapXSize * 3 > snapYSize * 4)
        {
            /* snap size 16:9*/
            pvVSize = pvVSize * 3 / 4;

            if (frontHSize * 3 > frontVSize * 4)
            { /*sensor is 16:9*/
                cropHsize = frontHSize;
                cropVSize = frontVSize;
            }
            else
            {
                cropVSize = cropVSize * 3 / 4;
            }
        }
#endif

        //	printf("H(%u),V(%u),CH(%u),CV(%u),frontHSize(%u),frontVSize(%u)\n",snapXSize,snapYSize,cropHsize,cropVSize,frontHSize,frontVSize);
        // 设置预览裁剪尺寸
		sp1kPreviewCropSizeSet(cropHsize, cropVSize);

		#if (STILL_NEW_ZOOM == 1)
		sp1kPvZoomCtrlParaSet(SP1K_PV_ZOOM_CTRL_PARA_CURRENT_LOG, stillCurrLog); // 设置PV变焦控制参数为当前记录日志
		#else
		PvZoomCtrl.factor = stillZFctor;
		PvZoomCtrl.curLog = stillCurrLog;
		sp1kPvZoomSet(1, PvZoomCtrl.factor, 0); // 设置PV变焦
		#endif
		
		#if 0
		#if (STILL_NEW_ZOOM == 0)
		sp1kPreviewSet(0, pvHSize, pvVSize, PvZoomCtrl.factor, 0);	/* will: still 320x240*/
		#else
		sp1kPreviewSet(0, pvHSize, pvVSize, stillZFctor, 0);	/* will: still 320x240*/
		#endif
		#endif
		sp1kPreviewSet(0, pvHSize, pvVSize, sp1kPvZoomFctGet(), 0);	/* 设置预览大小，320x240 */

		sp1kHalCdspImgSrcSel(0);	// 0: 选择来自前置摄像头的图像源

		HAL_FrontSignalWait(HAL_FRONT_WAIT_VSYNC, 1, 10); // 等待前端信号

		if (appPreStateGet(0) != APP_STATE_STILL_VIEW)
        {
			// HAL_DramZUpdateWait(HAL_DRAM_ZUPDATE_PV, 0); /* will mark*/
			sp1kDispImgWinEn(1); // 启用显示图像窗口
		}

		#if (STILL_NEW_ZOOM == 1)  /*fan.xiao 添加用于 mantis:0045941 @2012/9/25，AE启用在消息: SP1K_MSG_DZOOM_3A_UPDATE*/
		//sp1kAeSetModeEn(AE_ENABLE);
		#else
		sp1kAeSetModeEn(AE_ENABLE);
		#endif

		//sp1kAwbReset();
		sp1kAwbEnable(); // 启用AWB
    }

	// 若支持自动对焦，则设置自动对焦窗口
#if (FUNC_HOST_AF_FIXTURE == 1)
	appAfWinSet(1);
#endif

	// 若支持移动侦测，则初始化移动侦测
	#if FUNC_HOST_MD
	sp1kMDInit();
	sp1kMDEnSet(MD_ENABLE);
	#endif

	// 若支持全景拍摄，则初始化全景拍摄
	#if FUNC_HOST_PANORAMA
	if (1) 
    { // 启用全景拍摄需要执行
		sp1kSnapParamSet(SP1K_SNAP_PARAM_PANORAMA_OPT, 1); // 设置快照参数，启用全景拍摄
		sp1kPanoramaParamSet(SP1K_PANORAMA_PARA_STITCH_PIXEL, 40); // 设置全景拼接像素
		sp1kPanoramaParamSet(SP1K_PANORAMA_PARA_FRAME_NUM, 14); // 设置全景帧数
		//sp1kPanoramaFinalSizeSet(1440,208);//QVGA
		sp1kPanoramaFinalSizeSet(2880, 400); // 设置全景拍摄最终尺寸
		sp1kSnapSizeSet(2880, 400); // 设置快照尺寸
		sp1kPanoramaInit(); // 初始化全景拍摄
	}
	#endif

    // 若支持照片框功能，则恢复照片框
    #if FUNC_HOST_PF_PV
	if (appPreStateGet(0) != APP_STATE_MENU) 
    {
		appPvPhotoFrameRecover();
	}
    #endif

	// 若支持人脸检测，则根据参数设置人脸检测
	if (puiPara->FDMode == FDMODE_ON) 
    {
		if (!sp1kFdGetStatus()) 
        {
			sp1kFdReset();
			sp1kFdEnable();
		}

		if (puiPara->FDMetering == FD_AE_ON) 
        {
			sp1kFdAeSetModeEn(1);
		}
		else 
        {
			sp1kFdAeSetModeEn(0);
		}

		if (puiPara->SmileMode == SMILEMODE_ON) 
        {
			sp1kFdSetSmile(1);
			sp1kFdParamSet(FD_PARAM_ShowMode, 1);
		}
		else 
        {
			sp1kFdSetSmile(0);
			sp1kFdParamSet(FD_PARAM_ShowMode, 0);
		}
	}
	else 
    {
		sp1kFdAeSetModeEn(0);
		sp1kFdSetSmile(0);
		sp1kFdDisable();
	}

	// 根据参数设置闪光灯
	if (puiPara->BlinkMode == BLINKMODE_ON) 
    {
		sp1kSnapParamSet(SP1K_SNAP_PARAM_BLINK, 1);
	}
	else 
    {
		sp1kSnapParamSet(SP1K_SNAP_PARAM_BLINK, 0);
	}

	// 根据参数设置抖动消除
	if (puiPara->AntiShake == PRV_ANTISHAKE_ON) 
    {
		sp1kAeParamSet(SP1K_AE_PARA_CAP_ANTISHAKE_FPS, 15);
	}
	else 
    {
		sp1kAeParamSet(SP1K_AE_PARA_CAP_ANTISHAKE_FPS, 0);
	}

	// 关闭闪光灯
	puiPara->FlashLed= Status_0;    // 闪光灯 --- 关闭
	appSnapFlashLed(0); // 快门时闪光灯控制 --- 关闭
	uiUpdateFlashLed(LCD_WIDTH, 0, 0);
	
	// 若不是开机状态，或者图片大小标记为1，则初始化静态OSD
	// S_imgsizeflag = 0;
	if ((appPreStateGet(0) != APP_STATE_POWER_ON) || (S_imgsizeflag == 1))
    {
		S_imgsizeflag = 0;
		appStillOsdInit(1); // 初始化静态OSD
	}

	// 若进入了微调模式，发送微调消息
	if(enterMacroadjust == 1)
    {
		osMsgPost(SP1K_MSG_Adjust_Macro);
	}
    // 若进入了版本显示模式，并且SD卡未插入，则发送版本显示消息
	if((enterVersionshow == 1)&&(!sp1kSD_CdGet()))
    {
		enterVersionshow = 0;
		osMsgPost(SP1K_MSG_Version_Show);
	}
	
}

/**
* @param    None
* @retval   None
* @brief    赋值adjust_macro_sta
**/
void app_adjust_macro_sta_set(UINT8 sta) 
{
	adjust_macro_sta = sta;
}

/**
* @param    None
* @retval   返回adjust_macro_sta
* @brief    返回 adjust_macro_sta
**/
UINT8 app_adjust_macro_sta_get(void) 
{
	return adjust_macro_sta;
}

/**
* @param    countDown: 倒计时
* @param    enableShow: 是否显示倒计时
* @retval   None
* @brief    显示自拍倒计时
**/
void uiShowOSDSelfSnapCntDown(UINT8 countDown,UINT8 enableShow)
{
	UINT8 str[3]="00"; // 定义一个字符串用于存放倒计时的数字，初始化为"00"。

	//UINT8 OsdMaxX, OsdMaxY; // 定义变量存储OSD的最大宽度和高度，这部分代码被注释了。

	//sp1kOsdLayoutAttrGet(&OsdMaxX, &OsdMaxY); // 获取OSD布局的属性，包括最大宽度和高度，这部分代码被注释了。

	countDown=countDown&0x7f; // 将countDown的值与0x7F进行位与操作，确保countDown的值不会超过127。

	str[0]=countDown/10+48; // 计算倒计时的十位数字，并转换为ASCII码存储。
	str[1]=countDown%10+48; // 计算倒计时的个位数字，并转换为ASCII码存储。

	sp1kOsdClearCharExt(128, 88, 64, 64); // 清除OSD上指定区域的字符显示，这里可能是为了清除之前的倒计时显示。

	if(strWidthGet(str)<strWidthGet("44")) // 如果当前倒计时数字的宽度小于“44”的宽度，
	{
		sp1kOsdClearCharExt(LCD_WIDTH/2-MENU_CHAR_WIDTH+strWidthGet(str),LCD_HEIGHT/2,strWidthGet("44")-strWidthGet(str),MENU_CHAR_HEIGHT);
		// 则清除多余的显示区域，以保证倒计时显示的整洁。
	}

	if(enableShow) // 如果enableShow非零，即要求显示倒计时，
	{
		// 下面两行代码被注释了，原本可能用于显示倒计时的另一种方式。
		//osdStrDraw((OsdMaxX/2-2), OsdMaxY /2, str, (1<<4)|(1<<3));
		//sp1kOsdStrDraw((OsdMaxX/2 - 1), OsdMaxY /2, str, 0);
		sp1kOsdStrDrawExt(LCD_WIDTH/2-MENU_CHAR_WIDTH,LCD_HEIGHT/2,str,0); // 在屏幕中央显示倒计时数字。
	}
	else // 如果enableShow为零，即要求隐藏倒计时，
	{
		// 下面一行代码被注释了，原本可能用于清除倒计时显示的另一种方式。
		//sp1kOsdClearChar((OsdMaxX/2 - 1), OsdMaxY /2, 2, 1);
		sp1kOsdClearCharExt(LCD_WIDTH/2-MENU_CHAR_WIDTH,LCD_HEIGHT/2,strWidthGet("44"),MENU_CHAR_HEIGHT); // 清除屏幕中央的倒计时显示。
	}
}

static UINT16 selfSnapCountDown = 0; // 静态变量，用于记录自拍倒计时时间
/**
* @param    None
* @retval   None
* @brief    倒计时并在时间到达后发送自拍消息给快照
**/
void appSelfSnapMSGSend(void)
{
	uiPara_t* uiEnviroment; // 用户界面环境指针
	UINT16 selfTime; // 自拍时间

#if 1 // 解决自拍问题的 Sunyong 标识

	// 解决自拍问题的标志
	if((0 == appSDPlugStatusGet()) && (sp1kDiskTypeGet() == DEV_TYPE_DISK_SPI)) // 如果SD卡未插入且磁盘类型为SPI
	{
		sp1kTimer1Stop(); // 停止定时器1
		selfSnapCountDown = 0; // 重置自拍倒计时时间
		sp1kBtnEnableAll(); // 使能所有按钮
		return; // 返回
	}

	if (!selfSnapCountDown) // 如果自拍倒计时时间为0
    { 
		// 配置自拍
		uiEnviroment = appUiParaGet(); // 获取用户界面参数

		switch (uiEnviroment->DriverMode) // 根据拍摄模式选择
        { 
		case PRV_SNAP_SINGLE:   // 【关闭】
		case PRV_SNAP_BURST:    // 【连拍】
        {
            selfTime = 0;
			break;
        }
		case PRV_SNAP_SELF_2S:  // 【自拍2S】
		case PRV_SNAP_CYC_2S:
        {			
            selfTime = 2;
			break;
        }
		case PRV_SNAP_SELF_5S:  // 【自拍5S】
		case PRV_SNAP_CYC_5S:
        {
            selfTime = 5;
            break;
        }
        case PRV_SNAP_SELF_10S:  // 【自拍10S】
		case PRV_SNAP_CYC_10S:
        {
            selfTime = 10;
            break;
        }
        }
		
		selfSnapCountDown = selfTime * 5; // 设置自拍倒计时时间
		uiShowOSDSelfSnapCntDown(selfSnapCountDown / 5,1); // 在OSD上显示自拍倒计时时间
		osMsgPost(SP1K_MSG_SELFSNAP_BEEP); // 发送自拍蜂鸣器消息
		return;
	}

	if(!( --selfSnapCountDown)) // 如果自拍倒计时时间为0
    {
		// 准备拍摄
		selfSnapCountDown=0; // 重置自拍倒计时时间
		uiShowOSDSelfSnapCntDown(selfSnapCountDown / 5, 0); // 在OSD上显示自拍倒计时时间
		sp1kTimer1Stop(); // 停止定时器1
		osMsgPost(SP1K_MSG_KEY_PRESS_SELFSNAP); // 发送按键消息
	} 
    else 
    {
		if (!(selfSnapCountDown % 5)) // 每5个计时单位
        { 
			uiShowOSDSelfSnapCntDown(selfSnapCountDown / 5, 1); // 在OSD上显示自拍倒计时时间
			osMsgPost(SP1K_MSG_SELFSNAP_BEEP); // 发送自拍蜂鸣器消息
		} 
        else if (selfSnapCountDown <= 2 * 5) // 如果倒计时时间小于等于2个计时单位
        { 
			osMsgPost(SP1K_MSG_SELFSNAP_BEEP); // 发送自拍蜂鸣器消息
		}
	}

#else//sunyong @2008.6.23 add for mantis:25742 begin
	if(!selfSnapCountDown){//
		sp1kTimer1Stop();
		uiEnviroment = appUiParaGet();
		if(uiEnviroment->DriverMode==PRV_SNAP_SELF_10S) {
			selfTime = 10*(UINT16)1000;
			tmp0 = 2*(UINT16)1000;
			sp1kTimer1Config((UINT32)cnt, pFunc_SelfSnapMSGSendShell,NULL);
		}else if(uiEnviroment->DriverMode==PRV_SNAP_SELF_2S){
			selfTime = 2*(UINT16)1000;
			tmp0 = 1*(UINT16)1000;
			cnt=500;
			sp1kTimer1Config((UINT32)cnt, pFunc_SelfSnapMSGSendShell,NULL);
		}else{
			selfTime = 0;
		}
		selfSnapCountDown = selfTime;
		sp1kTimer1Start();
		return;
	}

	uiShowOSDSelfSnapCntDown(selfSnapCountDown/1000,1);
	appVoicePlay(SP1K_MSG_UNKNOWN_HOST,VOICE_TYPE_BEEP, VOICE_PLAYLEVLE_HIGH);
	sp1kLED_SelfSet(0);
	selfSnapCountDown -= cnt;
	if(selfSnapCountDown == tmp0){//change  frequency
		if(selfTime == 10*(UINT16)1000){
			cnt = 500;
		}else{
			cnt = 200;
		}
		sp1kTimer1Stop();
		sp1kTimer1Config((UINT32)cnt, pFunc_SelfSnapMSGSendShell,NULL);
		sp1kTimer1Start();
	} else if(selfSnapCountDown<=300) {//post capture mssage
		sp1kTimer1Stop();
		selfSnapCountDown=0;
		uiShowOSDSelfSnapCntDown(selfSnapCountDown, 0);
		osMsgPost(SP1K_MSG_KEY_PRESS_SELFSNAP);
		cnt=1000;
	}
	sp1kHalWait(50);//The time must less than "cnt". specify LED turn on time
	sp1kLED_SelfSet(1);

#endif //sunyong @2008.6.23 add for mantis:25742 end
}

/**
* @param    None
* @retval   None
* @brief    自拍处理函数
**/
void appSelfSnapHandle(void)
{
    // 播放一个未知主机的声音提示，类型为蜂鸣声，播放级别为高。
	appVoicePlay(SP1K_MSG_UNKNOWN_HOST,VOICE_TYPE_BEEP, VOICE_PLAYLEVLE_HIGH);

#ifndef WHITE_LED
    // 如果没有定义WHITE_LED，即不使用白色LED，那么执行关闭自拍LED灯的操作。
	sp1kLED_SelfSet(0);
#endif

    // 等待100毫秒，提供一个短暂的延时。
	sp1kHalWait(100);

#ifndef WHITE_LED
    // 如果没有定义WHITE_LED，即在不使用白色LED的情况下，执行打开自拍LED灯的操作。
	sp1kLED_SelfSet(1);
#endif
}
/**
* @param    None
* @retval   None
* @brief    根据拍摄模式设置静态拍摄参数
**/
void appStillPreviewReset(void) 
{
	UINT16 frontHSize,frontVSize;  // 定义前置摄像头水平和垂直尺寸变量
	UINT16 cropVSize;  // 定义裁剪垂直尺寸变量
	UINT16 cropHsize;  // 定义裁剪水平尺寸变量
	uiPara_t* puiPara ;  // 定义指向uiPara_t类型的指针变量puiPara
	UINT16 pvHSize,pvVSize;  // 定义预览水平和垂直尺寸变量
	UINT16 snapXSize,snapYSize;  // 定义拍摄图像的水平和垂直尺寸变量
	UINT8 ret;  // 定义返回值变量ret

	//sp1kDispImgWinEn(0);  // 关闭图像窗口显示

	sp1kFrontPreviewSizeGet(0, &frontHSize, &frontVSize);  // 获取前置摄像头预览尺寸

	sp1kSnapSizeGet(&snapXSize,&snapYSize);  // 获取拍摄图像尺寸

	pvHSize = K_Preview_Width;  // 设置预览窗口的水平尺寸为K_Preview_Width
	pvVSize = K_Preview_Height;  // 设置预览窗口的垂直尺寸为K_Preview_Height

	#if SUPPORT_QVGA_SENSOR
		// 如果支持QVGA传感器
		if(frontHSize*3 > frontVSize*4){  // 如果传感器尺寸为16:9
			cropHsize = frontHSize;  // 设置裁剪水平尺寸为前置摄像头水平尺寸
			cropVSize = frontVSize;  // 设置裁剪垂直尺寸为前置摄像头垂直尺寸
		}else{
			cropHsize = frontHSize;  // 设置裁剪水平尺寸为前置摄像头水平尺寸
			cropVSize = frontVSize;  // 设置裁剪垂直尺寸为前置摄像头垂直尺寸
		}

		if ( snapXSize*3 > snapYSize*4 ) {
			// 如果拍摄图像尺寸为16:9
			pvVSize = pvVSize*3/4;  // 调整预览垂直尺寸为3/4倍

			cropVSize = cropVSize*3/4;  // 调整裁剪垂直尺寸为3/4倍
		}
	#else
		// 如果不支持QVGA传感器
		/* 传感器比例 */
		if ( frontHSize*3 > frontVSize*4 ) {
			// 如果传感器尺寸为16:9
			cropHsize = frontVSize*4/3;  // 计算裁剪水平尺寸
			cropVSize = frontVSize;  // 设置裁剪垂直尺寸为前置摄像头垂直尺寸
		}
		else {
			// 如果传感器尺寸为4:3
			cropHsize = frontHSize;  // 设置裁剪水平尺寸为前置摄像头水平尺寸
			cropVSize = frontVSize;  // 设置裁剪垂直尺寸为前置摄像头垂直尺寸
		}
		if ( snapXSize*3 > snapYSize*4 ) {
			// 如果拍摄图像尺寸为16:9
			pvVSize = pvVSize*3/4;  // 调整预览垂直尺寸为3/4倍

			if(frontHSize*3 > frontVSize*4){  // 如果传感器尺寸为16:9
				cropHsize = frontHSize;  // 设置裁剪水平尺寸为前置摄像头水平尺寸
				cropVSize = frontVSize;  // 设置裁剪垂直尺寸为前置摄像头垂直尺寸
			}
			else{
				cropVSize = cropVSize*3/4;  // 调整裁剪垂直尺寸为3/4倍
			}
		}
	#endif

	//	printf("H(%u),V(%u),CH(%u),CV(%u),frontHSize(%u),frontVSize(%u)\n",snapXSize,snapYSize,cropHsize,cropVSize,frontHSize,frontVSize);
	sp1kPreviewCropSizeSet(cropHsize,cropVSize);  // 设置预览裁剪尺寸

	sp1kPreviewSet(0, pvHSize, pvVSize, sp1kPvZoomFctGet(), 0);  // 设置预览参数

	sp1kHalCdspImgSrcSel(0);	// 0: 从前置摄像头获取图像源

	// HAL_FrontSignalWait(HAL_FRONT_WAIT_VSYNC, 1, 4);  // 等待前置信号的垂直同步信号

	// sp1kDispImgWinEn(1); // hlc, Mantis #0021510   // 开启图像窗口显示
}

/**
* @param    None
* @retval   SUCCESS/FAIL
* @brief    根据不同的拍摄模式设置相应的拍摄参数，并执行相应的操作
**/
UINT8 appDriveModeSet(void)
{
	uiPara_t* puiEnviroment;  // 定义指向uiPara_t类型的指针变量puiEnviroment

	puiEnviroment = appUiParaGet();  // 获取UI参数

	switch (puiEnviroment->DriverMode) // 根据拍摄模式进行不同的操作
    {
    case PRV_SNAP_SINGLE: // 【关闭】
    {
        sp1kSnapParamSet(SP1K_SNAP_PARAM_BURST, 0); // 设置拍摄参数为单拍模式
        break;
    }
    case PRV_SNAP_BURST:  // 【连拍模式】
    {
        if (RemainPictureNO < 3)
        {                                                 // 如果剩余图片数量小于3
            uiUpdateOSDMemFull();                         // 更新OSD显示内存已满
            isUpdateNum = 1;                              // 设置更新数字标志
            appStillViewInit(SP1K_STILL_VIEW_INIT_SMART); // 初始化静态拍摄界面
            return FAIL;                                  // 返回失败
        }
        else
        {
            // sp1kSuperPreviewSet(0,0);
            appStillPreviewReset();                     // 重置静态预览设置
            sp1kSnapParamSet(SP1K_SNAP_PARAM_BURST, 1); // 设置拍摄参数为连拍模式
            // sp1kSnapParamSet(SP1K_SNAP_PARAM_DOFLOW_OPT, 1);  // 连拍模式
            return SUCCESS; // 返回成功
        }
        break;
    }
    case PRV_SNAP_SELF_2S:
	case PRV_SNAP_SELF_5S:
	case PRV_SNAP_SELF_10S:
    {
        if (RemainPictureNO == 0)
        {                                                 // 如果剩余图片数量为0
            uiUpdateOSDMemFull();                         // 更新OSD显示内存已满
            isUpdateNum = 1;                              // 设置更新数字标志
            appStillViewInit(SP1K_STILL_VIEW_INIT_SMART); // 初始化静态拍摄界面
        }
        else
        {
            sp1kSnapParamSet(SP1K_SNAP_PARAM_BURST, 0); // 设置拍摄参数为非连拍模式

            sp1kBtnDisableAll(); // 禁用所有按钮

            appOsdSelfSnapEnableOsd(1);                              // 开启自拍OSD
            pFunc_SelfSnapMSGSendShell();                            // 发送自拍消息到Shell
            sp1kTimer1Config(200, pFunc_SelfSnapMSGSendShell, NULL); // 配置定时器1
            sp1kTimer1Start();                                       // 启动定时器1
        }
        return FAIL; // 返回失败
        break;
    }
    case PRV_SNAP_CYC_2S:
	case PRV_SNAP_CYC_5S:
	case PRV_SNAP_CYC_10S:
    {
        if (RemainPictureNO == 0) // 如果剩余图片数量为0
        {
            uiUpdateOSDMemFull();                         // 更新OSD显示内存已满
            isUpdateNum = 1;                              // 设置更新数字标志
            appStillViewInit(SP1K_STILL_VIEW_INIT_SMART); // 初始化静态拍摄界面
        }
        else
        {
            sp1kSnapParamSet(SP1K_SNAP_PARAM_BURST, 0); // 设置拍摄参数为非连拍模式

            sp1kBtnDisableAll(); // 禁用所有按钮

            appOsdSelfSnapEnableOsd(1);                              // 开启自拍OSD
            pFunc_SelfSnapMSGSendShell();                            // 发送自拍消息到Shell
            sp1kTimer1Config(200, pFunc_SelfSnapMSGSendShell, NULL); // 配置定时器1
            sp1kTimer1Start();                                       // 启动定时器1
        }
        return FAIL; // 返回失败
        break;
    }
    }

	return SUCCESS;  // 默认返回成功
}

/**
* @param    effect: 摄像头效果枚举类型 @PRV_EFFECT
* @retval   SUCCESS/FAIL
* @brief    输入的效果参数设置相应的摄像头效果
**/
UINT8 appEffectSet(UINT8 effect)
{
	switch(effect)
	{
		case PRV_EFFECT_NORNAL:  // 【普通效果】
        {
			sp1kCdspEffect_Normal();  // 设置为普通效果
			sp1kAeNegativeEnSet(0);  // 设置自动曝光负片使能为0
			break;        
        }
		case PRV_EFFECT_BLACKWHITE:  // 【黑白效果】
        {
            sp1kCdspEffect_BlackWhite(); // 设置为黑白效果
            sp1kAeNegativeEnSet(0);      // 设置自动曝光负片使能为0
            break;
        }
        case PRV_EFFECT_SEPIA:  // 【复古效果】
        {
			sp1kCdspEffect_Sepia();  // 设置为复古效果
			sp1kAeNegativeEnSet(0);  // 设置自动曝光负片使能为0
			break;            
        }
		case PRV_EFFECT_NEGATIVE:  // 【负片效果】
        {
            sp1kCdspEffect_Negative(); // 设置为负片效果
            sp1kAeNegativeEnSet(1);    // 设置自动曝光负片使能为1
            break;
        }
        case PRV_EFFECT_RED:  // 【红色效果】
        {
            sp1kCdspEffect_Red();   // 设置为红色效果
            sp1kAeNegativeEnSet(0); // 设置自动曝光负片使能为0
            break;
        }
        case PRV_EFFECT_GREEN:  // 【绿色效果】
        {
            sp1kCdspEffect_Green(); // 设置为绿色效果
            sp1kAeNegativeEnSet(0); // 设置自动曝光负片使能为0
            break;
        }
        case PRV_EFFECT_BLUE:  // 【蓝色效果】
        {
            sp1kCdspEffect_Blue();  // 设置为蓝色效果
            sp1kAeNegativeEnSet(0); // 设置自动曝光负片使能为0
            break;
        }
        default:  // 默认情况
        {
            sp1kCdspEffect_Normal(); // 设置为正常效果
            sp1kAeNegativeEnSet(0);  // 设置自动曝光负片使能为0
            return FAIL;             // 返回失败
        }
    }
	//sp1kAwbParamSet(AWB_PARA_DarkSatMode, effect==PRV_EFFECT_NORNAL? AWB_FUNC_Enable : AWB_FUNC_Disable);
	return SUCCESS;  // 返回成功
}

/**
* @param    ContrastValue: @PRV_CONTRAST
* @retval   None
* @brief    根据输入的对比度值设置相应的对比度增强参数
**/
UINT8 appContrastSet(UINT8 ContrastValue)
{
    switch (ContrastValue)
    {
    case PRV_CONTRAST_HIGH: // 高对比度
    {
        sp1kCdspContrastEnhanSet(1, 0x28); // 设置为1.25倍增强
        break;
    }

    case PRV_CONTRAST_MIDDLE: // 中等对比度
    {
        sp1kCdspContrastEnhanSet(1, 0x20); // 设置为1倍增强
        break;
    }

    case PRV_CONTRAST_LOW: // 低对比度
    {
        sp1kCdspContrastEnhanSet(1, 0x18); // 设置为0.75倍增强
        break;
    }

    case PRV_CONTRAST_OFF: // 关闭对比度增强
    {
        sp1kCdspContrastEnhanSet(0, 0x20); // 设置为1倍增强
        break;
    }

    default: // 默认情况
    {
        sp1kCdspContrastEnhanSet(1, 0x20); // 设置为1倍增强
        break;
    }
    }

    return SUCCESS; // 返回成功
}

/**
* @param    SaturationValue: @PRV_SATURATION
* @retval   None
* @brief    根据输入的饱和度值设置相应的饱和度增强参数
**/
UINT8 appSaturationSet(UINT8 SaturationValue)
{
    switch (SaturationValue)
    {
    case PRV_SATURATION_HIGH: // 高饱和度
    {
        sp1kCdspSaturationEnhanSet(1, 0x2E, 0x00, 0x00); // 设置为1.25倍增强
        break;
    }

    case PRV_SATURATION_MIDDLE: // 中等饱和度
    {
        sp1kCdspSaturationEnhanSet(1, 0x2E, 0x00, 0x00); // 设置为1倍增强
        break;
    }

    case PRV_SATURATION_LOW: // 低饱和度
    {
        sp1kCdspSaturationEnhanSet(1, 0x2E, 0x00, 0x00); // 设置为0.75倍增强
        break;
    }

    default: // 默认情况
    {
        sp1kCdspSaturationEnhanSet(1, 0x2E, 0x00, 0x00); // 设置为1倍增强
        break;
    }
    }

    return SUCCESS; // 返回成功
}

/**
* @param    SharpnessValue: @PRV_SHARPNESS
* @retval   None
* @brief    根据输入的锐度值设置相应的锐度增强参数
**/
UINT8 appSharpnessSet(UINT8 SharpnessValue)
{
    switch (SharpnessValue)
    {
    case PRV_EDGE_SOFT: // 边缘柔和
    {
#if (EDGE_9_LEVEL_FUN == 0)
            sp1kCdspSharpnessEnhanSet(CDSP_DYNAMIC_DISABLE, CDSP_DYNAMIC_WEAK); // 设置为关闭动态增强，弱增强
#else
        sp1kCdspStillDynamicParaInit(SP1K_RSC_ID_STILLPAR_WEAK); // 初始化静态参数为弱增强
#endif
        break;
    }

    case PRV_EDGE_NORMAL: // 普通边缘
    {
#if (EDGE_9_LEVEL_FUN == 0)
            sp1kCdspSharpnessEnhanSet(CDSP_DYNAMIC_DISABLE, CDSP_DYNAMIC_MIDDLE); // 设置为关闭动态增强，中等增强
#else
        sp1kCdspStillDynamicParaInit(SP1K_RSC_ID_STILLPAR_MIDDLE); // 初始化静态参数为中等增强
#endif
        break;
    }

    case PRV_EDGE_HARD: // 边缘硬化
    {
#if (EDGE_9_LEVEL_FUN == 0)
            sp1kCdspSharpnessEnhanSet(CDSP_DYNAMIC_DISABLE, CDSP_DYNAMIC_STRONG); // 设置为关闭动态增强，强增强
#else
        sp1kCdspStillDynamicParaInit(SP1K_RSC_ID_STILLPAR_STRONG); // 初始化静态参数为强增强
#endif
        break;
    }

    default: // 默认情况
    {
#if (EDGE_9_LEVEL_FUN == 0)
            sp1kCdspSharpnessEnhanSet(CDSP_DYNAMIC_ENABLE, CDSP_DYNAMIC_WEAK); // 设置为开启动态增强，弱增强
#else
        sp1kCdspStillDynamicParaInit(SP1K_RSC_ID_STILLPAR_WEAK); // 初始化静态参数为弱增强
#endif
        break;
    }
    }

    return SUCCESS; // 返回成功
}

/**
* @param    scene: 场景 @PRV_SCENE
* @retval   None
* @brief    描述
**/
UINT8 appSceneSet(UINT8 scene)
{
    // 获取当前UI参数结构体指针
    uiPara_t *puiPara = appUiParaGet();

    // 定义色调资源ID数组
    UINT8 hueResId[3] = {0xe0, 0xe1, 0xe2};

    switch (scene)
    {
    default:
    case PRV_SCENE_AUTO:    // 【自动】
    {
        SCENE_SET_PRINT("auto\n"); // 设置场景为自动
        G_SceneMode = PRV_SCENE_AUTO;
        puiPara->AEMode = PRV_COMP_0EV;              // 设置曝光模式为0EV
        puiPara->Iso = PRV_ISO_AUTO;                 // 设置ISO为自动
        puiPara->WBMode = PRV_WB_AUTO;               // 设置白平衡模式为自动
        puiPara->ImageEffect = PRV_EFFECT_NORNAL;    // 设置图像效果为正常
        puiPara->FDMode = FDMODE_OFF;                // 设置人脸检测模式为关闭
        puiPara->DriverMode = PRV_SNAP_SINGLE;       // 设置驱动模式为单拍
        puiPara->Sharpness = PRV_EDGE_NORMAL;        // 设置锐度为普通
        puiPara->Saturation = PRV_SATURATION_MIDDLE; // 设置饱和度为中间
        puiPara->Contrast = PRV_CONTRAST_OFF;        // 设置对比度为关闭（或者中间）
        break;
    }
    case PRV_SCENE_LANDSCAPE:    // 【风景】
    {
        SCENE_SET_PRINT("landscap\n"); // 设置场景为风景
        G_SceneMode = PRV_SCENE_LANDSCAPE;
        puiPara->AEMode = PRV_COMP_0EV;
        puiPara->Iso = PRV_ISO_AUTO;
        puiPara->WBMode = PRV_WB_AUTO;
        puiPara->ImageEffect = PRV_EFFECT_NORNAL;
        puiPara->FDMode = FDMODE_OFF;
        puiPara->DriverMode = PRV_SNAP_SINGLE;
        puiPara->Sharpness = PRV_EDGE_HARD;        // 设置锐度为强
        puiPara->Saturation = PRV_SATURATION_HIGH; // 设置饱和度为高
        puiPara->Contrast = PRV_CONTRAST_HIGH;     // 设置对比度为高
        hueResId[0] = 0xda;
        hueResId[1] = 0xdb;
        hueResId[2] = 0xdc;
        break;
    }
    case PRV_SCENE_NIGHTSCENERY:    // 【夜景】
    {
        SCENE_SET_PRINT("night_scene\n"); // 设置场景为夜景
        G_SceneMode = PRV_SCENE_NIGHTSCENERY;
        puiPara->AEMode = PRV_COMP_0EV;
        puiPara->Iso = PRV_ISO_100; // 设置ISO为100
        puiPara->WBMode = PRV_WB_AUTO;
        puiPara->ImageEffect = PRV_EFFECT_NORNAL;
        puiPara->FDMode = FDMODE_OFF;
        puiPara->DriverMode = PRV_SNAP_SINGLE;
        puiPara->Sharpness = PRV_EDGE_NORMAL;
        puiPara->Saturation = PRV_SATURATION_MIDDLE;
        puiPara->Contrast = PRV_CONTRAST_MIDDLE;
        break;
    }
    case PRV_SCENE_PORTRAIT:    // 【肖像】
    {
        SCENE_SET_PRINT("portrait\n"); // 设置场景为肖像
        G_SceneMode = PRV_SCENE_PORTRAIT;
        puiPara->AEMode = PRV_COMP_0EV;
        puiPara->Iso = PRV_ISO_AUTO;
        puiPara->WBMode = PRV_WB_AUTO;
        puiPara->ImageEffect = PRV_EFFECT_NORNAL;
        puiPara->FDMode = FDMODE_ON; // 设置人脸检测模式为开启
        puiPara->DriverMode = PRV_SNAP_SINGLE;
        puiPara->Sharpness = PRV_EDGE_NORMAL;
        puiPara->Saturation = PRV_SATURATION_HIGH;
        puiPara->Contrast = PRV_CONTRAST_MIDDLE;
        hueResId[0] = 0xdd;
        hueResId[1] = 0xde;
        hueResId[2] = 0xdf;
        break;
    }
    case PRV_SCENE_NIGHTPORTRAIT:    // 【夜画像】
    {
        SCENE_SET_PRINT("night_portrait\n"); // 设置场景为夜间肖像
        G_SceneMode = PRV_SCENE_NIGHTPORTRAIT;
        puiPara->AEMode = PRV_COMP_0EV;
        puiPara->Iso = PRV_ISO_AUTO;
        puiPara->WBMode = PRV_WB_AUTO;
        puiPara->ImageEffect = PRV_EFFECT_NORNAL;
        puiPara->FDMode = FDMODE_ON;
        puiPara->DriverMode = PRV_SNAP_SINGLE;
        puiPara->Sharpness = PRV_EDGE_NORMAL;
        puiPara->Saturation = PRV_SATURATION_MIDDLE;
        puiPara->Contrast = PRV_CONTRAST_MIDDLE;
        break;
    }
    case PRV_SCENE_BEACH:    // 【海滩】
    {
        SCENE_SET_PRINT("beach\n"); // 设置场景为海滩
        G_SceneMode = PRV_SCENE_BEACH;
        puiPara->AEMode = PRV_COMP_0EV;
        puiPara->Iso = PRV_ISO_AUTO;
        puiPara->WBMode = PRV_WB_DAYLIGHT; // 设置白平衡模式为日光
        puiPara->ImageEffect = PRV_EFFECT_NORNAL;
        puiPara->FDMode = FDMODE_OFF;
        puiPara->DriverMode = PRV_SNAP_SINGLE;
        puiPara->Sharpness = PRV_EDGE_HARD;
        puiPara->Saturation = PRV_SATURATION_HIGH;
        puiPara->Contrast = PRV_CONTRAST_HIGH;
        hueResId[0] = 0xd7;
        hueResId[1] = 0xd8;
        hueResId[2] = 0xd9;
        break;
    }
    case PRV_SCENE_SPORT:    // 【运动】
    {
        SCENE_SET_PRINT("sport\n"); // 设置场景为运动
        G_SceneMode = PRV_SCENE_SPORT;
        puiPara->AEMode = PRV_COMP_0EV;
        puiPara->Iso = PRV_ISO_800; // 设置ISO为800
        puiPara->WBMode = PRV_WB_AUTO;
        puiPara->ImageEffect = PRV_EFFECT_NORNAL;
        puiPara->FDMode = FDMODE_OFF;
        puiPara->DriverMode = PRV_SNAP_SINGLE;
        puiPara->Sharpness = PRV_EDGE_NORMAL;
        puiPara->Saturation = PRV_SATURATION_MIDDLE;
        puiPara->Contrast = PRV_CONTRAST_MIDDLE;
        break;
    }
    case PRV_SCENE_DINNER:    // 【宴会】
    {
        SCENE_SET_PRINT("dinner\n"); // 设置场景为晚餐
        G_SceneMode = PRV_SCENE_DINNER;
        puiPara->AEMode = PRV_COMP_0EV;
        puiPara->Iso = PRV_ISO_AUTO;
        puiPara->WBMode = PRV_WB_AUTO;
        puiPara->ImageEffect = PRV_EFFECT_NORNAL;
        puiPara->FDMode = FDMODE_OFF;
        puiPara->DriverMode = PRV_SNAP_SINGLE;
        puiPara->Sharpness = PRV_EDGE_NORMAL;
        puiPara->Saturation = PRV_SATURATION_HIGH;
        puiPara->Contrast = PRV_CONTRAST_MIDDLE;
        break;
    }
    }

    appSharpnessSet(puiPara->Sharpness);   // 设置锐度
    appSaturationSet(puiPara->Saturation); // 设置饱和度
    appContrastSet(puiPara->Contrast);     // 设置对比度
    appEVSet(puiPara->AEMode);             // 设置曝光模式
    appISOSet(puiPara->Iso);               // 设置ISO
    appWBSet(puiPara->WBMode);             // 设置白平衡模式
    appEffectSet(puiPara->ImageEffect);    // 设置图像效果

    /* Hue */
    sp1kCdspHueEnhanSet(1, 0, hueResId[0]);
    // sp1kCdspHueEnhanSet(1, 1, hueResId[1]);
    // sp1kCdspHueEnhanSet(1, 2, hueResId[2]);

    return SUCCESS;
}

/**
 * @brief 根据传入的白平衡值设置相机的白平衡模式
 * 
 * @param WBValue 白平衡值，可能取值为 PRV_WB_AUTO, PRV_WB_CLOUDY, PRV_WB_DAYLIGHT, PRV_WB_FLUORESCENT_H, PRV_WB_TUNGSTEN
 * @return UINT8 操作成功返回SUCCESS
 */
/**
* @param    WBValue: 白平衡 @PRV_WB
* @retval   None
* @brief    根据传入的白平衡值设置相机的白平衡模式
**/
UINT8 appWBSet(UINT8 WBValue)
{
    switch (WBValue)
    {
    case PRV_WB_AUTO: // 【自动】
    {
        sp1kAwbModeSet(AWB_MODE_Auto); // 设置白平衡模式为自动
        break;
    }
    case PRV_WB_CLOUDY: // 【多云】
    {
        sp1kAwbModeSet(AWB_MODE_Cloudy); // 设置白平衡模式为多云
        break;
    }
    case PRV_WB_DAYLIGHT: // 【日光】
    {
        sp1kAwbModeSet(AWB_MODE_Sunny); // 设置白平衡模式为日光
        break;
    }
    case PRV_WB_FLUORESCENT_H: // 【荧光灯】
    {
        sp1kAwbModeSet(AWB_MODE_Fluorescent); // 设置白平衡模式为荧光灯
        break;
    }
    case PRV_WB_TUNGSTEN: // 【钨丝灯】
    {
        sp1kAwbModeSet(AWB_MODE_Tungsten); // 设置白平衡模式为钨灯
        break;
    }
    default:
    {
        sp1kAwbDisable(); // 禁用白平衡
        break;
    }
    }

    return SUCCESS;
}

/**
* @param    None
* @retval   操作成功返回IGNORE_DEFAULT，否则返回CALL_DEFAULT
* @brief    配置快速捕获功能的参数，包括图像大小、缩放因子等
**/
UINT8 appSpeedyCaptureConfig(void)
{
	UINT8 ret = CALL_DEFAULT;     // 返回值，默认为CALL_DEFAULT
	UINT8 dataModeBak, videoSizeBak;   // 数据模式和视频大小的备份变量
	uiPara_t* puiPara;
	puiPara = appUiParaGet();   // 获取UI参数指针
	sp1kDispImgWinEn(0);   // 关闭显示图像窗口

	if (puiPara->SpeedyCap == SPEEDY_CAPTURE_ON)   // 如果快速捕获开启
	{
		dataModeBak = puiPara->StampVideoMode;   // 备份数据模式
		videoSizeBak = puiPara->VideoSize;   // 备份视频大小
		puiPara->StampVideoMode = puiPara->StampMode;   // 设置数据模式为印章模式
		puiPara->VideoSize = MOV_SIZE_VGA;   // 设置视频大小为VGA

		__FUNC_TRACK__;
		printf("stillfactor=%d,curlog=%d\n", stillZFctor, stillCurrLog);   // 打印缩放因子和当前日志

		#if (VIDEO_NEW_ZOOM == 1)
		sp1kVideoZoomCtrlParaSet(SP1K_VIDEO_ZOOM_CTRL_PARA_CURRENT_LOG, stillCurrLog);   // 设置视频缩放参数
		#else
		video_zoomInFlag = 1;   // 设置视频缩放标志为1
		video_zoomFactor = stillZFctor;   // 设置视频缩放因子
		ZoomCurLog = stillCurrLog;   // 设置当前日志
		//sp1kPvZoomSet(0, video_zoomFactor, 0); // hlc, Mantis: 0021118
		//sp1kVideoRecZoomFacSet(video_zoomFactor, 0);
		#endif

		if ((sp1kVideoRecParamGet(VD_CAPTURE) == 0) || (appPreStateGet(0) != APP_STATE_MENU))   // 如果视频捕获参数为0或前置状态不为APP_STATE_MENU
		{
			sp1kHalCtrlScreen(0);   // 控制屏幕关闭

			sp1kAeSetModeEn(AE_DISABLE);   // 禁用自动曝光模式
			sp1kAwbDisable();   // 禁用白平衡
			sp1kFdDisable();   // 禁用人脸检测
			sp1kDistDisable();   // 禁用距离检测

			sp1kVideoRecParamSet(VD_CAPTURE, 1);   // 设置视频捕获参数为1
			sp1kVideoCapParamSet(VD_CAP_COUNT, 6);   // 设置视频捕获次数为6
			sp1kVideoCapParamSet(VD_CAP_SIZE, VD_CAP_VGA);   // 设置视频捕获大小为VGA
			//sp1kVideoCapParamSet(SP1K_VD_CAP_COUNT, 3);
			//sp1kVideoCapParamSet(SP1K_VD_CAP_SIZE, VD_CAP_HD);
			sp1kVideoCapParamSet(SP1K_VD_CAP_MOD, SP1K_SPEEDCAP_STILL);   // 设置视频捕获模式为静态快速捕获
			if (puiPara->ImageSize != PRV_SIZE_VGA)   // 如果图像大小不为PRV_SIZE_VGA
			{
				puiPara->ImageSize = PRV_SIZE_VGA;   // 设置图像大小为PRV_SIZE_VGA
			}

			isUpdateNum = 0;   // 更新数量为0
			appVideoResSet(puiPara->VideoSize);   // 设置视频分辨率
			sp1kYuvBufModeSet();   // 设置YUV缓冲模式，用于快速捕获使用4个第一路径帧缓冲，Mantis 45534
			appVideoPreSizeSet(puiPara->VideoSize);   // 设置视频预览大小

			sp1kVideoRecFrontRateSet(0x00);   // 设置前置摄像头速率为0x00
			sp1kVideoRecParamSet(VD_REC_YUV_FORMAT, VD_REC_YUV_422_420);   // 设置视频录制YUV格式为VD_REC_YUV_422_420
			sp1kAeSetModeEn(AE_ENABLE);   // 启用自动曝光模式
			sp1kAwbEnable();   // 启用白平衡

		}

		appIconPsTabLoad(ICON_MODE_PV);   // 加载图标PS表格
		appStillOsdInit(1);   // 初始化静态OSD
		appVideoStampSet();   // 在osdinit之后移动
		puiPara->StampVideoMode = dataModeBak;   // 恢复数据模式
		puiPara->VideoSize = videoSizeBak;   // 恢复视频大小

		sp1kHalCtrlScreen(3);   // 控制屏幕打开
		HAL_FrontSignalWait(HAL_FRONT_WAIT_VSYNC, 1, 4);   // 等待前置信号同步
		ret = IGNORE_DEFAULT;   // 返回值设为IGNORE_DEFAULT
	}
	else
	{
		if (sp1kVideoRecParamGet(VD_CAPTURE))   // 如果视频捕获参数为1
		{
			sp1kVideoRecParamSet(VD_CAPTURE, 0);   // 设置视频捕获参数为0
			appPvForceUpdateOn(1);   // 强制更新Pv
		}
		ret = CALL_DEFAULT;   // 返回值设为CALL_DEFAULT
	}

	sp1kDispImgWinEn(1);   // 打开显示图像窗口
	return ret;   // 返回操作成功标识
}

/**
* @param    None
* @retval   操作成功返回IGNORE_DEFAULT，否则返回CALL_DEFAULT
* @brief    配置快速捕获录制功能的参数，包括图像大小、缩放因子等
**/
UINT8 appSpeedyCaptureRecConfig(void)
{
	UINT8 ret = CALL_DEFAULT;     // 返回值，默认为CALL_DEFAULT
	UINT8 dataModeBak, videoSizeBak;   // 数据模式和视频大小的备份变量
	uiPara_t* puiPara;
	puiPara = appUiParaGet();   // 获取UI参数指针
	sp1kDispImgWinEn(0);   // 关闭显示图像窗口

	if (puiPara->SpeedyCapRec == SPEEDY_CAPTURE_REC_ON)   // 如果快速捕获录制开启
	{
		dataModeBak = puiPara->StampVideoMode;   // 备份数据模式
		videoSizeBak = puiPara->VideoSize;   // 备份视频大小
		puiPara->StampVideoMode = puiPara->StampMode;   // 设置数据模式为印章模式

		__FUNC_TRACK__;
		printf("stillfactor=%d,curlog=%d\n", stillZFctor, stillCurrLog);   // 打印缩放因子和当前日志

		if ((sp1kVideoRecParamGet(VD_CAPTURE) == 0) || (appPreStateGet(0) != APP_STATE_MENU))   // 如果视频捕获参数为0或前置状态不为APP_STATE_MENU
		{
			sp1kHalCtrlScreen(0);   // 控制屏幕关闭

			sp1kAeSetModeEn(AE_DISABLE);   // 禁用自动曝光模式
			sp1kAwbDisable();   // 禁用白平衡
			sp1kFdDisable();   // 禁用人脸检测
			sp1kDistDisable();   // 禁用距离检测

			sp1kVideoRecParamSet(VD_CAPTURE, 1);   // 设置视频捕获参数为1
			sp1kVideoCapParamSet(SP1K_VD_CAP_COUNT, 3);   // 设置视频捕获次数为3

			if (puiPara->ImageSize == PRV_SIZE_VGA)   // 如果图像大小为PRV_SIZE_VGA
			{
				sp1kVideoCapParamSet(SP1K_VD_CAP_SIZE, VD_CAP_VGA);   // 设置视频捕获大小为VD_CAP_VGA
				puiPara->VideoSize = MOV_SIZE_VGA;   // 设置视频大小为MOV_SIZE_VGA
			}
			else
			{
				sp1kVideoCapParamSet(SP1K_VD_CAP_SIZE, VD_CAP_HD);   // 设置视频捕获大小为VD_CAP_HD
				puiPara->VideoSize = MOV_SIZE_HD;   // 设置视频大小为MOV_SIZE_HD
				puiPara->ImageSize = PRV_SIZE_1M_16_9;   // 设置图像大小为PRV_SIZE_1M_16_9
			}

			sp1kVideoCapParamSet(SP1K_VD_CAP_MOD, SP1K_SPEEDCAP_CLIP);   // 设置视频捕获模式为剪辑快速捕获
			puiPara->FrameRate = MOV_FRM_MIDDLE;   // 设置帧率为MOV_FRM_MIDDLE

			isUpdateNum = 0;   // 更新数量为0
			appVideoFrmRateSet();   // 设置视频帧率
			appVideoResSet(puiPara->VideoSize);   // 设置视频分辨率
			sp1kYuvBufModeSet();   // 设置YUV缓冲模式，用于快速捕获使用4个第一路径帧缓冲，Mantis 45534

			appVideoPreSizeSet(puiPara->VideoSize);   // 设置视频预览大小

			#if (VIDEO_NEW_ZOOM == 1)
			sp1kVideoZoomCtrlParaSet(SP1K_VIDEO_ZOOM_CTRL_PARA_CURRENT_LOG, stillCurrLog);   // 设置视频缩放参数
			sp1kVideoRecZoomFacSet(videoZFctor, 0);
			#else
			video_zoomInFlag = 1;   // 设置视频缩放标志为1
			video_zoomFactor = stillZFctor;   // 设置视频缩放因子
			ZoomCurLog = stillCurrLog;   // 设置当前日志
			//sp1kPvZoomSet(0, video_zoomFactor, 0); // hlc, Mantis: 0021118
			//sp1kVideoRecZoomFacSet(video_zoomFactor, 0);
			#endif

			sp1kVideoRecFrontRateSet(0x00);   // 设置前置摄像头速率为0x00
			sp1kVideoRecParamSet(VD_REC_YUV_FORMAT, VD_REC_YUV_422_420);   // 设置视频录制YUV格式为VD_REC_YUV_422_420
			sp1kAeSetModeEn(AE_ENABLE);   // 启用自动曝光模式
			sp1kAwbEnable();   // 启用白平衡
		}

		appIconPsTabLoad(ICON_MODE_PV);   // 加载图标PS表格
		appStillOsdInit(1);   // 初始化静态OSD
		appVideoStampSet();   // 在osdinit之后移动
		puiPara->StampVideoMode = dataModeBak;   // 恢复数据模式
		puiPara->VideoSize = videoSizeBak;   // 恢复视频大小
		sp1kHalCtrlScreen(3);   // 控制屏幕打开
		HAL_FrontSignalWait(HAL_FRONT_WAIT_VSYNC, 1, 4);   // 等待前置信号同步
		ret = IGNORE_DEFAULT;   // 返回值设为IGNORE_DEFAULT
	}
	else
	{
		if (sp1kVideoRecParamGet(VD_CAPTURE))   // 如果视频捕获参数为1
		{
			sp1kVideoRecParamSet(VD_CAPTURE, 0);   // 设置视频捕获参数为0
			appPvForceUpdateOn(1);   // 强制更新Pv
		}
		ret = CALL_DEFAULT;   // 返回值设为CALL_DEFAULT
	}

	sp1kDispImgWinEn(1);   // 打开显示图像窗口
	return ret;   // 返回操作成功标识
}

/**
 * @fn		  void appStillViewZoomIn(void)
 * @brief		app StillView Zoom In flow
 * @param	NONE
 * @retval	NONE
 * @todo
 * @author	LinJieCheng
 * @since	2010-10-07
 */
#if (STILL_NEW_ZOOM == 0)
// hlc 0328, start
static UINT16 prvFact = 0;//jintao.liu 2008-4-4 add for 1x 4x
void appStillViewZoomIn(void) USING_0
{
	UINT16 newFact, PvZoomCtrlcurLogBack;
	UINT8 ZfStatus;

	PvZoomCtrlcurLogBack = PvZoomCtrl.curLog;

	PvZoomCtrl.curLog -= PvZoomCtrl.logStep;

	if (PvZoomCtrl.curLog < PvZoomCtrl.minLog) {
		PvZoomCtrl.curLog = PvZoomCtrl.minLog;
	}

	#if (STILL_ZOOM_FACTOR_LOOKUP_TABLE == 1)
	newFact = StillZoomFactor[(100-PvZoomCtrl.curLog)/2];
	#else
	newFact = (10000)/PvZoomCtrl.curLog;//zoom in 4X
	#endif

 	if(newFact <= PvZoomCtrl.MaxFct) {
		PvZoomCtrl.factor = newFact;
//		printf("fct:%d\n",PvZoomCtrl.factor );
	}
	else {
		PvZoomCtrl.curLog  = PvZoomCtrl.minLog;
		PvZoomCtrl.factor = PvZoomCtrl.MaxFct ;
		uiUpdateOSDPvZoom(400, 0);//mantis #27679 //zoom in 4X
		return;
	}

	ZfStatus = sp1kPvZoomSet(0, PvZoomCtrl.factor, 0);

	if (ZfStatus == FAIL) {
		PvZoomCtrl.curLog = PvZoomCtrlcurLogBack;
		return;
	}

	//jintao.liu 2008-4-4 add for 4x
	if(prvFact != PvZoomCtrl.factor)
	{
		prvFact = PvZoomCtrl.factor;
	}
	else
		return;
	//jintao.liu 2008-4-4 add for 4x end
	uiUpdateOSDPvZoom(PvZoomCtrl.factor, 0);
}
#endif

/**
 * @fn		  void appStillViewZoomIn(void)
 * @brief		app StillView Zoom In flow
 * @param	NONE
 * @retval	NONE
 * @todo
 * @author	LinJieCheng
 * @since	2010-10-07
 */
#if (STILL_NEW_ZOOM == 1)
void appStillViewZoomIn(void) USING_0
{
	if (sp1kPvTeleSet() == FAIL) {
		//note : max zoom factor
	}
}
#endif


/**
 * @fn		  void appStillViewZoomOut(void)
 * @brief		app StillView Zoom Out flow
 * @param	NONE
 * @retval	NONE
 * @todo
 * @author	LinJieCheng
 * @since	2010-10-07
 */
#if (STILL_NEW_ZOOM == 0)
void appStillViewZoomOut(void) USING_0
{
	UINT16 newFact, PvZoomCtrlcurLogBack;
	UINT8 ZfStatus;

	PvZoomCtrlcurLogBack = PvZoomCtrl.curLog;

	#if (STILL_ZOOM_FACTOR_LOOKUP_TABLE == 1)
	if(PvZoomCtrl.factor <= PvZoomCtrl.MinFct){
		return;
	}
	#endif

	PvZoomCtrl.curLog += PvZoomCtrl.logStep;

	if (PvZoomCtrl.curLog  > PvZoomCtrl.maxLog) {
		PvZoomCtrl.curLog  = PvZoomCtrl.maxLog;
	}

	#if (STILL_ZOOM_FACTOR_LOOKUP_TABLE == 1)
	newFact = StillZoomFactor[(100-PvZoomCtrl.curLog)/2];
	#else
	newFact = (10000)/PvZoomCtrl.curLog ;//zoom in 4X
	#endif

	if(newFact >= PvZoomCtrl.MinFct ) {
		PvZoomCtrl.factor = newFact;
	}
	else {
		PvZoomCtrl.curLog  = PvZoomCtrl.maxLog;
		PvZoomCtrl.factor = PvZoomCtrl.MinFct ;
		uiUpdateOSDPvZoom(100, 0);
		return;
	}
//	printf("fct:%d\n",PvZoomCtrl.factor );

	ZfStatus = sp1kPvZoomSet(0, PvZoomCtrl.factor, 0);

	if (ZfStatus == FAIL) {
		PvZoomCtrl.curLog = PvZoomCtrlcurLogBack;
		return;
	}

	//jintao.liu 2008-4-4 add for 1x
	if(prvFact != PvZoomCtrl.factor)
	{
		prvFact = PvZoomCtrl.factor;
	}
	else
		return;
	//jintao.liu 2008-4-4 add for 1x end
	uiUpdateOSDPvZoom(PvZoomCtrl.factor, 0);
}
#endif

/**
 * @fn		  void appStillViewZoomOut(void)
 * @brief		app StillView Zoom Out flow
 * @param	NONE
 * @retval	NONE
 * @todo
 * @author	LinJieCheng
 * @since	2010-10-07
 */
#if (STILL_NEW_ZOOM == 1)
void appStillViewZoomOut(void) USING_0
{
	if (sp1kPvWideSet() == FAIL) {
		//note : min zoom factor
	}
}
#endif

/**
 * @fn		  UINT8 appStillViewAEAWBReadyGet(UINT8 start)
 * @brief		NONE
 * @param	NONE
 * @retval	NONE
 * @todo
 * @author
 * @since
 */
// hlc 0328, end
#if 0
//suny add zoom function end
UINT8 appStillViewAEAWBReadyGet(UINT8 start)
{
#ifdef SNAP_WAIT_AEAWBREADY_OPTION
	return sp1kAeReadyGet(start );
#else
	return SUCCESS;
#endif
}
#endif

/**
* @param    None
* @retval   成功或失败的标志
* @brief    设置静态图像质量
**/
UINT8 appStillQualitySet(void)
{
	uiPara_t* uiPara; // UI参数指针
	UINT8 QTable; // Q表(图像压缩的技术)
	UINT8 saturation = 0; // 饱和度

	uiPara = appUiParaGet(); // 获取UI参数

	switch (uiPara->ImageSize)  // 图像大小
	{
		case PRV_SIZE_50M:
		case PRV_SIZE_44M:
		case PRV_SIZE_42M:
		case PRV_SIZE_36M:
			saturation = 0x00; // 设置饱和度
			break;
			
		case PRV_SIZE_24M:
		case PRV_SIZE_20M:
			saturation = 0x01;
			break;

		case PRV_SIZE_16M:
		case PRV_SIZE_12M:
		case PRV_SIZE_10M:
			saturation = 0x02;
			break;

		case PRV_SIZE_8M:
		case PRV_SIZE_5M:
			saturation = 0x10;
			break;

		case PRV_SIZE_40M:
		case PRV_SIZE_30M:
			saturation = 0x42;
			break;
			
		case PRV_SIZE_3M:
			saturation = 0x10;
			break;
		case PRV_SIZE_1M:
		case PRV_SIZE_1M_16_9:
			saturation = 0x20;
			break;
		case PRV_SIZE_VGA:
			saturation = 0x30;
			break ;
		case PRV_SIZE_9M:
		case PRV_SIZE_7M_16_9:
			saturation = 0x40;
			break ;
		default:
			saturation = 0x00;
			break;
	}

	switch (uiPara->ImageQuality)  // 图像质量
	{
		case PRV_QUAL_SUPER:
			saturation |= 0x00; // 设置饱和度
			break;
		case PRV_QUAL_FINE:
			saturation |= 0x01;
			break;
		case PRV_QUAL_NORMAL:
			saturation |= 0x02;
			break;
		default:
			saturation |= 0x00;
			break;
	}

	switch (saturation)  // 饱和度
	{
		case 0x00:
			QTable = 30; // 设置Q表
			break;
		case 0x01:
			QTable = 40;
			break;
		case 0x02:
			QTable = 50;
			break;
		case 0x10:
			QTable = 60;
			break;
		case 0x11:
			QTable = 80;
			break;
		case 0x12:
			QTable = 80;
			break;
		case 0x20:
			QTable = 80;
			break;
		case 0x21:
			QTable = 80;
			break;
		case 0x22:
			QTable = 80;
			break;
		case 0x30:
			QTable = 80;
			break;
		case 0x31:
			QTable = 80;
			break;
		case 0x32:
			QTable = 80;
			break;
		case 0x40:
			QTable = 80;
			break;
		case 0x41:
			QTable = 80;
			break;
		case 0x42:
			QTable = 70;
			break;
		default:
			QTable = 80;
			break;
	}

	// 为了文件大小添加 @20080423 wei.sun 开始
	if (sp1kSnapParamGet(SP1K_SNAP_PARAM_DOFLOW_OPT) && 
        (uiPara->DriverMode == PRV_SNAP_BURST))
	{
		QTable -= 5;
	}
	sp1kJpegQTableSet(QTable, 0, 0, 1); // 设置JPEG Q表

	return SUCCESS; // 返回成功标志
}

/**
* @param    resolution: 图像大小 @ref _PRV_SIZE
* @retval   None
* @brief    根据输入的分辨率设置前置摄像头的快照参数，包括裁剪尺寸和快照尺寸
**/
UINT8 appStillResolutionSet(UINT8 resolution)
{
	UINT16 frontHSize,frontVSize;   // 前置摄像头图像宽度和高度
	UINT16 cropVSize;   // 裁剪后图像高度
	UINT16 cropHsize;   // 裁剪后图像宽度

	sp1kFrontSnapSizeGet(0, &frontHSize, &frontVSize);  // 获取前置摄像头快照尺寸

	//printf("frontHSize=%d, frontVSize=%d\n",frontHSize,frontVSize);

    if (frontHSize * 3 > frontVSize * 4) // 如果前置摄像头图像宽高比大于4:3，则为16:9
    {
        cropHsize = frontVSize * 4 / 3; // 根据16:9比例计算裁剪后的宽度
        cropVSize = frontVSize; // 裁剪后的高度保持不变
    }
    else // 4:3
    {
        cropHsize = frontHSize; // 裁剪后的宽度保持不变
        cropVSize = frontVSize; // 裁剪后的高度保持不变
    }

    //printf("cropHsize=%d, cropVSize=%d\n",cropHsize,cropVSize);

	switch(resolution)  // 根据不同的分辨率设置快照参数
    {
		case PRV_SIZE_50M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);  // 为拍摄设置裁剪尺寸。不能大于传感器全尺寸
			sp1kSnapSizeSet( 8160, 6120 );  // 设置采集图像的水平/垂直尺寸
			S_imgRatioChg = 1;
			break;
		case PRV_SIZE_40M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 7200, 5400 );
			S_imgRatioChg = 1;
			break;
		case PRV_SIZE_30M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 6320, 4740 );
			S_imgRatioChg = 1;
			break;
		case PRV_SIZE_24M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 5600, 4200 );
			S_imgRatioChg = 1;
			break;
		case PRV_SIZE_20M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 5120, 3840 );
			S_imgRatioChg = 1;
			break;
		case PRV_SIZE_18M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 4896, 3672 );
			S_imgRatioChg = 1;
			break;

		case PRV_SIZE_16M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 4608, 3456 );
			S_imgRatioChg = 1;
			break;
		case PRV_SIZE_44M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 7664, 5748 );
			S_imgRatioChg = 1;
			break;
		case PRV_SIZE_12M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 3888, 2916 );
			S_imgRatioChg = 1;
			break;
		case PRV_SIZE_10M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 3888, 2916 );
			S_imgRatioChg = 1;
			break;
		case PRV_SIZE_9M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 3456, 2592 );
			S_imgRatioChg = 1;
			break;
		case PRV_SIZE_8M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 3264, 2448 );
			S_imgRatioChg = 0;
			break;
		case PRV_SIZE_7M_16_9:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 3648, 2048 );
			S_imgRatioChg = 1;
			break;
		case PRV_SIZE_5M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 2592, 1944 );
			S_imgRatioChg = 0;
			break;
		case PRV_SIZE_3M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 2048, 1536 );
			S_imgRatioChg = 0;
			break;
		case PRV_SIZE_2M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 1600, 1200 );
			S_imgRatioChg = 1;
			break;
		case PRV_SIZE_2M_16_9:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 1920, 1080 );//1920x1080
			S_imgRatioChg = 1;
			break;
		case PRV_SIZE_1M_16_9:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 1280, 720 );//1280x720
			S_imgRatioChg = 1;
			break;
		case PRV_SIZE_1M:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 1024, 768 );
			S_imgRatioChg = 0;
			break;
		case PRV_SIZE_VGA:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 640, 480 );
			S_imgRatioChg = 0;
			break;
		default:
			sp1kSnapCropSizeSet(cropHsize, cropVSize);
			sp1kSnapSizeSet( 2592, 1944 );
			S_imgRatioChg = 0;
			break;
	}
	return SUCCESS;
}

/**
* @param    None
* @retval   None
* @brief    根据当前的 UI 参数中的自动回顾时间设置快速预览参数
**/
void appStillQuickViewSet(void)
{
    // 获取当前的 UI 参数指针
    uiPara_t* puiEnviroment;

    puiEnviroment = appUiParaGet();
    // 根据自动回顾时间设置快速预览参数
    switch(puiEnviroment->AutoReviewTime)
    {
        // 若自动回顾关闭
        case QUICKVIEW_OFF:
            // 设置快速预览参数为关闭
            sp1kSnapParamSet(SP1K_SNAP_PARAM_QV, 0);
            // 设置快速预览周期为0，即不执行快速预览
            sp1kSnapParamSet(SP1K_SNAP_PARAM_QV_PERIOD, 0);
            break;
        // 若自动回顾时间为1秒
        case QUICKVIEW_1S:
            // 设置快速预览参数为开启
            sp1kSnapParamSet(SP1K_SNAP_PARAM_QV, 1);
            // 设置快速预览周期为1000毫秒（即1秒）
            sp1kSnapParamSet(SP1K_SNAP_PARAM_QV_PERIOD, 1000);
            break;
        // 若自动回顾时间为3秒
        case QUICKVIEW_3S:
            // 设置快速预览参数为开启
            sp1kSnapParamSet(SP1K_SNAP_PARAM_QV, 1);
            // 设置快速预览周期为3000毫秒（即3秒）
            sp1kSnapParamSet(SP1K_SNAP_PARAM_QV_PERIOD, 3000);
            break;
        // 默认情况下，开启快速预览，周期为1秒
        default:
            sp1kSnapParamSet(SP1K_SNAP_PARAM_QV, 1);
            sp1kSnapParamSet(SP1K_SNAP_PARAM_QV_PERIOD, 1000);
            break;
    }
}


#define ITEM_NUM			10  //8
#define DATA_LENGTH_MAX	20
#define EXIF_DEF 0
#define EXIF_CHG 1
void appExifParaInit(UINT8 chg)
{
	chg = chg;
#if 0 //hy
    struct IFD ifd[ITEM_NUM] = {
	{IFD0_TAG_IMGDESCRIPTION, 	IFD_DATATYPE_ASCII, 11, 0},
	{IFD0_TAG_MAKE,     IFD_DATATYPE_ASCII, 11, 0},
	{IFD0_TAG_MODEL,    IFD_DATATYPE_ASCII, 11, 0},
	//{IFD0_TAG_DATETIME, IFD_DATATYPE_ASCII, 20, 0},

	{EXIF_TAG_FNUMBER,      IFD_DATATYPE_RATIONAL, 1, 0},
	{EXIF_TAG_MAXAPERTURE,  IFD_DATATYPE_RATIONAL, 1, 0},
	{EXIF_TAG_FLASH, 		IFD_DATATYPE_SHORT,    1, 0x00000007},
	{EXIF_TAG_FOCALLENGTH,	IFD_DATATYPE_RATIONAL, 1, 0},
	{EXIF_TAG_WHITEBALANCE,	IFD_DATATYPE_SHORT,    1, 0x00000001},

	{EXIF_TAG_EXPOSURETIME,      IFD_DATATYPE_RATIONAL, 1, 0},
	{EXIF_TAG_ISOSPEEDRATINGS,      IFD_DATATYPE_SHORT, 1, 200},

    };
    UINT8 dataBuf[ITEM_NUM][DATA_LENGTH_MAX]={
	{"ICATCHTEK"},
	{"ICATCHTEK"},
	{"SPCA1628"},
	//{"2008:10:25 12:30:59"},

	{0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x0a},//big endian, 8/4
	{0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x0a},//big endian, 28/10
	{0},
	{0x00, 0x00, 0x00, 0x5D, 0x00, 0x00, 0x00, 0x0a},
	{0},

	{0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x0a},//big endian, 28/10
	{0},
    };
	UINT8 comment[]= " ";//Sunplus MM Inc.
	UINT8 ifdType;
	struct IFD *pIfd;
	UINT8* pval;
	UINT8 i=0;

	if(chg == EXIF_DEF)return;

	while(i<=ITEM_NUM){
		pIfd = &ifd[i];
		pval = dataBuf[i];
		if(i >= 3)
			ifdType = IFD_TYPE_EXIFIFD;
		else
			ifdType = IFD_TYPE_ZEROIFD;

		if(pIfd->tag == EXIF_TAG_FLASH || pIfd->tag == EXIF_TAG_WHITEBALANCE || pIfd->tag == EXIF_TAG_ISOSPEEDRATINGS)
			pval = NULL;

		if(i == ITEM_NUM){
			sp1kEXIFJPGCommentSet(comment,sizeof(comment));
		}else{
			sp1kEXIFTagValueSet(ifdType, pIfd->tag, pIfd->type, pIfd->count, pIfd->valueoffset,pval, 1); //big endian :1
		}
		i++;
	}
#endif
}

void appSnapParaInit(void)
{
	uiPara_t* snapPara;
	appExifParaInit(EXIF_DEF);
	snapPara = appUiParaGet();


	//sp1kSnapSizeSet( 2592, 1944 );//suny mark 20083.13
	appStillResolutionSet(snapPara->ImageSize);

	//sp1kSnapDzoomFactorSet( PvZoomCtrl.factor, PvZoomCtrl.factor );
	//sp1kSnapParamSet( SP1K_SNAP_PARAM_QUALITY, SP1K_SNAP_QUALITY_SUPER_FINE );//suny mark 20083.13

	sp1kSnapParamSet( SP1K_SNAP_PARAM_FORMAT, SP1K_SNAP_FORMAT_422 );
	//sp1kSnapParamSet( SP1K_SNAP_PARAM_BURST, 0 );//suny mark 20083.13
	//sp1kSnapParamSet( SP1K_SNAP_PARAM_QV, 1 );//suny mark 20083.13
	if(snapPara->DriverMode == PRV_SNAP_BURST) {
		sp1kSnapParamSet( SP1K_SNAP_PARAM_BURST, 1 );//suny mark 20083.13
	} else {
		sp1kSnapParamSet( SP1K_SNAP_PARAM_BURST, 0 );//suny mark 20083.13
	}

	appStillQuickViewSet();
	sp1kSnapParamSet( SP1K_SNAP_PARAM_PRE_BRC, 1 );
	sp1kSnapParamSet( SP1K_SNAP_PARAM_POST_BRC, 0 );
	sp1kSnapParamSet( SP1K_SNAP_PARAM_SHUTTER_SOUND_OPT,((~(snapPara->ShutterSound))&0x01));



	//sp1kSnapDzoomFactorSet( 100, 100 );

	sp1kSnapParamSet( SP1K_SNAP_PARAM_SAVE_RAW_OPT, 0 );
	sp1kSnapParamSet( SP1K_SNAP_PARAM_SAVE_YUV_OPT, 0 );
	sp1kSnapParamSet( SP1K_SNAP_PARAM_SAVE_JPG_OPT, 1 );

	sp1kSnapParamSet(SP1K_SNAP_PARAM_DZOOM_OPT, 1 );
	sp1kSnapParamSet(SP1K_SNAP_PARAM_CHANGERAW, 0 );

	sp1kSnapParamSet( SP1K_SNAP_PARAM_BURST, 0 );//burst mode

	sp1kSnapParamSet( SP1K_SNAP_PARAM_QV, 0 );//AutoReview mode

	sp1kSnapParamSet( SP1K_SNAP_PARAM_BYPASS_QV_THB_OPT, 0 );//no AutoReview, Thumbnail, QuickView mode

	sp1kSnapParamSet( SP1K_SNAP_PARAM_DOFLOW_OPT, 0 );//high speed burst mode
	sp1kSnapParamSet( SP1K_SNAP_PARAM_DOFLOW_STORE_FIRST_OPT, 0 );//high speed burst mode store first

	sp1kSnapParamSet( SP1K_SNAP_PARAM_STAMP_X_RATIO, 50 );//Adjust datestamp x offset
	sp1kSnapParamSet( SP1K_SNAP_PARAM_STAMP_Y_RATIO, 58 );//Adjust datestamp y offset

	sp1kSnapParamSet ( SP1K_SNAP_PARAM_STAMP_DT_DEFAULT_LOCATION_OPT, 1 );//use default date,time stamp location
}


/**
* @param    None
* @retval   None
* @brief    设置电子频率
**/
UINT8 appElecFreqSet(void)
{
	uiPara_t* uiPara; // 指向 UI 参数结构体的指针

	uiPara = appUiParaGet(); // 获取 UI 参数

	// 根据光源频率设置帧率参数和预览模式
	switch(uiPara->LightFreq)
	{
		case PRV_FREQ_50:
			sp1kAeParamSet(SP1K_AE_FRAME_RATE, SP1K_FRAME_RATE_25); // 设置帧率为 25
			/* 根据传感器输出的尺寸设置预览模式 */
			appCalBpPvModeSet(PV_MODE_HD);
			break;
		case PRV_FREQ_60:
			sp1kAeParamSet(SP1K_AE_FRAME_RATE, SP1K_FRAME_RATE_30); // 设置帧率为 30
			/* 根据传感器输出的尺寸设置预览模式 */
			appCalBpPvModeSet(PV_MODE_HD);
			break;
		default:
			sp1kAeParamSet(SP1K_AE_FRAME_RATE, SP1K_FRAME_RATE_25); // 默认设置帧率为 25
			/* 根据传感器输出的尺寸设置预览模式 */
			appCalBpPvModeSet(PV_MODE_HD);
			break;
	}
	return SUCCESS; // 返回成功状态
}


/* ------ Note: Demo code zone start flag, don't modify or delete this line!!! ------ */

#if (FUNC_HOST_PF_PV)

UINT8 appPvPhotoFrameSet(UINT8 idx)
{
	UINT16 resW, resH;
	sp1kPvPhotofrmEnable(0);
	if (idx == PRV_PHOTO_FRAME_OFF)
	{
		sp1kSnapParamSet( SP1K_SNAP_PARAM_PHOTO_FRM_OPT, 0 );	//xian ++ 20081216
		return FAIL;
	}

	#if (STILL_NEW_ZOOM == 0)
	//add for mantis #36993@start
	PvZoomCtrl.curLog=100;
	PvZoomCtrl.factor=100;
	sp1kPvZoomSet(1, PvZoomCtrl.factor, 0);	// TQ@20100702
	uiUpdateOSDPvZoom(PvZoomCtrl.factor, 0);
	#else
	sp1kPvZoomCtrlParaSet(SP1K_PV_ZOOM_CTRL_PARA_CURRENT_LOG, 0);
	sp1kPvZoomSet(1, sp1kPvZoomFctGet(), 0);	// TQ@20100702
	uiUpdateOSDPvZoom(sp1kPvZoomFctGet(), 0);
	#endif
	if (appPvPhotoFrameResLoad(idx, &resW, &resH) != SUCCESS)
	{
		ASSERT(0,0);
		return FAIL;
	}
    //xian ++ 20081216
	sp1kSnapParamSet(SP1K_SNAP_PARAM_PH_WIDTH, resW);
	sp1kSnapParamSet(SP1K_SNAP_PARAM_PH_HEIGHT, resH);
	sp1kSnapParamSet( SP1K_SNAP_PARAM_PHOTO_FRM_OPT, 1 );
	//xian --
	sp1kPvDlyPhofrmBlendCntSet(1);
	sp1kPvPhotofrmEnable(1);
	// menuPromptDisp(MENU_PROMPT_OK_ON | MENU_PROMPT_MENU_ON);
	return SUCCESS;
}

#endif

//-----------------------------------------------------------------------------
//appPbPhotoFrameSet
//-----------------------------------------------------------------------------
/**
 * @brief	photo frame for playback (only for view)
 * @param	void
 * @retval	success
 * @see
 * @author	jintao.liu
 * @since	2008-06-16
 * @todo
 * @bug
*/
#if (FUNC_HOST_PF_PB)
UINT8 appPbPhotoFrameSet(UINT8 idx, UINT8 bSave)
{
	UINT16 ResId;
	UINT32 ResAddr = K_SDRAM_ImagePlaybackBufAddr;
	UINT8 OsdMaxX, OsdMaxY;
	UINT8 ret;
	UINT16 width = 0, height = 0;
	sp1kOsdLayoutAttrGet(&OsdMaxX, &OsdMaxY);

	if (idx == PRV_PHOTO_FRAME_OFF)
	{
		RESUME_DISPBUFF();
		return FAIL;
	}

	switch (idx)
	{
		case PRV_PHOTO_FRAME_A:
			ResId = RES_PHOTO_FRAME_A;
			break;
		case PRV_PHOTO_FRAME_B:
			ResId = RES_PHOTO_FRAME_B;
			break;
		case PRV_PHOTO_FRAME_C:
			ResId = RES_PHOTO_FRAME_C;
			break;
		case PRV_PHOTO_FRAME_D:
			ResId = RES_PHOTO_FRAME_D;
			break;
		default:
			ResId = RES_PHOTO_FRAME_A;
			break;
	}
	if(bSave)
	{
		sp1kOsdClear(0);
		appUiBusyWait(250,0);
		ret = pbPhotoFrameSave( DOS_FileIndexGet( pbFile.dwIndex ),ResId);
		pbFile.dwIndex=sp1kPbFileCntGet();
		appUiBusyExit();
	}
	else
	{
		RESUME_DISPBUFF();//resume image before process
		//printf("resAddr = %lx; endAddr = %lx\n", ResAddr,K_SDRAM_TotalSize-256UL);
		ret = sp1kdecodePhotoFrameImage(ResId, &ResAddr, K_SDRAM_TotalSize-256UL, &width, &height );
		if(SUCCESS == ret)
		{
			sp1kHalGprmCpy(ResAddr, width, height, 0, 0,
			sp1kPbDispBuffGet(1),  PB_DECODE_WIDTH, PB_DECODE_HEIGHT, 0, 0, PB_DECODE_WIDTH, PB_DECODE_HEIGHT, 0x9F, 1);//9f
		}

		menuPromptDisp(MENU_PROMPT_OK_ON | MENU_PROMPT_MENU_ON);
	}
	return SUCCESS;
}
#endif

/* ------ Note: Demo code zone end flag, don't modify or delete this line!!! ------ */

/**
* @param    None
* @retval   返回值表示等待的状态，SUCCESS 表示成功，FAIL 表示失败
* @brief    等待自动曝光和白平衡完成
**/
UINT8 appStillViewAEAWBWait(void)
{
	//static UINT8 AEAWB_ReadyFlg = 0;
	//UINT8 sts = FAIL;

	if ( pvWaitCnt++ > PV_3A_WAIT_CNT_MAX ) {
		/* ae/awb waiting timeout */
		pvWaitCnt = 0;
		return SUCCESS;
	}

	#ifdef SNAP_WAIT_AEAWBREADY_OPTION
	if(sp1kAeReadyGet(4/*3 4 7*/)){
		osMsgPost(SP1K_MSG_3A_WAIT_PROC);
		return FAIL;
	}
	#endif
	
	return SUCCESS;
	
	#if 0
	if (AEAWB_ReadyFlg == 0) {
		sts = appStillViewAEAWBReadyGet(0);
		if(sts == SUCCESS) {
			AEAWB_ReadyFlg = 1;// - AEAWB_ReadyFlg;
		}
		sts = FAIL;
	} else if (AEAWB_ReadyFlg == 1){
		sts = appStillViewAEAWBReadyGet(1);
		if(sts == SUCCESS) {
			AEAWB_ReadyFlg = 1 - AEAWB_ReadyFlg;
		}
	} else {
		AEAWB_ReadyFlg = 0; /* exception handle */
	}

	/* msg resend for waiting AE/AWB ready */
	if ( sts != SUCCESS ) {
		osMsgPost(SP1K_MSG_3A_WAIT_PROC);
	}
	return sts;
	#endif
}


/**
* @param    None
* @retval   None
* @brief    环境检查函数，用于检查相机环境是否符合拍摄条件
**/
UINT8 appSnapEnvChk(void) 
{
    UINT8 ret = FAIL;

    // 如果是视频快拍剪辑模式且快拍状态为运行中
    if (sp1kVideoCapParamGet(SP1K_VD_CAP_MOD) == SP1K_SPEEDCAP_CLIP) 
    {
        if (sp1kSpeedyCapStsGet() == SPEEDCAP_STS_RUNNING) 
        {
            // 获取快拍记录时间
            HAL_GlobalReadGTimer(&speedyCapRecTime);
            count = (speedyCapRecTime - count) / 1000;
            // 停止快拍
            sp1kSpeedyCaptureStop();
            // 发送视频快拍完成消息
            osMsgPost(SP1K_MSG_MEDIA_VIDO_CAP_FINISH);
            return FAIL;
        }
    }

    /* add for tv play mode */
    // 用于TV播放模式的添加
#if (TV_OPTION == TV_PLAY_OPTION)
    // 如果面板类型为0
    if (appPanelTypeGet() == 0) {
        return FAIL;
    }
#endif

    /* check card status */
    // 检查存储卡状态
#if (!NAND_SUPPORT)//2008-7-14 mantis #28129
    // 如果SD卡未插入
    if (0 == appSDPlugStatusGet()) {
        // 显示无卡提示对话框
        uiOsdDialogDisp(ID_STR_NO_CARD_, 1000);
        // 初始化静态图像拍摄界面
        appStillViewInit(SP1K_STILL_VIEW_INIT_SMART);
        return FAIL;
    }
#endif
    // 如果SD卡被锁定
    if (1 == appSDProtectStatusGet()) //jintao.liu 2008-5-6 for sd lock
    {
        // 显示存储卡已锁定提示对话框
        uiOsdDialogDisp(ID_STR_CARD_LOCKED_, 1000);
        // 初始化静态图像拍摄界面
        appStillViewInit(SP1K_STILL_VIEW_INIT_SMART);
        return ret;
    }

/***************Just for QA test @start***************************************/
    // 仅用于QA测试的部分
    if (AutoTestMode == 0x55) 
    {
        // 如果剩余照片数小于10
        if (RemainPictureNO < 10) 
        {
            // 发送状态到静态播放消息
            osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY);
            return FAIL;
        }
    }

/***************Just for QA test @end***************************************/

    // 如果存储卡错误或格式错误
    if (appCardErrFmtErrChk()) 
    {
        // 显示存储卡错误提示对话框
        uiOsdDialogDisp(ID_STR_CARD_ERROR, 1000);
        // 初始化静态图像拍摄界面
        appStillViewInit(SP1K_STILL_VIEW_INIT_SMART);
        return FAIL;
    } 
    else 
    {
        // 检查剩余拍摄张数
        if (RemainPictureNO == 0) 
        {
            // 更新OSD显示内存已满提示
            uiUpdateOSDMemFull();
            isUpdateNum = 1;
            // 初始化静态图像OSD
            appStillOsdInit(1);
            // 初始化静态图像拍摄界面
            appStillViewInit(SP1K_STILL_VIEW_INIT_SMART);
            /* yp add from*/
            // 控制LCD关闭
            #ifdef _ENABLE_LCD_OFF_
            appOsdModeSw(0);
            #endif /*_ENABLE_LCD_OFF_*/
            /*yp add end */
            return FAIL;
        }
    }

    // 检查单位状态
    if (pvZoomSts != 0) 
    {
        /* the unit is still in zoom status */
        //printf("pvzmsts=%bx\n",pvZoomSts);
        return FAIL;
    }

    // 返回成功
    ret = SUCCESS;
    return ret;
}


UINT8 appSnapParaLoad(void)
{
	uiPara_t* snapPara;
	UINT8 ret = FAIL;
    UINT32 phAddr, resAddr = K_SDRAM_ImagePlaybackBufAddr;
    UINT16 phW, phH;

	snapPara = appUiParaGet();
	sp1kSnapParamSet( SP1K_SNAP_PARAM_SHUTTER_SOUND_OPT,((~(snapPara->ShutterSound))&0x01));
	if(snapPara->Volume ==0){
		sp1kSnapParamSet( SP1K_SNAP_PARAM_SHUTTER_SOUND_OPT,0);
	}
	appVoicePlay(SP1K_MSG_UNKNOWN_HOST, VOICE_TYPE_SHUTTER, VOICE_PLAYLEVLE_HIGH);

	appStillQuickViewSet();
	//xian ++ 20081216
	if(sp1kSnapParamGet(SP1K_SNAP_PARAM_PHOTO_FRM_OPT)){
        phW = sp1kSnapParamGet(SP1K_SNAP_PARAM_PH_WIDTH);
        phH = sp1kSnapParamGet(SP1K_SNAP_PARAM_PH_HEIGHT);
        phAddr = (UINT32)sp1kSnapParamGet(SP1K_SNAP_PARAM_PH_LOW_ADDR) +
            ((UINT32)sp1kSnapParamGet(SP1K_SNAP_PARAM_PH_HIGH_ADDR)<<16);
        if(phAddr >= resAddr && phAddr <= resAddr + (UINT32)phW*phH*2){
           // printf("ph addr over!");
            return FAIL;
        }

        sp1kHalGprmCpy(resAddr, phW, phH, 0, 0, phAddr, phW, phH, 0, 0, phW, phH, 0, 1);
    }
    
	//xian --

    //xian -- 20081216, move out for self snap
	//if(appDriveModeSet() !=SUCCESS) {
	//	return FAIL;
	//}
    //xian --

	ret = SUCCESS;
	return ret;
}

UINT8 appSnapPreProc(void)
{
	uiPara_t* snapPara = appUiParaGet();

	/*disable other key in burst mode*/
	if(snapPara->DriverMode == PRV_SNAP_BURST){
		sp1kBtnCfg(KEY_TYPE_ADC_A, 0xffff, 0);
		sp1kBtnCfg(KEY_TYPE_ADC_B, 0xffff, 0);
		sp1kBtnCfg(KEY_TYPE_GPIO, 0xffff, 0x02);
	} else {
		sp1kBtnDisableAll();
	}
	
	return SUCCESS;
}

UINT8 appSnapPostProc(UINT16 msg)
{
	UINT8 ret = SUCCESS;

	if ( msg == SP1K_SNAP_MSG_FILE_WRITE_ERROR ) {
		uiUpdateOSDMemFull();
		isUpdateNum=1;
		appStillViewInit(SP1K_STILL_VIEW_INIT_SMART);
		ret = FAIL;
	}
	else {
		if ( UI_LCD_ON_WITH_OSD == appGetOsdMode() ) {
			// UI update remain pix count
			//uiRemainPixShow();
			//uiUpdateRemainPix();
		}
	}
#ifdef _ENABLE_LCD_OFF_
	appOsdModeSw(2);//reload
#endif /*_ENABLE_LCD_OFF_*/

	/*disable other key in burst mode*/
	sp1kBtnEnableAll();

	/* power control -- preview */
	//appModePowerCtrl(MODE_PREVIEW);

	return ret;
}

UINT8 appSnapBurstCheck( UINT8 burstCnt )
{
	UINT8 burstChk = osMsgSeek( SP1K_MSG_KEY_RELEASE_S2 );

	UINT8 sts;
	UINT8 abort = TRUE;

	sts = sp1kBtnChk(SP1K_BTN_S2);

	#if 0 //suny mark for capture 3 images per press snap key in burst mode
		if( burstChk == TRUE || sts != KEY_STATUS_PRESSED ) {
			abort = FALSE;
		}
	#else
		if( burstCnt >= (UINT8)3 ) {
			abort = FALSE;
		}
	#endif

	return abort;
}

/* ------ Note: Demo code zone start flag, don't modify or delete this line!!! ------ */
//xian ++ 20081216

#if FUNC_HOST_PF_PV
void appPvPhotoFrameRecover(void)
{
	UINT16 phW, phH;
	uiPara_t* pUiPara;
	pUiPara = appUiParaGet();

	if(sp1kSnapParamGet(SP1K_SNAP_PARAM_DOFLOW_OPT) &&(pUiPara ->DriverMode == PRV_SNAP_BURST) )
	{
		sp1kPvPhotofrmEnable(0);
		return;//burst don't support photoframe
	}

	if(!sp1kSnapParamGet(SP1K_SNAP_PARAM_PHOTO_FRM_OPT))
	{
		return;
	}
	if (pUiPara->PhotoFrame != PRV_PHOTO_FRAME_OFF)
	{
		if (appPvPhotoFrameResLoad(pUiPara->PhotoFrame, &phW, &phH) != SUCCESS)
		{
			ASSERT(0,0);
			return;
		}

		//printf("load ph h=%d,v=%d\n",phW,phH);

		//xian ++ 20081216
		sp1kSnapParamSet(SP1K_SNAP_PARAM_PH_WIDTH, phW);
		sp1kSnapParamSet(SP1K_SNAP_PARAM_PH_HEIGHT, phH);
		sp1kSnapParamSet( SP1K_SNAP_PARAM_PHOTO_FRM_OPT, 1 );
		//xian --
		sp1kPvPhotofrmEnable(1);
	}
}

UINT8 appPvPhotoFrameResLoad(UINT8 idx, UINT16* resW, UINT16* resH)
{
	UINT32 ResId;
	UINT32 SrcAddr = K_SDRAM_PvPFJpgAddr;
	UINT32 ResAddr = K_SDRAM_PvPFYUVAddr;
//	struct Exif_Decompress_Parameter MainImgPara = {0};//xl
	ExifImage_t MainImgPara;//xl
	switch (idx)
	{
		case PRV_PHOTO_FRAME_A:
			ResId = RES_PHOTO_FRAME_A;
			break;
		case PRV_PHOTO_FRAME_B:
			ResId = RES_PHOTO_FRAME_B;
			break;
		case PRV_PHOTO_FRAME_C:
			ResId = RES_PHOTO_FRAME_C;
			break;
		case PRV_PHOTO_FRAME_D:
			ResId = RES_PHOTO_FRAME_D;
			break;
		case PRV_PHOTO_FRAME_5:
		case PRV_PHOTO_FRAME_6:
		case PRV_PHOTO_FRAME_7:
		case PRV_PHOTO_FRAME_8:
		case PRV_PHOTO_FRAME_9:
		case PRV_PHOTO_FRAME_10:
		case PRV_PHOTO_FRAME_11:
		case PRV_PHOTO_FRAME_12:
		case PRV_PHOTO_FRAME_13:
		case PRV_PHOTO_FRAME_14:
		case PRV_PHOTO_FRAME_15:
		case PRV_PHOTO_FRAME_16:
		case PRV_PHOTO_FRAME_17:
		case PRV_PHOTO_FRAME_18:
		case PRV_PHOTO_FRAME_19:
		case PRV_PHOTO_FRAME_20:
		case PRV_PHOTO_FRAME_21:
			ResId = 0x101 + (idx-PRV_PHOTO_FRAME_5);
			break;
	}

	/* photo frame big image decoder */
	if (appBGImgDec(ResId, &MainImgPara, SrcAddr, ResAddr, 1)) {
		sp1kDramFileSave("PHFM0001.JPG",K_SDRAM_PvPFJpgAddr,K_SDRAM_PvPFJpgSize);
		sp1kDramFileSave("PHFM0001.YUV",K_SDRAM_PvPFYUVAddr,K_SDRAM_PvPFYUVSize*2);
		ASSERT(0,1);
		return FAIL;
	}

/*	*resW = MainImgPara.Width;
	*resH = MainImgPara.Height;
*/	//xl
	*resW = MainImgPara.width;
	*resH = MainImgPara.height;

	sp1kSnapPhotoFrameParaSet(ResId,SrcAddr);	/* Past parameter to Snap_task.c */

	return SUCCESS;
}

#endif

static UINT8 pvUpdate = 0;
/**
* @param    on: Flag_true/Flag_false
* @retval   None
* @brief    设置强制更新Pv(置位/清除)
**/
void appPvForceUpdateOn(UINT8 on)
{
	pvUpdate= on;
}

/**
* @param    on: Flag_true/Flag_false
* @retval   None
* @brief    获取强制更新Pv状态
**/
UINT8 appPvForceUpdateGet(void)
{
	return pvUpdate;
}

/**
 * @brief		appISOSet
 * @param	ISOValue
 * @retval	SUCCESS
 * @see
 * @author	Li tuhong
 * @since		2011-03-09
 * @todo		Something need to be done.
 * @bug		a bug need to be resolved.
*/

UINT8 appISOSet(UINT8 ISOValue)
{
	sp1kAeIsoSet(ISOValue);
	return SUCCESS;
}

/**
 * @brief		appAntiShakeSet
 * @param	antiShake
 * @retval	NULL
 * @see
 * @author	Li tuhong
 * @since		2011-03-09
 * @todo		Something need to be done.
 * @bug		a bug need to be resolved.
*/

void appAntiShakeSet(UINT8 antiShake)
{
	switch(antiShake){
		case PRV_ANTISHAKE_ON:
			//printf("AntiShake on\n");
			sp1kAeAntiShakeFpsSet(20);
			break;
		case PRV_ANTISHAKE_OFF:
		default:
			//printf("AntiShake off\n");
			sp1kAeAntiShakeFpsSet(0);
			break;
	}
}


/**
* @param    None
* @retval   None
* @brief    设置自定义印章
**/
void appCustomizeStampSet(void)
{
	//return;
	/* set background function enable */
	// 设置背景功能使能
	sp1kStampBackgroundEnable(1);

	/* set customer string function enable */
	// 设置自定义字符串功能使能
	//sp1kStampTextEnable(STAMP_TEXT,1);

	/* set customer string */
	// 设置自定义字符串
	//sp1kStampTextModifyByString(STAMP_TEXT, "123");

	/* set background h,v size */
	// 设置背景水平和垂直尺寸
	sp1kSnapParamSet( SP1K_SNAP_PARAM_BACKGROUND_WIDTH, 640/*640*/ );
	sp1kSnapParamSet( SP1K_SNAP_PARAM_BACKGROUND_HEIGHT, 32/*60*/ );
}

/**
 * @brief		appSnapBackgroundResLoad
 * @param	dstAddrWord : load background destination buffer address
 *                bufByteSize : buffer size
 * @retval	FAIL, SUCCESS
 * @see
 * @author	LinJieCheng
 * @since		2011-03-09
 * @todo		Something need to be done.
 * @bug		a bug need to be resolved.
*/
UINT8 appSnapBackgroundResLoad(UINT32 dstAddrWord, UINT32 bufByteSize)
{
	UINT32 resId, bgSize;

	resId = 0x2d;

	sp1kNandRsvSizeGet(resId, NULL, &bgSize); // Res_0x2.res = calLen.bin

	if (bgSize > bufByteSize) {
		return FAIL;
	}

	if (!sp1kNandRsvRead(resId, (dstAddrWord << 1))) {
		return FAIL;
	}

	return SUCCESS;
}

/**
 * @brief		appSnapHostStamp
 * @param	NULL
 * @retval	FAIL, SUCCESS
 * @see      Fast Burst Capture + Host Draw Background Function -> libSnap need turn on SNAP_DOFLOW_BURST_CAPTURE_BG_STAMP_UPDATE Option
 * @author	LinJieCheng
 * @since		2011-03-09
 * @todo		Something need to be done.
 * @bug		a bug need to be resolved.
*/
UINT8 capNum = 0;
UINT8 appSnapHostStamp(void)
{
	#if 0//Fast Burst Capture + Host Draw Background Function Example Code
	UINT32 w,h,add;
	//printf("capNum=%bu\n",capNum);
	sp1kStampBackgroundGet(&add,&w,&h);
	switch ( capNum ) {
		case 0:
			sp1kStampTextModifyByString(STAMP_TEXT, "1/3  116F");
			sp1kStampTextSet(STAMP_TEXT,(w>>2)+48,(h-(h>>1))>>1,0xba);
			capNum++;
			break;
		case 1:
			sp1kStampTextModifyByString(STAMP_TEXT, "2/3  109F");
			sp1kStampTextSet(STAMP_TEXT,(w>>2)+48,(h-(h>>1))>>1,0xba);
			capNum++;
			break;
		case 2:
			sp1kStampTextModifyByString(STAMP_TEXT, "3/3  099F");
			sp1kStampTextSet(STAMP_TEXT,(w>>2)+48,(h-(h>>1))>>1,0xba);
			capNum = 0;
			break;
		default:
			ASSERT(0,1);
	}
	#endif

	return SUCCESS;
}

/**
 * @brief		appUserDefineMeter
 * @param	w : hsize, h : vsize
 * @retval	user meter point
 * @see
 * @author	LinJieCheng
 * @since	2011-07-28
 * @todo		Something need to be done.
 * @bug		a bug need to be resolved.
*/
UINT8* appUserDefineMeter(UINT8 w, UINT8 h)
{
	if (sizeof(userDefineMeter) >= (w*h)) {
		return &userDefineMeter[0];
	}
	else {
		return NULL;
	}
}

/**
 * @brief		appUserMeterModeSet
 * @param	en :
 *\n							  enable : 1
 *\n							   disable : 0
 * @retval	NONE
 * @see
 * @author	LinJieCheng
 * @since	2011-07-28
 * @todo		Something need to be done.
 * @bug		a bug need to be resolved.
*/
void appUserMeterModeSet(UINT8 en){
	userMeterEn = en;
}

/**
 * @brief		appUserMeterModeGet
 * @param	NONE
 * @retval	userMeterEn : user meter
 *\n                              enable : 1
 *\n                              disable : 0
 * @see
 * @author	LinJieCheng
 * @since	2011-07-28
 * @todo		Something need to be done.
 * @bug		a bug need to be resolved.
*/
UINT8 appUserMeterModeGet(void){
	return userMeterEn;
}

/**
 * @brief		appDynamicProgrammingUserMeterFlow
 * @param	NONE
 * @retval	NONE
 * @see
 * @author	LinJieCheng
 * @since	2011-07-28
 * @todo		Something need to be done.
 * @bug		a bug need to be resolved.
*/
void appDynamicProgrammingUserMeterFlow(void)
{
	UINT8 cnt, h, v;

	sp1kAeWindowDimensionGet(&h, &v);

	if ((h*v == sizeof(aeWindowInformation)) && (h*v == sizeof(userDefineMeter))) {

		sp1kAeWindowArrayGet(&aeWindowInformation[0]);

		for(cnt = 0; cnt < sizeof(aeWindowInformation); cnt++) {
			if (aeWindowInformation[cnt] > 200) {
				if (userDefineMeter[cnt] <= 48) {
					userDefineMeter[cnt]++;
				}
			}
			else {
				if (userDefineMeter[cnt] != 1) {
					userDefineMeter[cnt]--;
				}
			}
		}

		sp1kAeDynamicMeterChangeSet();
	}
}

#if (FUNC_HOST_AF_FIXTURE == 1)
UINT8 appAfWinSet(UINT8 En)
{
	UINT16 frontHSize,frontVSize;
	UINT16 cropHSize,cropVSize;

	sp1kFrontPreviewSizeGet(0, &frontHSize, &frontVSize);
	sp1kPreviewCropSizeGet(&cropHSize,&cropVSize);

	sp1kAfWinCfg(frontHSize,frontVSize,cropHSize,cropVSize);

	sp1kAfWinEnSet(En);

	return SUCCESS;
}
#endif

/**
 * @brief       appStillSmoothZoomMsgProcFlow
 * @param       NONE
 * @retval      NONE
 * @todo
 * @author      SunYong
 * @since       2007-12-18
 */
#if (STILL_NEW_ZOOM == 1)
UINT8 appStillSmoothZoomMsgProcFlow(UINT16 sysMsg) {
    uiPara_t* pUiPara;

    switch (sysMsg) {
        // 处理不同的系统消息
        case 1:
        case SP1K_MSG_CARD_PLUG_IN:
        case SP1K_MSG_CARD_PLUG_OUT:
        case SP1K_MSG_USB_PLUG_IN:
        case SP1K_MSG_TV_PLUG_IN:
        case SP1K_MSG_TV_PLUG_OUT:
        case SP1K_MSG_HDMI_PLUG_IN:
        case SP1K_MSG_CARD_ERROR:
        case SP1K_MSG_CARD_NOT_SUPPORT:
        case SP1K_MSG_NAND_ERROR:
        case SP1K_MSG_BAT_EMPTY:
            // 如果实时预览平滑缩放状态为启用
            if (sp1kPvSmoothZoomGet() == 1) {
                // 关闭实时预览平滑缩放
                sp1kPvSmoothZoomSet(0);
                // 设置实时预览缩放后处理
                sp1kPvZoom3APostSet();
                // 启用自动曝光模式
                sp1kAeSetModeEn(AE_ENABLE);
                // 启用自动白平衡
                sp1kAwbEnable();
            }
            // 等待实时预览平滑缩放状态完成
            while (sp1kPvSmoothZoomStatusGet() == FAIL);
            // 获取UI参数
            pUiPara = appUiParaGet();
            // 如果速拍功能开启或速拍录像功能开启
            if (pUiPara->SpeedyCap == SPEEDY_CAPTURE_ON || pUiPara->SpeedyCapRec == SPEEDY_CAPTURE_REC_ON) {
                // 获取视频缩放因子
                stillZFctor = sp1kVideoZoomFctGet();
                // 获取当前记录
                stillCurrLog = sp1kVideoZoomCtrlParaGet(SP1K_VIDEO_ZOOM_CTRL_PARA_CURRENT_LOG);
            } else {
                // 获取实时预览缩放因子
                stillZFctor = sp1kPvZoomFctGet();
                // 获取当前记录
                stillCurrLog = sp1kPvZoomCtrlParaGet(SP1K_PV_ZOOM_CTRL_PARA_CURRENT_LOG);
            }
            break;
        // 处理按键S2按下的情况
        case SP1K_MSG_KEY_PRESS_S2:
            // 如果实时预览平滑缩放状态为失败
            if (sp1kPvSmoothZoomStatusGet() == FAIL) {
                return FAIL;
            }
            break;
    }

    return SUCCESS;
}
#endif


