/***************************************************************************
 * File: app_key_scan.c
 * Author: Yang
 * Date: 2024-03-06 18:51:22
 * description:
 -----------------------------------
按键检测,消息处理
 -----------------------------------
****************************************************************************/
#include "general.h"
#include "reg_def.h"
#include "timer.h"
#include "gpio.h"
#include "gpio_api.h"
#include "key_scan.h"
#include "os_msg.h"
#include "app_msg.h"
#include "app_dev_storage.h"
#include "app_dev_usb.h"
#include "app_dev_disp.h"
#include "sp1k_util_api.h"
#include "sp1k_gpio_api.h"
#include "sp1k_hal_api.h"
#include "App_ui_osd.h"
#include "app_video_view.h"
#include "gui_core.h"
#include "gui_evt.h"
#include "gui_tp.h"
#include "app_tp_scan.h"
#include "sp1k_video_api.h"
#include "video_pb.h"
#include "App_ui_para.h"
#include "App_menu_micro.h"
#include "app_main.h"

/*-------------------- USER CODE: Include Begin --------------------*/

#include "User_Config.h"

/*-------------------- USER CODE: Include End --------------------*/


// 0: 表示使用旧的代码方式，即不使用专用引脚  1: 表示使用新的代码方式，即使用专用引脚
#define USE_PKG_DEDICATE_PIN 1

#define KEY_VALUE_AVEAGE 0

#if 0
#define DBG_KEYSCAN
#endif

/* Public variables==========================================================*/

UINT8 keyFlag = 0;
UINT8 prevKeyVal = 0;
UINT8 flag1627 = 0;
#if KEY_VALUE_AVEAGE
UINT8 keepChannelFlag = 0; // 控制通道的状态 1: 保持通道 0 : 允许改变通道
#endif

extern xdata UINT8 G_DgpioValue;
extern UINT8 affarflag;
extern UINT8 selfsnapbackfarflag;
extern UINT8 versionclearflag;
// extern UINT8 afdoingflag;

/*-------------------- USER CODE: Custom Begin --------------------*/

/*根据宏定义选择定义 ADC_A设备表 静态数组*/
#if (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_ || _HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_)
static UINT16 code keyAdcAValDevTbl[SP1K_ADC_BTN_A_NUM] = 
{
    SP1K_ADC_BTN_DEV_A_S1,
    SP1K_ADC_BTN_DEV_A_S2,
    SP1K_ADC_BTN_DEV_A_S3,
    SP1K_ADC_BTN_DEV_A_S4,
};
#else   // 【当前进入这】
static UINT16 code keyAdcAValDevTbl[SP1K_ADC_BTN_A_NUM] = 
{
    SP1K_ADC_BTN_DEV_A_S1,
    // SP1K_ADC_BTN_DEV_A_S2,
    // SP1K_ADC_BTN_DEV_A_S3,
    // SP1K_ADC_BTN_DEV_A_S4,
};
#endif

/*根据宏定义选择定义 ADC_B设备表 静态数组*/
#if (_HW_SOLUTION_ == _HW_EVB_ || _HW_SOLUTION_ == _HW_TK_MODEL_B_) // 【当前进入这】
static UINT16 code keyAdcBValDevTbl[SP1K_ADC_BTN_B_NUM] = 
{
    SP1K_ADC_BTN_DEV_B_S1,
    SP1K_ADC_BTN_DEV_B_S2,
    SP1K_ADC_BTN_DEV_B_S3,
    SP1K_ADC_BTN_DEV_B_S4,
    SP1K_ADC_BTN_DEV_B_S5,
    SP1K_ADC_BTN_DEV_B_S6,
    SP1K_ADC_BTN_DEV_B_S7,
};
#elif (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)
static UINT16 code keyAdcBValDevTbl[SP1K_ADC_BTN_B_NUM] = 
{
    SP1K_ADC_BTN_DEV_B_S1,
    SP1K_ADC_BTN_DEV_B_S2,
};
#elif (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_)
static UINT16 code keyAdcBValDevTbl[SP1K_ADC_BTN_B_NUM] = 
{
    SP1K_ADC_BTN_DEV_B_S1,
    SP1K_ADC_BTN_DEV_B_S2,
    SP1K_ADC_BTN_DEV_B_S3,
};
#elif (_HW_SOLUTION_ == _HW_RDK_A_)
static UINT16 code keyAdcBValDevTbl[SP1K_ADC_BTN_B_NUM] = 
{
    SP1K_ADC_BTN_DEV_B_S1,
    SP1K_ADC_BTN_DEV_B_S2,
    SP1K_ADC_BTN_DEV_B_S3,
    SP1K_ADC_BTN_DEV_B_S4,
};
#elif (_HW_SOLUTION_ == _HW_TK_MODEL_A_)
static UINT16 code keyAdcBValDevTbl[SP1K_ADC_BTN_B_NUM] = 
{
    SP1K_ADC_BTN_DEV_B_S1,
    SP1K_ADC_BTN_DEV_B_S2,
    SP1K_ADC_BTN_DEV_B_S3,
};
#endif

/*根据宏定义选择定义 ADC_A值 静态数组*/
#if (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_ || _HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_)
static UINT16 code keyAdcAVal[SP1K_ADC_BTN_A_NUM] = 
{
    SP1K_ADC_BTN_A_S1,
    SP1K_ADC_BTN_A_S2,
    SP1K_ADC_BTN_A_S3,
    SP1K_ADC_BTN_A_S4,
};
#else   // 【当前进入这】
static UINT16 code keyAdcAVal[SP1K_ADC_BTN_A_NUM] = 
{
    SP1K_ADC_BTN_A_S1,
    // SP1K_ADC_BTN_A_S2,
    // SP1K_ADC_BTN_A_S3,
    // SP1K_ADC_BTN_A_S4,
};
#endif

/*根据宏定义选择定义 ADC_B值 静态数组*/
#if (_HW_SOLUTION_ == _HW_EVB_ || _HW_SOLUTION_ == _HW_TK_MODEL_B_) // 【当前进入这】
static UINT16 code keyAdcBVal[SP1K_ADC_BTN_B_NUM] = 
{
    SP1K_ADC_BTN_B_S1,
    SP1K_ADC_BTN_B_S2,
    SP1K_ADC_BTN_B_S3,
    SP1K_ADC_BTN_B_S4,
    SP1K_ADC_BTN_B_S5,
    SP1K_ADC_BTN_B_S6,
    SP1K_ADC_BTN_B_S7,
};
#elif (_HW_SOLUTION_ == _HW_RDK_A_)

static UINT16 code keyAdcBVal[SP1K_ADC_BTN_B_NUM] = 
{
    SP1K_ADC_BTN_B_S1,
    SP1K_ADC_BTN_B_S2,
    SP1K_ADC_BTN_B_S3,
    SP1K_ADC_BTN_B_S4,
};
#elif (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)

static UINT16 code keyAdcBVal[SP1K_ADC_BTN_B_NUM] = 
{
    SP1K_ADC_BTN_B_S1,
    SP1K_ADC_BTN_B_S2,
};
#elif (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_)

static UINT16 code keyAdcBVal[SP1K_ADC_BTN_B_NUM] = 
{
    SP1K_ADC_BTN_B_S1,
    SP1K_ADC_BTN_B_S2,
    SP1K_ADC_BTN_B_S3,
};
#elif (_HW_SOLUTION_ == _HW_TK_MODEL_A_)
static UINT16 code keyAdcBVal[SP1K_ADC_BTN_B_NUM] = 
{
    SP1K_ADC_BTN_B_S1,
    SP1K_ADC_BTN_B_S2,
    SP1K_ADC_BTN_B_S3,
};
#endif

