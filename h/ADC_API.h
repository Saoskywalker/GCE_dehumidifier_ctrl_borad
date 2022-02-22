#ifndef __ADC_API_H
#define __ADC_API_H

#include "DataType.h"

extern GCE_XDATA TS_ADC_PARA G_Temp_Room;
extern GCE_XDATA TS_ADC_PARA G_Temp_Coil;
extern GCE_XDATA TS_ADC_PARA G_Hum_Para;  //温度变量
extern GCE_XDATA TS_ADC_PARA G_Temp_Comp; //温度变量

extern void Adc_Data_Init(void);

extern void Temp_Deal(void);

#define GET_ROOM_TEMP_F(void) G_Temp_Room.value              // 获取室温℉
#define GET_ROOM_TEMP_C(void) G_Temp_Room.C_value            // 获取室温摄氏度
#define GET_ROOM_TEMP_SENSOR_STATUS(void) G_Temp_Room.status //获取室温传感器故障状态

#define GET_COIL_TEMP_F(void) G_Temp_Coil.value              // 获取管温℉
#define GET_COIL_TEMP_C(void) G_Temp_Coil.C_value            // 获取管温摄氏度
#define GET_COIL_TEMP_SENSOR_STATUS(void) G_Temp_Coil.status //获取管温传感器故障状态

#define GET_ROOM_HUM(void) G_Hum_Para.value                //获取湿度值
#define GET_ROOM_HUM_SENSOR_STATUS(void) G_Hum_Para.status //获取湿度传感器故障状态

//#define    GET_Comp_TEMP_F(void)  G_Temp_Comp.value  // 获取管温℉
#define GET_Comp_TEMP_C(void) G_Temp_Comp.C_value            // 获取管温摄氏度
#define GET_Comp_TEMP_SENSOR_STATUS(void) G_Temp_Comp.status //获取管温传感器故障状态

#define WRITE_ROOM_HUM_BUF(a) G_Hum_Para.value_buf = a //写入室内湿度值

#endif
