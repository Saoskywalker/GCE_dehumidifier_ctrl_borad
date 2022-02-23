#ifndef __KEY_DEVICE_H_
#define __KEY_DEVICE_H_

#include "DataType.h"

#define POWER_KEY 1
#define FAN_KEY 3
#define SET_TIME_KEY 6

#define UP_KEY 4
#define DOWN_KEY 5

#define MODE_KEY 2
#define FILTER_CLEAN_KEY 7
// #define PUMP_KEY 7

#define SELF_TEST_KEY 0XF0
#define COMP_TEST_KEY 0Xf8

#define WIFI_RESET_KEY 0Xf9
#define WIFI_TEST_KEY 0XfA

//组合长按键
#define FAST_TEST_KEY 21
#define COMP_OVERTIME_PROTECT_KEY 23

#define WIFI_SELF_TEST_KEY_TIMER 5 //长按按键时间
#define WIFI_SELF_TEST_IN_TIMER 5  //允许进入等待时间

#define non_key 0xff

#define TK_0 0x00000001
#define TK_1 0x00000002
#define TK_2 0x00000004
#define TK_3 0x00000008

#define TK_4 0x00000010
#define TK_5 0x00000020
#define TK_6 0x00000040
#define TK_7 0x00000080

#define TK_8 0x00000100
#define TK_9 0x00000200
#define TK_10 0x00000400
#define TK_11 0x00000800

#define TK_12 0x00001000
#define TK_13 0x00002000
#define TK_14 0x00004000
#define TK_15 0x00008000

#define TK_16 0x00010000
#define TK_17 0x00020000
#define TK_18 0x00040000
#define TK_19 0x00080000

#define TK_20 0x00100000
#define TK_21 0x00200000
#define TK_22 0x00400000
#define TK_23 0x00800000

#define TK_24 0x01000000
#define TK_25 0x02000000
#define TK_26 0x04000000
#define TK_27 0x08000000

#define TK_28 0x10000000
#define TK_29 0x20000000
#define TK_30 0x40000000
#define TK_31 0x80000000

extern GCE_XDATA UI16 key_data;                  //当前按键按下值  未滤波
extern GCE_XDATA UI08 G_Key_Number;              //滤波后识别出的按键
extern GCE_XDATA UI16 WIFI_Self_Test_delay_time; //待機狀態下前5秒內,長按風速鍵5秒發送一次Wifi模产检模式指令

extern void Dev_Get_Key_Number(void);

#define Get_Key_No_Fliter_Data(void) key_data
#define Get_Key_Data(void) G_Key_Number
#define Clear_Key_Data G_Key_Number = 0

#endif /* __key_function_H*/
