
#include "General.h"

/* Private defines -----------------------------------------------------------*/
GCE_XDATA UI32 S_Key_Value_Flag = 0; //��ǰ�ְ�����־

GCE_XDATA UI16 S_Key_Data = 0; //��ǰ��������ֵ  δ�˲�

GCE_XDATA UI08 S_Key_New = 0;			  //�¼�ֵ
GCE_XDATA UI08 S_Key_Old = 0;			  //��һ�ΰ���ֵ
GCE_XDATA UI08 S_Key_One_Buf = 0;		  //�˲��󵥰��������ݴ�
GCE_XDATA UI16 S_Key_Last = 0;			  //����������ʱ��
GCE_XDATA UI16 S_Comp_Test_Key_Count = 0; //�����ж�ѹ����ǿ�Ʋ���ģʽ���밴��ʶ��

GCE_XDATA UI08 S_Shake_Count = 0;
GCE_XDATA UI08 G_Key_Number = 0; //�˲���ʶ����İ���

GCE_XDATA UI16 WIFI_Self_Test_delay_time = 0; //���C��B��ǰ5���,�L���L���I5��l��һ��Wifiģ����ģʽָ��

#define KEY_FILTER_COUNT 5 //�������˲���         ��ֵΪ 1~254

//#define  PREE_DOWN  1  //����ʶ��
#define PREE_DOWN 0 //�ɿ�ʶ��

#define TK_TOTAL 7
GCE_CONST UI32 S_TK_Number[TK_TOTAL] = {
	TK_8,  //����1
	TK_7,  //����2
	TK_6,  //����3
	TK_5,  //����4
	TK_4,  //����5
	TK_28, //����6
	TK_23, //����7
};

void Prg_S_Key_Device(void)
{
	if (!_1S_For_For_SYS)
	{
		return;
	}

	if (WIFI_Self_Test_delay_time > 0)
	{
		WIFI_Self_Test_delay_time--;
	}
}

// *****************************************************************************
// �������� : Dev_Read_Key_Data
// ����˵�� : ��ȡδ�˲��İ���״̬
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա :
// �����Ա :
// ������� :
// �޸ļ�¼ : 2021.5.13  V1.0�״η���
// *****************************************************************************
void Dev_Read_Key_Data(void)
{
	UI08 i = 0;
	if (SOCAPI_TouchKeyStatus & 0x80) //��Ҫ����2:  ������ɨ��һ�ֱ�־���Ƿ����TouchKeyScan()һ��Ҫ���ݴ˱�־λ�����
	{
		SOCAPI_TouchKeyStatus &= 0x7f;	   //��Ҫ����3: �����־λ�� ��Ҫ�ⲿ�����
		S_Key_Value_Flag = TouchKeyScan(); //�������ݴ�����
		TouchKeyRestart();				   //������һ��ת��
	}
	else
	{
		return;
	}

	S_Key_Data = 0;
	for (i = 0; i < TK_TOTAL; i++)
	{
		if (S_TK_Number[i] & S_Key_Value_Flag)
		{
			S_Key_Data |= (0x01 << i);
		}
	}
}

