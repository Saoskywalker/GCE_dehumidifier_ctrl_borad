#ifndef __DATA_EEP_H_
#define __DATA_EEP_H_

#include "DataType.h"

#define IapROM 0x00 // 0x00��ѡ��ROM������  0x02��ѡ��EEPROM������

#define IapCheckNum_ADDR (0xffff - 511) //�洢�����Ƿ���������(���һҳ)

#define SYS_DATA_ADDR (0xffff - 1023) //���Ͳ����洢(�����ڶ�)

#define SECTION_SIZE (512) //������С

#define EEP_MAX 20

union U_EEP
{ /* unsigned 16bit RAM */
	UI08 EEP_data[EEP_MAX];
	struct
	{
		UI08 E_Head;			   // 0 ������ʼ
		UI08 E_SYS_Power_Status;   // 1
		UI08 E_SYS_filter_time_hi; // 2
		UI08 E_SYS_filter_time_lo; // 3
		UI08 E_SYS_Mode;		   // 4 ϵͳģʽ
		UI08 E_SYS_Hum_Set;		   // 5 �趨ʪ��Ŀ��ֵ
		UI08 E_SYS_Fan_Tyde;	   // 6 ���з���
		UI08 E_Pump_Status;		   // 7
		UI08 E_buf1;			   // 8
		UI08 E_buf2;			   // 9
		UI08 E_buf3;			   // 10
		UI08 E_buf4;			   // 11
		UI08 E_buf5;			   // 12
		UI08 E_buf6;			   // 13
		UI08 E_buf7;			   // 14
		UI08 E_buf8;			   // 15
		UI08 E_buf9;			   // 16
		UI08 E_buf10;			   // 17
		UI08 E_buf11;			   // 18
		UI08 E_CRC;				   // 19
	} S_data;
};
typedef union U_EEP TU_EEP_Data;

extern GCE_XDATA UI16 EEP_OffSet_DATA_ADDR; //дEEPƫ�Ƶĵ�ַ(0x00~511,һҳΪ512)

extern void Sys_Data_Read_EEP(void);

extern void EEP_deal(void);

#endif
