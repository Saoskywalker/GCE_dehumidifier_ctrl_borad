#include "General.h"

GCE_XDATA UI08 S_Txd_Buf[txd_length] = {0}; //ͨ������ TXD
UI08 S_Pulse_Cnt = 0;                       //����bit���
UI08 S_Txd_Byte = 0;                        //�����ֽ�
UI08 S_Txd_Bit = 0;                         //����bit

GCE_XDATA UI08 S_Rxd_Buf[rece_data_lenth] = {0}; //ͨ������ RXD

UI08 S_Rxd_Bits = 0;        //����bit
UI08 S_Rxd_Bytes = 0;       //�������ݳ���
UI08 S_Rxd_Width_Count = 0; //�������ݳ���

UUI08 S_Communication_Bit = {0}; //ͨ�ű�����־λ

// *****************************************************************************
// �������� : Write_Txd_Data
// ����˵�� : д�������ݣ����������ݷ���
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա :
// �����Ա :
// ������� :
// �޸ļ�¼ : 2021.5.13  V1.0�״η���
// *****************************************************************************
UI08 Write_Txd_Data(UI08 *_ui08_data, UI08 _len)
{
    UI08 i = 0;

    if (_len > sizeof(S_Txd_Buf))
    {
        return 0;
    }

    for (i = 0; i < _len; i++)
    {
        S_Txd_Buf[i] = *_ui08_data;
        _ui08_data++;
    }

    S_Pulse_Cnt = 0;
    _txd_data = 0;
    _txd_end = 0;
    _txd_start = 1;
    _txd_en = 1;

    return 0;
}

// *****************************************************************************
// �������� : Read_Rxd_Data
// ����˵�� : ��ȡ�������ݵ����� ���ݳ���Ϊrece_data_lenth ע�ⶨ�������Ҫ��rece_data_lenth
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա :
// �����Ա :
// ������� :
// �޸ļ�¼ : 2021.5.13  V1.0�״η���
// *****************************************************************************
void Read_Rxd_Data(UI08 *_ui08_data)
{
    UI08 i = 0;

    for (i = 0; i < rece_data_lenth; i++)
    {
        *_ui08_data = S_Rxd_Buf[i];
        _ui08_data++;
    }
}

// *****************************************************************************
// �������� : Read_Rxd_Data_OK
// ����˵�� : ��ȡ����������ɱ�־����ɻش�1 δ��ɻش�0
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա :
// �����Ա :
// ������� :
// �޸ļ�¼ : 2021.5.13  V1.0�״η���
// *****************************************************************************
UI08 Read_Rxd_Data_OK(void)
{
    if (_Rxd_OK)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// *****************************************************************************
// �������� : Clear_Read_Rxd_Data_OK
// ����˵�� : �������������ɱ�־����ɻش�1 δ��ɻش�0
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա :
// �����Ա :
// ������� :
// �޸ļ�¼ : 2021.5.13  V1.0�״η���
// *****************************************************************************
void Clear_Read_Rxd_Data_OK(void)
{
    _Rxd_OK = 0;
}

// *****************************************************************************
// �������� : Communication_Txd_IRQ
// ����˵�� : ����Э�鴦��
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
static void Communication_Txd_IRQ(void)
{
    S_Pulse_Cnt++;

    if (_txd_data) //������
    {
        if (S_Txd_Buf[S_Txd_Byte] & S_Txd_Bit)
        {
            if (S_Pulse_Cnt > 15)
            {
                S_Pulse_Cnt = 1;
                S_Txd_Bit <<= 1;
            }
        }
        else
        {
            if (S_Pulse_Cnt > 10)
            {
                S_Pulse_Cnt = 1;
                S_Txd_Bit <<= 1;
            }
        }

        if (0x00 == S_Txd_Bit)
        {
            S_Txd_Bit = 0x01;
            S_Txd_Byte++;
            if (S_Txd_Byte >= txd_length)
            {
                S_Pulse_Cnt = 1;
                S_Txd_Byte = 0;
                _txd_end = 1;
                _txd_data = 0;
                _txd_start = 0;
            }
        }
    }
    else if (_txd_start) //��ͷ��
    {
        if (S_Pulse_Cnt > 20)
        {
            S_Pulse_Cnt = 1;
            S_Txd_Bit = 0x01;
            _txd_start = 0;
            _txd_data = 1;
            _txd_end = 0;
        }
    }
    else if (_txd_end)
    {
        if (S_Pulse_Cnt > 10)
        {
            _txd_start = 0;
            _txd_data = 0;
            _txd_end = 0;
            _txd_en = 0;
        }
    }
    else
    {
        S_Pulse_Cnt = 5;
        _txd_start = 1;
        _txd_data = 0;
        _txd_end = 0;
        _txd_en = 0;
    }

    if (S_Pulse_Cnt <= 5)
    {
        Txd_L;
    }
    else
    {
        Txd_H;
    }
}

// *****************************************************************************
// �������� : Communication_Rxd_IRQ
// ����˵�� : ����Э�鴦��
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
static void Communication_Rxd_IRQ(void)
{
    if ((S_Rxd_Width_Count >= 17) && (S_Rxd_Width_Count <= 23)) // head=40(+-6);
    {
        _Rxd_start = 1;
        S_Rxd_Bits = 0x01;
        S_Rxd_Bytes = 0;
        return;
    }

    if (!_Rxd_start)
    {
        return;
    }

    if (S_Rxd_Bytes >= rece_data_lenth)
    {
        _Rxd_start = 0;
        return;
    }

    if ((S_Rxd_Width_Count >= 13) && (S_Rxd_Width_Count <= 17)) // one=30(+-4)
    {
        S_Rxd_Buf[S_Rxd_Bytes] |= S_Rxd_Bits;
    }
    else if ((S_Rxd_Width_Count >= 8) && (S_Rxd_Width_Count <= 12)) // zero=20(+-4)
    {
        S_Rxd_Buf[S_Rxd_Bytes] &= ~S_Rxd_Bits;
    }
    else
    {
        _Rxd_start = 0;
        return;
    }

    S_Rxd_Bits <<= 1;
    if (0 == S_Rxd_Bits)
    {
        S_Rxd_Bits = 0x01;
        S_Rxd_Bytes++;
        if (S_Rxd_Bytes >= rece_data_lenth)
        {
            _Rxd_OK = 1;
            _Rxd_start = 0;
        }
    }
}

// *****************************************************************************
// �������� : Communication_INT_Deal
// ����˵�� : ͨѶ�������պͷ��ʹ���
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     �� ��ʱ�ж�ÿ0.125mS����һ��
//
// *****************************************************************************
void Communication_INT_Deal(void)
{
    if (_txd_en) //ʹ�ܷ���
    {
        Communication_Txd_IRQ();
    }

    S_Rxd_Width_Count++;
    if (!Rxd_IO)
    {
        if (_Rxd_IO_buf) //�½���
        {
            _Rxd_IO_buf = 0;
            Communication_Rxd_IRQ();
            S_Rxd_Width_Count = 0;
        }
    }
    else
    {
        _Rxd_IO_buf = 1;
    }
}
