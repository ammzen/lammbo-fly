/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_exti.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������EXTI�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_exti.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
#define EXTI_LineNone    ((u32)0x00000)  /* No interrupt selected */

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: EXTI_DeInit
* ��������: ��EXTI����Ĵ�����λ��Ĭ��ֵ.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void EXTI_DeInit(void)
{
  EXTI->IMR = 0x00000000;
  EXTI->EMR = 0x00000000;
  EXTI->RTSR = 0x00000000; 
  EXTI->FTSR = 0x00000000; 
  EXTI->PR = 0x0007FFFF;
}

/*******************************************************************************
* ��������: EXTI_Init
* ��������: ����EXIT_InitStruct��ָ���Ĳ�����ʼ��EXTI����
* �������: EXTI_InitStruct��ָ��EXTI_InitTypeDef�Ľṹ���������ض�EXTI��������Ϣ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct)
{
  /* Check the parameters [������]*/
  assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
  assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
  assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line));  
  assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));
     
  if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)
  {
    /* Clear EXTI line configuration [����ⲿ�ж�������]*/
    EXTI->IMR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->EMR &= ~EXTI_InitStruct->EXTI_Line;
    
    *(vu32 *)(EXTI_BASE + (u32)EXTI_InitStruct->EXTI_Mode)|= EXTI_InitStruct->EXTI_Line;

    /* Clear Rising Falling edge configuration [����������½��ر�������]*/
    EXTI->RTSR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->FTSR &= ~EXTI_InitStruct->EXTI_Line;
    
    /* Select the trigger for the selected external interrupts [ѡ�񴥷���ѡ����ⲿ�ж�]*/
    if (EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_Rising_Falling)
    {
      /* Rising Falling edge [�������½��ر���]*/
      EXTI->RTSR |= EXTI_InitStruct->EXTI_Line;
      EXTI->FTSR |= EXTI_InitStruct->EXTI_Line;
    }
    else
    {
      *(vu32 *)(EXTI_BASE + (u32)EXTI_InitStruct->EXTI_Trigger)|= EXTI_InitStruct->EXTI_Line;
    }
  }
  else
  {
    /* Disable the selected external lines [��ֹѡ�е��ⲿ�ж���]*/
    *(vu32 *)(EXTI_BASE + (u32)EXTI_InitStruct->EXTI_Mode)&= ~EXTI_InitStruct->EXTI_Line;
  }
}

/*******************************************************************************
* ��������: EXTI_StructInit
* ��������: ��EXTI_InitStruct��Ա��ΪĬ��ֵ
* �������: EXTI_InitStruct��ָ��EXTI_InitTypeDef�ṹ���ýṹ������ʼ����
* �������: ��
* ���ز���: ��
*******************************************************************************/
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct)
{
  EXTI_InitStruct->EXTI_Line = EXTI_LineNone;
  EXTI_InitStruct->EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStruct->EXTI_LineCmd = DISABLE;
}

/*******************************************************************************
* ��������: EXTI_GenerateSWInterrupt
* ��������: ����һ������ж�
* �������: EXTI_Line������ʹ�ܣ�����ܣ���EXTI�ߡ��ò�����ȡֵ��ֵ��0��18֮�� ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void EXTI_GenerateSWInterrupt(u32 EXTI_Line)
{
  /* Check the parameters [������]*/
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->SWIER |= EXTI_Line;
}

/*******************************************************************************
* ��������: EXTI_GetFlagStatus
* ��������: Checks whether the specified EXTI line flag is set or not.
* �������: ����ض���EXTI�б�־�Ƿ���λ
*                    �������������:
*                       - EXTI_Linex: �ⲿ�ж���x ��xȡֵ(0..18)
* �������: ��
* ���ز���: EXTI_Line����״̬��SET��RESET��.
*******************************************************************************/
FlagStatus EXTI_GetFlagStatus(u32 EXTI_Line)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [������]*/
  assert_param(IS_GET_EXTI_LINE(EXTI_Line));
  
  if ((EXTI->PR & EXTI_Line) != (u32)RESET)
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
* ��������: EXTI_ClearFlag
* ��������: ���EXTI�ߵĹ����־.
* �������: EXTI_Line�����������EXTI�߱�־��
*                    �����������������EXTI_Linex��ϣ�x������0��18.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void EXTI_ClearFlag(u32 EXTI_Line)
{
  /* Check the parameters [������]*/
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->PR = EXTI_Line;
}

/*******************************************************************************
* ��������: EXTI_GetITStatus
* ��������: ����ض���EXTI���Ƿ���λ��Ч��ƽ.
* �������: EXTI_Line����Ҫ���Ĺ���λ��
*                    �������������:EXTI_Linex: �ⲿ�ж���x,x����ȡ0��18��
* �������: ��
* ���ز���: EXTI_Line����״̬��SET��RESET��.
*******************************************************************************/
ITStatus EXTI_GetITStatus(u32 EXTI_Line)
{
  ITStatus bitstatus = RESET;
  u32 enablestatus = 0;

  /* Check the parameters [������]*/
  assert_param(IS_GET_EXTI_LINE(EXTI_Line));
  
  enablestatus =  EXTI->IMR & EXTI_Line;

  if (((EXTI->PR & EXTI_Line) != (u32)RESET) && (enablestatus != (u32)RESET))
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
* ��������: EXTI_ClearITPendingBit
* ��������: ���EXTI���й���λ
* �������: EXTI_Line�����������EXTI�й���λ��
*                    �������������:EXTI_Linex: �ⲿ�ж���x,x����ȡ0��18��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void EXTI_ClearITPendingBit(u32 EXTI_Line)
{
  /* Check the parameters [������]*/
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->PR = EXTI_Line;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
