/**
  ******************************************************************************
  * @file ADC_driver.c

  ******************************************************************************

  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "General.h" //系统 通用调用文件

/* Private defines -----------------------------------------------------------*/
//
//==========================================================================================
GCE_XDATA UI08 S_ADC_End = 0;     // 一个通道采样完成标志
GCE_XDATA UI08 S_Sample_Time = 0; // 采样时间间隔
GCE_XDATA UI16 S_ADC_Max = 0;     // 滤波算法中存储最大值
GCE_XDATA UI16 S_ADC_min = 0;     // 滤波算法中存储最小值

GCE_XDATA UI08 S_Sample_Cnt = 0;                                    // 采样次数计数
GCE_XDATA UI32 S_ADC_Sum = 0;                                       // 一个通道采样的AD累加值
GCE_XDATA UI16 S_ADC_Value_Buf;                                     // 一个通道采样完后的滤波后AD值
GCE_CONST UI08 S_Channel[ADC_channel_total] = {ADC_channel_define}; // 采样通道数组
GCE_XDATA UI08 S_Channel_Ptr = 0;                                   // 采样通道数据下标值

GCE_XDATA UI16 G_ADC_Value[5]; //存储AD值

// *****************************************************************************
// 函数名称 : ADC_Init
// 功能说明 : AD驱动初始化（包涵通道设置，和启动单次AD采样）
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
void ADC_Init(void)
{
    ADC_Device_Init(S_Channel[S_Channel_Ptr]);
}

// *****************************************************************************
// 函数名称 : Sample_Loop
// 功能说明 : AD采样滤波算法函数
// 入口参数 : _interval_time  采样间隔时间
//            _reject_count        通道切换后，丢弃前面多少采样
//            _total_sample_count 总共采样几次
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ： 去掉前面_reject_count次采样，之后去掉最大最小值，求和平均
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
    START_ADC(); //开始ADC转换
    //
    S_Sample_Cnt++;
    if (S_Sample_Cnt <= _reject_count)
    {
        return; //去掉前reject_count次
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
// 函数名称 : ADC_Sample
// 功能说明 : 转载采样完成的AD值，和通道切换，启动AD采样
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

void ADC_Sample(void)
{
    if (!_5mS_For_SYS) //每5ms执行一次
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
        //通道切换
        ADC_Init_Channel(S_Channel[S_Channel_Ptr]);
        S_Sample_Time = 0;
        S_ADC_End = 0;
    }
    else
    {
        Sample_Loop(1, 3, 33); //每5ms执行一次采样,采33次,去掉前3次,再平均
    }
}
