#include "General.h"

// GCE_XDATA RGB_LED_TYPE AirQ_Class_LED_buf = 0;

GCE_XDATA UI08 G_Disp_Machine_Temp_Time = 0; //������ʱ����ʾ���¹��µ�ʱ��

GCE_XDATA UUI08 S_LED_Data_Buf[COM_total] = {0}; // LED��ʾdata

GCE_XDATA UI08 S_CT1642_Display_Data_Buf[4] = {0xFF, 0xFF, 0X00, 0X00}; // �������ʾ�����ݴ�

GCE_XDATA TE_Flag_Status S_Flash_mS500 = SET; // 500ms��˸

GCE_XDATA UI16 S_Comp_Test_Disp_En_Time = 0; //����ѹ����ǿ�Ʋ���ģʽʱ ȫ��ʾʱ��

GCE_XDATA UI08 S_Test_cont1 = 0; //�Լ����

/******************************************************************************
 *         �������ʾ���ݶ���
 *******************************************************************************/
GCE_CONST UI08 BCD_tab[] = /*LED��ʾ���룬���ڲ��*/
    {
        (DATA_0), /*0*/
        (DATA_1), /*1*/
        (DATA_2), /*2*/
        (DATA_3), /*3*/
        (DATA_4), /*4*/
        (DATA_5), /*5*/
        (DATA_6), /*6*/
        (DATA_7), /*7*/
        (DATA_8), /*8*/
        (DATA_9), /*9*/
};

// *****************************************************************************
// �������� : LED_mS10_Deal
// ����˵�� : 10mSʱ��Ƭ
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
void LED_mS10_Deal(void)
{
   if (!_10mS_For_SYS)
   {
      return;
   }

   if (S_Comp_Test_Disp_En_Time > 0)
   {
      S_Comp_Test_Disp_En_Time--;
   }
}

// *****************************************************************************
// �������� : Prg_mS250_DSP
// ����˵�� : 250mSʱ��Ƭ
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
static void Prg_mS250_DSP(void)
{
   if (!_250mS_For_SYS)
   {
      return;
   }

   if ((G_SYS_Self_Test) && (G_Power_Delay_Time == 0))
   {
      if (S_Test_cont1 < 0xff)
      {
         S_Test_cont1++;
      }
   }
}

// *****************************************************************************
// �������� : Prg_mS500_DSP
// ����˵�� : 500mSʱ��Ƭ
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
void Prg_mS500_DSP(void)
{
   if (!_500mS_For_SYS)
   {
      return;
   }

   S_Flash_mS500 = (!S_Flash_mS500);
}

// *****************************************************************************
// �������� : Prg_S_DSP
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
void Prg_S_DSP(void)
{
   if (!_1S_For_For_SYS)
   {
      return;
   }

   if (G_Disp_Machine_Temp_Time > 0)
   {
      G_Disp_Machine_Temp_Time--;
   }
}

// *****************************************************************************
// �������� : Dev_Get_Key_Number
// ����˵�� : ����ʶ��
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
void dig1_2_dsp(UI08 _disp_data)
{
   if (_disp_data >= 100)
   {
      return;
   }
   dig2_num = BCD_tab[_disp_data % 10];
   dig1_num = BCD_tab[_disp_data / 10];
}

// *****************************************************************************
// �������� : Disp_Hum_Value
// ����˵�� : ʪ����ʾ
// ��ڲ��� : _data_buf  ʪ����ʾֵ
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
void Disp_Hum_Value(UI08 _data_buf)
{
   if (_data_buf >= 90)
   {
      dig1_num = DATA_9; // DATA_H;  20131122
      dig2_num = DATA_0; // DATA_1;
   }
   else if (_data_buf <= 20)
   {
      dig1_num = DATA_2; // DATA_L;  20131122
      dig2_num = DATA_0; // DATA_o;
   }
   else
   {
      dig1_2_dsp(_data_buf);
   }
   dig2_num |= BIT_P;
}

// *****************************************************************************
// �������� : Clear_Display_Data_Buf
// ����˵�� : ���������ʾ����
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
void Clear_Display_Data_Buf(void)
{
   UI08 i = 0;

   for (i = 0; i < sizeof(S_CT1642_Display_Data_Buf) / sizeof(S_CT1642_Display_Data_Buf[0]); i++)
   {
      S_CT1642_Display_Data_Buf[i] = 0;
   }

   for (i = 0; i < sizeof(S_LED_Data_Buf) / sizeof(S_CT1642_Display_Data_Buf[0]); i++)
   {
      S_LED_Data_Buf[i].byte = 0;
   }
}

