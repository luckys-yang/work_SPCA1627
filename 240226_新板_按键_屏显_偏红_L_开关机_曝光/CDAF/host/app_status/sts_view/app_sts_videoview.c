/**************************************************************************
 *         Copyright(c) 2007 by Sunplus mMedia  Technology Co., Ltd.       *
 *                                                                         *
 *  This software is copyrighted by and is the property of Sunplus mMedia  *
 *  Technology Co., Ltd. All rights are reserved by Sunplus mMedia Techno- *
 *  logy Co., Ltd. This software may only be used in accordance with the   *
 *  corresponding license agreement. Any unauthorized use, duplication,    *
 *  distribution, or disclosure of this software is expressly forbidden.   *
 *                                                                         *
 *  This Copyright notice "M U S T" not be removed or modified without     *
 *  prior written consent of Sunplus mMedia Technology  Co., Ltd.          *
 *                                                                         *
 *  Sunplus mMedia Technology Co., Ltd. reserves the right to modify this  *
 *  software without notice.                                               *
 *                                                                         *
 *  Sunplus mMedia Technology Co., Ltd.                                    *
 *  19-1, Innovation First Road, Science-Based Industrial Park,            *
 *  Hsin-Chu, Taiwan, R.O.C.                                               *
 *                                                                         *
 **************************************************************************/
#include "general.h"
#include "solution.h"
#include "app_main.h"
#include "os_msg.h"
#include "app_msg.h"
#include "initio.h"
#include "app_video_view.h"
#include "app_menu.h"
#include "app_menu_micro.h"
#include "app_osd_icon.h"
#include "app_menu_api.h"
#include "App_ui_para.h"
#include "app_ui_osd.h"
#include "app_dev_storage.h"
#include "app_dev_usb.h"
#include "app_dev_disp.h"
#include "app_pwr_mgr.h"
#include "app_battery.h"
#include "app_sts_pwroff.h"
#include "app_still_view.h"
#include "app_menu_tab.h"
#include "sp1k_aud_api.h"
#include "sp1k_pv_api.h"
#include "sp1k_disp_api.h"
#include "sp1k_ae_api.h"
#include "sp1k_awb_api.h"//wangjg++
#include "sp1k_video_api.h"
#include "sp1k_hal_api.h"
#include "sp1k_util_api.h"
#include "sp1k_vfs_api.h"
#include "sp1k_osd_api.h"
#include "key_scan.h"
#include "app_menu_core.h"
#include "cardlink.h"
#include "dpf_scan.h"
#include "smcfunc.h"
//#include "dos32.h"
#include "sp1k_dos_api.h"
#include "app_init.h"
#include "hal_dram.h"

#include "sp1k_disk_api.h"

#include "sp1k_pv_api.h"
#include "sp1k_front_api.h"
#include "macro_expand.h"
#include "dbg_def.h"
#include "sp1k_calibration_api.h"
#include "hal_front.h"
#include "sp1k_cdsp_api.h"
#include "app_ui_osd.h"
#include "func_ptr.h"
#include "sp1k_fd_api.h"
#include "sp1k_dist_api.h"
#include "app_dev_disp.h"
#include "app_menu_event.h"
#include "app_still_play.h"
#include "app_video_view.h"
#include "dcf_api.h"
#include "app_still_play.h"
#include "sp1k_pb_api.h"
#include "sp1k_stamp_api.h"
#include "sp1k_rsvblk_api.h"
#include "rsc_id.h"
#include "sp1k_video_capture.h"
#include "video_capture.h"
#include "sp1k_gpio_api.h"

/*-------------------- USER CODE: Include Begin --------------------*/
#include "User_Config.h"
/*-------------------- USER CODE: Include End --------------------*/


/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/
#define __FILE	  __FILE_ID_APP_STS_VIDEOVIEW__

#define AWB_SPEED 2					//wangjg - invalid
#define AWB_SPEED_FAST_ONCE 255	//wangjg - invalid
#define HD_PVSIZE_STSCHG_EN	0
#define APP_PCM_FMT		0
#define APP_ADPCM_FMT	1
#define APP_AUDIO_FMT    APP_PCM_FMT

#if FUNC_HOST_MD
#define MD_TD_FUNC_EN		0
#else
#define MD_TD_FUNC_EN		0//always disable
#endif

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

/**************************************************************************
 *						   G L O B A L	  D A T A						  *
 **************************************************************************/
 #define VIDEO_ZOOM_FACTOR_LOOKUP_TABLE	1 //Digital zoom used look up table switch

#if (VIDEO_ZOOM_FACTOR_LOOKUP_TABLE == 1)
#define VIDEO_ZOOM_MAX	   400// zoom 8X
#define VIDEO_ZOOM_MIN	   100
#define VIDEO_ZOOM_MAX_LOG   100// zoom 8X
#define VIDEO_ZOOM_MIN_LOG   40//25
#define VIDEO_ZOOM_LOG_STEP 4//4//3*2 // hlc 0328 improve zoom quality
#else
#define VIDEO_ZOOM_MAX	   400// zoom 8X
#define VIDEO_ZOOM_MIN	   100
#define VIDEO_ZOOM_MAX_LOG   100// zoom 8X
#define VIDEO_ZOOM_MIN_LOG   40
#define VIDEO_ZOOM_LOG_STEP 5//4//3*2 // hlc 0328 improve zoom quality
#endif

/* move to video_zoom.c for v2.1.8 compatibility*/
//#if (VIDEO_NEW_ZOOM == 0)
//UINT16 xdata video_zoomFactor = 100;
//UINT8 xdata video_zoomInFlag = 0;
//UINT8 xdata video_zoomOutFlag = 0;
//#endif
UINT8 xdata preVideoFrameRate = MOV_FRM_LOW;    // 帧率
UINT8 xdata preVideoSize = MOV_SIZE_VGA;    // 视频格式
UINT8 flag_menu_back_videoview = 0;
#if (VIDEO_NEW_ZOOM == 0)
//UINT16 xdata  zoomStep=0;
UINT16 ZoomCurLog = 100; // zoom 4X
#endif
#if (VIDEO_NEW_ZOOM == 0)
UINT16 ZoomCurLogBack = 100; // zoom 4X Back up
UINT16 video_zoomFactorBack = 100;// Back up
UINT8 video_zoomInFlagBack = 0;//Back up
UINT8 video_zoomOutFlagBack = 0;//Back up
#endif

UINT8 key_enter_seamless_flag = 0;

UINT16 videoZFctor=100;
#if (VIDEO_NEW_ZOOM == 1)
UINT16 videoCurrLog=0;
UINT8 G_aeAwbStatus = 0;
#else
UINT16 videoCurrLog=100;
#endif
xdata UINT8 QtableDefault[10];//={50,54,57,60,64,67,70,74,77,80};
xdata UINT8 QtableHD[10];//={20,28,35,40,45,50,55,60,65,70};
xdata UINT8 Qtable1080P[10];//={15,20,25,30,35,40,45,50,55,60};
extern UINT8 AutoTestMode;
//extern UINT8 seamLessLpRecEn;
//extern UINT32 videoSeamlessFilesize;
//extern appVidClipCtrl_t xdata vidClipCtrl;
//extern UINT8 g_vidAviFrameRate;
UINT8 afkeyflag = 0;

/*
	半按拍照键时触发AF计时
	1000毫秒以后如果没有全按拍照键则进行对焦
	否则不执行对焦
*/
UINT8 video_af_timer_start = 0;
UINT8 video_af_timer_out = 0;


