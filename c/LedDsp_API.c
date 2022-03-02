#include "General.h"

// GCE_XDATA RGB_LED_TYPE AirQ_Class_LED_buf = 0;

GCE_XDATA UI08 G_Disp_Machine_Temp_Time = 0; //进入快测时，显示室温管温的时间

GCE_XDATA UUI08 S_LED_Data_Buf[COM_total] = {0}; // LED显示data

GCE_XDATA UI08 S_CT1642_Display_Data_Buf[4] = {0xFF, 0xFF, 0X00, 0X00}; // 数码管显示数据暂存

GCE_XDATA TE_Flag_Status S_Flash_mS500 = SET; // 500ms闪烁

GCE_XDATA UI16 S_Comp_Test_Disp_En_Time = 0; //进入压缩机强制测试模式时 全显示时间

GCE_XDATA UI08 S_Test_cont1 = 0; //自检计数

/******************************************************************************
 *         数码管显示数据定义
 *******************************************************************************/
GCE_CONST UI08 BCD_tab[] = /*LED显示编码，用于查表*/
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
// 函数名称 : LED_mS10_Deal
// 功能说明 : 10mS时间片
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
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
// 函数名称 : Prg_mS250_DSP
// 功能说明 : 250mS时间片
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
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
// 函数名称 : Prg_mS500_DSP
// 功能说明 : 500mS时间片
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
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
// 函数名称 : Prg_S_DSP
// 功能说明 : S时间片
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
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
// 函数名称 : Dev_Get_Key_Number
// 功能说明 : 按键识别
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
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
// 函数名称 : Disp_Hum_Value
// 功能说明 : 湿度显示
// 入口参数 : _data_buf  湿度显示值
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
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
// 函数名称 : Clear_Display_Data_Buf
// 功能说明 : 清除所有显示缓存
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
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
// 函数名称 : LedDsp_Test
// 功能说明 : LED产检显示
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : Aysi-E
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
void LedDsp_Test(void)
{
   static GCE_XDATA UI08 test_key_data = 0; 
   static GCE_XDATA UI08 test_seq = 0;    
   GCE_XDATA UI08 key_num = 0;

   key_num = Get_Key_Data();
   Clear_Key_Data;
   if (test_seq == 0) //显示检测
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

   if (test_seq == 2) //结果显示
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

   switch (key_num) //按键检测
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

   if (test_key_data == 0x7f) //检测完成
   {
      test_seq = 2; //跳转至结果显示
   }

   if (key_num)
   {
      G_Buzz_Time = BUZZ_short_time;
   }
}

// *****************************************************************************
// 函数名称 : Disp_All
// 功能说明 : LED/数码管全显
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
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
// 函数名称 : Disp_Temp
// 功能说明 : 温度显示
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
void Disp_Temp(UI08 _temp_buf)
{
   if (_temp_buf >= 15) // 15等于0℃
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
      dig1_num = DATA_neg; //显示温度负号
   }
}

// *****************************************************************************
// 函数名称 : Disp_Machine_Temp
// 功能说明 : 机台温度显示
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
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
// 函数名称 : FAN_Speed_Disp
// 功能说明 : 风速显示
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
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
// 函数名称 : LedDsp_Content
// 功能说明 : 显示逻辑
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
static void LED_Dsp_Content(void)
{
   UI08 hum_dsp_com = 60; //显示湿度
   // SHORTCUT_STATUS io_status;

   //清除所有显示数据
   Clear_Display_Data_Buf();

   //
   if (G_Power_Delay_Time > 0)
   {
      Disp_All();
      return;
   }

   //进入快测显示温度
   if (G_Disp_Machine_Temp_Time > 0)
   {
      Disp_Machine_Temp();
      return;
   }

   //进入压缩机强制测试全显
   if (S_Comp_Test_Disp_En_Time > 0)
   {
      Disp_All();
      return;
   }

   //故障显示
   //数码管显示故障代码
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
   
   //水满报警显示
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

   //显示wifi信号强度
   // if((wifi_test_delay_time) && (M_wifi_rssi_dsp>=10))
   if (Wifi_Rssi_DSP_delay_time > 0)
   {
      wifi_rssi_Dsp();
      return;
   }
   // wifi状态显示
   if (WiFi_LED_Locate_buf)
   {
      LED_WIFI;
   }
   ////////////////////////////////

   if ((G_Time_Setting_Time > 0) || (G_Time_Run > 0)) //定时指示灯
   {
      LED_timer;
   }

   //正常数码管显示
   if (G_Disp_SA_Time > 0)
   {
      dig1_num = DATA_S; // DATA_H;  20131122
      dig2_num = DATA_A; // DATA_1;
   }
   else if (G_Set_SYS_Hum_Time > 0) //设定湿度显示
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
   else if (G_Time_Setting_Time > 0) //定时显示
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
   else if (G_SYS_Power_Status) //房间湿度显示
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
      //待机时, 若设定定时开机, 可设置和显示部分功能
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
   if (G_Pump_Status == ENABLE) //水管接入指示灯
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
   if (G_Filter_Status == ENABLE ) //滤网清洗时间
   {
      if (S_Flash_mS500)
      {
         LED_PUMP;
      }
   }

   if (G_Def_Reg.Defrost_status) //除霜指示灯
   {
      LED_def;
   }
   else if (G_SYS_Fast_Test) //快测指示灯
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
// 函数名称 : Get_LED_data
// 功能说明 : 更新LED、数码管显示值
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
void Get_LED_data(void)
{

   Write_Disp_LED_Data(((UI16)S_LED_Data_Buf[1].byte << 8) | S_LED_Data_Buf[0].byte); //填充LED显示值

   Write_CT1642_Disp_Data(S_CT1642_Display_Data_Buf, 2); //填充2位数码管显示值
}

// *****************************************************************************
// 函数名称 : LED_Display
// 功能说明 : 显示功能
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
void LED_Display(void)
{
   CT1642_Disp_Driver(); //调用CT1642驱动程序，让数码管显示

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

   Get_LED_data(); //填充数码管，LED显示值
}
