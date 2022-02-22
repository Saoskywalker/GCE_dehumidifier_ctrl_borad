//#define TOUCH_DYN_DEBUG     //!��̬���Դ����������SC95F8X1X_HighSensitiveTKDynamicDebug_S_V0.0.2.LIB
#include "General.h"

#ifdef TOUCH_DYN_DEBUG
#include "SOC_DebugTouchKey.h"
#endif

// *****************************************************************************
// �������� : main
// ����˵�� : main����
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
void main(void)
{
    Sys_Initial();
#ifdef TOUCH_DYN_DEBUG
    SOCAPI_DeBugTouchKey_Init(); //��̬�����
#endif
    //�ϵ�ȫ��
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
    // �û����ݶ�ȡ
    Sys_Data_Read_EEP();

    while (1)
    {

        if (_5mS_For_SYS) //�̶�ʱ������, �����������˸(�������ɨ������һ��)
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
