#include "General.h"

//����ýй©����
GCE_XDATA TE_Bool S_EC1_Err = FALSE;   // EC����
GCE_XDATA UI08 S_EC1_Cycle_Count = 0;  // EC��ʱ����
GCE_XDATA UI08 S_EC1_Step = 0;         // EC��ʱ����
GCE_XDATA UI16 S_EC1_Count_Time = 0;   // EC��ʱ����
GCE_XDATA UI16 S_EC1_COMP_ON_Time = 0; //
//
GCE_XDATA TE_Bool S_EC2_Err = FALSE;   // EC����
GCE_XDATA UI08 S_EC2_Step = 0;         // EC��ʱ����
GCE_XDATA UI16 S_EC2_Count_Time = 0;   // EC��ʱ����
GCE_XDATA UI16 S_EC2_Comp_ON_Time = 0; //
//
GCE_XDATA TS_Pump_Reg S_Pump_Type; // PUMP����

GCE_XDATA TU_Def_Reg G_Def_Reg; //��˪����

GCE_XDATA UI16 G_Fan_Force_Run_Time = 180; //ÿ�ΰ������߶�ʱ����������ˮ���ظ����ǿ������ʱ��

GCE_XDATA UI08 S_Filter_Minute_Count = 0; //������ʱ
GCE_XDATA UI16 G_Filter_Run_Time = 0;     //����ʱ��, ��λ:����
GCE_XDATA UI16 G_Filter_Run_Time_Buf = 0; //����ʱ��, ��λ:����
GCE_XDATA TE_FuncState  G_Filter_Status = DISABLE;

GCE_XDATA TU_Step S_High_T_P2_Step = Step0;

GCE_XDATA UI16 S_High_T_P3_Cont = 0;       //���±����߼�����ʱ����
GCE_XDATA UI08 S_High_T_P3_Cycle_Cont = 0; //���±����߼�����ʱ����
GCE_XDATA TE_ErrorStatus G_High_T_P3_Error_Status = SUCCESS;

GCE_XDATA TE_ErrorStatus G_Turn_On_H_T_Error_Status = SUCCESS; //����ʱ�����¡�40�� �쳣��־
GCE_XDATA UI16 S_High_T_P4_Cont1 = 0;                        //���±����߼�4��ʱ����
GCE_XDATA UI16 S_High_T_P4_Cont2 = 0;                        //���±����߼�4��ʱ����
GCE_XDATA TE_ErrorStatus G_High_T_P4_Error_Status = SUCCESS;

GCE_XDATA UI16 S_High_T_P5_Cont1 = 0;      //���±����߼�5��ʱ����
GCE_XDATA UI08 S_High_T_P5_Cycle_Cont = 0; //���±����߼�5��ʱ����
GCE_XDATA UI08 S_High_T_P5_LT = 0;         //���±����߼�5��ʱ����
GCE_XDATA UI08 S_High_T_P5_HT = 0;         //���±����߼�5��ʱ����

GCE_XDATA TE_ErrorStatus G_High_T_P5_Error_Status = SUCCESS;

//ѹ����ǿ�Ʋ��Թ��ܱ���
GCE_XDATA TE_FuncState G_Comp_Test_EN = DISABLE;
GCE_XDATA UI16 S_Comp_Test_Time = 0; //ѹ��������ʱ��

#define COMP_TEST_OFF_TIME 60 * 3 // 3���ӹ�
#define COMP_TEST_ON_TIME 60 * 6  // 6���ӿ�
//

GCE_XDATA TS_DO_Para_Def G_Comp_Para; //ѹ����
GCE_XDATA TS_DO_Para_Def G_Pump_Para; //ˮ��

GCE_XDATA TU_FAN_Speed_Type G_Fan_Tyde_Out = OFF_FAN;     //ʵ�����з���
GCE_XDATA TU_FAN_Speed_Type G_Fan_Tyde_Out_Buf = OFF_FAN; //ʵ�����з���
GCE_XDATA UI16 S_FAN_ON_Time = 0;
GCE_XDATA UI16 S_FAN_OFF_Time = 0;

GCE_XDATA TE_FuncState Comp_SA_EN = DISABLE; // SA������־

void Pump_S_General(void);

// *****************************************************************************
// �������� : Control_data_init
// ����˵�� : ϵͳ�߼����ݳ�ʼ��
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
void Control_data_init(void)
{
   UI08 i = 0;
   G_Pump_Para.on_time = 0;
   G_Pump_Para.off_time = 0;
   G_Pump_Para.BUF = OFF;
   G_Pump_Para.OUT = OFF;

   G_Comp_Para.on_time = 0;
   G_Comp_Para.off_time = 180;
   G_Comp_Para.BUF = OFF;
   G_Comp_Para.OUT = OFF;

   S_Pump_Type.on_timer = 0;
   S_Pump_Type.off_timer = 0;
   S_Pump_Type.on_count = 0;
   S_Pump_Type.step = Step0;
   S_Pump_Type.SW_ON_Timer = 0;
   S_Pump_Type.SW_OFF_Timer = 0;
   S_Pump_Type.off_delay = 0;

   G_Def_Reg.def_start_time = 0;
   G_Def_Reg.def_start_count_enable = 0;
   G_Def_Reg.def_time = 0;
   G_Def_Reg.def_cont = 0;
   G_Def_Reg.temp_room_point = 0;
   G_Def_Reg.def_type = TYPE_A;
   G_Def_Reg.Defrost_status = DISABLE;
   //��Ҫ�״��ϵ�ѹ��������X���Ӻ���жϳ�˪�߼�
   G_Def_Reg.def_Comp_mode = POWER_ON_STOP;

   //����Ҫ�״��ϵ�ѹ��������X���Ӻ���жϳ�˪�߼�
   // G_Def_Reg.def_Comp_mode = NO_FIRST_ON ;
}