#if (VIDEO_NEW_ZOOM == 0)
#if (VIDEO_ZOOM_FACTOR_LOOKUP_TABLE == 1)
//power (0.03333++, 1.6) curve
code UINT16 VideoZoomFactor[31] = {
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
#else
code UINT16 VideoZoomFactor[1]={0};
#endif
#endif

#if SUPPORT_LOCK_FILE
xdata UINT8  file_lock_flag;
#endif


/**************************************************************************
 *				   E X T E R N A L	  R E F E R E N C E S				  *
 **************************************************************************/
//extern void appStillViewZoomFactorSet(UINT16 value);
extern xdata UINT8 AdapterCharge; //  1: USB contect power adapter charge	0:USB contect PC
extern UINT8 appElecFreqSet(void);
extern xdata UINT8 frameRateIsL;/* 0:60    1:30 */
extern UINT16 pvWaitCnt;
extern UINT8 xdata storageReadyflag;
extern xdata UINT16 G_UIStatus;
//UINT8 EventPress_Flag = 0; //test code

extern UINT8
frontAeTest(
	UINT8 id,
	UINT8 userIdx
);

#if LIFE_SPAN_TEST
extern UINT8 vidClipRdy;
#endif
UINT8 appStillViewAEAWBWait(void);

extern UINT16 LCD_WIDTH;// ((UINT16)320)
extern UINT16 LCD_HEIGHT;// ((UINT16)240)
extern UINT16 MENU_CHAR_HEIGHT;

#if SUPPORT_AUTO_TEST
extern UINT8 autoTestOpenFlag;
// 0 : 按键和 U 盘测试
// 1 : 视频录制测试
// 2 : 视频播放测试
// 3 : 自动测试结束
UINT8 autoTestStatus = 0; 
// bit
// 0:up, 1:down, 2:ok, 3:pwr, 4:usb, 5:pb, 6:set
// UINT8 button_status_byte = 0xff;
UINT8 upKeyCheck = 0, downKeyCheck = 0, okKeyCheck = 0, pwrKeyCheck = 0, UsbCheck = 0, pbKeyCheck = 0, setKeyCheck;
#endif


extern UINT8 key_repeate_check_flag;

#if SUPPORT_AWB_ADJ
extern UINT8 awb_auto_disable_flag;
#endif

/* Public function prototypes=========================================================*/

void appVideoStampSet(void);
UINT8 appVideoQualitySet(void);
UINT8 appElecFreqSet(void);
UINT8 appVideoFrmRateSet(void);
void appVideoViewInit(UINT8 initMode);


/**
* @param    msg: 消息枚举类型 ( @ref appMsgList_t @ref btnMsgList_t )
* @retval   None
* @brief    录像视图
**/
void appVideoView (UINT16 msg) USING_0
{
    //	powerBatLv_t batlv;
    static UINT8 zoomRptCnt = 0; // xian ++ 20091023 for hd zoom
    uiPara_t *puiPara;
    UINT32 time = 0;
    //	UINT16 FileIdx;
    //	UINT8 sldeletefinename[12];
    //	UINT16 idx;
    static UINT8 auto_rec_flag = 1;

    UINT32 clusterSize = vfsClusterSizeGet();
#if (VIDEO_NEW_ZOOM == 0)
    UINT8 ZfStatus;
#endif

    puiPara = appUiParaGet();   // 【获取UI参数指针】

    switch (msg)
    {
    case SP1K_MSG_STATE_INIT: // 【初始化消息】
    {
        appEffectSet(PRV_EFFECT_NORNAL);     // 输入的效果参数设置相应的摄像头效果
        appVideoQualitySet();                // 选择快照质量
        appAutoOffTimeSet();                 // 设置自动关机时间
        appElecFreqSet();                    // 设置电子频率
        appVideoFrmRateSet();                // 设置视频帧率
        sp1kVideoRecAudSampleRateSet(11025); // 设置音频采样率
        // sp1kYuvBufModeSet();    // 根据系统内存大小和视频录制状态设置 YUV 缓冲区的模式
        appVideoViewInit(SP1K_VIDEO_VIEW_INIT_FULL); // 设置视频分辨率的应用程序应用程序接口
        appVideoStampSet();                          // 设置视频时间戳

#if MSG_AUTO_SEND
        frontAeTest(0, 0);
        sp1kHalWait(2000);
        osMsgPost(SP1K_MSG_KEY_PRESS_S2);
#endif

// 【自动测试功能】
#if SUPPORT_AUTO_TEST
        if (!autoTestOpenFlag)
        {
            if (auto_rec_flag)
            {
                auto_rec_flag = Flag_false;
                // osMsgPost(SP1K_MSG_KEY_RELEASE_SNAP);   // 快照按键释放消息
            }
        }
#else
        if (auto_rec_flag)
        {
            auto_rec_flag = Flag_false;
            // osMsgPost(SP1K_MSG_KEY_RELEASE_SNAP);   // 快照按键释放消息
        }
#endif

// 【自动测试功能】
#if SUPPORT_AUTO_TEST
        if (autoTestOpenFlag)
        {
            if (0 == autoTestStatus) // 按键和 U 盘测试
            {
                appButtonAutoTest();
            }
        }
#endif
        break;
    }

    case sp1k_MSG_REC_SEAMLESS_CLOSEFILE: // 【连续录制关闭文件消息】
    {
        if (file_lock_flag) // 如果文件锁定标志位已设置
        {
            int index = dcfFileIndexGet(); // 获取DCF文件索引
            if (index >= 2)
            {
                index -= 1; // 如果索引大于等于2，将索引减1
            }
            else
            {
                index = 1; // 否则将索引设置为1
            }
            dcfFileReadOnlyAttrSet(index, 0xff, ATTR_R);  // 设置DCF文件只读属性
            file_lock_flag = 0;                           // 清除文件锁定标志位
            sp1kOsdIconShow(ID_ICON_PROTECT, ICON_CLEAR); // 清除保护图标显示
        }
        break;
    }

    case SP1K_MSG_KEY_PRESS_UP: // 【上按键按下消息】
    {
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

// 【支持运动检测功能】
#if FUNC_HOST_MD
    case SP1K_MSG_MD_ACTION:
#if (MD_TD_FUNC_EN == 1)
        //linjiecheng fixed for alog. test flow
        //printf("MD_MSG\n");
        sp1kMDLearningStart(1);
#else
        if(sp1kVideoRecStatusGet() == VID_REC_IDLE)
        {
            osMsgPost(SP1K_MSG_KEY_PRESS_S2);
            appMotionDetectTimeSet();
        }
        else
        {
            if(sp1kvideoRecTimeGet() < 1500)
            {
                break;
            }
            sp1kMDLearningStart(1);
            appMotionDetectTimeSet();
        }
#endif
        break;
#endif

        /*
        case SP1K_MSG_KEY_REPEATE_AF:
        //case SP1K_MSG_KEY_REPEATE_SNAP:
            if (!key_repeate_check_flag) {
                key_repeate_check_flag = 1;
                if(sp1kVideoRecStatusGet() != VID_REC_IDLE)
                {
                    //appVideoSnap();
                }
                //osMsgPost(SP1K_MSG_STATE_TO_STILL_VIEW);
            }
            break;

        case SP1K_MSG_KEY_PRESS_AF:
        //case SP1K_MSG_KEY_PRESS_SNAP:
            key_repeate_check_flag = 0;
            break;
        */

    case SP1K_MSG_KEY_RELEASE_SNAP: // 【快照按键释放消息】
    {
        if (video_af_timer_start && !video_af_timer_out)
        {
            break;
        }
    }
    case SP1K_MSG_KEY_PRESS_AF: // 【自动对焦按键按下消息】
    {
        video_af_timer_out = 0;
        video_af_timer_start = 1;
#if SUPPORT_AUTO_TEST
        if (autoTestOpenFlag)
        {
            okKeyCheck = 1;
            appButtonAutoTest();
            osMsgPost(SP1K_MSG_STATE_TO_VIDEO_VIEW);
            break;
        }
#endif

        /*
        if(sp1kVideoRecStatusGet() == VID_REC_IDLE)
        {
            if(key_enter_seamless_flag){
                key_enter_seamless_flag = 0;
                puiPara->Seamless = !puiPara->Seamless;
                appVideoSeamlessTimeSet();
                uiCycleIcon();
                break;
            }
        }
        */
    }
    case SP1K_MSG_KEY_PRESS_S2: // 【S2按键按下消息】
    {
#if SUPPORT_AUTO_TEST
        if (autoTestOpenFlag)
        {
            if (1 == autoTestStatus)
            {
                appButtonAutoTest();
            }
        }
#endif

        if (appVideoSmoothZoomMsgProcFlow(msg) == FAIL)
        {
            return;
        }
        sp1kBtnDisableAll();
        pvWaitCnt = 0;
        /***************Just for QA test @start***************************************/
        if (AutoTestMode)
        {
            sp1kFdDisable();
            sp1kDistDisable();
        }
        /***************Just for QA test @end***************************************/
        if (sp1kVideoRecStatusGet() == VID_REC_IDLE)
        {
            uiUpdateVideoRecRemainTime(&time);

            // if(seamLessLpRecEn==0){
            if (0 == appSDPlugStatusGet())
            {
                uiOsdDialogDisp(ID_STR_NO_CARD_, 1000);
#if SUPPORT_RTC_OSD_UPDATE
                appCurrentTimeShow(1);
#endif
                if (key_repeate_check_flag)
                {
                    key_repeate_check_flag = 0;
                    break;
                }
                break;
            }
            else if (time == 0)
            {
                osMsgPost(SP1K_MSG_MEDIA_REC_DISK_FULL);
                sp1kBtnEnableAll();
                break;
            }
            //}
            // else{
            if (FAIL == sp1kVideoSLRecMemCheck())
            {
                osMsgPost(sp1k_MSG_REC_SEAMLESS_DISK_FULL);
                sp1kBtnEnableAll();
                break;
            }
#if 0
            vidClipCtrl.freeCluster = DOS_FreeDiskSpaceByCluster();
            vidClipCtrl.usedCluster = 1 + videoSeamlessFilesize / (UINT32)clusterSize;
            if(time == 0)
            {
                while(vidClipCtrl.usedCluster > vidClipCtrl.freeCluster)
                {
                    FileIdx = sp1kPbFileIndexByOrderGet(1);
                    dcfAviNameGet(sldeletefinename, FileIdx);
                    vfsFileDelete(sldeletefinename);
                    dcfFileDramFdbParameterClear(K_DCF_AVI_TYPE, FileIdx);
                    sp1kPbFileOrderReset(1);
                    sp1kPbLastIndexClear();
                    vidClipCtrl.freeCluster = DOS_FreeDiskSpaceByCluster();
                }
#endif
            //}
            //}
#if (MD_TD_FUNC_EN == 0)
            if (puiPara->AdvanceDenoise == REC_ADVANCEDENOISE_ON)
            {
                sp1kCdspPvDynamicTDEn(1, 0);
            }
#endif
            osMsgPost(SP1K_MSG_3A_WAIT_PROC);
        }
        else
        {
#if (MD_TD_FUNC_EN == 0)
            if (puiPara->AdvanceDenoise == REC_ADVANCEDENOISE_ON)
            {
                sp1kCdspPvDynamicTDEn(0, 0);
            }
#endif
            osMsgPost(SP1K_MSG_MEDIA_REC_STOP);
        }
        if (key_repeate_check_flag)
        {
            key_repeate_check_flag = 0;
            break;
        }
        break;
    }
    case SP1K_MSG_3A_WAIT_PROC:
    {
        if (appStillViewAEAWBWait() != SUCCESS)
        {
            break;
        }
    }
    case SP1K_MSG_MEDIA_REC_STOP:   // 【录制停止消息】
    {

        sp1kBtnEnableAll();

#if (TV_OPTION == TV_PLAY_OPTION)
        if (appPanelTypeGet() == 0)
        {
            break;
        }
#endif

#ifdef _ENABLE_LCD_OFF_
        appOsdModeSw(0);
#endif /*_ENABLE_LCD_OFF_*/

#if (!NAND_SUPPORT) // 2008-7-14 mantis #28129
        if (0 == appSDPlugStatusGet())
        {
            uiOsdDialogDisp(ID_STR_NO_CARD_, 1000);
            appVideoOsdInit();
            break;
        }
#endif

        if (1 == appSDProtectStatusGet()) // jintao.liu 2008-5-6 for sd lock
        {
            uiOsdDialogDisp(ID_STR_CARD_LOCKED_, 1000);
            appVideoOsdInit();
            break;
        }

        // wangjg - invalid
        // sp1kAwbScaleSet(SP1K_AWB_Jump_Scale, AWB_SPEED_FAST_ONCE);// force awb jump to target
        appVideoSnap();

        if (sp1kVideoRecStatusGet() != VID_REC_IDLE)
        {
            appAutoOffEn(0);
        }
        break;
    }

#if 0
        case SP1K_MSG_KEY_PRESS_AF:
            //key_enter_seamless_flag = 0;
            //if(sp1kVideoRecStatusGet() != VID_REC_IDLE){
#if (SUPPORT_AF && SUPPORT_FIXED_AF)
            AF_Switch();
#endif
            //}
            break;

        case SP1K_MSG_KEY_PRESS_AF:
            key_enter_seamless_flag = 0;
            //if(sp1kVideoRecStatusGet() != VID_REC_IDLE){
#if (SUPPORT_AF && SUPPORT_FIXED_AF)
            AF_Switch();
#endif
            //}
            break;
#elif 0
        //case SP1K_MSG_KEY_PRESS_AF:
        case SP1K_MSG_KEY_PRESS_MENU:
            //key_enter_seamless_flag = 0;
#if SUPPORT_AUTO_TEST
            if (autoTestOpenFlag)
            {
                setKeyCheck = 1;
                appButtonAutoTest();
                break;
            }
#endif

            if(sp1kVideoRecStatusGet() != VID_REC_IDLE)
            {
                //appVideoSnap();
                break;
            }

            if(sp1kVideoRecStatusGet() == VID_REC_IDLE)
            {
                //appVideoSnap();

                puiPara->FlashLed = 0;
                appSnapFlashLed(0);
                uiUpdateFlashLed(LCD_WIDTH, 0, 0);

                osMsgPost(SP1K_MSG_STATE_TO_MENU);
                preVideoFrameRate = puiPara->FrameRate;
            }
            //printf("menu\n");
            break;
#endif

        //case SP1K_MSG_KEY_PRESS_MENU:
        //	key_enter_seamless_flag = 0;
        //	break;

        case SP1K_MSG_KEY_PRESS_MODE:   // 【模式按键按下消息】
        {
            // key_enter_seamless_flag = 0;
            if (sp1kVideoRecStatusGet() != VID_REC_IDLE)
            {
                // appVideoSnap();
                break;
            }
            flag_menu_back_videoview = 1;

#if SUPPORT_VIDEO_EV
            puiPara->AEMode = PRV_COMP_0EV;
            appEVSet(puiPara->AEMode);
#endif

            puiPara->FlashLed = 0;
            appSnapFlashLed(0);
            uiUpdateFlashLed(LCD_WIDTH, 0, 0);

            osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY);
            // osMsgPost(SP1K_MSG_STATE_TO_MENU);
            // osMsgPost(SP1K_MSG_STATE_TO_STILL_VIEW);
            break;
        }

            // case SP1K_MSG_KEY_PRESS_DOWN:
            // key_enter_seamless_flag = 0;
            // break;

        case SP1K_MSG_KEY_PRESS_LEFT:   // 【左按键按下消息】
            break;

        case SP1K_MSG_KEY_PRESS_RIGHT:  // 【右按键按下消息】
        {
            if (sp1kVideoRecStatusGet() == VID_REC_IDLE)
            {
                if (!sp1kSD_CdGet() && sp1kPbFileCntGet() != 0)
                {
                    osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY);
                    osMsgPost(SP1K_MSG_SYS_QUICK_DELETE);
                }
            }
            break;
        }

        case SP1K_MSG_KEY_PRESS_F1: // 【F1按键按下消息】
            break;

#if (VIDEO_NEW_ZOOM == 1)
        case SP1K_MSG_DZOOM_OSD_UPDATE:
            uiUpdateOSDPvZoom(sp1kVideoZoomFctGet(), 1);
            videoZFctor = sp1kVideoZoomFctGet();
            videoCurrLog = sp1kVideoZoomCtrlParaGet(SP1K_VIDEO_ZOOM_CTRL_PARA_CURRENT_LOG);
            break;
        case SP1K_MSG_DZOOM_3A_UPDATE:
            G_aeAwbStatus = 1;
            sp1kVideoZoom3APostSet();
            sp1kAeSetModeEn(AE_ENABLE);
            sp1kAwbEnable();
            break;
#endif

        case SP1K_MSG_KEY_PRESS_ZOOM_IN:    // 【放大按键按下消息】
        case SP1K_MSG_KEY_PRESS_TELE:   // 【变焦放大按键按下消息】
        {
            // key_enter_seamless_flag = 0;
            zoomRptCnt = 0;

#ifdef _ENABLE_LCD_OFF_
            appOsdModeSw(0);
#endif /*_ENABLE_LCD_OFF_*/

            sp1kAeSetModeEn(AE_DISABLE); // hlc, stops 3A when zoom
            sp1kAwbDisable();
            // sp1kFdDisable();
            // sp1kDistDisable();
#if (VIDEO_NEW_ZOOM == 1)
            G_aeAwbStatus = 0;
            sp1kVideoZoom3APreSet();
#endif
            break;
        }

        case SP1K_MSG_KEY_REPEATE_ZOOM_IN:  // 【放大按键长按消息】
        case SP1K_MSG_KEY_REPEATE_TELE: // 【变焦放大按键长按消息】
        {
            // if(zoomRptCnt++ & 1){ //xian ++ 20091023
            //    break;
            //}

#if (VIDEO_NEW_ZOOM == 0)
            appVideoViewTele();
            if (sp1kVideoRecStatusGet() == VID_REC_IDLE)
            {
                ZfStatus = sp1kVideoRecZoomFacSet(video_zoomFactor, 0);
            }
            else
            {
                ZfStatus = sp1kVideoRecZoomFacSet(video_zoomFactor, 1);
            }

            if (ZfStatus == FAIL)
            {
                appVideoViewTeleReply();
            }

            uiUpdateOSDPvZoom(video_zoomFactor, 1);
#endif
#if (VIDEO_NEW_ZOOM == 1)
            if (G_aeAwbStatus == 1)
            {
                sp1kAeSetModeEn(AE_DISABLE); // hlc, stops 3A when zoom
                sp1kAwbDisable();
                sp1kVideoZoom3APreSet();
                G_aeAwbStatus = 0;
            }
            else
            {
                sp1kVideoSmoothZoomSet(1);
                appVideoViewTele();
            }
#endif
            break;
        }

        case SP1K_MSG_KEY_PRESS_ZOOM_OUT:   // 【缩小按键按下消息】
        case SP1K_MSG_KEY_PRESS_WIDE:   // 【变焦缩小按键按下消息】
        { 
            // key_enter_seamless_flag = 0;
            zoomRptCnt = 0;
#ifdef _ENABLE_LCD_OFF_
            appOsdModeSw(0);
#endif /*_ENABLE_LCD_OFF_*/
            sp1kAeSetModeEn(AE_DISABLE);
            sp1kAwbDisable();
            // sp1kFdDisable();
            // sp1kDistDisable();
#if (VIDEO_NEW_ZOOM == 1)
            G_aeAwbStatus = 0;
            sp1kVideoZoom3APreSet();
#endif
            break;
        }

        case SP1K_MSG_KEY_REPEATE_ZOOM_OUT: // 【缩小按键长按消息】
        case SP1K_MSG_KEY_REPEATE_WIDE:     // 【变焦缩小按键长按消息】
        {                                   // if(zoomRptCnt++ & 1){ //xian ++ 20091023
          //    break;
          // }
#if (VIDEO_NEW_ZOOM == 0)
            appVideoViewWide();
            if (sp1kVideoRecStatusGet() == VID_REC_IDLE)
            {
                ZfStatus = sp1kVideoRecZoomFacSet(video_zoomFactor, 0);
            }
            else
            {
                ZfStatus = sp1kVideoRecZoomFacSet(video_zoomFactor, 1);
            }

            if (ZfStatus == FAIL)
            {
                appVideoViewWideReply();
            }

            uiUpdateOSDPvZoom(video_zoomFactor, 1);
#endif
#if (VIDEO_NEW_ZOOM == 1)
            if (G_aeAwbStatus == 1)
            {
                sp1kAeSetModeEn(AE_DISABLE); // hlc, stops 3A when zoom
                sp1kAwbDisable();
                sp1kVideoZoom3APreSet();
                G_aeAwbStatus = 0;
            }
            else
            {
                sp1kVideoSmoothZoomSet(1);
                appVideoViewWide();
            }
#endif
            break;
        }

        case SP1K_MSG_KEY_RELEASE_ZOOM_IN:
        case SP1K_MSG_KEY_RELEASE_TELE:
            zoomRptCnt = 0;


#if (VIDEO_NEW_ZOOM == 0)
            appVideoViewTele();
            if(sp1kVideoRecStatusGet() == VID_REC_IDLE)
            {
                sp1kVideoRecZoomFacSet(video_zoomFactor, 0);
            }
            else
            {
                sp1kVideoRecZoomFacSet(video_zoomFactor, 1);
            }


            uiUpdateOSDPvZoom(video_zoomFactor, 1);
#endif
#if (VIDEO_NEW_ZOOM == 1)
            appVideoViewTele();
            sp1kVideoSmoothZoomSet(0);
#endif
#if (VIDEO_NEW_ZOOM == 0)
            sp1kAeSetModeEn(AE_ENABLE);
            sp1kAwbEnable();
#endif
            if(puiPara->DisMode == DISMODE_ON)
            {
                sp1kDistEnable();
            }
            break;


        case SP1K_MSG_KEY_RELEASE_ZOOM_OUT:
        case SP1K_MSG_KEY_RELEASE_WIDE:
            zoomRptCnt = 0;
#if (VIDEO_NEW_ZOOM == 0)
            appVideoViewWide();
            if(sp1kVideoRecStatusGet() == VID_REC_IDLE)
            {
                sp1kVideoRecZoomFacSet(video_zoomFactor, 0);
            }
            else
            {
                sp1kVideoRecZoomFacSet(video_zoomFactor, 1);
            }
            uiUpdateOSDPvZoom(video_zoomFactor, 1);
#endif
#if (VIDEO_NEW_ZOOM == 1)
            appVideoViewWide();
            sp1kVideoSmoothZoomSet(0);
#endif
#if (VIDEO_NEW_ZOOM == 0)
            sp1kAeSetModeEn(AE_ENABLE);
            sp1kAwbEnable();
#endif
            if(puiPara->DisMode == DISMODE_ON)
            {
                sp1kDistEnable();
            }
            break;

        case SP1K_MSG_KEY_PRESS_DISP:
            //printf("disp\n");
            break;

        //case SP1K_MSG_KEY_PRESS_PB:
        //case SP1K_MSG_KEY_PRESS_DOWN:
        case SP1K_MSG_KEY_PRESS_MENU:
            //key_enter_seamless_flag = 0;

            if ( !sp1kSD_CdGet() && (sp1kPbFileCntGet() != 0) && (sp1kVideoRecStatusGet() == VID_REC_IDLE) )
            {
                osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY);
                osMsgPost(SP1K_MSG_SYS_QUICK_DELETE);
            }
            else if ( (sp1kVideoRecStatusGet() == VID_REC_IDLE) && (sp1kPbFileCntGet() == 0) )
            {
                uiOsdDialogDisp(ID_STR_NO_FILE, 0);
                sp1kHalWait(500);
                appVideoOsdInit();
                //osMsgPost(SP1K_MSG_STATE_TO_VIDEO_VIEW);
            }

#if 0
#if SUPPORT_AUTO_TEST
            if (autoTestOpenFlag)
            {
                pbKeyCheck = 1;
                appButtonAutoTest();
                break;
            }
#endif

            if(sp1kVideoRecStatusGet() != VID_REC_IDLE)
            {
                appVideoSnap();
            }

            if(sp1kVideoRecStatusGet() == VID_REC_IDLE)
            {
                //osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY);
                osMsgPost(SP1K_MSG_SYS_QUICK_DELETE);
                sp1kAwbDisable();
                sp1kFdDisable();
                sp1kDistDisable();
            }
#endif
            break;

        case SP1K_MSG_CARD_PLUG_IN:
            appCardErrFmtErrSet(0);
            appVideoSmoothZoomMsgProcFlow(msg);
            if(sp1kVideoRecStatusGet() != VID_REC_IDLE)
            {
                sp1kVideoRecStop();

#if 0//SUPPORT_PANEL_PROTECT
                puiPara->ProtTime = PANEL_TIME_OFF;
#endif

#if (MD_TD_FUNC_EN == 0)
                if(puiPara->AdvanceDenoise == REC_ADVANCEDENOISE_ON)
                {
                    sp1kCdspPvDynamicTDEn(0, 0);
                }
#endif
            }
            appStorageMount(K_MEDIA_SDF);
#if HD_PVSIZE_STSCHG_EN		//for enable videoview HD size switch between 6/8 and 8/8
            if(sp1kHDPvsizeReSet())
            {
                //printf("###SD _in\n");
                appVideoPreSizeSet(puiPara->VideoSize);
            }
#endif
            uiRemainPixInit();
            appVideoOsdInit();
            break;

        case SP1K_MSG_CARD_PLUG_OUT:
#ifdef _ENABLE_LCD_OFF_          //yp for mantis 43034
            appOsdModeSw(0);
#endif /*_ENABLE_LCD_OFF_*/

            appCardErrFmtErrSet(0);
            appVideoSmoothZoomMsgProcFlow(msg);

            if(sp1kVideoRecStatusGet() != VID_REC_IDLE)
            {
                sp1kVideoRecStop();

#if 0//SUPPORT_PANEL_PROTECT
                puiPara->ProtTime = PANEL_TIME_OFF;
#endif

#if (MD_TD_FUNC_EN == 0)
                if(puiPara->AdvanceDenoise == REC_ADVANCEDENOISE_ON)
                {
                    sp1kCdspPvDynamicTDEn(0, 0);
                }
#endif
            }

            sp1kBtnEnableAll();
            sp1kOsdSelWinSet(-1, -1, -1, -1, 0, -1);//jintao.liu 2008-6-13 mantis #25594

#ifdef DISK_DRV_NAND
            appStorageMount(K_MEDIA_NANDF);
#endif
            if((appSDPlugStatusGet() == 0) && (sp1kDiskTypeGet() == DEV_TYPE_DISK_NAND) && \
                    (puiPara->VideoSize == MOV_SIZE_HD || puiPara->VideoSize == MOV_SIZE_1080P || puiPara == MOV_SIZE_D1))
            {
                //xiaolin.zhu

                puiPara->VideoSize = MOV_SIZE_VGA;
                appVideoResSet(puiPara->VideoSize);
            }

            sp1kHDPvsizeStatusSet(0);
            appVideoPreSizeSet(puiPara->VideoSize);
            uiRemainPixInit();
            appVideoOsdInit();

            //add for mantis #36294@start
            if((appSDPlugStatusGet() == 0) && (sp1kDiskTypeGet() == DEV_TYPE_DISK_SPI) && (sp1kDosRamStsGet() == FALSE))
            {
                osMsgFlush(MSG_CTRL_FLUSH_ALL);
                uiOsdDialogDisp(ID_STR_NO_CARD_, 0);

#if SUPPORT_RTC_OSD_UPDATE
                appCurrentTimeShow(1);
#endif
            }
            //add for mantis #36294@end
            break;

        case SP1K_MSG_USB_PLUG_IN:
            if(AdapterCharge)
            {
                if (sp1kVideoRecStatusGet() == VID_REC_IDLE)
                {
                    osMsgPost(SP1K_MSG_KEY_PRESS_S2);
                }
                break;
            }

            if(sp1kVideoRecStatusGet() != VID_REC_IDLE)
            {
                //appVideoSnap();
                sp1kVideoRecStop();
#if (MD_TD_FUNC_EN == 0)
                if(puiPara->AdvanceDenoise == REC_ADVANCEDENOISE_ON)
                {
                    sp1kCdspPvDynamicTDEn(0, 0);
                }
#endif
                uiUpdateVideoStatus(0, sp1kVideoRecStatusGet());
            }

#if 0
            if (storageReadyflag == SUCCESS && appSDPlugStatusGet())
            {
                puiPara->USBMode = USB_MASS;

            }
            else
            {
                puiPara->USBMode = USB_PCCAM;
            }
#endif
            // osMsgPost(SP1K_MSG_STATE_TO_USB);
            osMsgPost(SP1K_MSG_STATE_TO_MENU);
            sp1kAwbDisable();
            sp1kFdDisable();
            sp1kDistDisable();
            break;

        case SP1K_MSG_USB_PLUG_OUT:
#if SUPPORT_AUTO_TEST
            if (autoTestOpenFlag)
            {
                UsbCheck = 1;
                appButtonAutoTest();
                break;
            }
#endif
            break;

            if(sp1kVideoRecStatusGet() != VID_REC_IDLE)
            {
                sp1kVideoRecStop();

#if 0//SUPPORT_PANEL_PROTECT
                puiPara->ProtTime = PANEL_TIME_OFF;
#endif

            }
            osMsgPost(SP1K_MSG_POWER_OFF);
            break;

        case SP1K_MSG_TV_PLUG_IN:
            appVideoSmoothZoomMsgProcFlow(msg);
#if(TV_OPTION == TV_PLAY_OPTION)
            osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY);
            osMsgPost(SP1K_MSG_TV_PLUG_IN);
#elif (TV_OPTION == TV_FULL_OPTION)
            if(sp1kVideoRecStatusGet() != VID_REC_IDLE)
            {
                //appVideoSnap();
                sp1kVideoRecStop();
#if (MD_TD_FUNC_EN == 0)
                if(puiPara->AdvanceDenoise == REC_ADVANCEDENOISE_ON)
                {
                    sp1kCdspPvDynamicTDEn(0, 0);
                }
#endif
                uiUpdateVideoStatus(0, sp1kVideoRecStatusGet());
            }
            if(sp1kVideoRecStatusGet() == VID_REC_IDLE)
            {
                appDispSet();
            }
#endif
            break;

        case SP1K_MSG_TV_PLUG_OUT:
            appVideoSmoothZoomMsgProcFlow(msg);
#if(TV_OPTION == TV_PLAY_OPTION)
            osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY);
            osMsgPost(SP1K_MSG_TV_PLUG_OUT);