/*按键消息的定义*/
static keyMsg_t code keyMsg[SP1K_BTN_NUM] = 
{
#if (_HW_SOLUTION_ == _HW_EVB_)  // 【当前进入这】

    /*ADC A(SARIN0) button msg*/

    //{SP1K_MSG_KEY_PRESS_DOWN, 		SP1K_MSG_KEY_REPEATE_DOWN,			SP1K_MSG_KEY_RELEASE_DOWN},//left
    //{SP1K_MSG_KEY_PRESS_PB, 			SP1K_MSG_KEY_REPEATE_PB,			SP1K_MSG_KEY_RELEASE_PB},//del
    //{SP1K_MSG_KEY_PRESS_OK, 			SP1K_MSG_NULL,						SP1K_MSG_KEY_RELEASE_OK},//ok
    {SP1K_MSG_KEY_PRESS_POWER, SP1K_MSG_KEY_REPEATE_POWER, SP1K_MSG_KEY_RELEASE_POWER}, // power

    /*ADC B(SARIN2) button msg*/

    {SP1K_MSG_KEY_PRESS_PB, SP1K_MSG_KEY_REPEATE_PB, SP1K_MSG_KEY_RELEASE_PB}, // RIGHT1
    //{SP1K_MSG_KEY_PRESS_BURST, 		SP1K_MSG_KEY_REPEATE_BURST,			SP1K_MSG_KEY_RELEASE_BURST},
    {SP1K_MSG_KEY_PRESS_UP, SP1K_MSG_KEY_REPEATE_UP, SP1K_MSG_KEY_RELEASE_UP},       // UP1
    {SP1K_MSG_KEY_PRESS_BACK, SP1K_MSG_KEY_REPEATE_BACK, SP1K_MSG_KEY_RELEASE_BACK}, // BACK1
    {SP1K_MSG_KEY_PRESS_MODE, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_MODE},             // MODE1
    {SP1K_MSG_KEY_PRESS_DOWN, SP1K_MSG_KEY_REPEATE_DOWN, SP1K_MSG_KEY_RELEASE_DOWN}, // DOWN1
    {SP1K_MSG_KEY_PRESS_OK, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_OK},                 // OK1
    {SP1K_MSG_KEY_PRESS_MENU, SP1K_MSG_KEY_REPEATE_MENU, SP1K_MSG_KEY_RELEASE_MENU}, // DEL1

    /*GPIO(GPIO和DGPIO) button msg*/

    {SP1K_MSG_KEY_PRESS_ZOOM_OUT, SP1K_MSG_KEY_REPEATE_ZOOM_OUT, SP1K_MSG_KEY_RELEASE_ZOOM_OUT}, // w
    {SP1K_MSG_KEY_PRESS_SNAP, SP1K_MSG_KEY_REPEATE_SNAP, SP1K_MSG_KEY_RELEASE_SNAP},             // shutter
    {SP1K_MSG_KEY_PRESS_ZOOM_IN, SP1K_MSG_KEY_REPEATE_ZOOM_IN, SP1K_MSG_KEY_RELEASE_ZOOM_IN},    // t
    {SP1K_MSG_KEY_PRESS_AF, SP1K_MSG_NULL, SP1K_MSG_NULL},  // AF功放使能

#elif (_HW_SOLUTION_ == _HW_TK_MODEL_A_)

    /*ADC A button msg*/
    {SP1K_MSG_KEY_PRESS_RIGHT, SP1K_MSG_KEY_REPEATE_RIGHT, SP1K_MSG_KEY_RELEASE_RIGHT},
    {SP1K_MSG_KEY_PRESS_LEFT, SP1K_MSG_KEY_REPEATE_LEFT, SP1K_MSG_KEY_RELEASE_LEFT},
    {SP1K_MSG_KEY_PRESS_DOWN, SP1K_MSG_KEY_REPEATE_DOWN, SP1K_MSG_KEY_RELEASE_DOWN},
    {SP1K_MSG_KEY_PRESS_UP, SP1K_MSG_KEY_REPEATE_UP, SP1K_MSG_KEY_RELEASE_UP},
    {SP1K_MSG_KEY_PRESS_TELE, SP1K_MSG_KEY_REPEATE_TELE, SP1K_MSG_KEY_RELEASE_TELE},
    {SP1K_MSG_KEY_PRESS_WIDE, SP1K_MSG_KEY_REPEATE_WIDE, SP1K_MSG_KEY_RELEASE_WIDE},

    /*ADC B button msg*/
    {SP1K_MSG_KEY_PRESS_OK, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_OK},
    {SP1K_MSG_KEY_PRESS_PB, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_PB},
    {SP1K_MSG_KEY_PRESS_MENU, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_MENU},

    /*GPIO button msg*/
    {SP1K_MSG_KEY_PRESS_POWER, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_POWER},
    {SP1K_MSG_KEY_PRESS_S2, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_S2},

#elif (_HW_SOLUTION_ == _HW_TK_MODEL_B_)
    /*ADC A button msg*/
    {SP1K_MSG_KEY_PRESS_RIGHT, SP1K_MSG_KEY_REPEATE_RIGHT, SP1K_MSG_KEY_RELEASE_RIGHT},
    {SP1K_MSG_KEY_PRESS_LEFT, SP1K_MSG_KEY_REPEATE_LEFT, SP1K_MSG_KEY_RELEASE_LEFT},
    {SP1K_MSG_KEY_PRESS_DOWN, SP1K_MSG_KEY_REPEATE_DOWN, SP1K_MSG_KEY_RELEASE_DOWN},
    {SP1K_MSG_KEY_PRESS_UP, SP1K_MSG_KEY_REPEATE_UP, SP1K_MSG_KEY_RELEASE_UP},
    {SP1K_MSG_KEY_PRESS_TELE, SP1K_MSG_KEY_REPEATE_TELE, SP1K_MSG_KEY_RELEASE_TELE},
    {SP1K_MSG_KEY_PRESS_WIDE, SP1K_MSG_KEY_REPEATE_WIDE, SP1K_MSG_KEY_RELEASE_WIDE},

    /*ADC B button msg*/
    {SP1K_MSG_KEY_PRESS_POWER, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_POWER},
    {SP1K_MSG_KEY_PRESS_OK, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_OK},
    {SP1K_MSG_KEY_PRESS_PB, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_PB},
    {SP1K_MSG_KEY_PRESS_MENU, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_MENU},

    /*GPIO button msg*/
    {SP1K_MSG_NULL, SP1K_MSG_NULL, SP1K_MSG_NULL},
    {SP1K_MSG_KEY_PRESS_S2, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_S2},

#elif (_HW_SOLUTION_ == _HW_RDK_A_)

    /*ADC A button msg*/
    {SP1K_MSG_KEY_PRESS_RIGHT, SP1K_MSG_KEY_REPEATE_RIGHT, SP1K_MSG_KEY_RELEASE_RIGHT},
    {SP1K_MSG_KEY_PRESS_LEFT, SP1K_MSG_KEY_REPEATE_LEFT, SP1K_MSG_KEY_RELEASE_LEFT},
    {SP1K_MSG_KEY_PRESS_DOWN, SP1K_MSG_KEY_REPEATE_DOWN, SP1K_MSG_KEY_RELEASE_DOWN},
    {SP1K_MSG_KEY_PRESS_UP, SP1K_MSG_KEY_REPEATE_UP, SP1K_MSG_KEY_RELEASE_UP},
    {SP1K_MSG_KEY_PRESS_TELE, SP1K_MSG_KEY_REPEATE_TELE, SP1K_MSG_KEY_RELEASE_TELE},
    {SP1K_MSG_KEY_PRESS_WIDE, SP1K_MSG_KEY_REPEATE_WIDE, SP1K_MSG_KEY_RELEASE_WIDE},

    /*ADC B button msg*/
    {SP1K_MSG_KEY_PRESS_MENU, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_MENU},
    {SP1K_MSG_KEY_PRESS_PB, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_PB},
    //{SP1K_MSG_KEY_PRESS_F1,		SP1K_MSG_NULL,					SP1K_MSG_KEY_RELEASE_F1},
    {SP1K_MSG_KEY_PRESS_OK, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_OK},
    {SP1K_MSG_KEY_PRESS_POWER, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_POWER},

    /*GPIO button msg*/
    {SP1K_MSG_KEY_PRESS_S2, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_S2},
#elif (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)

    /*ADC A button msg*/
    {SP1K_MSG_KEY_PRESS_RIGHT, SP1K_MSG_KEY_REPEATE_RIGHT, SP1K_MSG_KEY_RELEASE_RIGHT},
    {SP1K_MSG_KEY_PRESS_LEFT, SP1K_MSG_KEY_REPEATE_LEFT, SP1K_MSG_KEY_RELEASE_LEFT},
    {SP1K_MSG_KEY_PRESS_DOWN, SP1K_MSG_KEY_REPEATE_DOWN, SP1K_MSG_KEY_RELEASE_DOWN},
    {SP1K_MSG_KEY_PRESS_UP, SP1K_MSG_KEY_REPEATE_UP, SP1K_MSG_KEY_RELEASE_UP},

    /*ADC B button msg*/
    {SP1K_MSG_KEY_PRESS_MENU, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_MENU},
    {SP1K_MSG_KEY_PRESS_PB, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_PB},
    /*GPIO button msg*/

    {SP1K_MSG_KEY_PRESS_POWER, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_POWER},
    {SP1K_MSG_KEY_PRESS_OK, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_OK},
#elif (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_)
    /*ADC A button msg*/
    {SP1K_MSG_KEY_PRESS_RIGHT, SP1K_MSG_KEY_REPEATE_RIGHT, SP1K_MSG_KEY_RELEASE_RIGHT},
    {SP1K_MSG_KEY_PRESS_LEFT, SP1K_MSG_KEY_REPEATE_LEFT, SP1K_MSG_KEY_RELEASE_LEFT},
    {SP1K_MSG_KEY_PRESS_DOWN, SP1K_MSG_KEY_REPEATE_DOWN, SP1K_MSG_KEY_RELEASE_DOWN},
    {SP1K_MSG_KEY_PRESS_UP, SP1K_MSG_KEY_REPEATE_UP, SP1K_MSG_KEY_RELEASE_UP},

    /*ADC B button msg*/
    {SP1K_MSG_KEY_PRESS_MENU, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_MENU}, // menu
    {SP1K_MSG_KEY_PRESS_F1, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_F1},     // mode
    {SP1K_MSG_KEY_PRESS_PB, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_PB},     // PB
    /*GPIO button msg*/

    {SP1K_MSG_KEY_PRESS_POWER, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_POWER},
    {SP1K_MSG_KEY_PRESS_OK, SP1K_MSG_NULL, SP1K_MSG_KEY_RELEASE_OK},

#endif // hw solution
};

/*-------------------- USER CODE: Custom End --------------------*/

/* Private function prototypes===============================================*/

static void keyScanAdcParsing(keyUnitAdc_t *pAdc);
static UINT16 adc_filter_noise(UINT16 value, UINT8 channel);
static UINT8 keyScanAdcIdleChk(UINT16 AdcValue, keyUnitAdc_t *pAdc);

static void battAdcParsing(battUnitAdc_t *pAdc);
static UINT8 keyScanAdcValParsing(UINT16 AdcValue, keyUnitAdc_t *pAdc, UINT8 *AdcIdx);

static void keyScanDgpioValParsing(keyUnitAdc_t *pAdc);
static void keyScanGpioParsing(keyUnitGpio_t *pGpio);
static void keyScanGpioValParsing(UINT8 gpioVal, UINT8 gpioChn, keyUnitGpio_t *pGpio, UINT8 gpioPrsFlag);
void keyScanTpParsing(void);
// static powerBatLv_t xdata BatteryLevel = 0;

static scanTag_t xdata sTag;
static UINT8 xdata sKeyEn[SP1K_BTN_NUM];

extern UINT8 TestFlag;
#if USE_PKG_DEDICATE_PIN
static UINT8 gpioByteIdS2, gpioBitMskS2, gpiopolarityS2;
extern void pkgDedicateGpioCfg(UINT8 pinId, UINT8 *pbyteId, UINT8 *pbitMsk, UINT8 *ppolarity, UINT8 dir);
#endif
extern UINT8 HAL_GlobalDGPIOModeGet(void);


#if SUPPORT_PANEL_PROTECT   // 【支持屏幕保护】
extern xdata UINT16 G_AutoOffPanelTime;
#endif

extern UINT8 af_timer_start;
extern UINT8 af_timer_out;

extern UINT8 video_af_timer_start;
extern UINT8 video_af_timer_out;

extern UINT8 video_play_timer_start;
extern UINT8 video_play_timer_out;

#if SUPPORT_AWB_ADJ // 【定时检测亮度是否发生变化，如果发生变化则打开AWB，否则关闭AWB】
extern UINT8 awb_auto_disable_flag;
extern UINT8 awb_adj_start_flag;
#endif