// *****************************************************************************
// �������� : Filter_Time_Logic
// ����˵�� : ������ʱ�߼�
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void Filter_Time_Logic(void)
{
   UI16 num = 0;

   if( G_Filter_Run_Time  >=  FILTER_CLEAN_TIME)
   {
       G_Filter_Status = ENABLE ;
   }
   else
   {
       G_Filter_Status = DISABLE ;
   }

   if (!_1S_For_For_SYS)
   {
      return;
   }
   //����
   //���ʱ��Сʱ����
   S_Filter_Minute_Count++;
   if ((S_Filter_Minute_Count < 60) || (G_SYS_Fast_Test))
   {
      return;
   }
   S_Filter_Minute_Count = 0;

   if (G_SYS_Fast_Test)
   {
      num = 60;
   }
   else
   {
      num = 1;
   }
   //
   if (G_Fan_Tyde_Out != OFF_FAN) //���������������ʼ��ʱ
   {
      G_Filter_Run_Time = UI16_Addition_Operation(G_Filter_Run_Time, num);
      //
      if (G_Filter_Run_Time % 60 == 0) //ÿһСʱ��¼һ������ʱ��
      {
         G_Filter_Run_Time_Buf = G_Filter_Run_Time;
      }
   }
}

// *****************************************************************************
// �������� : Prg_s_Control
// ����˵�� : Sʱ��Ƭ
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void Prg_s_Control(void)
{
   UI16 filter_en_timer = 0;
   UI08 num = 0;

   if (!_1S_For_For_SYS)
   {
      return;
   }

   if (G_SYS_Fast_Test)
   {
      num = 60;
   }
   else
   {
      num = 1;
   }

   //
   if ((G_Def_Reg.def_start_count_enable) && (G_Def_Reg.Defrost_status == DISABLE))
   {
      G_Def_Reg.def_start_time = UI16_Addition_Operation(G_Def_Reg.def_start_time, num);
   }
   G_Def_Reg.def_time = UI16_Addition_Operation(G_Def_Reg.def_time, num);
   //
   S_EC1_Count_Time = UI16_Addition_Operation(S_EC1_Count_Time, num);

   S_EC1_COMP_ON_Time = UI16_Addition_Operation(S_EC1_COMP_ON_Time, num);

   S_EC2_Count_Time = UI16_Addition_Operation(S_EC2_Count_Time, num);

   S_EC2_Comp_ON_Time = UI16_Addition_Operation(S_EC2_Comp_ON_Time, num);

   S_High_T_P3_Cont = UI16_Addition_Operation(S_High_T_P3_Cont, num);

   S_High_T_P4_Cont1 = UI16_Addition_Operation(S_High_T_P4_Cont1, num);

   S_High_T_P4_Cont2 = UI16_Addition_Operation(S_High_T_P4_Cont2, num);

   S_High_T_P5_Cont1 = UI16_Addition_Operation(S_High_T_P5_Cont1, num);
   //
   //Pump_S_General();

   if (S_Comp_Test_Time < 0Xffff)
   {
      S_Comp_Test_Time++;
   }

   if (G_Comp_Para.OUT)
   {
      G_Comp_Para.on_time = UI16_Addition_Operation(G_Comp_Para.on_time, num);
      G_Comp_Para.off_time = 0;
   }
   else
   {
      G_Comp_Para.off_time = UI16_Addition_Operation(G_Comp_Para.off_time, num);
      G_Comp_Para.on_time = 0;
   }

   if (G_Fan_Tyde_Out != OFF_FAN)
   {
      S_FAN_ON_Time = UI16_Addition_Operation(S_FAN_ON_Time, num);
      S_FAN_OFF_Time = 0;
   }
   else
   {
      S_FAN_OFF_Time = UI16_Addition_Operation(S_FAN_OFF_Time, num);
      S_FAN_ON_Time = 0;
   }

   G_Fan_Force_Run_Time = UI16_Subtraction_Operation(G_Fan_Force_Run_Time, num);
}

// *****************************************************************************
// �������� : EC_Protect_Load_Logic
// ����˵�� : ��ý���������ؿ����߼�
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void EC_Protect_Load_Logic(void)
{
   if (G_SYS_Power_Status == OFF)
   {
      return;
   }

   if (G_SYS_Mode == mode_SYS_HUM)
   {
      G_Fan_Tyde_Out_Buf = G_SYS_Fan_Tyde;
   }
   else if (G_SYS_Mode == mode_DRY)
   {
      G_Fan_Tyde_Out_Buf = HIGH_FAN;
   }

   G_Comp_Para.BUF = OFF;
}

