/**
 * @file      key_scan.h
 * @brief     Brief
 * @author    Phil
 * @since     2010-01-01
 * @date      2010-01-01
*/
#ifndef __KEY_SCAN_H__
#define __KEY_SCAN_H__

/**************************************************************************
 *                        H E A D E R   F I L E S
 **************************************************************************/
#include "button_def.h"
#include "solution.h"

/*-------------------- USER CODE: Include Begin --------------------*/

#include "User_Config.h"

/*-------------------- USER CODE: Include End --------------------*/

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

#define KEY_SCAN_ADC_VAL_MIN 0x0023U // 0x0010U

#define KEY_SCAN_ADC_VAL_MAX 0x03ffU
#if (_HW_SOLUTION_ == _HW_EVB_ || _HW_SOLUTION_ == _HW_TK_MODEL_A_)
#define KEY_SCAN_ADC_A_VAL_IDLE (KEY_SCAN_ADC_VAL_MIN + 0x2FDU) // 按钮波动值800
#else
#define KEY_SCAN_ADC_A_VAL_IDLE (KEY_SCAN_ADC_VAL_MIN + 0x2FD)
#endif

#if (_HW_SOLUTION_ == _HW_EVB_ || _HW_SOLUTION_ == _HW_TK_MODEL_A_)
#define KEY_SCAN_ADC_B_VAL_IDLE (KEY_SCAN_ADC_VAL_MIN + 0x0FU) // 按钮波动值50
#else
#define KEY_SCAN_ADC_B_VAL_IDLE (KEY_SCAN_ADC_VAL_MIN + 0x0fU)
#endif

// 用于控制触摸屏按键的重复响应速度(取值范围为 0 到 16，其中 0 表示没有重复按键，1 表示更频繁地重复按键)
#define TP_PEN_RPT 10
// 表示触摸屏首次重复按键的延迟时间
#define TP_PEN_RPT_START 40
// 用于控制按键扫描时的重复响应速度(取值范围为 0 到 16，其中 0 表示没有重复按键，1 表示更频繁地重复按键)
#define KEY_SCAN_GPIO_VAL_RPT 1
// 按键长按计数值(检测是10ms，则10*100=1000ms)
#define KEY_SCAN_GPIO_VAL_RPT_START 100

#define KEY_SCAN_ADC_VAL_RPT 2        // 按键扫描: 10ms
#define KEY_SCAN_ADC_VAL_RPT_START 10 // 20

#define KEY_SCAN_ADC_VAL_DEC 2

/*-------------------- USER CODE: Custom Begin --------------------*/

/* ADC-A(SARIN0)按钮值定义(1023 - 要进行判断的AD值 = 实际AD值) */
#if (_HW_SOLUTION_ == _HW_RDK_A_)
#define SP1K_ADC_BTN_A_S1 994 // 1010//1000		/* RIGHT */
#define SP1K_ADC_BTN_A_S2 924 // 946//931		/* LEFT */
#define SP1K_ADC_BTN_A_S3 827 // 852//835		/* DOWN */
#define SP1K_ADC_BTN_A_S4 676 // 696//680		/* UP */
#define SP1K_ADC_BTN_A_S5 519 // 530//524		/* TELE */
#define SP1K_ADC_BTN_A_S6 329 // 335 //332		/* WIDE */
#elif (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)
#define SP1K_ADC_BTN_A_S1 853 /* RIGHT */
#define SP1K_ADC_BTN_A_S2 696 /* LEFT */
#define SP1K_ADC_BTN_A_S3 536 /* DOWN */
#define SP1K_ADC_BTN_A_S4 341
#else
#define SP1K_ADC_BTN_A_S1 1023 // power(1023-1024=0)
// #define SP1K_ADC_BTN_A_S2			810		/*DEL*/
// #define SP1K_ADC_BTN_A_S3			1023	/*OK*/
// #define SP1K_ADC_BTN_A_S4			600		/*POWER*/
#endif

