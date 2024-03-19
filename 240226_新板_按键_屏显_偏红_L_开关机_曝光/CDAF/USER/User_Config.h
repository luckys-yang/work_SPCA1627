#ifndef __USER_CONFIG_H
#define __USER_CONFIG_H

/*-------------------- USER CODE: Include Begin --------------------*/

/*-------------------- USER CODE: Include End --------------------*/



/*-------------------- USER CODE: Custom Begin --------------------*/

/*-------------------- USER CODE: Custom End --------------------*/


/*===============================  配置宏定义 ============================*/

#define AF_AUTO_FOCUS_TIMER 30 // 自动对焦计数时间

/*===============================  控制宏定义 ============================*/

/*对焦电源控制*/
#define AF_CTRL_ENABLE 1    // 导通
#define AF_CTRL_DISABL 0    // 不导通

/*蓝色忙指示灯控制*/
#define LED_BUSY_ON 1   // 亮
#define LED_BUSY_OFF 0  // 灭

/*SPI Flash写保护控制*/
#define FLASH_SPI_LOCK 0    // SPI写保护使能
#define FLASH_SPI_UNLOCK 1  // SPI写保护失能

/*闪光灯控制*/
#define SPARK_LED_ON 1  // 亮
#define SPARK_LED_OFF 0 // 灭

/* 按键使能/失能控制*/
#define KEY_SWITCH_ENABLE 1 // 使能
#define KEY_SWITCH_DISABLE 0    // 失能

/* 屏幕背光控制*/
#define LCD_LED_ON 1 // 使能
#define LCD_LED_OFF 0    // 失能

/* 看门狗心跳控制 */
#define WTD_DOG_SET 1
#define WTD_DOG_RESET 0

/*拍照对焦显示框控制相关*/
#define AF_BOX_DISPLAY 1      // 显示
#define AF_BOX_HIDE 0         // 隐藏
#define AF_BOX_COLOR_GREEEN 1  // 绿色框框
#define AF_BOX_COLOR_YELLOW 0 // 黄色框框

/*===============================  PV模式下控制宏定义 ============================*/

/* 相框控制 */
#define PV_PhotoFrame_Enable 1
#define PV_PhotoFrame_Disable 0


// 系统状态枚举类型
typedef enum
{
    Flag_false = 0,
    Flag_true = 1
} SystemStatus_et;

// Ui状态枚举类型
typedef enum
{
    Status_0 = 0,
    Status_1 = 1,
    Status_2 = 2,
    Status_3 = 3,
    Status_4 = 4,
    Status_5 = 5,
    Status_6 = 6
} UiStatus_et;

// GPIO引脚编号枚举类型
typedef enum
{
    GPIO_PIN_1 = 0x01,  // 引脚1(0x01) 0000 0001
    GPIO_PIN_2 = 0x02,  // 引脚2(0x02) 0000 0010
    GPIO_PIN_3 = 0x04,  // 引脚3(0x04) 0000 0100
    GPIO_PIN_4 = 0x08,  // 引脚4(0x08) 0000 1000
    GPIO_PIN_5 = 0x10,  // 引脚5(0x10) 0001 0000
    GPIO_PIN_6 = 0x20,  // 引脚6(0x20) 0010 0000
    GPIO_PIN_7 = 0x40,  // 引脚7(0x40) 0100 0000
    GPIO_PIN_8 = 0x80   // 引脚8(0x80) 1000 0000
} GPIO_Pin_TypeDef_et;

// GPIO电平状态枚举类型
typedef enum
{
    GPIO_PIN_RESET = 0,  // 引脚低电平
    GPIO_PIN_1_SET   = GPIO_PIN_1,   // 引脚1(0x01)高电平
    GPIO_PIN_2_SET   = GPIO_PIN_2,   // 引脚2(0x02)高电平
    GPIO_PIN_3_SET   = GPIO_PIN_3,   // 引脚3(0x04)高电平
    GPIO_PIN_4_SET   = GPIO_PIN_4,   // 引脚4(0x08)高电平
    GPIO_PIN_5_SET   = GPIO_PIN_5,   // 引脚5(0x10)高电平
    GPIO_PIN_6_SET   = GPIO_PIN_6,   // 引脚6(0x20)高电平
    GPIO_PIN_7_SET   = GPIO_PIN_7,   // 引脚7(0x40)高电平
    GPIO_PIN_8_SET   = GPIO_PIN_8,   // 引脚8(0x80)高电平
} GPIO_PinState_et;

#endif
