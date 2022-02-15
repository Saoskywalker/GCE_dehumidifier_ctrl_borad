/**
 ******************************************************************************
 * @file IO_API.h
 *
 ******************************************************************************
 *改文件输入为DO_out[],将其输出到IO口,并返回DI_status,及每个输入输出的持续时间
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

extern GCE_XDATA TS_DI_Para_Def G_Water_Full_Para;        //水箱水位检测
extern GCE_XDATA TS_DI_Para_Def G_Pump_Full_Sw_Para;      //副水箱水位检测
extern GCE_XDATA TS_DI_Para_Def G_Pump_water_pipe_Status; //水泵水管接入状态

#define GET_WATER_FULL_IO_STATUS(void) G_Water_Full_Para.Status            //获取水满状态
#define WRITE_WATER_FULL_IO_STATUS_BUF(a) G_Water_Full_Para.Status_buf = a //写入水满状态

#define GET_PUMP_FULL_IO_STATUS(void) G_Pump_Full_Sw_Para.Status            //获取副水箱水位状态
#define WRITE_PUMP_FULL_IO_STATUS_BUF(a) G_Pump_Full_Sw_Para.Status_buf = a //写入副水箱水位状态

#define GET_PUMP_WATER_PIPE_STATUS(void) G_Pump_water_pipe_Status.Status            //获取水泵水管接入状态
#define WRITE_PUMP_WATER_PIPE_STATUS_BUF(a) G_Pump_water_pipe_Status.Status_buf = a //写入水泵水管接入状态

#endif /* __IO_API_H */
