#include "General.h"

// xdata UUI16 SYS_FlashAddr;
// xdata UUI16 SYS_FlashData;
// xdata UI08  SYS_flash_en;

UI08 code *IapAddr = 0x00;

/*****************************************************
*�������ƣ�void IAPPageErase(uint Addr,uchar IAPArea)
*�������ܣ�IAPҳ����
*��ڲ�����Addr,IAPArea
IAPArea 0x00��ѡ��ROM������  0x02��ѡ��EEPROM������
Addr    0~0xffff
*���ڲ�����void
*****************************************************/
void IAPPageErase(UI16 Addr, UI08 IAPArea)
{
    bit temp = EA;
    EA = 0;
    IAPADE = IAPArea;

    IAPADH = (unsigned char)((Addr >> 8)); //д��������׵�ַ��λ
    IAPADL = (unsigned char)Addr;          //д��������׵�ַ��λ

    IAPKEY = 0XF0;
    IAPCTL = 0X20;
    IAPCTL |= 0X02;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    IAPADE = 0x00;
    EA = temp;
}
/*****************************************************
*�������ƣ�uchar IAPRead(uchar Addr,uchar IAPArea)
*�������ܣ�IAP����ʼ��
*��ڲ�����Addr,IAPArea
IAPArea 0x00��ѡ��ROM������  0x02��ѡ��EEPROM������
Addr    0~0xffff
*���ڲ�����ReadValue
*****************************************************/
UI08 IAPRead(UI16 Addr, UI08 IAPArea)
{
    UI08 ReadValue = 0x00;
    bit temp = EA;
    EA = 0;
    // IAPKEY = 0XF0;
    IAPADE = IAPArea;
    ReadValue = *(IapAddr + Addr); //��ȡ������
    IAPADE = 0x00;                 // MOVCָ��ROM
    // IAPKEY = 0X00;
    EA = temp;
    return ReadValue;
}

/*****************************************************
*�������ƣ�void IAPWrite(uchar Addr,uchar Value,uchar IAPArea)
*�������ܣ�IAPд��ʼ��
*��ڲ�����Addr,Value,IAPArea
IAPArea 0x00��ѡ��ROM������  0x02��ѡ��EEPROM������
Addr    0~0xffff
Value   д�������
*���ڲ�����void
*****************************************************/
// IAPд����
void IAPWrite(UI16 Addr, UI08 Value, UI08 IAPArea)
{
    bit temp = EA;
    EA = 0;
    IAPADE = IAPArea;                      // IAPArea=0x00��ѡ��ROM������  IAPArea=0x02��ѡ��EEPROM������
    IAPDAT = Value;                        //д������Data
    IAPADH = (unsigned char)((Addr >> 8)); //д���ַ�ĸ�8λ
    IAPADL = (unsigned char)Addr;          //д���ַ�ĵ�8λ
    IAPKEY = 0xF0;                         // IAP������������ʱ��
    IAPCTL = 0X10;                         //ִ�С�д���������������󣬽���flash��̲���
    IAPCTL |= 0x02;                        //ִ�С�д��������������
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    IAPADE = 0x00; // MOVCָ��ROM
    EA = temp;
}
