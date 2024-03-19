/***************************************************************************
 * File: xxx.c
 * Author: Yang
 * Date: 2024-03-08 11:17:59
 * description: 
 -----------------------------------
电池检测相关应用程序
 -----------------------------------
****************************************************************************/
#include "general.h"

#include "os_msg.h"
#include "app_msg.h"
#include "main.h"
#include "gpio.h"
#include "gpio_api.h"
#include "key_scan.h"
#include "sp1k_disp_api.h"	 
#include "sp1k_osd_api.h"
#include "sp1k_util_api.h"
#include "sp1k_tmr_svc.h"
#include "sp1k_gpio_api.h"		  
#include "app_ui_para.h"
#include "app_menu_micro.h"
#include "app_main.h"
#include "app_battery.h"
#include "app_sts_pwroff.h"
#include "dbg_def.h"
#include "sp1k_hal_api.h"
#include "initio.h"
#include "app_dev_usb.h"
#include "app_ui_osd.h"

 /*定义电池检测的间隙值*/
#define BATT_DETECT_DEV	0   //10

/*
注：客户在此定义电池电量、
当主机收到 msg "SP1K_MSG_BAT_LV_UPDATE" 时
您可以通过 "appBatLvGet "获取电池电量
*/

static UINT16 xdata battAdcVal[] = 
{
	POWER_BAT_LV_FULL,
	POWER_BAT_LV_HALF,
	POWER_BAT_LV_LOW,
	POWER_BAT_LV_EMPTY,
};

static powerBatLv_t xdata G_BatteryLevel = POWER_BAT_FULL;

xdata UINT8 chargeMode;

/* Public function prototypes=========================================================*/

void appBatLvGet(powerBatLv_t* batlv);
void appBattInit(void);
UINT8 appBattStsHandle(void);


/**
* @param    *batlv: 存储获取到的电池电量值
* @retval   None
* @brief    获取电池电量信息
**/
void appBatLvGet(powerBatLv_t* batlv)
{
    scanTag_t* pAdcHdl;

    // 获取ADC句柄
    sp1kAdcHdlGet(&pAdcHdl);

    // 将电池电量值赋给传入的指针变量
    *batlv = pAdcHdl->battAdc.battLvl;
    //printf("Lv(%bu)\n", *batlv);
}

/**
* @param    None
* @retval   None
* @brief    初始化电池检测功能
**/
void appBattInit(void)
{
	UINT8 battLv; // 定义一个无符号8位整数变量battLv，用于存储电池电量等级。

	/*Battery detect configure*/
	battDetectCfg(sizeof(battAdcVal) / sizeof(UINT16), &battAdcVal[0], BATT_DETECT_DEV); 
	// 配置电池检测。计算battAdcVal数组的长度，并传递给电池检测配置函数，以及电池检测设备标识符。

	/*Battery detect initialize, The value returned is the current battery level*/
	battLv = battDetectInit(); 
	// 初始化电池检测，并获取当前电池电量等级。

	/*Enable battery detect function*/
	sp1kBattEnable(1); 
	// 启用电池检测功能。参数1表示启用。

	/*Disabled battery detect in power on process on EVB hw solution*/
	// 这里是条件编译部分，已被注释掉。在特定的硬件解决方案(EVB)中可能需要禁用开机时的电池检测。
	if (sp1kUSB_PlugGet() != 0) {
	// if (!gpioByteInGet(GPIO_BYTE_DISP0, 0x02, 1)) {
		/*If power on with the USB power, disable the battery detect function*/
		sp1kBattEnable(DISABLE); 
		// 如果是通过USB电源开机，禁用电池检测功能。DISABLE宏通常定义为0，表示禁用。
	} else {
		if (battLv >= POWER_BAT_BELOWEMPTY) {
			/*Low battery detected in power on process*/
			printf("Low battery!\n"); 
			// 在开机过程中检测到低电量，打印“Low battery!”信息。

			// appPowerShutDown(); 
			// 如果需要，可以在这里调用电源关闭函数，但这行代码被注释了。

			appPowerHoldPullDown(); 
			// 执行低电量处理，具体动作依赖于此函数的实现，可能是进入电量保持状态。
		}
	}
	//#endif
}

/**
* @param    None
* @retval   None
* @brief    处理电池状态
**/
UINT8 appBattStsHandle(void)
{
    powerBatLv_t batlv;

    // 获取电池电量信息
    appBatLvGet(&batlv);

    // 如果电量低于或等于预设的阈值
    if (batlv >= POWER_BAT_BELOWEMPTY) 
    {
#if BATT_LOW_PWR_OFF
        /* 检查是否正在执行关机流程 */
        if (!sp1kPwrOffProcChk()) 
        {
            /* 如果没有，则配置并启动关机流程 */
            sp1kPwrOffCfg(1, BATT_LOW_PWR_OFF_DELAY); // 5秒
        }
#else
        // 更新UI显示电池为空
        uiUpdateOSDBatEmpty();
        // 发送关机消息给操作系统
        osMsgPost(SP1K_MSG_POWER_OFF);
        // 关闭所有按钮
        sp1kBtnDisableAll();
#endif
    }

    // 返回电池电量
    return batlv;
}