// *****************************************************************************
// �������� : Ec_Protect1_Logic
// ����˵�� : ��ý��������1�߼�
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void Ec_Protect1_Logic(void)
{
   UI08 g_comp_room_dt = 0; //ѹ�����������²�
   UI08 g_room_coil_dt = 0; //����������²�
   UI08 temp_room_C = 0;
   UI08 temp_coil_C = 0;
   UI08 temp_comp_C = 0;

   if (S_EC1_Err)
   {
      G_Fan_Tyde_Out_Buf = OFF_FAN;
      G_Comp_Para.BUF = OFF;
      return;
   }

   if (G_SYS_Power_Status == OFF)
   {
      return;
   }
   /*
   if(  (G_SYS_Power_Status == OFF)
        ||( (G_Comp_Para.OUT== OFF) && (S_EC1_Step != 2)&&( G_Comp_Para.BUF == OFF ))
        ||(G_Def_Reg.Defrost_status)
        ||(G_Sys_Err.Water_Full)
        ||((GET_COM_STATUS() == ERROR))
       )
   {
       S_EC1_Cycle_Count = 0;
       S_EC1_Count_Time = 0;
      S_EC1_COMP_ON_Time=0;
      S_EC1_Step=0;
       return;
   }
   */
   if (G_Comp_Para.OUT == OFF)
   {
      S_EC1_COMP_ON_Time = 0;
   }

   switch (S_EC1_Step)
   {
   case 0:
   {

      if (S_EC1_COMP_ON_Time >= (15UL * 60)) //ѹ����������15����
      {
         S_EC1_Step = 2;
         S_EC1_Count_Time = 0;
      }
   }
   break;

   case 1:
   {

      if (S_EC1_COMP_ON_Time >= (10UL * 60)) //ѹ����������15����
      {
         S_EC1_Step = 2;
         S_EC1_Count_Time = 0;
      }
   }
   break;

   case 2:
   {
      temp_room_C = GET_ROOM_TEMP_C();
      temp_coil_C = GET_COIL_TEMP_C();
      temp_comp_C = GET_COIL_TEMP_C();

      if (temp_comp_C > temp_room_C)
      {
         g_comp_room_dt = temp_comp_C - temp_room_C;
      }
      else
      {
         g_comp_room_dt = 0;
      }

      if (temp_room_C > temp_coil_C)
      {
         g_room_coil_dt = temp_room_C - temp_coil_C;
      }
      else
      {
         g_room_coil_dt = 0;
      }

      if ((g_comp_room_dt > 50)    //ѹ�����¶ȴ�������50��
          && (g_room_coil_dt > 18) //���´��ڹ���18��
      )
      {
         S_EC1_Step = 3;
         S_EC1_Count_Time = 0;
      }

      if ((g_comp_room_dt < 50)    //ѹ�����¶�С������50��
          && (g_room_coil_dt > 10) //���´��ڹ���18��
      )
      {
         S_EC1_Cycle_Count = 0;
         S_EC1_Count_Time = 0;
         S_EC1_COMP_ON_Time = 0;
         S_EC1_Step = 1;
      }
   }
   break;

   case 3:
   {
      EC_Protect_Load_Logic();
      if (S_EC1_Count_Time > (5UL * 60))
      {
         S_EC1_Cycle_Count++;
         if (S_EC1_Cycle_Count >= 3)
         {
            S_EC1_Step = 3;
            S_EC1_Count_Time = 0;
         }
         else
         {
            S_EC1_Count_Time = 0;
            S_EC1_COMP_ON_Time = 0;
            S_EC1_Step = 1;
         }
      }
   }
   break;

   case 4:
   {
      EC_Protect_Load_Logic();
      if (S_EC1_Count_Time > (10UL * 60))
      {
         S_EC1_Err = TRUE;
         S_EC1_Step = 4;
      }
   }
   break;

   case 5:
   {
   }
   break;

   default:
   {
   }
   break;
   }
}

// *****************************************************************************
// �������� : Ec_Protect2_Logic
// ����˵�� : ��ý��������2�߼�
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
/* static void Ec_Protect2_Logic(void)
{
   UI08 g_dt = 0; //�����²�
   UI08 temp_room_C;
   UI08 temp_coil_C;

   if (S_EC2_Err)
   {
      G_Fan_Tyde_Out_Buf = OFF_FAN;
      G_Comp_Para.BUF = OFF;
      return;
   }

   if ((G_Comp_Para.OUT == OFF) || (G_Def_Reg.Defrost_status) || (G_SYS_Power_Status == OFF))
   {
      S_EC2_Comp_ON_Time = 0;
      S_EC2_Count_Time = 0;
      S_EC2_Step = 0;
      return;
   }

   //
   temp_room_C = GET_ROOM_TEMP_C();
   temp_coil_C = GET_COIL_TEMP_C();
   if (temp_room_C > temp_coil_C)
   {
      g_dt = temp_room_C - temp_coil_C;
   }
   else
   {
      g_dt = 0;
   }

   if (S_EC2_Comp_ON_Time >= 60UL * 120)
   {
      S_EC2_Step = 0;
      S_EC2_Count_Time = 0;
      // S_EC2_Comp_ON_Time = 0 ;
      return;
   }

   switch (S_EC2_Step)
   {
   case 0:
   {
      if (S_EC2_Comp_ON_Time >= 60UL * 5) //ѹ��������δ��5����
      {
         S_EC2_Count_Time = 0;
         S_EC2_Step = 1;
      }
   }
   break;

   case 1:
   {
      if (g_dt <= 23)
      {
         S_EC2_Count_Time = 0;
      }

      if (S_EC2_Count_Time >= 60UL * 1)
      {
         S_EC2_Step = 2;
         S_EC2_Count_Time = 0;
      }
   }
   break;

   case 2:
   {
      if (temp_room_C > (20 + 15))
      {
         S_EC2_Err = TRUE;
         S_EC2_Step = 3;
      }

      if (S_EC2_Count_Time >= 60UL * 1)
      {
         S_EC2_Step = 1;
         S_EC2_Count_Time = 0;
      }
   }
   break;

   case 3:
   {
   }
   break;

   default:
   {
   }
   break;
   }
}
 */
// *****************************************************************************
// �������� : Ec_Protect_Deal
// ����˵�� : ��ý���������
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void Ec_Protect_Deal(void)
{
   Ec_Protect1_Logic();
   // Ec_Protect2_Logic();
}

// *****************************************************************************
// �������� : Defost_Enb
// ����˵�� : ��˪��־ʹ�ܣ���ѭ������ȷ��
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void Defost_Enb(void)
{
   UI08 cycle_index;

   G_Def_Reg.Defrost_status = TRUE;
   G_Def_Reg.def_start_time = 0;
   G_Def_Reg.def_start_count_enable = FALSE;
   if (G_Def_Reg.def_type == TYPE_A)
   {
      cycle_index = 5;
   }
   else
   {
      cycle_index = 4;
   }

   G_Def_Reg.def_cont++;
   if (G_Def_Reg.def_cont > cycle_index)
   {
      G_Def_Reg.def_cont = 1;
   }
}