/* ADC-B(SARIN2)按钮值定义(1023 - 要进行判断的AD值 = 实际AD值) */
#if (_HW_SOLUTION_ == _HW_EVB_)
#define SP1K_ADC_BTN_B_S1 559  // RIGHT1(1023-559=464)
#define SP1K_ADC_BTN_B_S2 860  // UP1(1023-860=163)
#define SP1K_ADC_BTN_B_S3 715  // BACK1(1023-715=308)
#define SP1K_ADC_BTN_B_S4 93   // MODE1(1023-93=930)
#define SP1K_ADC_BTN_B_S5 409  // DOWN1(1023-409=614)
#define SP1K_ADC_BTN_B_S6 1023 // OK1(1023-1023=0)
#define SP1K_ADC_BTN_B_S7 275  // DEL1(1023-275=748)
#elif (_HW_SOLUTION_ == _HW_TK_MODEL_A_)
#define SP1K_ADC_BTN_B_S1 326 /* DISP */
#define SP1K_ADC_BTN_B_S2 490 /* MODE */
#define SP1K_ADC_BTN_B_S3 685 /* OK/MENU */

#elif (_HW_SOLUTION_ == _HW_TK_MODEL_B_)
#define SP1K_ADC_BTN_B_S1 170 // 224		/* POWER */
#define SP1K_ADC_BTN_B_S2 326 // 373		/* DISP */
#define SP1K_ADC_BTN_B_S3 490 // 506		/* MODE */
#define SP1K_ADC_BTN_B_S4 685 // 684		/* OK/MENU */

#elif (_HW_SOLUTION_ == _HW_RDK_A_)
#define SP1K_ADC_BTN_B_S1 329 // 340//333		/*menu */
#define SP1K_ADC_BTN_B_S2 676 // 680//682		/* PB */
// #define SP1K_ADC_BTN_B_S3			597		/* F1 */
#define SP1K_ADC_BTN_B_S3 519 // 530//527		/* OK */
#define SP1K_ADC_BTN_B_S4 184 // 180//185		/* POWER */
#elif (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)
#define SP1K_ADC_BTN_B_S1 341 /*menu */
#define SP1K_ADC_BTN_B_S2 696 /* mode */
#endif   

/* ADC-A(SARIN0) AD值对称范围*/
#if (_HW_SOLUTION_ == _HW_RDK_A_)
#define SP1K_ADC_BTN_DEV_A_S1 30
#define SP1K_ADC_BTN_DEV_A_S2 30
#define SP1K_ADC_BTN_DEV_A_S3 50
#define SP1K_ADC_BTN_DEV_A_S4 50
#define SP1K_ADC_BTN_DEV_A_S5 80
#define SP1K_ADC_BTN_DEV_A_S6 80
#elif (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)
#define SP1K_ADC_BTN_DEV_A_S1 60
#define SP1K_ADC_BTN_DEV_A_S2 60
#define SP1K_ADC_BTN_DEV_A_S3 50
#define SP1K_ADC_BTN_DEV_A_S4 25
#else
#define SP1K_ADC_BTN_DEV_A_S1 100
// #define SP1K_ADC_BTN_DEV_A_S2		100
// #define SP1K_ADC_BTN_DEV_A_S3		100
// #define SP1K_ADC_BTN_DEV_A_S4		100
#endif
/* ADC-B(SARIN2) AD值对称范围*/
#if (_HW_SOLUTION_ == _HW_EVB_)
#define SP1K_ADC_BTN_DEV_B_S1 50
#define SP1K_ADC_BTN_DEV_B_S2 45
#define SP1K_ADC_BTN_DEV_B_S3 50
#define SP1K_ADC_BTN_DEV_B_S4 50
#define SP1K_ADC_BTN_DEV_B_S5 50
#define SP1K_ADC_BTN_DEV_B_S6 45
#define SP1K_ADC_BTN_DEV_B_S7 50

#elif (_HW_SOLUTION_ == _HW_TK_MODEL_A_)
#define SP1K_ADC_BTN_DEV_B_S1 52
#define SP1K_ADC_BTN_DEV_B_S2 78
#define SP1K_ADC_BTN_DEV_B_S3 95
#elif (_HW_SOLUTION_ == _HW_TK_MODEL_B_)
#define SP1K_ADC_BTN_DEV_B_S1 29
#define SP1K_ADC_BTN_DEV_B_S2 52
#define SP1K_ADC_BTN_DEV_B_S3 78
#define SP1K_ADC_BTN_DEV_B_S4 95
#elif (_HW_SOLUTION_ == _HW_RDK_A_)
#define SP1K_ADC_BTN_DEV_B_S1 50
#define SP1K_ADC_BTN_DEV_B_S2 50
#define SP1K_ADC_BTN_DEV_B_S3 50
#define SP1K_ADC_BTN_DEV_B_S4 50
// #define SP1K_ADC_BTN_DEV_B_S5		20
#elif (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)
#define SP1K_ADC_BTN_DEV_B_S1 100
#define SP1K_ADC_BTN_DEV_B_S2 100

