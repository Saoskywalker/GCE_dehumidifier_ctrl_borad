/********************************************************************************
  // �ļ�����:   LedDsp_API.h
  // ��������:   �������������
  //            (����Ӳ���豸���������ݶ�д��Э��ʵ�֣���������ķ�װ���ӿ����)
*********************************************************************************/
#ifndef _LedDsp_API_H
#define _LedDsp_API_H

#include "DataType.h"
#include "Led_driver.h"
//==============================================================================
//      2) ��������(��Ҫ���ñ���)
//==============================================================================
//�������ʾ���ݶ���
#define BIT_A bit7 //          A
#define BIT_B bit4 //       -------
#define BIT_C bit2 //      |       |
#define BIT_D bit3 //    F |       |  B
#define BIT_E bit1 //       ---G---
#define BIT_F bit6 //      |       |  C
#define BIT_G bit5 //    E |       |
#define BIT_P bit0 //       ---D---   P
/******************************************************************************
 *         �������ʾ���붨��
 *******************************************************************************/
#define DATA_0 BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F
#define DATA_1 BIT_B | BIT_C
#define DATA_2 BIT_A | BIT_B | BIT_D | BIT_E | BIT_G
#define DATA_3 BIT_A | BIT_B | BIT_C | BIT_D | BIT_G
#define DATA_4 BIT_B | BIT_C | BIT_F | BIT_G
#define DATA_5 BIT_A | BIT_C | BIT_D | BIT_F | BIT_G
#define DATA_6 BIT_A | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_7 BIT_A | BIT_B | BIT_C
#define DATA_8 BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_9 BIT_A | BIT_B | BIT_C | BIT_D | BIT_F | BIT_G
#define DATA_A BIT_A | BIT_B | BIT_C | BIT_E | BIT_F | BIT_G
#define DATA_b BIT_C | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_C BIT_A | BIT_D | BIT_E | BIT_F
#define DATA_E BIT_A | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_F BIT_A | BIT_E | BIT_F | BIT_G
#define DATA_I BIT_E | BIT_F
#define DATA_i BIT_E
#define DATA_n BIT_A | BIT_B | BIT_C | BIT_E | BIT_F
#define DATA_o BIT_C | BIT_D | BIT_E | BIT_G
#define DATA_t BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_e BIT_A | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_U BIT_B | BIT_C | BIT_D | BIT_E | BIT_F
#define DATA_P BIT_A | BIT_B | BIT_E | BIT_F | BIT_G
#define DATA_L BIT_D | BIT_E | BIT_F
#define DATA_H BIT_B | BIT_C | BIT_E | BIT_F | BIT_G
#define DATA_r BIT_E | BIT_G
#define DATA_neg BIT_G
#define DATA_M1 BIT_A | BIT_B | BIT_E | BIT_F
#define DATA_M2 BIT_A | BIT_B | BIT_C | BIT_F
#define DATA_d BIT_B | BIT_C | BIT_D | BIT_E | BIT_G
#define DATA_y BIT_B | BIT_G | BIT_E | BIT_F
#define DATA_NON 0
//------------------------
extern GCE_XDATA UUI08 S_LED_Data_Buf[COM_total]; // LED��ʾdata
// S_LED_Data_Buf[2]��֮������ʵ�ʲ���ʾ
#define LED_water S_LED_Data_Buf[0].bit_.b0 = 1
#define LED_def S_LED_Data_Buf[0].bit_.b1 = 1

#define LED_HUM_MODE S_LED_Data_Buf[0].bit_.b2 = 1
#define LED_DYR_MODE S_LED_Data_Buf[0].bit_.b3 = 1

#define LED_FAN_LOW S_LED_Data_Buf[0].bit_.b6 = 1
#define LED_FAN_MID S_LED_Data_Buf[0].bit_.b5 = 1
#define LED_FAN_HIGH S_LED_Data_Buf[0].bit_.b4 = 1

#define LED_WIFI S_LED_Data_Buf[0].bit_.b7 = 1

#define LED_timer S_LED_Data_Buf[1].bit_.b0 = 1
#define LED_PUMP S_LED_Data_Buf[1].bit_.b1 = 1

extern GCE_XDATA UI08 S_CT1642_Display_Data_Buf[4];
#define dig1_num S_CT1642_Display_Data_Buf[0]
#define dig2_num S_CT1642_Display_Data_Buf[1]

extern GCE_XDATA UI08 G_Disp_Machine_Temp_Time; //������ʱ����ʾ���¹��µ�ʱ��

extern GCE_XDATA UI16 S_Comp_Test_Disp_En_Time; //����ѹ����ǿ�Ʋ���ģʽʱ ȫ��ʾʱ��

//==============================================================================
//      3) ��������(����ӿ�)
//==============================================================================

extern void LED_Display(void);

extern void Disp_All(void);

extern void dig1_2_dsp(UI08 _disp_data);

#endif