/**
* @param    None
* @retval   None
* @brief    按键检测(10ms一次) -- 2010-5-18
**/
void keyScan(void)
{
    static UINT8 scanTag = 0;  // 定义静态变量scanTag，用于标记扫描状态
    static UINT8 uiOSDTime = 0;  // 定义静态变量uiOSDTime，用于显示OSD时间

#if SUPPORT_PANEL_PROTECT   // 【支持屏幕保护】
    static UINT8 ucPanel100MsDleay = 100;  // 定义静态变量ucPanel100MsDleay，用于面板保护延时
    static UINT8 PanelTime = 100;  // 定义静态变量PanelTime，用于面板保护时间
#endif

    uiPara_t *puiPara;  // 定义指向uiPara_t结构体的指针puiPara

#if SUPPORT_MCU_WTD // 【支持MCU做看门狗】
    static UINT16 heartbeatSendDelay3Sec = 1;  // 定义静态变量heartbeatSendDelay3Sec，用于心跳发送延时
    static UINT8 highLowStatus = 0;  // 定义静态变量highLowStatus，用于高低状态切换
#endif

    static UINT16 afbackfar = 500;  // 定义静态变量afbackfar，用于后退远离自动对焦
    static UINT16 versionclear = 500;  // 定义静态变量versionclear，用于版本清除
    static UINT16 af_timer_cnt = AF_AUTO_FOCUS_TIMER;  // 定义静态变量af_timer_cnt，用于自动对焦计时器
    static UINT16 video_af_timer_cnt = 100;  // 定义静态变量video_af_timer_cnt，用于视频自动对焦计时器
    static UINT16 video_play_timer_cnt = 100;  // 定义静态变量video_play_timer_cnt，用于视频播放计时器

#if SUPPORT_AWB_ADJ // 【定时检测亮度是否发生变化，如果发生变化则打开AWB，否则关闭AWB】
    static UINT16 awb_timer_cnt = 500;  // 定义静态变量awb_timer_cnt，用于AWB调节计时器
    static UINT16 awb_auto_off_cnt = 100;  // 定义静态变量awb_auto_off_cnt，用于AWB自动关闭计时器
#endif

    puiPara = appUiParaGet();  // 获取UI参数

#if SUPPORT_PANEL_PROTECT   // 【支持屏幕保护】
    // 面板保护逻辑(获取视频回放状态)
    if (G_AutoOffPanelTime != 0xffff && sp1kVideoPbStatusGet() == VID_PB_IDLE)
    {
        if (G_AutoOffPanelTime)
        {
            if (PanelTime)
                PanelTime--;
            else
            {
                PanelTime = 100;
                G_AutoOffPanelTime--;
            }
        }
    }
    else
    {
        PanelTime = 100;
    }
#endif

#if (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_)
    // 特定硬件解决方案逻辑
    static UINT16 KeyFlagCount = 0;

    if (keyFlag == 0)
    {
        KeyFlagCount++;
        if (KeyFlagCount == 500)
        {
            keyFlag = 1;
            KeyFlagCount = 0;
            osMsgPost(SP1K_MSG_CLEAN_ZOOM_OSD);  // 发送消息清除放大OSD
        }
    }
    else
    {
        KeyFlagCount = 0;
    }
#endif

#if KEY_VALUE_AVEAGE
    keepChannelFlag = 0;  // 初始化通道保持标志
#endif

    switch (scanTag)
    {
    case 0:
    {
        // #if (_HW_SOLUTION_ != _HW_EVB_)
        battAdcParsing(&sTag.battAdc);  // 电池ADC解析
        // #endif

        // printf("->0\n");
        sp1kADCSarSel(0x00);  // 选择ADC SAR
        scanTag = 0x01;  // 切换扫描标记到1
        break;
    }
    case 1:
    {
        if (flag1627 != 0)
        {
            // keyScanDgpioValParsing(&sTag.keyAdcA);
        }
        else
        {
            // keyScanAdcParsing(&sTag.keyAdcA);
        }
        keyScanAdcParsing(&sTag.keyAdcA);  // 解析按键ADC值

#if KEY_VALUE_AVEAGE
        if (keepChannelFlag)
        {
            return;  // 如果需要保持通道，则返回
        }
#endif

        // printf("->1\n");
        sp1kADCSarSel(0x02);  // 选择ADC SAR
        scanTag = 0x02;  // 切换扫描标记到2
        break;
    }
    case 2:
    {
        keyScanAdcParsing(&sTag.keyAdcB);  // 解析按键ADC值
        // printf("->2\n");

#if KEY_VALUE_AVEAGE
        if (keepChannelFlag)
        {
            return;  // 如果需要保持通道，则返回
        }
#endif

        sp1kADCSarSel(0x00);  // 选择ADC SAR
        scanTag = 0x00;  // 切换扫描标记到0
        break;
    }
    default:
        break;
    }

    keyScanGpioParsing(&sTag.keyGpio);  // 解析按键GPIO

#if FUNC_HOST_TOUCH_PANEL   // 【支持触摸面板的功能】
    keyScanTpParsing();  // 解析触摸面板
#endif

#if SUPPORT_PANEL_PROTECT   // 【支持屏幕保护】
    // 面板保护定时任务
    if (--ucPanel100MsDleay == 0)
    {
        ucPanel100MsDleay = 100;
        osMsgPost(SP1K_MSG_PANEL_PROTECT);  // 发送面板保护消息
    }
#endif

#if SUPPORT_RTC_OSD_UPDATE  // 【支持在预览界面显示当前时间】
    // RTC OSD更新定时任务
    if ((++uiOSDTime >= 100) && (puiPara->StampMode == PRV_STAMP_DATETIME) && ((appCurrStateGet() == APP_STATE_STILL_VIEW) || (appCurrStateGet() == APP_STATE_VIDEO_VIEW)))
    {
        uiOSDTime = 0;
        osMsgPost(SP1K_MSG_UPDATE_RTCTIME);  // 发送更新RTC时间消息
    }
#endif

#if SUPPORT_MCU_WTD // 【支持MCU做看门狗】
    // MCU看门狗定时任务
    if (--heartbeatSendDelay3Sec == 0)
    {
        heartbeatSendDelay3Sec = 50; // 200;
        appSetWTDHeartbeat(highLowStatus);
        highLowStatus = !highLowStatus;
    }
#endif

    // 版本清除逻辑
    if (versionclearflag)
    {
        if (--versionclear == 0)
        {
            versionclearflag = 0;
            versionclear = 500;

            osMsgPost(SP1K_MSG_Version_Clear);  // 发送清除版本消息
        }
    }
    else
    {
        versionclear = 500;
    }

    // 自动对焦后退远离逻辑
    if (affarflag && !selfsnapbackfarflag)
    {
        if (--afbackfar == 0)
        {
            selfsnapbackfarflag = 0;
            affarflag = 0;
            afbackfar = 500;

            osMsgPost(SP1K_MSG_SYS_AF_BACK_FAR);  // 发送系统自动对焦后退消息
        }
    }
    else
    {
        afbackfar = 500;
    }

    // 自动对焦计时器逻辑
    if (af_timer_start)
    {
        if (--af_timer_cnt == 0)
        {
            af_timer_start = Flag_false;
            af_timer_cnt = AF_AUTO_FOCUS_TIMER;
            af_timer_out = Flag_true;

            osMsgPost(SP1K_MSG_KEY_PRESS_AF);  // 发送按键自动对焦消息
        }
    }
    else
    {
        af_timer_cnt = AF_AUTO_FOCUS_TIMER;
    }

    // 视频自动对焦计时器逻辑
    if (video_af_timer_start)
    {
        if (--video_af_timer_cnt == 0)
        {
            video_af_timer_start = 0;
            video_af_timer_cnt = 100;
            video_af_timer_out = 1;

            // osMsgPost(SP1K_MSG_KEY_PRESS_AF);
        }
    }
    else
    {
        video_af_timer_cnt = 100;
    }

    // 视频播放计时器逻辑
    if (video_play_timer_start)
    {
        if (--video_play_timer_cnt == 0)
        {
            video_play_timer_start = 0;
            video_play_timer_cnt = 100;
            video_play_timer_out = 1;

            // osMsgPost(SP1K_MSG_KEY_PRESS_AF);
        }
    }
    else
    {
        video_play_timer_cnt = 100;
    }

#if SUPPORT_AWB_ADJ // 【定时检测亮度是否发生变化，如果发生变化则打开AWB，否则关闭AWB】
    // AWB调节定时任务
    if (awb_adj_start_flag)
    {
        if (--awb_timer_cnt == 0)
        {
            awb_timer_cnt = 500;
            osMsgPost(SP1K_MSG_AWB_CHECK);  // 发送AWB检查消息
        }
    }
    else
    {
        awb_timer_cnt = 500;
    }

    // AWB自动关闭逻辑
    if (awb_auto_disable_flag)
    {
        if (--awb_auto_off_cnt == 0)
        {
            awb_auto_disable_flag = 0;
            awb_auto_off_cnt = 100;
            osMsgPost(SP1K_MSG_AWB_OFF);  // 发送AWB关闭消息
        }
    }
    else
    {
        awb_auto_off_cnt = 100;
    }
#endif
}

