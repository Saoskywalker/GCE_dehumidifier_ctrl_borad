#include "General.h"

GCE_XDATA TU_EEP_Data S_Sys_Memory_Data;     // EEP数组
GCE_XDATA TU_EEP_Data S_Sys_Memory_Data_Buf; // EEP校验数组

GCE_XDATA UI16 EEP_OffSet_DATA_ADDR = 0; //写EEP偏移的地址(0x00~511,一页为512)

GCE_XDATA TE_FuncState S_Check_EEP_EN = DISABLE;

GCE_XDATA UI08 S_Write_Memory_Data_Delay = 0;
GCE_XDATA UI32 S_Write_Memory_Data_Flag = 0;

// *****************************************************************************
// 函数名称 : Sys_Data_Write
// 功能说明 : 数据写EEP判断，及写EEP
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
static void Sys_Data_Write(void)
{
    UI08 i;
    UI08 crc = 0;
    // UI08 same_flag = 0;
    UI32 buf_flag = 0;

    if (S_Write_Memory_Data_Delay > 0)
    {
        S_Write_Memory_Data_Delay--;
    }

    for (i = 0; i < EEP_MAX; i++)
    {
        S_Sys_Memory_Data_Buf.EEP_data[i] = 0;
    }

    S_Sys_Memory_Data_Buf.S_data.E_Head = 0XAA;
    S_Sys_Memory_Data_Buf.S_data.E_SYS_filter_time_hi = (UI08)(G_Filter_Run_Time_Buf >> 8);
    S_Sys_Memory_Data_Buf.S_data.E_SYS_filter_time_lo = (UI08)(G_Filter_Run_Time_Buf);
    S_Sys_Memory_Data_Buf.S_data.E_SYS_Power_Status = (UI08)(G_SYS_Power_Status);
    S_Sys_Memory_Data_Buf.S_data.E_SYS_Mode = (UI08)(G_SYS_Mode_Buf);
    S_Sys_Memory_Data_Buf.S_data.E_SYS_Hum_Set = (UI08)G_SYS_Hum_Set_Buf;
    S_Sys_Memory_Data_Buf.S_data.E_SYS_Fan_Tyde = (UI08)G_SYS_Fan_Tyde_Buf;
    S_Sys_Memory_Data_Buf.S_data.E_Pump_Status = (UI08)G_Pump_Status;
    S_Sys_Memory_Data_Buf.S_data.E_Comp_Overtime_Protect_Flag = (UI08)G_Comp_Overtime_Protect_Flag;

    for (i = 0; i < EEP_MAX; i++) //数据对比， 不同则重新写eep
    {
        if (S_Sys_Memory_Data.EEP_data[i] != S_Sys_Memory_Data_Buf.EEP_data[i])
        {
            buf_flag |= 0x00000001 << i;
        }
    }

    if (S_Write_Memory_Data_Flag != buf_flag)
    {
        S_Write_Memory_Data_Flag = buf_flag;
        S_Write_Memory_Data_Delay = 6;
    }

    if ((S_Write_Memory_Data_Delay > 0) || (S_Write_Memory_Data_Flag == 0))
    {
        return;
    }

    S_Sys_Memory_Data_Buf.S_data.E_CRC = CRC_Check(&S_Sys_Memory_Data_Buf.EEP_data[0], EEP_MAX - 1); // CRC校验

    if (EEP_OffSet_DATA_ADDR >= (SECTION_SIZE - EEP_MAX)) // 31*16=496byte(一页512byte)
    {
        IAPPageErase(SYS_DATA_ADDR, IapROM);
        EEP_OffSet_DATA_ADDR = 0;

        return;
    }

    S_Write_Memory_Data_Flag = 0;
    //写入
    for (i = 0; i < EEP_MAX; i++)
    {
        IAPWrite(SYS_DATA_ADDR + i + EEP_OffSet_DATA_ADDR, S_Sys_Memory_Data_Buf.EEP_data[i], IapROM);
    }

    EEP_OffSet_DATA_ADDR = EEP_OffSet_DATA_ADDR + EEP_MAX; //偏移到下一组

    S_Check_EEP_EN = ENABLE;
}