// *****************************************************************************
// �������� : Def_Start_Jude
// ����˵�� : �����˪�߼��Ĵ���
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void Def_Start_Jude(TE_Bool _condition1, TE_Bool _condition2)
{
   // condition1 ����30���Ӽ�ʱ����
   // condition2 30���Ӽ�ʱ��;��������
   UI16 end_check_time = 0;
   UI16 check_time1 = 0;
   UI16 check_time2 = 0;
   UI16 buf = 0;
   if (_condition1)
   {
      G_Def_Reg.def_start_count_enable = TRUE;
   }

   if (_condition2)
   {
      G_Def_Reg.def_start_count_enable = FALSE;
      G_Def_Reg.def_start_time = 0;
   }

   buf = 2;
   if (G_SYS_Fast_Test)
   {
      buf = buf * 60;
   }
   if (G_Def_Reg.def_start_time <= buf) //��ʼ��ʱǰ2�룬��ȡ��ǰ���£�������˪�г�һ��ѭ���ڶ�ʹ���������
   {

      G_Def_Reg.temp_room_point = GET_ROOM_TEMP_C();
   }

   if ((G_Def_Reg.temp_room_point < defrost_para7) //����С��10��
       || (G_Sys_Err.temp_room_err)                //���¹���
       || (G_Sys_Err.temp_coil_err)                //���¹���
       || (G_Sys_Err.hum_Sensor_err)               //ʪ�ȴ���������
       || (G_Sys_Err.temp_comp_err))
   {
      G_Def_Reg.def_type = TYPE_B;
      check_time1 = defrost_para4;
      check_time2 = 60UL * 19;
   }
   else
   {
      G_Def_Reg.def_type = TYPE_A;
      check_time1 = defrost_para4;
      check_time2 = 60UL * 24;
   }
   //
   //�ڼ�ʱ�г��У��ڵ�10���Ӻ͵�24���ӷֱ���ͭ��SENSOR��Ӧ�����¶�
   if ((G_Def_Reg.def_start_time == check_time1) || (G_Def_Reg.def_start_time == check_time2))
   {
      if (!_condition1)
      {
         G_Def_Reg.def_start_count_enable = FALSE;
         G_Def_Reg.def_start_time = 0; //��ʱ����     �����Ǽ�ʱֹͣ����Ѵ��������
      }
   }
   //
   end_check_time = check_time2 + 60; // 30����

   if (G_Def_Reg.def_start_time >= end_check_time)
   {
      Defost_Enb();
   }
}

// *****************************************************************************
// �������� : Defrost_Logic
// ����˵�� : ��˪�߼�
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void Defrost_Logic(void)
{
   UI16 def_end_time;
   UI16 timer_buf = 0;
   UI08 cycle_index = 0;
   TE_Bool condition1;
   TE_Bool condition2;
   UI16 coil_ad = 0;

   //��¼�Ƿ�ѹ������һ������
   if (G_Def_Reg.def_Comp_mode == POWER_ON_STOP)
   {
      if (G_Comp_Para.OUT == ON)
      {
         G_Def_Reg.def_Comp_mode = FIRST_ON;
      }
   }
   else if (G_Def_Reg.def_Comp_mode == FIRST_ON)
   {
      if (G_Comp_Para.OUT == OFF)
      {
         G_Def_Reg.def_Comp_mode = NO_FIRST_ON;
      }
   }

   //
   if (G_Sys_Err.Water_Full)
   {
      G_Def_Reg.def_cont = 0;
   }

   if ((G_SYS_Power_Status == OFF) || (G_Sys_Err.Water_Full))
   {
      G_Def_Reg.def_start_time = 0;
      G_Def_Reg.def_time = 0;
      G_Def_Reg.Defrost_status = FALSE;

      return;
   }

   //===================
   if (G_Def_Reg.Defrost_status == FALSE) //�ϵ���һ��ѹ��������10�����ڲ����г�˪�ж�
   {
      G_Def_Reg.def_time = 0;

      if (G_Def_Reg.def_Comp_mode == FIRST_ON)
      {
         timer_buf = defrost_para1;
      }
      else
      {
         timer_buf = 0;
      }
      //��˪��ʼ�����ж�
      if ((G_Sys_Err.temp_coil_err) || (G_Sys_Err.temp_comp_err)) //���¹���
      {
         condition1 = (TE_Bool)(G_Comp_Para.OUT);        //ͭ�ܹ���ѹ��������ʱֱ�ӽ����˪�г�2013-3-7
                                                         // &&(Temp_room.value<defrost_para12);//Troom<15
         condition2 = (TE_Bool)(G_Comp_Para.OUT == OFF); // Troom>=16
      }
      else //�޹��ϳ�˪
      {
         coil_ad = GET_COIL_TEMP_AD();
         condition1 = (TE_Bool)((G_Comp_Para.on_time >= timer_buf) && (coil_ad <= defrost_para2)); // coil<0?
         condition2 = (TE_Bool)((coil_ad >= defrost_para3) || (G_Comp_Para.OUT == OFF));           // coil>=20
      }
      Def_Start_Jude(condition1, condition2);
   }
   else
   {
      //��˪����
      G_Comp_Para.BUF = OFF;
      G_Fan_Tyde_Out_Buf = HIGH_FAN;
      G_Set_Fan_Tyde_EN = DISABLE;

      //��˪���������ж�
      if (G_Def_Reg.def_type == TYPE_A)
      {
         cycle_index = 5;
         def_end_time = defrost_para8;
      }
      else
      {
         cycle_index = 4;
         def_end_time = defrost_para9;
      }

      if (G_Def_Reg.def_cont == cycle_index)
      {
         def_end_time = defrost_para11;
      }

      if (G_Def_Reg.def_time >= def_end_time)
      {
         G_Def_Reg.Defrost_status = FALSE;
      }
   }
}

// *****************************************************************************
// �������� : HiPriority_Protect_Deal
// ����˵�� : ��ˮ��ʾ/ͨ�Ź��� ����
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void HiPriority_Protect_Deal(void)
{
   SHORTCUT_STATUS status_buf;

   status_buf = GET_WATER_FULL_IO_STATUS();
   if (status_buf == DI_CUT)
   {
      if (G_Sys_Err.Water_Full == DISABLE)
      {
         G_Buzz_Cnt = 5;
      }
      G_Sys_Err.Water_Full = ENABLE;
   }
   else
   {
      if (G_Sys_Err.Water_Full == ENABLE)
      {
         G_Fan_Force_Run_Time = 180; //���ˮ�����������ǿ�ƿ���180��
      }
      G_Sys_Err.Water_Full = DISABLE;
      G_Buzz_Cnt = 0;
   }

   //���ϱ����������
   if (((GET_COM_STATUS() == ERROR)) || (G_Sys_Err.Water_Full))
   {
      G_Pump_Para.BUF = OFF;
      G_Comp_Para.BUF = OFF;
      G_Fan_Tyde_Out_Buf = OFF_FAN;

      G_Fan_Force_Run_Time = 0;
   }
}