/**
* @param    gpioVal：表示 GPIO 值
* @param    gpioChn：表示 GPIO 通道
* @param    *pGpio：指向 keyUnitGpio_t 结构体的指针
* @param    gpioPrsFlag：GPIO 解析标志位(即按下的电平是什么就写什么)
* @retval   None
* @brief    处理按键扫描过程中的GPIO数值 --- 2008-02-20
**/
static void keyScanGpioValParsing(UINT8 gpioVal, UINT8 gpioChn, keyUnitGpio_t *pGpio, UINT8 gpioPrsFlag)
{
    UINT16 gpioMsg = SP1K_MSG_NULL; // 初始化按键消息为NULL
    UINT8 *gpioPreSts = &pGpio->keyAttr[gpioChn].keyStsPrev; // 指向按键前一个状态的指针

    /*----------电平不是按下电平---------------*/
    if (gpioVal != gpioPrsFlag) // 如果按键状态改变
    {
        if (*gpioPreSts == KEY_STATUS_RELEASED) // 如果前一个状态是释放状态
        {
            // TODO : idle -> idle : No key action(即无按键动作)
            return; // 无需执行任何操作，直接返回
        }
        else // 前一个状态不是释放状态
        {
            // TODO : Active -> idle : Released(释放操作)
            *gpioPreSts = KEY_STATUS_RELEASED; // 设置当前按键状态为释放状态
            if (*pGpio->keyAttr[gpioChn].pKeyEn & KEY_STATUS_RELEASED) // 如果允许释放状态触发按键事件
            {
                pGpio->keyCtrl.keyValRpt = KEY_SCAN_GPIO_VAL_RPT; // 设置按键重复次数
                gpioMsg = pGpio->pKeyMsg[gpioChn].keyMsgRls; // 设置为对应的按键释放消息

#if SUPPORT_PANEL_PROTECT   // 【支持屏幕保护】
                if (!app_get_lcd_backlight_status()) // 如果LCD背光未开启
                {
                    gpioMsg = SP1K_MSG_SYS_OPEN_LCD_BACKLIGHT; // 设置消息为打开屏幕背光消息
                }
#endif
            }
        }
    }
    /*----------电平是按下电平，而且上一个状态是释放状态---------------*/
    else if (*gpioPreSts == KEY_STATUS_RELEASED) // 如果前一个状态是释放状态
    {
        // TODO : idle -> Active : Pressed(按下操作)
        *gpioPreSts = KEY_STATUS_PRESSED; // 设置当前按键状态为按下状态
        if (*pGpio->keyAttr[gpioChn].pKeyEn & KEY_STATUS_PRESSED) // 如果允许按下状态触发按键事件
        {
            pGpio->keyCtrl.keyValRpt = KEY_SCAN_GPIO_VAL_RPT_START; // 设置按键长按次数起始值
            gpioMsg = pGpio->pKeyMsg[gpioChn].keyMsgPrs; // 设置为对应的按键按下消息
#if SUPPORT_PANEL_PROTECT   // 【支持屏幕保护】
            if (!app_get_lcd_backlight_status()) // 如果LCD背光未开启
            {
                gpioMsg = SP1K_MSG_NULL; // 设置消息为空
            }
#endif
        }
    }
    /*----------电平是按下电平，上一个状态是按下状态---------------*/
    else if (*pGpio->keyAttr[gpioChn].pKeyEn & KEY_STATUS_REPEAT) // 按键使能
    {
        // TODO : Active -> Active : Repeated(长按操作)
        if (0 == --(pGpio->keyCtrl.keyValRpt)) // 如果按键长按计数0
        {
            pGpio->keyCtrl.keyValRpt = KEY_SCAN_GPIO_VAL_RPT; // 重置按键长按计数值
            gpioMsg = pGpio->pKeyMsg[gpioChn].keyMsgRpt; // 设置为对应的按键长按消息
#if SUPPORT_PANEL_PROTECT   // 【支持屏幕保护】
            if (!app_get_lcd_backlight_status()) // 如果LCD背光未开启
            {
                if (gpioMsg != SP1K_MSG_KEY_REPEATE_POWER) // 如果消息不是长按电源键
                {
                    gpioMsg = SP1K_MSG_NULL; // 设置消息为空
                }
            }
#endif
        }
    }

    if (gpioMsg != SP1K_MSG_NULL) // 如果有需要发送的按键消息
    {
        osMsgPost(gpioMsg); // 发送按键消息

#if SUPPORT_PANEL_PROTECT   // 【支持屏幕保护】
        if (app_get_lcd_backlight_status()) // 如果LCD背光已开启
        {
            osMsgPost(SP1K_MSG_SYS_OPEN_LCD_BACKLIGHT); // 发送打开LCD背光的消息
        }
#endif
    }
}

/**
* @param    pGpio: 指向keyUnitGpio_t结构的指针
* @retval   None
* @brief    用于扫描特定的GPIO引脚
**/
static void keyScanGpioParsing(keyUnitGpio_t *pGpio)
{
    UINT8 gpioVal, curKeyVal;
    UINT8 gpioChn;

#if (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_ || _HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_)

    // 处理自定义型号A和B的硬件方案
    gpioChn = 0; // 电源键

    if (*pGpio->keyAttr[gpioChn].pKeyEn)
    {
        HAL_GpioByteDirSet_Intr(HAL_GPIO_BYTE_MA1, 0x40, 0x00); // 将MA[14]设置为输入。
        gpioVal = HAL_GpioByteInGet(HAL_GPIO_BYTE_MA1, 0x40, 1);

        keyScanGpioValParsing(gpioVal, gpioChn, pGpio, 0x40);
    }

    gpioChn = 1; // S2键
    if (*pGpio->keyAttr[gpioChn].pKeyEn)
    {
        gpioVal = gpioByteInGet(gpioByteIdS2, gpioBitMskS2, 1);
        keyScanGpioValParsing(gpioVal, gpioChn, pGpio, gpioBitMskS2);
    }

#elif (_HW_SOLUTION_ == _HW_EVB_ || _HW_SOLUTION_ == _HW_TK_MODEL_A_)     // 【当前进入这】

    // 处理EVB和TK型号A的硬件方案
    /* GPIO扫描 */
#if 0
    gpioChn = 0; // 接收GPIO 9
    if (*pGpio->keyAttr[gpioChn].pKeyEn) {
        HAL_GpioByteDirSet_Intr(HAL_GPIO_BYTE_GEN1, 0x02, 0x00);
        gpioVal = HAL_GpioByteInGet(HAL_GPIO_BYTE_GEN1, 0x02, 1);
        keyScanGpioValParsing(gpioVal, gpioChn, pGpio, 0x02);
    }
#endif

    /* DGPIO扫描 */

    if (flag1627 == 2)  // 通过硬件版本选择对应硬件接口来进行读取IO电平
    {
        gpioVal = G_DgpioValue;
    }
    else if (flag1627 == 1)
    {
        gpioVal = XBYTE[REG_Disp_Rsv3];
    }

/*-------------------- USER CODE: Custom Begin --------------------*/

    gpioChn = 0; // 读取W1，即DGPIO1
    curKeyVal = (gpioVal & GPIO_PIN_2);
    if (*pGpio->keyAttr[gpioChn].pKeyEn)    // 判断按键是否使能
    {
        keyScanGpioValParsing(curKeyVal, gpioChn, pGpio, 0x00);
    }

    gpioChn = 1; // 读取SHUTTER，即DGPIO2
    curKeyVal = (gpioVal & GPIO_PIN_3);
    if (*pGpio->keyAttr[gpioChn].pKeyEn)
    {
        // 参数4是按下的电平状态，具体看原理图
        keyScanGpioValParsing(curKeyVal, gpioChn, pGpio, 0x00);
    }

    gpioChn = 2; // 读取T1，即DGPIO3
    curKeyVal = (gpioVal & GPIO_PIN_4);
    if (*pGpio->keyAttr[gpioChn].pKeyEn)
    {
        keyScanGpioValParsing(curKeyVal, gpioChn, pGpio, 0x00);
    }

    /* GPIO扫描 */
    gpioChn = 3; // 读取AF，即GPIO8
    if (*pGpio->keyAttr[gpioChn].pKeyEn)
    {
        /* gpio 8 */
        sp1kPullEnSet(5, 0x02, 0x02);   // 使能内部上下拉 --- gpio功能
        sp1kPullSelSet(3, 0x20, 0x20);  // 使能内部上拉
        HAL_GpioByteDirSet_Intr(HAL_GPIO_BYTE_GEN1, GPIO_PIN_1, 0); // GPIO中断 --- 将GPIO 设置为输入
        gpioVal = gpioByteInGet(HAL_GPIO_BYTE_GEN1, GPIO_PIN_1, 1); // 获取电平输入状态

        keyScanGpioValParsing(gpioVal, gpioChn, pGpio, 0x00);
    }

/*-------------------- USER CODE: Custom End --------------------*/

#else

    // 其他硬件方案的默认情况
    gpioChn = 0; // S2键

    if (*pGpio->keyAttr[gpioChn].pKeyEn)
    {
        gpioVal = gpioByteInGet(gpioByteIdS2, gpioBitMskS2, 1);
        keyScanGpioValParsing(gpioVal, gpioChn, pGpio, 0x00);
    }

#endif
}

