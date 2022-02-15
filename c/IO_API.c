#include "General.h"

GCE_XDATA TS_DI_Para_Def G_Water_Full_Para;		   //ˮ��ˮλ���
GCE_XDATA TS_DI_Para_Def G_Pump_Full_Sw_Para;	   //��ˮ��ˮλ���
GCE_XDATA TS_DI_Para_Def G_Pump_water_pipe_Status; //ˮ��ˮ�ܽ���״̬

// *****************************************************************************
// �������� : IO_Data_Init
// ����˵�� : ���ݳ�ʼ��
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
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
// �������� : IO_Filter_Logic
// ����˵�� : IO�˲�
// ��ڲ��� : io_num IO�ṹ���ַ cut_time    ��·�˲�����            short_time  ��·�˲�����
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
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

		if (_io_num->cut_time >= _cut_time) //�˲�ʱ�䵽
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
// �������� : Read_IO_Status
// ����˵�� : ��IO״̬
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
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
// �������� : IO_Operate
// ����˵�� : IO���ܴ���
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
void IO_Operate(void)
{
	Read_IO_Status();
}
/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
