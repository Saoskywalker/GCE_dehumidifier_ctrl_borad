#include "General.h"

GCE_XDATA TS_SYS_Config G_Sys_Config;

GCE_XDATA UI32 G_Time_Run = 0;          //定时剩余时间
GCE_XDATA UI08 G_Time_Set = 1;          //定时设定时间
GCE_XDATA UI08 G_Time_Buf = 1;          //定时设定时间buf
GCE_XDATA UI08 G_Time_Setting_Time = 0; //定时设定(确认时间)
GCE_XDATA UI08 S_Time_Set_IN = 0;       //是否进行了定时设置

GCE_XDATA UI08 G_Power_Delay_Time = 3; //系统开机延时

GCE_XDATA UI08 G_Test_Step = 0;  //自检
GCE_XDATA UI08 G_Test_Cont1 = 0; //自检
GCE_XDATA UI08 G_Test_Cont2 = 0; //自检

GCE_XDATA TS_SYS_Err G_Sys_Err;                   //故障
GCE_XDATA TE_FuncState G_SYS_Self_Test = DISABLE; //自检标志
GCE_XDATA TE_FuncState G_SYS_Fast_Test = DISABLE; // 快测标志

//
GCE_XDATA ONOFF_STATUS G_SYS_Power_Status = OFF;     // 开关机状态
GCE_XDATA ONOFF_STATUS G_SYS_Power_Status_Old = OFF; // 开关机状态

GCE_XDATA TU_SYS_Mode G_SYS_Mode = mode_SYS_HUM;     // 系统模式
GCE_XDATA TU_SYS_Mode G_SYS_Mode_Buf = mode_SYS_HUM; // 系统模式
GCE_XDATA UI08 S_Set_SYS_Mode_Time = 0;              // 模式设定时间

GCE_XDATA UI08 G_SYS_Hum_Set = 60;     //湿度设定
GCE_XDATA UI08 G_SYS_Hum_Set_Buf = 60; //湿度设定buf
GCE_XDATA UI08 G_Set_SYS_Hum_Time = 0;

GCE_XDATA TU_FAN_Speed_Type G_SYS_Fan_Tyde = LOW_FAN;     //运行风速
GCE_XDATA TU_FAN_Speed_Type G_SYS_Fan_Tyde_Buf = LOW_FAN; //运行风速
GCE_XDATA UI08 G_Set_SYS_Fan_Tyde_Time = 0;               //设置风速时间
GCE_XDATA TE_FuncState G_Set_Fan_Tyde_EN = ENABLE;        //是否允许设定风速

GCE_XDATA UI08 G_M_Sleep_Time = 10; //待机后多久给电源板发送睡眠标志

UI16 G_Buzz_Time = 0;                     //蜂鸣器时间
TE_Pin_Status G_Buzzer_IO_Status = RESET; //蜂鸣器当前IO状态
GCE_XDATA UI08 G_Buzz_Cnt = 0;            //水满报警时，蜂鸣器响次数
GCE_XDATA UI08 Key_ERR_Buzz_Cnt = 0;      //按键无效时，蜂鸣器响的次数

GCE_XDATA TE_FuncState G_Pump_Status = DISABLE; //自动水泵抽水功能是否打开

GCE_XDATA UI08 G_Disp_SA_Time = 0;

void Buf_Confirm_Logic(void);