/**
* @param    None
* @retval   None
* @brief    初始化按键扫描模块 -- 2008-02-20
**/
void keyScanInit(void)
{
    scanTag_t *pTag; // 扫描标签指针
    UINT8 i;

    pTag = &sTag;

    /*判断IC版本进行选择对应的硬件接口*/
    if (((pkgChipIdGet() == 0x1627) || (pkgChipIdGet() == 0x1626)) && (HAL_GlobalDGPIOModeGet() != 0x01))
    {
        XBYTE[REG_Disp_VsVdIntEn] |= 0x02;
        G_DgpioValue = 0;
        flag1627 = 2;
    }
    else if (((pkgChipIdGet() == 0x1627) || (pkgChipIdGet() == 0x1626)) && (HAL_GlobalDGPIOModeGet() == 0x01))
    {
        XBYTE[REG_Disp_Rsv0] |= 0x04;
        flag1627 = 1;
    }

    for (i = 0; i < SP1K_ADC_BTN_NUM; i++)
    {
        sKeyEn[i] = 0x00; // 将默认按键状态设置为禁用
    }
    /*按键A 单元*/
    pTag->keyAdcA.keyActiveIdx = (SP1K_ADC_BTN_A_NUM - 1);
    pTag->keyAdcA.keyAttr.pKeyEn = &sKeyEn[0];
    pTag->keyAdcA.keyAttr.keyNum = SP1K_ADC_BTN_A_NUM;
    pTag->keyAdcA.keyAttr.keyType = KEY_TYPE_ADC_A;
    pTag->keyAdcA.keyCtrl.keyDec = KEY_SCAN_ADC_VAL_DEC;
    pTag->keyAdcA.keyCtrl.keyValRpt = 0;  // KEY_SCAN_ADC_VAL_RPT_START;
    pTag->keyAdcA.keyCtrl.keyValPrev = 0; // KEY_SCAN_ADC_B_VAL_IDLE;
    pTag->keyAdcA.keyCtrl.pKeyDev = &keyAdcAValDevTbl[0];
    pTag->keyAdcA.keyCtrl.pKeyVal = &keyAdcAVal[0];
    pTag->keyAdcA.pKeyMsg = &keyMsg[0];
    /*按键B 单元*/
    pTag->keyAdcB.keyActiveIdx = (SP1K_ADC_BTN_B_NUM - 1);
    pTag->keyAdcB.keyAttr.pKeyEn = &sKeyEn[0 + SP1K_ADC_BTN_A_NUM];
    pTag->keyAdcB.keyAttr.keyNum = SP1K_ADC_BTN_B_NUM;
    pTag->keyAdcB.keyAttr.keyType = KEY_TYPE_ADC_B;
    pTag->keyAdcB.keyCtrl.keyDec = KEY_SCAN_ADC_VAL_DEC;
    pTag->keyAdcB.keyCtrl.keyValRpt = 0;  // KEY_SCAN_ADC_VAL_RPT_START;
    pTag->keyAdcB.keyCtrl.keyValPrev = 0; // KEY_SCAN_ADC_B_VAL_IDLE;
    pTag->keyAdcB.keyCtrl.pKeyDev = &keyAdcBValDevTbl[0];
    pTag->keyAdcB.keyCtrl.pKeyVal = &keyAdcBVal[0];
    pTag->keyAdcB.pKeyMsg = &keyMsg[0 + SP1K_ADC_BTN_A_NUM];
    /*按键 GPIO 单元*/
    for (i = 0; i < SP1K_GPIO_BTN_NUM; i++)
    {
        pTag->keyGpio.keyAttr[i].pKeyEn = &sKeyEn[0 + SP1K_ADC_BTN_NUM];
        pTag->keyGpio.keyAttr[i].keyStsPrev = KEY_STATUS_RELEASED;
    }
    pTag->keyGpio.keyCtrl.keyDec = 1;
    pTag->keyGpio.keyCtrl.keyValRpt = KEY_SCAN_GPIO_VAL_RPT_START;
    pTag->keyGpio.pKeyMsg = &keyMsg[0 + SP1K_ADC_BTN_NUM];

#if USE_PKG_DEDICATE_PIN
#if (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_ || _HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_)
    gpioPullSelSet(4, 0x02, 0x00); // GPIO12 拉低
    gpioPullEnSet(5, 0x20, 0xFF);
#else
    if (pkgChipIdGet() == 0x1626)
    {
        // gpioPullSelSet(3,0x40,0x00);
        // gpioPullEnSet(5,0x04,0x00);//GPIO9 拉高失效

        // gpioPullSelSet(2,0x40,0x40);
        // gpioPullEnSet(4,0x04,0x04);//GPIO1 拉高

        // gpioPullSelSet(2,0x80,0x00);
        // gpioPullEnSet(4,0x08,0x00);//GPIO2 拉高
    }
    else
    {
        // gpioPullSelSet(3,0x40,0x40);
        // gpioPullEnSet(5,0x04,0x04);//拉高
    }
#endif
    pkgDedicateGpioCfg(PKG_DEDICATE_PIN_S2, &gpioByteIdS2, &gpioBitMskS2, &gpiopolarityS2, 0);
    HAL_GpioByteFuncSet(gpioByteIdS2, gpioBitMskS2, 0xFF);
#endif
}

/**
* @param    battLvlTol: 电池电量容差
* @param    pBattAdcVal: 指向电池ADC值的指针
* @param    battAdvValDev: 电池高级值偏差
* @retval   None
* @brief    配置电池检测参数 -- 2008-07-01
**/
UINT8 battDetectCfg(UINT8 battLvlTol, UINT16 *pBattAdcVal, UINT16 battAdvValDev)
{
    scanTag_t *pTag;

    /* 参数检查 */
    if (!battLvlTol || pBattAdcVal == NULL)
    {
        return FAIL;
    }

    // 获取scanTag结构体的指针
    pTag = &sTag;

    // 设置电池ADC检测参数
    pTag->battAdc.battLvlTol = battLvlTol;
    pTag->battAdc.pbattAdcVal = pBattAdcVal;
    pTag->battAdc.battAdcDev = battAdvValDev;

    return SUCCESS;
}

/**
* @param    None
* @retval   电池电量级别
* @brief    初始化电池检测 -- 2008-02-20
**/
UINT8 battDetectInit(void)
{
    scanTag_t *pTag;   // 定义指向scanTag结构体的指针
    UINT16 adcVal;     // 存储ADC采样值
    UINT8 i;           // 循环变量

    pTag = &sTag;  // 获取scanTag结构体的指针
    adcVal = sp1kADCValueSnap();  // 获取ADC值

#if (_HW_SOLUTION_ != _HW_EVB_)
    // 准备检查电源键
    sp1kADCSarSel(0x02);
#endif

    /* 默认为最低电量级别 */
    pTag->battAdc.battLvl = pTag->battAdc.battLvlTol;

    /* 扫描ADC值，如果匹配成功，设置对应电池电量级别 */
    for (i = 0; i < pTag->battAdc.battLvlTol; i++)
    {
        if (adcVal >= pTag->battAdc.pbattAdcVal[i])
        {
            pTag->battAdc.battLvl = i;
            break;
        }
    }

    pTag->battAdc.battDec = 0;          // 电池电量下降设为0
    pTag->battAdc.battDetectEn = 0;     // 电池检测使能设为0

    /* 如果USB插入，强制将电池设为满电量 */
    if (sp1kUSB_PlugGet() == USB_PLUG_IN)
    {
        // if (!gpioByteInGet(GPIO_BYTE_DISP0, 0x02, 1)) {
        pTag->battAdc.battLvl = 0;
    }

    // #if (_HW_SOLUTION_ == _HW_EVB_) //2008-7-22 mantis #28665
    // pTag->battAdc.battLvl = 0;
    // #endif

    return pTag->battAdc.battLvl;  // 返回电池电量级别
}

/**
* @param    enable: 启用 / 禁用
* @retval   None
* @brief    用于启用或禁用电池检测 -- 2008-02-20
**/
void sp1kBattEnable(UINT8 enable)
{
    scanTag_t *pTag;  // 定义指向scanTag结构体的指针

    pTag = &sTag;  // 获取scanTag结构体的指针

    if (enable)
    {
        pTag->battAdc.battDetectEn = 1;  // 如果启用，设置电池检测使能为1
    }
    else
    {
        pTag->battAdc.battDetectEn = 0;  // 如果禁用，设置电池检测使能为0
    }
}

/**
* @param    pAdc: 电池单元ADC结构体指针
* @retval   SUCCESS
* @brief    解析电池ADC数值并进行电池级别的更新
**/
static void battAdcParsing(battUnitAdc_t *pAdc)
{
    UINT16 adcVal; // ADC数值
    UINT8 battLv; // 电池级别
    static UINT16 pwrOffRetry = 1; // 关机重试次数
    UINT16 adcGap; // ADC间隔
    UINT8 i;
    UINT8 adcTot; // ADC总数

    static UINT8 pr_delay = 30; // 打印延迟

    if (!pAdc->battDetectEn)
    {
        return; // 如果未启用电池检测，直接返回
    }

    /*获取ADC数值*/
    adcVal = sp1kADCValueSnapIntr();

#if 0
	if (--pr_delay == 0) {
		pr_delay = 30;
		printf("batter val(%u)\n", adcVal);
	}
#endif

    if (adcVal >= 600)
    {
        return; // 如果ADC数值大于等于600，直接返回
    }

    adcGap = pAdc->battAdcDev;
    adcTot = pAdc->battLvlTol;

    battLv = -1;
    for (i = 0; i <= adcTot; i++)
    {
        if (!i)
        {
            /* 当i == 0 时 */
            if (adcVal >= pAdc->pbattAdcVal[i])
            {
                battLv = i;
                break;
            }
        }
        else if (i == adcTot)
        {
            /* 当扫描到最后一个数值时 */
            if (adcVal < pAdc->pbattAdcVal[i - 1] - adcGap)
            {
                battLv = adcTot;
                break;
            }
        }
        else
        {
            if (adcVal >= pAdc->pbattAdcVal[i] && adcVal < pAdc->pbattAdcVal[i - 1] - adcGap)
            {
                battLv = i;
                break;
            }
        }
    }

    if (battLv == -1)
    {
        /* ADC数值在间隔中 */
        // printf("gap!\n");
        return;
    }

    /* 如果电量较低，需要周期性发送SP1K_MSG_BAT_LV_UPDATE消息 */
    if (battLv == adcTot)
    {

        /* 防抖处理 */
        if (++pAdc->battDec < POWER_BAT_DEC)
        {
            return;
        }

        /* 电源关闭重试 */
        if (!(--pwrOffRetry))
        {

            pAdc->battLvl = battLv;
            if (pAdc->battDec)
            {
                /* 恢复battdec */
                --pAdc->battDec;
            }
            pwrOffRetry = BATT_LOW_POWER_OFF_RETRY;
            osMsgPost(SP1K_MSG_BAT_LV_UPDATE);
        }
    }
    else if (battLv != pAdc->battLvl)
    {
        /* 防抖处理 */
        if (++pAdc->battDec < POWER_BAT_DEC + 10)
        {
            return;
        }
        else
        {
            /* 电池级别发生变化 */
            pAdc->battDec = 0;
            pAdc->battLvl = battLv;
            // printf("->(%bu)\n", pAdc->battLvl);
            osMsgPost(SP1K_MSG_BAT_LV_UPDATE);
        }
    }
    else
    {
        /* 电池级别未发生变化 */
#ifdef DBG_KEYSCAN
        // #if 1
        if (pAdc->battDec)
        {
            // printf("dec(%bu)\n", pAdc->battDec);
            pAdc->battDec = 0;
        }
#else
        pAdc->battDec = 0;
#endif
    }
}

