#ifndef __TIMER_H__
#define __TIMER_H__

#include "DataType.h"

extern UUI16 G_Time_Bit; //ʱ��Ƭ��־
#define _5mS_For_SYS G_Time_Bit.bit_.b0
#define _10mS_For_SYS G_Time_Bit.bit_.b1
#define _100mS_For_SYS G_Time_Bit.bit_.b2
#define _500mS_For_SYS G_Time_Bit.bit_.b3
#define _1S_For_For_SYS G_Time_Bit.bit_.b4
#define _1Minute_For_SYS G_Time_Bit.bit_.b5
#define _250mS_For_SYS G_Time_Bit.bit_.b6

#define CLEAR_SYS_TIME_BIT() G_Time_Bit.word = 0

extern void Timer_Init(void);

extern void Timer_Deal(void);

extern void SYS_Tick_IRQ(void);

#endif