#elif (TV_OPTION == TV_FULL_OPTION)
            if(sp1kVideoRecStatusGet() != VID_REC_IDLE)
            {
                //appVideoSnap();
                sp1kVideoRecStop();
#if (MD_TD_FUNC_EN == 0)
                if(puiPara->AdvanceDenoise == REC_ADVANCEDENOISE_ON)
                {
                    sp1kCdspPvDynamicTDEn(0, 0);
                }
#endif
                uiUpdateVideoStatus(0, sp1kVideoRecStatusGet());
            }
            if(sp1kVideoRecStatusGet() == VID_REC_IDLE)
            {
                appDispSet();
            }
#endif
            break;

        case SP1K_MSG_HDMI_PLUG_IN:
            appVideoSmoothZoomMsgProcFlow(msg);
            if(sp1kVideoRecStatusGet() != VID_REC_IDLE)
            {
                sp1kVideoRecStop();

#if 0//SUPPORT_PANEL_PROTECT
                puiPara->ProtTime = PANEL_TIME_OFF;
#endif

#if (MD_TD_FUNC_EN == 0)
                if(puiPara->AdvanceDenoise == REC_ADVANCEDENOISE_ON)
                {
                    sp1kCdspPvDynamicTDEn(0, 0);
                }
#endif
                uiUpdateVideoStatus(0, sp1kVideoRecStatusGet());
            }
            osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY);
            osMsgPost(SP1K_MSG_HDMI_PLUG_IN);
            //appDispHdmiOpen();
            break;

        case SP1K_MSG_HDMI_PLUG_OUT:
            appDispSet();
            break;

        case SP1K_MSG_POWER_OFF:
        case SP1K_MSG_KEY_REPEATE_POWER:
