/***************************************************************************
 * File: dev_init.c
 * Author: Yang
 * Date: 2024-03-07 13:48:46
 * description: 
 -----------------------------------
设备初始化相关
 -----------------------------------
****************************************************************************/
#include "general.h"

#include "hal_device.h"
#include "hal_spi.h"
#include "func_ptr.h"

#include "dev_init.h"
#include "device.h"
#include "main.h"
#include "initio.h"
#include "timer.h"
#include "gpio.h"
#include "gpio_api.h"
#include "key_scan.h"
#include "pb_general.h"

#include "sp1k_hal_api.h"
#include "sp1k_front_api.h"
#include "sp1k_gpio_api.h"
#include "sp1k_disp_api.h"
#include "sp1k_osd_api.h"
#include "sp1k_pb_api.h"
#include "sp1k_aud_api.h"
#include "sp1k_util_api.h"
#include "sp1k_zoom_api.h"

#include "app_def.h"
#include "app_init.h"
#include "app_msg.h"
#include "app_battery.h"
#include "app_dev_disp.h"
#include "app_dev_storage.h"
#include "app_menu.h"
#include "app_menu_micro.h"
#include "app_ui_para.h"
#include "app_audio_play.h"
#include "app_gpio.h"
#include "app_sts_pwroff.h"
#include "sp1k_dos_api.h"
#include "app_cal_api.h"

#include "host_func_opt.h"
#include "initio.h"
#include "app_ui_osd.h"

#if SUPPORT_SENSOR_SWITCH
UINT8 sensor_switch = 1;    // 1:大镜头 0:自拍
#endif

UINT8 enterMacroadjust = 0; // 进入应用程序调整宏标志位

UINT8 enterVersionshow = 0;

UINT8 enterPCCamflag = 0;

extern panelStatus_t G_PanelStatus;
extern UINT8 TV_Open;

/**
* @param    None
* @retval   None
* @brief    进行音频初始化设置
**/
UINT8 appAudioInit(void)
{
    uiPara_t *pPara = appUiParaGet();  // 获取UI参数

    sp1kAudioVolumeSet(VOLUME_10);  // 设置音频音量为10

    // 用于改善音频录制
    sp1kAudParamSet(SP1K_AUD_PARAM_ALC_MIN_VOL, 15);  // 设置自动增益控制的最小音量为15
    sp1kAudParamSet(SP1K_AUD_PARAM_ALC_MAX_VOL, 100);  // 设置自动增益控制的最大音量为100
    sp1kAudParamSet(SP1K_AUD_PARAM_ALC_DEFAULT_GAIN_IDX, 34);  // 设置自动增益控制的默认增益索引为34
    sp1kAudParamSet(SP1K_AUD_PARAM_ALC_GAIN_DOWN_SPEED, 4);  // 设置自动增益控制的降低增益速度为4
    sp1kAudParamSet(SP1K_AUD_PARAM_ALC_GAIN_UP_SPEED, 30);  // 设置自动增益控制的提高增益速度为30
    sp1kAudParamSet(SP1K_AUD_PARAM_ALC_OPT, 1);  // 启用自动增益控制

    return SUCCESS;  // 返回成功状态
}

/**
* @param    None
* @retval   return = SUCCESS / FAIL
* @brief    初始化前置设备
**/
UINT8 appFrontInit(void)
{
    UINT8 sensorId, actId;

    sensorId = sp1kFrontSensorInit();  // 前置传感器初始化
    sp1kFrontDevActSet(sensorId);  // 设置前置设备动作
    sp1kFrontPowerOn(sensorId);  // 前置设备上电

    actId = sp1kFrontDevActGet();  // 获取前置设备动作ID

    if (SUCCESS == sp1kFrontHighQualityDigitalZoomCapabilityGet(actId))
    {
        sp1kFrontHighQualityDigitalZoomModeSet(actId, 1);  // 如果支持高质量数字变焦，则设置为模式1
    }
    else
    {
        sp1kFrontHighQualityDigitalZoomModeSet(actId, 0);  // 如果不支持高质量数字变焦，则设置为模式0
    }

    return SUCCESS;  // 返回成功状态
}

