#include "General.h"

//防冷媒泄漏变量
GCE_XDATA TE_Bool S_EC1_Err = FALSE;   // EC故障
GCE_XDATA UI08 S_EC1_Cycle_Count = 0;  // EC计时变量
GCE_XDATA UI08 S_EC1_Step = 0;         // EC计时变量
GCE_XDATA UI16 S_EC1_Count_Time = 0;   // EC计时变量
GCE_XDATA UI16 S_EC1_COMP_ON_Time = 0; //
//
GCE_XDATA TE_Bool S_EC2_Err = FALSE;   // EC故障
GCE_XDATA UI08 S_EC2_Step = 0;         // EC计时变量
GCE_XDATA UI16 S_EC2_Count_Time = 0;   // EC计时变量
GCE_XDATA UI16 S_EC2_Comp_ON_Time = 0; //
//
GCE_XDATA TS_Pump_Reg S_Pump_Type; // PUMP变量

GCE_XDATA TU_Def_Reg G_Def_Reg; //除霜变量

GCE_XDATA UI16 G_Fan_Force_Run_Time = 180; //每次按键或者定时开机，或者水满回复风机强制运行时间

GCE_XDATA UI08 S_Filter_Minute_Count = 0; //滤网计时
GCE_XDATA UI16 G_Filter_Run_Time = 0;     //滤网时间, 单位:分钟
GCE_XDATA UI16 G_Filter_Run_Time_Buf = 0; //滤网时间, 单位:分钟
GCE_XDATA TE_FuncState  G_Filter_Status = DISABLE;

GCE_XDATA TU_Step S_High_T_P2_Step = Step0;

GCE_XDATA UI16 S_High_T_P3_Cont = 0;       //高温保护逻辑三计时变量
GCE_XDATA UI08 S_High_T_P3_Cycle_Cont = 0; //高温保护逻辑三计时变量
GCE_XDATA TE_ErrorStatus G_High_T_P3_Error_Status = SUCCESS;

GCE_XDATA TE_ErrorStatus G_Turn_On_H_T_Error_Status = SUCCESS; //开机时，室温≥40℃ 异常标志
GCE_XDATA UI16 S_High_T_P4_Cont1 = 0;                        //高温保护逻辑4计时变量
GCE_XDATA UI16 S_High_T_P4_Cont2 = 0;                        //高温保护逻辑4计时变量
GCE_XDATA TE_ErrorStatus G_High_T_P4_Error_Status = SUCCESS;

GCE_XDATA UI16 S_High_T_P5_Cont1 = 0;      //高温保护逻辑5计时变量
GCE_XDATA UI08 S_High_T_P5_Cycle_Cont = 0; //高温保护逻辑5计时变量
GCE_XDATA UI08 S_High_T_P5_LT = 0;         //高温保护逻辑5计时变量
GCE_XDATA UI08 S_High_T_P5_HT = 0;         //高温保护逻辑5计时变量

GCE_XDATA TE_ErrorStatus G_High_T_P5_Error_Status = SUCCESS;

//压缩机强制测试功能变量
GCE_XDATA TE_FuncState G_Comp_Test_EN = DISABLE;
GCE_XDATA UI16 S_Comp_Test_Time = 0; //压缩机测试时间

#define COMP_TEST_OFF_TIME 60 * 3 // 3分钟关
#define COMP_TEST_ON_TIME 60 * 6  // 6分钟开
//

GCE_XDATA TS_DO_Para_Def G_Comp_Para; //压缩机
GCE_XDATA TS_DO_Para_Def G_Pump_Para; //水泵

GCE_XDATA TU_FAN_Speed_Type G_Fan_Tyde_Out = OFF_FAN;     //实际运行风速
GCE_XDATA TU_FAN_Speed_Type G_Fan_Tyde_Out_Buf = OFF_FAN; //实际运行风速
GCE_XDATA UI16 S_FAN_ON_Time = 0;
GCE_XDATA UI16 S_FAN_OFF_Time = 0;

GCE_XDATA TE_FuncState Comp_SA_EN = DISABLE; // SA保护标志

void Pump_S_General(void);

// *****************************************************************************
// 函数名称 : Control_data_init
// 功能说明 : 系统逻辑数据初始化
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
   //需要首次上电压缩机运行X分钟后才判断除霜逻辑
   G_Def_Reg.def_Comp_mode = POWER_ON_STOP;

   //不需要首次上电压缩机运行X分钟后才判断除霜逻辑
   // G_Def_Reg.def_Comp_mode = NO_FIRST_ON ;
}