#if 0
            if(sp1kVideoRecStatusGet() != VID_REC_RUN)
            {
                if(sp1kVideoRecStatusGet() == VID_REC_PAUSE)
                {
                    appVideoSnap();
                }
                //if VID_REC_IDLE, no need to end snap process
                //20080327 suny add  start
#if(_HW_SOLUTION_ == _HW_TK_MODEL_B_)
                sp1kHalPanelOff();
#endif
                //20080327 suny add  end
                XBYTE[0x2030] &= ~0x02;
            }
            //if VID_REC_RUN, do nothing
#else
            if(sp1kVideoRecStatusGet() != VID_REC_IDLE)     //2008-7-9 mask for mantis #27979
            {
                sp1kVideoRecStop();

#if 0//SUPPORT_PANEL_PROTECT
                puiPara->ProtTime = PANEL_TIME_OFF;
#endif

            }

            if(sp1kVideoRecStatusGet() == VID_REC_IDLE)
            {
                appPowerOffProc();
            }
#endif
            break;

        case SP1K_MSG_MEDIA_REC_DISK_FULL:
            appVideoSmoothZoomMsgProcFlow(msg);
            /***************Just for QA test @start***************************************/
            if(AutoTestMode == 0xAA)
            {
                //uiPara_t* puiPara;
                //printf("atuoTestVideo\n");
                sp1kVideoRecStop();

#if 0//SUPPORT_PANEL_PROTECT
                puiPara->ProtTime = PANEL_TIME_OFF;
#endif

#if (MD_TD_FUNC_EN == 0)
                if(puiPara->AdvanceDenoise == REC_ADVANCEDENOISE_ON)
                {
                    sp1kCdspPvDynamicTDEn(0, 0);
                }
#endif

                uiUpdateVideoStatus(0, sp1kVideoRecStatusGet());
                osMsgFlush(MSG_CTRL_FLUSH_ALL);
                osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY);
                break;

                /*appFileDelete(SP1K_FILE_PROC_ALL);
                puiPara = appUiParaGet();
                puiPara->VideoSize++;
                if (MOV_SIZE_MAX== puiPara->VideoSize)
                {
                	puiPara->VideoSize = MOV_SIZE_HD;
                }
                osMsgPost(SP1K_MSG_STATE_INIT);
                osMsgPost(SP1K_MSG_KEY_PRESS_S2);*/

            }
            /***************Just for QA test @end***************************************/

            if(sp1kVideoRecStatusGet() != VID_REC_IDLE)
            {
                sp1kVideoRecStop();
#if (MD_TD_FUNC_EN == 0)
                if(puiPara->AdvanceDenoise == REC_ADVANCEDENOISE_ON)
                {
                    sp1kCdspPvDynamicTDEn(0, 0);
                }
#endif
                uiUpdateVideoStatus(0, sp1kVideoRecStatusGet());
                osMsgFlush(MSG_CTRL_FLUSH_ALL);
            }
            //show memory full
            uiUpdateOSDMemFull();

#if LIFE_SPAN_TEST
            vidClipRdy = 0;
#endif
            break;

        case SP1K_MSG_MEDIA_REC_FILE_MAX:
            appVideoSmoothZoomMsgProcFlow(msg);
            if(sp1kVideoRecStatusGet() != VID_REC_IDLE)
            {
                sp1kVideoRecStop();
#if (MD_TD_FUNC_EN == 0)
                if(puiPara->AdvanceDenoise == REC_ADVANCEDENOISE_ON)
                {
                    sp1kCdspPvDynamicTDEn(0, 0);
                }
#endif
                uiUpdateVideoStatus(0, sp1kVideoRecStatusGet());
                osMsgFlush(MSG_CTRL_FLUSH_ALL);
            }
            osMsgPost(SP1K_MSG_KEY_PRESS_S2);

            break;

        case sp1k_MSG_REC_SEAMLESS_DISK_FULL:
            appVideoSmoothZoomMsgProcFlow(msg);
            if(sp1kVideoRecStatusGet() != VID_REC_IDLE)
            {
                sp1kVideoRecStop();

#if 0//SUPPORT_PANEL_PROTECT
                puiPara->ProtTime = PANEL_TIME_OFF;
#endif

#if (MD_TD_FUNC_EN == 0)
                if(puiPara->AdvanceDenoise == REC_ADVANCEDENOISE_ON)
                {
                    sp1kCdspPvDynamicTDEn(0, 0);
                }
#endif
                uiUpdateVideoStatus(0, sp1kVideoRecStatusGet());
                osMsgFlush(MSG_CTRL_FLUSH_ALL);
            }
            uiUpdateOSDMemFull();
            break;

        /*******************************************************************/
        //dcfFileReadOnlyAttrSet(0x0010, 0xff,  ATTR_R);
        //dcfFileAttrSet(filename,filename, ATTR_R,ATTR_MODIFY_ATTR);
        /*******************************************************************/
        case SP1K_MSG_MEDIA_REC_OK:
            //update osds
#if LIFE_SPAN_TEST
            osMsgFlush(MSG_CTRL_FLUSH_BTN);
            osMsgPost(SP1K_MSG_KEY_PRESS_S2);
#endif
            if(sp1kVideoRecStatusGet() == VID_REC_IDLE) //mantis #41568
            {
                appVideoOsdInit();	//lryy
                appAutoOffEn(1);
                uiRemainPixInit();
            }

#if 0//SUPPORT_PANEL_PROTECT
            puiPara->ProtTime = PANEL_TIME_OFF;
            app_set_close_lcd_backlight_time();
#endif

#if SUPPORT_LOCK_FILE
            if(file_lock_flag)
            {
                int index = dcfFileIndexGet();
                if(index >= 2)
                    index -= 1;
                dcfFileReadOnlyAttrSet(index, 0xff, ATTR_R);
                file_lock_flag = 0;
                sp1kOsdIconShow(ID_ICON_PROTECT, ICON_CLEAR);
            }
