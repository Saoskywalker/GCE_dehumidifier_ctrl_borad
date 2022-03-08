#ifndef __General_func_H
#define __General_func_H

#include "DataType.h"

extern GCE_XDATA TS_SYS_Config G_Sys_Config;

extern GCE_XDATA UI32 G_Time_Run;          //��ʱʣ��ʱ��
extern GCE_XDATA UI08 G_Time_Set;          //��ʱ�趨ʱ��
extern GCE_XDATA UI08 G_Time_Buf;          //��ʱ�趨ʱ��buf
extern GCE_XDATA UI08 G_Time_Setting_Time; //��ʱ�趨(ȷ��ʱ��)
extern GCE_XDATA UI08 S_Time_Set_IN;       //�Ƿ�����˶�ʱ����

extern GCE_XDATA UI08 G_Power_Delay_Time; //ϵͳ������ʱ

extern GCE_XDATA TS_SYS_Err G_Sys_Err;                //����
extern GCE_XDATA TE_FuncState G_SYS_Self_Test;        //�Լ��־
extern GCE_XDATA TE_FuncState G_SYS_Fast_Test;        // ����־
extern GCE_XDATA TE_FuncState G_Comp_Overtime_Protect_Flag; //ѹ������������ͣ�����
extern GCE_XDATA ONOFF_STATUS G_SYS_Power_Status;     //���ػ�״̬
extern GCE_XDATA ONOFF_STATUS G_SYS_Power_Status_Old; // ���ػ�״̬

extern GCE_XDATA TE_ErrorStatus G_High_T_P3_Error_Status;
extern GCE_XDATA TE_ErrorStatus G_High_T_P4_Error_Status;
extern GCE_XDATA TE_ErrorStatus G_High_T_P5_Error_Status;
extern GCE_XDATA TE_ErrorStatus G_Turn_On_H_T_Error_Status;

extern GCE_XDATA TU_SYS_Mode G_SYS_Mode;     //ϵͳģʽ
extern GCE_XDATA TU_SYS_Mode G_SYS_Mode_Buf; //ϵͳģʽ
extern GCE_XDATA UI08 G_Set_SYS_Mode_Time;   // ģʽ�趨ʱ��

extern GCE_XDATA UI08 G_SYS_Hum_Set;     //ʪ���趨
extern GCE_XDATA UI08 G_SYS_Hum_Set_Buf; //ʪ���趨buf
extern GCE_XDATA UI08 G_SYS_Hum_WIFI_Set_Buf; //ʪ���趨buf
extern GCE_XDATA UI08 G_Set_SYS_Hum_Time;

extern GCE_XDATA TU_FAN_Speed_Type G_SYS_Fan_Tyde;     //���з���
extern GCE_XDATA TU_FAN_Speed_Type G_SYS_Fan_Tyde_Buf; //���з���
extern GCE_XDATA UI08 G_Set_SYS_Fan_Tyde_Time;         //���÷���ʱ��
extern GCE_XDATA TE_FuncState G_Set_Fan_Tyde_EN;       //�Ƿ��������

extern GCE_XDATA UI08 G_Buzz_Cnt;       //ˮ������ʱ�������������
extern GCE_XDATA UI08 Key_ERR_Buzz_Cnt; //������Чʱ����������Ĵ���
extern UI16 G_Buzz_Time;                //������ʱ��
extern TE_Pin_Status G_Buzzer_IO_Status;

extern GCE_XDATA TE_FuncState G_Pump_Status;

extern GCE_XDATA UI08 G_Disp_SA_Time;

extern void Turn_On(void);
extern void Turn_Off(void);
extern void prg_s_general(void);

extern void Sys_Initial(void); //	ϵͳ ��ʼ������

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
