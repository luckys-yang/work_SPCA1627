/***************************************************************************
 * File: app_init.c
 * Author: Yang
 * Date: 2024-03-07 11:02:18
 * description: 
 -----------------------------------
None
 -----------------------------------
****************************************************************************/
#define __FILE		__FILE_ID_APP_INIT__
#include "general.h"
#include "initio.h"
#include "os_api.h"
#include "os_custom.h"
#include "app_main.h"
#include "app_init.h"
#include "os_msg.h"
#include "app_msg.h"
#include "main.h"
#include "audio.h"
#include "gpio.h"
#include "sp1k_bp_api.h"
#include "app_ui_para.h"
#include "app_menu.h"
#include "app_menu_micro.h"
#include "func_ptr.h"
#include "key_scan.h"
#include "app_dev_storage.h"
#include "app_dev_usb.h"
#include "app_dev_disp.h"
#include "sp1k_pv_api.h"
#include "sp1k_snap_api.h"
#include "sp1k_ae_api.h"
#include "sp1k_awb_api.h"//wangjg++
#include "sp1k_fd_api.h"//wangjg++
#include "sp1k_rsvblk_api.h"
#include "sp1k_disp_api.h"
#include "sp1k_front_api.h"
#include "sp1k_osd_api.h"
#include "sp1k_hal_api.h"
#include "sp1k_util_api.h"
#include "sp1k_cdsp_api.h"
#include "sp1k_aud_api.h"
#include "sp1k_cmd_api.h"
#include "sp1k_dcf_api.h"
#include "sp1k_pb_api.h"
#include "app_audio_play.h"
#include "app_battery.h"
#include "sp1k_vfs_api.h"
#include "dramfunc.h"
//#include "dos32.h"
#include "sp1k_dos_api.h"
#include "sp1k_calibration_api.h"
#include "gpio_api.h"
#include "device.h"
#include "cdsp_init.h"
#include "sp1k_gpio_api.h"
#include "snap_task.h"
#include "app_sts_pwroff.h"
#include "dbg_def.h"
#include "app_init.h"
#include "hal_cdsp_iq.h"
#include "snap_core.h"
#include "sp1k_video_api.h"
#include "app_video_view.h"
#include "rsc_id.h"
#include "osd_mem.h"
#include "res_macro.h"
#include "host_func_opt.h"
#include "lsc_cal.h"
#include "app_still_view.h"
#include "Sp1k_dos_api.h "
#include "Sp1k_disk_api.h"
#include "sp1k_stamp_api.h"
#include "gui_api.h"
#include "gui_dbg.h"
#include "host_func_opt.h"
#include "initio.h"
#include "app_tp_scan.h"
#include "sp1k_video_capture.h"

UINT8 TV_Open = 0;
extern void appOsdColorSet(UINT8 attr);
extern xdata UINT8 G_ucMSDC_MLUN;

/* Public function prototypes=========================================================*/

void appCdspInit(void);
void appVdSvcInit(void);
void appBpInit(void);
void appSnapInit(void);
void appPbInit(void);
void appSpeedyCaptureInit(void);
void appSpeedyCaptureRecInit(void);
void appUIToolInfoCfg(void);
void appOsdInit(void);
void appUsbScriptInit(void);
void appUsbInit(void);
void appVersionShow(void);
void appMemDosRamInit(void);
void appMemDosRamInit(void);
void appInit(void);

