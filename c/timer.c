#include "General.h"

UI08 S_mS5_Cont = 0;     // 5ms����
UI08 S_ms10_Cont = 0;    // 10ms����
UI08 S_mS100_Cont = 0;   // 100ms����
UI08 S_mS250_Cont = 0;   // 250ms����
UI08 S_mS500_Cont = 0;   // 500ms����
UI08 S_S1_Cont = 0;      // 1S����
UI08 S_Minute_Count = 0; //����count

UUI16 G_Time_Bit = {0}; // 1msʱ��Ƭ

// *****************************************************************************
// �������� : Timer_Init
// ����˵�� : ϵͳʱ�䶨ʱ����ʼ��
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

void Timer_Init(void)
{
  SYS_Tick_Time(); // 0.125mS ��ʱ
}

// *****************************************************************************
// �������� : Prg_mS1
// ����˵�� : ʱ��Ƭ��־־��һ
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     �� ��ʱ�� ÿ1mS����һ��
//
// *****************************************************************************
static void Prg_mS1(void) //�˺��������жϳ�����ִ��
{
  if (G_Buzz_Time > 0)
  {
    G_Buzz_Time--;
  }

  S_mS5_Cont++;
  if (S_mS5_Cont >= 5)
  {
    S_mS5_Cont = 0;
    _5mS_For_SYS = 1;
  }

  S_ms10_Cont++;
  if (S_ms10_Cont >= 10)
  {
    _10mS_For_SYS = 1;
    S_ms10_Cont = 0;
    S_mS100_Cont++;
    S_mS250_Cont++;
  }

  if (S_mS250_Cont >= 25)
  {
    S_mS250_Cont = 0;
    _250mS_For_SYS = 1;
  }

  if (S_mS100_Cont >= 10)
  {
    _100mS_For_SYS = 1;
    S_mS100_Cont = 0;
    S_mS500_Cont++;
    if (S_mS500_Cont >= 5)
    {
      S_mS500_Cont = 0;
      _500mS_For_SYS = 1;
    }

    S_S1_Cont++;
    if (S_S1_Cont >= 10)
    {
      S_S1_Cont = 0;
      _1S_For_For_SYS = 1;
    }
  }
}

// *****************************************************************************
// �������� : SYS_Tick_IRQ
// ����˵�� : ϵͳ��ʱ���жϵ��ú���
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��  ÿ0.125mS�ж�һ��
//
// *****************************************************************************
void SYS_Tick_IRQ(void)
{
  static UI08 s_mS_rj = 0;
  if (G_Buzz_Time > 0)
  {
    if (G_Buzzer_IO_Status == RESET)
    {
      BUZZ_ON;
      G_Buzzer_IO_Status = SET;
    }
    else
    {
      BUZZ_OFF;
      G_Buzzer_IO_Status = RESET;
    }
  }
  else
  {
    BUZZ_OFF;
  }

  Communication_INT_Deal();

  //================   1msʱ��Ƭ   =====================================
  if (++s_mS_rj >= 8)
  {
    s_mS_rj = 0;
    Prg_mS1();
    Led_Scan();
  }
}

// *****************************************************************************
// �������� : Timer_Deal
// ����˵�� : ����1���ʱ���־��һ
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
void Timer_Deal(void) //�˺���������������ִ��
{
  if (!_1S_For_For_SYS)
  {
    return;
  }

  S_Minute_Count++;
  if (S_Minute_Count >= 60)
  {
    S_Minute_Count = 0;
    _1Minute_For_SYS = 1;
  }
}