// *****************************************************************************
// 函数名称 : Sys_Data_Check
// 功能说明 : 写入EEP的数据回读并判断
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
static void Sys_Data_Check(void)
{

    UI16 i;
    UI08 crc = 0;
    UI08 check_EEP_data = 0; //检查是否存储有EEP数据
    UI16 check_offset = 0;   // EEP存储的偏移量

    if (S_Check_EEP_EN == DISABLE)
    {
        return;
    }
    S_Check_EEP_EN = DISABLE;

    if (EEP_OffSet_DATA_ADDR >= EEP_MAX)
    {
        check_offset = EEP_OffSet_DATA_ADDR - EEP_MAX;
    }
    else
    {
        check_offset = (SECTION_SIZE - EEP_MAX) / 20;
    }

    //重新读出来校验
    for (i = 0; i < EEP_MAX; i++)
    {
        S_Sys_Memory_Data.EEP_data[i] = IAPRead(SYS_DATA_ADDR + check_offset + i, IapROM);
    }

    crc = CRC_Check(&S_Sys_Memory_Data.EEP_data[0], EEP_MAX - 1); // CRC校验
    //校验   || 写的范围已满  擦除从新开始
    if ((0xaa != S_Sys_Memory_Data.S_data.E_Head) || (crc != S_Sys_Memory_Data.S_data.E_CRC))
    {
        for (i = 0; i < EEP_MAX; i++)
        {
            S_Sys_Memory_Data.EEP_data[i] = 0;
        }
        IAPPageErase(SYS_DATA_ADDR, IapROM);
        EEP_OffSet_DATA_ADDR = 0;
    }
}

// *****************************************************************************
// 函数名称 : Sys_First_Data_Check
// 功能说明 : 从新上电的EEP数据读取处理
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
static void Sys_First_Data_Check(void)
{

    UI16 i;
    UI08 crc = 0;
    UI08 check_EEP_data = 0; //检查是否存储有EEP数据
    UI16 check_offset = 0;   // EEP存储的偏移量

    for (i = 0; i < (SECTION_SIZE - EEP_MAX); i = i + EEP_MAX)
    {
        check_EEP_data = IAPRead(SYS_DATA_ADDR + i, IapROM);
        if (check_EEP_data != 0xAA) //检查第一个字节
        {
            if (i == 0) //没有存过数据
            {
                check_offset = 0;
                break;
            }
            else
            {
                check_offset = i - EEP_MAX;
                break;
            }
        }
        else
        {
            check_offset = i;
        }
    }

    //重新读出来校验
    for (i = 0; i < EEP_MAX; i++)
    {
        S_Sys_Memory_Data.EEP_data[i] = IAPRead(SYS_DATA_ADDR + check_offset + i, IapROM);
    }

    crc = CRC_Check(&S_Sys_Memory_Data.EEP_data[0], EEP_MAX - 1); // CRC校验
    //校验   || 写的范围已满  擦除从新开始
    if ((0xaa != S_Sys_Memory_Data.S_data.E_Head) || (crc != S_Sys_Memory_Data.S_data.E_CRC))
    {
        for (i = 0; i < EEP_MAX; i++)
        {
            S_Sys_Memory_Data.EEP_data[i] = 0;
        }
        IAPPageErase(SYS_DATA_ADDR, IapROM);
        EEP_OffSet_DATA_ADDR = 0;
    }
    else
    {
        EEP_OffSet_DATA_ADDR = check_offset + EEP_MAX;
    }
}

