#ifndef __General_func_H
#define __General_func_H

#include "DataType.h"

extern GCE_XDATA TS_SYS_Config G_Sys_Config;

extern GCE_XDATA UI32 G_Time_Run;          //定时剩余时间
extern GCE_XDATA UI08 G_Time_Set;          //定时设定时间
extern GCE_XDATA UI08 G_Time_Buf;          //定时设定时间buf
extern GCE_XDATA UI08 G_Time_Setting_Time; //定时设定(确认时间)
extern GCE_XDATA UI08 S_Time_Set_IN;       //是否进行了定时设置

extern GCE_XDATA UI08 G_Power_Delay_Time; //系统开机延时

extern GCE_XDATA TS_SYS_Err G_Sys_Err;                //故障
extern GCE_XDATA TE_FuncState G_SYS_Self_Test;        //自检标志
extern GCE_XDATA TE_FuncState G_SYS_Fast_Test;        // 快测标志
extern GCE_XDATA TE_FuncState G_Comp_Overtime_Protect_Flag; //压缩机连续运行停机标记
extern GCE_XDATA ONOFF_STATUS G_SYS_Power_Status;     //开关机状态
extern GCE_XDATA ONOFF_STATUS G_SYS_Power_Status_Old; // 开关机状态

extern GCE_XDATA TE_ErrorStatus G_High_T_P3_Error_Status;
extern GCE_XDATA TE_ErrorStatus G_High_T_P4_Error_Status;
extern GCE_XDATA TE_ErrorStatus G_High_T_P5_Error_Status;
extern GCE_XDATA TE_ErrorStatus G_Turn_On_H_T_Error_Status;

extern GCE_XDATA TU_SYS_Mode G_SYS_Mode;     //系统模式
extern GCE_XDATA TU_SYS_Mode G_SYS_Mode_Buf; //系统模式
extern GCE_XDATA UI08 G_Set_SYS_Mode_Time;   // 模式设定时间

extern GCE_XDATA UI08 G_SYS_Hum_Set;     //湿度设定
extern GCE_XDATA UI08 G_SYS_Hum_Set_Buf; //湿度设定buf
extern GCE_XDATA UI08 G_SYS_Hum_WIFI_Set_Buf; //湿度设定buf
extern GCE_XDATA UI08 G_Set_SYS_Hum_Time;

extern GCE_XDATA TU_FAN_Speed_Type G_SYS_Fan_Tyde;     //运行风速
extern GCE_XDATA TU_FAN_Speed_Type G_SYS_Fan_Tyde_Buf; //运行风速
extern GCE_XDATA UI08 G_Set_SYS_Fan_Tyde_Time;         //设置风速时间
extern GCE_XDATA TE_FuncState G_Set_Fan_Tyde_EN;       //是否允许风速

extern GCE_XDATA UI08 G_Buzz_Cnt;       //水满报警时，蜂鸣器响次数
extern GCE_XDATA UI08 Key_ERR_Buzz_Cnt; //按键无效时，蜂鸣器响的次数
extern UI16 G_Buzz_Time;                //蜂鸣器时间
extern TE_Pin_Status G_Buzzer_IO_Status;

extern GCE_XDATA TE_FuncState G_Pump_Status;

extern GCE_XDATA UI08 G_Disp_SA_Time;

extern void Turn_On(void);
extern void Turn_Off(void);
extern void prg_s_general(void);

extern void Sys_Initial(void); //	系统 初始化程序

extern UI16 UI16_Addition_Operation(UI16 _augend, UI16 _addend);
extern UI16 UI16_Subtraction_Operation(UI16 _Subtraction, UI16 _minuend);

extern void SYS_Data_Rest(void);
extern void General_Deal(void);
extern void SYS_Data_Init(void);

extern TE_FuncState Get_Invalid_Key(UI08 key_num);
extern void Set_Power_Status(void);
extern void Rest_Key_Buzzer(void);
extern void Set_FAN_Tyde(void);
extern void Set_In_Time(void);
extern void Set_Timer_Up(void);
extern void Set_Timer_Down(void);

extern void Set_Hum_Up(void);
extern void Filter_Clean(void);
extern void Set_Hum_Down(void);
extern void Set_SYS_Mode(void);
extern void Set_SYS_Mode(void);
extern void Set_Pump_Status(void);
extern void Up_Key_Function(void);
extern void Down_Key_Function(void);
extern void Set_In_Fast_Test(void);
extern void Set_In_Comp_Test(void);
extern void Set_RESET_WIFI(void);
extern void Set_TEST_WIFI(void);
extern void Set_Comp_Overtime_Protect(void);
extern void Set_In_Self_Test(void);

#endif
