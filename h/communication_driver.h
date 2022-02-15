#ifndef _COMMUNICATION_DRIVER_H
#define _COMMUNICATION_DRIVER_H

#include "DataType.h"

#define rece_data_lenth 10
#define txd_length 5

extern UUI08 S_Communication_Bit;
#define _txd_en S_Communication_Bit.bit_.b0     //发送使能
#define _txd_start S_Communication_Bit.bit_.b1  //开始发送
#define _txd_end S_Communication_Bit.bit_.b2    //发送结束
#define _txd_data S_Communication_Bit.bit_.b3   //发送数据
#define _Rxd_start S_Communication_Bit.bit_.b4  //开始接收
#define _Rxd_OK S_Communication_Bit.bit_.b5     //接收完成
#define _txd_tick S_Communication_Bit.bit_.b6   //发送间隔
#define _Rxd_IO_buf S_Communication_Bit.bit_.b7 //接收数据的IO buf

extern void Communication_INT_Deal(void);

extern UI08 Write_Txd_Data(UI08 *ui08_data, UI08 len);
extern void Read_Rxd_Data(UI08 *ui08_data);
extern UI08 Read_Rxd_Data_OK(void);
extern void Clear_Read_Rxd_Data_OK(void);

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
// Designed by Benkye.Zhang 2010.10.28
///////////////////////////////////////////////////////