extern void menuBankGet(UIToolInfo*info);  // 获取菜单 bank 信息函数声明
/**
* @param    None
* @retval   None
* @brief    用于初始化 CDSP 应用
**/
void appCdspInit(void)
{
    sp1kCdspParaInit(SP1K_RSC_ID_CDSPPAR);  // 初始化 CDSP 参数

#ifndef COMPARE_ANSWER
    sp1kCdspGammaInit(SP1K_RSC_ID_GAMMA);  // 初始化 Gamma 校正参数
    sp1kCdspLscInit(SP1K_RSC_ID_LSC);      // 初始化镜头阴影校正参数
    sp1kCdspWdrInit(SP1K_RSC_ID_WDR);      // 初始化宽动态范围参数
    sp1kCdspPvParaInit(SP1K_RSC_ID_PVPAR); // 初始化预览参数
    sp1kCdspPvDynamicParaInit(SP1K_RSC_ID_PVPAR); // 初始化预览动态参数
    #if (EDGE_9_LEVEL_FUN == 0)
    sp1kCdspStillParaInit(SP1K_RSC_ID_STILLPAR); // 初始化静态图像参数
    sp1kCdspStillDynamicParaInit(SP1K_RSC_ID_STILLPAR); // 初始化静态图像动态参数
    #else
    sp1kCdspStillParaInit(SP1K_RSC_ID_STILLPAR_MIDDLE); // 初始化静态图像中等参数
    sp1kCdspStillDynamicParaInit(SP1K_RSC_ID_STILLPAR_MIDDLE); // 初始化静态图像中等动态参数
    #endif
    sp1kCDSPInit();  // 初始化 CDSP
#endif

#ifdef MI_LSC_CAL_OPTION
    if (sp1kUSB_PlugGet())  // 如果 USB 插入
    { 
        if (sp1kBtnChk(SP1K_BTN_S2) == KEY_STATUS_PRESSED) // 如果按键 S2 被按下
        {
            appSetMILSCCalSts(1);  // 设置 MI LSC 校准状态为开启
            //printf("MI Cal EN!\n\n\n");  // 打印信息：MI 校准已开启
        }
    }
#endif
}

/**
* @param    None
* @retval   None
* @brief    初始化视频服务
**/
void appVdSvcInit(void)
{
    /* AWB 初始化 */
    sp1kAwbInit();
    sp1kAwbGainAdjust(245, 256); // 调整 AWB 增益

    /* AE 初始化 */
    sp1kAeInit();
    sp1kAeParamSet(SP1K_AE_TARGET_LUMA, 140); // 设置目标亮度

    /* AE 用户测光回调函数初始化 */
    sp1kAeCallbackReg(SP1K_AE_CALLBACK_USER_DEFINE_METER, pFunc_AeUserDefineMeter);

    /* 设置用户测光模式启用/禁用 */
    appUserMeterModeSet(0);

    /* 检查 AE 用户测光是否启用 */
    if (appUserMeterModeGet() == 1) 
    {
        /* 设置用户测光 */
        sp1kAeParamSet(SP1K_AE_METERING, SP1K_AE_METER_USER_DEFINE);

        /* 如果需要动态用户测光流程（主控制器启用或禁用） */
        osTaskCreate(pFunc_AeDynamicUserDefineMeter, OS_PRIO_DYMETER);
    }

    /* 人脸检测初始化 */
    sp1kFdInit();

    /* VD 服务初始化 */
    sp1kVdInit();

    /* 设置电力频率 */
    appElecFreqSet();
}

/**
* @param    None
* @retval   None
* @brief    初始化坏点校正
**/
void appBpInit(void)
{
    //UINT16 snapW, snapH;  // 预览图像宽度和高度
    UINT32 addr;  // 地址

    //sp1kFrontSnapSizeGet(0, &snapW, &snapH);  // 获取前置摄像头预览图像大小

    /* 预览 */
    addr = K_SDRAM_BadPixelCoordBufAddr << 1;  // 坏点坐标缓冲区地址
    if (sp1kNandRsvRead(0x05, addr) == TRUE)  // 从 NAND Flash 读取预览坏点校正数据
    {
        //HAL_CdspPvBpTblAddrSet(K_SDRAM_BadPixelCoordBufAddr);  // 设置预览坏点表地址
        //HAL_CdspPvBpEnSet(1);  // 启用预览坏点校正
    }

    /* BP 预览 HD 模式 */
    addr = (K_SDRAM_BadPixelCoordBufAddr + K_SDRAM_BadPixel_PrvBufSize + K_SDRAM_BadPixel_CapBufSize_Upper + K_SDRAM_BadPixel_CapBufSize_Lower) << 1;  // BP 预览 HD 模式数据地址
    if (sp1kNandRsvRead(0xd6, addr) == TRUE)  // 从 NAND Flash 读取 BP 预览 HD 模式数据
    {
        //printf("load success!");
    }

    /* 捕获 */
    #if 1  /* 解决 MOE 实时坏点补偿 */
        addr = (K_SDRAM_BadPixelCoordBufAddr + K_SDRAM_BadPixel_PrvBufSize) << 1;  // BP 捕获数据地址
        if (sp1kNandRsvRead(0x19, addr) == TRUE)  // 从 NAND Flash 读取 BP 捕获数据
        {
            //HAL_CdspStillBpCfg(0,0,0,snapW);  // 配置静态图像坏点校正
            //HAL_CdspStillBpEnSet(1);  // 启用静态图像坏点校正
            sp1kSnapParamSet(SP1K_SNAP_PARAM_BAD_PIXEL_OPT, SNAP_BP_OFFLINE);  // 设置捕获参数为离线坏点校正
            //printf("suc up bp rsc add=0x%lx\n",addr);
        }

        addr += (K_SDRAM_BadPixel_CapBufSize_Upper << 1);  // 下半部分坏点校正数据地址
        if (sp1kNandRsvRead(0x1a, addr) == TRUE)  // 从 NAND Flash 读取下半部分坏点校正数据
        {
            //printf("suc down bp rsc add=0x%lx\n",addr);
            //printf("success bp rsc add=0x%lx\n",( K_SDRAM_BadPixelCoordBufAddr + K_SDRAM_BadPixel_PrvBufSize) << 1);
        }

    #else
        if (sp1kNandRsvRead(0x06, (K_SDRAM_BadPixelCoordBufAddr + K_SDRAM_BadPixel_PrvBufSize) << 1) == TRUE)  // 从 NAND Flash 读取捕获数据
        {
            //HAL_CdspStillBpCfg(0,0,0,snapW);  // 配置静态图像坏点校正
            //HAL_CdspStillBpEnSet(1);  // 启用静态图像坏点校正
            sp1kSnapParamSet(SP1K_SNAP_PARAM_BAD_PIXEL_OPT, SNAP_BP_OFFLINE);  // 设置捕获参数为离线坏点校正
            //printf("success bp rsc add=0x%lx\n",( K_SDRAM_BadPixelCoordBufAddr + K_SDRAM_BadPixel_PrvBufSize) << 1);
        }
    #endif
}

