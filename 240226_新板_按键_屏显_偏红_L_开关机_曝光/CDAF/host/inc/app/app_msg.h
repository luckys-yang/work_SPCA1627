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
 * @file      app_msg.h
 * @brief     message definition for host
 * @author    Phil Lin
 * @since     2010-01-01
 * @date      2010-01-01
*/

#ifndef __APP_MSG_H__
#define __APP_MSG_H__

/**************************************************************************
 *                        H E A D E R   F I L E S
 **************************************************************************/
#include "os_msg.h" 
#include "host_func_opt.h"
/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/
 
/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
 
/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

typedef enum
{
    SP1K_MSG_START_HOST = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0000, // 0x0000

    /* mode switch msg */
    
    // ��ʼ����Ϣ
    SP1K_MSG_STATE_INIT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0001,          
    // ����״̬��Ϣ
    SP1K_MSG_STATE_TO_POWER_ON = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0002,   
    // ������ͼ״̬��Ϣ
    SP1K_MSG_STATE_TO_STILL_VIEW = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0003, 
    // ¼����ͼ״̬��Ϣ
    SP1K_MSG_STATE_TO_VIDEO_VIEW = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0004, 
    // ��Ƶ¼��״̬��Ϣ
    SP1K_MSG_STATE_TO_AUDIO_REC = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0005,  
    // ���ջط���Ϣ
    SP1K_MSG_STATE_TO_STILL_PLAY = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0006, 
    // ��ý��ط���Ϣ
    SP1K_MSG_STATE_TO_MULTI_PLAY = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0007, 
    // ��Ƶ�ط���Ϣ
    SP1K_MSG_STATE_TO_VIDEO_PLAY = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0008, 
    // ��Ƶ�ط���Ϣ
    SP1K_MSG_STATE_TO_AUDIO_PLAY = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0009, 
    // �˵�״̬��Ϣ
    SP1K_MSG_STATE_TO_MENU = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x000a,       
    // USBģʽ��Ϣ
    SP1K_MSG_STATE_TO_USB = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x000b,        
    // �õ�Ƭ����ģʽ��Ϣ
    SP1K_MSG_STATE_TO_SLIDE_SHOW = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x000c, 
    // ȫ������ģʽ��Ϣ
    SP1K_MSG_STATE_TO_PAN_SHOW = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x000d,   
    // ���� DPS ģʽ��Ϣ
    SP1K_MSG_STATE_TO_DPS = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x000e,        
    // ����״̬��Ϣ
    SP1K_MSG_STATE_TO_IDLE = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x000f,       
    // �ػ���Ϣ
    SP1K_MSG_STATE_TO_POWER_OFF = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0010,  
    SP1K_MSG_STATE_MAX = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0011,           

    /* GPIO msg */
    // SD��������Ϣ
    SP1K_MSG_CARD_PLUG_IN = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0400,      
    // SD���γ���Ϣ
    SP1K_MSG_CARD_PLUG_OUT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0401,     
    // USB������Ϣ
    SP1K_MSG_USB_PLUG_IN = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0402,       
    // USB�γ���Ϣ
    SP1K_MSG_USB_PLUG_OUT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0403,      
    // TV������Ϣ
    SP1K_MSG_TV_PLUG_IN = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0404,        
    // TV�γ���Ϣ
    SP1K_MSG_TV_PLUG_OUT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0405,       
    // ��ص���������Ϣ
    SP1K_MSG_BAT_LV_UPDATE = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0406,     
    // ��ص�������Ϣ
    SP1K_MSG_BAT_EMPTY = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0407,         
    // HDMI������Ϣ
    SP1K_MSG_HDMI_PLUG_IN = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0408,      
    // HDMI�γ���Ϣ
    SP1K_MSG_HDMI_PLUG_OUT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0409,     
    // ����س�����Ϣ
    SP1K_MSG_CHG_BATTERY_FULL = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x040a,  
    // ����ض�·��Ϣ
    SP1K_MSG_CHG_BATTERY_SHORT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x040b, 
    // �����Զ��Խ�������Ϣ
    SP1K_MSG_UPDATE_AF_WINDOW = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x040c,  

#if SUPPORT_AF
    // AF��ʼ��Ϣ
    SP1K_MSG_SYS_AF_START = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x040d,   
    // AFУ׼��Ϣ
    SP1K_MSG_SYS_AF_CAL = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x040e,     
    // AF�ɹ���Ϣ
    SP1K_MSG_SYS_AF_SUCCESS = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x040f, 
    // AFʧ����Ϣ
    SP1K_MSG_SYS_AF_FAIL = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0410,    
#endif
    // AF�˳���Ϣ
    SP1K_MSG_SYS_AF_BACK_FAR = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0411, 
// ��֧����Ԥ��������ʾ��ǰʱ�䡿
#if SUPPORT_RTC_OSD_UPDATE
    // ����RTCʱ����Ϣ
    SP1K_MSG_UPDATE_RTCTIME = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0412, 
#endif
    /* power msg */
    // ������Ϣ
    SP1K_MSG_POWER_ON = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0600,      
    // �ػ���Ϣ
    SP1K_MSG_POWER_OFF = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0601,     
    // �ر���Ϣ
    SP1K_MSG_SHUT_DOWN = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0602,     
    // ���������Ϣ 
    SP1K_MSG_ENTER_SUSPEND = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0603, 

    /*TP msg*/
    // ����ģʽ�л���Ϣ
    SP1K_MSG_TP_MODE_SWITCH = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0700,  
    // ���� S2 ��Ϣ
    SP1K_MSG_TP_S2 = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0701,          
    // �����˵���Ϣ
    SP1K_MSG_TP_MENU = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0702,
    // ��������Ϣ
    SP1K_MSG_TP_UP = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0703,           
    // ��������Ϣ
    SP1K_MSG_TP_DOWN = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0704,         
    // ��������Ϣ
    SP1K_MSG_TP_LEFT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0705,         
    // ��������Ϣ
    SP1K_MSG_TP_RIGHT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0706,        
    // ����ȷ����Ϣ
    SP1K_MSG_TP_OK = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0707,           
    // �����佹�Ŵ���Ϣ
    SP1K_MSG_TP_TELE = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0708, 
    // �����佹��С��Ϣ        
    SP1K_MSG_TP_WIDE = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0709,         
    // �����Ϣ
    SP1K_MSG_TP_HIT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x070A,          
    // ����У׼�����Ϣ
    SP1K_MSG_TP_CAL_COMPLETE = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x070B, 
    SP1K_MSG_TP_MAX = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x070F,          
    /* disk msg */

    // SD��������Ϣ
    SP1K_MSG_CARD_ERROR = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a00,          
    // NAND������Ϣ
    SP1K_MSG_NAND_ERROR = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a01,          
    // SD����֧����Ϣ
    SP1K_MSG_CARD_NOT_SUPPORT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a02,    
    // SD��DCFFOLDER����Ϣ  
    SP1K_MSG_CARD_DCFFOLDER_FULL = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a03, 
// ��֧��������
#if SUPPORT_PANEL_PROTECT
    // ��屣����Ϣ
    SP1K_MSG_PANEL_PROTECT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a04,           
    // �ر���Ļ������Ϣ 
    SP1K_MSG_SYS_CLOSE_LCD_BACKLIGHT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a05, 
    // ����Ļ������Ϣ
    SP1K_MSG_SYS_OPEN_LCD_BACKLIGHT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a06,  
#endif
    // ����ɾ����Ϣ
    SP1K_MSG_SYS_QUICK_DELETE = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a07, 
    // ��������Ϣ
    SP1K_MSG_Adjust_Macro = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a08,    
// ����ʱ��������Ƿ����仯��
#if SUPPORT_AWB_ADJ
    // AWB�����Ϣ
    SP1K_MSG_AWB_CHECK = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a09, 
    // �ر�AWB��Ϣ
    SP1K_MSG_AWB_OFF = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a0a, 
#endif
    // ��ʾ�汾��Ϣ
    SP1K_MSG_Version_Show = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a0b,  
    // ����汾��Ϣ
    SP1K_MSG_Version_Clear = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a0c, 

    /* unknown msg */

    // δ֪������Ϣ
    SP1K_MSG_UNKNOWN_HOST = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0fff, 
} appMsgList_t;