// *****************************************************************************
// �������� : LedDsp_Test
// ����˵�� : LED������ʾ
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : Aysi-E
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
void LedDsp_Test(void)
{
   static GCE_XDATA UI08 test_key_data = 0; 
   static GCE_XDATA UI08 test_seq = 0;    
   GCE_XDATA UI08 key_num = 0;

   key_num = Get_Key_Data();
   Clear_Key_Data;
   if (test_seq == 0) //��ʾ���
   {
      switch (S_Test_cont1)
      {
      case 0:
      {
         Clear_Display_Data_Buf();
         dig1_num = DATA_C;
         dig2_num = DATA_H;
      }
      break;
      case 1:
      {
         LED_water;
      }
      break;
      case 2:
      {
         LED_def;
      }
      break;
      case 3:
      {
         LED_HUM_MODE;
      }
      break;
      case 4:
      {
         LED_DYR_MODE;
      }
      break;
      case 5:
      {
         LED_FAN_HIGH;
      }
      break;
      case 6:
      {
         LED_FAN_MID;
      }
      break;
      case 7:
      {
         LED_FAN_LOW;
      }
      break;
      case 8:
      {
         LED_WIFI;
      }
      break;
      case 9:
      {
         LED_timer;
      }
      break;
      case 10:
      {
         LED_PUMP;
      }
      break;
      default:
      {
         S_Test_cont1 = 0;
      }
      break;
      }
   }

   if (test_seq == 2) //�����ʾ
   {
      Clear_Display_Data_Buf();
      if (GET_COM_STATUS() == ERROR)
      {
         dig1_num = DATA_E;
         dig2_num = DATA_5;
      }
      else if (G_Uart_Test_Error)
      {
         dig1_num = DATA_E;
         dig2_num = DATA_6;
      }
      else if ((GET_TEST_AD() <= 470) || (GET_TEST_AD() >= 550))
      {
         dig1_num = DATA_E;
         dig2_num = DATA_7;
      }
      else
      {
         dig1_num = DATA_r;
         dig2_num = BCD_tab[Soft_Version];
      }
   }

   switch (key_num) //�������
   {
   case POWER_KEY:
      test_key_data |= bit0;
      break;
   case MODE_KEY:
      test_key_data |= bit1;
      break;
   case FAN_KEY:
      test_key_data |= bit2;
      break;
   case DOWN_KEY:
      test_key_data |= bit3;
      break;
   case UP_KEY:
      test_key_data |= bit4;
      break;
   case SET_TIME_KEY:
      test_key_data |= bit5;
      break;
   case FILTER_CLEAN_KEY:
      test_key_data |= bit6;
      break;
   default: break;
   }

   if (test_key_data == 0x7f) //������
   {
      test_seq = 2; //��ת�������ʾ
   }

   if (key_num)
   {
      G_Buzz_Time = BUZZ_short_time;
   }
}

// *****************************************************************************
// �������� : Disp_All
// ����˵�� : LED/�����ȫ��
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
void Disp_All(void)
{
   UI08 i = 0;
   for (i = 0; i < sizeof(S_CT1642_Display_Data_Buf) / sizeof(S_CT1642_Display_Data_Buf[0]); i++)
   {
      S_CT1642_Display_Data_Buf[i] = 0xff;
   }

   for (i = 0; i < sizeof(S_LED_Data_Buf) / sizeof(S_LED_Data_Buf[0]); i++)
   {
      S_LED_Data_Buf[i].byte = 0xff;
   }
}

// *****************************************************************************
// �������� : Disp_Temp
// ����˵�� : �¶���ʾ
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
void Disp_Temp(UI08 _temp_buf)
{
   if (_temp_buf >= 15) // 15����0��
   {
      dig1_2_dsp(_temp_buf - 15);
   }
   else
   {
      _temp_buf = 15 - _temp_buf;
      if (_temp_buf > 9)
      {
         _temp_buf = 9;
      }
      dig1_2_dsp(_temp_buf);
      dig1_num = DATA_neg; //��ʾ�¶ȸ���
   }
}

