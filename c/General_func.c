#include "General.h"

GCE_XDATA TS_SYS_Config G_Sys_Config;

GCE_XDATA UI32 G_Time_Run = 0;          //��ʱʣ��ʱ��
GCE_XDATA UI08 G_Time_Set = 1;          //��ʱ�趨ʱ��
GCE_XDATA UI08 G_Time_Buf = 1;          //��ʱ�趨ʱ��buf
GCE_XDATA UI08 G_Time_Setting_Time = 0; //��ʱ�趨(ȷ��ʱ��)
GCE_XDATA UI08 S_Time_Set_IN = 0;       //�Ƿ�����˶�ʱ����

GCE_XDATA UI08 G_Power_Delay_Time = 3; //ϵͳ������ʱ

GCE_XDATA UI08 G_Test_Step = 0;  //�Լ�
GCE_XDATA UI08 G_Test_Cont1 = 0; //�Լ�
GCE_XDATA UI08 G_Test_Cont2 = 0; //�Լ�

GCE_XDATA TS_SYS_Err G_Sys_Err;                   //����
GCE_XDATA TE_FuncState G_SYS_Self_Test = DISABLE; //�Լ��־
GCE_XDATA TE_FuncState G_SYS_Fast_Test = DISABLE; // ����־

//
GCE_XDATA ONOFF_STATUS G_SYS_Power_Status = OFF;     // ���ػ�״̬
GCE_XDATA ONOFF_STATUS G_SYS_Power_Status_Old = OFF; // ���ػ�״̬

GCE_XDATA TU_SYS_Mode G_SYS_Mode = mode_SYS_HUM;     // ϵͳģʽ
GCE_XDATA TU_SYS_Mode G_SYS_Mode_Buf = mode_SYS_HUM; // ϵͳģʽ
GCE_XDATA UI08 S_Set_SYS_Mode_Time = 0;              // ģʽ�趨ʱ��

GCE_XDATA UI08 G_SYS_Hum_Set = 60;     //ʪ���趨
GCE_XDATA UI08 G_SYS_Hum_Set_Buf = 60; //ʪ���趨buf
GCE_XDATA UI08 G_Set_SYS_Hum_Time = 0;

GCE_XDATA TU_FAN_Speed_Type G_SYS_Fan_Tyde = LOW_FAN;     //���з���
GCE_XDATA TU_FAN_Speed_Type G_SYS_Fan_Tyde_Buf = LOW_FAN; //���з���
GCE_XDATA UI08 G_Set_SYS_Fan_Tyde_Time = 0;               //���÷���ʱ��
GCE_XDATA TE_FuncState G_Set_Fan_Tyde_EN = ENABLE;        //�Ƿ������趨����

GCE_XDATA UI08 G_M_Sleep_Time = 10; //�������ø���Դ�巢��˯�߱�־

UI16 G_Buzz_Time = 0;                     //������ʱ��
TE_Pin_Status G_Buzzer_IO_Status = RESET; //��������ǰIO״̬
GCE_XDATA UI08 G_Buzz_Cnt = 0;            //ˮ������ʱ�������������
GCE_XDATA UI08 Key_ERR_Buzz_Cnt = 0;      //������Чʱ����������Ĵ���

GCE_XDATA TE_FuncState G_Pump_Status = DISABLE; //�Զ�ˮ�ó�ˮ�����Ƿ��

GCE_XDATA UI08 G_Disp_SA_Time = 0;

void Buf_Confirm_Logic(void);

// *****************************************************************************
// �������� : SYS_Data_Rest
// ����˵�� : �ظ���������ʱ����Ҫ��λ�Ĳ���
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
void SYS_Data_Rest(void)
{
    G_SYS_Fast_Test = DISABLE;

    G_Time_Run = 0;
    G_Time_Set = 1;
    G_Time_Buf = 1;

    G_Pump_Status = DISABLE;

    G_SYS_Mode = mode_SYS_HUM;     //ϵͳģʽ
    G_SYS_Mode_Buf = mode_SYS_HUM; //ϵͳģʽ
    S_Set_SYS_Mode_Time = 0;       //ģʽ�趨ʱ��

    G_SYS_Hum_Set = 60;     //ʪ���趨
    G_SYS_Hum_Set_Buf = 60; //ʪ���趨buf
    G_Set_SYS_Hum_Time = 0; //ʪ���趨(ȷ��ʱ��)

    G_SYS_Fan_Tyde = LOW_FAN;     //���з���
    G_SYS_Fan_Tyde_Buf = LOW_FAN; //���з���
    G_Set_Fan_Tyde_EN = ENABLE;
    G_Set_SYS_Fan_Tyde_Time = 0;

    G_Def_Reg.Defrost_status = DISABLE;
    G_Filter_Run_Time = 0;
    G_Filter_Run_Time_Buf = 0;
}