/**
* @param    None
* @retval   return = SUCCESS / FAIL
* @brief    初始化变焦设备
**/
UINT8 appZoomDevInit(void)
{
    sp1kZoomDevInit();  // 初始化变焦设备

    if(VIDEO_NEW_ZOOM == 0 && STILL_NEW_ZOOM == 0)
    {
        sp1kZoomDevActSet(SP1K_ZOOM_LOGI_STILL_MODE, SP1K_ZOOM_LOGI_BLOCKING_TYPE);  // 设置静态模式的变焦逻辑为阻塞类型

        sp1kZoomDevActSet(SP1K_ZOOM_LOGI_VIDEO_MODE, SP1K_ZOOM_LOGI_BLOCKING_TYPE);  // 设置视频模式的变焦逻辑为阻塞类型
    }
    else
    {
        sp1kZoomDevActSet(SP1K_ZOOM_LOGI_STILL_MODE, SP1K_ZOOM_LOGI_NONBLOCKING_TYPE);  // 设置静态模式的变焦逻辑为非阻塞类型

        sp1kZoomDevActSet(SP1K_ZOOM_LOGI_VIDEO_MODE, SP1K_ZOOM_LOGI_NONBLOCKING_TYPE);  // 设置视频模式的变焦逻辑为非阻塞类型
    }

    return SUCCESS;  // 返回成功状态
}

/**
* @param    None
* @retval   return = SUCCESS / FAIL
* @brief    初始化各个GPIO引脚的配置，包括设置GPIO的功能、方向、输出值等
**/
UINT8 appDispInit(void)
{
    UINT8 pnlType, pnlSts;
    UINT16 decWidth, decHeight;
    uiPara_t *pPara = appUiParaGet();  // 获取UI参数结构体指针

    /*
    	Panel driver logic install process
    	The flow is NOT the physic driver, so the panel was still not been opened
    */
    sp1kDispPanelInit();  // 初始化显示面板
    sp1kDispTvNtscInit();  // 初始化TV NTSC
    sp1kDispTvPalInit();  // 初始化TV PAL
    sp1kDispHdmiInit();  // 初始化HDMI

    /* parioty: TV > HDMI > LCD */
    if (IS_GPIO_TV_PLUGIN())  // 如果TV插入
    {
        TV_Open = 1;  // TV打开标志设为1
        if (pPara->TVMode == TVMODE_NTSC)  // 如果TV模式为NTSC
        {
            pnlType = DISP_DEV_NTSC;  // 显示设备类型设为NTSC
            pnlSts  = PANEL_STATUS_NTSC;  // 面板状态设为NTSC
        }
        else
        {
            pnlType = DISP_DEV_PAL;  // 显示设备类型设为PAL
            pnlSts  = PANEL_STATUS_PAL;  // 面板状态设为PAL
        }
    }
    /*The open image must be showed on the LCD while HDMI in.*/
    /* else if (IS_GPIO_HDMI_PLUGIN()) {
    	pnlType = DISP_DEV_HDMI;
    	pnlSts  = PANEL_STATUS_LCD;
    }*/
    else
    {
        pnlType = DISP_DEV_PANEL;  // 显示设备类型设为面板
        pnlSts  = PANEL_STATUS_LCD;  // 面板状态设为LCD
    }

    /* panel physic open here */
    sp1kDispDevActSet(pnlType);  // 设置显示设备动作
    sp1kDispPowerOn(pnlType);  // 显示设备上电
    G_PanelStatus = pnlSts;  // 全局面板状态设为当前面板状态

    if (pPara->OpenPic)
    {
        //sp1kHalJpegDecodeWait();
        if (IS_DISP_MODE_TVOUT())
        {
            sp1kHalWait(500); //wait TV contect ready
        }
        sp1kPbDecodeSizeGet(&decWidth, &decHeight);  // 获取解码的宽度和高度
        sp1kDispImgShow(sp1kPbDispBuffGet(1), decWidth, decHeight, 100);  // 显示图片
    }

    return SUCCESS;  // 返回成功状态
}

