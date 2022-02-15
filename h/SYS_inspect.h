#ifndef _SYS_INSPECT_H
#define _SYS_INSPECT_H

#include "DataType.h"

extern GCE_XDATA UUI08 SYS_Inspect_bit;
extern GCE_XDATA UI16 SYS_Inspect_Key_Data;
extern GCE_XDATA UI16 Comp_Test_Time;
extern GCE_XDATA UI16 Comp_Test_Key_count;
extern GCE_XDATA UI16 Comp_Test_Disp_En_Timer;
extern GCE_XDATA UI08 Comp_test_Delay_ms;
extern GCE_XDATA UI08 Disp_PWM_VALUE_TIMER;

#define _SYS_Inspect_ms SYS_Inspect_bit.bit_.b0
//#define _SYS_Inspect_s               SYS_Inspect_bit.bit_.b1 // �Ƶ�s_bit�ﶨ��
#define _COMP_TEST_EN SYS_Inspect_bit.bit_.b2
#define _Comp_Test_Key_Count_EN SYS_Inspect_bit.bit_.b3
#define _SYS_Inspect_Key_OK SYS_Inspect_bit.bit_.b4
#define _Comp_Test_Key_First_Status SYS_Inspect_bit.bit_.b5

#if 1
#define COMP_TEST_OFF_TIME 60 * 3 // 3���ӹ�
#define COMP_TEST_ON_TIME 60 * 6  // 6���ӿ�
#else
#define COMP_TEST_OFF_TIME 3 // 3���ӹ�
#define COMP_TEST_ON_TIME 6  // 6���ӿ�
#endif
extern void SYS_Inspect_Deal(void);
extern void SYS_Inspect_Disp(void);
extern void disp_pwm_temp(void);
extern void SYS_Inspect_ms_general(void);
extern void Lcd_fill(void);
#endif