#endif
            break;

        case SP1K_MSG_MEDIA_REC_FAIL:
            osMsgFlush(MSG_CTRL_FLUSH_ALL);
            osMsgPost(SP1K_MSG_MEDIA_REC_FILE_MAX);
            //osMsgPost(SP1K_MSG_MEDIA_REC_STOP);
            break;

        //jintao.liu 2008-3-12
        //add for battery detect
        case SP1K_MSG_BAT_LV_UPDATE://jintao.liu 2008-3-13 add for battery action
            uiUpdateBatteryLevel();
            appBattStsHandle();
            break;

        case SP1K_MSG_BAT_EMPTY:
            appVideoSmoothZoomMsgProcFlow(msg);
            if(sp1kVideoRecStatusGet() != VID_REC_IDLE) //2008-7-9 mantis #27979
            {
                sp1kVideoRecStop();

#if 0//SUPPORT_PANEL_PROTECT
                puiPara->ProtTime = PANEL_TIME_OFF;
#endif

#if (MD_TD_FUNC_EN == 0)
                if(puiPara->AdvanceDenoise == REC_ADVANCEDENOISE_ON)
                {
                    sp1kCdspPvDynamicTDEn(0, 0);
                }
#endif
                uiUpdateVideoStatus(0, sp1kVideoRecStatusGet());//2008-7-15 mantis #28057
            }

            uiUpdateOSDBatEmpty();
            osMsgPost(SP1K_MSG_POWER_OFF);
            sp1kBtnDisableAll();//ready to power off, disable all button!
            break;

        case SP1K_MSG_NAND_ERROR:
            uiOsdDialogDisp(ID_STR_CARD_ERROR, 0);
            break;

        case SP1K_MSG_CARD_ERROR:
            osMsgFlush(MSG_CTRL_FLUSH_HOST | MSG_CTRL_FLUSH_BTN);
            if(sp1kDiskErrorCodeGet() == DISK_ERR_DETECT)
            {
                uiOsdDialogDisp(ID_STR_PLEASE_PLUG_OUT_CARD, 1000);
            }
            else
            {
                uiOsdDialogDisp(ID_STR_CARD_ERROR, 1000);
                appCardErrFmtSta(1);
            }
            break;

        case SP1K_MSG_CARD_NOT_SUPPORT:
            uiOsdDialogDisp(ID_STR_CARD_ERROR, 1000);
            sp1kBtnDisableAll();
            break;

#if SUPPORT_RTC_OSD_UPDATE
        case SP1K_MSG_UPDATE_RTCTIME:
            appCurrentTimeShow(0);
            break;
#endif

#if SUPPORT_PANEL_PROTECT
        case SP1K_MSG_PANEL_PROTECT:
            appPanelAutoProt();
            break;

        case SP1K_MSG_SYS_CLOSE_LCD_BACKLIGHT:
            app_set_lcd_backlight_status(0);
            break;

        case SP1K_MSG_SYS_OPEN_LCD_BACKLIGHT:
            app_set_lcd_backlight_status(1);
            break;
#endif

#if SUPPORT_AWB_ADJ
        case SP1K_MSG_AWB_OFF:
            sp1kAwbDisable();
            break;

        case SP1K_MSG_AWB_CHECK:
        {
            static UINT8 prev_luma = 0;
            xdata UINT8 curr_luma = 0, diff_luma = 0;

            sp1kAeStatusGet(SP1K_AE_STS_CURRENT_LUMA, &curr_luma);

            if (curr_luma > prev_luma)
            {
                diff_luma = curr_luma - prev_luma;
            }
            else
            {
                diff_luma = prev_luma - curr_luma;
            }

            //printf("prev luma : %d\n", (UINT16)prev_luma);
            //printf("curr luma : %d\n", (UINT16)curr_luma);
            //printf("diff luma : %d\n", (UINT16)diff_luma);

            prev_luma = curr_luma;

            if (diff_luma > 50)
            {
                sp1kAwbEnable();
                awb_auto_disable_flag = 1;
            }
            else
            {
                sp1kAwbDisable();
            }
        }
        break;
#endif

        default:
            break;
        }

        return;
    }


/**
* @param    initMode: 初始化模式
* @retval   None
* @brief    设置视频分辨率的应用程序应用程序接口(设置视频参数、录制视频、预览视频、控制自动曝光、自动白平衡、人脸检测等功能)
**/
void appVideoViewInit(UINT8 initMode) USING_0
{
    UINT16 width; // 当前视频宽度
    UINT16 height; // 当前视频高度
    UINT16 pre_width; // 上一帧视频宽度
    UINT16 pre_height; // 上一帧视频高度
    uiPara_t *puiPara = appUiParaGet(); // 获取UI参数

#if 0
    // 以下代码块目前被注释掉
    if (((appSDPlugStatusGet() == 0) || (sysMemTotSizeGet() <= DEV_MEM_SIZE_64M)) && \
    ((puiPara->VideoSize == MOV_SIZE_HD) || (puiPara->VideoSize == MOV_SIZE_1080P) || (puiPara->VideoSize == MOV_SIZE_D1)))
    {
        //xiaolin.zhu
        puiPara->VideoSize = MOV_SIZE_VGA; // 如果SD卡未插入或内存不足64M，并且视频尺寸为HD、1080P或D1，则将视频尺寸设置为VGA
    }
#endif

    //sp1kSuperPreviewSet(1,1);

    sp1kVideoRecParamSet(VD_FRONT_FPS, 30); // 设置前置摄像头帧率为30fps
    sp1kVideoRecParamSet(VD_CAPTURE, 0); // 设置视频录制参数为0
    // 设置视频预览大小模式：6/8 或 8/8
    sp1kHDPvsizeStatusSet(0); // 0：设置为6/8，1：设置为8/8
#if HD_PVSIZE_STSCHG_EN
    sp1kHDPvsizeReSet(); // 如果定义了HD_PVSIZE_STSCHG_EN，则重新设置HD预览大小
#endif
    /* 电源控制 - 视频预览 */
    //appModePowerCtrl(MODE_PREVIEW); // 控制电源模式为预览模式
    sp1kPvPhotofrmEnable(0); // 禁用PV photo frame
#ifdef DPF_SCANNER
    puiPara->VideoSize = MOV_SIZE_VGA; // 如果定义了DPF_SCANNER，则将视频尺寸设置为VGA
#endif

#if MSG_AUTO_SEND
    {
        static UINT8 VSize = 2; // 静态变量用于存储视频尺寸
        VSize++; // 视频尺寸加一
        if (VSize == MOV_SIZE_MAX)
        {
            VSize = 3; // 如果视频尺寸达到MOV_SIZE_MAX，则重置为3
        }
        puiPara->VideoSize = VSize; // 更新UI参数中的视频尺寸
    }
#endif


    appVideoBrcSet();
    sp1kVideoRecSizeGet(&pre_width, &pre_height);
    appVideoResSet(puiPara->VideoSize);
    if (initMode == SP1K_VIDEO_VIEW_INIT_FULL)
    {
        // 如果是全屏视频预览模式

        // 获取当前视频录制尺寸
        sp1kVideoRecSizeGet(&width, &height);

        // 设置无缝录制时间
        appVideoSeamlessTimeSet();

        // 检查是否需要更新预览设置
        if ((width != pre_width) || (preVideoFrameRate != puiPara->FrameRate) || (appPreStateGet(0) == APP_STATE_STILL_VIEW)
        || (appPreStateGet(0) == APP_STATE_STILL_PLAY) || (appPreStateGet(0) == APP_STATE_MUTI_PLAY)
        || (appPreStateGet(0) == APP_STATE_VIDEO_PLAY) || (appPreStateGet(0) == APP_STATE_AUDIO_PLAY)
        || (appPreStateGet(0) == APP_STATE_USB) || (appPreStateGet(0) == APP_STATE_AUDIO_REC)
        || appPvForceUpdateGet() || sp1kSuperPreviewStatusGet(1))
        {
            // 如果需要更新预览设置

            appPvForceUpdateOn(0); // 关闭预览强制更新

            sp1kDispImgWinEn(0); // 禁用图像窗口显示
            sp1kCdspPvDynamicParaFullLoad(0x3c); // 加载预览动态参数

            sp1kAeSetModeEn(AE_DISABLE); // 禁用自动曝光
            sp1kAwbDisable(); // 禁用自动白平衡
            sp1kFdDisable(); // 禁用人脸检测
            sp1kDistDisable(); // 禁用失真校正

            // 设置视频预览大小
            appVideoPreSizeSet(puiPara->VideoSize);

            if (puiPara->VideoSize == MOV_SIZE_1080P)
            {
                // 如果视频尺寸为1080P

                sp1kVideoRecFrontRateSet(0x00); // 设置前置视频录制帧率为0x00
                sp1kVideoRecParamSet(VD_REC_YUV_FORMAT, VD_REC_YUV_420); // 设置录制的YUV格式为420
            }
            else
            {
                // 其他情况

                sp1kVideoRecFrontRateSet(0x00); // 设置前置视频录制帧率为0x00
                sp1kVideoRecParamSet(VD_REC_YUV_FORMAT, VD_REC_YUV_420); // 设置录制的YUV格式为420
            }

#if (VIDEO_NEW_ZOOM == 1)
            sp1kVideoZoomCtrlParaSet(SP1K_VIDEO_ZOOM_CTRL_PARA_CURRENT_LOG, videoCurrLog); // 设置缩放控制参数
            sp1kVideoRecZoomFacSet(videoZFctor, 0); // 设置视频录制缩放因子
#else
            video_zoomInFlag = 1;
            video_zoomFactor = videoZFctor;
            ZoomCurLog = videoCurrLog;
            sp1kPvZoomSet(0, video_zoomFactor, 0); // 设置PV缩放
            sp1kVideoRecZoomFacSet(video_zoomFactor, 0); // 设置视频录制缩放因子
#endif

            sp1kAeSetModeEn(AE_ENABLE); // 启用自动曝光
            sp1kAwbEnable(); // 启用自动白平衡

            sp1kDispImgWinEn(1); // 启用图像窗口显示
        }
    }

#if FUNC_HOST_MD
    sp1kMDInit(); // 初始化运动检测
    sp1kMDEnSet(MD_ENABLE); // 启用运动检测

#if (MD_TD_FUNC_EN == 1)
    sp1kCdspPvDynamicTDEn(1, 0); // 如果定义了MD_TD_FUNC_EN，启用动态温度依赖去噪功能
    sp1kMDAdjustTempoalDenoiseWeightEnSet(MD_ADJUST_TD_ENABLE); // 设置调整时序去噪权重使能
#endif
#endif

    if (puiPara->DisMode == DISMODE_ON)
    {
        if (!sp1kDistStatusGet())
        {
            sp1kDistInit(); // 初始化数字图像稳定功能
            sp1kDistEnable(); // 启用数字图像稳定功能
        }
    }
    else {
        sp1kDistDisable(); // 禁用数字图像稳定功能
    }

    /*
    if (puiPara->WDRMode == WDRMODE_ON) {
        sp1kCalibrationWDRSet(1);
    }
    else {
        sp1kCalibrationWDRSet(0);
    }
    */

#if 0
    if (puiPara->VideoSize == MOV_SIZE_1080P)
    {
        //vidClipCtrl.frameRate = 15;
        //g_vidAviFrameRate = 15;
        sp1kVideoRecFrontRateSet (0x11);
        //sp1kVideoRecFrontRateSet (15);
        sp1kVideoRecParamSet(VD_REC_YUV_FORMAT, VD_REC_YUV_420);
    }
    else {
#if 0 //for ov5653 50fps 720p test code linjiecheng
        if (puiPara->VideoSize == MOV_SIZE_HD)
        {
            sp1kVideoRecFrontRateSet (0x11);
        }
        else
#endif
        {
            sp1kVideoRecFrontRateSet (0x00);
            //sp1kVideoRecFrontRateSet (30);
            //printf("frontRateValue=%bd\n",sp1kVideoRecFrontRateGet());
            sp1kVideoRecParamSet(VD_REC_YUV_FORMAT, VD_REC_YUV_422_420);
        }
    }
#endif

    puiPara->FlashLed = 0; // 关闭闪光灯
    appSnapFlashLed(0); // 关闭闪光灯
    uiUpdateFlashLed(LCD_WIDTH, 0, 0); // 更新闪光灯显示

    appVideoOsdInit(); // 初始化视频OSD

    return;
}