/**
* @param    AdcValue: ADC采样值
* @param    pAdc: 指向keyUnitAdc_t结构体的指针
* @retval   SUCCESS：ADC采样值符合空闲状态条件
* @brief    检查ADC采样值是否符合空闲状态条件的函数(波动过后类似消抖过后)
**/
static UINT8 keyScanAdcIdleChk(UINT16 AdcValue, keyUnitAdc_t *pAdc)
{
#if (_HW_SOLUTION_ == _HW_RDK_A_)
    // 如果硬件方案为RDK A，并且满足对应的ADC键盘类型及空闲状态条件，则返回TRUE
    if ((pAdc->keyAttr.keyType == KEY_TYPE_ADC_A && AdcValue <= KEY_SCAN_ADC_A_VAL_IDLE) ||
        (pAdc->keyAttr.keyType == KEY_TYPE_ADC_B && AdcValue <= KEY_SCAN_ADC_B_VAL_IDLE))
#elif (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)
    // 如果硬件方案为自定义模型A，并且满足对应的ADC键盘类型及空闲状态条件，则返回TRUE
    if ((pAdc->keyAttr.keyType == KEY_TYPE_ADC_A && AdcValue <= KEY_SCAN_ADC_A_VAL_IDLE) ||
        (pAdc->keyAttr.keyType == KEY_TYPE_ADC_B && AdcValue <= KEY_SCAN_ADC_B_VAL_IDLE))
#elif (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_)
    // 如果硬件方案为自定义模型B，并且满足对应的ADC键盘类型及空闲状态条件，则返回TRUE
    if ((pAdc->keyAttr.keyType == KEY_TYPE_ADC_A && AdcValue <= KEY_SCAN_ADC_A_VAL_IDLE) ||
        (pAdc->keyAttr.keyType == KEY_TYPE_ADC_B && AdcValue <= KEY_SCAN_ADC_B_VAL_IDLE))
#elif (_HW_SOLUTION_ == _HW_EVB_ || _HW_SOLUTION_ == _HW_TK_MODEL_A_)
    // 如果硬件方案为EVB或TK模型A，并且满足对应的ADC键盘类型及空闲状态条件，则返回TRUE
    if ((pAdc->keyAttr.keyType == KEY_TYPE_ADC_A && AdcValue >= KEY_SCAN_ADC_A_VAL_IDLE) ||
        (pAdc->keyAttr.keyType == KEY_TYPE_ADC_B && AdcValue >= KEY_SCAN_ADC_B_VAL_IDLE))
#elif (_HW_SOLUTION_ == _HW_TK_MODEL_B_)
    // 如果硬件方案为TK模型B，并且满足对应的ADC键盘类型及空闲状态条件，则返回TRUE
    if ((pAdc->keyAttr.keyType == KEY_TYPE_ADC_A && AdcValue >= KEY_SCAN_ADC_A_VAL_IDLE) ||
        (pAdc->keyAttr.keyType == KEY_TYPE_ADC_B && AdcValue <= KEY_SCAN_ADC_B_VAL_IDLE))
#endif
    {
        return TRUE;  // 返回TRUE，表示ADC采样值符合空闲状态条件
    }
    else
    {
        return FALSE;  // 返回FALSE，表示ADC采样值不符合空闲状态条件
    }
}

/*------------------------- 其他 -----------------------------*/

UINT8 adca_sample_count = 0; // adca通道采样计数器
UINT8 adcb_sample_count = 0; // adcb通道采样计数器
UINT16 adca_sample[2]; // adca通道采样数组
UINT16 adcb_sample[2]; // adcb通道采样数组

#define ADC_SAMPLE_ERR_LIMIT 53 // ADC采样两次之间误差限制最大值
#define ADC_DIFF(a, b) (a > b ? a - b : b - a) // 计算两个数值的差值
/**
* @param    value: 获取的ADC数值
* @param    channel: 通道号(@keyType_t)
* @retval   滤波后的ADC数值，如果发生采样错误则返回-1
* @brief    ADC值滤波(通过对连续两次采样值进行比较，并计算差值，如果差值大于预设的阈值ADC_SAMPLE_ERR_LIMIT，则认为采样出现错误，否则取两次采样值的平均值作为滤波后的结果)
**/
static UINT16 adc_filter_noise(UINT16 value, UINT8 channel)
{
    UINT16 diff; // 两次采样值的差值

    if (KEY_TYPE_ADC_A == channel) // 如果是adca通道
    {
        adca_sample[adca_sample_count] = value; // 存储采样值到数组中
        adca_sample_count++; // 采样计数器加1

        if (adca_sample_count >= 2) // 如果采样计数器达到2
        {
            adca_sample_count = 0; // 重置采样计数器

            diff = ADC_DIFF(adca_sample[0], adca_sample[1]); // 计算两次采样值的差值
            if (diff > ADC_SAMPLE_ERR_LIMIT) // 如果差值大于误差限制
            {
                return -1; // 返回-1表示采样出错
            }
            // 返回两次采样值的平均值作为滤波后的结果(>>1则等价于/2)
            return (adca_sample[0] + adca_sample[1]) >> 1;
        }
        else
        {
            return -1; // 返回-1表示采样未完成
        }
    }
    else if (KEY_TYPE_ADC_B == channel) // 如果是adcb通道
    {
        adcb_sample[adcb_sample_count] = value; // 存储采样值到数组中
        adcb_sample_count++; // 采样计数器加1

        if (adcb_sample_count >= 2) // 如果采样计数器达到2
        {
            adcb_sample_count = 0; // 重置采样计数器

            diff = ADC_DIFF(adcb_sample[0], adcb_sample[1]); // 计算两次采样值的差值
            if (diff > ADC_SAMPLE_ERR_LIMIT) // 如果差值大于误差限制
            {
                return -1; // 返回-1表示采样出错
            }
            // 返回两次采样值的平均值作为滤波后的结果(>>1则等价于/2)
            return (adcb_sample[0] + adcb_sample[1]) >> 1;
        }
        else
        {
            return -1; // 返回-1表示采样未完成
        }
    }
}

/*------------------------- 按键扫描 -----------------------------*/

/**
* @param    pAdc: 指向keyUnitAdc_t结构体的指针
* @retval   SUCCESS
* @brief    ADC按键扫描函数，根据采样值判断按键的状态（按下、释放、长按），并通过消息机制发送相应的消息
**/
static void keyScanAdcParsing(keyUnitAdc_t *pAdc)
{
    UINT16 adcVal;              // ADC采样值
    UINT16 msg = SP1K_MSG_NULL; // 默认是空消息

    static UINT8 ucCountb = 5, ucCounta = 20; // 静态计数器

    adcVal = sp1kADCValueSnapIntr(); // 捕捉刚在中断中使用的 ADC 采样值

#if 1   // 是否使用滤波
    adcVal = adc_filter_noise(adcVal, pAdc->keyAttr.keyType);   // 过滤ADC噪声
    if (0xFFFF == adcVal)   // 如果过滤后的采样值为0xFFFF，则返回
    {
        return;
    }
#endif

    if (KEY_TYPE_ADC_B == pAdc->keyAttr.keyType)    // 如果是ADC-B类型
    {
        adcVal = 1023 - adcVal; // 对采样值进行处理(1023是芯片的AD位数精度决定)

#if 0   // LOG调试
		if (--ucCountb == 0) 
        {
			ucCountb = 5;
			printf("adc bbbbb val : %d\n", adcVal);
		}
#endif
    }
    else
    {

#if 0   // LOG调试
		if (--ucCounta == 0) 
        {
			ucCounta = 20;
			printf("adc aaaaa val : %d\n", adcVal);
		}
#endif
    }

    /*------------------- AD按键长按操作 -----------------------*/
    // 如果采样值波动过后 && 长按值为真
    if ((keyScanAdcIdleChk(adcVal, pAdc) == TRUE) && pAdc->keyCtrl.keyValRpt)
    {
        // printf("press 2 adcval : %d\n", adcVal);
        if (pAdc->keyAttr.pKeyEn[pAdc->keyActiveIdx] & KEY_STATUS_REPEAT)   // 按键使能 + 长按状态
        {
            if (0 == --(pAdc->keyCtrl.keyValRpt))   // 递减键值长按值
            {
                pAdc->keyCtrl.keyValRpt = KEY_SCAN_ADC_VAL_RPT; // 重新设置键长按值

                if (pAdc->pKeyMsg[pAdc->keyActiveIdx].keyMsgRpt != SP1K_MSG_NULL)   // 如果长按按键消息不为空
                {
                    msg = pAdc->pKeyMsg[pAdc->keyActiveIdx].keyMsgRpt;  // 设置为按键长按消息

#if SUPPORT_PANEL_PROTECT   // 【支持屏幕保护】
                    if (!app_get_lcd_backlight_status())    // 如果LCD背光已关闭    
                    {
                        if (msg != SP1K_MSG_KEY_REPEATE_POWER)
                        {
                            msg = SP1K_MSG_NULL;    // 消息设置为空
                        }
                    }
#endif
                }

                goto EXIT;  // 跳转到EXIT标签
            }
        }
    }
    /*------------------- AD按键松开操作 -----------------------*/
    if (keyScanAdcIdleChk(pAdc->keyCtrl.keyValPrev, pAdc) == TRUE)  // 判断前一个值如果有按下
    {
        // printf("press 3 adcval : %d\n", adcVal);
        if (keyScanAdcIdleChk(adcVal, pAdc) == TRUE)    // 如果当前键值也是按下
        {
            // TODO: Idle -> Idle : No key action
            // 无操作
        }
        else    // 当前为松开状态
        {
            pAdc->keyCtrl.keyValRpt = 0; // 重置键值长按次数为0
             // 解析前一个键值
            if (keyScanAdcValParsing(pAdc->keyCtrl.keyValPrev, pAdc, &pAdc->keyActiveIdx) != SUCCESS)
            {
                // No matched adc value
                adcVal = 0;
                goto EXIT;  // 跳转到EXIT标签
            }

            // TODO: Active -> Idle : Key released【松开状态】
            if (pAdc->keyAttr.pKeyEn[pAdc->keyActiveIdx] & KEY_STATUS_RELEASED) // 如果该按键使能
            {
                msg = pAdc->pKeyMsg[pAdc->keyActiveIdx].keyMsgRls;  // 设置释放消息
                pAdc->keyActiveIdx = -1; // key idle    // 键盘空闲

#if SUPPORT_PANEL_PROTECT   // 【支持屏幕保护】
                if (!app_get_lcd_backlight_status())    // 如果LCD背光已关闭  
                {
                    msg = SP1K_MSG_SYS_OPEN_LCD_BACKLIGHT;
                }
#endif
            }
        }

        goto EXIT;
    }
    /*------------------- AD按键按下操作 -----------------------*/
    else if (keyScanAdcIdleChk(adcVal, pAdc) == TRUE)   // 按键波动过后(有按键按下)则
    {
        // printf("press 1 adcval : %d\n", adcVal);
        // TODO: Idle  -> Active : 【按键按下】

        if (keyScanAdcValParsing(adcVal, pAdc, &pAdc->keyActiveIdx) != SUCCESS)  // 解析当前键值
        {
            // 没有匹配的AD值
            adcVal = 0;
            goto EXIT;
        }
        // 按键已使能
        if (pAdc->keyAttr.pKeyEn[pAdc->keyActiveIdx] & KEY_STATUS_PRESSED)
        {
            pAdc->keyCtrl.keyValRpt = KEY_SCAN_ADC_VAL_RPT_START;   // 设置键值长按计数值起始值
            msg = pAdc->pKeyMsg[pAdc->keyActiveIdx].keyMsgPrs;  // 设置为按键按下消息

#if SUPPORT_PANEL_PROTECT   // 【支持屏幕保护】
            if (!app_get_lcd_backlight_status())     // 如果LCD背光已关闭  
            {
                msg = SP1K_MSG_NULL;
            }
#endif
        }
    }

EXIT:
    pAdc->keyCtrl.keyValPrev = adcVal;  // 更新前一个键值为当前键值

    if (msg != SP1K_MSG_NULL)   // 如果消息不为空
    {
        // 判断消息是否在释放消息范围内 是则
        if ((msg < SP1K_MSG_KEY_RELEASE_MAX) && (msg > SP1K_MSG_KEY_PRESS_MAX))
        {
            osMsgPostForce(msg);    // 强制发送消息
        }
        else    // 不是则
        {
            osMsgPost(msg); // 发送消息
        }
#if SUPPORT_PANEL_PROTECT   // 【支持屏幕保护】
        if (app_get_lcd_backlight_status())  // 如果LCD背光已关闭  
        {
            osMsgPost(SP1K_MSG_SYS_OPEN_LCD_BACKLIGHT);
        }
#endif
    }
}