typedef enum btnMsgList_e
{
    SP1K_MSG_START_BTN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0000, // ��ʼ��ť��Ϣ

    /* UI������Ϣ */

    SP1K_MSG_KEY_PRESS_S1 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0100,       // S1����������Ϣ
    SP1K_MSG_KEY_PRESS_S2 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0101,       // S2����������Ϣ
    SP1K_MSG_KEY_PRESS_MENU = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0102,     // �˵�����������Ϣ
    SP1K_MSG_KEY_PRESS_UP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0103,       // �ϰ���������Ϣ
    SP1K_MSG_KEY_PRESS_DOWN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0104,     // �°���������Ϣ
    SP1K_MSG_KEY_PRESS_LEFT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0105,     // �󰴼�������Ϣ
    SP1K_MSG_KEY_PRESS_RIGHT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0106,    // �Ұ���������Ϣ
    SP1K_MSG_KEY_PRESS_OK = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0107,       // ȷ�ϰ���������Ϣ
    SP1K_MSG_KEY_PRESS_TELE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0108,     // �佹�Ŵ󰴼�������Ϣ
    SP1K_MSG_KEY_PRESS_WIDE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0109,     // �佹��С����������Ϣ
    SP1K_MSG_KEY_PRESS_DISP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x010a,     // ��ʾ����������Ϣ
    SP1K_MSG_KEY_PRESS_F1 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x010b,       // F1����������Ϣ
    SP1K_MSG_KEY_PRESS_F2 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x010c,       // F2����������Ϣ
    SP1K_MSG_KEY_PRESS_PB = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x010d,       // �طŰ���������Ϣ
    SP1K_MSG_KEY_PRESS_POWER = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x010e,    // ��Դ����������Ϣ
    SP1K_MSG_KEY_PRESS_MODE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x010f,     // ģʽ����������Ϣ
    SP1K_MSG_KEY_PRESS_SNAP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0110,     // ���հ���������Ϣ
    SP1K_MSG_KEY_PRESS_SENSOR = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0111,   // ����������������Ϣ
    SP1K_MSG_KEY_PRESS_AF = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0112,       // �Զ��Խ�����������Ϣ
    SP1K_MSG_KEY_PRESS_ZOOM_IN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0113,  // �Ŵ󰴼�������Ϣ
    SP1K_MSG_KEY_PRESS_ZOOM_OUT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0114, // ��С����������Ϣ
    SP1K_MSG_KEY_PRESS_BURST = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0115,    // ���İ���������Ϣ
    SP1K_MSG_KEY_PRESS_BACK = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0116,     // ���ذ���������Ϣ
    SP1K_MSG_KEY_PRESS_MAX = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x01ff,      // ����������Ϣ���ֵ

    SP1K_MSG_KEY_RELEASE_S1 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0200,       // S1�����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_S2 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0201,       // S2�����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_MENU = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0202,     // �˵������ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_UP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0203,       // �ϰ����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_DOWN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0204,     // �°����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_LEFT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0205,     // �󰴼��ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_RIGHT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0206,    // �Ұ����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_OK = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0207,       // ȷ�ϰ����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_TELE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0208,     // �佹�Ŵ󰴼��ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_WIDE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0209,     // �佹��С�����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_DISP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x020a,     // ��ʾ�����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_F1 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x020b,       // F1�����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_F2 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x020c,       // F2�����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_PB = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x020d,       // �طŰ����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_POWER = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x020e,    // ��Դ�����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_MODE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x020F,     // ģʽ�����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_SNAP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0210,     // ���հ����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_SENSOR = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0211,   // �����������ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_AF = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0212,       // �Զ��Խ������ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_ZOOM_IN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0213,  // �Ŵ󰴼��ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_ZOOM_OUT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0214, // ��С�����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_BURST = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0215,    // ���İ����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_BACK = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0216,     // ���ذ����ͷ���Ϣ
    SP1K_MSG_KEY_RELEASE_MAX = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0217,      // �����ͷ���Ϣ���ֵ

    SP1K_MSG_KEY_REPEATE_UP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0300,       // �ϰ���������Ϣ
    SP1K_MSG_KEY_REPEATE_DOWN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0301,     // �°���������Ϣ
    SP1K_MSG_KEY_REPEATE_LEFT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0302,     // �󰴼�������Ϣ
    SP1K_MSG_KEY_REPEATE_RIGHT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0303,    // �Ұ���������Ϣ
    SP1K_MSG_KEY_REPEATE_TELE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0304,     // �佹�Ŵ󰴼�������Ϣ
    SP1K_MSG_KEY_REPEATE_WIDE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0305,     // �佹��С����������Ϣ
    SP1K_MSG_KEY_REPEATE_PB = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0306,       // �طŰ���������Ϣ
    SP1K_MSG_KEY_REPEATE_POWER = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0307,    // ��Դ����������Ϣ
    SP1K_MSG_KEY_REPEATE_MENU = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0308,     // �˵�����������Ϣ
    SP1K_MSG_KEY_REPEATE_SNAP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0309,     // ���հ���������Ϣ
    SP1K_MSG_KEY_REPEATE_SENSOR = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x030a,   // ����������������Ϣ
    SP1K_MSG_KEY_REPEATE_AF = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x030b,       // �Զ��Խ�����������Ϣ
    SP1K_MSG_KEY_REPEATE_ZOOM_IN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x030c,  // �Ŵ󰴼�������Ϣ
    SP1K_MSG_KEY_REPEATE_ZOOM_OUT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x030d, // ��С����������Ϣ
    SP1K_MSG_KEY_REPEATE_BURST = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x030e,    // ���İ���������Ϣ
    SP1K_MSG_KEY_REPEATE_BACK = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x030f,     // ���ذ���������Ϣ
    SP1K_MSG_KEY_REPEATE_MAX = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0310,      // ����������Ϣ���ֵ

    SP1K_MSG_KEY_MAX = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0400, // ������Ϣ���ֵ

    SP1K_MSG_UNKNOWN_BTN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0fff, // δ֪������Ϣ
} btnMsgList_t;

/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/
 
/**************************************************************************
 *                 E X T E R N A L    R E F E R E N C E S                 *
 **************************************************************************/
 
/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

#endif  /* __APP_MSG_H__ */