// *****************************************************************************
// �������� : Dev_Get_Key_Number
// ����˵�� : ����ʶ��
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
void Dev_Get_Key_Number(void)
{
	UI08 i = 0;
	UI08 key_total_count = 0;
	UI08 number = 0;

	Prg_S_Key_Device();

	//
	if (_10mS_For_SYS) //���ݲ�ͬ�����ײ㣬��������ʱ��
	{
		Dev_Read_Key_Data(); //���õײ��ȡδ�˲��İ���ֵ
	}

	if (!_10mS_For_SYS)
	{
		return;
	}

	if (S_Key_Last < 0xffff)
	{
		S_Key_Last++;
	}

	if (S_Comp_Test_Key_Count < 0xffff)
	{
		S_Comp_Test_Key_Count++;
	}

	for (i = 0; i < 8; i++)
	{
		if (S_Key_Data & (UI08)(0x01 << i))
		{
			key_total_count++;
			number = i + 1;
		}
	}

	if (key_total_count == 0)
	{
		if (S_Shake_Count == KEY_FILTER_COUNT)
		{
			G_Key_Number = S_Key_One_Buf;
		}

		S_Shake_Count = 0;
		S_Key_Last = 0;
		return;
	}
	else if (key_total_count == 1) //������
	{
		S_Key_New = number;
	}
	else if (key_total_count == 2) //˫��������ϰ���
	{
		S_Shake_Count = 0xff;
	}
	else //ͬʱ����3������3�����ϰ�����������
	{
		S_Key_New = 0;
		S_Key_Last = 0;
		S_Shake_Count = 0xff;
		return;
	}
	/////////////////////////////////////////////////////////////////////////////////
	if (S_Key_New != S_Key_Old)
	{
		S_Key_Old = S_Key_New;
		if (key_total_count == 1)
		{
			if (S_Shake_Count < KEY_FILTER_COUNT)
			{
				S_Shake_Count = 0;
			}
			else
			{
				S_Shake_Count = 0xff;
			}
		}
		S_Key_Last = 0;
		return;
	}

	//������
	if (S_Shake_Count < KEY_FILTER_COUNT)
	{
		S_Shake_Count++;
		if ((S_Shake_Count >= KEY_FILTER_COUNT) && (key_total_count == 1))
		{
			S_Key_One_Buf = S_Key_Old;
#if PREE_DOWN
			G_Key_Number = S_Key_Old;
			S_Shake_Count = 0xff;
#endif
		}
	}

	// ����������˫���� ����ʶ��
	/////////////////////////////////////////////////////////////////////////////////
	if ((S_Key_Last == 300) && (S_Key_Data == ((0x01 << (DOWN_KEY - 1)) | (0x01 << (SET_TIME_KEY - 1)))))
	{
		G_Key_Number = FAST_TEST_KEY;
		S_Shake_Count = 0xff;
	}

	if ((S_Key_Last == 1000) && (S_Key_Data == (0x01 << (POWER_KEY - 1))) && (S_Comp_Test_Key_Count > S_Key_Last) && ((S_Comp_Test_Key_Count - S_Key_Last) > 10) //�ϵ�ǰ100������Ϊ��Ч
		&& ((S_Comp_Test_Key_Count - S_Key_Last) < (300 - 10))																									 //��Ҫ���ϵ�ȫ��ʾ�ڼ�
	)
	{
		G_Key_Number = COMP_TEST_KEY;
		S_Shake_Count = 0xff;
	}

	//������Դ��
	// wifi��λ/����ģʽ�л�    ��������״̬��-wifi��λ δ����״̬���л�������ʽ
	if ((S_Key_Last == 500) && (S_Key_Data == (0x01 << (POWER_KEY - 1))))
	{
		G_Key_Number = WIFI_RESET_KEY;
		S_Shake_Count = 0xff;
	}

	//����wifiģ�����
	/*
	���C��B��ǰ5���,�L���L���I(SW2)5��l��һ��Wifiģ����ģʽָ��
	�B�Ӯa�z�~̖�ɹ���,Wifiָʾ���c��,,�p�˟��@ʾ��̖����.�������ģ�K�~̖��Ϣ�K�C�_�֏�Ĭ�J�O��.
	�˳��l��:1.���Դ�I�_�C����
			 2.�M��a�zģʽ�r�g���^3���.
	 ���Sģʽ�r,��̖���60,���c��Wifiָʾ��.
	*/
	if ((WIFI_Self_Test_delay_time > 0) && (S_Key_Data == (0x01 << (FAN_KEY - 1))))
	{
		WIFI_Self_Test_delay_time = WIFI_SELF_TEST_IN_TIMER;
		if (S_Key_Last == 500)
		{
			WIFI_Self_Test_delay_time = 0;

			G_Key_Number = WIFI_TEST_KEY;
			S_Shake_Count = 0xff;
		}
	}
}
