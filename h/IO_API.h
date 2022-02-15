/**
 ******************************************************************************
 * @file IO_API.h
 *
 ******************************************************************************
 *���ļ�����ΪDO_out[],���������IO��,������DI_status,��ÿ����������ĳ���ʱ��
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IO_API_H
#define __IO_API_H

#include "GCE_Data_Type_V2.h"

#define DO_total 6

extern void prg_s_IO(void);

extern void IO_Operate(void);
extern void IO_Data_Init(void);

extern GCE_XDATA TS_DI_Para_Def G_Water_Full_Para;        //ˮ��ˮλ���
extern GCE_XDATA TS_DI_Para_Def G_Pump_Full_Sw_Para;      //��ˮ��ˮλ���
extern GCE_XDATA TS_DI_Para_Def G_Pump_water_pipe_Status; //ˮ��ˮ�ܽ���״̬

#define GET_WATER_FULL_IO_STATUS(void) G_Water_Full_Para.Status            //��ȡˮ��״̬
#define WRITE_WATER_FULL_IO_STATUS_BUF(a) G_Water_Full_Para.Status_buf = a //д��ˮ��״̬

#define GET_PUMP_FULL_IO_STATUS(void) G_Pump_Full_Sw_Para.Status            //��ȡ��ˮ��ˮλ״̬
#define WRITE_PUMP_FULL_IO_STATUS_BUF(a) G_Pump_Full_Sw_Para.Status_buf = a //д�븱ˮ��ˮλ״̬

#define GET_PUMP_WATER_PIPE_STATUS(void) G_Pump_water_pipe_Status.Status            //��ȡˮ��ˮ�ܽ���״̬
#define WRITE_PUMP_WATER_PIPE_STATUS_BUF(a) G_Pump_water_pipe_Status.Status_buf = a //д��ˮ��ˮ�ܽ���״̬

#endif /* __IO_API_H */
