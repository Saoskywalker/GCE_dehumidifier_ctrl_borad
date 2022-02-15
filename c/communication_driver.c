#include "General.h"

GCE_XDATA UI08 S_Txd_Buf[txd_length] = {0}; //通信数据 TXD
UI08 S_Pulse_Cnt = 0;                       //发送bit宽度
UI08 S_Txd_Byte = 0;                        //发送字节
UI08 S_Txd_Bit = 0;                         //发送bit

GCE_XDATA UI08 S_Rxd_Buf[rece_data_lenth] = {0}; //通信数据 RXD

UI08 S_Rxd_Bits = 0;        //接收bit
UI08 S_Rxd_Bytes = 0;       //接收数据长度
UI08 S_Rxd_Width_Count = 0; //接收数据长度

UUI08 S_Communication_Bit = {0}; //通信变量标志位

// *****************************************************************************
// 函数名称 : Write_Txd_Data
// 功能说明 : 写发送数据，并启动数据发送
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 :
// 审核人员 :
// 审核日期 :
// 修改记录 : 2021.5.13  V1.0首次发布
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
// 函数名称 : Read_Rxd_Data
// 功能说明 : 读取接收数据的数据 数据长度为rece_data_lenth 注意定义的数组要≥rece_data_lenth
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 :
// 审核人员 :
// 审核日期 :
// 修改记录 : 2021.5.13  V1.0首次发布
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
// 函数名称 : Read_Rxd_Data_OK
// 功能说明 : 读取接收数据完成标志，完成回传1 未完成回传0
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 :
// 审核人员 :
// 审核日期 :
// 修改记录 : 2021.5.13  V1.0首次发布
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
// 函数名称 : Clear_Read_Rxd_Data_OK
// 功能说明 : 清除接收数据完成标志，完成回传1 未完成回传0
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 :
// 审核人员 :
// 审核日期 :
// 修改记录 : 2021.5.13  V1.0首次发布
// *****************************************************************************
void Clear_Read_Rxd_Data_OK(void)
{
    _Rxd_OK = 0;
}

// *****************************************************************************
// 函数名称 : Communication_Txd_IRQ
// 功能说明 : 发送协议处理
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
static void Communication_Txd_IRQ(void)
{
    S_Pulse_Cnt++;

    if (_txd_data) //发数据
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
    else if (_txd_start) //发头码
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
// 函数名称 : Communication_Rxd_IRQ
// 功能说明 : 接收协议处理
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
// 函数名称 : Communication_INT_Deal
// 功能说明 : 通讯驱动接收和发送处理
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ： 定时中断每0.125mS调用一次
//
// *****************************************************************************
void Communication_INT_Deal(void)
{
    if (_txd_en) //使能发送
    {
        Communication_Txd_IRQ();
    }

    S_Rxd_Width_Count++;
    if (!Rxd_IO)
    {
        if (_Rxd_IO_buf) //下降沿
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
