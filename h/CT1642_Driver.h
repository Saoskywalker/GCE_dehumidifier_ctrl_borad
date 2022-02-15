#ifndef __CT1642_DRIVER_H
#define __CT1642_DRIVER_H

#include "DataType.h"

#define LOW_TIMER 1 // 4
#define HI_TIMER 2  // 8

extern void CT1642_Disp_Driver(void);
extern void Write_CT1642_Disp_Data(UI08 *_CT1642_display_data, UI08 _com_max);

#endif
