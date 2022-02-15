#include "General.h"

// GCE_XDATA FuncState _Self_Test = DISABLE;

/*************************************************
 // 函数名称    : io_init
 // 功能描述    : IO口初始化
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void IO_Init(void)
{
    // TK上的IO设置为强推挽输出1
    /*
        P00:NC       P01:NC     P02:NC    P03:NC
        P04:SW6    P05:DISP_RXD   P06:DISP_TXD    P07:CMOD
    */
    P0CON = _1101_1111; // 0/1 输入/输出
    P0PH = _0010_0000;  // 0/1 不使能上拉电阻/使能能上拉电阻
    /*
        P10:SW1      P11:DEBUG_C    P12:LED8    P13:DEBUG_D
        P14:NC       P15:NC   P16:NC    P17:NC
    */
    P1CON = _1111_1101;
    P1PH = _0000_0010;
    /*
        P20:LED7  P21:LED6    P22:LED5   P23:LED4
        P24:LED3      P25:LED9      P26:LED10      P27:SW7
    */
    P2CON = _1111_1111;
    P2PH = _0000_0000;
    /*
        P30:SW2      P31:SW3        P32:SW4   P33:SW5
        P34:NC       P35:NC         P36:NC   P37:NC
    */
    P3CON = _1111_1111;
    P3PH = _0000_0000;
    /*
        P40:LED2       P41:LED1       P42:AD_COMP   P43:DATA
        P44:WIFI_TXD      P45:WIFI_RXD
    */
    P4CON = _1101_1011;
    P4PH = _0011_0000;
    /*
        P50:OSC_I     P51:OSC_O      P52:CLK   P53:BUZZ
    */
    P5CON = _1111_1111;
    P5PH = _0000_1000;

    // IO口输出电平设置
    P0 = _1111_1111;
    P1 = _1111_1111;
    P2 = _1111_1111;
    P3 = _1111_1111;
    P4 = _1111_1111;
    P5 = _1101_1111;
}

/*****************************************************
 *函数名称：void ADC_Init(uint Channel)
 *函数功能：ADC初始化
 *入口参数：void
 *出口参数：Channel
 *****************************************************/
void ADC_Init_Channel(UI08 _channel)
{
    ADCCON = 0X80 | _channel; //开启ADC电源模块 选择Channel位ADC采样口
    if (_channel < 8)
    {
        ADCCFG1 = 0;
        ADCCFG0 = 1 << _channel;
    }
    else
    {
        ADCCFG0 = 0;
        ADCCFG1 = 1 << (_channel - 8);
    }
    //
    START_ADC(); //开始ADC转换
}

void SYS_Tick_Time(void)
{
    //主时钟 8M

    /*
      TMCON |= bit0; //------111 ;Timer0 选择时钟Fsys
      //T0设置
      TMOD |= 0x01;                     //0000 0001;Timer0设置工作方式1
      TL0 = (65536 - (2000 - 1)) % 256; //溢出时间：时钟为Fsys，则2000*（1/Fsys）=125us;
      TH0 = (65536 - (2000 - 1)) / 256;
      TR0 = 0;
      ET0 = 1; //定时器0允许
      TR0 = 1; //打开定时器0
    */

    // T3设置
    TXINX = 0x03; //选择定时器T3
    TXMOD = 0x80;
    TXCON = 0x00;                  //设置为16位重载寄存
    RCAPXH = (65536 - 1999) / 256; //溢出时间：时钟为Fsys，则32000*（1/Fsys）=2ms;
    RCAPXL = (65536 - 1999) % 256;
    TRX = 0;
    IE1 |= 0x40; //定时器3允许
    IP1 |= 0X40;
    TRX = 1; //打开定时器3
}

UI16 Get_ADC_Value(void)
{
    UI16 adc_buf;

    adc_buf = ADCVL >> 4;          // ADC值的低四位  1111xxxx
    adc_buf |= (UI16)(ADCVH << 4); //高8bit + 低4bit =12bit
    //
    adc_buf = adc_buf >> 2; //取10位

    return adc_buf;
}

/*****************************************************************************
//名称        : ADC_init
//功能        :
//入口参数    :	无
//出口参数    :	无
//构	建: 	GCE
//修	改: 	GCE
*****************************************************************************/
void ADC_Device_Init(UI08 _channel)
{
    ADCCFG2 = _0000_0101; // Tadc2=(36+14)/fadc 采样时钟频率   ADC时钟频率- fHRC/12
    // ADC的参考电压为VDD(配置字中设置)
    ADC_Init_Channel(_channel);
}

/*
void IAP_Reset()
{
    EA = 0;
    IAPKEY = 0X0; //不使能IAP
    //IAPCTL &= ~0x04;//BTLD控制位清0，下次复位后进入APROM
    PCON |= 0X08; //软件复位
    _nop_();      //等待8个_nop_()
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}
*/
/*****************************************************
 *函数名称：void BTM_Int(void) interrupt 9
 *函数功能：中断函数 1S定时
 *入口参数：void
 *出口参数：void
 *****************************************************/
// void BTM_Int(void) interrupt 9
// {
//     if (!(BTMCON & 0X40)) //中断标志位判断
//     {
//         s_bit.word = 0xffff;
//     }
// }

/**************************************************
 *函数名称：void timer0()interrupt 1
 *函数功能：定时器中断服务函数  125us
 *入口参数：void
 *出口参数：void
 **************************************************/
void timer0(void) interrupt 1
{
    TL0 = (65536 - (2000 - 1)) % 256; //溢出时间：时钟为Fsys，则1000*（1/Fsys）=125us;  主频8M
    TH0 = (65536 - (2000 - 1)) / 256;
    //

    // S_Tick_IRQ();
}

void Timer3Int(void) interrupt 13
{
    TXINX = 0x03; //选择定时器T3
    TFX = 0;      //溢出清零

    SYS_Tick_IRQ();
}
