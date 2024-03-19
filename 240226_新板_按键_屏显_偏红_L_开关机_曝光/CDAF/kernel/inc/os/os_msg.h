/**************************************************************************
 *         Copyright(c) 2007 by Sunplus mMedia  Technology Co., Ltd.       *
 *                                                                         *
 *  This software is copyrighted by and is the property of Sunplus mMedia  *
 *  Technology Co., Ltd. All rights are reserved by Sunplus mMedia Techno- *
 *  logy Co., Ltd. This software may only be used in accordance with the   *
 *  corresponding license agreement. Any unauthorized use, duplication,    *
 *  distribution, or disclosure of this software is expressly forbidden.   *
 *                                                                         *
 *  This Copyright notice "M U S T" not be removed or modified without     *
 *  prior written consent of Sunplus mMedia Technology  Co., Ltd.          *
 *                                                                         *
 *  Sunplus mMedia Technology Co., Ltd. reserves the right to modify this  *
 *  software without notice.                                               *
 *                                                                         *
 *  Sunplus mMedia Technology Co., Ltd.                                    *
 *  19-1, Innovation First Road, Science-Based Industrial Park,            *
 *  Hsin-Chu, Taiwan, R.O.C.                                               *
 *                                                                         *
 **************************************************************************/
#ifndef __OS_MSG_H__
#define __OS_MSG_H__

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/
#define MSG_CTRL_IDX_GROUP_SHIFT	(8 + 4)
#define MSG_CTRL_IDX_GROUP_MASK		((UINT16)0x0F << MSG_CTRL_IDX_GROUP_SHIFT) //0xF000
/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
#define DZOOM_OSD_MSG_TO_HOST	1
/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
typedef UINT16 msg_t;
 
typedef enum msgCtrlType_e 
{
	MSG_CTRL_TYPE_HOST = 0x00,	// 用于主机流消息
	MSG_CTRL_TYPE_BTN  = 0x01,  // 用于按钮驱动器消息
	MSG_CTRL_TYPE_SYS  = 0x02,	// 用于系统内部消息
	MSG_CTRL_TYPE_MAX  = 0x03,  // 枚举类型的最大值
} msgCtrlType_t;


typedef enum msgCtrlFlush_e 
{
	MSG_CTRL_FLUSH_HOST = 0x01 << 0,  // 刷新主机流消息
	MSG_CTRL_FLUSH_BTN	= 0x01 << 1,  // 刷新按钮驱动器消息
	MSG_CTRL_FLUSH_SYS  = 0x01 << 2,  // 刷新系统内部消息
	MSG_CTRL_FLUSH_ALL  = MSG_CTRL_FLUSH_HOST | MSG_CTRL_FLUSH_BTN | MSG_CTRL_FLUSH_SYS,  // 刷新所有消息类型
} msgCtrlFlush_t;

typedef struct MainMsgQue_s
{
	struct MainMsgQue_s* pQue;  // 指向消息队列的指针
	msg_t* QueStart;  // 消息队列的起始位置指针
	msg_t* QueEnd;  // 消息队列的结束位置指针
	msg_t* QueIn;  // 消息队列的输入指针
	msg_t* QueOut;  // 消息队列的输出指针
	UINT16 QueEntries;  // 消息队列中当前的消息数
	UINT16 QueSize;  // 消息队列的容量大小
	msg_t* MsgBuf;  // 指向消息缓冲区的指针
	UINT8 bLock;  // 消息队列的锁定状态标志位
} MainMsgQue_t;

