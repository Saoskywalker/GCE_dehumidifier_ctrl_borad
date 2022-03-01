#ifndef __Control_H
#define __Control_H

//=================需要配置变量==================================================
#define COMP_PROTECT_ON_TIME 180  //压缩机开保护时间
#define COMP_PROTECT_OFF_TIME 180 //压缩机关保护时间
#define FILTER_CLEAN_TIME (60UL * 250) //滤网清洗时间, 250h

//===========除霜参数
#define defrost_para1 (10 * 60)  //压缩机运转X分钟后开始计时
#define defrost_para2 134        // 0度对应AD值  (10/(10+28.38))*1024-1=266
#define defrost_para3 157        // 5度对应AD值  (10/(10+22.72))*1024-1=312
#define defrost_para4 (10 * 60)  //计时过程中X分钟判断管温
#define defrost_para5 (19 * 60)  //计时过程中Y分钟判断管温
#define defrost_para6 (20 * 60)  //除霜开始时间计时
#define defrost_para7 (10 + 15)  //除霜时环境温度判断  10°  0度对应15
#define defrost_para8 (7 * 60)   //除霜时间1
#define defrost_para9 (7 * 60)   //除霜时间2
#define defrost_para10 (4)       //第X次除霜，除霜时间延长
#define defrost_para11 (15 * 60) //第X次除霜，除霜时间延长至Y分钟
#define defrost_para12 (15 + 9)  //铜管故障时，以室温判断 <15

//==================对外接口===============================================================

extern GCE_XDATA TU_Def_Reg G_Def_Reg;

extern GCE_XDATA TS_Pump_Reg Pump_type;

extern GCE_XDATA UI16 G_Fan_Force_Run_Time;

extern GCE_XDATA TE_FuncState Fan_set_enable;

extern GCE_XDATA UI16 G_Filter_Run_Time;     //滤网时间
extern GCE_XDATA UI16 G_Filter_Run_Time_Buf; //滤网时间
extern GCE_XDATA TE_FuncState  G_Filter_Status;

///
extern GCE_XDATA TE_FuncState G_Comp_Test_EN;
extern GCE_XDATA TS_DO_Para_Def G_Comp_Para; //压缩机
extern GCE_XDATA TS_DO_Para_Def G_Pump_Para; //水泵

extern GCE_XDATA TU_FAN_Speed_Type G_Fan_Tyde_Out; //实际运行风速
extern GCE_XDATA TU_FAN_Speed_Type G_Fan_Tyde_Out_Buf; //实际运行风速
///
extern GCE_XDATA TE_FuncState Comp_SA_EN; // SA保护标志

// extern void Pump_S_general(void);
// extern void prg_s_control(void);
// extern void prg_minute_control(void);
// extern void prg_hour_conl(void);
extern void Sys_Control(void);
// extern void prg_200ms_control(void);
// extern void load_set_self_test(void);
extern void Control_data_init(void);
extern void BUZZ_CONTROL(void);
extern void Run_reg_init(void);

extern void SYS_Mode_Change_Handle(TU_SYS_Mode _now_mode, TU_SYS_Mode _set_mode);

#endif

//////////////////////////////////////////////////////////
//
//      eeeeeeeeee       n           nn      ddddddddd
//     ee               nnn         nn      dd       dd
//    ee               nn   n      nn      dd        dd
//   eeeeeeeeee       nn     n    nn      dd         dd
//  ee               nn       n  nn      dd         dd
// ee               nn         nnn      dd         dd
// eeeeeeeeee       nn          nn      ddddddddddd
//
// Designed by Benkye.Zhang 2010.10.28
///////////////////////////////////////////////////////