// *****************************************************************************
// 函数名称 : SYS_Data_Rest
// 功能说明 : 回复出厂设置时，需要复位的参数
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
void SYS_Data_Rest(void)
{
    G_SYS_Fast_Test = DISABLE;

    G_Time_Run = 0;
    G_Time_Set = 1;
    G_Time_Buf = 1;

    G_Pump_Status = DISABLE;

    G_SYS_Mode = mode_SYS_HUM;     //系统模式
    G_SYS_Mode_Buf = mode_SYS_HUM; //系统模式
    S_Set_SYS_Mode_Time = 0;       //模式设定时间

    G_SYS_Hum_Set = 60;     //湿度设定
    G_SYS_Hum_Set_Buf = 60; //湿度设定buf
    G_Set_SYS_Hum_Time = 0; //湿度设定(确认时间)

    G_SYS_Fan_Tyde = LOW_FAN;     //运行风速
    G_SYS_Fan_Tyde_Buf = LOW_FAN; //运行风速
    G_Set_Fan_Tyde_EN = ENABLE;
    G_Set_SYS_Fan_Tyde_Time = 0;

    G_Def_Reg.Defrost_status = DISABLE;
    G_Filter_Run_Time = 0;
    G_Filter_Run_Time_Buf = 0;
}

// *****************************************************************************
// 函数名称 : SYS_Data_Init
// 功能说明 : 变量初始化
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
void SYS_Data_Init(void)
{
    G_Sys_Config.Auto_Restart = ENABLE;

    Adc_Data_Init();
    Control_data_init();
    IO_Data_Init();
    SYS_Data_Rest();
    //
    G_Buzz_Time = BUZZ_long_time;
    G_Buzz_Cnt = 0;

    G_Sys_Err.Water_Full = DISABLE;
    G_Sys_Err.temp_room_err = DISABLE;
    G_Sys_Err.temp_coil_err = DISABLE;
    G_Sys_Err.temp_comp_err = DISABLE;
    G_Sys_Err.hum_Sensor_err = DISABLE;
}

// *****************************************************************************
// 函数名称 : UI16_Addition_Operation
// 功能说明 : 16位加法
// 入口参数 : _Subtraction 被加的值   _minuend加多少值
// 出口参数 : 16位运算结果
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
UI16 UI16_Addition_Operation(UI16 _augend, UI16 _addend)
{
    if (_augend < (0xffff - _addend))
    {
        _augend = _augend + _addend;
    }
    else
    {
        _augend = 0xffff;
    }

    return _augend;
}

// *****************************************************************************
// 函数名称 : UI16_Subtraction_Operation
// 功能说明 : 16位减法
// 入口参数 : _Subtraction 被减的值   _minuend减多少值
// 出口参数 : 16位运算结果
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
UI16 UI16_Subtraction_Operation(UI16 _Subtraction, UI16 _minuend)
{
    if (_Subtraction > _minuend)
    {
        _Subtraction = _Subtraction - _minuend;
    }
    else
    {
        _Subtraction = 0;
    }

    return _Subtraction;
}

// *****************************************************************************
// 函数名称 : Prg_mS100_General
// 功能说明 : 100mS处理函数
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
void Prg_mS100_General(void)
{
    if (!_100mS_For_SYS)
    {
        return;
    }

    Buf_Confirm_Logic();
}

// *****************************************************************************
// 函数名称 : Prg_mS500_General
// 功能说明 : 500mS处理函数
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
void Prg_mS500_General(void)
{

    if (!_500mS_For_SYS)
    {
        return;
    }

    if ((G_SYS_Self_Test) && (G_Power_Delay_Time == 0))
    {
        if (G_Test_Cont1 < 0xff)
        {
            G_Test_Cont1++;
        }

        if ((G_Test_Cont2 < 0xff) && (G_Test_Cont1 >= 8))
        {
            G_Test_Cont2++;
        }
    }
}

// *****************************************************************************
// 函数名称 : Prg_S_General
// 功能说明 : S处理函数
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
void Prg_S_General(void)
{

    if (!_1S_For_For_SYS)
    {
        return;
    }

    if (G_Power_Delay_Time > 0)
    {
        G_Power_Delay_Time--;
    }

    if (G_Buzz_Cnt > 0)
    {
        G_Buzz_Cnt--;
        G_Buzz_Time = BUZZ_long_time;
    }
    else if (Key_ERR_Buzz_Cnt > 0)
    {
        Key_ERR_Buzz_Cnt--;
        G_Buzz_Time = BUZZ_short_time;
    }
    //

    if (G_M_Sleep_Time > 0)
    {
        G_M_Sleep_Time--;
        if (G_M_Sleep_Time == 0)
        {
            _Sleep_status = 1;
        }
    }
}

