#include "General.h"

GCE_XDATA TS_DI_Para_Def G_Water_Full_Para;		   //水箱水位检测
GCE_XDATA TS_DI_Para_Def G_Pump_Full_Sw_Para;	   //副水箱水位检测
GCE_XDATA TS_DI_Para_Def G_Pump_water_pipe_Status; //水泵水管接入状态

// *****************************************************************************
// 函数名称 : IO_Data_Init
// 功能说明 : 数据初始化
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
void IO_Data_Init(void)
{
	G_Water_Full_Para.Status = DI_SHORT;
	G_Water_Full_Para.short_time = 0;
	G_Water_Full_Para.cut_time = 0;

	G_Pump_Full_Sw_Para.Status = DI_SHORT;
	G_Pump_Full_Sw_Para.short_time = 0;
	G_Pump_Full_Sw_Para.cut_time = 0;

	G_Pump_water_pipe_Status.Status = DI_SHORT;
	G_Pump_water_pipe_Status.short_time = 0;
	G_Pump_water_pipe_Status.cut_time = 0;
}

// *****************************************************************************
// 函数名称 : IO_Filter_Logic
// 功能说明 : IO滤波
// 入口参数 : io_num IO结构体地址 cut_time    开路滤波次数            short_time  断路滤波次数
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
static void IO_Filter_Logic(TS_DI_Para_Def *_io_num, UI16 _cut_time, UI16 _short_time)
{
	if (_io_num->Status_buf == DI_CUT)
	{
		if (_io_num->cut_time < 0xffff)
		{
			_io_num->cut_time++;
		}
		_io_num->short_time = 0;

		if (_io_num->cut_time >= _cut_time) //滤波时间到
		{
			_io_num->Status = DI_CUT;
		}
	}
	else
	{
		if (_io_num->short_time < 0xffff)
		{
			_io_num->short_time++;
		}
		_io_num->cut_time = 0;

		if (_io_num->short_time >= _short_time)
		{
			_io_num->Status = DI_SHORT;
		}
	}
}

// *****************************************************************************
// 函数名称 : Read_IO_Status
// 功能说明 : 读IO状态
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
static void Read_IO_Status(void)
{
	if (!_10mS_For_SYS)
	{
		return;
	}

	IO_Filter_Logic(&G_Water_Full_Para, 1, 1);

	IO_Filter_Logic(&G_Pump_Full_Sw_Para, 1, 1);

	IO_Filter_Logic(&G_Pump_water_pipe_Status, 1, 1);
}

// *****************************************************************************
// 函数名称 : IO_Operate
// 功能说明 : IO功能处理
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
void IO_Operate(void)
{
	Read_IO_Status();
}
/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
