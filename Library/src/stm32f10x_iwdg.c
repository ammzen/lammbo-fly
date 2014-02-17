/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_iwdg.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: This file provides all the IWDG firmware functions.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_iwdg.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
/* ---------------------- IWDG registers bit mask ------------------------ */
/* KR register bit mask */
#define KR_KEY_Reload    ((u16)0xAAAA)
#define KR_KEY_Enable    ((u16)0xCCCC)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: IWDG_WriteAccessCmd
* ��������: ʹ�ܣ�����ܣ���IWDG_PR��IWDG_RLR�Ĵ�����д����.
* �������: IWDG_WriteAccess: ��IWDG_PR��IWDG_RLR�Ĵ�����д���ʵ��µ�״̬��
*                    ������������������ֵ֮һ:
*                       - IWDG_WriteAccess_Enable: ��IWDG_PR��IWDG_RLR�Ĵ�����д���ʱ�ʹ��
*                       - IWDG_WriteAccess_Disable: ��IWDG_PR��IWDG_RLR�Ĵ�����д���ʱ���ֹ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void IWDG_WriteAccessCmd(u16 IWDG_WriteAccess)
{
  /* Check the parameters [������]*/
  assert_param(IS_IWDG_WRITE_ACCESS(IWDG_WriteAccess));

  IWDG->KR = IWDG_WriteAccess;
}

/*******************************************************************************
* ��������: IWDG_SetPrescaler
* ��������: ����IWDGԤ��Ƶֵ.
* �������: IWDG_Prescaler: IWDGԤ��Ƶ�Ĵ�����ֵ.
*                    ������������������ֵ֮һ:
*                       - IWDG_Prescaler_4: IWDGԤ��Ƶ��������Ϊ 4
*                       - IWDG_Prescaler_8: IWDGԤ��Ƶ��������Ϊ 8
*                       - IWDG_Prescaler_16: IWDGԤ��Ƶ��������Ϊ 16
*                       - IWDG_Prescaler_32: IWDGԤ��Ƶ��������Ϊ 32
*                       - IWDG_Prescaler_64: IWDGԤ��Ƶ��������Ϊ 64
*                       - IWDG_Prescaler_128: IWDGԤ��Ƶ��������Ϊ 128
*                       - IWDG_Prescaler_256: IWDGԤ��Ƶ��������Ϊ 256
* �������: ��
* ���ز���: ��
*******************************************************************************/
void IWDG_SetPrescaler(u8 IWDG_Prescaler)
{
  /* Check the parameters [������]*/
  assert_param(IS_IWDG_PRESCALER(IWDG_Prescaler));

  IWDG->PR = IWDG_Prescaler;
}

/*******************************************************************************
* ��������: IWDG_SetReload
* ��������: ����IWDG������ֵ.
* �������: Reload: IWDG���ؼĴ�����ֵ��
*                    �������һ������'0'��'0x0FFF'֮�����.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void IWDG_SetReload(u16 Reload)
{
  /* Check the parameters [������]*/
  assert_param(IS_IWDG_RELOAD(Reload));

  IWDG->RLR = Reload;
}

/*******************************************************************************
* ��������: IWDG_ReloadCounter
* ��������: ͨ�������ؼĴ����ж����˵�ֵ����IWDG����������IWDG_PR��IWDG_RLR�Ĵ�����д���ʱ����ܣ�.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void IWDG_ReloadCounter(void)
{
  IWDG->KR = KR_KEY_Reload;
}

/*******************************************************************************
* ��������: IWDG_Enable
* ��������: ʹ��IWDG�����ܶ�IWDG_PR��IWDG_RLR�Ĵ�����д���ʣ�.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void IWDG_Enable(void)
{
  IWDG->KR = KR_KEY_Enable;
}

/*******************************************************************************
* ��������: IWDG_GetFlagStatus
* ��������: ����ָ����IWDG��־�Ƿ���λ.
* �������: IWDG_FLAG:���ı�־��
*                    ������������������ֵ֮һ:
*                       - IWDG_FLAG_PVU: Ԥ��Ƶ����ֵ������
*                       - IWDG_FLAG_RVU: ����ֵ������
* �������: ��
* ���ز���: IWDG_FLAG�µ�״̬����λ��λ��.
*******************************************************************************/
FlagStatus IWDG_GetFlagStatus(u16 IWDG_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [������]*/
  assert_param(IS_IWDG_FLAG(IWDG_FLAG));

  if ((IWDG->SR & IWDG_FLAG) != (u32)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* Return the flag status [����״̬��־]*/
  return bitstatus;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/