// *****************************************************************************
// 函数名称 : Turn_Off
// 功能说明 : 触发关机时的变量处理
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
void Turn_On(void)
{
    G_SYS_Power_Status = ON;
    G_Time_Run = 0;
    G_Time_Setting_Time = 0;
    S_Time_Set_IN = 0;
    G_Set_SYS_Hum_Time = 0;
    WIFI_Self_Test_delay_time = 0; //关机状态才可进产测
}

// *****************************************************************************
// 函数名称 : Turn_Off
// 功能说明 : 触发关机时的变量处理
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
void Turn_Off(void)
{
    G_SYS_Power_Status = OFF;
    //
    G_Time_Run = 0;
    G_Time_Setting_Time = 0;
    S_Time_Set_IN = 0;
    G_Set_SYS_Hum_Time = 0;

    G_Comp_Test_EN = DISABLE;
    WIFI_Self_Test_delay_time = WIFI_SELF_TEST_IN_TIMER; //关机状态才可进产测
}

// *****************************************************************************
// 函数名称 : Wdt_Init
// 功能说明 : 看门狗初始化
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
void Wdt_Init(void)
{
    // 1--1 -- 00   ?可Code Option选择ENWDT
    WDTCON = 0x10; //看门狗打开通过烧?时配置字(示波器实测700ms复位)
}

// *****************************************************************************
// 函数名称 : Sys_Initial
// 功能说明 : 系统初始化
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
void Sys_Initial(void)
{
    EA = 0;
    IO_Init();
    // ADC_Init();
    Timer_Init();
    TouchKeyInit();

    SYS_Data_Init();
    UART_Init();
    Wdt_Init();
    EA = 1;
}

// *****************************************************************************
// 函数名称 : Get_Invalid_Key
// 功能说明 : 判断是否可以执行按键功能
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
TE_FuncState Get_Invalid_Key(UI08 key_num)
{
    if (G_Power_Delay_Time > 0) //期间按下无效
    {
        return ENABLE;
    }

    if (key_num == 0x00) //没有按键按下
    {
        return ENABLE;
    }

    if (G_Sys_Err.Water_Full == ENABLE)
    {
        Key_ERR_Buzz_Cnt = 3;
        return ENABLE;
    }

    return DISABLE;
}

// *****************************************************************************
// 函数名称 : Set_Power_Status
// 功能说明 : 设置开关机状态
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
void Set_Power_Status(void)
{
    G_Buzz_Time = BUZZ_long_time;

    if (G_SYS_Fast_Test)
    {
        G_SYS_Fast_Test = DISABLE;
        return;
    }

    if (G_SYS_Power_Status == ON)
    {
        Turn_Off();
    }
    else
    {
        Turn_On();
        G_Fan_Force_Run_Time = 180;
        if (Comp_SA_EN == ENABLE)
        {
            G_Disp_SA_Time = 30;
        }
    }

    if (G_SYS_Fast_Test)
    {
        G_SYS_Fast_Test = DISABLE;
    }
}

// *****************************************************************************
// 函数名称 : Set_FAN_Tyde
// 功能说明 : 设置风机档位
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
void Set_FAN_Tyde(void)
{
    if ((G_SYS_Power_Status == OFF) || (G_Set_Fan_Tyde_EN == DISABLE))
    {
        Key_ERR_Buzz_Cnt = 3;
        return;
    }
    if (Comp_SA_EN == ENABLE)
    {
        G_Disp_SA_Time = 30;
    }
    G_SYS_Fan_Tyde_Buf++;
    if (G_SYS_Fan_Tyde_Buf > HIGH_FAN)
    {
        G_SYS_Fan_Tyde_Buf = LOW_FAN;
    }
    G_Set_SYS_Fan_Tyde_Time = 20;

    G_Buzz_Time = BUZZ_short_time;
}

