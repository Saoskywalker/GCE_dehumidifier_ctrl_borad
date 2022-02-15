#ifndef _COMMUNICATION_API_H
#define _COMMUNICATION_API_H

#include "DataType.h"

extern GCE_XDATA UI08 G_Communication_Time;
//
extern GCE_XDATA UUI08 Comm_Bit;
#define _Sleep_status Comm_Bit.bit_.b1     // ˯��״̬
#define _Sleep_status_buf Comm_Bit.bit_.b2 // ˯��״̬buf
#define _INFAN_ERR Comm_Bit.bit_.b3        // �ڷ������
#define _OUTFAN_ERR Comm_Bit.bit_.b4       // ��������

extern GCE_XDATA TE_ErrorStatus Comm_Status;

#define GET_COM_STATUS() Comm_Status

extern UI08 CRC_Check(UI08 *buffer, UI08 length);
extern void Communication_Deal(void);

#endif