#endif

/* ADC-A(SARIN0) 数量*/
#if (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)
#define SP1K_ADC_BTN_A_NUM 4
#else
#define SP1K_ADC_BTN_A_NUM 1
#endif
/* ADC-B(SARIN2) 数量*/
#if (_HW_SOLUTION_ == _HW_EVB_)
#define SP1K_ADC_BTN_B_NUM 7
#elif (_HW_SOLUTION_ == _HW_TK_MODEL_A_)
#define SP1K_ADC_BTN_B_NUM 3
#elif (_HW_SOLUTION_ == _HW_TK_MODEL_B_)
#define SP1K_ADC_BTN_B_NUM 4
#elif (_HW_SOLUTION_ == _HW_RDK_A_)
#define SP1K_ADC_BTN_B_NUM 4
#elif (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)
#define SP1K_ADC_BTN_B_NUM 2
#endif

/*ADC 按钮总数量(ADC-A + ADC-B)*/
#define SP1K_ADC_BTN_NUM (SP1K_ADC_BTN_A_NUM + SP1K_ADC_BTN_B_NUM)

/*GPIO button status*/
/* --- 2024.3.11注释(工程里没用到这些宏)
#define SP1K_GPIO_BTN_A_STATUS KEY_STATUS_RELEASED // Power
#define SP1K_GPIO_BTN_B_STATUS KEY_STATUS_RELEASED // S2
*/

/*GPIO button num*/
#if (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)
#define SP1K_GPIO_BTN_NUM 2
#else
/*GPIO/DGPIO 按钮数量*/
#define SP1K_GPIO_BTN_NUM 4 // 分别是W1，SHUTTER，T1，AF(+1 only for 1626 evb)
#endif

// 板子使用的按键总数
#define SP1K_BTN_NUM (SP1K_ADC_BTN_NUM + SP1K_GPIO_BTN_NUM)

/*-------------------- USER CODE: Custom End --------------------*/

#define POWER_BAT_DEC 5
#define BATT_LOW_POWER_OFF_RETRY ((UINT16)100) // 100*30ms

#define UI_TEST  1

/* -----------------------基本枚举/结构体------------------------ */
// 按键状态枚举
typedef enum keyStatus_e
{
    KEY_STATUS_PRESSED = (unsigned char)0x01,  // 按键按下状态
    KEY_STATUS_RELEASED = (unsigned char)0x02, // 按键释放状态
    KEY_STATUS_REPEAT = (unsigned char)0x04,   // 按键长按状态
} keyStatus_t;
// 按键类型枚举
typedef enum keyType_e
{
    KEY_TYPE_ADC_A = (unsigned char)0x01, // ADC A(SARIN0) 类型按键
    KEY_TYPE_ADC_B = (unsigned char)0x02, // ADC B(SARIN2) 类型按键
    KEY_TYPE_GPIO = (unsigned char)0x03,  // GPIO 类型按键(GPIO和DGPIO)
} keyType_t;
// 按键消息枚举(消息在@keyMsg里)
typedef struct keyMsg_s
{
    UINT16 keyMsgPrs; // 按键按下消息
    UINT16 keyMsgRpt; // 按键长按消息
    UINT16 keyMsgRls; // 按键释放消息
} keyMsg_t;

/* -----------------------按键属性结构体------------------------ */
// ADC 按键属性结构体
typedef struct keyAttrAdc_s
{
    UINT8 keyNum; // 按键编号
    UINT8 keyType; // 按键类型（keyType_t）
    UINT8 *pKeyEn; // 指向按键使能状态的指针
} keyAttrAdc_t;
// GPIO 按键属性结构体
typedef struct keyAttrGpio_s
{
    UINT8 keyStsPrev; // 前一按键状态
    UINT8 *pKeyEn; // 指向按键使能状态的指针
} keyAttrGpio_t;

/* -----------------------按键控制结构体------------------------ */
// ADC 按键控制结构体
typedef struct keyCtrlAdc_s
{
    UINT8 keyDec; // 按键减小
    UINT16 code *pKeyVal; // 指向按键值（ADC）的指针(即@keyAdcAVal @keyAdcBVal)
    UINT16 code *pKeyDev; // 指向按键设备的指针(即@keyAdcAValDevTbl @keyAdcBValDevTbl)
    UINT16 keyValPrev; // 前一按键值
    UINT16 keyValRpt; // 按键长按值
} keyCtrlAdc_t;
// GPIO 按键控制结构体
typedef struct keyCtrlGpio_s
{
    UINT8 keyDec; // 按键减小
    UINT16 keyValRpt; // 按键长按值
} keyCtrlGpio_t;

