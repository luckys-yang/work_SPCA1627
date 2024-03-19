/**
 * @file      host_func_opt.h
 * @brief     主机级功能的选项
 * @author    Phil Lin
 * @since     2010-01-01
 * @date      2010-01-01
*/

#ifndef __HOST_FUNC_OPT_H__
#define __HOST_FUNC_OPT_H__

/**************************************************************************
 *                        H E A D E R   F I L E S
 **************************************************************************/

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

/* function support for calibration mode */
// 校准模式的功能支持
#define FUNC_HOST_CAL    1

/* function support for DPS */
// 为 DPS 提供功能支持
#define FUNC_HOST_DPS    1

/* function support for copy to card */
// 支持复制到卡的功能
#define FUNC_HOST_CP2CARD    1

/* function support for photoframe in preview mode */
// 在预览模式中为照片框架提供功能支持
#define FUNC_HOST_PF_PV	   1
/* function support for photoframe in playback mode */
// 在播放模式中支持相框功能
#define FUNC_HOST_PF_PB    1

/* function support for display */
// 显示功能支持
#define FUNC_HOST_DISP    1

/* function support for Motion detect */
// 支持运动检测功能
#define FUNC_HOST_MD    0

/* function support for Panorama */
// 支持全景功能
#define FUNC_HOST_PANORAMA    0

/* function support for HDMI */
// 支持 HDMI 功能
#define FUNC_HOST_HDMI    1

/* function support for TV out */
// 支持电视输出功能
#define FUNC_HOST_TV    1

/* function support for badpixel */
// 支持 badpixel 的功能
#define FUNC_HOST_BADPIXEL    1

/* function support for IGBT */
// 支持 IGBT 功能
#define FUNC_HOST_IGBT_FLASH    0

/* function support for touchPanel */
// 支持触摸面板的功能
#define FUNC_HOST_TOUCH_PANEL    0

/* function support for fixtrue */
// 支持 fixtrue 的函数
#define FUNC_HOST_AF_FIXTURE	0

/* function support for fixtrue */
// 自动对焦（AF）或对焦窗口
#define FUNC_HOST_AF_FOCUS_ONE_WIN	1




// 输出串口信息
#define DBG_MODE					0

// 支持无卡操作
#define SUPPORT_NOCARD_OPERATION	1

// 支持文件锁定
#define SUPPORT_LOCK_FILE			1

// 根据当前状态更新USB充电图标
#define SUPPORT_USB_ICON			1

// 支持屏保
#define SUPPORT_PANEL_PROTECT		1

// 根据当前状态更新LED闪烁方式
#define SUPPORT_UPDATE_LED			1

// SPI写保护功能
#define SUPPORT_SPI_WRITE_CONTROL	1

// GPIO6 控制功放使能
#define SUPPORT_SPEAKER_CONTROL		1

// 支持MCU做看门狗
#define SUPPORT_MCU_WTD				1

#define SUPPORT_SENSOR_SWITCH			1

/* 定时检测亮度是否发生变化，如果发生变化则打开AWB，否则关闭AWB */
#define SUPPORT_AWB_ADJ					0

// 支持在预览界面显示当前时间
#define SUPPORT_RTC_OSD_UPDATE			1


/*
	支持AF调焦
*/
#define SUPPORT_AF						1
#ifdef SUPPORT_AF
/* 支持两段式固定焦距 远距/微距 */
#define SUPPORT_FIXED_AF				1

/* 支持拍照前自动对焦 */
#define SUPPORT_SNAP_AF					1
#endif

/*录像EV降低*/
#define SUPPORT_VIDEO_EV					0


// 自动测试功能
#define SUPPORT_AUTO_TEST				1
#if SUPPORT_AUTO_TEST
#define SUPPORT_AUTO_TEST_FILENAME		"D:\\TEST.TXT"
#endif

// 版本信息
#define FW_VERSION						ID_STR_DC403S_5647_9851_221220

/**************************************************************************/

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/

/**************************************************************************
 *                 E X T E R N A L    R E F E R E N C E S                 *
 **************************************************************************/

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

#endif  /* __HOST_FUNC_OPT_H__ */