// *****************************************************************************
// �������� : Sys_Sensor_Err_Deal
// ����˵�� : ϵͳ�������쳣����
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void Sys_Sensor_Err_Deal(void)
{
   TS_AISTATUS status_buf;

   status_buf = GET_ROOM_TEMP_SENSOR_STATUS();
   if (status_buf != AI_NORMAL) //����·
   {
      G_Sys_Err.temp_room_err = ENABLE;
   }
   else
   {
      G_Sys_Err.temp_room_err = DISABLE;
   }

   status_buf = GET_COIL_TEMP_SENSOR_STATUS();
   if (status_buf != AI_NORMAL) //����·
   {
      G_Sys_Err.temp_coil_err = ENABLE;
   }
   else
   {
      G_Sys_Err.temp_coil_err = DISABLE;
   }

   status_buf = GET_Comp_TEMP_SENSOR_STATUS();
   if (status_buf != AI_NORMAL) //����·
   {
      G_Sys_Err.temp_comp_err = ENABLE;
   }
   else
   {
      G_Sys_Err.temp_comp_err = DISABLE;
   }

   status_buf = GET_ROOM_HUM_SENSOR_STATUS();
   if (status_buf != AI_NORMAL) //����·
   {
      G_Sys_Err.hum_Sensor_err = ENABLE;
   }
   else
   {
      G_Sys_Err.hum_Sensor_err = DISABLE;
   }

   if ((G_SYS_Power_Status) && ((G_SYS_Mode == mode_DRY) || (G_SYS_Mode == mode_SYS_HUM))) //��Ҫѹ����������ģʽ
   {
      if ((G_Sys_Err.temp_room_err)     //���¹���
          || (G_Sys_Err.temp_coil_err)  //���¹���
          || (G_Sys_Err.hum_Sensor_err) //ʪ�ȹ���
          || (G_Sys_Err.temp_comp_err)  //�³����¹���
      )
      {
         G_Comp_Para.BUF = OFF;
         G_Fan_Tyde_Out_Buf = OFF_FAN;
      }
   }
}

// *****************************************************************************
// �������� : Comp_OverLoad
// ����˵�� : ѹ�������ر���
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void Comp_OverLoad(void)
{
   if (G_Comp_Overtime_Protect_Flag)
   {
      if (G_Comp_Para.on_time >= 3600UL * 12) //��������12Сʱ�����
      {
         Set_Power_Status();
         G_Fan_Force_Run_Time = 0;
      }
   }
}

// *****************************************************************************
// �������� : High_Temperature_Protection2
// ����˵�� : ���±����߼�2
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void High_Temperature_Protection2(void)
{
   UI08 temp_room_C;

   if ((G_SYS_Power_Status == OFF) || (G_Sys_Err.Water_Full))
   {
      S_High_T_P2_Step = Step0;
      return;
   }

   //////

   temp_room_C = GET_ROOM_TEMP_C();

   switch (S_High_T_P2_Step)
   {
   case Step0:
   {
      // if( (G_Comp_Para.OUT == ON)
      //	 &&(temp_room_C > (42+15))
      if (temp_room_C > (42 + 15))
      {
         S_High_T_P2_Step = Step1;
      }
   }
   break;
   case Step1:
   {
      G_Comp_Para.BUF = OFF; //ע��:���ѹ������ǿ����ת3����ʱ��ҲҪǿ�ƹر�
      G_Fan_Tyde_Out_Buf = OFF_FAN;

      if (temp_room_C <= (41 + 15))
      {

         S_High_T_P2_Step = Step0;
      }
   }
   break;

   case Step3:
   {
   }
   break;

   default:
   {
   }
   break;
   }
}

// *****************************************************************************
// �������� : High_Temperature_Protection1
// ����˵�� : ���±����߼�1
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void High_Temperature_Protection1(void)
{
   static GCE_XDATA TU_Step s_high_T_P1_step = Step0;
   UI08 temp_room_C;

   if ((G_SYS_Power_Status == OFF) || (G_Sys_Err.Water_Full))
   {
      s_high_T_P1_step = Step0;
      return;
   }

   temp_room_C = GET_ROOM_TEMP_C();

   switch (s_high_T_P1_step)
   {
   case Step0:
   {
      if (temp_room_C > (32 + 15))
      {
         s_high_T_P1_step = Step1;
      }
   }
   break;

   case Step1:
   {
      G_Fan_Tyde_Out_Buf = HIGH_FAN;

      if (temp_room_C <= (31 + 15))
      {
         s_high_T_P1_step = Step0;
      }
   }
   break;

   default:
   {
   }
   break;
   }
}

static void High_Temperature_Protection3(void)
{
   static GCE_XDATA TU_Step s_high_T_P3_step = Step0;
   UI16 temp_comp_t;

   if ((G_SYS_Power_Status == OFF) || (G_Sys_Err.Water_Full))
   {
      s_high_T_P3_step = Step0;
      S_High_T_P3_Cont = 0;
      return;
   }

   temp_comp_t = GET_Comp_TEMP_C();

   switch (s_high_T_P3_step)
   {
   case Step0:
   {
      if ((temp_comp_t > (85 + 15)) // 85���϶�
                                    //&&(G_Comp_Para.OUT == ON)
      )
      {
         s_high_T_P3_step = Step1;
      }

      if ((temp_comp_t > (90 + 15)) // 90���϶�
          && (G_Comp_Para.OUT == ON))
      {
         s_high_T_P3_step = Step2;
      }
   }
   break;

   case Step1:
   {
      G_Fan_Tyde_Out_Buf = HIGH_FAN;

      if ((temp_comp_t > (90 + 15)) // 90���϶�
          && (G_Comp_Para.OUT == ON))
      {
         s_high_T_P3_step = Step2;
      }

      if (temp_comp_t <= (84 + 15)) // 84���϶�
      {
         s_high_T_P3_step = Step0;
      }
   }
   break;

   case Step2:
   {
      G_Comp_Para.BUF = OFF;
      G_Fan_Tyde_Out_Buf = HIGH_FAN;

      if (temp_comp_t <= (45 + 15)) // 45���϶�
      {
         S_High_T_P3_Cycle_Cont++;
         if (S_High_T_P3_Cycle_Cont >= 3)
         {
            s_high_T_P3_step = Step3;
            S_High_T_P3_Cont = 0;
         }
         else
         {
            s_high_T_P3_step = Step0;
         }
      }
   }
   break;

   case Step3:
   {
      G_Comp_Para.BUF = OFF;

      if (S_High_T_P3_Cont >= 180) //����180��
      {
         G_Fan_Tyde_Out_Buf = OFF_FAN;
         G_High_T_P3_Error_Status = ERROR;
      }
      else
      {
         G_Fan_Tyde_Out_Buf = HIGH_FAN;
      }
   }
   break;
   }
}