/**
* @param    None
* @retval   None
* @brief    用于初始化拍摄功能
**/
void appSnapInit(void)
{
    /* 坏点校正初始化 */
    appBpInit();

    /* 初始化拍摄相关功能 */
    sp1kSnapInit(K_SDRAM_CommonEndAddr, K_SDRAM_TotalSize);

    /* 注册拍摄主机 burst 检查回调函数 */
    sp1kSnapCallbackReg(SP1K_SNAP_CALLBACK_BURST, pFunc_SnapBurst);

    /* 注册拍摄主机加载背景回调函数 */
    sp1kSnapCallbackReg(SP1K_SNAP_CALLBACK_BACKGROUND_RES_LOAD, pFunc_SnapBackgroundResLoad);

    /* 注册拍摄主机时间戳回调函数 */
    sp1kSnapCallbackReg(SP1K_SNAP_CALLBACK_HOST_STAMP, pFunc_SnapHostStamp);

    /* 加载日期时间戳字体 */
    sp1kStampFontInit();
}

/**
* @param    None
* @retval   None
* @brief    PB初始化
**/
void appPbInit(void)
{
    sp1kPbCallbackReg(SP1K_PB_CALLBACK_ABORT, pFunc_pbAbort);
}

/**
* @param    None
* @retval   None
* @brief    快速捕获初始化
**/
void appSpeedyCaptureInit(void)
{
    sp1kSpeedyCaptureCallbackReg(SP1K_SPEEDCAP_STILL, pFunc_VideoCap);
}

/**
* @param    None
* @retval   None
* @brief    快速捕获记录初始化
**/
void appSpeedyCaptureRecInit(void)
{
	sp1kSpeedyCaptureCallbackReg( SP1K_SPEEDCAP_CLIP,pFunc_VideoCapRec);
}