// *****************************************************************************
// �������� : Disp_Machine_Temp
// ����˵�� : ��̨�¶���ʾ
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
void Disp_Machine_Temp(void)
{
   UI08 dsp_temp = 0;
   //
   if (G_Disp_Machine_Temp_Time == 7)
   {
      dsp_temp = GET_ROOM_TEMP_C();
   }
   else if (G_Disp_Machine_Temp_Time == 5)
   {
      dsp_temp = GET_COIL_TEMP_C();
   }
   else if (G_Disp_Machine_Temp_Time == 3)
   {
      Disp_All();
      return;
   }
   else
   {
      return;
   }

   Disp_Temp(dsp_temp);
}

// *****************************************************************************
// �������� : FAN_Speed_Disp
// ����˵�� : ������ʾ
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
void FAN_Speed_Disp(TU_FAN_Speed_Type _fan_buf)
{
   if (_fan_buf == HIGH_FAN)
   {
      LED_FAN_HIGH;
   }
   else if (_fan_buf == MID_FAN)
   {
      LED_FAN_MID;
   }
   else if (_fan_buf == LOW_FAN)
   {
      LED_FAN_LOW;
   }
}

// *****************************************************************************
// �������� : LedDsp_Content
// ����˵�� : ��ʾ�߼�
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
static void LED_Dsp_Content(void)
{
   UI08 hum_dsp_com = 60; //��ʾʪ��
   // SHORTCUT_STATUS io_status;

   //���������ʾ����
   Clear_Display_Data_Buf();

   //
   if (G_Power_Delay_Time > 0)
   {
      Disp_All();
      return;
   }

   //��������ʾ�¶�
   if (G_Disp_Machine_Temp_Time > 0)
   {
      Disp_Machine_Temp();
      return;
   }

   //����ѹ����ǿ�Ʋ���ȫ��
   if (S_Comp_Test_Disp_En_Time > 0)
   {
      Disp_All();
      return;
   }

   //������ʾ
   //�������ʾ���ϴ���
   if (GET_COM_STATUS() == ERROR)
   {
      dig1_num = DATA_E;
      dig2_num = DATA_5;
      return;
   }
   else if (G_Sys_Err.temp_room_err)
   {
      dig1_num = DATA_E;
      dig2_num = DATA_1;
      dig2_num &= ~BIT_P;
      return;
   }
   else if (G_Sys_Err.temp_coil_err)
   {
      dig1_num = DATA_E;
      dig2_num = DATA_2;
      dig2_num &= ~BIT_P;
      return;
   }
   else if (G_Sys_Err.hum_Sensor_err)
   {
      dig1_num = DATA_E;
      dig2_num = DATA_3;
      dig2_num &= ~BIT_P;
      return;
   }
   else if (G_Sys_Err.temp_comp_err)
   {
      dig1_num = DATA_E;
      dig2_num = DATA_6;
      dig2_num &= ~BIT_P;
      return;
   }
   else if ((G_High_T_P4_Error_Status = ERROR) || (G_Turn_On_H_T_Error_Status == ERROR))
   {
      dig1_num = DATA_E;
      dig2_num = DATA_4;
      dig2_num &= ~BIT_P;
      return;
   }
   else if ((G_High_T_P5_Error_Status == ERROR) || (G_High_T_P3_Error_Status = ERROR))
   {
      dig1_num = DATA_E;
      dig2_num = DATA_8;
      dig2_num &= ~BIT_P;
      return;
   }
   
   //ˮ��������ʾ
   if (G_Sys_Err.Water_Full)
   {
      if (S_Flash_mS500)
      {
         LED_water;
      }
      // dig1_num = DATA_F;
      // dig2_num = DATA_U;
      return;
   }

   //��ʾwifi�ź�ǿ��
   // if((wifi_test_delay_time) && (M_wifi_rssi_dsp>=10))
   if (Wifi_Rssi_DSP_delay_time > 0)
   {
      wifi_rssi_Dsp();
      return;
   }
   // wifi״̬��ʾ
   if (WiFi_LED_Locate_buf)
   {
      LED_WIFI;
   }
   ////////////////////////////////

   if ((G_Time_Setting_Time > 0) || (G_Time_Run > 0)) //��ʱָʾ��
   {
      LED_timer;
   }

   //�����������ʾ
   if (G_Disp_SA_Time > 0)
   {
      dig1_num = DATA_S; // DATA_H;  20131122
      dig2_num = DATA_A; // DATA_1;
   }
   else if (G_Set_SYS_Hum_Time > 0) //�趨ʪ����ʾ
   {
      if (S_Flash_mS500)
      {
         if (G_SYS_Hum_Set_Buf == 25)
         {
            dig1_num = DATA_C; // DATA_H;  20131122
            dig2_num = DATA_o; // DATA_1;
         }
         else
         {
            dig1_2_dsp(G_SYS_Hum_Set_Buf);
         }
      }
   }
   else if (G_Time_Setting_Time > 0) //��ʱ��ʾ
   {
      if (S_Flash_mS500)
      {
         dig1_2_dsp(G_Time_Buf);
      }
   }
   else if ((G_SYS_Power_Status == OFF) && (G_Time_Run))
   {
      dig1_2_dsp(G_Time_Buf);
   }
   else if (G_SYS_Power_Status) //����ʪ����ʾ
   {
      if (G_SYS_Mode_Buf == mode_SYS_HUM)
      {
         hum_dsp_com = GET_ROOM_HUM();
         Disp_Hum_Value(hum_dsp_com);
      }
      else if (G_SYS_Mode_Buf == mode_DRY)
      {
         dig1_num = DATA_d;
         dig2_num = DATA_y;
      }
   }

   if (G_SYS_Power_Status == OFF)
   {
      //����ʱ, ���趨��ʱ����, �����ú���ʾ���ֹ���
      if (G_Set_SYS_Fan_Tyde_Time > 0 || G_Set_SYS_Mode_Time > 0)
      {
         FAN_Speed_Disp(G_SYS_Fan_Tyde_Buf);

         if (G_SYS_Mode_Buf==mode_SYS_HUM)
         {
            LED_HUM_MODE;
         }
         else if(G_SYS_Mode_Buf== mode_DRY)
         {
            LED_DYR_MODE;
         }
      }
      return;
   }
/*
   if (G_Pump_Status == ENABLE) //ˮ�ܽ���ָʾ��
   {
      io_status = GET_PUMP_WATER_PIPE_STATUS();
      if (io_status == DI_CUT)
      {
         if (S_Flash_mS500)
         {
            LED_PUMP;
         }
      }
      else
      {
         LED_PUMP;
      }
   }
 */
   if (G_Filter_Status == ENABLE ) //������ϴʱ��
   {
      if (S_Flash_mS500)
      {
         LED_PUMP;
      }
   }

   if (G_Def_Reg.Defrost_status) //��˪ָʾ��
   {
      LED_def;
   }
   else if (G_SYS_Fast_Test) //���ָʾ��
   {
      if (S_Flash_mS500)
      {
         LED_def;
      }
   }

   switch (G_SYS_Mode_Buf)
   {
   case mode_SYS_HUM:
   {
      LED_HUM_MODE;
   }
   break;

   case mode_DRY:
   {
      LED_DYR_MODE;
   }
   break;

   default:
   {
   }
   break;
   };

   if (G_Set_SYS_Fan_Tyde_Time > 0)
   {
      FAN_Speed_Disp(G_SYS_Fan_Tyde_Buf);
   }
   else
   {
      FAN_Speed_Disp(G_Fan_Tyde_Out);
   }
}

// *****************************************************************************
// �������� : Get_LED_data
// ����˵�� : ����LED���������ʾֵ
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
void Get_LED_data(void)
{

   Write_Disp_LED_Data(((UI16)S_LED_Data_Buf[1].byte << 8) | S_LED_Data_Buf[0].byte); //���LED��ʾֵ

   Write_CT1642_Disp_Data(S_CT1642_Display_Data_Buf, 2); //���2λ�������ʾֵ
}

// *****************************************************************************
// �������� : LED_Display
// ����˵�� : ��ʾ����
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
void LED_Display(void)
{
   CT1642_Disp_Driver(); //����CT1642�����������������ʾ

   LED_mS10_Deal();
   Prg_mS250_DSP();
   Prg_mS500_DSP();
   Prg_S_DSP();

   if (G_SYS_Self_Test)
   {
      LedDsp_Test();
   }
   else
   {
      LED_Dsp_Content();
   }

   Get_LED_data(); //�������ܣ�LED��ʾֵ
}
