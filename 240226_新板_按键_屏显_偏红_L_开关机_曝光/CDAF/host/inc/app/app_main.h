#ifndef __APP_MAIN_H__
#define __APP_MAIN_H__

#define APP_STATE_BUFF_SIZE	2   // 最大可以存储之前上一个状态的值
// 默认上电显示的界面
#define STATE_DEFAULT	SP1K_MSG_STATE_TO_STILL_VIEW

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
typedef void (code* pfpSTATE)(UINT16);

typedef struct appState_s
{
    pfpSTATE StateFunc; // 指向状态函数的指针
    UINT8 FuncBank;     // 功能银行
} appState_t;

typedef struct CamState_s
{
    UINT8 PrevState[APP_STATE_BUFF_SIZE]; // 用于存储之前状态的缓冲区
    UINT8 ActiveState;                    // 活动状态
} CamState_t;

typedef enum
{
    APP_STATE_NULL = 0, // 空状态，无功能

    APP_STATE_POWER_ON,       // 开机状态
    APP_STATE_STILL_VIEW,     // 拍照视图
    APP_STATE_VIDEO_VIEW,     // 录像视图
    APP_STATE_AUDIO_REC,      // 音频录制状态
    APP_STATE_STILL_PLAY,     // 静态图像播放状态
    APP_STATE_MUTI_PLAY,      // 多媒体播放状态
    APP_STATE_VIDEO_PLAY,     // 视频播放状态
    APP_STATE_AUDIO_PLAY,     // 音频播放状态
    APP_STATE_MENU,           // 菜单状态
    APP_STATE_USB,            // USB连接状态
    APP_STATE_SLIDE_SHOW,     // 幻灯片播放状态
    APP_STATE_PAN_SHOW,       // 平移展示状态
    APP_STATE_IDLE,           // 空闲状态
    APP_STATE_POWER_OFF,      // 关机状态
    APP_STATE_DPS,            // 直接打印服务状态
    APP_STATE_UNKNOWN = 0xff, // 未知状态
} appStateList_t;

typedef enum appStateId_e
{
    STATE_FUNC_ID_POWER_ON,   // 开机状态函数ID
    STATE_FUNC_ID_STILL_VIEW, // 静态图像浏览状态函数ID
    STATE_FUNC_ID_VIDEO_VIEW, // 视频浏览状态函数ID
    STATE_FUNC_ID_AUDIO_REC,  // 音频录制状态函数ID
    STATE_FUNC_ID_STILL_PLAY, // 静态图像播放状态函数ID
    STATE_FUNC_ID_VIDEO_PLAY, // 视频播放状态函数ID
    STATE_FUNC_ID_AUDIO_PLAY, // 音频播放状态函数ID
    STATE_FUNC_ID_MUTI_PLAY,  // 多媒体播放状态函数ID
    STATE_FUNC_ID_USB,        // USB连接状态函数ID
    STATE_FUNC_ID_MENU,       // 菜单状态函数ID
    STATE_FUNC_ID_SLIDE_SHOW, // 幻灯片播放状态函数ID
    STATE_FUNC_ID_PAN_SHOW,   // 平移展示状态函数ID
    STATE_FUNC_ID_IDLE,       // 空闲状态函数ID
    STATE_FUNC_ID_POWER_OFF,  // 关机状态函数ID
    STATE_FUNC_ID_DPS,        // 直接打印服务状态函数ID
    STATE_FUNC_ID_TOTAL,      // 总状态函数ID
} appStateId_t;

void appMain (void);
UINT8 appPreStateGet(UINT8 PreLev);
UINT8 appCurrStateGet(void);

#endif  /* __APP_MAIN_H__ */
