#ifndef __MCU_H_
#define __MCU_H_

#include "DataType.h"

/*蜂鸣器IO定义*/
#define BUZZ_ON P53 = 1
#define BUZZ_OFF P53 = 0
////

/*模拟红外通讯IO定义*/
#define Txd_H P06 = 1
#define Txd_L P06 = 0
#define Rxd_IO (P05)
////

/*LED显示IO定义*/
#define SEG_A_ON P41 = 0 // LED1
#define SEG_A_OFF P41 = 1

#define SEG_B_ON P40 = 0 // LED2
#define SEG_B_OFF P40 = 1

#define SEG_C_ON P24 = 0 // LED3
#define SEG_C_OFF P24 = 1

#define SEG_D_ON P23 = 0 // LED4
#define SEG_D_OFF P23 = 1

#define SEG_E_ON P22 = 0 // LED5
#define SEG_E_OFF P22 = 1

#define SEG_F_ON P21 = 0 // LED6
#define SEG_F_OFF P21 = 1

#define SEG_G_ON P20 = 0 // LED7
#define SEG_G_OFF P20 = 1

#define SEG_H_ON P12 = 0 // LED8
#define SEG_H_OFF P12 = 1

#define SEG_I_ON P25 = 0 // LED9
#define SEG_I_OFF P25 = 1

#define SEG_J_ON P26 = 0 // LED10
#define SEG_J_OFF P26 = 1

////

/*数码管驱动芯片CT1642 通讯IO定义*/
#define CT1642_CLK_SET P52 = 1
#define CT1642_CLK_CLR P52 = 0

#define CT1642_DATA_SET P43 = 1
#define CT1642_DATA_CLR P43 = 0

//软件启动ADC单次采样
#define START_ADC() ADCCON |= 0X40

extern void ADC_Init_Channel(UI08 _channel);
extern UI16 Get_ADC_Value(void);
extern void ADC_Device_Init(UI08 _channel);
extern void SYS_Tick_Time(void);

extern void IO_Init(void);

#endif