//�_�C�r���h���ضȂɜy���R40��C�r�C̨�������\�D��ָʾ���@ʾE4��
void Turn_On_High_Temperature_Deal(void)
{
   UI08 temp_room_C;

   if (G_SYS_Power_Status_Old == G_SYS_Power_Status)
   {
      return;
   }

   if ((G_SYS_Power_Status == ON) && (G_SYS_Power_Status_Old == OFF))
   {
      temp_room_C = GET_ROOM_TEMP_C();

      if (temp_room_C >= (40 + 15))
      {
         G_Turn_On_H_T_Error_Status = ERROR;
      }
   }
   G_SYS_Power_Status_Old = G_SYS_Power_Status;
}

// *****************************************************************************
// �������� : High_Temperature_Protection4
// ����˵�� : ���±����߼�4
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void High_Temperature_Protection4(void)
{
   static GCE_XDATA TU_Step s_high_T_P4_step = Step0;
   UI08 temp_room_C;

   Turn_On_High_Temperature_Deal();

   if ((G_SYS_Power_Status == OFF) || (G_Sys_Err.Water_Full))
   {
      G_High_T_P4_Error_Status = SUCCESS;
      s_high_T_P4_step = Step0;
      return;
   }

   temp_room_C = GET_ROOM_TEMP_C();

   switch (s_high_T_P4_step)
   {
   case Step0:
   {
      G_High_T_P4_Error_Status = SUCCESS;

      if (temp_room_C < (40 + 15))
      {
         S_High_T_P4_Cont1 = 0;
      }

      if (G_Comp_Para.OUT == ON)
      {
         if (S_High_T_P4_Cont1 >= (5UL * 60))
         {
            s_high_T_P4_step = Step1;
            S_High_T_P4_Cont1 = 0;
            S_High_T_P4_Cont2 = 0;
            G_High_T_P4_Error_Status = ERROR;
         }
      }
      else
      {
         S_High_T_P4_Cont1 = 0;
      }
   }
   break;

   case Step1:
   {

      G_Comp_Para.BUF = OFF;

      if (S_High_T_P4_Cont1 >= (5UL * 60)) //����180��
      {
         G_Fan_Tyde_Out_Buf = OFF_FAN;
      }
      else
      {
         G_Fan_Tyde_Out_Buf = HIGH_FAN;
      }

      if (temp_room_C > (40 + 15))
      {
         S_High_T_P4_Cont2 = 0;
      }
      else
      {
         if (S_High_T_P4_Cont2 >= (5UL * 60))
         {
            s_high_T_P4_step = Step0;
            S_High_T_P4_Cont1 = 0;
            G_High_T_P4_Error_Status = SUCCESS;
         }
      }
   }
   break;

   default:
   {
   }
   break;
   }
}

// *****************************************************************************
// �������� : High_Temperature_Protection5
// ����˵�� : ���±����߼�4
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void High_Temperature_Protection5(void)
{
   static GCE_XDATA TU_Step s_high_T_P5_step = Step0;
   UI08 temp_buf;

   if ((G_SYS_Power_Status == OFF) || (G_Sys_Err.Water_Full))
   {
      s_high_T_P5_step = Step0;
      S_High_T_P5_Cont1 = 0;
      S_High_T_P5_Cycle_Cont = 0;
      return;
   }

   if (G_High_T_P5_Error_Status == ERROR)
   {
      G_Comp_Para.BUF = OFF;
      G_Fan_Tyde_Out_Buf = OFF_FAN;
      return;
   }

   temp_buf = GET_Comp_TEMP_C();

   if (S_High_T_P5_Cont1 >= (30UL * 60))
   {
      S_High_T_P5_Cont1 = 0;
      S_High_T_P5_Cycle_Cont = 0;
   }
   else
   {
      if (S_High_T_P5_Cycle_Cont >= 3)
      {
         G_High_T_P5_Error_Status = ERROR;
         return;
      }
   }

   switch (s_high_T_P5_step)
   {
   case Step0: //��ʼ
   {
      S_High_T_P5_LT = temp_buf;
      S_High_T_P5_HT = temp_buf;
      s_high_T_P5_step = 1;
      S_High_T_P5_Cont1 = 0;
      S_High_T_P5_Cycle_Cont = 0;
   }
   break;

   case Step1: //����
   {
      if (temp_buf < S_High_T_P5_LT)
      {
         S_High_T_P5_LT = temp_buf;
         S_High_T_P5_HT = S_High_T_P5_LT;
      }
      else if (temp_buf > S_High_T_P5_HT)
      {
         S_High_T_P5_HT = temp_buf;
      }

      if ((S_High_T_P5_HT > S_High_T_P5_LT) && (S_High_T_P5_HT - S_High_T_P5_LT) > 15)
      {
         s_high_T_P5_step = 2;
         S_High_T_P5_LT = S_High_T_P5_HT;
      }
   }
   break;

   case Step2: //�½�
   {

      if (temp_buf < S_High_T_P5_LT)
      {
         S_High_T_P5_LT = temp_buf;
      }
      else if (temp_buf > S_High_T_P5_HT)
      {
         S_High_T_P5_HT = temp_buf;
         S_High_T_P5_LT = S_High_T_P5_HT;
      }

      if ((S_High_T_P5_HT > S_High_T_P5_LT) && (S_High_T_P5_HT - S_High_T_P5_LT) > 15)
      {
         s_high_T_P5_step = 1;
         S_High_T_P5_HT = S_High_T_P5_LT;
         if (S_High_T_P5_Cycle_Cont < 0xff)
         {
            S_High_T_P5_Cycle_Cont++;
         }
      }
   }
   break;

   default:
   {
   }
   break;
   }
}
// *****************************************************************************
// �������� : High_T_Protect_Deal
// ����˵�� : ���±����߼��ܵ���
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void High_T_Protect_Deal(void)
{
   High_Temperature_Protection1(); //  4-1.�h���ߜر��o:  32��
   High_Temperature_Protection2(); //  4-1.�h���ߜر��o:  42��
   High_Temperature_Protection3(); // 2 . ���s�C�����ضȱ��o  2.2
   High_Temperature_Protection4(); // 2 . ���s�C�����ضȱ��o  2-3
   High_Temperature_Protection5(); // 2 . ���s�C�����ضȱ��o  2-5
}

