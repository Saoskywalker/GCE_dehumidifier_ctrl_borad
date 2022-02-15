#ifndef _Led_driver_H
#define _Led_driver_H

#include "DataType.h"

#define COM_total 3
#define led_scan_interval 2

extern void Led_Scan(void);

extern void Write_Disp_LED_Data(UI16 disp_buf);

#endif
//////////////////////////////////////////////////////////
//
//      eeeeeeeeee       n           nn      ddddddddd
//     ee               nnn         nn      dd       dd
//    ee               nn   n      nn      dd        dd
//   eeeeeeeeee       nn     n    nn      dd         dd
//  ee               nn       n  nn      dd         dd
// ee               nn         nnn      dd         dd
// eeeeeeeeee       nn          nn      ddddddddddd
//
// Designed by caozhongfu 2011.03.30
///////////////////////////////////////////////////////////
/******** (C) COPYRIGHT 2011 GCE -----END OF FILE ********/
