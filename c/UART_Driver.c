#include "General.h"

xdata UI08 uart_tx_len = 0; //串口发发送数据长度
xdata UI08 uart_tx_byte = 0;
xdata UI08 uart_tx_OK = 1;

#define UART_TX_BUF wifi_uart_tx_buf
#define UART_RX_BUF wifi_uart_rx_buf
/*************************************************
 // 函数名称    : UART0_Driver_Init
 // 功能描述    : 串口初始化
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void UART_Driver_Init(void)
{
    //串口初始化
#if (UartSelect == 0)
    SCON = 0x50; //串口设置为模式2,REN要置1
    // TXINX = 0X02;		//定时器控制寄存器
    TXCON &= 0XCF; // UART0由定时器1产生波特率发生器
    TMCON |= 0X02; //------111 ;Timer0、Tiemr1和Tiemr2选择时钟Fsys
    TMOD &= 0x0f;  // 0000 0001;Timer0设置工作方式1
    TMOD |= 0x10;  // 0000 0001;Timer0设置工作方式1
    TH1 = (Fsoc / UartBaud) / 256;
    TL1 = (Fsoc / UartBaud) % 256;
    TR1 = 0;
    ET1 = 0;
    EUART = 1;
#endif
#if (UartSelect == 1)
    OTCON |= 0X30;
    US0CON0 |= 0X50; //串口设置为10位全双工异步通讯
    US0CON2 = (Fsoc / UartBaud) / 256;
    US0CON1 = (Fsoc / UartBaud) % 256;
#endif
#if (UartSelect == 2)
    OTCON |= 0XC0;
    US1CON0 |= 0X50; //串口设置为10位全双工异步通讯
    US1CON2 = (Fsoc / UartBaud) / 256;
    US1CON1 = (Fsoc / UartBaud) % 256;
#endif
#if (UartSelect == 3)
    TMCON |= 0xC0;
    US2CON0 |= 0X50; //串口设置为10位全双工异步通讯
    US2CON2 = (Fsoc / UartBaud) / 256;
    US2CON1 = (Fsoc / UartBaud) % 256;
    IE2 |= bit1; //使能USCI2中断
#endif
}

/*************************************************
 // 函数名称    : Wifi_UART_Txd_Data
 // 功能描述    : 串口数据的发送
 // 入口参数    : 无
 // 出口参数    : 无
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