typedef enum
{
    SP1K_MSG_NULL = 0x0000, // 空消息

    /* 媒体消息 */

    SP1K_MSG_MEDIA_REC_OK = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0800,          // 录制成功消息
    SP1K_MSG_MEDIA_REC_FAIL = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0801,        // 录制失败消息
    SP1K_MSG_MEDIA_REC_DISK_FULL = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0802,   // 磁盘已满消息
    SP1K_MSG_MEDIA_PLAY_OK = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0803,         // 播放成功消息
    SP1K_MSG_MEDIA_PLAY_FAIL = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0804,       // 播放失败消息
    SP1K_MSG_MEDIA_PLAY_END = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0805,        // 播放结束消息
    SP1K_MSG_MEDIA_FF_TO_END = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0806,       // 快进到结尾消息
    SP1K_MSG_MEDIA_RW_TO_END = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0807,       // 倒带到结尾消息
    SP1K_MSG_MEDIA_SOUND_FINISH = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0808,    // 声音播放完成消息
    SP1K_MSG_MEDIA_PLAY_START = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0809,      // 播放开始消息
    SP1K_MSG_MEDIA_REC_START = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x080a,       // 录制开始消息
    SP1K_MSG_MEDIA_REC_STOP = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x080b,        // 录制停止消息
    SP1K_MSG_MEDIA_VOICE_MEMO_END = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x080c,  // 语音备忘录结束消息
    SP1K_MSG_MEDIA_REC_FILE_MAX = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x080d,    // 录制文件达到最大消息
    SP1K_MSG_MEDIA_VIDO_CAP_ERROR = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x080e,  // 视频拍摄错误消息
    SP1K_MSG_MEDIA_VIDO_CAP_FINISH = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x080f, // 视频拍摄完成消息

    /* 杂项控制 */
    // 自拍按键按下消息
    SP1K_MSG_KEY_PRESS_SELFSNAP = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0900, // 自拍按键按下消息
    // 自拍蜂鸣器消息
    SP1K_MSG_SELFSNAP_BEEP = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0901,   // 自拍蜂鸣器消息
    SP1K_MSG_CMD_PANEL_OFF = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0902,   // 命令面板关闭消息
    SP1K_MSG_CMD_PANEL_ON = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0903,    // 命令面板打开消息
    SP1K_MSG_3A_WAIT_PROC = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0904,    // 等待3A处理消息
    SP1K_MSG_SELFSNAP_COUNT = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0905,  // 自拍计数消息
    SP1K_MSG_TIMER_COUNT = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0906,     // 计时器计数消息
    SP1K_MSG_GUI_PEN_SLIDE = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0907,   // GUI触摸笔滑动消息
    SP1K_MSG_GUI_PEN_UP = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0908,      // GUI触摸笔抬起消息
    SP1K_MSG_GUI_PEN_REPEATE = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0909, // GUI触摸笔重复消息
    SP1K_MSG_DO_RESET = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x090a,        // 执行重置消息

    /* 幻灯片消息 */

    SP1K_MSG_SLIDE_SHOW_START = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0b00,  // 幻灯片播放开始消息
    SP1K_MSG_SLIDE_SHOW_STOP = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0b01,   // 幻灯片播放停止消息
    SP1K_MSG_SLIDE_SHOW_NEXT = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0b02,   // 幻灯片播放下一页消息
    SP1K_MSG_SLIDE_SHOW_FINISH = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0b03, // 幻灯片播放结束消息

    /* DPS */

    SP1K_MSG_DPS_NEW_HDISCVRY = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0c01,       // 新设备发现消息
    SP1K_MSG_DPS_NEW_CONFIG_PRINTER = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0c02, // 打印机发送新配置消息
    SP1K_MSG_DPS_NEW_DEVICE_STATUS = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0c03,  // 打印机发送新设备状态消息
    SP1K_MSG_DPS_NEW_JOB_STATUS = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0c04,     // 打印机发送新作业状态消息
    SP1K_MSG_DPS_START_JOB = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0c05,          // 打印作业开始消息
    SP1K_MSG_DPS_ERROR = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0c06,              // 打印错误消息
    SP1K_MSG_DPS_ERROR_SIDC = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0c07,         // 打印SIDC错误消息
    sp1k_MSG_DPS_NOT_FIND_PRINTER = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0c08,   // 未找到打印机消息

    /* 人脸检测 */
    SP1K_MSG_FD_SMILE = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0d01,                    // 微笑检测消息//0x2d01
    SP1K_MSG_EYE_BLINK = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0d02,                   // 眨眼检测消息//0x2d02
    SP1K_MSG_MD_ACTION = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0d03,                   // 运动检测消息//0x2d03
    sp1k_MSG_MD_UPDATE_TIME = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0d04,              // 更新时间消息//0x2d04
    sp1k_MSG_REC_SEAMLESS_DISK_FULL = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0d05,      // 连续录制磁盘已满消息//0x2d04
    sp1k_MSG_REC_SEAMLESS_CLOSEFILE = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0d06,      // 连续录制关闭文件消息//0x2d04
    sp1k_MSG_REC_SEAMLESS_NOT_CHANGENAME = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0d07, // 连续录制未改变文件名消息//0x2d04

/* 数码变焦 */
#if (DZOOM_OSD_MSG_TO_HOST == 0)                                                                  // linjiecheng move to host msg queue
    SP1K_MSG_DZOOM_OSD_UPDATE = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0e01, // OSD更新消息//0x2e01
#else
    SP1K_MSG_DZOOM_OSD_UPDATE = ((UINT16)MSG_CTRL_TYPE_HOST << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0e01, // OSD更新消息//0x0e01
#endif
    SP1K_MSG_DZOOM_3A_UPDATE = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0e02, // 3A更新消息//0x2e02

    /* 全景 */
    SP1K_MSG_GMV_BY_FRAME_CALCULUS = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0f01, // 帧计算全景消息//0x2f01
    SP1K_MSG_SUM_OF_GMV_OSD_DRAW = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0f02,   // GMV和的OSD绘制消息//0x2f02
    SP1K_MSG_GMV_TOO_FAST_OSD_DRAW = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0f03, // GMV过快OSD绘制消息//0x2f03
    SP1K_MSG_GMV_TOO_SLOW_OSD_DRAW = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0f04, // GMV过慢OSD绘制消息//0x2f04
    SP1K_MSG_PANORAMA_FINISH = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0f05,       // 全景拍摄完成消息//0x2f05

    SP1K_MSG_UNKNOWN_SYS = ((UINT16)MSG_CTRL_TYPE_SYS << MSG_CTRL_IDX_GROUP_SHIFT) | 0x0fff, // 未知系统消息//0x2fff
} osMsgList_t;


/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/
void osMsgRcv(void);
void osMsgInit (void);
UINT8 osMsgQuery (void);
UINT8 osMsgAccept (UINT16* msg);
UINT8 osMsgPost (UINT16 msg);
UINT8 osMsgPostFront (UINT16 msg);	
UINT8 osMsgPostForce (UINT16 msg);
UINT8 osMsgWait(UINT16 msg);
void osMsgFlush (UINT8 msgGrp);
UINT8 osMsgSeek(UINT16 msg);
UINT8 osMsgIntrSeek(UINT16 msg);
void osMsgLock (UINT8 bLock, msgCtrlFlush_t msgGrp);
#endif /*__OS_MSG_H__*/