// *****************************************************************************
// �������� : SYS_Data_Init
// ����˵�� : ������ʼ��
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
// �������� : UI16_Addition_Operation
// ����˵�� : 16λ�ӷ�
// ��ڲ��� : _Subtraction ���ӵ�ֵ   _minuend�Ӷ���ֵ
// ���ڲ��� : 16λ������
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
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
// �������� : UI16_Subtraction_Operation
// ����˵�� : 16λ����
// ��ڲ��� : _Subtraction ������ֵ   _minuend������ֵ
// ���ڲ��� : 16λ������
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
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
// �������� : Prg_mS100_General
// ����˵�� : 100mS������
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
void Prg_mS100_General(void)
{
    if (!_100mS_For_SYS)
    {
        return;
    }

    Buf_Confirm_Logic();
}

// *****************************************************************************
// �������� : Prg_mS500_General
// ����˵�� : 500mS������
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
// �������� : Prg_S_General
// ����˵�� : S������
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
// �������� : Turn_Off
// ����˵�� : �����ػ�ʱ�ı�������
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
void Turn_On(void)
{
    G_SYS_Power_Status = ON;
    G_Time_Run = 0;
    G_Time_Setting_Time = 0;
    S_Time_Set_IN = 0;
    G_Set_SYS_Hum_Time = 0;
    WIFI_Self_Test_delay_time = 0; //�ػ�״̬�ſɽ�����
}

// *****************************************************************************
// �������� : Turn_Off
// ����˵�� : �����ػ�ʱ�ı�������
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
void Turn_Off(void)
{
    G_SYS_Power_Status = OFF;
    //
    G_Time_Run = 0;
    G_Time_Setting_Time = 0;
    S_Time_Set_IN = 0;
    G_Set_SYS_Hum_Time = 0;

    G_Comp_Test_EN = DISABLE;
    WIFI_Self_Test_delay_time = WIFI_SELF_TEST_IN_TIMER; //�ػ�״̬�ſɽ�����
}

// *****************************************************************************
// �������� : Wdt_Init
// ����˵�� : ���Ź���ʼ��
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
void Wdt_Init(void)
{
    // 1--1 -- 00   ?��Code Optionѡ��ENWDT
    WDTCON = 0x10; //���Ź���ͨ����?ʱ������(ʾ����ʵ��700ms��λ)
}

// *****************************************************************************
// �������� : Sys_Initial
// ����˵�� : ϵͳ��ʼ��
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
// �������� : Get_Invalid_Key
// ����˵�� : �ж��Ƿ����ִ�а�������
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
TE_FuncState Get_Invalid_Key(UI08 key_num)
{
    if (G_Power_Delay_Time > 0) //�ڼ䰴����Ч
    {
        return ENABLE;
    }

    if (key_num == 0x00) //û�а�������
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
// �������� : Set_Power_Status
// ����˵�� : ���ÿ��ػ�״̬
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
// �������� : Set_FAN_Tyde
// ����˵�� : ���÷����λ
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
// �������� : Set_Hum_Up
// ����˵�� : Ŀ���ʪֵ��+���ż�����
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
// �������� : Set_Hum_Down
// ����˵�� : Ŀ���ʪֵ��-���ż�����
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
// �������� : Set_Timer_Up
// ����˵�� : ��ʱ��+���ż�����
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
// �������� : Set_Timer_Down
// ����˵�� : ��ʱ��-���ż�����
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
// �������� : Up_Key_Function
// ����˵�� : ��+���ż�����
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

    if (G_Time_Setting_Time > 0) //���ö�ʱ
    {
        Set_Timer_Up();
    }
    else //����ʪ��
    {
        Set_Hum_Up();
    }
}

// *****************************************************************************
// �������� : Down_Key_Function
// ����˵�� : ��-���ż�����
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

    if (G_Time_Setting_Time > 0) //���ö�ʱ
    {
        Set_Timer_Down();
    }
    else //����ʪ��
    {
        Set_Hum_Down(); //����ʪ��
    }
}

