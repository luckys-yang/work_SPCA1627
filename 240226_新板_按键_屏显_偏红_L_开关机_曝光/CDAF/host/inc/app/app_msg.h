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
    
    // 初始化消息
    SP1K_MSG_STATE_INIT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0001,          
    // 开机状态消息
    SP1K_MSG_STATE_TO_POWER_ON = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0002,   
    // 拍照视图状态消息
    SP1K_MSG_STATE_TO_STILL_VIEW = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0003, 
    // 录像视图状态消息
    SP1K_MSG_STATE_TO_VIDEO_VIEW = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0004, 
    // 音频录制状态消息
    SP1K_MSG_STATE_TO_AUDIO_REC = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0005,  
    // 拍照回放消息
    SP1K_MSG_STATE_TO_STILL_PLAY = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0006, 
    // 多媒体回放消息
    SP1K_MSG_STATE_TO_MULTI_PLAY = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0007, 
    // 视频回放消息
    SP1K_MSG_STATE_TO_VIDEO_PLAY = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0008, 
    // 音频回放消息
    SP1K_MSG_STATE_TO_AUDIO_PLAY = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0009, 
    // 菜单状态消息
    SP1K_MSG_STATE_TO_MENU = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x000a,       
    // USB模式消息
    SP1K_MSG_STATE_TO_USB = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x000b,        
    // 幻灯片播放模式消息
    SP1K_MSG_STATE_TO_SLIDE_SHOW = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x000c, 
    // 全景播放模式消息
    SP1K_MSG_STATE_TO_PAN_SHOW = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x000d,   
    // 进入 DPS 模式消息
    SP1K_MSG_STATE_TO_DPS = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x000e,        
    // 空闲状态消息
    SP1K_MSG_STATE_TO_IDLE = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x000f,       
    // 关机消息
    SP1K_MSG_STATE_TO_POWER_OFF = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0010,  
    SP1K_MSG_STATE_MAX = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0011,           

    /* GPIO msg */
    // SD卡插入消息
    SP1K_MSG_CARD_PLUG_IN = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0400,      
    // SD卡拔出消息
    SP1K_MSG_CARD_PLUG_OUT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0401,     
    // USB插入消息
    SP1K_MSG_USB_PLUG_IN = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0402,       
    // USB拔出消息
    SP1K_MSG_USB_PLUG_OUT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0403,      
    // TV插入消息
    SP1K_MSG_TV_PLUG_IN = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0404,        
    // TV拔出消息
    SP1K_MSG_TV_PLUG_OUT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0405,       
    // 电池电量更新消息
    SP1K_MSG_BAT_LV_UPDATE = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0406,     
    // 电池电量低消息
    SP1K_MSG_BAT_EMPTY = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0407,         
    // HDMI插入消息
    SP1K_MSG_HDMI_PLUG_IN = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0408,      
    // HDMI拔出消息
    SP1K_MSG_HDMI_PLUG_OUT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0409,     
    // 充电电池充满消息
    SP1K_MSG_CHG_BATTERY_FULL = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x040a,  
    // 充电电池短路消息
    SP1K_MSG_CHG_BATTERY_SHORT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x040b, 
    // 更新自动对焦窗口消息
    SP1K_MSG_UPDATE_AF_WINDOW = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x040c,  

#if SUPPORT_AF
    // AF开始消息
    SP1K_MSG_SYS_AF_START = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x040d,   
    // AF校准消息
    SP1K_MSG_SYS_AF_CAL = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x040e,     
    // AF成功消息
    SP1K_MSG_SYS_AF_SUCCESS = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x040f, 
    // AF失败消息
    SP1K_MSG_SYS_AF_FAIL = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0410,    
#endif
    // AF退出消息
    SP1K_MSG_SYS_AF_BACK_FAR = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0411, 
// 【支持在预览界面显示当前时间】
#if SUPPORT_RTC_OSD_UPDATE
    // 更新RTC时间消息
    SP1K_MSG_UPDATE_RTCTIME = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0412, 