/**
 * @fn        void appVideoBrcSet(void)
 * @brief     appVideoBrcSet
 * @param     NONE
 * @retval    NONE
 * @see       NULL
 * @author    dongdong.wang
 * @since     2010-9-16
 * @todo      N/A
 * @bug       N/A
*/
void appVideoBrcSet(void)
{
	UINT32 vlcsize=0;   // 初始化视频比特率大小
	uiPara_t* uiPara;
	uiPara = appUiParaGet();

	switch(uiPara->VideoSize)   // 根据视频尺寸选择不同的设置
	{

		case MOV_SIZE_D1:
			vlcsize=(UINT32)60*30*1024*8;   // 设置比特率大小
			sp1kVideoRecBitRateSet(vlcsize);    // 设置视频录制比特率
			break;
		case MOV_SIZE_VGA:
			vlcsize=(UINT32)50*30*1024*8;
			sp1kVideoRecBitRateSet(vlcsize);
			sp1kVideoQTabUpdate(QtableDefault); //1.46MB/s
			break;
		case MOV_SIZE_QVGA:
			vlcsize=(UINT32)30*30*1024*8;
			sp1kVideoRecBitRateSet(vlcsize);
			break;
		case MOV_SIZE_HD:
			vlcsize=(UINT32)110*30*1024*8;// 3.2MB/s
			sp1kVideoRecBitRateSet(vlcsize);
			sp1kVideoQTabUpdate(QtableHD);
			break;
		case MOV_SIZE_1080P:
			vlcsize=(UINT32)110*30*1024*8;
			sp1kVideoRecBitRateSet(vlcsize);
			sp1kVideoQTabUpdate(Qtable1080P);
			break;
		default:
			vlcsize=(UINT32)50*30*1024*8;
			sp1kVideoRecBitRateSet(vlcsize);
			sp1kVideoQTabUpdate(QtableDefault);
			break;
	}

}

//-----------------------------------------------------------------------------
//appVideoResSet
//-----------------------------------------------------------------------------
/**
 * @brief		用于设置视频分辨率的应用程序 api
 * @param	res: 0:VGA ,1: QVGA
 * @retval	NONE
 * @see
 * @author	sunyong
 * @since	2008-03-15
 * @todo
 * @bug
*/
void appVideoResSet(UINT8 res) USING_0
{
	switch(res) {
		case MOV_SIZE_VGA:
			sp1kVideoRecSizeSet(VID_VGA);
			break;
		case MOV_SIZE_QVGA:
			sp1kVideoRecSizeSet(VID_QVGA);
			break;
		case MOV_SIZE_QVGA_HFR:
			sp1kVideoRecSizeSet(VID_QVGA);
			break;
		case MOV_SIZE_D1:
			sp1kVideoRecSizeSet(VID_D1);
			break;
		case MOV_SIZE_HD:
			sp1kVideoRecSizeSet(VID_HD);
			break;
		case MOV_SIZE_1080P:
			sp1kVideoRecSizeSet(VID_1080p);
			break;
		default:
			sp1kVideoRecSizeSet(VID_VGA);
			break;
	}

	return;
}



//-----------------------------------------------------------------------------
//appVideoPreSizeSet
//-----------------------------------------------------------------------------
/**
 * @brief		设置视频分辨率的应用程序应用程序接口
 * @param	res: 0:VGA ,1: QVGA
 * @retval	NONE
 * @see
 * @author	sunyong
 * @since	2008-03-15
 * @todo
 * @bug
*/
void appVideoPreSizeSet(UINT8 res) USING_0
{
	UINT16 frontHSize,frontVSize;
	UINT16 cropVSize;
	UINT16 cropHsize;
	UINT16 cropHsizeHD;
	UINT16 cropVSizeHD;
	uiPara_t* puiPara = appUiParaGet();

	#if (STILL_NEW_ZOOM == 1 || VIDEO_NEW_ZOOM == 1)
	appPreviewCdspDispResSet();
	#endif

	sp1kFrontPreviewSizeGet(0, &frontHSize, &frontVSize);
	#if SUPPORT_QVGA_SENSOR
	if(frontHSize*3 > frontVSize*4){/*sensor size 16:9*/
		cropHsize = frontHSize;
		cropVSize = frontVSize;
	}else{
		cropHsize = frontHSize;
		cropVSize = frontVSize;
	}
	#else
	if(frontHSize*3 > frontVSize*4)//16:9
	{
		cropHsize=frontVSize*4/3;
		cropVSize=frontVSize;
		cropHsizeHD=frontHSize;
		cropVSizeHD=frontVSize;
	}
	else
	{
		cropHsize=frontHSize;
		cropVSize=frontVSize;
		cropHsizeHD=frontHSize;
		cropVSizeHD=frontHSize*9/16;
	}
	#endif
	switch(res) {
		case MOV_SIZE_VGA:
			switch(puiPara->FrameRate)
			{
				case MOV_FRM_HIGH:
					break;
				case MOV_FRM_LOW:
				case MOV_FRM_MIDDLE:
					break;
				default:
					ASSERT(0,0);
					break;
			}

			sp1kPreviewCropSizeSet(cropHsize,cropVSize);
			#if SUPPORT_QVGA_SENSOR
			sp1kPreviewSet(1, 320, 240, sp1kVideoZoomFctGet(), 0); /* will: video 640x4800b*/
			#else
			sp1kPreviewSet(1, 640, 480, sp1kVideoZoomFctGet(), 0); /* will: video 640x4800b*/
			#endif
			break;
		case MOV_SIZE_QVGA:
			sp1kPreviewCropSizeSet(cropHsize,cropVSize);
			sp1kPreviewSet(1, 320, 240, sp1kVideoZoomFctGet(), 0); /* will: video 320x240b*/
			break;
		case MOV_SIZE_QVGA_HFR:
			sp1kPreviewCropSizeSet(cropHsize,cropVSize);
			sp1kPreviewSet(1, 320, 240, sp1kVideoZoomFctGet(), 0); /* will: video 320x240b*/
			break;
		case MOV_SIZE_D1:
			if(frontHSize*3 > frontVSize*4)//16:9
			{
				cropHsizeHD =frontVSize*40/27;
			}else{
				cropVSizeHD=frontHSize*27/40;//modify for D1 size
			}
			//sp1kPreviewCropSizeSet(1272,720);
			sp1kPreviewCropSizeSet(cropHsizeHD,cropVSizeHD);
			//sp1kPreviewSet(1, 848, 480, sp1kVideoZoomFctGet(), 0); /* will: video 848x4800b*/
			sp1kPreviewSet(1, 640, 432, sp1kVideoZoomFctGet(), 0);//modify for D1 size
			break;
		case MOV_SIZE_HD:

			sp1kPreviewCropSizeSet(cropHsizeHD,cropVSizeHD);

			if((G_UIStatus & 0xff00) == 0x2100 /*K_UISTATUS_USBMODE_PCCAM*/)
			{
				sp1kPreviewSet(1, 1280, 720, sp1kVideoZoomFctGet(), 0); /* will: video 1280x720 */
			}
			else{
				sp1kPreviewSet(1, 960, 540, sp1kVideoZoomFctGet(), 0); /* will: video 960x540 */
			}

			break;
		case MOV_SIZE_1080P:
			sp1kPreviewCropSizeSet(cropHsizeHD,cropVSizeHD);
			sp1kPreviewSet(1, 960, 540, sp1kVideoZoomFctGet(), 0); /* will: video 1280x720 */
			break;
		default:
			sp1kPreviewCropSizeSet(cropHsize,cropVSize);
			sp1kPreviewSet(1, 640, 480, sp1kVideoZoomFctGet(), 0); /* will: video 640x4800b*/
			break;
	}
	//sp1kCalibrationLscSet(1);

	sp1kHalCdspImgSrcSel(0);	// 0: image source from front

	HAL_FrontSignalWait(HAL_FRONT_WAIT_VSYNC, 1, 4);
}


//-----------------------------------------------------------------------------
//appVideoPauseProcess
//-----------------------------------------------------------------------------
/**
 * @brief		用于视频暂停过程的应用程序 api
 * @param	NONE
 * @retval	NONE
 * @see
 * @author	sunyong
 * @since	2008-03-15
 * @todo
 * @bug
*/
void appVideoPauseProcess(void) USING_0
{
	UINT32 time;

	if(sp1kVideoRecStatusGet() == VID_REC_PAUSE) {
		sp1kVideoRecResume();
	} else {
		time = sp1kvideoRecTimeGet();
		if(time<1500) {
			return;
		}
		sp1kVideoRecPause();
	}
	uiUpdateVideoStatus(0, sp1kVideoRecStatusGet());

	return;
}

//-----------------------------------------------------------------------------
//appVideoSnap
//-----------------------------------------------------------------------------
/**
 * @brief		视频快照的应用程序应用程序接口
 * @param	NONE
 * @retval	NONE
 * @see
 * @author	sunyong
 * @since	2008-03-15
 * @todo
 * @bug
*/
void appVideoSnap(void) USING_0
{
	UINT32 time=0;
	uiPara_t* puiPara;
	puiPara = appUiParaGet();

	//appModePowerCtrl(MODE_VIDEOREC);
	//sp1kVideoRecFrameBufNumSet(2); //rf++
	sp1kVideoRecParamSet(VD_DUMY_MODE,VD_DUMY_MODE_COPY_IDX);
	if(sp1kVideoRecStatusGet() == VID_REC_IDLE) {

	 /*	if((appSDPlugStatusGet()==1)&&(sp1kSDSpeedClassGet() >=SD_SPEED_CLASS4) &&(sysMemTotSizeGet()== DEV_MEM_SIZE_64M))
		{
			sp1kVideoEnableBalanceFrame(1);
			printf("balance drop enable\n");
		}
		else
		{
			sp1kVideoEnableBalanceFrame(0);
			printf("balance drop disable\n");
		}
		*/
		if ( appCardErrFmtErrChk() ) {
			uiOsdDialogDisp(ID_STR_CARD_ERROR, 1000);
			appVideoViewInit(SP1K_VIDEO_VIEW_INIT_SMART);
			return;
		}
		else {
			uiUpdateVideoRecRemainTime(&time);

			if(time == 0) {
			   osMsgPost(SP1K_MSG_MEDIA_REC_DISK_FULL);
			   return;
			}
		}
		

		sp1kAudioVolumeSet(VOLUME_10/*puiPara->Volume*/);//volume gain up 1x
		
		sp1kvideoRecAudFmtSet(APP_AUDIO_FMT);

		#if SUPPORT_SPEAKER_CONTROL
			app_set_speaker(0);
		#endif


		#if 0//SUPPORT_PANEL_PROTECT
		puiPara->ProtTime = PANEL_TIME_3MIN;
		app_set_close_lcd_backlight_time();
		#endif
		
		sp1kVideoRecStart();
		if(sp1kVideoRecStatusGet() == VID_REC_RUN)
		{
			uiUpdateVideoRecTime(0);
		}
		else
		{
			//printf("vidRec Start err\n");
		}
	} else {
		time = sp1kvideoRecTimeGet();
		if(time<1500) {
			return;
		}
		osMsgFlush(MSG_CTRL_FLUSH_ALL);
		appVideoSmoothZoomMsgProcFlow(1);
		sp1kVideoRecStop();

		
		#if 0//SUPPORT_PANEL_PROTECT
		puiPara->ProtTime = PANEL_TIME_OFF;
		app_set_close_lcd_backlight_time();
		#endif

		#if (MD_TD_FUNC_EN == 0)
		if(puiPara->AdvanceDenoise == REC_ADVANCEDENOISE_ON){
			sp1kCdspPvDynamicTDEn(0,0);
		}
		#endif
		/*recover the Volume pre-seting */
		sp1kAudioVolumeSet(VOLUME_10);
		#if MSG_AUTO_SEND
		osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY);
		#endif

		#if SUPPORT_AUTO_TEST
		if (autoTestOpenFlag) {
			if (1 == autoTestStatus) {
				autoTestStatus = 2;
				osMsgPost(SP1K_MSG_STATE_TO_STILL_PLAY);
			}
		}
		#endif
	}
	
	uiUpdateVideoStatus(0, sp1kVideoRecStatusGet());

	return;
}