// *****************************************************************************
// 函数名称 : Sys_Data_Read_EEP
// 功能说明 : 从新上电读取EEP数据，并更新系统变量
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
void Sys_Data_Read_EEP(void)
{
    if (!G_Sys_Config.Auto_Restart)
    {
        return;
    }

    Sys_First_Data_Check();

    WIFI_Self_Test_delay_time = WIFI_SELF_TEST_IN_TIMER;

    if (S_Sys_Memory_Data.S_data.E_Head != 0XAA)
    {
        return;
    }

    G_Comp_Overtime_Protect_Flag = (TE_FuncState)S_Sys_Memory_Data.S_data.E_Comp_Overtime_Protect_Flag;

    G_Filter_Run_Time = S_Sys_Memory_Data.S_data.E_SYS_filter_time_hi;
    G_Filter_Run_Time = (G_Filter_Run_Time << 8) + S_Sys_Memory_Data.S_data.E_SYS_filter_time_lo;
    G_Filter_Run_Time_Buf = G_Filter_Run_Time;

    G_SYS_Power_Status = (ONOFF_STATUS)S_Sys_Memory_Data.S_data.E_SYS_Power_Status;
    if (G_SYS_Power_Status == ON)
    {
        G_Comp_Para.off_time = 0;
        WIFI_Self_Test_delay_time = 0;
    }
    else //如不再工作状态则按系统默认
    {
        return;
    }

    G_SYS_Mode = (TU_SYS_Mode)S_Sys_Memory_Data.S_data.E_SYS_Mode; //系统模式
    G_SYS_Mode_Buf = G_SYS_Mode;

    G_SYS_Hum_Set = (UI08)S_Sys_Memory_Data.S_data.E_SYS_Hum_Set; //湿度设定
    G_SYS_Hum_Set_Buf = G_SYS_Hum_Set;

    G_SYS_Fan_Tyde = (TU_FAN_Speed_Type)S_Sys_Memory_Data.S_data.E_SYS_Fan_Tyde; //运行风速
    G_SYS_Fan_Tyde_Buf = G_SYS_Fan_Tyde;

    G_Pump_Status = (TE_FuncState)S_Sys_Memory_Data.S_data.E_Pump_Status; //水泵
}

// *****************************************************************************
// 函数名称 : Sys_Data_Write_2
// 功能说明 : 写入EEP第二区域的数据
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : Aysi-E
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
/* static void Sys_Data_Write_2(void)
{
    UI08 i, crc;
    GCE_XDATA UI08 Sys_Memory_Data[6] = {0}; //数据头部，数据。。。。， 数据校验码

    for (i = 0; i < sizeof(Sys_Memory_Data); i++)
    {
        Sys_Memory_Data[i] = IAPRead(IapCheckNum_ADDR + i, IapROM);
    }

    crc = CRC_Check(&Sys_Memory_Data[0], sizeof(Sys_Memory_Data) - 1); // CRC校验
    //校验   || 写的范围已满  擦除从新开始
    if ((0xaa != Sys_Memory_Data[0]) || (crc != Sys_Memory_Data[5]))
    {
        IAPPageErase(SYS_DATA_ADDR, IapROM);
    }
    else
    {
        G_Comp_Overtime_Protect_Flag = Sys_Memory_Data[1];
    }
} */

// *****************************************************************************
// 函数名称 : Sys_Data_Read_2
// 功能说明 : 读EEP第二区域的数据
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : Aysi-E
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
/* static void Sys_Data_Read_2(void)
{
    UI08 i, j;
    GCE_XDATA UI08 Sys_Memory_Data[6] = {0}; //数据头部，数据。。。。， 数据校验码

    j = IAPRead(IapCheckNum_ADDR + 1, IapROM);
    if (G_Comp_Overtime_Protect_Flag == j)
        return;

    Sys_Memory_Data[0] = 0XAA;
    Sys_Memory_Data[1] = (UI08)G_Comp_Overtime_Protect_Flag;
    Sys_Memory_Data[2] = 0;
    Sys_Memory_Data[3] = 0;
    Sys_Memory_Data[4] = 0;
    Sys_Memory_Data[5] = CRC_Check(&Sys_Memory_Data[0], sizeof(Sys_Memory_Data) - 1); // CRC校验

    //写入
    IAPPageErase(IapCheckNum_ADDR, IapROM);
    for (i = 0; i < sizeof(Sys_Memory_Data); i++)
    {
        IAPWrite(IapCheckNum_ADDR + i, Sys_Memory_Data[i], IapROM);
    }
} */

// *****************************************************************************
// 函数名称 : EEP_deal
// 功能说明 : 运行中的系统数据写EEP处理
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
void EEP_deal(void)
{
    if (!_500mS_For_SYS)
    {
        return;
    }

    if (!G_Sys_Config.Auto_Restart)
    {
        return;
    }

    Sys_Data_Check();
    Sys_Data_Write();
}
