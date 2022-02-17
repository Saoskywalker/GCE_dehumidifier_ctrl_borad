#include "General.h"

GCE_XDATA UUI08 Comm_Bit = {0};            //��־λ����
GCE_XDATA UI08 G_Communication_Time = 180; //ͨ�Ź���ʱ��

GCE_XDATA TE_ErrorStatus Comm_Status; // ͨѶ����״̬

GCE_XDATA TE_FuncState Comm_TX_HALT_Status = DISABLE; // ����Դ�巢��˯�߱�־
GCE_XDATA TE_FuncState Comm_RX_HALT_Status = DISABLE; // ���յ���Դ��ظ���˯�߱�־

GCE_XDATA UI08 S_Halt_Time = 10; //���ֹͣ��������

// *****************************************************************************
// �������� : Comm_HALT_Logic
// ����˵�� : ֹͣ���Դ��ͨѶ�߼��ж�
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
void Comm_HALT_Logic(void)
{
    if ((G_SYS_Power_Status)          // ����
        || (G_SYS_Self_Test)          //�Լ�
        || (G_Pump_Para.OUT)          //ˮ�ÿ���
        || (G_Time_Run > 0)           //������ʱ
        || (G_Sys_Err.Water_Full)     // ˮ������
        || (G_Sys_Err.temp_room_err)  // ���´���������
        || (G_Sys_Err.temp_coil_err)  // ���´���������
        || (G_Sys_Err.hum_Sensor_err) //ʪ�ȴ���������
        || (G_Sys_Err.temp_comp_err)  //ʪ�ȴ���������
        || (Comm_Status = ERROR)      // ͨ�Ź���
        || (G_Buzz_Time > 0)          // ��������
    )
    {
        S_Halt_Time = 10;
    }

    if (S_Halt_Time > 0)
    {
        Comm_TX_HALT_Status = DISABLE;
    }
    else
    {
        Comm_TX_HALT_Status = ENABLE;
    }
}

// *****************************************************************************
// �������� : Comm_Err_Logic
// ����˵�� : ͨѶ�����߼��ж�
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
void Comm_Err_Logic(void)
{
    if (S_Halt_Time == 0)
    {
        G_Communication_Time = 180;
    }

    if (G_Communication_Time > 0)
    {
        Comm_Status = SUCCESS;
    }
    else
    {
        Comm_Status = ERROR;
    }
}

// *****************************************************************************
// �������� : Communication_1S_Deal
// ����˵�� : ͨѶ����ر�������
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
static void Communication_1S_Deal(void)
{
    if (!_1S_For_For_SYS)
    {
        return;
    }

    if (G_Communication_Time > 0)
    {
        G_Communication_Time--;
    }

    if (S_Halt_Time > 0)
    {
        S_Halt_Time--;
    }
}

// *****************************************************************************
// �������� : CRC_Check
// ����˵�� : 8λCRC_Check
// ��ڲ��� : _buffer  ��Ҫ��У��������׵�ַ
//           _length   ���ݳ���
// ���ڲ��� : 8λCRCֵ
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
UI08 CRC_Check(UI08 *_buffer, UI08 _length)
{
    UI08 crc = 0;
    UI08 loop = 0;
    UI08 loop_1 = 0;
    for (loop = 0; loop < _length; loop++)
    {
        crc ^= _buffer[loop];
        for (loop_1 = 0; loop_1 < 8; loop_1++)
        {
            if ((crc & 1) == 1)
            {
                crc = crc >> 1;
                crc ^= 0x8C;
            }
            else
            {
                crc = crc >> 1;
            }
        }
    }
    return crc;
}