// *****************************************************************************
// 函数名称 : Set_Hum_Up
// 功能说明 : 目标除湿值“+”号键功能
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
void Set_Hum_Up(void)
{

    if (G_SYS_Mode_Buf != mode_SYS_HUM)
    {
        Key_ERR_Buzz_Cnt = 3;
        return;
    }

    G_Buzz_Time = BUZZ_short_time;
    if (G_Set_SYS_Hum_Time == 0)
    {
        G_Set_SYS_Hum_Time = 50;
        return;
    }

    G_Set_SYS_Hum_Time = 50;
    G_Buzz_Time = BUZZ_short_time;

    if (G_SYS_Hum_Set_Buf < 60)
    {
        G_SYS_Hum_Set_Buf += 5;
    }
}

// *****************************************************************************
// 函数名称 : Set_Hum_Down
// 功能说明 : 目标除湿值“-”号键功能
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
void Set_Hum_Down(void)
{

    if (G_SYS_Mode_Buf != mode_SYS_HUM)
    {
        Key_ERR_Buzz_Cnt = 3;
        return;
    }

    G_Buzz_Time = BUZZ_short_time;
    if (G_Set_SYS_Hum_Time == 0)
    {
        G_Set_SYS_Hum_Time = 50;
        return;
    }

    G_Set_SYS_Hum_Time = 50;

    if (G_SYS_Hum_Set_Buf >= 30)
    {
        G_SYS_Hum_Set_Buf -= 5;
    }
}

// *****************************************************************************
// 函数名称 : Set_Timer_Up
// 功能说明 : 定时“+”号键功能
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
void Set_Timer_Up(void)
{
    G_Time_Setting_Time = 50;
    S_Time_Set_IN = 1;
    G_Buzz_Time = BUZZ_short_time;
    if (G_Time_Buf < 8)
    {
        G_Time_Buf++;
    }
    else
    {
        G_Time_Buf = 0;
    }
}

// *****************************************************************************
// 函数名称 : Set_Timer_Down
// 功能说明 : 定时“-”号键功能
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
void Set_Timer_Down(void)
{
    G_Time_Setting_Time = 50;
    S_Time_Set_IN = 1;
    G_Buzz_Time = BUZZ_short_time;
    if (G_Time_Buf > 0)
    {
        G_Time_Buf--;
    }
    else
    {
        G_Time_Buf = 8;
    }
}

// *****************************************************************************
// 函数名称 : Up_Key_Function
// 功能说明 : “+”号键功能
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
void Up_Key_Function(void)
{
    if ((G_SYS_Power_Status == OFF) && (G_Time_Setting_Time == 0))
    {
        Key_ERR_Buzz_Cnt = 3;
        return;
    }
    if (G_Disp_SA_Time > 0)
    {
        return;
    }
    if (Comp_SA_EN == ENABLE)
    {
        if ((G_Time_Setting_Time == 0) && (G_Set_SYS_Hum_Time == 0))
        {
            G_Disp_SA_Time = 30;
        }
    }

    if (G_Time_Setting_Time > 0) //设置定时
    {
        Set_Timer_Up();
    }
    else //设置湿度
    {
        Set_Hum_Up();
    }
}

// *****************************************************************************
// 函数名称 : Down_Key_Function
// 功能说明 : “-”号键功能
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
void Down_Key_Function(void)
{
    if ((G_SYS_Power_Status == OFF) && (G_Time_Setting_Time == 0))
    {
        Key_ERR_Buzz_Cnt = 3;
        return;
    }

    if (G_Disp_SA_Time > 0)
    {
        return;
    }

    if (Comp_SA_EN == ENABLE)
    {
        if ((G_Time_Setting_Time == 0) && (G_Set_SYS_Hum_Time == 0))
        {
            G_Disp_SA_Time = 30;
        }
    }

    if (G_Time_Setting_Time > 0) //设置定时
    {
        Set_Timer_Down();
    }
    else //设置湿度
    {
        Set_Hum_Down(); //设置湿度
    }
}