// *****************************************************************************
// 函数名称 : Filter_Time_Logic
// 功能说明 : 滤网计时逻辑
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
   //分钟
   //快测时，小时变秒
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
   if (G_Fan_Tyde_Out != OFF_FAN) //风机开启，滤网开始计时
   {
      G_Filter_Run_Time = UI16_Addition_Operation(G_Filter_Run_Time, num);
      //
      if (G_Filter_Run_Time % 60 == 0) //每一小时记录一次滤网时间
      {
         G_Filter_Run_Time_Buf = G_Filter_Run_Time;
      }
   }
}

// *****************************************************************************
// 函数名称 : Prg_s_Control
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
// 函数名称 : EC_Protect_Load_Logic
// 功能说明 : 冷媒保护，负载控制逻辑
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
// 函数名称 : Ec_Protect1_Logic
// 功能说明 : 冷媒保护条件1逻辑
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
static void Ec_Protect1_Logic(void)
{
   UI08 g_comp_room_dt = 0; //压缩机与室温温差
   UI08 g_room_coil_dt = 0; //室温与管温温差
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

      if (S_EC1_COMP_ON_Time >= (15UL * 60)) //压缩机启动达15分钟
      {
         S_EC1_Step = 2;
         S_EC1_Count_Time = 0;
      }
   }
   break;

   case 1:
   {

      if (S_EC1_COMP_ON_Time >= (10UL * 60)) //压缩机启动达15分钟
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

      if ((g_comp_room_dt > 50)    //压缩机温度大于室温50度
          && (g_room_coil_dt > 18) //室温大于管温18度
      )
      {
         S_EC1_Step = 3;
         S_EC1_Count_Time = 0;
      }

      if ((g_comp_room_dt < 50)    //压缩机温度小于室温50度
          && (g_room_coil_dt > 10) //室温大于管温18度
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
// 函数名称 : Ec_Protect2_Logic
// 功能说明 : 冷媒保护条件2逻辑
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
/* static void Ec_Protect2_Logic(void)
{
   UI08 g_dt = 0; //计算温差
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
      if (S_EC2_Comp_ON_Time >= 60UL * 5) //压缩机启动未达5分钟
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
// 函数名称 : Ec_Protect_Deal
// 功能说明 : 冷媒保护总入口
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
static void Ec_Protect_Deal(void)
{
   Ec_Protect1_Logic();
   // Ec_Protect2_Logic();
}

// *****************************************************************************
// 函数名称 : Defost_Enb
// 功能说明 : 除霜标志使能，及循环归零确认
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
// 函数名称 : Def_Start_Jude
// 功能说明 : 进入除霜逻辑的处理
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
static void Def_Start_Jude(TE_Bool _condition1, TE_Bool _condition2)
{
   // condition1 进入30分钟计时条件
   // condition2 30分钟计时中途清零条件
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
   if (G_Def_Reg.def_start_time <= buf) //开始计时前2秒，读取当前室温，后续除霜行程一个循环内都使用这个室温
   {

      G_Def_Reg.temp_room_point = GET_ROOM_TEMP_C();
   }

   if ((G_Def_Reg.temp_room_point < defrost_para7) //室温小于10℃
       || (G_Sys_Err.temp_room_err)                //室温故障
       || (G_Sys_Err.temp_coil_err)                //管温故障
       || (G_Sys_Err.hum_Sensor_err)               //湿度传感器故障
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
   //在计时行程中，于第10分钟和第24分钟分别检测铜管SENSOR感应到的温度
   if ((G_Def_Reg.def_start_time == check_time1) || (G_Def_Reg.def_start_time == check_time2))
   {
      if (!_condition1)
      {
         G_Def_Reg.def_start_count_enable = FALSE;
         G_Def_Reg.def_start_time = 0; //计时清零     假如是计时停止，则把此语句屏蔽
      }
   }
   //
   end_check_time = check_time2 + 60; // 30分钟

   if (G_Def_Reg.def_start_time >= end_check_time)
   {
      Defost_Enb();
   }
}

// *****************************************************************************
// 函数名称 : Defrost_Logic
// 功能说明 : 除霜逻辑
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
static void Defrost_Logic(void)
{
   UI16 def_end_time;
   UI16 timer_buf = 0;
   UI08 cycle_index = 0;
   TE_Bool condition1;
   TE_Bool condition2;
   UI16 coil_ad = 0;

   //记录是否压缩机第一次启动
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
   if (G_Def_Reg.Defrost_status == FALSE) //上电后第一次压缩机启动10分钟内不进行除霜判断
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
      //除霜开始条件判断
      if ((G_Sys_Err.temp_coil_err) || (G_Sys_Err.temp_comp_err)) //管温故障
      {
         condition1 = (TE_Bool)(G_Comp_Para.OUT);        //铜管故障压缩机开启时直接进入除霜行程2013-3-7
                                                         // &&(Temp_room.value<defrost_para12);//Troom<15
         condition2 = (TE_Bool)(G_Comp_Para.OUT == OFF); // Troom>=16
      }
      else //无故障除霜
      {
         coil_ad = GET_COIL_TEMP_AD();
         condition1 = (TE_Bool)((G_Comp_Para.on_time >= timer_buf) && (coil_ad <= defrost_para2)); // coil<0?
         condition2 = (TE_Bool)((coil_ad >= defrost_para3) || (G_Comp_Para.OUT == OFF));           // coil>=20
      }
      Def_Start_Jude(condition1, condition2);
   }
   else
   {
      //除霜动作
      G_Comp_Para.BUF = OFF;
      G_Fan_Tyde_Out_Buf = HIGH_FAN;
      G_Set_Fan_Tyde_EN = DISABLE;

      //除霜结束条件判断
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
// 函数名称 : HiPriority_Protect_Deal
// 功能说明 : 满水提示/通信故障 处理
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
         G_Fan_Force_Run_Time = 180; //解除水满报警，风机强制开启180秒
      }
      G_Sys_Err.Water_Full = DISABLE;
      G_Buzz_Cnt = 0;
   }

   //故障保护负载输出
   if (((GET_COM_STATUS() == ERROR)) || (G_Sys_Err.Water_Full))
   {
      G_Pump_Para.BUF = OFF;
      G_Comp_Para.BUF = OFF;
      G_Fan_Tyde_Out_Buf = OFF_FAN;

      G_Fan_Force_Run_Time = 0;
   }
}

// *****************************************************************************
// 函数名称 : Sys_Sensor_Err_Deal
// 功能说明 : 系统传感器异常处理
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
static void Sys_Sensor_Err_Deal(void)
{
   TS_AISTATUS status_buf;

   status_buf = GET_ROOM_TEMP_SENSOR_STATUS();
   if (status_buf != AI_NORMAL) //开短路
   {
      G_Sys_Err.temp_room_err = ENABLE;
   }
   else
   {
      G_Sys_Err.temp_room_err = DISABLE;
   }

   status_buf = GET_COIL_TEMP_SENSOR_STATUS();
   if (status_buf != AI_NORMAL) //开短路
   {
      G_Sys_Err.temp_coil_err = ENABLE;
   }
   else
   {
      G_Sys_Err.temp_coil_err = DISABLE;
   }

   status_buf = GET_Comp_TEMP_SENSOR_STATUS();
   if (status_buf != AI_NORMAL) //开短路
   {
      G_Sys_Err.temp_comp_err = ENABLE;
   }
   else
   {
      G_Sys_Err.temp_comp_err = DISABLE;
   }

   status_buf = GET_ROOM_HUM_SENSOR_STATUS();
   if (status_buf != AI_NORMAL) //开短路
   {
      G_Sys_Err.hum_Sensor_err = ENABLE;
   }
   else
   {
      G_Sys_Err.hum_Sensor_err = DISABLE;
   }

   if ((G_SYS_Power_Status) && ((G_SYS_Mode == mode_DRY) || (G_SYS_Mode == mode_SYS_HUM))) //需要压缩机开启的模式
   {
      if ((G_Sys_Err.temp_room_err)     //室温故障
          || (G_Sys_Err.temp_coil_err)  //管温故障
          || (G_Sys_Err.hum_Sensor_err) //湿度故障
          || (G_Sys_Err.temp_comp_err)  //吐出管温故障
      )
      {
         G_Comp_Para.BUF = OFF;
         G_Fan_Tyde_Out_Buf = OFF_FAN;
      }
   }
}

// *****************************************************************************
// 函数名称 : Comp_OverLoad
// 功能说明 : 压缩机过载保护
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
static void Comp_OverLoad(void)
{
   if (G_Comp_Overtime_Protect_Flag)
   {
      if (G_Comp_Para.on_time >= 3600UL * 12) //连续运行12小时变待机
      {
         Set_Power_Status();
         G_Fan_Force_Run_Time = 0;
      }
   }
}

// *****************************************************************************
// 函数名称 : High_Temperature_Protection2
// 功能说明 : 高温保护逻辑2
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
      G_Comp_Para.BUF = OFF; //注意:如果压缩机有强制运转3分钟时，也要强制关闭
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
// 函数名称 : High_Temperature_Protection1
// 功能说明 : 高温保护逻辑1
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
      if ((temp_comp_t > (85 + 15)) // 85摄氏度
                                    //&&(G_Comp_Para.OUT == ON)
      )
      {
         s_high_T_P3_step = Step1;
      }

      if ((temp_comp_t > (90 + 15)) // 90摄氏度
          && (G_Comp_Para.OUT == ON))
      {
         s_high_T_P3_step = Step2;
      }
   }
   break;

   case Step1:
   {
      G_Fan_Tyde_Out_Buf = HIGH_FAN;

      if ((temp_comp_t > (90 + 15)) // 90摄氏度
          && (G_Comp_Para.OUT == ON))
      {
         s_high_T_P3_step = Step2;
      }

      if (temp_comp_t <= (84 + 15)) // 84摄氏度
      {
         s_high_T_P3_step = Step0;
      }
   }
   break;

   case Step2:
   {
      G_Comp_Para.BUF = OFF;
      G_Fan_Tyde_Out_Buf = HIGH_FAN;

      if (temp_comp_t <= (45 + 15)) // 45摄氏度
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

      if (S_High_T_P3_Cont >= 180) //至少180秒
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

//開機時當環境溫度偵測到≧40°C時機台不啟動運轉，指示燈顯示E4。
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
// 函数名称 : High_Temperature_Protection4
// 功能说明 : 高温保护逻辑4
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

      if (S_High_T_P4_Cont1 >= (5UL * 60)) //至少180秒
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
// 函数名称 : High_Temperature_Protection5
// 功能说明 : 高温保护逻辑4
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
   case Step0: //初始
   {
      S_High_T_P5_LT = temp_buf;
      S_High_T_P5_HT = temp_buf;
      s_high_T_P5_step = 1;
      S_High_T_P5_Cont1 = 0;
      S_High_T_P5_Cycle_Cont = 0;
   }
   break;

   case Step1: //上升
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

   case Step2: //下降
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
// 函数名称 : High_T_Protect_Deal
// 功能说明 : 高温保护逻辑总调用
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
static void High_T_Protect_Deal(void)
{
   High_Temperature_Protection1(); //  4-1.環境高溫保護:  32℃
   High_Temperature_Protection2(); //  4-1.環境高溫保護:  42℃
   High_Temperature_Protection3(); // 2 . 壓縮機異常溫度保護  2.2
   High_Temperature_Protection4(); // 2 . 壓縮機異常溫度保護  2-3
   High_Temperature_Protection5(); // 2 . 壓縮機異常溫度保護  2-5
}

// *****************************************************************************
// 函数名称 : Protect_Logic
// 功能说明 : 保护逻辑总调用
// 入口参数 :
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
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
// 函数名称 : Comp_Safe_Logic
// 功能说明 : 压缩机耐久性保护功能
// 入口参数 :
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
static void Comp_Safe_Logic(void)
{
   if (G_Comp_Para.BUF == ON)
   {
      if ((G_Comp_Para.off_time < COMP_PROTECT_OFF_TIME) && (G_Comp_Para.OUT == OFF)) //压缩机停机保护时间未到 且压缩机未运转
      {
         G_Comp_Para.OUT = OFF;
      }
      else
      {
         G_Comp_Para.OUT = ON;
         G_Comp_Para.off_time = 0; //避免快速按开关机键，压缩机可以不断开或关
      }
   }
   else
   {
      if ((G_SYS_Power_Status == OFF)                     //机台关机
          || (G_Sys_Err.Water_Full)                       //机台发生水满
          || (G_Comp_Para.OUT == OFF)                     //压缩机原本为关
          || (S_High_T_P2_Step == Step1)                  //高温保护逻辑2，室温大于42度，强制关闭压缩机
          || (G_Comp_Para.on_time > COMP_PROTECT_ON_TIME) //压缩最小运行时间到
          || (G_Sys_Err.temp_room_err)                    //室温故障
          || (G_Sys_Err.temp_coil_err)                    //管温故障
          || (G_Sys_Err.hum_Sensor_err)                   //湿度故障
          || (G_Sys_Err.temp_comp_err)                    //吐出管温故障
      )
      {
         G_Comp_Para.OUT = OFF;
         G_Comp_Para.on_time = 0; //避免快速按开关机键，压缩机可以不断开或关
      }
      else
      {
         G_Comp_Para.OUT = ON;
      }
   }
}

// *****************************************************************************
// 函数名称 : Load_Set
// 功能说明 : 负载状态最后确认 buf 输出给 out
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 :
// 审核人员 :
// 审核日期 :
// 修改记录 : 2021.5.13  V1.0首次发布
// *****************************************************************************
static void Load_Set(void)
{
   Comp_Safe_Logic();
   //-----------------------------------------------水泵控制
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
   if (G_Fan_Tyde_Out == OFF_FAN) //风机关闭时，快速清零风机运行时间
   {
      S_FAN_ON_Time = 0;
   }
}

// *****************************************************************************
// 函数名称 : Pump_S_General
// 功能说明 : 水泵逻辑的秒计时处理
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 :
// 审核人员 :
// 审核日期 :
// 修改记录 : 2021.5.13  V1.0首次发布
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
// 函数名称 : Pump_Control
// 功能说明 : 水泵逻辑
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 :
// 审核人员 :
// 审核日期 :
// 修改记录 : 2021.5.13  V1.0首次发布
// *****************************************************************************
/*
static void Pump_Control(void)
{
   SHORTCUT_STATUS io_status;

   io_status = GET_PUMP_WATER_PIPE_STATUS(); //水管接入状态
   if ((G_Pump_Status == DISABLE) || (io_status == DI_CUT) || (S_Pump_Type.off_delay >= 3))
   {
      G_Pump_Para.BUF = OFF;
      //计时重新开始
      S_Pump_Type.step = Step0;
      S_Pump_Type.on_count = 0;
      return;
   }
   //
   switch (S_Pump_Type.step)
   {
   case Step0:
   {
      io_status = GET_PUMP_FULL_IO_STATUS(); //副水箱开关状态
      if (io_status == DI_SHORT)             //客户要求水泵不随开关机动作
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
// 函数名称 : HUM_Mode_Logic
// 功能说明 : 除湿模式功能逻辑
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 :
// 审核人员 :
// 审核日期 :
// 修改记录 : 2021.5.13  V1.0首次发布
// *****************************************************************************
static void Hum_Mode_Logic(void)
{
   UI08 hum_buf = 0;
   hum_buf = GET_ROOM_HUM();

   //压缩机
   if (G_SYS_Hum_Set == 25)
   {
      G_Comp_Para.BUF = ON;
   }
   else if (hum_buf >= (G_SYS_Hum_Set + 5)) //当环境湿度>设定湿度5%压缩机启动
   {
      G_Comp_Para.BUF = ON;
   }
   else if ((hum_buf + 5) <= G_SYS_Hum_Set) //当环境湿度<设定湿度5%压缩机停止
   {
      G_Comp_Para.BUF = OFF;
   }

   //风机
   // if ((G_Comp_Para.OUT == ON)        //压缩机运转时
   //     || (G_Fan_Force_Run_Time > 0)) //每次按电源键开机、定时到开机，或者水满报警解除之后风机强制按设定风速运转3分钟
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
// 函数名称 : Dry_Mode_Logic
// 功能说明 : 干衣模式逻辑
// 入口参数 :
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
static void Dry_Mode_Logic(void)
{
   //压缩机
   G_Comp_Para.BUF = ON;

   //风机
   G_Set_Fan_Tyde_EN = DISABLE;
   G_Fan_Tyde_Out_Buf = HIGH_FAN;
}

// *****************************************************************************
// 函数名称 : Comp_Test_General
// 功能说明 : 压缩机强制测试模式逻辑
// 入口参数 :
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
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
// 函数名称 : SYS_Mode_Change_Handle
// 功能说明 : 模式切换调用  确认那些参数需要修改
// 入口参数 : _now_mode   当前系统运行的模式
//           _set_mode    需要切换为新的模式
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：  模式切换后调用次函数
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
// 函数名称 : Sys_Control
// 功能说明 : 系统逻辑、保护处理
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
void Sys_Control(void)
{
   Prg_s_Control();
   //
   if (G_SYS_Self_Test)
   {
      return;
   }
   //
   if (G_SYS_Power_Status) //开机状态
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
   else //关机状态
   {
      G_Fan_Tyde_Out_Buf = OFF_FAN;
      G_Comp_Para.BUF = OFF;
   }

   Comp_Test_General();
   //Pump_Control(); //打水PUMP功能
   Protect_Logic(); //保护逻辑
   Load_Set(); //负载输出
   Filter_Time_Logic();
}
