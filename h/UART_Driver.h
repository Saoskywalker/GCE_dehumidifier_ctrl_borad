#ifndef _UART_DRIVER_H
#define _UART_DRIVER_H

#include "DataType.h"

#define UartSelect 3        //选择UART:0:UART0,1:USCI0,2:USCI1,3:USCI2
#define UartBaud 115200     //波特率写入
#define Fsoc (32000000 / 2) //主频选择

#if (UartSelect == 0)
#define UART_SFR SBUF
#define READ_RI RI
#define CLEAR_RI RI = 0
#define READ_TI TI
#define CLEAR_TI TI = 0
#endif

#if (UartSelect == 1)
#define UART_SFR US0CON3
#define READ_RI US0CON0 & 0X01
#define CLEAR_RI US0CON0 = US0CON0 & 0XFE
#define READ_TI US0CON0 & 0X02
#define CLEAR_TI US0CON0 = US0CON0 & 0XFD
#endif

#if (UartSelect == 2)
#define UART_SFR US1CON3
#define READ_RI US1CON0 & 0X01
#define CLEAR_RI US1CON0 = US1CON0 & 0XFE
#define READ_TI US1CON0 & 0X02
#define CLEAR_TI US1CON0 = US1CON0 & 0XFD
#endif

#if (UartSelect == 3)
#define UART_SFR US2CON3
#define READ_RI US2CON0 & 0X01
#define CLEAR_RI US2CON0 = US2CON0 & 0XFE
#define READ_TI US2CON0 & 0X02
#define CLEAR_TI US2CON0 = US2CON0 & 0XFD
#endif

extern xdata UI08 uart_tx_len; //串口发发送数据长度
extern xdata UI08 uart_tx_byte;
extern xdata UI08 uart_tx_OK;

extern void UART_Driver_Init(void);

#endif
