#include "General.h"

// *****************************************************************************
// �������� : Key_Decode
// ����˵�� : �������ܴ���
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
static void Key_Decode(void)
{
	UI08 key_num = 0;
	TE_FuncState invalid_key_buf = DISABLE;

	if (!_10mS_For_SYS)
	{
		return;
	}

	if (G_SYS_Self_Test)
	{
		return;
	}

	key_num = Get_Key_Data(); //��ȡ����ֵ
	Clear_Key_Data;

	invalid_key_buf = Get_Invalid_Key(key_num); //��Ч�����жϼ�����
	if (invalid_key_buf == ENABLE)
	{
		return;
	}

	//�������ܵ���
	switch (key_num)
	{
	case POWER_KEY:
	{
		Set_Power_Status();
	}
	break;

	case SET_TIME_KEY:
	{
		Set_In_Time();
	}
	break;

	case UP_KEY:
	{
		Up_Key_Function();
	}
	break;

	case DOWN_KEY:
	{
		Down_Key_Function();
	}
	break;

	case PUMP_KEY:
	{
		Set_Pump_Status();
	}
	break;

	case MODE_KEY:
	{
		Set_SYS_Mode();
	}
	break;

	case FAN_KEY:
	{
		Set_FAN_Tyde();
	}
	break;

	case FAST_TEST_KEY: //���
	{
		Set_In_Fast_Test();
	};
	break;
	case COMP_TEST_KEY:
	{
		Set_In_Comp_Test();
	}
	break;
	case WIFI_RESET_KEY:
	{
		Set_RESET_WIFI();
	}
	break;
	case WIFI_TEST_KEY:
	{
		Set_TEST_WIFI();
	}
	break;
		//
	default:
	{
	}
	break;
	}
}

// *****************************************************************************
// �������� : Key_Deal
// ����˵�� : ��������
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
void Key_Deal(void)
{
	Dev_Get_Key_Number();

	Key_Decode();
}

/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