// *****************************************************************************
// 函数名称 : Set_In_Time
// 功能说明 : 设置定时功能
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
void Set_In_Time(void)
{
    G_Buzz_Time = BUZZ_short_time;

    if (G_Disp_SA_Time > 0)
    {
        return;
    }

    if (Comp_SA_EN == ENABLE)
    {
        if (G_Time_Setting_Time == 0)
        {
            G_Disp_SA_Time = 30;
        }
    }

    if (G_Time_Setting_Time > 0) //正在设置则取消定时
    {
        G_Time_Setting_Time = 0;
        S_Time_Set_IN = 0;
        G_Time_Run = 0;
        G_Time_Set = G_Time_Set; //恢复设置值
    }
    else //刚进入设置
    {
        G_Time_Setting_Time = 50; //显示时间
        if (G_Time_Run > 0)
        {
            S_Time_Set_IN = 0;
        } //如果正在定时  则不重新赋值定时
        else
        {
            S_Time_Set_IN = 1;
        }
    }

    if (G_Set_SYS_Hum_Time > 0) //调整湿度时, 马上确认湿度设置
    {
        G_Set_SYS_Hum_Time = 1;
    }
}

// *****************************************************************************
// 函数名称 : Set_SYS_Mode
// 功能说明 : 设置系统模式
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
void Set_SYS_Mode(void)
{
    if (G_SYS_Power_Status == OFF)
    {
        Key_ERR_Buzz_Cnt = 3;
        return;
    }
    if (Comp_SA_EN == ENABLE)
    {
        G_Disp_SA_Time = 30;
    }
    G_SYS_Mode_Buf++;
    if (G_SYS_Mode_Buf > mode_SYS_HUM)
    {
        G_SYS_Mode_Buf = mode_DRY;
    }

    S_Set_SYS_Mode_Time = 20;
    G_Buzz_Time = BUZZ_short_time;

    if (G_Set_SYS_Hum_Time > 0) //调整湿度时, 马上确认湿度设置
    {
        G_Set_SYS_Hum_Time = 1;
    }

    if (G_Time_Setting_Time > 0)
    {
        G_Time_Setting_Time = 1;
    }
}

// *****************************************************************************
// 函数名称 : Set_Pump_Status
// 功能说明 : 设置打水PUMP功能
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
void Set_Pump_Status(void)
{
    if (G_SYS_Power_Status == OFF)
    {
        Key_ERR_Buzz_Cnt = 3;
        return;
    }
    if (Comp_SA_EN == ENABLE)
    {
        G_Disp_SA_Time = 30;
    }
    G_Buzz_Time = BUZZ_short_time;

    if (G_Pump_Status == ENABLE)
    {
        G_Pump_Status = DISABLE;
    }
    else
    {
        G_Pump_Status = ENABLE;
    }

    G_Buzz_Time = BUZZ_short_time;
}

// *****************************************************************************
// 函数名称 : Buf_Confirm_Logic
// 功能说明 : 设置机台参数延时后确认使用
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
void Buf_Confirm_Logic(void)
{
    if (!_100mS_For_SYS)
    {
        return;
    }
    //
    if (S_Set_SYS_Mode_Time > 0)
    {
        S_Set_SYS_Mode_Time--;
        if (S_Set_SYS_Mode_Time == 0)
        {
            SYS_Mode_Change_Handle(G_SYS_Mode, G_SYS_Mode_Buf);
            G_SYS_Mode = G_SYS_Mode_Buf;
        }
    }
    //
    if (G_Set_SYS_Hum_Time > 0)
    {
        G_Set_SYS_Hum_Time--;
        if (G_Set_SYS_Hum_Time == 0)
        {
            G_SYS_Hum_Set = G_SYS_Hum_Set_Buf;
        }
    }
    //
    if (G_Set_SYS_Fan_Tyde_Time > 0)
    {
        G_Set_SYS_Fan_Tyde_Time--;
        if (G_Set_SYS_Fan_Tyde_Time == 0)
        {
            G_SYS_Fan_Tyde = G_SYS_Fan_Tyde_Buf;
        }
    }
    //

    if (G_Time_Setting_Time > 0) //定时设定确认
    {
        G_Time_Setting_Time--;
        if ((G_Time_Setting_Time == 0) && (S_Time_Set_IN == 1))
        {
            G_Time_Set = G_Time_Buf;
            G_Time_Run = G_Time_Set;
            G_Time_Run = G_Time_Run * 3600;
            G_Time_Setting_Time = 0;
        }
    }

    if (G_Disp_SA_Time > 0)
    {
        G_Disp_SA_Time--;
    }
}