/**
* @param    None
* @retval   None
* @brief    初始化GPIO配置
**/
void appGpioInit(void)
{
    // 仅用于1527调试 @090520
#if TAG_TODO
#if(_HW_SOLUTION_ == _HW_TK_MODEL_B_ || _HW_SOLUTION_ == _HW_RDK_A_)
    // 使能扬声器
    sp1kSpeaker_ENCfg();  // 设置扬声器使能配置
#endif
#endif

    // 如果芯片ID为0x1627或0x1626
    if((pkgChipIdGet() == 0x1627) || (pkgChipIdGet() == 0x1626))
    {
        gpioPullSelSet(6, 0xFF, 0xFF);   // 设置GPIO 6上下拉配置
        gpioPullEnSet(7, 0xF0, 0xF0);    // 设置GPIO 7上下拉配置
        gpioPullEnSet(8, 0x0F, 0x0F);    // 设置GPIO 8上下拉配置，DGPIO0~7拉高电平
    }

    sp1kSDWPDectCfg();  // 配置SD卡写保护检测

    sp1kTV_PlugCfg(TV_PLUG, 0);  // 配置TV插入检测
    sp1kHDMI_PlugCfg();  // 配置HDMI插入检测
    sp1kUSB_PlugCfg();  // 配置USB插入检测

    // 添加卡检测极性设置
    //sp1kSD_CD_PolaritySet(0);
    sp1kSD_CdCfg();  // 配置SD卡插入检测

#if TAG_TODO
    // 如果选择LED功能
    sp1kLED_SelfCfg();  // 配置自身LED
    sp1kLED_SelfSet(1);  // 设置自身LED状态为开启
    ///
    sp1kLED_BusyCfg();  // 配置忙状态LED
    sp1kLED_BusySet(1); // 关闭状态LED
#endif

    appChargePinCfg();  // 充电引脚配置

#if (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)
    /*传感器断电*/
    gpioByteFuncSet(GPIO_BYTE_GEN2, 0x80, 0x80);  // 设置通用IO引脚功能
    gpioByteOutSet(GPIO_BYTE_GEN2, 0x80, 0x00);   // 设置通用IO引脚输出值
    gpioByteDirSet(GPIO_BYTE_GEN2, 0x80, 0x80);   // 设置通用IO引脚方向

    /*lcd背光控制gpio6*/
    gpioByteFuncSet(GPIO_BYTE_GEN0, 0x40, 0x40);  // 设置通用IO引脚功能
    gpioByteOutSet(GPIO_BYTE_GEN0, 0x40, 0x40);   // 设置通用IO引脚输出值
    gpioByteDirSet(GPIO_BYTE_GEN0, 0x40, 0x40);   // 设置通用IO引脚方向
#endif

#if (_HW_SOLUTION_ == _HW_RDK_A_)
#if TAG_TODO
    //lcd电源
    sp1kGpioByteOutSet(GPIO_BYTE_P1, 0x80, 0x00);  // 设置通用IO引脚输出值
    sp1kGpioByteDirSet(GPIO_BYTE_P1, 0x80, 0xff);  // 设置通用IO引脚方向

    //传感器电源
    sp1kGpioByteOutSet(GPIO_BYTE_GEN2, 0x01, 0x00); // 设置通用IO引脚输出值
    sp1kGpioByteDirSet(GPIO_BYTE_GEN2, 0x01, 0xff); // 设置通用IO引脚方向
#endif
#endif

    // GPIO 0 ：自动对焦
    appAFCtrl(0);  // 控制自动对焦

    // GPIO 1 闪光灯
    appSnapFlashLed(0);  // 控制闪光灯

    // GPIO 2
#if SUPPORT_SENSOR_SWITCH
    appSensorPWDNSwitch(sensor_switch);  // 控制传感器开关(大镜头/自拍)
#endif

    app_set_led(1);  // 设置LED状态为开启

    // GPIO 6 SPI写保护
#if SUPPORT_SPI_WRITE_CONTROL
    appSetGpio6SpiWP(1);  // 设置GPIO 6 SPI写保护
#endif

    // GPIO 8 功放使能
#if SUPPORT_SPEAKER_CONTROL
    app_set_speaker(1);  // 控制扬声器使能
#endif

    // GPIO 10 看门狗
#if SUPPORT_MCU_WTD
    appSetWTDHeartbeat(1);  // 设置看门狗心跳
#endif
}

/**
* @param    None
* @retval   None
* @brief    初始化RTC（实时时钟）模块
**/
void appRtcInit(void)
{
    dateStc_t DateInfo;  // 定义日期结构体变量DateInfo，用于存储日期信息

    DateInfo.Year = 22;  // 设置年份为22年
    DateInfo.Month = 1;  // 设置月份为1月
    DateInfo.Day = 1;    // 设置日期为1日
    DateInfo.Hour = 0;   // 设置小时为0时
    DateInfo.Minute = 0; // 设置分钟为0分
    DateInfo.Second = 0; // 设置秒数为0秒

    sp1kRTCDateInit(&DateInfo);  // 将DateInfo结构体传入，初始化RTC日期时间
}

