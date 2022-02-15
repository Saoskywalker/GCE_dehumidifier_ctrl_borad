#ifndef _WIFI_API_H
#define _WIFI_API_H

#include "GCE_Data_Type_V2.h"
#include "DataType.h"

#define WIFI_COMM_ERR_TIME (60 * 3) //��WIFIͨ�Ź���ʱ��

typedef struct
{
  ONOFF_STATUS SYS_Power_Status;   //���ػ�״̬
  UI08 SYS_Hum_Set;                //�趨Ŀ��ʪ��
  TU_FAN_Speed_Type SYS_Fan_Tyde;  //�趨���� U_FAN_Speed_Type G_SYS_Fan_Tyde
  TU_SYS_Mode SYS_Mode;            //����ģʽ
  UI08 Room_HUM;                   //����ʪ��
  UI08 SYS_fault;                  //��������
  TE_FuncState Water_Full;         //ˮ��״̬
  TE_FuncState SYS_defrost_status; //��˪״̬
  UI08 SYS_PUMP_status;
} WIFI_UPDATE;
extern GCE_XDATA WIFI_UPDATE Wifi_Updata;

extern GCE_XDATA UUI16 flag_wifi;
#define _Flash_250ms flag_wifi.bit_.b0
#define _Flash_1500ms flag_wifi.bit_.b1
#define WiFi_LED_Locate_buf flag_wifi.bit_.b2
#define _wifi_reset_En flag_wifi.bit_.b3
#define _Wifi_Online_Status flag_wifi.bit_.b4
#define _Wifi_Comm_Err flag_wifi.bit_.b5
#define _Wifi_factory_test flag_wifi.bit_.b6
#define _Self_Test_wifi_TXD_en flag_wifi.bit_.b7 // wifi�ӿ��Լ�
#define _WIFI_10S_check_status flag_wifi.bit_.b8 // 10S������ѯһ��wifi ״̬
#define _all_status_updata_en flag_wifi.bit_.b9  //�ϱ���̨��Ϣ
#define _Self_Test_wifi_err flag_wifi.bit_.b10   // wifi���Լ�
#define _props_run_set flag_wifi.bit_.b11        //��ʱ�����ϱ�
#define _wifi_WakeUp_En flag_wifi.bit_.b12       //�����˯��

extern GCE_XDATA UI08 M_wifi_rssi_dsp;          // wifi �ź�ǿ��
extern GCE_XDATA UI08 all_data_update_point;    //ȫ�������ϱ�ʱ�ļ���ֵ
extern GCE_XDATA UI08 Wifi_Rssi_DSP_delay_time; //

extern void UART_Init(void);
extern void WIFI_Deal(void);
extern void wifi_rssi_Dsp(void);

#endif
