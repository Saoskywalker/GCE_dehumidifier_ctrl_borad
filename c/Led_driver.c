#include "General.h"

GCE_XDATA UI16 S_Display_Out_Buf = 0; //数码管显示输出缓存

// *****************************************************************************
// 函数名称 : Write_Disp_LED
// 功能说明 : 写如LED点亮数据
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 :
// 审核人员 :
// 审核日期 :
// 修改记录 : 2021.5.13  V1.0首次发布
// *****************************************************************************
void Write_Disp_LED_Data(UI16 _disp_buf)
{
    S_Display_Out_Buf = _disp_buf;
}

// *****************************************************************************
// 函数名称 : SEGNum_out
// 功能说明 : LED IO输出
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
// 函数名称 : Led_Scan
// 功能说明 : LED 扫描
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
void Led_Scan(void)
{
    static UI08 s_led_scan_time = 0; // LED扫描时间

    s_led_scan_time++;
    if (s_led_scan_time < led_scan_interval)
    {
        return;
    }
    s_led_scan_time = 0;

    SEGNum_out(S_Display_Out_Buf);
}
