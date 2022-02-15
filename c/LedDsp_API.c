#include "General.h"

// GCE_XDATA RGB_LED_TYPE AirQ_Class_LED_buf = 0;

GCE_XDATA UI08 G_Disp_Machine_Temp_Time = 0; //������ʱ����ʾ���¹��µ�ʱ��

GCE_XDATA UUI08 S_LED_Data_Buf[COM_total] = {0}; // LED��ʾdata

GCE_XDATA UI08 S_CT1642_Display_Data_Buf[4] = {0xFF, 0xFF, 0X00, 0X00}; // �������ʾ�����ݴ�

GCE_XDATA TE_Flag_Status S_Flash_mS500 = SET; // 500ms��˸

GCE_XDATA UI16 S_Comp_Test_Disp_En_Time = 0; //����ѹ����ǿ�Ʋ���ģʽʱ ȫ��ʾʱ��

GCE_XDATA UI08 S_Test_Key_Data = 0; //�Լ�data

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
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
void LedDsp_Test(void)
{
#if 0
    if(M_test_seq==0)
    {
         switch(M_test_cont1)
         {
	    case 0: clear_all();break;
            case 1: dig1_num|=BIT_A;break;
            case 2: dig1_num|=BIT_B;break;
            case 3: dig1_num|=BIT_C;break;
            case 4: dig1_num|=BIT_D;break;
            case 5: dig1_num|=BIT_E;break;
            case 6: dig1_num|=BIT_F;break;
            case 7: dig1_num|=BIT_G;break;
	    case 8: ;break;
	    case 9: ;break;
	    case 10: ;break;
	    case 11: ;break;
	    case 12: ;break;
	    case 13: ;break;
	    case 14: ;break;
	    case 15: ;break;
	    default:
	    {
              ;
	    }break;
         }
         dig2_num=dig1_num;
         //
         switch(M_test_cont2)
         {
	    case 0: {;}break;
            case 1: {LED_WIFI;}break;
            case 2: {LED_CON;}break;
            case 3: {LED_HUM;}break;
            case 4: {LED_DYR;}break;
            case 5: {LED_timer;}break;
            case 6: {LED_PUMP;}break;
            case 7: {LED_def;}break;
	    case 8: {LED_water;}break;
            case 9:
	    {
	       dutyindex[0]=8; //��ɫ
               dutyindex[1]=100; //��ɫ
               dutyindex[2]=0;  //��ɫ
	    }break;
	    case 10:
	    {
	       dutyindex[0]=8; //��ɫ
               dutyindex[1]=100; //��ɫ
               dutyindex[2]=0;  //��ɫ
	    }break;
	    case 11:
	    {
	       dutyindex[0]=8; //��ɫ
               dutyindex[1]=100; //��ɫ
               dutyindex[2]=0;  //��ɫ
	    }break;
            case 12:
	    {
	      dutyindex[0]=100; //��ɫ
              dutyindex[1]=0; //��ɫ
              dutyindex[2]=0;  //��ɫ
	    }break;
	    case 13:
	    {
	      dutyindex[0]=100; //��ɫ
              dutyindex[1]=0; //��ɫ
              dutyindex[2]=0;  //��ɫ
	    }break;
	    case 14:
	    {
	      dutyindex[0]=100; //��ɫ
              dutyindex[1]=0; //��ɫ
              dutyindex[2]=0;  //��ɫ
	    }break;
	    case 15:
	    {
	      dutyindex[0]=0;
              dutyindex[1]=0;
              dutyindex[2]=100;
	    }break;
            case 16:
	    {
	      dutyindex[0]=0;
              dutyindex[1]=0;
              dutyindex[2]=100;
	    }break;
	    case 17:
	    {
	      dutyindex[0]=0;
              dutyindex[1]=0;
              dutyindex[2]=100;
	    }break;
	    default:
	    {
	       dutyindex[0]=0;
               dutyindex[1]=0;
               dutyindex[2]=0;
	       M_test_cont2=0;
	       M_test_cont1=0;
	    }break;
        }
    }
    if(M_test_seq==1)
    {

    }
    //
    if(M_test_seq==2)
    {

		if(Sys_Err.comm_err==ENABLE)
		{
			dig1_num=DATA_E;
			dig2_num=DATA_5;
		}
                else if(_Self_Test_wifi_err)
                {
			dig1_num=DATA_E;
			dig2_num=DATA_6;
		}
		else
		{
				dig1_num=DATA_r;
				dig2_num=BCD_tab[Soft_Version];
	        }
   }
   //

    switch(M_Key_Number)
	{
		case power_key:	        S_Test_Key_Data|=bit0;break;
		case set_hum_key:	        S_Test_Key_Data|=bit1;break;
	        case set_timer_key:         S_Test_Key_Data|=bit2;break;
	        case dry_key:	        S_Test_Key_Data|=bit3;break;
	        case LAMP_key :	        S_Test_Key_Data|=bit4;break;
		case swing_mode_key:	        S_Test_Key_Data|=bit5;break;
		case hum_mode_key:   	S_Test_Key_Data|=bit6;break;
         default:break;
	}
	if(M_Key_Number)
	{M_Buzz_Time=BUZZ_short_time;}
	M_Key_Number=0;
        //
	if(S_Test_Key_Data==0x3f)
	{M_test_seq=1;}
	else if(S_Test_Key_Data==0x7f)
	{M_test_seq=2;}
#endif
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
   SHORTCUT_STATUS io_status;
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
   if (G_Sys_Err.Water_Full) //ˮ��������ʾ
   {
      if (S_Flash_mS500)
      {
         LED_water;
      }
      dig1_num = DATA_F;
      dig2_num = DATA_U;
      return;
   }

   if (GET_COM_STATUS() == ERROR)
   {
      dig1_num = DATA_E;
      dig2_num = DATA_5;
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
      dig1_num = DATA_5; // DATA_H;  20131122
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
   //
   //�������ʾ���ϴ���
   if (G_Sys_Err.temp_room_err)
   {
      dig1_num = DATA_E;
      dig2_num = DATA_1;

      dig2_num &= ~BIT_P;
   }
   else if (G_Sys_Err.temp_coil_err)
   {
      dig1_num = DATA_E;
      dig2_num = DATA_2;

      dig2_num &= ~BIT_P;
   }
   else if (G_Sys_Err.hum_Sensor_err)
   {
      dig1_num = DATA_E;
      dig2_num = DATA_3;

      dig2_num &= ~BIT_P;
   }
   else if (G_Sys_Err.temp_comp_err)
   {
      dig1_num = DATA_E;
      dig2_num = DATA_6;

      dig2_num &= ~BIT_P;
   }
   else if ((G_High_T_P4_Error_Status = ERROR) || (G_Turn_On_H_T_Error_Status == ERROR))
   {
      dig1_num = DATA_E;
      dig2_num = DATA_4;
      dig2_num &= ~BIT_P;
   }
   else if ((G_High_T_P5_Error_Status == ERROR) || (G_High_T_P3_Error_Status = ERROR))
   {
      dig1_num = DATA_E;
      dig2_num = DATA_8;
      dig2_num &= ~BIT_P;
   }

   //

   if (G_SYS_Power_Status == OFF)
   {
      return;
   }

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
      if (G_Set_SYS_Fan_Tyde_Time > 0)
      {
         FAN_Speed_Disp(G_SYS_Fan_Tyde_Buf);
      }
      else
      {
         FAN_Speed_Disp(G_Fan_Tyde_Out);
      }
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
