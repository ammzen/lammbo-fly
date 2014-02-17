/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_systick.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������SysTick�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_systick.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
/* ---------------------- SysTick registers bit mask -------------------- */
/* CTRL TICKINT Mask */
#define CTRL_TICKINT_Set      ((u32)0x00000002)
#define CTRL_TICKINT_Reset    ((u32)0xFFFFFFFD)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: SysTick_CLKSourceConfig
* ��������: ����SysTick��ʱ��Դ.
* �������: SysTick_CLKSource:SysTick��ʱ��Դ��
*                    �����������ѡ�����ֵ֮һ:
*                       - SysTick_CLKSource_HCLK_Div8: SysTickʱ��ԴΪAHBʱ�ӵ�1/8.
*                       - SysTick_CLKSource_HCLK: SysTickʱ��ԴΪAHBʱ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SysTick_CLKSourceConfig(u32 SysTick_CLKSource)
{
  /* Check the parameters [������]*/
  assert_param(IS_SYSTICK_CLK_SOURCE(SysTick_CLKSource));

  if (SysTick_CLKSource == SysTick_CLKSource_HCLK)
  {
    SysTick->CTRL |= SysTick_CLKSource_HCLK;
  }
  else
  {
    SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;
  }
}

/*******************************************************************************
* ��������: SysTick_SetReload
* ��������: ����SysTick������ֵ.
* �������: Reload��SysTick����һ����ֵ���������������1��0x00FFFFFF֮��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SysTick_SetReload(u32 Reload)
{
  /* Check the parameters [������]*/
  assert_param(IS_SYSTICK_RELOAD(Reload));

  SysTick->LOAD = Reload;
}

/*******************************************************************************
* ��������: SysTick_CounterCmd
* ��������: ʹ�ܻ�ȡ��SysTick����.
* �������: SysTick_Counter: SysTick����������״̬��
*                    �����������ѡ�����ֵ֮һ:
*                       - SysTick_Counter_Disable: ��ֹ������
*                       - SysTick_Counter_Enable: ���������
*                       - SysTick_Counter_Clear: ����������
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SysTick_CounterCmd(u32 SysTick_Counter)
{
  /* Check the parameters [������]*/
  assert_param(IS_SYSTICK_COUNTER(SysTick_Counter));

  if (SysTick_Counter == SysTick_Counter_Enable)
  {
    SysTick->CTRL |= SysTick_Counter_Enable;
  }
  else if (SysTick_Counter == SysTick_Counter_Disable) 
  {
    SysTick->CTRL &= SysTick_Counter_Disable;
  }
  else /* SysTick_Counter == SysTick_Counter_Clear */
  {
    SysTick->VAL = SysTick_Counter_Clear;
  }    
}

/*******************************************************************************
* ��������: SysTick_ITConfig
* ��������: ʹ�ܻ�ȡ��SysTick�ж�.
* �������: NewState:SysTick�жϵ���״̬���������������ENABLE����DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SysTick_ITConfig(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    SysTick->CTRL |= CTRL_TICKINT_Set;
  }
  else
  {
    SysTick->CTRL &= CTRL_TICKINT_Reset;
  }
}

/*******************************************************************************
* ��������: SysTick_GetCounter
* ��������: ��ȡSysTick��������ֵ.
* �������: ��
* �������: ��
* ���ز���: SysTick�ĵ�ǰֵ
*******************************************************************************/
u32 SysTick_GetCounter(void)
{
  return(SysTick->VAL);
}

/*******************************************************************************
* ��������: SysTick_GetFlagStatus
* ��������: ����ض���SysTick����Ƿ�����.
* �������: SysTick_FLAG:�����ı��.
*                    �����������ѡ�����ֵ֮һ:
*                       - SysTick_FLAG_COUNT
*                       - SysTick_FLAG_SKEW
*                       - SysTick_FLAG_NOREF
* �������: ��
* ���ز���: ��
*******************************************************************************/
FlagStatus SysTick_GetFlagStatus(u8 SysTick_FLAG)
{
  u32 statusreg = 0, tmp = 0 ;
  FlagStatus bitstatus = RESET;

  /* Check the parameters [������]*/
  assert_param(IS_SYSTICK_FLAG(SysTick_FLAG));

  /* Get the SysTick register index [ȡ��SysTick�Ĵ�����־]*/
  tmp = SysTick_FLAG >> 3;

  if (tmp == 2) /* The flag to check is in CTRL register [���CTRL�Ĵ�����־]*/
  {
    statusreg = SysTick->CTRL;
  }
  else          /* The flag to check is in CALIB register [���CALIB�Ĵ�����־]*/
  {
    statusreg = SysTick->CALIB;
  }

  if ((statusreg & ((u32)1 << SysTick_FLAG)) != (u32)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
