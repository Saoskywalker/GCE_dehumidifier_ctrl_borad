#include "General.h"

xdata UI08 uart_tx_len = 0; //���ڷ��������ݳ���
xdata UI08 uart_tx_byte = 0;
xdata UI08 uart_tx_OK = 1;

#define UART_TX_BUF wifi_uart_tx_buf
#define UART_RX_BUF wifi_uart_rx_buf
/*************************************************
 // ��������    : UART0_Driver_Init
 // ��������    : ���ڳ�ʼ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void UART_Driver_Init(void)
{
    //���ڳ�ʼ��
#if (UartSelect == 0)
    SCON = 0x50; //��������Ϊģʽ2,RENҪ��1
    // TXINX = 0X02;		//��ʱ�����ƼĴ���
    TXCON &= 0XCF; // UART0�ɶ�ʱ��1���������ʷ�����
    TMCON |= 0X02; //------111 ;Timer0��Tiemr1��Tiemr2ѡ��ʱ��Fsys
    TMOD &= 0x0f;  // 0000 0001;Timer0���ù�����ʽ1
    TMOD |= 0x10;  // 0000 0001;Timer0���ù�����ʽ1
    TH1 = (Fsoc / UartBaud) / 256;
    TL1 = (Fsoc / UartBaud) % 256;
    TR1 = 0;
    ET1 = 0;
    EUART = 1;
#endif
#if (UartSelect == 1)
    OTCON |= 0X30;
    US0CON0 |= 0X50; //��������Ϊ10λȫ˫���첽ͨѶ
    US0CON2 = (Fsoc / UartBaud) / 256;
    US0CON1 = (Fsoc / UartBaud) % 256;
#endif
#if (UartSelect == 2)
    OTCON |= 0XC0;
    US1CON0 |= 0X50; //��������Ϊ10λȫ˫���첽ͨѶ
    US1CON2 = (Fsoc / UartBaud) / 256;
    US1CON1 = (Fsoc / UartBaud) % 256;
#endif
#if (UartSelect == 3)
    TMCON |= 0xC0;
    US2CON0 |= 0X50; //��������Ϊ10λȫ˫���첽ͨѶ
    US2CON2 = (Fsoc / UartBaud) / 256;
    US2CON1 = (Fsoc / UartBaud) % 256;
    IE2 |= bit1; //ʹ��USCI2�ж�
#endif
}

/*************************************************
 // ��������    : Wifi_UART_Txd_Data
 // ��������    : �������ݵķ���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void UART_Txd_Data(void)
{
    if ((uart_tx_byte < uart_tx_len) && (uart_tx_byte < (sizeof(wifi_uart_tx_buf))))
    {
        UART_SFR = wifi_uart_tx_buf[uart_tx_byte];
        uart_tx_byte++;
        uart_tx_OK = 0;
    }
    else
    {
        uart_tx_OK = 1;
        uart_tx_len = 0;
        uart_tx_byte = 0;
    }
}

void UartInt(void) interrupt 16
{
    if (READ_TI)
    {
        CLEAR_TI;

        if (G_SYS_Self_Test)
            uart_test_send_process();
        else
            UART_Txd_Data();
    }
    if (READ_RI)
    {
        if (G_SYS_Self_Test)
            uart_test_receive_process(UART_SFR);
        else
            uart_receive_input(UART_SFR);

        CLEAR_RI;
    }
}