/**
* @param    None
* @retval   None
* @brief    初始化 OSD（屏幕显示）
**/
void appUIToolInfoCfg(void)
{
    UIToolInfo info;  // 定义 UITool 信息结构体变量

    menuBankGet(&info);  // 获取菜单 bank 信息

    //info.adfAddr = 0;
    //info.adfSize = sizeof(uiPara_t);
    info.strAddr = (info.currentBank + 1) << 15;  // 字符串地址，每个 bank 占 32kbytes，包括公共 bank
    info.strIdxAddr = info.strAddr + info.strIdxAddr;  // 字符串索引地址
    info.paraAddr = ((info.paraAddr & 0x0000ffff) - 0x8000) + info.strAddr;  // 参数地址
    //info.adfAddr = 0;//(K_SDRAM_UsbAddr+512*4) << 1;
    //sp1kNandRsvSizeGet(0x0,NULL,&info.adfSize);
    //printf("\n\n\ninfo.adfSize=%ld\n",info.adfSize);
    //printf("straddr=0x%lx,0x%lx\nstridxaddr=0x%lx,0x%lx\n paraaddr=0x%lx,size=0x%lx\n\n\n",info.strAddr,
    //info.strSize,info.strIdxAddr,info.strIdxSize,info.paraAddr,info.paraSize);
    //dramFileSave("PARA", info.paraAddr>>1, info.paraSize);

    // 初始化 UITool
    sp1kUIToolSet(0xff, 0x00, 0, 0x0, 0x0, 0);  // 初始化 UITool
    sp1kUIToolSet(0, 0x01, 0, sizeof(uiPara_t), 0x0000, 0);  // 设置 ADF.bin
    sp1kUIToolSet(1, 0x02, 0, 0, 0x0100, 0);  // 设置 menu.bin
    sp1kUIToolSet(2, 0x03, 0, sizeof(uiPara_t), 0x0801, SP1K_MSG_DO_RESET);  // 设置 udf.bin
    sp1kUIToolSet(3, 0x04, info.paraAddr, info.paraSize, 0xffff, 0);  // 设置参数
    sp1kUIToolSet(4, 0x05, info.strAddr, info.strSize, 0xffff, 0);  // 设置字符串
    sp1kUIToolSet(5, 0x06, info.strIdxAddr, info.strIdxSize, 0xffff, 0);  // 设置字符串索引
    sp1kUIToolSet(6, 0x10, 0, 0, 0x0048, 0);  // 设置 exif.bin
    sp1kUIToolSet(7, 0x20, 0, 0, 0x000c, 0);  // 设置 CUSTOMIZATION\UsbStr.txt
    sp1kUIToolSet(8, 0x30, 0, 0, 0x000d, 0);  // 设置 JPG\Open.jpg，打开图像
    sp1kUIToolSet(9, 0x31, 0, 0, 0x000f, 0);  // 设置 JPG\Close.jpg，关闭图像
    //sp1kUIToolSet(6,0x30, 0			,	0,				0x0801	,0);//JPG\Open.jpg,open image
    //sp1kUIToolSet(7,0x31, 0			,	0,				0x0801	,0);//JPG\Close.jpg,close image
    sp1kUIToolSet(10, 0x32, 0, 0, 0x000e, 0);  // 设置 WAV\Open_1.wav(Res_0x0e.res)
    sp1kUIToolSet(11, 0x33, 0, 0, 0x0018, 0);  // 设置 WAV\Close_1.wav(Res_0x18.res)
}

/**
* @param    None
* @retval   None
* @brief    初始化应用OSD(屏幕显示)
**/
void appOsdInit(void)
{
    uiPara_t *puiPara = appUiParaGet(); // 获取UI参数结构体指针

    /* Osd related function initial */
    appUIToolInfoCfg(); // 配置UI工具信息
    sp1kOsdInit(appPanelTypeGet()); // 初始化OSD（On-Screen Display），根据面板类型设置参数
    sp1kOsdLoad(); // 载入OSD资源
    sp1kOsdFontSizeSet(16, 32); /* 设置OSD字体大小，应为(16, 32)，(12, 24)，(8, 16) */
    sp1kOsdClear(0); // 清空OSD显示区域

    #if FUNC_HOST_TOUCH_PANEL
    guiObjInit(); // 初始化GUI对象
    guiDbgEnSet(0); // 设置GUI调试模式为关闭
    guiObjOpModeSet(GUI_OP_MODE_PENDN_ONLY, 0); // 设置GUI对象操作模式为仅按下
    guiObjOpModeSet(GUI_OP_MODE_PENUP, MODE_PENUP_BY_PENDN); // 设置GUI对象操作模式为抬起方式由按下决定
    guiObjOpModeSet(GUI_OP_MODE_SLIDE_EN, 1); // 设置GUI对象操作模式为启用滑动
    guiObjOpParaSet(GUI_OP_PARA_SLIDE_THD_X, 0); // 设置GUI对象操作参数为X轴滑动阈值为0
    guiObjOpParaSet(GUI_OP_PARA_SLIDE_THD_Y, 48); // 设置GUI对象操作参数为Y轴滑动阈值为48
    appGuiEnableSet(1); // 启用GUI
    tpCalDatLoad(0xfb, K_SDRAM_CommonEndAddr); // 加载触摸屏校准数据
    #endif

    appOsdColorSet(MENU_STR_LOST_FOCUS); // 设置OSD颜色为失去焦点状态下的颜色
    menuDispParaInit(); // 初始化菜单显示参数
    //appVersionShow(); // 显示应用版本信息

    if(puiPara->OpenPic == PB_OPENPIC_OFF)
    {
        sp1kDispImgShow(pbDispBuffGet(1), PB_DECODE_WIDTH, PB_DECODE_HEIGHT, 100); // 如果UI参数中的OpenPic为关闭状态，则显示图片
    }

    sp1kHalCtrlScreen(3); // 控制屏幕行为，参数3为显示
}