#endif
    /* power msg */
    // 开机消息
    SP1K_MSG_POWER_ON = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0600,      
    // 关机消息
    SP1K_MSG_POWER_OFF = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0601,     
    // 关闭消息
    SP1K_MSG_SHUT_DOWN = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0602,     
    // 进入挂起消息 
    SP1K_MSG_ENTER_SUSPEND = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0603, 

    /*TP msg*/
    // 触摸模式切换消息
    SP1K_MSG_TP_MODE_SWITCH = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0700,  
    // 触摸 S2 消息
    SP1K_MSG_TP_S2 = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0701,          
    // 触摸菜单消息
    SP1K_MSG_TP_MENU = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0702,
    // 触摸上消息
    SP1K_MSG_TP_UP = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0703,           
    // 触摸下消息
    SP1K_MSG_TP_DOWN = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0704,         
    // 触摸左消息
    SP1K_MSG_TP_LEFT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0705,         
    // 触摸右消息
    SP1K_MSG_TP_RIGHT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0706,        
    // 触摸确定消息
    SP1K_MSG_TP_OK = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0707,           
    // 触摸变焦放大消息
    SP1K_MSG_TP_TELE = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0708, 
    // 触摸变焦缩小消息        
    SP1K_MSG_TP_WIDE = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0709,         
    // 点击消息
    SP1K_MSG_TP_HIT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x070A,          
    // 触摸校准完成消息
    SP1K_MSG_TP_CAL_COMPLETE = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x070B, 
    SP1K_MSG_TP_MAX = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x070F,          
    /* disk msg */

    // SD卡错误消息
    SP1K_MSG_CARD_ERROR = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a00,          
    // NAND错误消息
    SP1K_MSG_NAND_ERROR = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a01,          
    // SD卡不支持消息
    SP1K_MSG_CARD_NOT_SUPPORT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a02,    
    // SD卡DCFFOLDER满消息  
    SP1K_MSG_CARD_DCFFOLDER_FULL = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a03, 
// 【支持屏保】
#if SUPPORT_PANEL_PROTECT
    // 面板保护消息
    SP1K_MSG_PANEL_PROTECT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a04,           
    // 关闭屏幕背光消息 
    SP1K_MSG_SYS_CLOSE_LCD_BACKLIGHT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a05, 
    // 打开屏幕背光消息
    SP1K_MSG_SYS_OPEN_LCD_BACKLIGHT = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a06,  
#endif
    // 快速删除消息
    SP1K_MSG_SYS_QUICK_DELETE = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a07, 
    // 调整宏消息
    SP1K_MSG_Adjust_Macro = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a08,    
// 【定时检测亮度是否发生变化】
#if SUPPORT_AWB_ADJ
    // AWB检查消息
    SP1K_MSG_AWB_CHECK = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a09, 
    // 关闭AWB消息
    SP1K_MSG_AWB_OFF = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a0a, 
#endif
    // 显示版本消息
    SP1K_MSG_Version_Show = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a0b,  
    // 清除版本消息
    SP1K_MSG_Version_Clear = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0a0c, 

    /* unknown msg */

    // 未知主机消息
    SP1K_MSG_UNKNOWN_HOST = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0fff, 
} appMsgList_t;