//-----------------------------------------------------------------------------
//appVideoQualitySet
//-----------------------------------------------------------------------------

/**
* @param    None
* @retval   None
* @brief    选择拍摄质量
**/
UINT8 appVideoQualitySet(void)
{
	uiPara_t* uiPara; // 指向UI参数结构体的指针
	uiPara = appUiParaGet(); // 获取UI参数

	// 如果拍摄视频尺寸为高清（HD）
	if(uiPara->VideoSize == MOV_SIZE_HD)
	{
		sp1kVideoRecQTableSet(QtableHD[4]); // 设置视频录制量化表为高清质量
	}

	// 如果拍摄视频尺寸为1080P
	else if(uiPara->VideoSize == MOV_SIZE_1080P)
	{
		sp1kVideoRecQTableSet(Qtable1080P[4]); // 设置视频录制量化表为1080P质量
	}

	else
	{
		// 根据图像质量设置视频录制量化表
		switch(uiPara->ImageQuality)
		{
			case MOV_QUAL_SUPER:
				sp1kVideoRecQTableSet(QtableDefault[9]);//80); // 设置视频录制量化表为超级质量
				//qt = 0;//K_Q90
				//putchar('9');
				break;
			case MOV_QUAL_FINE:
				sp1kVideoRecQTableSet(QtableDefault[5]); // 设置视频录制量化表为精细质量
				//qt = 1;//K_Q80
				//putchar('8');
				break;
			case MOV_QUAL_NORMAL:
				sp1kVideoRecQTableSet(QtableDefault[0]); // 设置视频录制量化表为普通质量
				//qt = 2;//K_Q50
				//putchar('5');
				break;
			default:
				break;
		}
	}
	return SUCCESS; // 返回成功状态
}

/**
* @param    None
* @retval   成功
* @brief    选择视频帧率
**/
UINT8 appVideoFrmRateSet(void)
{
	uiPara_t* uiPara; // 指向 UI 参数结构体的指针

	uiPara = appUiParaGet(); // 获取 UI 参数

	#if 0
	if(uiPara->VideoSize!= MOV_SIZE_VGA)
	{
		uiPara->FrameRate = MOV_FRM_MIDDLE;
	}
	else if(uiPara->FrameRate == MOV_FRM_LOW) {
		uiPara->FrameRate = MOV_FRM_MIDDLE;
	}
	else if(frameRateIsL== 0)
	{
		uiPara->FrameRate = MOV_FRM_HIGH;
	}
	if(uiPara->VideoSize==MOV_SIZE_1080P)      //yp begin for 42848
	{
		uiPara->FrameRate=MOV_FRM_LOW;
	}       //yp end for manits42848
	#else
	uiPara->FrameRate = MOV_FRM_MIDDLE;
	#endif
	
	sp1kVideoRecParamSet(VD_DUMY_SPACE, 1); // 设置视频录制参数

	// 根据帧率设置相应的参数
	switch(uiPara->FrameRate)
	{
		case MOV_FRM_HIGH: // 高帧率
			sp1kVideoRecFrameRateSet(60); // 设置录制帧率为 60
			sp1kVideoRecAviFrameRateSet(60); // 设置 AVI 文件帧率为 60
			sp1kVideoRecParamSet(VD_FRONT_FPS,60); // 设置前置摄像头帧率为 60
			// 根据光源频率设置帧率参数
			if(uiPara->LightFreq == PRV_FREQ_50)
			{
				sp1kAeParamSet(SP1K_AE_FRAME_RATE, SP1K_FRAME_RATE_50); // 设置自动曝光帧率为 50
			}
			else
			{
				sp1kAeParamSet(SP1K_AE_FRAME_RATE, SP1K_FRAME_RATE_60); // 设置自动曝光帧率为 60
			}
			break;
		case MOV_FRM_LOW: // 低帧率
			sp1kVideoRecFrameRateSet(15); // 设置录制帧率为 15
			sp1kVideoRecAviFrameRateSet(15); // 设置 AVI 文件帧率为 15
			sp1kVideoRecParamSet(VD_FRONT_FPS,15); // 设置前置摄像头帧率为 15
			// 根据光源频率设置帧率参数
			if(uiPara->LightFreq == PRV_FREQ_50) {
				sp1kAeParamSet(SP1K_AE_FRAME_RATE, SP1K_FRAME_RATE_12P5); // 设置自动曝光帧率为 12.5
			}
			else {
				sp1kAeParamSet(SP1K_AE_FRAME_RATE, SP1K_FRAME_RATE_15); // 设置自动曝光帧率为 15
			}
			break;
		case MOV_FRM_MIDDLE: // 中间帧率（默认）
		default:
			sp1kVideoRecFrameRateSet(30); // 设置录制帧率为 30
			sp1kVideoRecAviFrameRateSet(30); // 设置 AVI 文件帧率为 30
			sp1kVideoRecParamSet(VD_FRONT_FPS,30); // 设置前置摄像头帧率为 30
			// 根据光源频率设置帧率参数
			if(uiPara->LightFreq == PRV_FREQ_50) {
				sp1kAeParamSet(SP1K_AE_FRAME_RATE, SP1K_FRAME_RATE_25); // 设置自动曝光帧率为 25
			}
			else {
				sp1kAeParamSet(SP1K_AE_FRAME_RATE, SP1K_FRAME_RATE_30); // 设置自动曝光帧率为 30
			}
			break;
	}

	return SUCCESS; // 返回成功状态
}



//-----------------------------------------------------------------------------
//videoRecKeyOnUp
//-----------------------------------------------------------------------------
/**
 * @brief		视频键启动过程
 * @param	NONE
 * @retval	NONE
 * @todo
 * @author	SunYong
 * @since	2007-12-18
 */
#if (VIDEO_NEW_ZOOM == 0)
void appVideoViewTele (void) USING_0
{
	UINT16 newFact;
	//__FUNC_TRACK__;
	// printf("curlog=%d,factor=%d",ZoomCurLog,video_zoomFactor);
	ZoomCurLogBack = ZoomCurLog;
	video_zoomFactorBack = video_zoomFactor;
	video_zoomInFlagBack = video_zoomInFlag;

	ZoomCurLog	-= VIDEO_ZOOM_LOG_STEP;
	video_zoomInFlag = 1;

	if (ZoomCurLog < VIDEO_ZOOM_MIN_LOG)
		ZoomCurLog = VIDEO_ZOOM_MIN_LOG;

	#if (VIDEO_ZOOM_FACTOR_LOOKUP_TABLE == 1)
	newFact = VideoZoomFactor[(100-ZoomCurLog)/2];
	#else
	newFact = 10000/ZoomCurLog;
	#endif

 	if(newFact <= VIDEO_ZOOM_MAX) {
		video_zoomFactor = newFact;
		// printf("fac:%d\n",video_zoomFactor);
	}
	else {
		ZoomCurLog  = VIDEO_ZOOM_MIN_LOG;
		video_zoomFactor = VIDEO_ZOOM_MAX;
	}
	 //printf("fac:%d\n",video_zoomFactor);
}
#endif

/**
 * @brief		视频键启动过程
 * @param	NONE
 * @retval	NONE
 * @todo
 * @author	SunYong
 * @since	2007-12-18
 */
#if (VIDEO_NEW_ZOOM == 1)
void appVideoViewTele (void) USING_0
{
	if (sp1kVideoTeleSet() == FAIL) {
		//note : max zoom factor
	}
}
#endif

//-----------------------------------------------------------------------------
//videoRecKeyOnDown
//-----------------------------------------------------------------------------
/**
 * @brief		视频按键开机过程
 * @param	NONE
 * @retval	NONE
 * @todo
 * @author	SunYong
 * @since	2007-12-18
 */
#if (VIDEO_NEW_ZOOM == 0)
void appVideoViewWide (void) USING_0
{
	UINT16 newFact;

	ZoomCurLogBack = ZoomCurLog;
	video_zoomFactorBack = video_zoomFactor;
	video_zoomOutFlagBack = video_zoomOutFlag;

	if(video_zoomFactor<= VIDEO_ZOOM_MIN){
		return;
	}
	ZoomCurLog += VIDEO_ZOOM_LOG_STEP;
	video_zoomOutFlag = 1;
	if (ZoomCurLog > VIDEO_ZOOM_MAX_LOG)
		ZoomCurLog = VIDEO_ZOOM_MAX_LOG;

	#if (VIDEO_ZOOM_FACTOR_LOOKUP_TABLE == 1)
	newFact = VideoZoomFactor[(100-ZoomCurLog)/2];
	#else
	newFact = 10000/ZoomCurLog ;// zoom 4X
	#endif

	//printf("fct:%d\n",video_zoomFactor );

	if(newFact >= VIDEO_ZOOM_MIN ) {
		video_zoomFactor  = newFact;
	}
	else {
		ZoomCurLog  = VIDEO_ZOOM_MAX_LOG;
		video_zoomFactor = VIDEO_ZOOM_MIN;
	}
}
#endif

/**
 * @brief		视频按键启动过程
 * @param	NONE
 * @retval	NONE
 * @todo
 * @author	SunYong
 * @since	2007-12-18
 */
#if (VIDEO_NEW_ZOOM == 1)
void appVideoViewWide (void) USING_0
{
	if (sp1kVideoWideSet() == FAIL) {
		//note : min zoom factor
	}
}
#endif


//-----------------------------------------------------------------------------
//videoRecKeyOnUpReply
//-----------------------------------------------------------------------------
/**
 * @brief		视频键向上回复过程
 * @param	NONE
 * @retval	NONE
 * @todo
 * @author	SunYong
 * @since	2007-12-18
 */
#if (VIDEO_NEW_ZOOM == 0)
void appVideoViewTeleReply (void) USING_0
{
	ZoomCurLog = ZoomCurLogBack;
	video_zoomFactor = video_zoomFactorBack;
	video_zoomInFlag = video_zoomInFlagBack;
}
#endif

//-----------------------------------------------------------------------------
//videoRecKeyOnDownReply
//-----------------------------------------------------------------------------
/**
 * @brief		video key on down Reply process
 * @param	NONE
 * @retval	NONE
 * @todo
 * @author	SunYong
 * @since	2007-12-18
 */
#if (VIDEO_NEW_ZOOM == 0)
void appVideoViewWideReply (void) USING_0
{
	ZoomCurLog = ZoomCurLogBack;
	video_zoomFactor = video_zoomFactorBack;
	video_zoomOutFlag = video_zoomOutFlagBack;
}
#endif
UINT8 appVideoRecMemFullChk(void)
{
	UINT32 time;
	UINT8 sts=1; //1    1:Memory full,0:Remain  space

	uiUpdateVideoRecRemainTime(&time);
	if(time >100){
		sts =0;
	}

	return sts;
}

#if FUNC_HOST_MD
void appMotionDetectTimeSet(void)
{
	sp1kTimer1Config(20000, pFunc_MDTimeProcess,NULL);
	sp1kTimer1Start();
}
#endif


/**
 * @brief		appVideoSmoothZoomMsgProcFlow
 * @param	NONE
 * @retval	NONE
 * @todo
 * @author	SunYong
 * @since	2007-12-18
 */