/**
* @param    None
* @retval   None
* @brief    初始化USB脚本
**/
void appUsbScriptInit(void)
{
    //cx++ 用于读取USB描述符
    //printf("\n**@@@\n"); // 打印调试信息

    sp1kNandRsvRead(0x1c, (K_SDRAM_UsbAddr << 1)); // 读取USB1.1配置描述符，从K_SDRAM_UsbAddr地址处读取，偏移左移1位（相当于乘以2）

    //sp1kNandRsvRead(0x1b, K_SDRAM_CodeSize*2+(0xc000-0x4000)+10240); // 读取USB2.0描述符，从G_FPGAMemPool[0x3000]的地址开始读取，因为大小超过1024，所以使用特定的偏移量

    sp1kNandRsvRead(0x1b, (K_SDRAM_UsbAddr << 1) + 1024); // 从K_SDRAM_UsbAddr地址处偏移1024字节处读取USB2.0描述符

    //cx++ 结束
}

/**
* @param    None
* @retval   None
* @brief    初始化USB功能
**/
void appUsbInit(void)
{
    /* 加载USB自定义字符串 */
    sp1kUsbStrSet(0x0c); // 设置USB自定义字符串地址为0x0c
    appUsbScriptInit(); // 初始化USB脚本
}

/**
* @param    None
* @retval   None
* @brief    显示应用版本信息
**/
void appVersionShow(void)
{
	UINT8 strVer[128] = "VER."; // 版本信息字符串
	UINT16 higth, width; // 屏幕高度和宽度

	// 获取屏幕尺寸
	sp1kDispPnlDimGet(&higth, &width);

	// 将版本信息追加到字符串后面
	strcpy(strVer + 4, sp1kFWVerGet());

	// 清空OSD显示区域
	sp1kOsdClear(0);

	// 在屏幕中央底部绘制版本信息字符串
	sp1kOsdStrDrawExt(width / 2 - 40, higth - 32, strVer, 0);
}

// #define DISK_DRV_DRAM
/**
* @param    None
* @retval   None
* @brief    初始化DOS RAM功能
**/
void appMemDosRamInit(void)
{
	UINT32 totSize = sysMemTotSizeGet();    // 获取系统总内存大小
    UINT32 size; // DOS RAM大小
    UINT8 enable; /* DOS RAM功能开关 */

    enable = 1; // 默认开启DOS RAM功能

    switch (totSize) 
    {
        case DEV_MEM_SIZE_16M:  // 如果总内存大小为16M
        case DEV_MEM_SIZE_64M:   // 或者总内存大小为64M
            size = 0; // DOS RAM大小设为0
            break;
        case DEV_MEM_SIZE_128M: // 如果总内存大小为128M
            size = 0x18000; // DOS RAM大小设为0x18000（4M字）
            break;
        case DEV_MEM_SIZE_256M: // 如果总内存大小为256M
            size = 0xC00000; // DOS RAM大小设为0xC00000（12M字）
            break;
        case DEV_MEM_SIZE_512M: // 如果总内存大小为512M
            size = 0x1400000; // DOS RAM大小设为0x1400000（20M字）
            break;
        default: // 其他情况
            size = 0; // DOS RAM大小设为0
            break;
    }

    if (totSize <= DEV_MEM_SIZE_64M) 
    { // 如果总内存大小小于等于64M
        enable = 0; // 关闭DOS RAM功能
    }

    #ifndef DISK_DRV_DRAM
    size = 0; // 如果未定义DISK_DRV_DRAM宏，则将DOS RAM大小设为0
    enable = 0; // 如果未定义DISK_DRV_DRAM宏，则关闭DOS RAM功能
    #endif

    sp1kDosRamCfg(enable); // 配置DOS RAM功能
    sysMemSizeSet(MEM_IDX_DOS_RAM, size); // 设置DOS RAM的内存大小
}

