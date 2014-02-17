/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_rtc.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������RTC�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_rtc.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
#define CRL_CNF_Set      ((u16)0x0010)      /* Configuration Flag Enable Mask */
#define CRL_CNF_Reset    ((u16)0xFFEF)      /* Configuration Flag Disable Mask */
#define RTC_LSB_Mask     ((u32)0x0000FFFF)  /* RTC LSB Mask */
#define PRLH_MSB_Mask    ((u32)0x000F0000)  /* RTC Prescaler MSB Mask */

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: RTC_ITConfig
* ��������: ʹ�ܻ�ر�ָ����RTC�ж�.
* �������: (1)RTC_IT:Ҫʹ�ܻ�رյ�RTC�ж�Դ.
*                    �����������������ֵ���������:
*                       - RTC_IT_OW: ����ж�ʹ��
*                       - RTC_IT_ALR: �����ж�ʹ��
*                       - RTC_IT_SEC: �ڶ����ж�ʹ��
*           (2)NewState:ָ��RTC�жϵ���״̬.�������������:ENALBLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RTC_ITConfig(u16 RTC_IT, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_RTC_IT(RTC_IT));  
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    RTC->CRH |= RTC_IT;
  }
  else
  {
    RTC->CRH &= (u16)~RTC_IT;
  }
}

/*******************************************************************************
* ��������: RTC_EnterConfigMode
* ��������: ����RTC����ģʽ.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RTC_EnterConfigMode(void)
{
  /* Set the CNF flag to enter in the Configuration Mode [��λCNF��־�Խ�������ģʽ]*/
  RTC->CRL |= CRL_CNF_Set;
}

/*******************************************************************************
* ��������: RTC_ExitConfigMode
* ��������: �˳�RTC����ģʽ.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RTC_ExitConfigMode(void)
{
  /* Reset the CNF flag to exit from the Configuration Mode [��λCNF��־���˳�����ģʽ]*/
  RTC->CRL &= CRL_CNF_Reset;
}

/*******************************************************************************
* ��������: RTC_GetCounter
* ��������: ���RTC������ֵ.
* �������: ��
* �������: ��
* ���ز���: RTC����ֵ.
*******************************************************************************/
u32 RTC_GetCounter(void)
{
  u16 tmp = 0;
  tmp = RTC->CNTL;

  return (((u32)RTC->CNTH << 16 ) | tmp) ;
}

/*******************************************************************************
* ��������: RTC_SetCounter
* ��������: ����RTC������ֵ.
* �������: CounterValue:RTC������ֵ.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RTC_SetCounter(u32 CounterValue)
{ 
  RTC_EnterConfigMode();

  /* Set RTC COUNTER MSB word [����RTC�������߰��ֽ�]*/
  RTC->CNTH = CounterValue >> 16;
  /* Set RTC COUNTER LSB word [����RTC�������Ͱ��ֽ�]*/
  RTC->CNTL = (CounterValue & RTC_LSB_Mask);

  RTC_ExitConfigMode();
}

/*******************************************************************************
* ��������: RTC_SetPrescaler
* ��������: ���RTCԤ��Ƶ��ֵ.
* �������: PrescalerValue:RTCԤ��Ƶ����ֵ.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RTC_SetPrescaler(u32 PrescalerValue)
{
  /* Check the parameters [������]*/
  assert_param(IS_RTC_PRESCALER(PrescalerValue));
  
  RTC_EnterConfigMode();

  /* Set RTC PRESCALER MSB word [����RTC��Ƶ���߰��ֽ�]*/
  RTC->PRLH = (PrescalerValue & PRLH_MSB_Mask) >> 16;
  /* Set RTC PRESCALER LSB word [����RTC��Ƶ���Ͱ��ֽ�]*/
  RTC->PRLL = (PrescalerValue & RTC_LSB_Mask);

  RTC_ExitConfigMode();
}

/*******************************************************************************
* ��������: RTC_SetAlarm
* ��������: ����RTC����ֵ.
* �������: AlarmValue:RTC������ֵ.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RTC_SetAlarm(u32 AlarmValue)
{  
  RTC_EnterConfigMode();

  /* Set the ALARM MSB word [����ALARM�߰��ֽ�]*/
  RTC->ALRH = AlarmValue >> 16;
  /* Set the ALARM LSB word [����ALARM�Ͱ��ֽ�]*/
  RTC->ALRL = (AlarmValue & RTC_LSB_Mask);

  RTC_ExitConfigMode();
}

/*******************************************************************************
* ��������: RTC_GetDivider
* ��������: ���RTC�ָ�ֵ.
* �������: ��
* �������: ��
* ���ز���: RTC�ָ�ֵ.
*******************************************************************************/
u32 RTC_GetDivider(void)
{
  u32 tmp = 0x00;

  tmp = ((u32)RTC->DIVH & (u32)0x000F) << 16;
  tmp |= RTC->DIVL;

  return tmp;
}

