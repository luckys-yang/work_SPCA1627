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
#define KEY_SCAN_ADC_A_VAL_IDLE (KEY_SCAN_ADC_VAL_MIN + 0x2FDU) // ��ť����ֵ800
#else
#define KEY_SCAN_ADC_A_VAL_IDLE (KEY_SCAN_ADC_VAL_MIN + 0x2FD)
#endif

#if (_HW_SOLUTION_ == _HW_EVB_ || _HW_SOLUTION_ == _HW_TK_MODEL_A_)
#define KEY_SCAN_ADC_B_VAL_IDLE (KEY_SCAN_ADC_VAL_MIN + 0x0FU) // ��ť����ֵ50
#else
#define KEY_SCAN_ADC_B_VAL_IDLE (KEY_SCAN_ADC_VAL_MIN + 0x0fU)
#endif

// ���ڿ��ƴ������������ظ���Ӧ�ٶ�(ȡֵ��ΧΪ 0 �� 16������ 0 ��ʾû���ظ�������1 ��ʾ��Ƶ�����ظ�����)
#define TP_PEN_RPT 10
// ��ʾ�������״��ظ��������ӳ�ʱ��
#define TP_PEN_RPT_START 40
// ���ڿ��ư���ɨ��ʱ���ظ���Ӧ�ٶ�(ȡֵ��ΧΪ 0 �� 16������ 0 ��ʾû���ظ�������1 ��ʾ��Ƶ�����ظ�����)
#define KEY_SCAN_GPIO_VAL_RPT 1
// ������������ֵ(�����10ms����10*100=1000ms)
#define KEY_SCAN_GPIO_VAL_RPT_START 100

#define KEY_SCAN_ADC_VAL_RPT 2        // ����ɨ��: 10ms
#define KEY_SCAN_ADC_VAL_RPT_START 10 // 20

#define KEY_SCAN_ADC_VAL_DEC 2

/*-------------------- USER CODE: Custom Begin --------------------*/

/* ADC-A(SARIN0)��ťֵ����(1023 - Ҫ�����жϵ�ADֵ = ʵ��ADֵ) */
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

/* ADC-B(SARIN2)��ťֵ����(1023 - Ҫ�����жϵ�ADֵ = ʵ��ADֵ) */
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

/* ADC-A(SARIN0) ADֵ�ԳƷ�Χ*/
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
/* ADC-B(SARIN2) ADֵ�ԳƷ�Χ*/
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

/* ADC-A(SARIN0) ����*/
#if (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)
#define SP1K_ADC_BTN_A_NUM 4
#else
#define SP1K_ADC_BTN_A_NUM 1
#endif
/* ADC-B(SARIN2) ����*/
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

/*ADC ��ť������(ADC-A + ADC-B)*/
#define SP1K_ADC_BTN_NUM (SP1K_ADC_BTN_A_NUM + SP1K_ADC_BTN_B_NUM)

/*GPIO button status*/
/* --- 2024.3.11ע��(������û�õ���Щ��)
#define SP1K_GPIO_BTN_A_STATUS KEY_STATUS_RELEASED // Power
#define SP1K_GPIO_BTN_B_STATUS KEY_STATUS_RELEASED // S2
*/

/*GPIO button num*/
#if (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)
#define SP1K_GPIO_BTN_NUM 2
#else
/*GPIO/DGPIO ��ť����*/
#define SP1K_GPIO_BTN_NUM 4 // �ֱ���W1��SHUTTER��T1��AF(+1 only for 1626 evb)
#endif

// ����ʹ�õİ�������
#define SP1K_BTN_NUM (SP1K_ADC_BTN_NUM + SP1K_GPIO_BTN_NUM)

/*-------------------- USER CODE: Custom End --------------------*/

#define POWER_BAT_DEC 5
#define BATT_LOW_POWER_OFF_RETRY ((UINT16)100) // 100*30ms

#define UI_TEST  1

/* -----------------------����ö��/�ṹ��------------------------ */
// ����״̬ö��
typedef enum keyStatus_e
{
    KEY_STATUS_PRESSED = (unsigned char)0x01,  // ��������״̬
    KEY_STATUS_RELEASED = (unsigned char)0x02, // �����ͷ�״̬
    KEY_STATUS_REPEAT = (unsigned char)0x04,   // ��������״̬
} keyStatus_t;
// ��������ö��
typedef enum keyType_e
{
    KEY_TYPE_ADC_A = (unsigned char)0x01, // ADC A(SARIN0) ���Ͱ���
    KEY_TYPE_ADC_B = (unsigned char)0x02, // ADC B(SARIN2) ���Ͱ���
    KEY_TYPE_GPIO = (unsigned char)0x03,  // GPIO ���Ͱ���(GPIO��DGPIO)
} keyType_t;
// ������Ϣö��(��Ϣ��@keyMsg��)
typedef struct keyMsg_s
{
    UINT16 keyMsgPrs; // ����������Ϣ
    UINT16 keyMsgRpt; // ����������Ϣ
    UINT16 keyMsgRls; // �����ͷ���Ϣ
} keyMsg_t;