/**
* @param    None
* @retval   None
* @brief    系统启动时对内存进行初始化和一些配置
**/
void memInit(void)
{
	/*
		注意：此函数首先被主函数调用。
		请不要在此函数中添加任何与功能无关的内容！！！
	*/

	/* 系统内存块大小初始化（绝对值）*/
	osdSrcSize_t srcSize; // 定义存储OSD源尺寸的结构体变量
	srcSize.iconSize = (UINT32)ICON_FILE_SIZE;  // 设置图标文件大小
	srcSize.FontSize = (UINT32)FONT_FILE_SIZE;  // 设置字体文件大小
	srcSize.EngSize = (UINT32)ENGLISH_FILE_SIZE;  // 设置英文文件大小
	srcSize.StrSize = (UINT32)STRING_FILE_SIZE;  // 设置字符串文件大小

	sp1kOsdSrcSizeSet(&srcSize);  // 设置OSD源尺寸
	sysMemSizeInit();  // 初始化系统内存大小

	/* 在这里自定义内存块大小 */
	//sysMemSizeSet(MEM_IDX_DOS_RAM, 0);  // 自定义设置DOS RAM内存大小
	appMemDosRamInit();  // 初始化DOS RAM内存

	/* 更新整个系统内存地址 */
	sysMemAddrUpdate();  // 更新系统内存地址
}

/**
* @param    None
* @retval   None
* @brief    初始化应用程序
**/
void appInit(void)
{
	/* OS任务创建 */
	#if FUNC_HOST_TOUCH_PANEL
	osTaskCreate(pFunc_GuiProc, OS_PRIO_GUI); // 创建GUI处理任务
	#endif
	osTaskCreate(pFunc_CardDetect, OS_PRIO_CARD); // 创建卡片检测任务
	osTaskCreate(pFunc_TVDetect, OS_PRIO_TV); // 创建TV检测任务
	osTaskCreate(pFunc_BatteryChgDetect, OS_PRIO_BCHG); // 创建电池充电检测任务
	osTaskCreate(pFunc_CheckCountforTestUnitReady, OS_PRIO_CHECK_USB); // 创建USB检测任务

#ifdef MI_LSC_CAL_OPTION
	if (!appGetMILSCCalSts()) {
		osTaskCreate(pFunc_USBDetect, OS_PRIO_USB); // 如果未进行LSC校准，则创建USB检测任务
	}
#else
	osTaskCreate(pFunc_USBDetect, OS_PRIO_USB); // 创建USB检测任务
#endif

	if (sp1kDosRamStsGet() == TRUE) // 如果DOS RAM状态为真
	{
		sp1kDosRamBufSet(K_SDRAM_DosRamBufSize); // 设置DOS RAM缓冲区大小
	}

	printf("IC Version: %c\n", XBYTE[0x20FF] ? 'B' : 'A'); // 打印IC版本信息

	/* CDSP初始化 */
	appCdspInit();

	/* 3A/FD初始化 */
	appVdSvcInit();

	#if FUNC_HOST_IGBT_FLASH
	sp1kAwbModeSet(AWB_MODE_Flash);//4 //fan.xiao modified 2011-09-21 , 13:52:40  Add for flash
	#endif

	/* PB初始化 */
	appPbInit();

	/* 快照初始化 */
	appSnapInit();

	/* 快速捕获初始化 */
	appSpeedyCaptureInit();

	/* 快速捕获记录初始化 */
	appSpeedyCaptureRecInit();

	sp1kdcfQuickInitSet(0);

	/* OSD初始化 */
	appOsdInit();

	/* USB初始化 */
	appUsbInit();

	/* 准备切换到第一个主机状态 */
	if (appChargeModeGet()) // 如果处于充电模式
    { 
		osMsgPost(SP1K_MSG_STATE_TO_IDLE); // 发送消息，切换到空闲状态
	}
	else 
    {
		osMsgPost(SP1K_MSG_STATE_TO_POWER_ON); // 发送消息，切换到开机状态
	}
}