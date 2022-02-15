#include "General.h"

// GCE_XDATA FuncState _Self_Test = DISABLE;

/*************************************************
 // ��������    : io_init
 // ��������    : IO�ڳ�ʼ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void IO_Init(void)
{
    // TK�ϵ�IO����Ϊǿ�������1
    /*
        P00:NC       P01:NC     P02:NC    P03:NC
        P04:SW6    P05:DISP_RXD   P06:DISP_TXD    P07:CMOD
    */
    P0CON = _1101_1111; // 0/1 ����/���
    P0PH = _0010_0000;  // 0/1 ��ʹ����������/ʹ������������
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

    // IO�������ƽ����
    P0 = _1111_1111;
    P1 = _1111_1111;
    P2 = _1111_1111;
    P3 = _1111_1111;
    P4 = _1111_1111;
    P5 = _1101_1111;
}

/*****************************************************
 *�������ƣ�void ADC_Init(uint Channel)
 *�������ܣ�ADC��ʼ��
 *��ڲ�����void
 *���ڲ�����Channel
 *****************************************************/
void ADC_Init_Channel(UI08 _channel)
{
    ADCCON = 0X80 | _channel; //����ADC��Դģ�� ѡ��ChannelλADC������
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
    START_ADC(); //��ʼADCת��
}

void SYS_Tick_Time(void)
{
    //��ʱ�� 8M

    /*
      TMCON |= bit0; //------111 ;Timer0 ѡ��ʱ��Fsys
      //T0����
      TMOD |= 0x01;                     //0000 0001;Timer0���ù�����ʽ1
      TL0 = (65536 - (2000 - 1)) % 256; //���ʱ�䣺ʱ��ΪFsys����2000*��1/Fsys��=125us;
      TH0 = (65536 - (2000 - 1)) / 256;
      TR0 = 0;
      ET0 = 1; //��ʱ��0����
      TR0 = 1; //�򿪶�ʱ��0
    */

    // T3����
    TXINX = 0x03; //ѡ��ʱ��T3
    TXMOD = 0x80;
    TXCON = 0x00;                  //����Ϊ16λ���ؼĴ�
    RCAPXH = (65536 - 1999) / 256; //���ʱ�䣺ʱ��ΪFsys����32000*��1/Fsys��=2ms;
    RCAPXL = (65536 - 1999) % 256;
    TRX = 0;
    IE1 |= 0x40; //��ʱ��3����
    IP1 |= 0X40;
    TRX = 1; //�򿪶�ʱ��3
}

UI16 Get_ADC_Value(void)
{
    UI16 adc_buf;

    adc_buf = ADCVL >> 4;          // ADCֵ�ĵ���λ  1111xxxx
    adc_buf |= (UI16)(ADCVH << 4); //��8bit + ��4bit =12bit
    //
    adc_buf = adc_buf >> 2; //ȡ10λ

    return adc_buf;
}

/*****************************************************************************
//����        : ADC_init
//����        :
//��ڲ���    :	��
//���ڲ���    :	��
//��	��: 	GCE
//��	��: 	GCE
*****************************************************************************/
void ADC_Device_Init(UI08 _channel)
{
    ADCCFG2 = _0000_0101; // Tadc2=(36+14)/fadc ����ʱ��Ƶ��   ADCʱ��Ƶ��- fHRC/12
    // ADC�Ĳο���ѹΪVDD(������������)
    ADC_Init_Channel(_channel);
}

/*
void IAP_Reset()
{
    EA = 0;
    IAPKEY = 0X0; //��ʹ��IAP
    //IAPCTL &= ~0x04;//BTLD����λ��0���´θ�λ�����APROM
    PCON |= 0X08; //�����λ
    _nop_();      //�ȴ�8��_nop_()
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
 *�������ƣ�void BTM_Int(void) interrupt 9
 *�������ܣ��жϺ��� 1S��ʱ
 *��ڲ�����void
 *���ڲ�����void
 *****************************************************/
// void BTM_Int(void) interrupt 9
// {
//     if (!(BTMCON & 0X40)) //�жϱ�־λ�ж�
//     {
//         s_bit.word = 0xffff;
//     }
// }

/**************************************************
 *�������ƣ�void timer0()interrupt 1
 *�������ܣ���ʱ���жϷ�����  125us
 *��ڲ�����void
 *���ڲ�����void
 **************************************************/
void timer0(void) interrupt 1
{
    TL0 = (65536 - (2000 - 1)) % 256; //���ʱ�䣺ʱ��ΪFsys����1000*��1/Fsys��=125us;  ��Ƶ8M
    TH0 = (65536 - (2000 - 1)) / 256;
    //

    // S_Tick_IRQ();
}

void Timer3Int(void) interrupt 13
{
    TXINX = 0x03; //ѡ��ʱ��T3
    TFX = 0;      //�������

    SYS_Tick_IRQ();
}
