/**************************************************************************
 *         Copyright(c) 2010 by i-Catch Technology Co., Ltd.              *
 *                                                                        *
 *  This software is copyrighted by and is the property of i-Catch Tech-  *
 *  nology Co., Ltd. All rights are reserved by i-Catch Technology Co.,   *
 *  Ltd. This software may only be used in accordance with the            *
 *  corresponding license agreement. Any unauthorized use, duplication,   *
 *  distribution, or disclosure of this software is expressly forbidden.  *
 *                                                                        *
 *  This Copyright notice "M U S T" not be removed or modified without    *
 *  prior written consent of i-Catch Technology Co., Ltd.                 *
 *                                                                        *
 *  i-Catch Technology Co., Ltd. reserves the right to modify this        *
 *  software without notice.                                              *
 *                                                                        *
 *  i-Catch Technology Co., Ltd.                                          *
 *  19-1, Innovation First Road, Science-Based Industrial Park,           *
 *  Hsin-Chu, Taiwan, R.O.C.                                              *
 *                                                                        *
 *************************************************************************/

/**
 * @file      global_init.c
 * @brief     Brief
 * @author    Phil Lin
 * @since     2010-01-01
 * @date      2010-01-01
*/

/**************************************************************************
 *                        H E A D E R   F I L E S
 **************************************************************************/
#define __FILE __FILE_ID_GLOBAL_INIT__
#include "general.h"
#define SYS_INIT_INTERNAL // for system initialization internal use

#include "hal_global.h"
#include "hal_dram.h"
#include "hal_usb.h"
#include "hal_device.h"

#include "reg_def.h"
#include "global_init.h"
#include "os_api.h"
#include "os_custom.h"
#include "os_msg.h"
#include "cmd.h"
#include "func_init_def.h"
#include "timer.h"
#include "solution.h"
#include "initio.h"
#include "gpio.h"
#include "ctlsetup.h"
#include "bulkout.h"
#include "usbmsdc2.h"
#include "main.h"
#include "uiflow.h"
#include "cardopt.h"
#include "class.h"
#include "dbg_file_list.h"
#include "dbg_def.h"

#include "sp1k_gpio_api.h"
#include "sp1k_aud_api.h"
#include "sp1k_disk_api.h"
#include "sp1k_util_api.h"
#include "sp1k_hal_api.h"
#include "sp1k_zoom_api.h"
/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/
UINT8 xdata bUsbPwr;
xdata UINT8  G_UI_USBConnect;
xdata UINT16 G_UIStatus;
/**************************************************************************
 *                 E X T E R N A L    R E F E R E N C E S                 *
 **************************************************************************/
extern SYS_INIT_DCL pFuncInit code funcInitArray[];
extern SYS_INIT_DCL pFuncInit code funcInitMain;

extern UINT8 xdata G_ucPktStorIntrf;
extern UINT8 xdata G_MLUN_CardSupport;
extern xdata UINT8 G_USBAudioSetInterface;
extern UINT8 HAL_TpPreCfg(void);
extern UINT8 zoomDevInit(void);
/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

/**
* @param    None
* @retval   None
* @brief    初始化UART -- 2011-2-15
**/
static void uartInit(void)
{
    // 设置在 "startup.a51" 中定义的波特率
	HAL_GlobalUartSet(1);   // 选择RX/TX引脚的复用
	globalInitPrintf("\niCatLite\n");   // 上电过程中的第一条调试信息
}

/**
* @param    None
* @retval   None
* @brief    初始化CPU -- 2011-2-15
**/
static void cpuInit(void)
{
    TCON = 0x40;                    //TCON寄存器: IT0=0 INT0负边沿触发
                                    //            IE0=0 INT0中断标志位
                                    //            IT1=0 INT1负电平触发
                                    //            IE1=0 INT1中断标志位
                                    //            TR0=0 TIMER0禁用
                                    //            TF0=0 TIMER0溢出标志位
                                    //            TR1=1 TIMER1启用
                                    //            TF1=0 TIMER1溢出标志位

    IP = 0x03;                      //IP寄存器:   PX0=1 INT0优先级
                                    //            PT0=1 TIMER0优先级
                                    //            PX1=0 INT1优先级
                                    //            PT1=0 TIMER1优先级
                                    //            PS=0 串口优先级
                                    //            PT2=0 Timer2优先级

    IE = 0x81;                      //IE寄存器:   EX0=1 INT0中断使能
                                    //            ET0=0 TIMER0中断禁用
                                    //            EX1=0 INT1中断禁用
                                    //            ET1=0 TIMER1中断禁用
                                    //            ES=0 串口中断禁用
                                    //            ET2=0 TIMER2中断禁用
                                    //            EA=1  所有中断使能

    TMOD = 0x11;                    //TMOD寄存器: Timer0 M1M0=1 (模式1=16位计数器)
                                    //            Timer0 C/T~=0 (定时器)
                                    //            Timer0 GATE=0
                                    //            Timer1 M1M0=2 (模式2=8位计数器)
                                    //            Timer1 C/T~=0 (定时器)
                                    //            Timer1 GATE=0

	IT1 = 1; 						// 决定EX1的触发方式

	TH1 = K_Timer1IntervalHi;       // 设置Timer1的高8位初值
	TL1 = K_Timer1IntervalLo;       // 设置Timer1的低8位初值
}