/**
* @param    AdcValue: ADC采样值
* @param    NopAdc: 指向keyUnitAdc_t结构体的指针，表示ADC键盘的参数和状态ne
* @param    AdcIdx: 指向UINT8类型的指针，用于存储解析出的ADC索引值
* @retval   SUCCESS：成功解析出匹配的AD值 FAIL：解析失败
* @brief    解析ADC采样值是否匹配宏定义的AD值和误差值--2008-02-20
**/
static UINT8 keyScanAdcValParsing(UINT16 AdcValue, keyUnitAdc_t *pAdc, UINT8 *AdcIdx)
{
    UINT8 i;
    UINT8 adcNum = pAdc->keyAttr.keyNum;     // ADC键盘按键数量
    UINT16 *pAdcVal = &pAdc->keyCtrl.pKeyVal[0]; // 指向ADC键盘按键值数组的指针
    UINT16 *pAdcDev = &pAdc->keyCtrl.pKeyDev[0]; // 指向ADC键盘按键误差数组的指针

    if (pAdc == NULL || AdcIdx == NULL)  // 如果指针为空，则返回失败
    {
        return FAIL;
    }

    for (i = 0; i < adcNum; i++) // 遍历所有的ADC键盘按键
    {
        /*
            如果采样值小于按键值加上误差值则
            假设是OK-> 1023 < (1023 + 45)
        */
        if (AdcValue < (*pAdcVal + *pAdcDev)) 
        {
            if (((*pAdcVal > *pAdcDev) && (AdcValue > (*pAdcVal - *pAdcDev))) ||
                ((*pAdcVal <= *pAdcDev) && (AdcValue <= (*pAdcVal + *pAdcDev)))) // 判断采样值是否在误差范围内
            {
                // printf("idx(%bu)\n", i); // 打印索引值
                pAdc->keyActiveIdx = i; // 设置活动的索引值
                *AdcIdx = i; // 存储解析出的ADC索引值
                return SUCCESS; // 解析成功
            }
        }
        pAdcVal++; // 指针递增
        pAdcDev++; // 指针递增
    }

    return FAIL; // 解析失败
}



static UINT8 cnt = 0;
/**
* @param    pAdc: 指向keyUnitAdc_t结构体的指针
* @retval   None
* @brief    解析键盘值，并根据键盘值判断当前按下或释放的按键，并发送相应的消息(没用到)
**/
static void keyScanDgpioValParsing(keyUnitAdc_t* pAdc)
{
    UINT16 msg = SP1K_MSG_NULL;   // 消息变量，默认为NULL
    UINT8 curKeyVal = 0xFF;       // 当前按键值，默认为无效值
    UINT8 keyValue;               // 键盘值变量

    if (flag1627 == 2)
    {
        keyValue = G_DgpioValue;  // 获取键盘值
    }
    else if (flag1627 == 1)
    {
        keyValue = XBYTE[REG_Disp_Rsv3];  // 获取键盘值
    }

    // 经过一系列的判断和处理，最终获取curKeyVal的值
    if (((pkgChipIdGet() == 0x1627) || (pkgChipIdGet() == 0x1626)))  // 如果是特定芯片型号
    {
        keyValue >>= 2;  // 右移2位
        switch (keyValue & 0x3F)
        {
        case 0x3D:  // WIDE键
            curKeyVal = 5;
            break;
        case 0x3E:  // TELE键
            curKeyVal = 4;
            break;
        case 0x3B:  // UP键
            curKeyVal = 3;
            break;
        case 0x2F:  // LEFT键
            curKeyVal = 1;
            break;
        case 0x1F:  // RIGHT键
            curKeyVal = 0;
            break;
        case 0x37:  // DOWN键
            curKeyVal = 2;
            break;
        case 0x3F:  // 其他情况
        default:
            if (prevKeyVal < 3)  // 如果之前的按键值小于3
            {
                if (pAdc->keyAttr.pKeyEn[pAdc->keyActiveIdx] & KEY_STATUS_RELEASED)  // 如果按键状态为释放状态
                {
                    msg = pAdc->pKeyMsg[(UINT8)(pAdc->keyCtrl.keyValPrev)].keyMsgRls;  // 获取释放消息
                }
            }
            break;
        }
    }
    else  // 不是特定芯片型号
    {
        if (!(keyValue & 0x02))  // 如果键盘值的第二位为0
        {  // dgpio1 left键
            curKeyVal = 1;
        }
        else if (!(keyValue & 0x04))  // 如果键盘值的第三位为0
        {  // dgpio2 right键
            curKeyVal = 2;
        }
        else if (!(keyValue & 0x08))  // 如果键盘值的第四位为0
        {  // dgpio3 right键
            curKeyVal = 3;
        }
        else
        {
            if (prevKeyVal < 4)  // 如果之前的按键值小于4
            {
                if (pAdc->keyAttr.pKeyEn[pAdc->keyActiveIdx] & KEY_STATUS_RELEASED)  // 如果按键状态为释放状态
                {
                    msg = pAdc->pKeyMsg[(UINT8)(pAdc->keyCtrl.keyValPrev)].keyMsgRls;  // 获取释放消息
                }
            }
        }
    }

    if ((prevKeyVal == curKeyVal) && (curKeyVal != 0xFF))  // 如果之前的按键值等于当前按键值且当前按键值不为无效值
    {
        if (pAdc->keyAttr.pKeyEn[pAdc->keyActiveIdx] & KEY_STATUS_REPEAT)  // 如果按键状态为重复状态
        {
            if (--pAdc->keyCtrl.keyValRpt == 0)  // 如果重复计数减为0
            {
                pAdc->keyCtrl.keyValRpt = KEY_SCAN_ADC_VAL_RPT;  // 重置重复计数

                msg = pAdc->pKeyMsg[curKeyVal].keyMsgRpt;  // 获取重复消息

                // 如果LCD背光状态为关闭，则将消息置为NULL
#if SUPPORT_PANEL_PROTECT
                if (!app_get_lcd_backlight_status())
                {
                    if (msg != SP1K_MSG_KEY_REPEATE_POWER)
                    {
                        msg = SP1K_MSG_NULL;
                    }
                }
#endif
            }
        }
    }
    else if (curKeyVal != 0xff)  // 如果当前按键值不为无效值
    {
        if (pAdc->keyAttr.pKeyEn[pAdc->keyActiveIdx] & KEY_STATUS_PRESSED)  // 如果按键状态为按下状态
        {
            msg = pAdc->pKeyMsg[curKeyVal].keyMsgPrs;  // 获取按下消息
            pAdc->keyCtrl.keyValRpt = KEY_SCAN_ADC_VAL_RPT_START * 3;  // 设置重复计数

            // 如果LCD背光状态为关闭，则将消息置为NULL
#if SUPPORT_PANEL_PROTECT
            if (!app_get_lcd_backlight_status())
            {
                msg = SP1K_MSG_NULL;
            }
#endif
        }
    }

    prevKeyVal = curKeyVal;  // 更新之前的按键值为当前按键值
    pAdc->keyCtrl.keyValPrev = curKeyVal;  // 更新ADC键盘控制结构体中的之前的按键值

    if (msg != SP1K_MSG_NULL)  // 如果消息不为空
    {
        if ((msg < SP1K_MSG_KEY_RELEASE_MAX) && (msg > SP1K_MSG_KEY_PRESS_MAX))  // 如果消息为释放类消息
            osMsgPostForce(msg);  // 强制发送消息
        else
            osMsgPost(msg);  // 发送消息
    }
}

/**
* @param    BtnType: 按钮类型，可选值为 Adc A/ Adc B/ Gpio
* @param    BitMask: 按钮位掩码，用于指定要配置的按键
* @param    Enable:  可选值为 KEY_SWITCH_ENABLE KEY_SWITCH_DISABLE
* @retval   SUCCESS: 配置成功
* @brief    配置按钮的使能/失能
**/
UINT8 sp1kBtnCfg(keyType_t BtnType, UINT16 BitMask, UINT8 Enable)
{
    UINT8 keyNum;
    UINT8 *pKeyEn;
    UINT8 i;

    switch (BtnType)
    {
    case KEY_TYPE_ADC_A:
    {
        keyNum = SP1K_ADC_BTN_A_NUM; // Adc A 按钮数量
        pKeyEn = &sKeyEn[0];         // Adc A 按钮的使能数组起始地址
        break;
    }
    case KEY_TYPE_ADC_B:
    {
        keyNum = SP1K_ADC_BTN_B_NUM;              // Adc B 按钮数量
        pKeyEn = &sKeyEn[0 + SP1K_ADC_BTN_A_NUM]; // Adc B 按钮的使能数组起始地址
        break;
    }
    case KEY_TYPE_GPIO:
    {
        keyNum = SP1K_GPIO_BTN_NUM;             // Gpio 按钮数量
        pKeyEn = &sKeyEn[0 + SP1K_ADC_BTN_NUM]; // Gpio 按钮的使能数组起始地址
        break;
    }
    default:
        return FAIL; // 返回配置失败
    }

    for (i = 0; i < keyNum; i++)
    {
        if ((BitMask >> i) & 0x01)
        {
            *pKeyEn = Enable; // 配置指定按钮的使能状态
        }
        pKeyEn++;
    }

    return SUCCESS; // 返回配置成功
}