/**
* @param    None
* @retval   None
* @brief    主机设备初始化函数
**/
void hostDevInit(void)
{
    uiPara_t *pPara;  // 用户界面参数指针
    UINT16 adcVal = 0;  // ADC 值
    UINT8 enterCardUpdateFlag = 0;  // 进入卡升级标志

    // 初始化 GPIO
    appGpioInit();
    sp1kHalWait(100);  // 延时

    // 卡升级检测
#if 0
    sp1kADCSarSel(0x02);
    sp1kHalWait(50);
    adcVal = sp1kADCValueSnapIntr();
    sp1kHalWait(50);
    if ((adcVal >= (391 - 30) && adcVal <= (391 + 30)) || (adcVal >= (336 - 30) && adcVal <= (336 + 30)))
    {
        enterCardUpdateFlag = 1;
    }
#endif

    // 微距调试模式检测
#if 0
    if ((adcVal >= (626 - 30) && adcVal <= (626 + 30)) || (adcVal >= (587 - 30) && adcVal <= (587 + 30)))
    {
        enterMacroadjust = 1;
    }
#endif

    // USB 模式检测
    sp1kADCSarSel(0x00);
    sp1kHalWait(50);
    adcVal = sp1kADCValueSnapIntr();
    if ((adcVal >= (600 - 100) && adcVal <= (600 + 100)) && (HAL_GpioByteInGet(HAL_GPIO_BYTE_DISP0, 0x04, 0)) && (!sp1kUSB_PlugGet()))
    {
        enterVersionshow = 1;
    }
    else if ((adcVal >= (600 - 100) && adcVal <= (600 + 100)) && (HAL_GpioByteInGet(HAL_GPIO_BYTE_DISP0, 0x04, 0)) && (sp1kUSB_PlugGet()))
    {
        enterPCCamflag = 1;
    }

    // 无USB时需要长按开机
#if 1
    if (!sp1kUSB_PlugGet())
    {
        sp1kADCSarSel(0x00);
        sp1kHalWait(50);
        adcVal = sp1kADCValueSnapIntr();
        if (adcVal >= (600 - 100) && adcVal <= (600 + 100))
        {
            sp1kHalWait(50);
            adcVal = sp1kADCValueSnapIntr();
            if (!(adcVal >= (600 - 100) && adcVal <= (600 + 100)))
            {
                appPowerHoldPullDown();
            }
        }
        else
        {
            appPowerHoldPullDown();
        }
    }
    sp1kADCSarSel(0x00);
#endif

    // SPI 写控制支持
#ifdef DISK_DRV_SPI
#if SUPPORT_SPI_WRITE_CONTROL
    appSpiWpSet(1);
#else
    // 注册 SPI 的写保护回调函数
    sp1kSfCallbackReg(SP1K_SPI_CALLBACK_WP_SET, pFunc_SfWPSet);
#endif
#endif

    /* 加载用户定义数据 */
    appUiParaInit(PARA_INIT_FROM_UDF);
    pPara = appUiParaGet();

    appUiItemOrderCfg();  // 用户界面项目顺序配置

    /* 检查充电模式 */
    appChargeModeChk();

    if(appChargeModeGet())  // 如果处于充电模式
    {
        /*
        	在充电模式下的背景图片解码过程
        	当前看不到，因为显示面板尚未打开。
        */
        appImgDecode(0x38);
    }
    else     /* 非充电模式 */
    {
        /*
           打开图片解码过程
           当前看不到，因为显示面板尚未打开。
        */
        appOpenImgDec(pPara->OpenPic);

        /*
        	第一次电池检测
        	如果检测到电池电量低，则系统将关机。
        */
        appBattInit();

        /*
        	电源键检测
        	如果此时没有按下电源键，则系统将关机。
        */
#if (_HW_SOLUTION_ != _HW_EVB_)
        if (!prjDbgMode)
        {
            if (sp1kBtnChk(SP1K_BTN_POWER) != KEY_STATUS_PRESSED)
            {
                appPowerShutDown();
            }
        }
#endif
    }

    /* 面板打开 */
    appDispInit();

    // 开机卡升级
    if (!sp1kSD_CdGet())
    {
        appStorageMount(K_MEDIA_SDF);
#if SUPPORT_MCU_WTD
        appMCUWDTEnterCardISPMode();
#endif
    }

    /* 音频初始化 */
    appAudioInit();

    if(!appChargeModeGet())
    {
        appVoicePlay(SP1K_MSG_UNKNOWN_HOST, VOICE_TYPE_OPEN, VOICE_PLAYLEVLE_HIGH);

#if SUPPORT_MCU_WTD
        appSetWTDHeartbeat(0);
#endif
#if SUPPORT_MCU_WTD
        appSetWTDHeartbeat(1);
#endif
        sp1kHalWait(907);  /*mantis 42008*/
    }
    sp1kAudioVolumeSet(pPara->Volume);  // 设置音频音量

    /* 前置摄像头初始化 */
    appFrontInit();

    /* 变焦设备初始化 */
    appZoomDevInit();

    /* RTC 日期初始化 */
    appRtcInit();
}

