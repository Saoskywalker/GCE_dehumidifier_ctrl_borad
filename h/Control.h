#ifndef __Control_H
#define __Control_H

//=================��Ҫ���ñ���==================================================
#define COMP_PROTECT_ON_TIME 180  //ѹ����������ʱ��
#define COMP_PROTECT_OFF_TIME 180 //ѹ�����ر���ʱ��
#define FILTER_CLEAN_TIME (60UL * 250) //������ϴʱ��, 250h

//===========��˪����
#define defrost_para1 (10 * 60)  //ѹ������תX���Ӻ�ʼ��ʱ
#define defrost_para2 134        // 0�ȶ�ӦADֵ  (10/(10+28.38))*1024-1=266
#define defrost_para3 157        // 5�ȶ�ӦADֵ  (10/(10+22.72))*1024-1=312
#define defrost_para4 (10 * 60)  //��ʱ������X�����жϹ���
#define defrost_para5 (19 * 60)  //��ʱ������Y�����жϹ���
#define defrost_para6 (20 * 60)  //��˪��ʼʱ���ʱ
#define defrost_para7 (10 + 15)  //��˪ʱ�����¶��ж�  10��  0�ȶ�Ӧ15
#define defrost_para8 (7 * 60)   //��˪ʱ��1
#define defrost_para9 (7 * 60)   //��˪ʱ��2
#define defrost_para10 (4)       //��X�γ�˪����˪ʱ���ӳ�
#define defrost_para11 (15 * 60) //��X�γ�˪����˪ʱ���ӳ���Y����
#define defrost_para12 (15 + 9)  //ͭ�ܹ���ʱ���������ж� <15

//==================����ӿ�===============================================================

extern GCE_XDATA TU_Def_Reg G_Def_Reg;

extern GCE_XDATA TS_Pump_Reg Pump_type;

extern GCE_XDATA UI16 G_Fan_Force_Run_Time;

extern GCE_XDATA TE_FuncState Fan_set_enable;

extern GCE_XDATA UI16 G_Filter_Run_Time;     //����ʱ��
extern GCE_XDATA UI16 G_Filter_Run_Time_Buf; //����ʱ��
extern GCE_XDATA TE_FuncState  G_Filter_Status;

///
extern GCE_XDATA TE_FuncState G_Comp_Test_EN;
extern GCE_XDATA TS_DO_Para_Def G_Comp_Para; //ѹ����
extern GCE_XDATA TS_DO_Para_Def G_Pump_Para; //ˮ��

extern GCE_XDATA TU_FAN_Speed_Type G_Fan_Tyde_Out; //ʵ�����з���
extern GCE_XDATA TU_FAN_Speed_Type G_Fan_Tyde_Out_Buf; //ʵ�����з���
///
extern GCE_XDATA TE_FuncState Comp_SA_EN; // SA������־

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