// *****************************************************************************
// 函数名称 : SYS_Timing_Switch_Logic
// 功能说明 : 定时开关机功能
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
void SYS_Timing_Switch_Logic(void)
{

    if (!_1S_For_For_SYS)
    {
        return;
    }

    if (G_Time_Run > 0) //定时运行
    {
        G_Time_Run--;
        if (G_Time_Run == 0)
        {
            if (G_SYS_Power_Status == ON)
            {
                Turn_Off();
            }
            else
            {
                Turn_On();
            }
        }

        if ((G_Time_Run % 3600) == 0)
        {
            G_Time_Buf = (UI08)(G_Time_Run / 3600); //显示剩余时间
        }
    }
}

// *****************************************************************************
// 函数名称 : Set_In_Fast_Test
// 功能说明 : 进入快测的参数设置
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
void Set_In_Fast_Test(void)
{
    G_SYS_Fast_Test = ENABLE;
    G_Buzz_Time = BUZZ_short_time;
    G_Disp_Machine_Temp_Time = 8;
}

// *****************************************************************************
// 函数名称 : Set_In_Comp_Test
// 功能说明 : 进入压缩机自检的参数设置
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
void Set_In_Comp_Test(void)
{
    Turn_On(); //因触摸按键是松开有效，  所以在进入强制模式时将其开机
               //
    G_Comp_Test_EN = ENABLE;
    S_Comp_Test_Disp_En_Time = 200;
    G_Buzz_Time = BUZZ_long_time;
}

// *****************************************************************************
// 函数名称 : Set_RESET_WIFI
// 功能说明 : WIFI参数初始化
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
void Set_RESET_WIFI(void)
{
    // Wifi_Comm_Err_Dsp_time=3;//wifi模块通信故障   在按下wifi复位键后显示 3s
    _wifi_reset_En = 1;
    G_Buzz_Time = BUZZ_short_time;
}

// *****************************************************************************
// 函数名称 : Set_TEST_WIFI
// 功能说明 : WIFI 信号产测
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
void Set_TEST_WIFI(void)
{
    SYS_Data_Rest(); //自检写入系统默认参数

    _Wifi_factory_test = 1; //产检使能

    G_Buzz_Time = BUZZ_short_time;
}

// *****************************************************************************
// 函数名称 : Set_In_Self_Test
// 功能说明 : 进入自检 参数恢复出厂设置
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
void Set_In_Self_Test(void)
{
    //
    G_Buzz_Time = BUZZ_long_time;
    G_SYS_Self_Test = ENABLE;
    G_Communication_Time = 2;
    //自检写入系统默认参数
    G_SYS_Power_Status = OFF;
    G_SYS_Hum_Set = 60;
    G_SYS_Mode = mode_SYS_HUM;
    G_SYS_Mode_Buf = mode_SYS_HUM;

    G_Pump_Status = DISABLE;
    G_Filter_Run_Time = 0;
}

/*************************************************
 // 函数名称    : general_deal
 // 功能描述    :
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void General_Deal(void)
{
    Prg_mS100_General();
    Prg_mS500_General();
    Prg_S_General();
    SYS_Timing_Switch_Logic();
    Timer_Deal();
}