UINT8 appVideoSmoothZoomMsgProcFlow (UINT16 sysMsg)
{

	sysMsg=sysMsg;
	#if (VIDEO_NEW_ZOOM == 1)
	switch ( sysMsg ) {
		case 1:
		case SP1K_MSG_CARD_PLUG_IN:
		case SP1K_MSG_CARD_PLUG_OUT:
		case SP1K_MSG_USB_PLUG_IN:
		case SP1K_MSG_TV_PLUG_IN:
		case SP1K_MSG_TV_PLUG_OUT:
		case SP1K_MSG_HDMI_PLUG_IN:
		case SP1K_MSG_MEDIA_REC_DISK_FULL:
		case SP1K_MSG_MEDIA_REC_FILE_MAX:
		case sp1k_MSG_REC_SEAMLESS_DISK_FULL:
		case SP1K_MSG_BAT_EMPTY:
			if (sp1kVideoSmoothZoomGet() == 1) {
				sp1kVideoSmoothZoomSet(0);
				sp1kVideoZoom3APostSet();
				sp1kAeSetModeEn(AE_ENABLE);
				sp1kAwbEnable();
			}
			while(sp1kVideoSmoothZoomStatusGet() == FAIL);
			uiUpdateOSDPvZoom(sp1kVideoZoomFctGet(), 1);
			break;
		case SP1K_MSG_KEY_PRESS_S2:
			if (sp1kVideoSmoothZoomStatusGet() == FAIL) {
				return FAIL;
			}
			break;
	}
	#endif

	return SUCCESS;
}




void appVideoSeamlessTimeSet(void)
{
	uiPara_t *puiPara = appUiParaGet();
	UINT32 seamlessTime = 0;

	switch (puiPara->Seamless) {
		case 0 :
			seamlessTime = 0;
			break;
		case 1 :
			seamlessTime = 180;
			break;
	}
		
	
	if(seamlessTime){
		sp1kVideoSeamlessLPRecStsSet(1);
		sp1kVideoRecParamSet(VD_SLREC_RenameMode,2);
		sp1kVideoRecParamSet(VD_SLREC_MODE, 1);
		sp1kVideoSeamlessLPRecTimeSet(seamlessTime);
	}else{
		sp1kVideoSeamlessLPRecStsSet(0);
	}
}

#if 1
//extern UINT8 videoRecStampSet(UINT16 stampW, UINT16 stampH, UINT16 fontW,UINT16 fontH);

/**
* @param    None
* @retval   None
* @brief    设置视频时间戳
**/
void appVideoStampSet(void) 
{
    uiPara_t* puiPara;
    UINT8  mode, recMode, pos;
    UINT16 fmt = 0;

    UINT16 bgid, bgw, bgh, fntw, fnth;
    UINT8 isTwoLine = 1;
    UINT8 isLoadImage = 0;
    UINT32 bgAddr, bgW, bgH;
    UINT16 timeOffx, timeOffy;

    puiPara = appUiParaGet();
    if (puiPara->StampVideoMode) {
        mode = VD_STAMP_MODE_DATE;
        recMode = VD_STAMP_REC_MODE_ALL_FRAME;
        pos = VD_STAMP_RIGHT | VD_STAMP_BOTTOM;

        // 配置视频录制时间戳
        sp1kVideoRecStampCfg(mode, pos, recMode);

        switch (puiPara->VideoSize) {
            case MOV_SIZE_1080P:
            case MOV_SIZE_HD:
                bgid = 0; bgw = 320; bgh = 96; fntw = 24; fnth = 48;
                break;

            case MOV_SIZE_VGA:
            case MOV_SIZE_QVGA:
                bgid = 0; bgw = 160; bgh = 32; fntw = 8; fnth = 16;
                break;

            case MOV_SIZE_D1:
            default:
                bgid = 0; bgw = 240; bgh = 64; fntw = 16; fnth = 32;
                break;
        }

        if (!isTwoLine) { // 是否双行
            bgw /= 2; bgh *= 2;
            timeOffx = 0; timeOffy = fnth;
        } else {
            timeOffx = fntw * 12; timeOffy = 0;
        }

        // 初始化日期/时间/图像时间戳
        sp1kStampTextReset();
        sp1kVideoRecStampSet(0x10, bgw, bgh, fntw, fnth);
        sp1kStampDateTimeFmtSet(0, 2); // 设置日期和时间格式

        // 设置日期和时间在背景上的位置和透明度
        sp1kStampTextSet(STAMP_DATE, 0, 0, 5);
        sp1kStampTextSet(STAMP_TIME, 0, fnth, 5);

        // 根据视频时间戳模式选择显示内容
        switch (puiPara->StampVideoMode) {
            case PRV_STAMP_DATE:
                sp1kStampTextEnable(STAMP_DATE, 1);
                sp1kStampTextEnable(STAMP_TIME, 0);
                break;
            case PRV_STAMP_DATETIME:
                sp1kStampTextEnable(STAMP_DATE, 1);
                sp1kStampTextEnable(STAMP_TIME, 1);
                break;
            default:
                sp1kStampTextEnable(STAMP_DATE, 0);
                sp1kStampTextEnable(STAMP_TIME, 0);
                break;
        }

        // 设置日期时间戳的偏移
        sp1kVideoRecParamSet(VD_STAMP_DISOFFX, 1920); // 设置日期时间戳的X偏移
        sp1kVideoRecParamSet(VD_STAMP_DISOFFY, 1088); // 设置日期时间戳的Y偏移

        sp1kStampBackgroundGet(&bgAddr, &bgW, &bgH);
        // 退出背景图像
        if (bgid) {
            sp1kNandRsvRead((UINT32)bgid, bgAddr << 1);
        } else {
            sp1kPbFillBlack(bgAddr, bgW, bgH);
        }

        // OSD初始化
        {
            video_osd_text_t* vidDate = sp1kVideoRecOsdTextGet(STAMP_DATE);
            video_osd_text_t* vidTime = vidDate + 1;
            UINT16 fntw, fnth;
            UINT16 dispH = LCD_HEIGHT, dispW = LCD_WIDTH;
            dateStc_t dateInf;
            sp1kOsdFontSizeGet(&fntw, &fnth);
            vidDate->width = strWidthGet("44/44/4444  ");
            vidTime->width = strWidthGet("44:44:44");
            vidDate->height = fnth;
            vidTime->height = fnth;

            vidTime->xoff = dispW - vidTime->width - 10;
            vidDate->xoff = vidTime->xoff - vidDate->width - 10;

            vidDate->yoff = dispH - 2 * fnth + 3;
            vidTime->yoff = dispH - 2 * fnth + 3;

            sp1kHalReadRTC(&dateInf);
            stampDateFormat(vidDate->text, dateInf.Year, dateInf.Month, dateInf.Day, 0);
            stampTimeFormat(vidTime->text, dateInf.Hour, dateInf.Minute, dateInf.Second, 2);
        }

        sp1kVideoRecStampStsSet(1);
        sp1kVideoCapDateStampEn(1); // 用于速拍捕获
    } else {
        sp1kVideoRecStampStsSet(0);
        sp1kVideoCapDateStampEn(0); // 用于速拍捕获
    }
}


#else
void appVideoStampSet(void)
{
	uiPara_t* puiPara;
	UINT8  mode,recMode, pos;
	UINT16 fmt=0;
	puiPara = appUiParaGet();

	if(puiPara->StampVideoMode)
	{


		switch(puiPara->DateStyle)
		{
			case DATESTYLE_MMDDYYYY:
				fmt = VD_STAMP_FMT_MMDDYYYY;
				break;
			case DATESTYLE_DDMMYYYY:
				fmt = VD_STAMP_FMT_DDMMYYYY;
				break;
			case DATESTYLE_YYYYMMDD:
			default:
				fmt = VD_STAMP_FMT_YYYYMMDD;
				break;
		}
		if(puiPara->StampVideoMode == PRV_STAMP_DATETIME)
		{
			fmt |= VD_STAMP_FMT_HHMMSS;
		}
	    fmt |=VD_STAMP_REE_MODE_TWO_LINE; //Date and time two line show; Note if in two line mode,can not set img and date time
		mode = VD_STAMP_MODE_DATE ;
		recMode = VD_STAMP_REC_MODE_ALL_FRAME;
		pos = VD_STAMP_RIGHT | VD_STAMP_BOTTOM;

		sp1kVideoRecStampCfg(mode, pos, recMode);
		sp1kVideoRecStampDateFmtSet(fmt);
		sp1kVideoRecParamSet(VD_STAMP_DATETIME_DIS_X_RATIO, 1);
		sp1kVideoRecParamSet(VD_STAMP_DISOFFX, 1920); //set date stamp OffsetX
		sp1kVideoRecParamSet(VD_STAMP_DISOFFY, 1088);  //set date stamp OffsetY

		if(mode == VD_STAMP_MODE_DATE)
		{  if(((fmt & VD_STAMP_REE_MODE_TWO_LINE) == VD_STAMP_REE_MODE_TWO_LINE )&& (fmt & 0x30)) //Date and time two line show
		  {
				if(puiPara->VideoSize == MOV_SIZE_QVGA)
				{
					sp1kVideoRecStampImgResLoad(0, 160, 32); //back ground image
				}
				else if(puiPara->VideoSize == MOV_SIZE_1080P)
				{
					sp1kVideoRecStampImgResLoad(0, 320, 96);
				}
				else if(puiPara->VideoSize == MOV_SIZE_HD)
				{
					sp1kVideoRecStampImgResLoad(0, 320, 96);
				}
				else if(puiPara->VideoSize == MOV_SIZE_D1)
				{
					sp1kVideoRecStampImgResLoad(0, 240, 64);
				}
				else
				{
					sp1kVideoRecStampImgResLoad(0, 240, 64);
				}
		  }
		  else
		  	{
		  		if(puiPara->VideoSize == MOV_SIZE_QVGA)
				{
					sp1kVideoRecStampImgResLoad(0, 240, 16); //back ground image
				}
				else if(puiPara->VideoSize == MOV_SIZE_1080P)
				{
					sp1kVideoRecStampImgResLoad(0, 720, 48);
				}
				else if(puiPara->VideoSize == MOV_SIZE_HD)
				{
					sp1kVideoRecStampImgResLoad(0, 720, 48);
				}
				else if(puiPara->VideoSize == MOV_SIZE_D1)
				{
					sp1kVideoRecStampImgResLoad(0, 480, 32);
				}
				else
				{
					sp1kVideoRecStampImgResLoad(0, 480, 32);
				}
		  	}
		}
		else if(mode == VD_STAMP_MODE_IMG_DATE)
		{ //img and Date time
			if(puiPara->VideoSize == MOV_SIZE_QVGA)
			{
				sp1kVideoRecStampImgResLoad(0xba, 192, 16); //back ground image
			}
			else if(puiPara->VideoSize == MOV_SIZE_HD)
			{
				sp1kVideoRecStampImgResLoad(0xb8, 720, 48);
			}
			else if(puiPara->VideoSize == MOV_SIZE_D1)
			{
				sp1kVideoRecStampImgResLoad(0xb9, 512, 32);

			}
			else
			{
				sp1kVideoRecStampImgResLoad(0xb9, 512, 32);
			}
		}
		else
		{
			//img
			if(puiPara->VideoSize == MOV_SIZE_QVGA)
			{
				sp1kVideoRecStampImgResLoad(0xba, 192, 16); //back ground image
			}
			else if(puiPara->VideoSize == MOV_SIZE_HD)
			{
				sp1kVideoRecStampImgResLoad(0xb8, 720, 48);
			}
			else if(puiPara->VideoSize == MOV_SIZE_D1)
			{
				sp1kVideoRecStampImgResLoad(0xb9,512, 32);

			}
			else
			{
				sp1kVideoRecStampImgResLoad(0xb9, 512, 32);
			}
		}
		sp1kVideoRecStampStsSet(1);
	}
	else
	{
		sp1kVideoRecStampStsSet(0);
	}

}
#endif

