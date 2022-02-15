#include "General.h"

GCE_XDATA UI16 S_Display_Out_Buf = 0; //�������ʾ�������

// *****************************************************************************
// �������� : Write_Disp_LED
// ����˵�� : д��LED��������
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա :
// �����Ա :
// ������� :
// �޸ļ�¼ : 2021.5.13  V1.0�״η���
// *****************************************************************************
void Write_Disp_LED_Data(UI16 _disp_buf)
{
    S_Display_Out_Buf = _disp_buf;
}

// *****************************************************************************
// �������� : SEGNum_out
// ����˵�� : LED IO���
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
static void SEGNum_out(UI16 _data_buf)
{
    if (_data_buf & bit0)
    {
        SEG_A_ON;
    }
    else
    {
        SEG_A_OFF;
    }

    if (_data_buf & bit1)
    {
        SEG_B_ON;
    }
    else
    {
        SEG_B_OFF;
    }

    if (_data_buf & bit2)
    {
        SEG_C_ON;
    }
    else
    {
        SEG_C_OFF;
    }

    if (_data_buf & bit3)
    {
        SEG_D_ON;
    }
    else
    {
        SEG_D_OFF;
    }

    if (_data_buf & bit4)
    {
        SEG_E_ON;
    }
    else
    {
        SEG_E_OFF;
    }

    if (_data_buf & bit5)
    {
        SEG_F_ON;
    }
    else
    {
        SEG_F_OFF;
    }

    if (_data_buf & bit6)
    {
        SEG_G_ON;
    }
    else
    {
        SEG_G_OFF;
    }

    if (_data_buf & bit7)
    {
        SEG_H_ON;
    }
    else
    {
        SEG_H_OFF;
    }

    if (_data_buf & bit8)
    {
        SEG_I_ON;
    }
    else
    {
        SEG_I_OFF;
    }

    if (_data_buf & bit9)
    {
        SEG_J_ON;
    }
    else
    {
        SEG_J_OFF;
    }
}

// *****************************************************************************
// �������� : Led_Scan
// ����˵�� : LED ɨ��
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
void Led_Scan(void)
{
    static UI08 s_led_scan_time = 0; // LEDɨ��ʱ��

    s_led_scan_time++;
    if (s_led_scan_time < led_scan_interval)
    {
        return;
    }
    s_led_scan_time = 0;

    SEGNum_out(S_Display_Out_Buf);
}
