#ifndef __DATATYPE_H
#define __DATATYPE_H

#include "GCE_Data_Type_V2.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define BUZZ_long_time 300
#define BUZZ_short_time 150

#define ABSOLUTE_VALUE(a, b) ((a > b) ? (a - b) : (b - a))

typedef enum
{
  FALSE = 0,
  TRUE = !FALSE
} TE_Bool;

typedef enum
{
  DISABLE = 0,
  ENABLE = !DISABLE
} TE_FuncState;

typedef enum
{
  RESET = 0,
  SET = !RESET
} TE_Flag_Status,
    TE_IT_Status, TE_Pin_Status;

typedef enum
{
  ERROR = 0,
  SUCCESS = !ERROR
} TE_ErrorStatus;

typedef enum
{
  mode_DRY = 0,     //干衣模式
  mode_SYS_HUM = 1, //除湿模式
} TU_SYS_Mode;

//
typedef enum
{
  LOW_FAN = 0,
  MID_FAN = 1,
  HIGH_FAN = 2,
  OFF_FAN = 3
} TU_FAN_Speed_Type;

//
typedef struct
{
  TE_FuncState Auto_Restart; //掉电记忆使能
  TE_FuncState Wifi; //Wifi功能
} TS_SYS_Config;
//

typedef enum
{
  TYPE_A = 0,
  TYPE_B = 1
} TU_Def_TYPE; //

typedef struct
{
  TE_FuncState Water_Full;     //水满报警
  TE_FuncState temp_room_err;  //室温故障
  TE_FuncState temp_coil_err;  //管温故障
  TE_FuncState temp_comp_err;  //管温故障
  TE_FuncState hum_Sensor_err; //湿度传感器故障
} TS_SYS_Err;

typedef enum
{
  Step0 = 0,
  Step1 = 1,
  Step2 = 2,
  Step3 = 3,
  Step4 = 4,
  Step5 = 5,
  Step6 = 6,
  Step7 = 7,
  Step8 = 8,
  Step9 = 9,
  Step10 = 10,
} TU_Step;

typedef struct
{
  UI16 on_timer;     //水泵打开时间
  UI16 off_timer;    //关闭时间
  UI08 on_count;     //打开次数
  TU_Step step;      //状态
  UI08 SW_ON_Timer;  //排水马达运行时间
  UI08 SW_OFF_Timer; //排水马达关闭时间
  UI08 off_delay;
} TS_Pump_Reg;

typedef enum
{
  POWER_ON_STOP = 0, //首次上压缩机停止状态
  FIRST_ON = 1,      //上电首次开压缩机
  NO_FIRST_ON = 2    //非上电首次开压缩机
} TU_Def_Comp;

typedef struct
{
  UI16 def_start_time;         //除霜计时
  UI08 def_start_count_enable; //除霜计时使能
  UI16 def_time;               //除霜计时
  UI08 def_cont;               //除霜次数
  UI16 temp_room_point;        //除霜室温温度点
  TU_Def_TYPE def_type;        //除霜方式(A/B)
  TU_Def_Comp def_Comp_mode;   //通电开机后，压缩机第一次启动变量
  TE_FuncState Defrost_status;
} TU_Def_Reg;

typedef struct
{
  UI08 C_value;
  UI08 value;         //温度时是度F值         湿度是为湿度值
  UI08 value_buf;     //温度时是度F值         湿度是为湿度值
  TS_AISTATUS status; //
  UI08 Err_Cut_Count;
  UI08 Err_Short_Count;
  UI08 Err_Normal_Count;
} TS_ADC_PARA;

#endif
