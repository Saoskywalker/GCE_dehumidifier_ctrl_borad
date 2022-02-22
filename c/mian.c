//#define TOUCH_DYN_DEBUG     //!动态调试触摸，需添加SC95F8X1X_HighSensitiveTKDynamicDebug_S_V0.0.2.LIB
#include "General.h"

#ifdef TOUCH_DYN_DEBUG
#include "SOC_DebugTouchKey.h"
#endif

// *****************************************************************************
// 函数名称 : main
// 功能说明 : main函数
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
void main(void)
{
    Sys_Initial();
#ifdef TOUCH_DYN_DEBUG
    SOCAPI_DeBugTouchKey_Init(); //动态库调试
#endif
    //上电全显
    while (G_Power_Delay_Time > 0)
    {

        if (_5mS_For_SYS)
        {
            WDTCON |= 0x10; // WDT_Clear
            LED_Display();
            // Temp_Deal();
            Key_Deal();
            General_Deal();
            Communication_Deal();
            CLEAR_SYS_TIME_BIT();
        }
    }
    // 用户数据读取
    Sys_Data_Read_EEP();

    while (1)
    {

        if (_5mS_For_SYS) //固定时间运行, 避免数码管闪烁(因数码管扫描间隔不一致)
        {
            WDTCON |= 0x10; // WDT_Clear
            LED_Display();
            Key_Deal();
            Temp_Deal();
            General_Deal();
            EEP_deal();
            IO_Operate();
            Sys_Control();
            Communication_Deal();
            WIFI_Deal();
            CLEAR_SYS_TIME_BIT();
        }
    }
}
