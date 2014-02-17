/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_exti.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有EXTI的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_exti.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
#define EXTI_LineNone    ((u32)0x00000)  /* No interrupt selected */

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: EXTI_DeInit
* 功能描述: 将EXTI外设寄存器复位到默认值.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
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
* 函数名称: EXTI_Init
* 功能描述: 根据EXIT_InitStruct中指定的参数初始化EXTI外设
* 输入参数: EXTI_InitStruct：指向EXTI_InitTypeDef的结构，它包含特定EXTI的配置信息。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
  assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
  assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line));  
  assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));
     
  if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)
  {
    /* Clear EXTI line configuration [清除外部中断线配置]*/
    EXTI->IMR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->EMR &= ~EXTI_InitStruct->EXTI_Line;
    
    *(vu32 *)(EXTI_BASE + (u32)EXTI_InitStruct->EXTI_Mode)|= EXTI_InitStruct->EXTI_Line;

    /* Clear Rising Falling edge configuration [清除上升沿下降沿边沿配置]*/
    EXTI->RTSR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->FTSR &= ~EXTI_InitStruct->EXTI_Line;
    
    /* Select the trigger for the selected external interrupts [选择触发已选择的外部中断]*/
    if (EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_Rising_Falling)
    {
      /* Rising Falling edge [上升沿下降沿边沿]*/
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
    /* Disable the selected external lines [禁止选中的外部中断线]*/
    *(vu32 *)(EXTI_BASE + (u32)EXTI_InitStruct->EXTI_Mode)&= ~EXTI_InitStruct->EXTI_Line;
  }
}

/*******************************************************************************
* 函数名称: EXTI_StructInit
* 功能描述: 将EXTI_InitStruct成员置为默认值
* 输入参数: EXTI_InitStruct：指向EXTI_InitTypeDef结构，该结构将被初始化。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct)
{
  EXTI_InitStruct->EXTI_Line = EXTI_LineNone;
  EXTI_InitStruct->EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStruct->EXTI_LineCmd = DISABLE;
}

/*******************************************************************************
* 函数名称: EXTI_GenerateSWInterrupt
* 功能描述: 产生一个软件中断
* 输入参数: EXTI_Line：将被使能（或禁能）的EXTI线。该参数可取值的值在0到18之间 。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void EXTI_GenerateSWInterrupt(u32 EXTI_Line)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->SWIER |= EXTI_Line;
}

/*******************************************************************************
* 函数名称: EXTI_GetFlagStatus
* 功能描述: Checks whether the specified EXTI line flag is set or not.
* 输入参数: 检查特定的EXTI行标志是否被置位
*                    这个参数可以是:
*                       - EXTI_Linex: 外部中断线x ，x取值(0..18)
* 输出参数: 无
* 返回参数: EXTI_Line的新状态（SET或RESET）.
*******************************************************************************/
FlagStatus EXTI_GetFlagStatus(u32 EXTI_Line)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [检查参数]*/
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
* 函数名称: EXTI_ClearFlag
* 功能描述: 清除EXTI线的挂起标志.
* 输入参数: EXTI_Line：将被清除的EXTI线标志。
*                    这个参数可以是任意EXTI_Linex组合，x可以是0到18.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void EXTI_ClearFlag(u32 EXTI_Line)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->PR = EXTI_Line;
}

/*******************************************************************************
* 函数名称: EXTI_GetITStatus
* 功能描述: 检查特定的EXTI行是否被置位有效电平.
* 输入参数: EXTI_Line：需要检查的挂起位。
*                    这个参数可以是:EXTI_Linex: 外部中断线x,x可以取0到18。
* 输出参数: 无
* 返回参数: EXTI_Line的新状态（SET或RESET）.
*******************************************************************************/
ITStatus EXTI_GetITStatus(u32 EXTI_Line)
{
  ITStatus bitstatus = RESET;
  u32 enablestatus = 0;

  /* Check the parameters [检查参数]*/
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
* 函数名称: EXTI_ClearITPendingBit
* 功能描述: 清除EXTI的行挂起位
* 输入参数: EXTI_Line：将被清除的EXTI行挂起位。
*                    这个参数可以是:EXTI_Linex: 外部中断线x,x可以取0到18。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void EXTI_ClearITPendingBit(u32 EXTI_Line)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->PR = EXTI_Line;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