// *****************************************************************************
// �������� : rxd_data_protocl
// ����˵�� : ���յ����ݽ���
// ��ڲ��� : _ui08_data  ���յ��������׵�ַ
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void rxd_data_protocl(UI08 *_ui08_data)
{
    UI16 ui16_buf = 0;
    SHORTCUT_STATUS io_status;

    if (G_SYS_Self_Test)
    {
        G_Communication_Time = 2;
    }
    else
    {
        G_Communication_Time = 180;
    }

    if (*(_ui08_data + 1) & bit0)
    {
        io_status = DI_CUT;
    }
    else
    {
        io_status = DI_SHORT;
    }
    WRITE_WATER_FULL_IO_STATUS_BUF(io_status); //  д��ˮ��IO״̬

    if (*(_ui08_data + 1) & bit1) //
    {
        io_status = DI_SHORT;
    }
    else
    {
        io_status = DI_CUT;
    }
    WRITE_PUMP_FULL_IO_STATUS_BUF(io_status); //��ˮ��IO״̬

    if (*(_ui08_data + 1) & bit5)
    {
        io_status = DI_CUT; //���㵹
    }
    else
    {
        io_status = DI_SHORT;
    }
    WRITE_DUMP_STATUS_BUF(io_status); //�㵹����״̬
    
    if (*(_ui08_data + 1) & bit7)
    {
        io_status = DI_CUT;
    }
    else
    {
        io_status = DI_SHORT;
    }
    WRITE_PUMP_WATER_PIPE_STATUS_BUF(io_status); //ˮ�ܽ���io״̬

    //��Դ��ظ�����ָ��
    if (*(_ui08_data + 1) & bit6)
    {
        Comm_RX_HALT_Status = ENABLE;
    }
    else
    {
        Comm_RX_HALT_Status = DISABLE;
    }

    //���� AD
    ui16_buf = (UI16)(*(_ui08_data + 2) & 0x03);
    ui16_buf = (ui16_buf << 8) + *(_ui08_data + 3);
    WRITE_ROOM_TEMP_AD(ui16_buf);

    //���� AD
    ui16_buf = (UI16)(*(_ui08_data + 2) & 0x0c);
    ui16_buf = (ui16_buf << 6) + *(_ui08_data + 4);
    ui16_buf >>= 1; //��������, ��Ϊ9λ
    WRITE_COIL_TEMP_AD(ui16_buf);

    // hum  AD
    ui16_buf = (UI16)(*(_ui08_data + 2) & 0x30);
    ui16_buf = (ui16_buf << 4) + *(_ui08_data + 5);
    WRITE_ROOM_HUM_AD(ui16_buf);

    WRITE_ROOM_HUM_BUF(*(_ui08_data + 6)); //��ת����ʪ��ֵ

    //ѹ�����¶�AD
    ui16_buf = (UI16)(*(_ui08_data + 2) & 0xC0);
    ui16_buf = (ui16_buf << 2) + *(_ui08_data + 7);
    ui16_buf >>= 1; //��������, ��Ϊ9λ
    WRITE_COMP_TEMP_AD(ui16_buf);
}

// *****************************************************************************
// �������� : txd_data_protocol
// ����˵�� : ��Ҫ���͵��������
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
static void txd_data_protocol(void)
{
    UUI08 buf = {0};
    UI08 txd_data_buf[txd_length];
    txd_data_buf[0] = 0xaa;
    //
    buf.byte = 0;
    if ((G_SYS_Power_Status) || (G_Pump_Para.OUT))
    {
        buf.bit_.b0 = 1;
    } //���ػ�or pump_para==1
    //
    if (G_Comp_Para.OUT == ON) //ѹ����
    {
        buf.bit_.b1 = 1;
    }

    if (G_Pump_Para.OUT) // ˮ��
    {
        buf.bit_.b5 = 1;
    }

    //˯��/����       bit6
    if (Comm_TX_HALT_Status)
    {
        buf.bit_.b6 = 1;
    }
    //
    if (G_SYS_Self_Test)
    {
        buf.bit_.b7 = 1;
    } //�Լ�

    txd_data_buf[1] = buf.byte;

    //�����λ
    buf.byte = 0;
    buf.bit_.b5 = 1;
    if (G_Fan_Tyde_Out == HIGH_FAN)
    {
        buf.bit_.b0 = 1;
        buf.bit_.b1 = 1;
    }
    else if (G_Fan_Tyde_Out == MID_FAN)
    {
        buf.bit_.b0 = 0;
        buf.bit_.b1 = 1;
    }
    else if (G_Fan_Tyde_Out == LOW_FAN)
    {
        buf.bit_.b0 = 1;
        buf.bit_.b1 = 0;
    }
    else
    {
        buf.bit_.b0 = 0;
        buf.bit_.b1 = 0;
    }

    txd_data_buf[2] = buf.byte;

    txd_data_buf[3] = 0;

    txd_data_buf[txd_length - 1] = CRC_Check(txd_data_buf, txd_length - 1);

    Write_Txd_Data(txd_data_buf, txd_length);
}

// *****************************************************************************
// �������� : Communication_Txd
// ����˵�� : ���������ж�
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
static void Communication_Txd(void)
{
    if (!_500mS_For_SYS)
    {
        return;
    }
    //
    if ((Comm_TX_HALT_Status) && (Comm_RX_HALT_Status))
    {
        return;
    }
    txd_data_protocol();
}

// *****************************************************************************
// �������� : Communication_Rxd
// ����˵�� : �������ݸ�ʽ�ж�
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
static void Communication_Rxd(void)
{
    UI08 ui08_crc = 0;
    UI08 i = 0;
    UI08 data_buf[rece_data_lenth];

    if (!Read_Rxd_Data_OK()) //��ȡ�����Ƿ�������
    {
        return;
    }

    Clear_Read_Rxd_Data_OK(); //������ݽ������
    //
    Read_Rxd_Data(data_buf); //��ȡ���յ�������

    if (data_buf[0] != 0xaa) //
    {
        return;
    }

    ui08_crc = CRC_Check(data_buf, rece_data_lenth - 1);
    //

    if (data_buf[rece_data_lenth - 1] != ui08_crc)
    {
        return;
    }
    //
    rxd_data_protocl(data_buf);
}

// *****************************************************************************
// �������� : Communication_Deal
// ����˵�� : ͨѶ����ܴ���
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
void Communication_Deal(void)
{
    Communication_1S_Deal();
    Comm_HALT_Logic();
    Comm_Err_Logic();
    Communication_Rxd();
    Communication_Txd();
}
