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
  mode_DRY = 0,     //����ģʽ
  mode_SYS_HUM = 1, //��ʪģʽ
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
  TE_FuncState Auto_Restart; //�������ʹ��
  TE_FuncState Wifi; //Wifi����
} TS_SYS_Config;
//

typedef enum
{
  TYPE_A = 0,
  TYPE_B = 1
} TU_Def_TYPE; //

typedef struct
{
  TE_FuncState Water_Full;     //ˮ������
  TE_FuncState temp_room_err;  //���¹���
  TE_FuncState temp_coil_err;  //���¹���
  TE_FuncState temp_comp_err;  //���¹���
  TE_FuncState hum_Sensor_err; //ʪ�ȴ���������
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
  UI16 on_timer;     //ˮ�ô�ʱ��
  UI16 off_timer;    //�ر�ʱ��
  UI08 on_count;     //�򿪴���
  TU_Step step;      //״̬
  UI08 SW_ON_Timer;  //��ˮ�������ʱ��
  UI08 SW_OFF_Timer; //��ˮ���ر�ʱ��
  UI08 off_delay;
} TS_Pump_Reg;

typedef enum
{
  POWER_ON_STOP = 0, //�״���ѹ����ֹͣ״̬
  FIRST_ON = 1,      //�ϵ��״ο�ѹ����
  NO_FIRST_ON = 2    //���ϵ��״ο�ѹ����
} TU_Def_Comp;

typedef struct
{
  UI16 def_start_time;         //��˪��ʱ
  UI08 def_start_count_enable; //��˪��ʱʹ��
  UI16 def_time;               //��˪��ʱ
  UI08 def_cont;               //��˪����
  UI16 temp_room_point;        //��˪�����¶ȵ�
  TU_Def_TYPE def_type;        //��˪��ʽ(A/B)
  TU_Def_Comp def_Comp_mode;   //ͨ�翪����ѹ������һ����������
  TE_FuncState Defrost_status;
} TU_Def_Reg;

typedef struct
{
  UI08 C_value;
  UI08 value;         //�¶�ʱ�Ƕ�Fֵ         ʪ����Ϊʪ��ֵ
  UI08 value_buf;     //�¶�ʱ�Ƕ�Fֵ         ʪ����Ϊʪ��ֵ
  TS_AISTATUS status; //
  UI08 Err_Cut_Count;
  UI08 Err_Short_Count;
  UI08 Err_Normal_Count;
} TS_ADC_PARA;

#endif