// *****************************************************************************
// �������� : Set_In_Time
// ����˵�� : ���ö�ʱ����
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

    if (G_Time_Setting_Time > 0) //����������ȡ����ʱ
    {
        G_Time_Setting_Time = 0;
        S_Time_Set_IN = 0;
        G_Time_Run = 0;
        G_Time_Set = G_Time_Set; //�ָ�����ֵ
    }
    else //�ս�������
    {
        G_Time_Setting_Time = 50; //��ʾʱ��
        if (G_Time_Run > 0)
        {
            S_Time_Set_IN = 0;
        } //������ڶ�ʱ  �����¸�ֵ��ʱ
        else
        {
            S_Time_Set_IN = 1;
        }
    }

    if (G_Set_SYS_Hum_Time > 0) //����ʪ��ʱ, ����ȷ��ʪ������
    {
        G_Set_SYS_Hum_Time = 1;
    }
}

// *****************************************************************************
// �������� : Set_SYS_Mode
// ����˵�� : ����ϵͳģʽ
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

    if (G_Set_SYS_Hum_Time > 0) //����ʪ��ʱ, ����ȷ��ʪ������
    {
        G_Set_SYS_Hum_Time = 1;
    }

    if (G_Time_Setting_Time > 0)
    {
        G_Time_Setting_Time = 1;
    }
}

// *****************************************************************************
// �������� : Set_Pump_Status
// ����˵�� : ���ô�ˮPUMP����
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
// �������� : Buf_Confirm_Logic
// ����˵�� : ���û�̨������ʱ��ȷ��ʹ��
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

    if (G_Time_Setting_Time > 0) //��ʱ�趨ȷ��
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
// �������� : SYS_Timing_Switch_Logic
// ����˵�� : ��ʱ���ػ�����
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
void SYS_Timing_Switch_Logic(void)
{

    if (!_1S_For_For_SYS)
    {
        return;
    }

    if (G_Time_Run > 0) //��ʱ����
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
            G_Time_Buf = (UI08)(G_Time_Run / 3600); //��ʾʣ��ʱ��
        }
    }
}

// *****************************************************************************
// �������� : Set_In_Fast_Test
// ����˵�� : ������Ĳ�������
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
void Set_In_Fast_Test(void)
{
    G_SYS_Fast_Test = ENABLE;
    G_Buzz_Time = BUZZ_short_time;
    G_Disp_Machine_Temp_Time = 8;
}

// *****************************************************************************
// �������� : Set_In_Comp_Test
// ����˵�� : ����ѹ�����Լ�Ĳ�������
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
void Set_In_Comp_Test(void)
{
    Turn_On(); //�����������ɿ���Ч��  �����ڽ���ǿ��ģʽʱ���俪��
               //
    G_Comp_Test_EN = ENABLE;
    S_Comp_Test_Disp_En_Time = 200;
    G_Buzz_Time = BUZZ_long_time;
}

// *****************************************************************************
// �������� : Set_RESET_WIFI
// ����˵�� : WIFI������ʼ��
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
void Set_RESET_WIFI(void)
{
    // Wifi_Comm_Err_Dsp_time=3;//wifiģ��ͨ�Ź���   �ڰ���wifi��λ������ʾ 3s
    _wifi_reset_En = 1;
    G_Buzz_Time = BUZZ_short_time;
}

// *****************************************************************************
// �������� : Set_TEST_WIFI
// ����˵�� : WIFI �źŲ���
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
void Set_TEST_WIFI(void)
{
    SYS_Data_Rest(); //�Լ�д��ϵͳĬ�ϲ���

    _Wifi_factory_test = 1; //����ʹ��

    G_Buzz_Time = BUZZ_short_time;
}

// *****************************************************************************
// �������� : Set_In_Self_Test
// ����˵�� : �����Լ� �����ָ���������
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
void Set_In_Self_Test(void)
{
    //
    G_Buzz_Time = BUZZ_long_time;
    G_SYS_Self_Test = ENABLE;
    G_Communication_Time = 2;
    //�Լ�д��ϵͳĬ�ϲ���
    G_SYS_Power_Status = OFF;
    G_SYS_Hum_Set = 60;
    G_SYS_Mode = mode_SYS_HUM;
    G_SYS_Mode_Buf = mode_SYS_HUM;

    G_Pump_Status = DISABLE;
    G_Filter_Run_Time = 0;
}

/*************************************************
 // ��������    : general_deal
 // ��������    :
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void General_Deal(void)
{
    Prg_mS100_General();
    Prg_mS500_General();
    Prg_S_General();
    SYS_Timing_Switch_Logic();
    Timer_Deal();
}