/* -----------------------单元结构体------------------------ */
// ADC 按键单元结构体
typedef struct keyUnitAdc_s
{
    struct keyAttrAdc_s keyAttr; // 按键 ADC 属性
    struct keyMsg_s code *pKeyMsg; // 指向按键消息的指针
    struct keyCtrlAdc_s keyCtrl; // 按键 ADC 控制
    UINT8 keyActiveIdx; // 激活的按键索引(从0开始，也就是按键的个数)
} keyUnitAdc_t;
// GPIO 按键单元结构体
typedef struct keyUnitGpio_s
{
    struct keyAttrGpio_s keyAttr[SP1K_GPIO_BTN_NUM]; // GPIO 按键属性数组
    struct keyMsg_s code *pKeyMsg; // 指向按键消息的指针
    struct keyCtrlGpio_s keyCtrl; // 按键 GPIO 控制
} keyUnitGpio_t;
// 电池电量 单元结构体
typedef struct battUnitAdc_s
{
    UINT8 battLvl; // 电池电量
    UINT8 battDec; // 电池减小
    UINT8 battDetectEn; // 电池检测使能
    UINT8 battLvlTol; // 电池电量容差
    UINT16 *pbattAdcVal; // 指向电池 ADC 值的指针
    UINT16 battAdcDev; // 电池 ADC 设备
} battUnitAdc_t;

// 总体单元模块结构体
typedef struct scanTag_s
{
    struct keyUnitAdc_s keyAdcA; // 按键 ADC-A 单元
    struct keyUnitAdc_s keyAdcB; // 按键 ADC-B 单元
    struct keyUnitGpio_s keyGpio; // 按键 GPIO/DGPIO 单元
    struct battUnitAdc_s battAdc; // 电池 ADC 单元
} scanTag_t;


/*禁用*/
// 禁用所有按键，将按键使能状态设置为 0
#define sp1kBtnDisableAll()	do {\
								sp1kBtnCfg(KEY_TYPE_ADC_A, 0xffffU, 0x00);\ 
								sp1kBtnCfg(KEY_TYPE_ADC_B, 0xfffeU, 0x00);\
								sp1kBtnCfg(KEY_TYPE_GPIO, 0xffffU, 0x00);\
							} while(0)
// 启用所有按键，将按键使能状态设置为 0xff
#define sp1kBtnEnableAll()	do {\
								sp1kBtnCfg(KEY_TYPE_ADC_A, 0xffffU, 0xff);\
								sp1kBtnCfg(KEY_TYPE_ADC_B, 0xffffU, 0xff);\
								sp1kBtnCfg(KEY_TYPE_GPIO, 0xffffU, 0xff);\
							} while(0)

// 禁用所有按键的中断，将按键中断使能状态设置为 0
#define sp1kBtnDisableAllIntr()	do {\
								sp1kBtnCfgIntr(KEY_TYPE_ADC_A, 0xffffU, 0x00);\
								sp1kBtnCfgIntr(KEY_TYPE_ADC_B, 0xfffeU, 0x00);\
								sp1kBtnCfgIntr(KEY_TYPE_GPIO, 0xffffU, 0x00);\
							} while(0)
// 启用所有按键的中断，将按键中断使能状态设置为 0xff
#define sp1kBtnEnableAllIntr()	do {\
								sp1kBtnCfgIntr(KEY_TYPE_ADC_A, 0xffffU, 0xff);\
								sp1kBtnCfgIntr(KEY_TYPE_ADC_B, 0xffffU, 0xff);\
								sp1kBtnCfgIntr(KEY_TYPE_GPIO, 0xffffU, 0xff);\
							} while(0)

void keyScanInit(void);
void keyScan(void);
UINT8 sp1kBtnCfg(keyType_t BtnType, UINT16 BitMask, UINT8 Enable);
UINT8 sp1kBtnCfgIntr(keyType_t BtnType, UINT16 BitMask, UINT8 Enable);
void sp1kAdcHdlGet(const scanTag_t** pAdcHdl);
UINT8 battDetectInit(void);
UINT8 battDetectCfg(UINT8 battLvlTol, UINT16* pBattAdcVal, UINT16 battAdvValDev);
void sp1kBattEnable(UINT8 enable);
UINT8 sp1kBtnChk(UINT8 button);
#endif  /* __KEY_SCAN_H__ */