/**
* @param    None
* @retval   None
* @brief    初始化DRAM -- 2011-2-15
**/
static void dramInit(void)
{
	/* DRAM访问优先级设置 */
	XBYTE[REG_Dram_Master0b_Sel] = 0x00;	// 设置CDSP优先级高于显示

	XBYTE[REG_Dram_SdckMode] &= ~0x40; // MA14输出使能，当DRAM为16*16时必须设置
	/* 代码哨兵设置 */ // 移动到"sysMemAddrUpdate"
	/* 首先保护1个字，然后在系统内存初始化后为所有代码区域进行保护 */
	HAL_DramCodeSentry(0x00, 0x00, 0x00);

	/* 看门狗定时器初始化 */
	dbgWdtCfg(1, WDT_PROC_DBG_ALL, WDT_TIME_OUT);

	/* 全局定时器初始化 */
	HAL_GlobalTimerInit();
}

/**
* @param    None
* @retval   None
* @brief    GPIO初始化函数 -- 2011-2-15
**/
static void gpioInit(void)
{
	XBYTE[0x201a] &= 0xf0;	// 复用引脚功能：GPIO 28~31

	XBYTE[0x2474] = 0x01;
	// 获取当前芯片封装中的芯片ID为0x1627 或 为0x1626
	if((pkgChipIdGet()== 0x1627)||(pkgChipIdGet()== 0x1626))
	{
		XBYTE[0x2009] = 0x00; // 仅适用于1627
		XBYTE[0x2049] = 0xA9;
		XBYTE[0x204A] = 0x56;
		XBYTE[0x2060] = 0x56; // 螳螂为42697
	}

	#ifndef LIB_RELEASE
		#if(_HW_SOLUTION_ == _HW_EVB_)
			XBYTE[0x2048] = 0x75; // 仅适用于EVB，增加驱动力
		#endif
	#endif

	#if defined(AMCAP_OPTION) || defined(ICAT_OPTION)
	//#ifdef ICAT_OPTION
		bUsbPwr = 0;

		//if (prjDbgMode) {
		//	bUsbPwr = !bUsbPwr;
		//}
	#else

		#if(_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_||_HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_)
			if (sp1kBtnChk(SP1K_BTN_MODE) != KEY_STATUS_PRESSED) {
				bUsbPwr = 0;
			} else {
				bUsbPwr = 1;
			}
		#else
			if (sp1kBtnChk(SP1K_BTN_S2) != KEY_STATUS_PRESSED) {
				bUsbPwr = 0;
			} else {
				bUsbPwr = 1;
			}
		#endif

		if (prjDbgMode) {
			bUsbPwr = !bUsbPwr;
		}
	#endif //ICAT_OPTION

	sp1kAdcInit();  // 初始化 adc gpio
}

