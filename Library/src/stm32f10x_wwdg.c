/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_wwdg.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: This file provides all the WWDG firmware functions.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_wwdg.h"
#include "stm32f10x_rcc.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
/* ----------- WWDG registers bit address in the alias region ----------- */
#define WWDG_OFFSET       (WWDG_BASE - PERIPH_BASE)

/* Alias word address of EWI bit */
#define CFR_OFFSET        (WWDG_OFFSET + 0x04)
#define EWI_BitNumber     0x09
#define CFR_EWI_BB        (PERIPH_BB_BASE + (CFR_OFFSET * 32) + (EWI_BitNumber * 4))

/* --------------------- WWDG registers bit mask ------------------------ */
/* CR register bit mask */
#define CR_WDGA_Set       ((u32)0x00000080)

/* CFR register bit mask */
#define CFR_WDGTB_Mask    ((u32)0xFFFFFE7F)
#define CFR_W_Mask        ((u32)0xFFFFFF80)

#define BIT_Mask          ((u8)0x7F)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: WWDG_DeInit
* ��������: ����WWDG��Χ�豸�Ĵ���Ϊ���ǵ�Ĭ������ֵ.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void WWDG_DeInit(void)
{
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, DISABLE);
}

/*******************************************************************************
* ��������: WWDG_SetPrescaler
* ��������: ����WWDGԤ��Ƶ��.
* �������: WWDG_Prescaler��ָ��WWDGԤ��Ƶ��.
*                    ������������������ֵ֮һ:
*                       - WWDG_Prescaler_1: WWDG������ʱ�� = (PCLK1/4096)/1
*                       - WWDG_Prescaler_2: WWDG������ʱ�� = (PCLK1/4096)/2
*                       - WWDG_Prescaler_4: WWDG������ʱ�� = (PCLK1/4096)/4
*                       - WWDG_Prescaler_8: WWDG������ʱ�� = (PCLK1/4096)/8
* �������: ��
* ���ز���: ��
*******************************************************************************/
void WWDG_SetPrescaler(u32 WWDG_Prescaler)
{
  u32 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_WWDG_PRESCALER(WWDG_Prescaler));

  /* Clear WDGTB[1:0] bits [��WDGTB[1:0]λ]*/
  tmpreg = WWDG->CFR & CFR_WDGTB_Mask;

  /* Set WDGTB[1:0] bits according to WWDG_Prescaler value [����WWDGԤ��Ƶ����ֵ����WDGTB[1:0]λ]*/
  tmpreg |= WWDG_Prescaler;

  /* Store the new value */
  WWDG->CFR = tmpreg;
}

/*******************************************************************************
* ��������: WWDG_SetWindowValue
* ��������: ����WWDG����ֵ.
* �������: WindowValue��ָ��Ҫ�͵��������ȽϵĴ���ֵ.�����������ֵ����С��0x80.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void WWDG_SetWindowValue(u8 WindowValue)
{
  u32 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_WWDG_WINDOW_VALUE(WindowValue));

  /* Clear W[6:0] bits [��W[6:0]λ]*/
  tmpreg = WWDG->CFR & CFR_W_Mask;

  /* Set W[6:0] bits according to WindowValue value [���մ���ֵ��ֵ����W[6:0]λ]*/
  tmpreg |= WindowValue & BIT_Mask;

  /* Store the new value [�����µ�ֵ]*/
  WWDG->CFR = tmpreg;
}

/*******************************************************************************
* ��������: WWDG_EnableIT
* ��������: ʹ��WWDG��ǰ�����ж�.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void WWDG_EnableIT(void)
{
  *(vu32 *) CFR_EWI_BB = (u32)ENABLE;
}

/*******************************************************************************
* ��������: WWDG_SetCounter
* ��������: ����WWDG��������ֵ.
* �������: Counter:��ָ�����Ź���������ֵ�������������0x40��0x70֮���һ����.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void WWDG_SetCounter(u8 Counter)
{
  /* Check the parameters [������]*/
  assert_param(IS_WWDG_COUNTER(Counter));

  /* Write to T[6:0] bits to configure the counter value[Ϊ���ü�������ֵдT[6:0]λ], no need to do
     a read-modify-write[����Ҫ��-�޸�-д]; writing a 0 to WDGA bit does nothing [����д0��WDGAλ�������κ���]*/
  WWDG->CR = Counter & BIT_Mask;
}

/*******************************************************************************
* ��������: WWDG_Enable
* ��������: ʹ��WWDG�������������ֵ.
* �������: ��������ָ�����Ź���������ֵ�������������0x40��0x7F֮���һ����.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void WWDG_Enable(u8 Counter)
{
  /* Check the parameters [������]*/
  assert_param(IS_WWDG_COUNTER(Counter));

  WWDG->CR = CR_WDGA_Set | Counter;
}

/*******************************************************************************
* ��������: WWDG_GetFlagStatus
* ��������: �����ǰ�����жϱ���Ƿ���λ.
* �������: ��
* �������: ��
* ���ز���: ��ǰ�����жϱȽϵ���״̬����λ��λ��
*******************************************************************************/
FlagStatus WWDG_GetFlagStatus(void)
{
  return (FlagStatus)(WWDG->SR);
}

/*******************************************************************************
* ��������: WWDG_ClearFlag
* ��������: �����ǰ�����жϱ��
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void WWDG_ClearFlag(void)
{
  WWDG->SR = (u32)RESET;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