typedef enum btnMsgList_e
{
    SP1K_MSG_START_BTN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0000, // 开始按钮消息

    /* UI按键消息 */

    SP1K_MSG_KEY_PRESS_S1 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0100,       // S1按键按下消息
    SP1K_MSG_KEY_PRESS_S2 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0101,       // S2按键按下消息
    SP1K_MSG_KEY_PRESS_MENU = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0102,     // 菜单按键按下消息
    SP1K_MSG_KEY_PRESS_UP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0103,       // 上按键按下消息
    SP1K_MSG_KEY_PRESS_DOWN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0104,     // 下按键按下消息
    SP1K_MSG_KEY_PRESS_LEFT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0105,     // 左按键按下消息
    SP1K_MSG_KEY_PRESS_RIGHT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0106,    // 右按键按下消息
    SP1K_MSG_KEY_PRESS_OK = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0107,       // 确认按键按下消息
    SP1K_MSG_KEY_PRESS_TELE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0108,     // 变焦放大按键按下消息
    SP1K_MSG_KEY_PRESS_WIDE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0109,     // 变焦缩小按键按下消息
    SP1K_MSG_KEY_PRESS_DISP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x010a,     // 显示按键按下消息
    SP1K_MSG_KEY_PRESS_F1 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x010b,       // F1按键按下消息
    SP1K_MSG_KEY_PRESS_F2 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x010c,       // F2按键按下消息
    SP1K_MSG_KEY_PRESS_PB = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x010d,       // 回放按键按下消息
    SP1K_MSG_KEY_PRESS_POWER = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x010e,    // 电源按键按下消息
    SP1K_MSG_KEY_PRESS_MODE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x010f,     // 模式按键按下消息
    SP1K_MSG_KEY_PRESS_SNAP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0110,     // 快照按键按下消息
    SP1K_MSG_KEY_PRESS_SENSOR = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0111,   // 传感器按键按下消息
    SP1K_MSG_KEY_PRESS_AF = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0112,       // 自动对焦按键按下消息
    SP1K_MSG_KEY_PRESS_ZOOM_IN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0113,  // 放大按键按下消息
    SP1K_MSG_KEY_PRESS_ZOOM_OUT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0114, // 缩小按键按下消息
    SP1K_MSG_KEY_PRESS_BURST = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0115,    // 连拍按键按下消息
    SP1K_MSG_KEY_PRESS_BACK = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0116,     // 返回按键按下消息
    SP1K_MSG_KEY_PRESS_MAX = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x01ff,      // 按键按下消息最大值

    SP1K_MSG_KEY_RELEASE_S1 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0200,       // S1按键释放消息
    SP1K_MSG_KEY_RELEASE_S2 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0201,       // S2按键释放消息
    SP1K_MSG_KEY_RELEASE_MENU = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0202,     // 菜单按键释放消息
    SP1K_MSG_KEY_RELEASE_UP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0203,       // 上按键释放消息
    SP1K_MSG_KEY_RELEASE_DOWN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0204,     // 下按键释放消息
    SP1K_MSG_KEY_RELEASE_LEFT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0205,     // 左按键释放消息
    SP1K_MSG_KEY_RELEASE_RIGHT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0206,    // 右按键释放消息
    SP1K_MSG_KEY_RELEASE_OK = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0207,       // 确认按键释放消息
    SP1K_MSG_KEY_RELEASE_TELE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0208,     // 变焦放大按键释放消息
    SP1K_MSG_KEY_RELEASE_WIDE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0209,     // 变焦缩小按键释放消息
    SP1K_MSG_KEY_RELEASE_DISP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x020a,     // 显示按键释放消息
    SP1K_MSG_KEY_RELEASE_F1 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x020b,       // F1按键释放消息
    SP1K_MSG_KEY_RELEASE_F2 = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x020c,       // F2按键释放消息
    SP1K_MSG_KEY_RELEASE_PB = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x020d,       // 回放按键释放消息
    SP1K_MSG_KEY_RELEASE_POWER = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x020e,    // 电源按键释放消息
    SP1K_MSG_KEY_RELEASE_MODE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x020F,     // 模式按键释放消息
    SP1K_MSG_KEY_RELEASE_SNAP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0210,     // 快照按键释放消息
    SP1K_MSG_KEY_RELEASE_SENSOR = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0211,   // 传感器按键释放消息
    SP1K_MSG_KEY_RELEASE_AF = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0212,       // 自动对焦按键释放消息
    SP1K_MSG_KEY_RELEASE_ZOOM_IN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0213,  // 放大按键释放消息
    SP1K_MSG_KEY_RELEASE_ZOOM_OUT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0214, // 缩小按键释放消息
    SP1K_MSG_KEY_RELEASE_BURST = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0215,    // 连拍按键释放消息
    SP1K_MSG_KEY_RELEASE_BACK = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0216,     // 返回按键释放消息
    SP1K_MSG_KEY_RELEASE_MAX = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0217,      // 按键释放消息最大值

    SP1K_MSG_KEY_REPEATE_UP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0300,       // 上按键长按消息
    SP1K_MSG_KEY_REPEATE_DOWN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0301,     // 下按键长按消息
    SP1K_MSG_KEY_REPEATE_LEFT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0302,     // 左按键长按消息
    SP1K_MSG_KEY_REPEATE_RIGHT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0303,    // 右按键长按消息
    SP1K_MSG_KEY_REPEATE_TELE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0304,     // 变焦放大按键长按消息
    SP1K_MSG_KEY_REPEATE_WIDE = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0305,     // 变焦缩小按键长按消息
    SP1K_MSG_KEY_REPEATE_PB = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0306,       // 回放按键长按消息
    SP1K_MSG_KEY_REPEATE_POWER = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0307,    // 电源按键长按消息
    SP1K_MSG_KEY_REPEATE_MENU = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0308,     // 菜单按键长按消息
    SP1K_MSG_KEY_REPEATE_SNAP = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0309,     // 快照按键长按消息
    SP1K_MSG_KEY_REPEATE_SENSOR = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x030a,   // 传感器按键长按消息
    SP1K_MSG_KEY_REPEATE_AF = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x030b,       // 自动对焦按键长按消息
    SP1K_MSG_KEY_REPEATE_ZOOM_IN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x030c,  // 放大按键长按消息
    SP1K_MSG_KEY_REPEATE_ZOOM_OUT = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x030d, // 缩小按键长按消息
    SP1K_MSG_KEY_REPEATE_BURST = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x030e,    // 连拍按键长按消息
    SP1K_MSG_KEY_REPEATE_BACK = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x030f,     // 返回按键长按消息
    SP1K_MSG_KEY_REPEATE_MAX = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0310,      // 按键长按消息最大值

    SP1K_MSG_KEY_MAX = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0400, // 按键消息最大值

    SP1K_MSG_UNKNOWN_BTN = ((UINT16)MSG_CTRL_TYPE_BTN << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0fff, // 未知按键消息
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