/**
* @param    BtnType: 按钮类型   
* @param    BitMask: 位掩码
* @param    Enable: 使能状态
* @retval   None
* @brief    对按钮的批量使能配置
**/
UINT8 sp1kBtnCfgIntr(keyType_t BtnType, UINT16 BitMask, UINT8 Enable)
{
    UINT8 keyNum;   // 按钮数量
    UINT8 *pKeyEn;  // 按钮使能数组指针
    UINT8 i;

    return SUCCESS;   // 直接返回成功，函数提前结束，不执行后续代码

    switch (BtnType)   // 根据按钮类型进行配置
    {
    case KEY_TYPE_ADC_A:    // Adc A 按钮
        keyNum = SP1K_ADC_BTN_A_NUM;   // 按钮数量
        pKeyEn = &sKeyEn[0];   // 按钮使能数组起始地址
        break;
    case KEY_TYPE_ADC_B:    // Adc B 按钮
        keyNum = SP1K_ADC_BTN_B_NUM;   // 按钮数量
        pKeyEn = &sKeyEn[0 + SP1K_ADC_BTN_A_NUM];   // 按钮使能数组起始地址
        break;
    case KEY_TYPE_GPIO:     // Gpio 按钮
        keyNum = SP1K_GPIO_BTN_NUM;    // 按钮数量
        pKeyEn = &sKeyEn[0 + SP1K_ADC_BTN_NUM];     // 按钮使能数组起始地址
        break;
    default:
        return FAIL;   // 返回配置失败
    }
    /**/
    for (i = 0; i < keyNum; i++)   // 遍历每个按钮的使能状态
    {
        if ((BitMask >> i) & 0x01)   // 判断位掩码中对应位是否为1
        {
            *pKeyEn = Enable;   // 设置按钮的使能状态
        }
        pKeyEn++;
    }
    // printf("En flag %u\n", *pKeyEn);
    return SUCCESS;   // 返回配置成功
}

/**
* @param    pAdcHdl 指向 ADC 句柄的指针
* @retval   None
* @brief    获取 ADC（模数转换器）句柄 -- 2008-02-20
**/
void sp1kAdcHdlGet(const scanTag_t **pAdcHdl)
{
    *pAdcHdl = (const scanTag_t *)&sTag;  // 将指向 sTag 的指针赋值给 pAdcHdl
}

/**
* @param    button 按钮编号
* @retval   按钮状态(1:按下(KEY_STATUS_PRESSED) 2:释放(KEY_STATUS_RELEASED))
* @brief    检查特定按钮的状态 -- 2008-02-20
**/
UINT8 sp1kBtnChk(UINT8 button)
{
    UINT8 btnSts;   // 按钮状态
    UINT8 gpioVal;  // GPIO 值

    return KEY_STATUS_RELEASED;  // 返回释放状态

#if (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_ || _HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_)
    if (button == SP1K_BTN_S2)  // 如果按钮是 S2
    {
#if USE_PKG_DEDICATE_PIN
        pkgDedicateGpioCfg(PKG_DEDICATE_PIN_S2, &gpioByteIdS2, &gpioBitMskS2, &gpiopolarityS2, 0);  // 配置专用 GPIO
        if (pkgChipIdGet() == 0x1626)  // 如果芯片 ID 是 0x1626
        {
            gpioPullSelSet(2, 0x40, 0x40);   // 设置 GPIO1 上拉
            gpioPullEnSet(4, 0x04, 0x04);    // GPIO1 上拉使能
        }
        else  // 否则
        {
            gpioPullSelSet(3, 0x40, 0x40);   // 设置 GPIO1 上拉
            gpioPullEnSet(5, 0x04, 0x04);    // GPIO1 上拉使能
        }
        gpioVal = gpioByteInGet(gpioByteIdS2, gpioBitMskS2, gpiopolarityS2);  // 读取 GPIO 值
#endif
        if (gpioVal)  // 如果 GPIO 值为真
        {
            btnSts = KEY_STATUS_PRESSED;  // 按钮状态为按下
        }
        else  // 否则
        {
            btnSts = KEY_STATUS_RELEASED;  // 按钮状态为释放
        }
    }
#else
    if (button == SP1K_BTN_S2)  // 如果按钮是 S2
    {
#if USE_PKG_DEDICATE_PIN
        pkgDedicateGpioCfg(PKG_DEDICATE_PIN_S2, &gpioByteIdS2, &gpioBitMskS2, &gpiopolarityS2, 0);  // 配置专用 GPIO
        if (pkgChipIdGet() == 0x1626)  // 如果芯片 ID 是 0x1626
        {
            gpioPullSelSet(2, 0x40, 0x40);   // 设置 GPIO1 上拉
            gpioPullEnSet(4, 0x04, 0x04);    // GPIO1 上拉使能
        }
        else  // 否则
        {
            // gpioPullSelSet(3,0x40,0x40);
            // gpioPullEnSet(5,0x04,0x04);//pull high
        }
        gpioVal = gpioByteInGet(gpioByteIdS2, gpioBitMskS2, 1);  // 读取 GPIO 值
#endif
        if (!gpioVal)  // 如果 GPIO 值为假
        {
            btnSts = KEY_STATUS_RELEASED;  // 按钮状态为释放
        }
        else  // 否则
        {
            btnSts = KEY_STATUS_PRESSED;  // 按钮状态为按下
        }
    }
#endif

    else if (button == SP1K_BTN_POWER)  // 否则，如果按钮是 POWER
    {
#if (_HW_SOLUTION_ == _HW_EVB_ || _HW_SOLUTION_ == _HW_TK_MODEL_A_)
        gpioByteDirSet(GPIO_BYTE_GEN1, 0x04, 0x00);  // 设置通用 GPIO 方向
        if (!gpioByteInGet(GPIO_BYTE_GEN1, 0x04, 1))  // 如果 GPIO 值为假
        {
            btnSts = KEY_STATUS_RELEASED;  // 按钮状态为释放
        }
        else  // 否则
        {
            btnSts = KEY_STATUS_PRESSED;  // 按钮状态为按下
        }
#elif (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_ || _HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_)
        {
            gpioByteDirSet(GPIO_BYTE_MA1, 0x40, 0x00);  // 设置专用 GPIO 方向
            if (!gpioByteInGet(GPIO_BYTE_MA1, 0x40, 1))  // 如果 GPIO 值为假
            {
                btnSts = KEY_STATUS_RELEASED;  // 按钮状态为释放
            }
            else  // 否则
            {
                btnSts = KEY_STATUS_PRESSED;  // 按钮状态为按下
            }
        }
#elif (_HW_SOLUTION_ == _HW_TK_MODEL_B_ || _HW_SOLUTION_ == _HW_RDK_A_)
        {
            UINT16 value;
            sp1kADCSarSel(0x02);  // 选择 ADC 通道
            sp1kHalWait(5);  // 等待 5 毫秒
            value = sp1kADCValueSnap();  // 获取 ADC 值
            if ((value > (SP1K_ADC_BTN_B_S4 + SP1K_ADC_BTN_DEV_B_S4)) ||  // 如果值在范围内
                (value < (SP1K_ADC_BTN_B_S4 - SP1K_ADC_BTN_DEV_B_S4)))
            {
                btnSts = KEY_STATUS_RELEASED;  // 按钮状态为释放
            }
            else  // 否则
            {
                btnSts = KEY_STATUS_PRESSED;  // 按钮状态为按下
            }
            sp1kADCSarSel(0x00);  // 恢复 ADC 通道
        }
#endif
    }

#if (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_ || _HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_)
    else if (button == SP1K_BTN_MODE)  // 否则，如果按钮是 MODE
    {
        UINT16 value;
        sp1kADCSarSel(0x02);  // 选择 ADC 通道
        sp1kHalWait(5);  // 等待 5 毫秒
        value = sp1kADCValueSnap();  // 获取 ADC 值
        if ((value > (SP1K_ADC_BTN_B_S2 + SP1K_ADC_BTN_DEV_B_S2)) ||  // 如果值在范围内
            (value < (SP1K_ADC_BTN_B_S2 - SP1K_ADC_BTN_DEV_B_S2)))
        {
            btnSts = KEY_STATUS_RELEASED;  // 按钮状态为释放
        }
        else  // 否则
        {
            btnSts = KEY_STATUS_PRESSED;  // 按钮状态为按下
        }
        sp1kADCSarSel(0x00);  // 恢复 ADC 通道
    }
#endif

    return btnSts;  // 返回按钮状态
}

#if FUNC_HOST_TOUCH_PANEL   // 【支持触摸面板的功能】
/**
* @param    None
* @retval   None
* @brief    按键触摸屏解析 -- 2011-12-23
**/
void keyScanTpParsing(void)
{
    static guiObj_t *pObjAct;     // 当前活动 GUI 对象指针
    static UINT16 tpPenRptVal = TP_PEN_RPT_START;  // 触摸笔重复值

    guiObj_t *pObj = NULL;  // GUI 对象指针

    // 如果当前触摸屏状态为按下
    if (tpCurrStsGet() == TP_PENDOWN)
    {
        guiObjActHdlGet(&pObj);  // 获取当前活动 GUI 对象指针
        // 如果当前活动 GUI 对象不为空，并且允许触摸重复
        if ((pObj != NULL) && (pObj->pObjExt && pObj->pObjExt->penRepeateEn))
        {
            // 如果当前活动对象为空
            if (pObjAct == NULL)
            {
                pObjAct = pObj;  // 设置当前活动对象为当前对象
            }
            // 触摸笔重复值递减
            if (--tpPenRptVal == 0)
            {
                tpPenRptVal = TP_PEN_RPT;  // 重置触摸笔重复值
                guiObjActHdlGet(&pObj);  // 获取当前活动 GUI 对象指针
                // 如果当前对象仍然是活动对象
                if (pObj == pObjAct)
                {
                    // 如果当前对象拓展属性中含有消息
                    if (pObj->pObjExt && pObj->pObjExt->msg)
                    {
                        osMsgPost(pObj->pObjExt->msg);  // 发送消息
                    }
                }
            }
        }
    }
    else
    {
        tpPenRptVal = TP_PEN_RPT_START;  // 重置触摸笔重复值
        pObjAct = NULL;  // 清空当前活动对象指针
    }
}
#endif