/*******************************************************************************
* ��������: RTC_WaitForLastTask
* ��������: �ȴ�ֱ��RTC�Ĵ�д������ɡ�
*               �������һ��Ҫ���κζ�RTC�Ĵ���д����֮ǰ����.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RTC_WaitForLastTask(void)
{
  /* Loop until RTOFF flag is set [ѭ����RTOFF��־��λ]*/
  while ((RTC->CRL & RTC_FLAG_RTOFF) == (u16)RESET)
  {
  }
}

/*******************************************************************************
* ��������: RTC_WaitForSynchro
* ��������: �ȴ�RTC�Ĵ���(RTC_CNT,RTC_ALR��RTC_PRL)��RTC APBʱ��ͬ����
*               �������һ��Ҫ���κζ�APB��λ��APBʱ��ֹͣ���д����֮ǰ���á�.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RTC_WaitForSynchro(void)
{
  /* Clear RSF flag [��RSF��־]*/
  RTC->CRL &= (u16)~RTC_FLAG_RSF;

  /* Loop until RSF flag is set [ѭ����RSF��־��λ]*/
  while ((RTC->CRL & RTC_FLAG_RSF) == (u16)RESET)
  {
  }
}

/*******************************************************************************
* ��������: RTC_GetFlagStatus
* ��������: ���ָ����RTC��־�������.
* �������: RTC_FLAG:ָ��Ҫ���ı�־.
*                    �����������ѡ�����ֵ֮һ:
*                       - RTC_FLAG_RTOFF: RTC�����رձ�־
*                       - RTC_FLAG_RSF: �Ĵ���ͬ����־
*                       - RTC_FLAG_OW: ����жϱ�־
*                       - RTC_FLAG_ALR: �����жϱ�־
*                       - RTC_FLAG_SEC: �ڶ��жϱ�־
* �������: ��
* ���ز���: RTC_FLAG����״̬(SET��RESET).
*******************************************************************************/
FlagStatus RTC_GetFlagStatus(u16 RTC_FLAG)
{
  FlagStatus bitstatus = RESET;
  
  /* Check the parameters [������]*/
  assert_param(IS_RTC_GET_FLAG(RTC_FLAG)); 
  
  if ((RTC->CRL & RTC_FLAG) != (u16)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/*******************************************************************************
* ��������: RTC_ClearFlag
* ��������: ���RTC�����־.
* �������: RTC_FLAG:��������ı�־��
*                    �����������������ֵ���������:
*                       - RTC_FLAG_RSF: ͬ���Ĵ�����־. ֻ����SPB��λ��APBʱ��ֹ֮ͣ�����..
*                       - RTC_FLAG_OW: �����־
*                       - RTC_FLAG_ALR: ������־
*                       - RTC_FLAG_SEC: �ڶ��α�־
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RTC_ClearFlag(u16 RTC_FLAG)
{
  /* Check the parameters [������]*/
  assert_param(IS_RTC_CLEAR_FLAG(RTC_FLAG)); 
    
  /* Clear the coressponding RTC flag [���coressponding RTC��־]*/
  RTC->CRL &= (u16)~RTC_FLAG;
}

/*******************************************************************************
* ��������: RTC_GetITStatus
* ��������: ���ָ����RTC�жϷ������..
* �������: RTC_IT:Ҫ����RTC�ж�Դ..
*                    �����������ѡ�����ֵ֮һ:
*                       - RTC_IT_OW: ����ж�
*                       - RTC_IT_ALR: �����ж�
*                       - RTC_IT_SEC: �ڶ����ж�
* �������: ��
* ���ز���: RTC_IT����״̬(SET��RESET).
*******************************************************************************/
ITStatus RTC_GetITStatus(u16 RTC_IT)
{
  ITStatus bitstatus = RESET;

  /* Check the parameters [������]*/
  assert_param(IS_RTC_GET_IT(RTC_IT)); 
  
  bitstatus = (ITStatus)(RTC->CRL & RTC_IT);

  if (((RTC->CRH & RTC_IT) != (u16)RESET) && (bitstatus != (u16)RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/*******************************************************************************
* ��������: RTC_ClearITPendingBit
* ��������: ���RTC�жϹ���λ.
* �������: RTC_IT:��Ҫ������жϹ���λ..
*                    �����������������ֵ���������:
*                       - RTC_IT_OW: ����ж�
*                       - RTC_IT_ALR: �����ж�
*                       - RTC_IT_SEC: �ڶ����ж�
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RTC_ClearITPendingBit(u16 RTC_IT)
{
  /* Check the parameters [������]*/
  assert_param(IS_RTC_IT(RTC_IT));  
  
  /* Clear the coressponding RTC pending bit [���coressponding RTC�����־]*/
  RTC->CRL &= (u16)~RTC_IT;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
