/**
  ******************************************************************************
  * @file ADC_driver.c

  ******************************************************************************

  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "General.h" //ϵͳ ͨ�õ����ļ�

/* Private defines -----------------------------------------------------------*/
//
//==========================================================================================
GCE_XDATA UI08 S_ADC_End = 0;     // һ��ͨ��������ɱ�־
GCE_XDATA UI08 S_Sample_Time = 0; // ����ʱ����
GCE_XDATA UI16 S_ADC_Max = 0;     // �˲��㷨�д洢���ֵ
GCE_XDATA UI16 S_ADC_min = 0;     // �˲��㷨�д洢��Сֵ

GCE_XDATA UI08 S_Sample_Cnt = 0;                                    // ������������
GCE_XDATA UI32 S_ADC_Sum = 0;                                       // һ��ͨ��������AD�ۼ�ֵ
GCE_XDATA UI16 S_ADC_Value_Buf;                                     // һ��ͨ�����������˲���ADֵ
GCE_CONST UI08 S_Channel[ADC_channel_total] = {ADC_channel_define}; // ����ͨ������
GCE_XDATA UI08 S_Channel_Ptr = 0;                                   // ����ͨ�������±�ֵ

GCE_XDATA UI16 G_ADC_Value[5]; //�洢ADֵ

// *****************************************************************************
// �������� : ADC_Init
// ����˵�� : AD������ʼ��������ͨ�����ã�����������AD������
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
void ADC_Init(void)
{
    ADC_Device_Init(S_Channel[S_Channel_Ptr]);
}

// *****************************************************************************
// �������� : Sample_Loop
// ����˵�� : AD�����˲��㷨����
// ��ڲ��� : _interval_time  �������ʱ��
//            _reject_count        ͨ���л��󣬶���ǰ����ٲ���
//            _total_sample_count �ܹ���������
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     �� ȥ��ǰ��_reject_count�β�����֮��ȥ�������Сֵ�����ƽ��
//
// *****************************************************************************
static void Sample_Loop(UI08 _interval_time, UI08 _reject_count, UI08 _total_sample_count)
{
    UI16 adc_buf;
    UI16 x;

    if (S_ADC_End)
    {
        return;
    }

    if (++S_Sample_Time < _interval_time)
    {
        return;
    }
    S_Sample_Time = 0;

    //
    adc_buf = Get_ADC_Value();
    //====================================
    //
    START_ADC(); //��ʼADCת��
    //
    S_Sample_Cnt++;
    if (S_Sample_Cnt <= _reject_count)
    {
        return; //ȥ��ǰreject_count��
    }

    if (S_Sample_Cnt >= (_reject_count + 3))
    {
        if (adc_buf > S_ADC_Max)
        {
            x = S_ADC_Max;
            S_ADC_Max = adc_buf;
            adc_buf = x;
        }
        else if (adc_buf < S_ADC_min)
        {
            x = S_ADC_min;
            S_ADC_min = adc_buf;
            adc_buf = x;
        }

        S_ADC_Sum += adc_buf;

        if (S_Sample_Cnt < _total_sample_count)
        {
            return;
        }

        S_Sample_Cnt = 0;
        S_ADC_Value_Buf = S_ADC_Sum / (_total_sample_count - _reject_count - 2);
        S_ADC_Sum = 0;
        S_ADC_End = 1;
    }
    else if (S_Sample_Cnt == (_reject_count + 1))
    {
        S_ADC_Max = adc_buf;
    }
    else if (S_Sample_Cnt == (_reject_count + 2))
    {
        S_ADC_min = adc_buf;
        if (S_ADC_Max < S_ADC_min)
        {
            adc_buf = S_ADC_Max;
            S_ADC_Max = S_ADC_min;
            S_ADC_min = adc_buf;
        }
    }
}

// *****************************************************************************
// �������� : ADC_Sample
// ����˵�� : ת�ز�����ɵ�ADֵ����ͨ���л�������AD����
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

void ADC_Sample(void)
{
    if (!_5mS_For_SYS) //ÿ5msִ��һ��
    {
        return;
    }

    if (S_ADC_End)
    {
        if (S_Channel[S_Channel_Ptr] == Tcomp_channel)
        {
            WRITE_TEST_AD(S_ADC_Value_Buf);
        }

        if (++S_Channel_Ptr >= ADC_channel_total)
        {
            S_Channel_Ptr = 0;
        }
        //ͨ���л�
        ADC_Init_Channel(S_Channel[S_Channel_Ptr]);
        S_Sample_Time = 0;
        S_ADC_End = 0;
    }
    else
    {
        Sample_Loop(1, 3, 33); //ÿ5msִ��һ�β���,��33��,ȥ��ǰ3��,��ƽ��
    }
}