// *****************************************************************************
// �������� : Protect_Logic
// ����˵�� : �����߼��ܵ���
// ��ڲ��� :
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void Protect_Logic(void)
{
   Sys_Sensor_Err_Deal();
   Defrost_Logic();
   High_T_Protect_Deal();
   Ec_Protect_Deal();
   Comp_OverLoad();
   HiPriority_Protect_Deal();
}

// *****************************************************************************
// �������� : Comp_Safe_Logic
// ����˵�� : ѹ�����;��Ա�������
// ��ڲ��� :
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void Comp_Safe_Logic(void)
{
   if (G_Comp_Para.BUF == ON)
   {
      if ((G_Comp_Para.off_time < COMP_PROTECT_OFF_TIME) && (G_Comp_Para.OUT == OFF)) //ѹ����ͣ������ʱ��δ�� ��ѹ����δ��ת
      {
         G_Comp_Para.OUT = OFF;
      }
      else
      {
         G_Comp_Para.OUT = ON;
         G_Comp_Para.off_time = 0; //������ٰ����ػ�����ѹ�������Բ��Ͽ����
      }
   }
   else
   {
      if ((G_SYS_Power_Status == OFF)                     //��̨�ػ�
          || (G_Sys_Err.Water_Full)                       //��̨����ˮ��
          || (G_Comp_Para.OUT == OFF)                     //ѹ����ԭ��Ϊ��
          || (S_High_T_P2_Step == Step1)                  //���±����߼�2�����´���42�ȣ�ǿ�ƹر�ѹ����
          || (G_Comp_Para.on_time > COMP_PROTECT_ON_TIME) //ѹ����С����ʱ�䵽
          || (G_Sys_Err.temp_room_err)                    //���¹���
          || (G_Sys_Err.temp_coil_err)                    //���¹���
          || (G_Sys_Err.hum_Sensor_err)                   //ʪ�ȹ���
          || (G_Sys_Err.temp_comp_err)                    //�³����¹���
      )
      {
         G_Comp_Para.OUT = OFF;
         G_Comp_Para.on_time = 0; //������ٰ����ػ�����ѹ�������Բ��Ͽ����
      }
      else
      {
         G_Comp_Para.OUT = ON;
      }
   }
}

// *****************************************************************************
// �������� : Load_Set
// ����˵�� : ����״̬���ȷ�� buf ����� out
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա :
// �����Ա :
// ������� :
// �޸ļ�¼ : 2021.5.13  V1.0�״η���
// *****************************************************************************
static void Load_Set(void)
{
   Comp_Safe_Logic();
   //-----------------------------------------------ˮ�ÿ���
  /*
   if (G_Pump_Para.BUF == ON)
   {
      G_Pump_Para.OUT = ON;
   }
   else
   {
      G_Pump_Para.OUT = OFF;
   }
  */
   G_Pump_Para.OUT = OFF;

   G_Fan_Tyde_Out = G_Fan_Tyde_Out_Buf;
   if (G_Fan_Tyde_Out == OFF_FAN) //����ر�ʱ����������������ʱ��
   {
      S_FAN_ON_Time = 0;
   }
}

