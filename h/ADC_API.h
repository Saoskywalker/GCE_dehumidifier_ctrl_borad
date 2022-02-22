#ifndef __ADC_API_H
#define __ADC_API_H

#include "DataType.h"

extern GCE_XDATA TS_ADC_PARA G_Temp_Room;
extern GCE_XDATA TS_ADC_PARA G_Temp_Coil;
extern GCE_XDATA TS_ADC_PARA G_Hum_Para;  //�¶ȱ���
extern GCE_XDATA TS_ADC_PARA G_Temp_Comp; //�¶ȱ���

extern void Adc_Data_Init(void);

extern void Temp_Deal(void);

#define GET_ROOM_TEMP_F(void) G_Temp_Room.value              // ��ȡ���¨H
#define GET_ROOM_TEMP_C(void) G_Temp_Room.C_value            // ��ȡ�������϶�
#define GET_ROOM_TEMP_SENSOR_STATUS(void) G_Temp_Room.status //��ȡ���´���������״̬

#define GET_COIL_TEMP_F(void) G_Temp_Coil.value              // ��ȡ���¨H
#define GET_COIL_TEMP_C(void) G_Temp_Coil.C_value            // ��ȡ�������϶�
#define GET_COIL_TEMP_SENSOR_STATUS(void) G_Temp_Coil.status //��ȡ���´���������״̬

#define GET_ROOM_HUM(void) G_Hum_Para.value                //��ȡʪ��ֵ
#define GET_ROOM_HUM_SENSOR_STATUS(void) G_Hum_Para.status //��ȡʪ�ȴ���������״̬

//#define    GET_Comp_TEMP_F(void)  G_Temp_Comp.value  // ��ȡ���¨H
#define GET_Comp_TEMP_C(void) G_Temp_Comp.C_value            // ��ȡ�������϶�
#define GET_Comp_TEMP_SENSOR_STATUS(void) G_Temp_Comp.status //��ȡ���´���������״̬

#define WRITE_ROOM_HUM_BUF(a) G_Hum_Para.value_buf = a //д������ʪ��ֵ

#endif
