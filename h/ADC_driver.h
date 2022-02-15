/**
 ******************************************************************************
 * @file GCE_adc.h
 * @brief This file contains all HW registers definitions and memory mapping.
 * @author STMicroelectronics - MCD Application Team
 * @version V1.1.1
 * @date 2010-8-19
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_driver_H
#define __ADC_driver_H
//
//==========================================================================================

//==========================================================================================
#define Tcomp_channel 14

#define ADC_channel_define Tcomp_channel
#define ADC_channel_total 1

extern GCE_XDATA UI16 G_ADC_Value[4]; //?ÇôADÔ«
#define ROOM_TEMP_AD G_ADC_Value[0]
#define COIL_TEMP_AD G_ADC_Value[1]
#define HUM_AD G_ADC_Value[2]
#define COMP_TEMP_AD G_ADC_Value[3]

extern void ADC_Init(void);
extern void ADC_Sample(void);

#define GET_ROOM_TEMP_AD(void) ROOM_TEMP_AD
#define WRITE_ROOM_TEMP_AD(a) ROOM_TEMP_AD = a

#define GET_COIL_TEMP_AD(void) COIL_TEMP_AD
#define WRITE_COIL_TEMP_AD(a) COIL_TEMP_AD = a

#define GET_ROOM_HUM_AD(void) HUM_AD
#define WRITE_ROOM_HUM_AD(a) HUM_AD = a

#define GET_COMP_TEMP_AD(void) COMP_TEMP_AD
#define WRITE_COMP_TEMP_AD(a) COMP_TEMP_AD = a

#endif
