#include "General.h"

GCE_XDATA UUI08 SYS_Inspect_bit = {0};   // Inspect������־λ
GCE_XDATA UI16 SYS_Inspect_Key_Data = 0; //����data

GCE_XDATA UI16 Comp_Test_first_count = 0; //��ʱcount

GCE_XDATA UI08 Comp_test_Delay_ms = 0;      //����ʱ��
GCE_XDATA UI08 Disp_PWM_VALUE_TIMER = 0xff; //��ʾʱ��
/*********************************************************
������: int main(void)
��  ��: ������
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
void SYS_Inspect_ms_general(void)
{
    /*if(!_SYS_Inspect_ms)
    {return;}
   _SYS_Inspect_ms=0;*/
}

/*********************************************************
������: SYS_Inspect_s_general
��  ��: Sʱ��Ƭ
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
void SYS_Inspect_s_general(void)
{
    if (!_1S_For_For_SYS)
    {
        return;
    }
}

/*********************************************************
������: SYS_Inspect_Key
��  ��:
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
/*
void SYS_Inspect_Key(void)
{
    if (!_SYS_Inspect_Key_OK)
    {
        return;
    }
    _SYS_Inspect_Key_OK = 0;
    //����powerkey���� ���˳�
    if (SYS_Inspect_Key_Data != (0x01 << (power_key - 1)))
    {
        Comp_Test_Key_count = 0;
        _Comp_Test_Key_Count_EN = 0;
        return;
    }
    //
    if ((Comp_Test_Key_count < 100) || (Comp_Test_Key_count == 0xffff))
    {
        return;
    }
    //
    if (_COMP_TEST_EN)
    {
        _COMP_TEST_EN = 0;
        Disp_PWM_VALUE_TIMER = 0;
    }
    //��� �ϵ� 200ms �ڰ���  ����Ч
    if ((Comp_Test_first_count <= 100) || (_Comp_Test_Key_First_Status))
    {
        _Comp_Test_Key_First_Status = 1;
        return;
    }

    if (Power_Delay_Time > 0)
    {
        _Comp_Test_Key_Count_EN = 1;
    }

    if ((Comp_Test_Key_count > (1000 * 20)) && (Comp_Test_Key_count < 0xffff) && (_Comp_Test_Key_Count_EN))
    //&&(M_Power_Status==ON)
    {
        Turn_On(); //��CORA0-1273A�ǰ���������Ч  ����ֻ�ܷ��ڽ���ǿ��ģʽʱ���俪��
                   //
        _COMP_TEST_EN = 1;
        _Comp_Test_Key_Count_EN = 0;
        Comp_Test_Disp_En_Timer = 2000;
        Comp_Test_Key_count = 0xffff;
        Buzz_Time = BUZZ_long_time;
    }
}
*/

/*********************************************************
������: SYS_Inspect_Key
��  ��:
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
/*
void SYS_Inspect_Key(void)
{
    if (!_SYS_Inspect_Key_OK)
    {
        return;
    }
    _SYS_Inspect_Key_OK = 0;
    //����powerkey���� ���˳�
    if (SYS_Inspect_Key_Data != (0x01 << (power_key - 1)))
    {
        Comp_Test_Key_count = 0;
        _Comp_Test_Key_Count_EN = 0;
        return;
    }
    //
    if ((Comp_Test_Key_count < 100) || (Comp_Test_Key_count == 0xffff))
    {
        return;
    }
    //
    if (_COMP_TEST_EN)
    {
        _COMP_TEST_EN = 0;
        Disp_PWM_VALUE_TIMER = 0;
    }
    //��� �ϵ� 200ms �ڰ���  ����Ч
    if ((Comp_Test_first_count <= 100) || (_Comp_Test_Key_First_Status))
    {
        _Comp_Test_Key_First_Status = 1;
        return;
    }

    if (Power_Delay_Time > 0)
    {
        _Comp_Test_Key_Count_EN = 1;
    }

    if ((Comp_Test_Key_count > (1000 * 20)) && (Comp_Test_Key_count < 0xffff) && (_Comp_Test_Key_Count_EN))
    //&&(M_Power_Status==ON)
    {
        Turn_On(); //��CORA0-1273A�ǰ���������Ч  ����ֻ�ܷ��ڽ���ǿ��ģʽʱ���俪��
                   //
        _COMP_TEST_EN = 1;
        _Comp_Test_Key_Count_EN = 0;
        Comp_Test_Disp_En_Timer = 2000;
        Comp_Test_Key_count = 0xffff;
        Buzz_Time = BUZZ_long_time;
    }
}
*/

/*
/*********************************************************
������: Comp_Test_general
��  ��:
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
/*
void Comp_Test_general(void)
{
    if (!_COMP_TEST_EN)
    {
        Comp_Test_Time = 0;
        return;
    }

    if (Comp_Test_Time < COMP_TEST_ON_TIME)
    {
        G_Comp_Para.OUT = ON;
        Fan_Speed_Out_Buf = HIGH_FAN;
    }
    else if (Comp_Test_Time < (COMP_TEST_ON_TIME + COMP_TEST_OFF_TIME))
    {
        G_Comp_Para.OUT = OFF;
        Fan_Speed_Out_Buf = OFF_FAN;
    }
    else
    {
        Comp_Test_Time = 0;
    }
}
*/
/*********************************************************
������: disp_pwm_temp
��  ��:
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
/*
void disp_pwm_temp(void)
{
    UI08 dsp_temp = 0;

    if ((Disp_PWM_VALUE_TIMER % 2) == 0)
    {
        return;
    }
    //
    if (Disp_PWM_VALUE_TIMER == 1)
    {
        dsp_temp = (UI08)Temp_room.value;
        //
        if (dsp_temp > 9)
        {
            dig1_2_dsp(dsp_temp - 9);
        }
        else
        {
            dsp_temp = 9 - dsp_temp;
            if (dsp_temp > 9)
            {
                dsp_temp = 9;
            }
            dig1_2_dsp(dsp_temp);
            dig1_num = DATA_neg; //-
        }
    }
    else if (Disp_PWM_VALUE_TIMER == 3)
    {
        dsp_temp = (UI08)Temp_coil.value;
        //
        if (dsp_temp > 9)
        {
            dig1_2_dsp(dsp_temp - 9);
        }
        else
        {
            dsp_temp = 9 - dsp_temp;
            if (dsp_temp > 9)
            {
                dsp_temp = 9;
            }
            dig1_2_dsp(dsp_temp);
            dig1_num = DATA_neg; //-
        }
    }
    else if (Disp_PWM_VALUE_TIMER == 5)
    {
        Disp_All();
    }
}
*/
/*********************************************************
������: SYS_Inspect_Disp
��  ��:
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
/*
void SYS_Inspect_Disp(void)
{

    if (Comp_Test_Disp_En_Timer > 0)
    {
        Disp_All();
    }
}
*/
/*********************************************************
������: SYS_Inspect_Deal
��  ��:
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
void SYS_Inspect_Deal(void)
{
    // SYS_Inspect_ms_general();
    // SYS_Inspect_s_general();
    // SYS_Inspect_Key();
    // Comp_Test_general();
}