/* -----------------------�������Խṹ��------------------------ */
// ADC �������Խṹ��
typedef struct keyAttrAdc_s
{
    UINT8 keyNum; // �������
    UINT8 keyType; // �������ͣ�keyType_t��
    UINT8 *pKeyEn; // ָ�򰴼�ʹ��״̬��ָ��
} keyAttrAdc_t;
// GPIO �������Խṹ��
typedef struct keyAttrGpio_s
{
    UINT8 keyStsPrev; // ǰһ����״̬
    UINT8 *pKeyEn; // ָ�򰴼�ʹ��״̬��ָ��
} keyAttrGpio_t;

/* -----------------------�������ƽṹ��------------------------ */
// ADC �������ƽṹ��
typedef struct keyCtrlAdc_s
{
    UINT8 keyDec; // ������С
    UINT16 code *pKeyVal; // ָ�򰴼�ֵ��ADC����ָ��(��@keyAdcAVal @keyAdcBVal)
    UINT16 code *pKeyDev; // ָ�򰴼��豸��ָ��(��@keyAdcAValDevTbl @keyAdcBValDevTbl)
    UINT16 keyValPrev; // ǰһ����ֵ
    UINT16 keyValRpt; // ��������ֵ
} keyCtrlAdc_t;
// GPIO �������ƽṹ��
typedef struct keyCtrlGpio_s
{
    UINT8 keyDec; // ������С
    UINT16 keyValRpt; // ��������ֵ
} keyCtrlGpio_t;

/* -----------------------��Ԫ�ṹ��------------------------ */
// ADC ������Ԫ�ṹ��
typedef struct keyUnitAdc_s
{
    struct keyAttrAdc_s keyAttr; // ���� ADC ����
    struct keyMsg_s code *pKeyMsg; // ָ�򰴼���Ϣ��ָ��
    struct keyCtrlAdc_s keyCtrl; // ���� ADC ����
    UINT8 keyActiveIdx; // ����İ�������(��0��ʼ��Ҳ���ǰ����ĸ���)
} keyUnitAdc_t;
// GPIO ������Ԫ�ṹ��
typedef struct keyUnitGpio_s
{
    struct keyAttrGpio_s keyAttr[SP1K_GPIO_BTN_NUM]; // GPIO ������������
    struct keyMsg_s code *pKeyMsg; // ָ�򰴼���Ϣ��ָ��
    struct keyCtrlGpio_s keyCtrl; // ���� GPIO ����
} keyUnitGpio_t;
// ��ص��� ��Ԫ�ṹ��
typedef struct battUnitAdc_s
{
    UINT8 battLvl; // ��ص���
    UINT8 battDec; // ��ؼ�С
    UINT8 battDetectEn; // ��ؼ��ʹ��
    UINT8 battLvlTol; // ��ص����ݲ�
    UINT16 *pbattAdcVal; // ָ���� ADC ֵ��ָ��
    UINT16 battAdcDev; // ��� ADC �豸
} battUnitAdc_t;

// ���嵥Ԫģ��ṹ��
typedef struct scanTag_s
{
    struct keyUnitAdc_s keyAdcA; // ���� ADC-A ��Ԫ
    struct keyUnitAdc_s keyAdcB; // ���� ADC-B ��Ԫ
    struct keyUnitGpio_s keyGpio; // ���� GPIO/DGPIO ��Ԫ
    struct battUnitAdc_s battAdc; // ��� ADC ��Ԫ
} scanTag_t;


/*����*/
// �������а�����������ʹ��״̬����Ϊ 0
#define sp1kBtnDisableAll()	do {\
								sp1kBtnCfg(KEY_TYPE_ADC_A, 0xffffU, 0x00);\ 
								sp1kBtnCfg(KEY_TYPE_ADC_B, 0xfffeU, 0x00);\
								sp1kBtnCfg(KEY_TYPE_GPIO, 0xffffU, 0x00);\
							} while(0)
// �������а�����������ʹ��״̬����Ϊ 0xff
#define sp1kBtnEnableAll()	do {\
								sp1kBtnCfg(KEY_TYPE_ADC_A, 0xffffU, 0xff);\
								sp1kBtnCfg(KEY_TYPE_ADC_B, 0xffffU, 0xff);\
								sp1kBtnCfg(KEY_TYPE_GPIO, 0xffffU, 0xff);\
							} while(0)

// �������а������жϣ��������ж�ʹ��״̬����Ϊ 0
#define sp1kBtnDisableAllIntr()	do {\
								sp1kBtnCfgIntr(KEY_TYPE_ADC_A, 0xffffU, 0x00);\
								sp1kBtnCfgIntr(KEY_TYPE_ADC_B, 0xfffeU, 0x00);\
								sp1kBtnCfgIntr(KEY_TYPE_GPIO, 0xffffU, 0x00);\
							} while(0)
// �������а������жϣ��������ж�ʹ��״̬����Ϊ 0xff
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