/**
* @param    None
* @retval   None
* @brief    USB初始化函数 -- 2011-2-15
**/
static void usbInit(void)
{
	G_ucPktStorIntrf = K_StorageInterface1; // 当前数据包属于哪个存储接口
	G_ucCtrlPhase = K_CommandPhase;
	G_ucBulkPhase = K_CommandPhase;
	G_USBAudioSetInterface = 0; // CX 添加

#if (USBMSDC_OPTION == 2)
	G_USBMSDC_usSizeProcessed = 0;
	G_USBMSDC_ucSenseCodeIdx = 0x00;
	G_USBMSDC_ucCSWStatus = K_USBMSDC_CommandPassed;
	G_UIStatus = 0; // K_UISTATUS_USBMODE_PCCAM; 用于prjmode！用于视频 pv size 6/8
#endif

#if 0
	gUsbMode = K_UIOPTION_STORAGE_SIDC;
#else
	gUsbMode = K_UIOPTION_STORAGE_MSDC;
#endif

	G_MLUN_CardSupport = K_CARD_TYPE;

	XBYTE[REG_Usb_TransactionEn] &= 0xf9; // 拒绝(NAK)
	// 下一个EP0 Out数据包
	// 下一个EP0 In数据包

	XBYTE[REG_Usb_EpAckIntSts] = 0xff; // 清除所有状态
	XBYTE[REG_Usb_IntSts] = 0xff; // 清除状态
	XBYTE[REG_Usb_EpAckIntEn] = 0x07; // 使能中断
	// EP0 Setup数据包 OK
	// EP0 Out数据包 OK
	// EP0 In数据包 OK
	XBYTE[REG_Usb_IntEn] |= 0x01; // USBRstVTEn
#if 1
	XBYTE[REG_Usb_Status] = 0x00; // 00:启用HS，02:强制FS，禁用SoftPlugOut !!
#else
	XBYTE[REG_Usb_Status] = 0x02; // 00:启用HS，02:强制FS，禁用SoftPlugOut !!
#endif
	// XBYTE[0x2045] |= 0x01; // 使能GPIO8下降沿中断 (USB检测器)
	// XBYTE[0x207B] &= ~0x01; // 清除GPIO8下降沿事件
	XBYTE[0x2044] |= 0x08; // 使能GPIO3下降沿中断 (USB检测器)
	XBYTE[0x207A] &= ~0x08; // 清除GPIO3下降沿事件

#if(K_AUDIOINTER)
    XBYTE[REG_Usb_Alt_If1_NumMax] = 0x05; // 设置交替最大为5
    XBYTE[REG_Usb_UsbMode] = 0x01;  // 0x256F
    XBYTE[REG_Usb_LogicEp1_Disable] = 0xe0;  // 0x2530
#else
    XBYTE[REG_Usb_Alt_If1_NumMax] = 0x08; // 设置交替最大为8
    XBYTE[REG_Usb_UsbMode] = 0x03;
    XBYTE[REG_Usb_LogicEp1_Disable] = 0x00;
#endif

	HAL_UsbSwPlugOut();
    G_UI_USBConnect = K_UI_USB_DISCONNECT;
}

/**
* @param    None
* @retval   None
* @brief    主机初始化函数 -- 2011-2-15
**/
static void hostInit(void)
{
	UINT8 cnt = 0;
	pFuncInit* pf = &funcInitArray[0];

	/* 检查数组大小 */
	// ASSERT(sizeof(funcInitArray) / sizeof(pFuncInit) < FUNC_SYS_INIT_MAX, 0);

	while (*pf) 
    {
		(*pf)();
		pf++;
		if (++cnt >= FUNC_SYS_INIT_MAX) {
			/* funcInitArray 配置错误 */
			ASSERT(0, 0);
		}
	}
}

/**
* @param    None
* @retval   None
* @brief    全局初始化函数 -- 2011-2-15
**/
void globalInit(void)
{
	pFuncInit *pFuncInitMain = &funcInitMain;  // 函数指针赋值 
	uartInit(); // 串口初始化
	cpuInit();  // CPU初始化
	dramInit(); // DRAM 相关功能初始化
	gpioInit(); // GPIO 初始化
	hostInit(); // 主机初始化回调

#if 0 //for NEW ZOOM function 20131213
	zoomDevInit();     /* 新增用于 v2.1.8 兼容性的功能 */
	sp1kZoomDevActSet(SP1K_ZOOM_LOGI_VIDEO_MODE, SP1K_ZOOM_LOGI_NONBLOCKING_TYPE);   // 修复 mantis 45825
#endif

	usbInit();  // USB 初始化
    
	osInit();   // 系统基础服务初始化
	osTaskCreate(osTaskIdle, OS_PRIO_ACTIVE_FUNC);
	osMsgInit();
	cmdInit();

	sp1kDevInit();  // sp1k设备属性初始化
	sp1kAudioPlayInit();
	sp1kRTCInit();
	sp1kDiskMemInit();

	ASSERT(pFuncInitMain, 0);  /* 跳转到主机设备初始化流程 */
	(*pFuncInitMain)(); // 设备初始化devInit
	HAL_TpPreCfg();
}