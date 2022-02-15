#include "General.h"

GCE_XDATA UI08 S_CT1642_Scan_Time = 0;                                 //显示间隔变量
GCE_XDATA UI08 S_CT1642_Scan_Position = 0;                             //当前显示的是哪位数码管
GCE_XDATA UI08 S_CT1642_Disp_Num = 4;                                  //有多少位数码管显示
GCE_XDATA UI08 S_CT1642_Display_Out_Buf[4] = {0xFF, 0xFF, 0X00, 0X00}; //发送给数码管显示的数据

// *****************************************************************************
// 函数名称 : CT1642_Delay
// 功能说明 : CT1642通讯使用的延时程序
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
static void CT1642_Delay(UI16 _i)
{
     while (_i)
     {
          _i--;
     }
}

// *****************************************************************************
// 函数名称 : CT1642_Send_Byte
// 功能说明 : CT1642发送一字节数据
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
static void CT1642_Send_Byte(UI08 _data_buf)
{
     UI08 i = 0;
     for (i = 0; i < 8; i++) //发送8位地址//位
     {
          CT1642_CLK_CLR;
          CT1642_Delay(LOW_TIMER); //
          if ((_data_buf << i) & 0x80)
          {
               CT1642_DATA_SET;
          }
          else
          {
               CT1642_DATA_CLR;
          }
          CT1642_Delay(LOW_TIMER);
          CT1642_CLK_SET; //上升沿读取
          CT1642_Delay(HI_TIMER);
     }
}

// *****************************************************************************
// 函数名称 : CT1642_Send_Data
// 功能说明 : CT1642发送一帧数据
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
static void CT1642_Send_Data(UI08 _data_buf, UI08 _COM)
{
     UI08 sent_com_buf;
     switch (_COM)
     {
     case 0:
          sent_com_buf = 0xEf;
          break;
     case 1:
          sent_com_buf = 0xDf;
          break;
     case 2:
          sent_com_buf = 0xBf;
          break;
     case 3:
          sent_com_buf = 0x7f;
          break;
     default:
     {
          sent_com_buf = 0xff;
     }
     break;
     }
     CT1642_Send_Byte(sent_com_buf); //发送4bit位选 + 4bit空位
                                     ///发送两个空位  //数据结构  4 bit位选 + 6 bit 空位 + 8bit段数据 =18bit
     CT1642_CLK_CLR;
     CT1642_Delay(LOW_TIMER); //
     CT1642_DATA_CLR;
     CT1642_Delay(LOW_TIMER);
     CT1642_CLK_SET;
     CT1642_Delay(HI_TIMER);
     CT1642_CLK_CLR; //第二bit
     CT1642_Delay(HI_TIMER);
     CT1642_CLK_SET;
     CT1642_Delay(HI_TIMER);
     CT1642_Send_Byte(_data_buf);
}

// *****************************************************************************
// 函数名称 : CT1642_Lock_Data
// 功能说明 : CT1642锁存数据
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
static void CT1642_Lock_Data(void)
{
     CT1642_CLK_SET;          // clk=1;
     CT1642_Delay(LOW_TIMER); //
     CT1642_DATA_CLR;         // da=0;
     CT1642_Delay(HI_TIMER);
     CT1642_DATA_SET; // da=1;
     CT1642_Delay(HI_TIMER);
}

// *****************************************************************************
// 函数名称 : CT1642_Enable_Out_Data
// 功能说明 : CT1642使能数据输出
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
static void CT1642_Enable_Out_Data(void)
{
     CT1642_DATA_CLR; // da=0;
     CT1642_Delay(HI_TIMER);
     CT1642_CLK_CLR; // clk=0;
     CT1642_Delay(HI_TIMER);
     CT1642_DATA_SET;        // da=1;
     CT1642_Delay(HI_TIMER); //
}

/*************************************************
//名称        :	CT1642_Disp_API(void)
//功能        : CT1642显示驱动
//入口参数    :
//出口参数    :	无
//构	建: 	GCE
//修	改: 	GCE XXX

************************************************/
// *****************************************************************************
// 函数名称 : CT1642_Disp_Driver
// 功能说明 : CT1642显示驱动程序
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
// S_CT1642_Display_Out_Buf[0]  为CT1642 COM0显示数据
// S_CT1642_Display_Out_Buf[1]  为CT1642 COM1显示数据
// S_CT1642_Display_Out_Buf[2]  为CT1642 COM2显示数据
// S_CT1642_Display_Out_Buf[3]  为CT1642 COM3显示数据
//
// *****************************************************************************
void CT1642_Disp_Driver(void)
{
     if (S_CT1642_Disp_Num > 4) // com_max最多显示4组数据
     {
          return;
     }

     if (!_5mS_For_SYS)
     {
          return;
     }

     S_CT1642_Scan_Time++;
     if (S_CT1642_Scan_Time < 1)
     {
          return;
     }
     S_CT1642_Scan_Time = 0;

     S_CT1642_Scan_Position++;
     if (S_CT1642_Scan_Position >= S_CT1642_Disp_Num) // 0 1 2 ---3位COM
     {
          S_CT1642_Scan_Position = 0;
     }

     CT1642_Send_Data(S_CT1642_Display_Out_Buf[S_CT1642_Scan_Position], S_CT1642_Scan_Position);
     CT1642_Lock_Data();
     CT1642_Enable_Out_Data();
}

// *****************************************************************************
// 函数名称 : Write_CT1642_Disp_Data
// 功能说明 : 写数码管显示数据
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 :
// 审核人员 :
// 审核日期 :
// 修改记录 : 2021.5.13  V1.0首次发布
// 备注     ： 需要已转为7段码
//    a 对应bit0
//    b 对应bit1
//    c 对应bit2
//    如此类推
// *****************************************************************************
void Write_CT1642_Disp_Data(UI08 *_CT1642_display_data, UI08 _com_max)
{
     UI08 i;
     if (_com_max > 4)
     {
          return;
     }

     for (i = 0; i < _com_max; i++)
     {
          S_CT1642_Display_Out_Buf[i] = *_CT1642_display_data;
          _CT1642_display_data++;
     }
     S_CT1642_Disp_Num = _com_max;
}