// *****************************************************************************
// �������� : Pump_S_General
// ����˵�� : ˮ���߼������ʱ����
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա :
// �����Ա :
// ������� :
// �޸ļ�¼ : 2021.5.13  V1.0�״η���
// *****************************************************************************
/*
static void Pump_S_General(void)
{
   SHORTCUT_STATUS status_buf;

   status_buf = GET_PUMP_FULL_IO_STATUS();
   if (status_buf == DI_SHORT)
   {
      S_Pump_Type.off_delay = 0;
   }
   else
   {
      if (S_Pump_Type.off_delay < 0xff)
      {
         S_Pump_Type.off_delay++;
      }
   }
   //
   if (G_Pump_Para.BUF == ON)
   {
      if (S_Pump_Type.on_timer < 0xffff)
      {
         S_Pump_Type.on_timer++;
      }
      S_Pump_Type.off_timer = 0;
   }
   else
   {
      if (S_Pump_Type.off_timer < 0xffff)
      {
         S_Pump_Type.off_timer++;
      }
      S_Pump_Type.on_timer = 0;
   }
}
*/
// *****************************************************************************
// �������� : Pump_Control
// ����˵�� : ˮ���߼�
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա :
// �����Ա :
// ������� :
// �޸ļ�¼ : 2021.5.13  V1.0�״η���
// *****************************************************************************
/*
static void Pump_Control(void)
{
   SHORTCUT_STATUS io_status;

   io_status = GET_PUMP_WATER_PIPE_STATUS(); //ˮ�ܽ���״̬
   if ((G_Pump_Status == DISABLE) || (io_status == DI_CUT) || (S_Pump_Type.off_delay >= 3))
   {
      G_Pump_Para.BUF = OFF;
      //��ʱ���¿�ʼ
      S_Pump_Type.step = Step0;
      S_Pump_Type.on_count = 0;
      return;
   }
   //
   switch (S_Pump_Type.step)
   {
   case Step0:
   {
      io_status = GET_PUMP_FULL_IO_STATUS(); //��ˮ�俪��״̬
      if (io_status == DI_SHORT)             //�ͻ�Ҫ��ˮ�ò��濪�ػ�����
      {
         G_Pump_Para.BUF = ON;
         S_Pump_Type.off_delay = 0;
         S_Pump_Type.step = Step1;
      }
      else
      {
         G_Pump_Para.BUF = OFF;
      }
   }
   break;
   //
   case Step1:
   {
      G_Pump_Para.BUF = ON;
      if (S_Pump_Type.on_timer >= 300)
      {
         S_Pump_Type.step = Step2;
         G_Pump_Para.BUF = OFF;
      }
   }
   break;
   //
   case Step2:
   {
      G_Pump_Para.BUF = OFF;
      if (S_Pump_Type.off_timer >= 180)
      {

         S_Pump_Type.on_count++;
         if (S_Pump_Type.on_count >= 3)
         {
            S_Pump_Type.step = Step3;
            G_Pump_Para.BUF = OFF;
         }
         else
         {
            S_Pump_Type.step = Step1;
            G_Pump_Para.BUF = ON;
         }
      }
   }
   break;
   //
   case Step3:
   {
      G_Pump_Para.BUF = OFF;
   }
   break;
   //
   default:
   {
   }
   break;
   }
   //
}
*/
// *****************************************************************************
// �������� : HUM_Mode_Logic
// ����˵�� : ��ʪģʽ�����߼�
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա :
// �����Ա :
// ������� :
// �޸ļ�¼ : 2021.5.13  V1.0�״η���
// *****************************************************************************
static void Hum_Mode_Logic(void)
{
   UI08 hum_buf = 0;
   hum_buf = GET_ROOM_HUM();

   //ѹ����
   if (G_SYS_Hum_Set == 25)
   {
      G_Comp_Para.BUF = ON;
   }
   else if (hum_buf >= (G_SYS_Hum_Set + 5)) //������ʪ��>�趨ʪ��5%ѹ��������
   {
      G_Comp_Para.BUF = ON;
   }
   else if ((hum_buf + 5) <= G_SYS_Hum_Set) //������ʪ��<�趨ʪ��5%ѹ����ֹͣ
   {
      G_Comp_Para.BUF = OFF;
   }

   //���
   // if ((G_Comp_Para.OUT == ON)        //ѹ������תʱ
   //     || (G_Fan_Force_Run_Time > 0)) //ÿ�ΰ���Դ����������ʱ������������ˮ���������֮����ǿ�ư��趨������ת3����
   // {
   //    G_Fan_Tyde_Out_Buf = G_SYS_Fan_Tyde;
   // }
   // else
   // {
   //    G_Fan_Tyde_Out_Buf = OFF_FAN;
   // }
   G_Fan_Tyde_Out_Buf = G_SYS_Fan_Tyde;
   G_Set_Fan_Tyde_EN = ENABLE;
}

// *****************************************************************************
// �������� : Dry_Mode_Logic
// ����˵�� : ����ģʽ�߼�
// ��ڲ��� :
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void Dry_Mode_Logic(void)
{
   //ѹ����
   G_Comp_Para.BUF = ON;

   //���
   G_Set_Fan_Tyde_EN = DISABLE;
   G_Fan_Tyde_Out_Buf = HIGH_FAN;
}

// *****************************************************************************
// �������� : Comp_Test_General
// ����˵�� : ѹ����ǿ�Ʋ���ģʽ�߼�
// ��ڲ��� :
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void Comp_Test_General(void)
{
   if (!G_Comp_Test_EN)
   {
      S_Comp_Test_Time = 0;
      return;
   }

   if (S_Comp_Test_Time < COMP_TEST_ON_TIME)
   {
      G_Comp_Para.OUT = ON;
      G_Fan_Tyde_Out_Buf = HIGH_FAN;
   }
   else if (S_Comp_Test_Time < (COMP_TEST_ON_TIME + COMP_TEST_OFF_TIME))
   {
      G_Comp_Para.OUT = OFF;
      G_Fan_Tyde_Out_Buf = OFF_FAN;
   }
   else
   {
      S_Comp_Test_Time = 0;
   }
}

// *****************************************************************************
// �������� : SYS_Mode_Change_Handle
// ����˵�� : ģʽ�л�����  ȷ����Щ������Ҫ�޸�
// ��ڲ��� : _now_mode   ��ǰϵͳ���е�ģʽ
//           _set_mode    ��Ҫ�л�Ϊ�µ�ģʽ
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��  ģʽ�л�����ôκ���
//
// *****************************************************************************
void SYS_Mode_Change_Handle(TU_SYS_Mode _now_mode, TU_SYS_Mode _set_mode)
{
   switch (_set_mode)
   {
   case mode_DRY:
   {
      if (_now_mode)
      {
      }
   }
   break;

   case mode_SYS_HUM:
   {
   }
   break;

   default:
   {
   }
   break;
   }
}

// *****************************************************************************
// �������� : Sys_Control
// ����˵�� : ϵͳ�߼�����������
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
void Sys_Control(void)
{
   Prg_s_Control();
   //
   if (G_SYS_Self_Test)
   {
      return;
   }
   //
   if (G_SYS_Power_Status) //����״̬
   {
      if (G_SYS_Mode == mode_DRY)
      {
         Dry_Mode_Logic();
      }
      else if (G_SYS_Mode == mode_SYS_HUM)
      {
         Hum_Mode_Logic();
      }
   }
   else //�ػ�״̬
   {
      G_Fan_Tyde_Out_Buf = OFF_FAN;
      G_Comp_Para.BUF = OFF;
   }

   Comp_Test_General();
   //Pump_Control(); //��ˮPUMP����
   Protect_Logic(); //�����߼�
   Load_Set(); //�������
   Filter_Time_Logic();
}