void appChargeModeChk(void)
{
#if (_HW_SOLUTION_ != _HW_EVB_)
	if (prjDbgMode) {
		if (sp1kBtnChk(SP1K_BTN_POWER) == KEY_STATUS_PRESSED) {
			appChargeModeSet(1);
		} else {
			appChargeModeSet(0);
		}
	} else {//normal mode
		if (!bUsbPwr) {
			if (sp1kUSB_PlugGet() == USB_PLUG_IN) {
				appChargeModeSet(1);
			} else {
				appChargeModeSet(0);
			}			
		}
	}
#endif
}

void appChargeModeSet(UINT8 value)
{
	chargeMode=value;

}

UINT8  appChargeModeGet(void)
{
	return chargeMode;
}


void appChargePinCfg(void)
{
#if (_HW_SOLUTION_ == _HW_RDK_A_)
	gpioByteDirSet(GPIO_BYTE_FM2,0x02,0x02);// fmgpio 17 
	gpioByteOutSet(GPIO_BYTE_FM2,0x02,0x02);//disable charge 
	gpioByteDirSet(GPIO_BYTE_FM1,0x40,0x00);//fmgpio 14 charge done detect pin
	gpioPullSelSet(1,0x40,0x40);
	gpioPullEnSet(1,0x40,0x40);
#elif((_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_) ||(_HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_))
	gpioByteFuncSet(GPIO_BYTE_GEN2,0x04,0x04);
	gpioByteDirSet(GPIO_BYTE_GEN2,0x04,0x04);//gpio 18
	gpioByteOutSet(GPIO_BYTE_GEN2,0x04,0x00);//disable charge 
#endif
}

void appChargeEnable(void)
{
#if (_HW_SOLUTION_ == _HW_RDK_A_)
		gpioByteOutSet(GPIO_BYTE_FM2,0x02,0x00);// fmgpio 17 batter charge en
#elif((_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_) ||(_HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_))
		gpioByteOutSet(GPIO_BYTE_GEN2,0x04,0x04);//gpio18 charge en
#endif

}
void appChargeDisable(void)
{
#if (_HW_SOLUTION_ == _HW_RDK_A_)
		gpioByteOutSet(GPIO_BYTE_FM2,0x02,0x02);// fmgpio 17 batter charge disable
#elif((_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_) ||(_HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_))
		if(!appChargeModeGet())
		{
		gpioByteOutSet(GPIO_BYTE_GEN2,0x04,0x00);//gpio18 charge disable
		}
#endif

}
void appBatteryChgDetect(void)
{

#if (_HW_SOLUTION_ == _HW_RDK_A_)
	static UINT8 chgOver=0xFF;
	static UINT8 gpioValue=0xff;
	static UINT8 time;
	
	if(appCurrStateGet() != APP_STATE_IDLE)
	{
		return;
	}
	gpioByteOutSet(GPIO_BYTE_FM2,0x02,0x00);//charge enable 
	if(gpioValue != gpioByteInGet(GPIO_BYTE_FM1,0x40,1))//fmgpio14     0x10:batter charge done	0:charging
	{
		gpioValue= gpioByteInGet(GPIO_BYTE_FM1,0x40,1);
		time=0;
	}
	else
	{
		time++;
	}
	if(time>10)
	{
		time=0;
		if(chgOver != gpioValue)
		{
			chgOver=gpioValue;
			if(chgOver)
			{
				osMsgPost(SP1K_MSG_CHG_BATTERY_FULL);
			}
			else
			{
				osMsgPost(SP1K_MSG_CHG_BATTERY_SHORT);
			}

		}
	}
	
			
#elif((_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_) ||(_HW_SOLUTION_ == _HW_CUSTOM_MODEL_B_))
	UINT16 adcVal=0;
	UINT8 sts=0;
	static UINT16 preMsg = SP1K_MSG_NULL;
	UINT8 reg2649=0;
	static UINT8 timeH=0;
	static UINT8 timeL=0;
	if(appCurrStateGet() != APP_STATE_IDLE)
	{
		return;
	}
	ENTER_CRITICAL(sts);
	reg2649 = sp1kADCSarGet;
	sp1kAdcValGet(0x00,&adcVal);
	sp1kADCSarSel(reg2649);
	EXIT_CRITICAL(sts);
	if(adcVal > 640)
	{
		timeH++;
		timeL=0;
		if(timeH<5)
		{
			return;
		}
		else
		{
			timeH=0;
		}
		if(preMsg != SP1K_MSG_CHG_BATTERY_FULL)	
		{
			osMsgPost(SP1K_MSG_CHG_BATTERY_FULL);
			preMsg=SP1K_MSG_CHG_BATTERY_FULL;
	}
	}
	if(adcVal < 620)
	{
		timeL++;
		timeH=0;
		if(timeL<5)
		{
			return;
		}
		else
		{
			timeL=0;
		}
		if(preMsg != SP1K_MSG_CHG_BATTERY_SHORT)
		{
			osMsgPost(SP1K_MSG_CHG_BATTERY_SHORT);
			preMsg=SP1K_MSG_CHG_BATTERY_SHORT;
	}
	}
#endif
}

