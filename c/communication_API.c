#include "General.h"

GCE_XDATA UUI08 Comm_Bit = {0};            //标志位变量
GCE_XDATA UI08 G_Communication_Time = 180; //通信故障时间

GCE_XDATA TE_ErrorStatus Comm_Status; // 通讯故障状态

GCE_XDATA TE_FuncState Comm_TX_HALT_Status = DISABLE; // 给电源板发送睡眠标志
GCE_XDATA TE_FuncState Comm_RX_HALT_Status = DISABLE; // 接收到电源板回复的睡眠标志

GCE_XDATA UI08 S_Halt_Time = 10; //多久停止发送数据

// *****************************************************************************
// 函数名称 : Comm_HALT_Logic
// 功能说明 : 停止与电源板通讯逻辑判断
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
void Comm_HALT_Logic(void)
{
    if ((G_SYS_Power_Status)          // 开机
        || (G_SYS_Self_Test)          //自检
        || (G_Pump_Para.OUT)          //水泵开启
        || (G_Time_Run > 0)           //开启定时
        || (G_Sys_Err.Water_Full)     // 水满保护
        || (G_Sys_Err.temp_room_err)  // 室温传感器故障
        || (G_Sys_Err.temp_coil_err)  // 管温传感器故障
        || (G_Sys_Err.hum_Sensor_err) //湿度传感器故障
        || (G_Sys_Err.temp_comp_err)  //湿度传感器故障
        || (Comm_Status = ERROR)      // 通信故障
        || (G_Buzz_Time > 0)          // 蜂鸣器响
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
// 函数名称 : Comm_Err_Logic
// 功能说明 : 通讯故障逻辑判断
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
// 函数名称 : Communication_1S_Deal
// 功能说明 : 通讯秒相关变量处理
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
// 函数名称 : CRC_Check
// 功能说明 : 8位CRC_Check
// 入口参数 : _buffer  需要做校验的数据首地址
//           _length   数据长度
// 出口参数 : 8位CRC值
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
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
// 函数名称 : rxd_data_protocl
// 功能说明 : 接收的数据解析
// 入口参数 : _ui08_data  接收到的数据首地址
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
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
    WRITE_WATER_FULL_IO_STATUS_BUF(io_status); //  写入水满IO状态

    if (*(_ui08_data + 1) & bit1) //
    {
        io_status = DI_SHORT;
    }
    else
    {
        io_status = DI_CUT;
    }
    WRITE_PUMP_FULL_IO_STATUS_BUF(io_status); //副水箱IO状态

    if (*(_ui08_data + 1) & bit5)
    {
        io_status = DI_CUT; //已倾倒
    }
    else
    {
        io_status = DI_SHORT;
    }
    WRITE_DUMP_STATUS_BUF(io_status); //倾倒开关状态
    
    if (*(_ui08_data + 1) & bit7)
    {
        io_status = DI_CUT;
    }
    else
    {
        io_status = DI_SHORT;
    }
    WRITE_PUMP_WATER_PIPE_STATUS_BUF(io_status); //水管接入io状态

    //电源板回复休眠指令
    if (*(_ui08_data + 1) & bit6)
    {
        Comm_RX_HALT_Status = ENABLE;
    }
    else
    {
        Comm_RX_HALT_Status = DISABLE;
    }

    //室温 AD
    ui16_buf = (UI16)(*(_ui08_data + 2) & 0x03);
    ui16_buf = (ui16_buf << 8) + *(_ui08_data + 3);
    WRITE_ROOM_TEMP_AD(ui16_buf);

    //管温 AD
    ui16_buf = (UI16)(*(_ui08_data + 2) & 0x0c);
    ui16_buf = (ui16_buf << 6) + *(_ui08_data + 4);
    ui16_buf >>= 1; //本程序查表法, 表为9位
    WRITE_COIL_TEMP_AD(ui16_buf);

    // hum  AD
    ui16_buf = (UI16)(*(_ui08_data + 2) & 0x30);
    ui16_buf = (ui16_buf << 4) + *(_ui08_data + 5);
    WRITE_ROOM_HUM_AD(ui16_buf);

    WRITE_ROOM_HUM_BUF(*(_ui08_data + 6)); //已转换的湿度值

    //压缩机温度AD
    ui16_buf = (UI16)(*(_ui08_data + 2) & 0xC0);
    ui16_buf = (ui16_buf << 2) + *(_ui08_data + 7);
    ui16_buf >>= 1; //本程序查表法, 表为9位
    WRITE_COMP_TEMP_AD(ui16_buf);
}

// *****************************************************************************
// 函数名称 : txd_data_protocol
// 功能说明 : 需要发送的数据填充
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
    } //开关机or pump_para==1
    //
    if (G_Comp_Para.OUT == ON) //压缩机
    {
        buf.bit_.b1 = 1;
    }

    if (G_Pump_Para.OUT) // 水泵
    {
        buf.bit_.b5 = 1;
    }

    //睡眠/运行       bit6
    if (Comm_TX_HALT_Status)
    {
        buf.bit_.b6 = 1;
    }
    //
    if (G_SYS_Self_Test)
    {
        buf.bit_.b7 = 1;
    } //自检

    txd_data_buf[1] = buf.byte;

    //风机档位
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
// 函数名称 : Communication_Txd
// 功能说明 : 启动发送判断
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
// 函数名称 : Communication_Rxd
// 功能说明 : 接收数据格式判断
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
static void Communication_Rxd(void)
{
    UI08 ui08_crc = 0;
    UI08 i = 0;
    UI08 data_buf[rece_data_lenth];

    if (!Read_Rxd_Data_OK()) //读取数据是否接收完毕
    {
        return;
    }

    Clear_Read_Rxd_Data_OK(); //清除数据接收完成
    //
    Read_Rxd_Data(data_buf); //读取接收到的数据

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
// 函数名称 : Communication_Deal
// 功能说明 : 通讯相关总处理
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
void Communication_Deal(void)
{
    Communication_1S_Deal();
    Comm_HALT_Logic();
    Comm_Err_Logic();
    Communication_Rxd();
    Communication_Txd();
}