/**
* @param    None
* @retval   None
* @brief    按键初始化函数，用于初始化设备按键
**/
void appButtonInit(void) USING_0
{
    devAttr_t *pDevAttr;  // 设备属性指针

    sp1kBtnDisableAll();  // 禁用所有按键

    pDevAttr = sp1kDevHdlGet();  // 获取设备句柄
    pDevAttr[DEV_ID_KEYBROAD].devSts = DEV_STS_RDY;  // 设置键盘设备状态为就绪
    pDevAttr[DEV_ID_KEYBROAD].devEn = 1;  // 启用键盘设备
    pDevAttr[DEV_ID_KEYBROAD].devType = DEV_TYPE_KEYBROAD_COM;  // 设置键盘设备类型为通用键盘
}

/**
* @param    mode 模式参数
* @retval   return = SUCCESS / FAIL 返回执行结果，成功或失败
* @brief    打开图像解码功能
**/
UINT8 appOpenImgDec(UINT8 mode) USING_0
{
    UINT32 resID;  // 资源ID
    uiPara_t *pPara;  // UI参数指针

    pPara = appUiParaGet();  // 获取UI参数

    if (mode == PB_OPENPIC_OFF)
    {
        resID = 0;  // 如果模式为关闭图像功能，则资源ID为0。
    }
    else if (mode == PB_OPENPIC_USER_DEF)
    {
        /* 指定用户自定义启动图像 */
        resID = 0x800;  // 如果模式为用户自定义图像，则资源ID为0x800。
    }
    else 
    {
        /* 指定默认启动图像 */
        resID = 0x0d;  // 否则指定默认启动图像资源ID为0x0d。
    }

    appImgDecode(resID);  // 调用图像解码函数解码指定资源ID的图像。

    return SUCCESS;  // 返回执行成功。
}

/**
* @param    resId 要解码的图像资源ID
* @retval   return = SUCCESS / FAIL 返回执行结果，成功或失败
* @brief    图像解码操作
**/
UINT8 appImgDecode(UINT32 resId) USING_0
{
    UINT32 freeBuffAddr = K_SDRAM_TotalSize - 0x100000;  // 空闲缓冲区地址，用于解码后的图像数据存储。
    UINT16 decWidth = PB_DECODE_WIDTH;  // 解码宽度
    UINT16 decHeight = PB_DECODE_HEIGHT;  // 解码高度

    sp1kPbDecodeSizeSet(decWidth, decHeight);  // 设置解码尺寸
    sp1kPbDispBuffSwitch();  // 切换显示缓冲区

    if(resId)
    {
        // 如果资源ID有效，则调用图像背景解码函数解码图像并显示。
        appBGImgDec(resId, NULL, freeBuffAddr, sp1kPbDispBuffGet(1), 0);
    }
    else 
    {
        // 否则，填充黑色显示缓冲区。
        sp1kPbFillBlack(sp1kPbDispBuffGet(1), decWidth, decWidth);
    }

    return SUCCESS;  // 返回执行成功。
}

/**
* @param    None
* @retval   None
* @brief    初始化设备
**/
void devInit(void)
{
    keyScanInit();    // 初始化按键扫描

    appButtonInit();  // 初始化应用按钮

    TIMER0_Start();   // 启动定时器0

    appInit();        // 应用初始化
}
