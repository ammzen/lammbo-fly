/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_systick.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有SysTick的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_systick.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
/* ---------------------- SysTick registers bit mask -------------------- */
/* CTRL TICKINT Mask */
#define CTRL_TICKINT_Set      ((u32)0x00000002)
#define CTRL_TICKINT_Reset    ((u32)0xFFFFFFFD)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: SysTick_CLKSourceConfig
* 功能描述: 配置SysTick的时钟源.
* 输入参数: SysTick_CLKSource:SysTick的时钟源。
*                    这个参数可以选下面的值之一:
*                       - SysTick_CLKSource_HCLK_Div8: SysTick时钟源为AHB时钟的1/8.
*                       - SysTick_CLKSource_HCLK: SysTick时钟源为AHB时钟.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SysTick_CLKSourceConfig(u32 SysTick_CLKSource)
{
  /* Check the parameters [检查参数]*/
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
* 函数名称: SysTick_SetReload
* 功能描述: 设置SysTick的重载值.
* 输入参数: Reload：SysTick重载一个新值，这个参数必须在1和0x00FFFFFF之间.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SysTick_SetReload(u32 Reload)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SYSTICK_RELOAD(Reload));

  SysTick->LOAD = Reload;
}

/*******************************************************************************
* 函数名称: SysTick_CounterCmd
* 功能描述: 使能或取消SysTick计数.
* 输入参数: SysTick_Counter: SysTick计数器的新状态。
*                    这个参数可以选下面的值之一:
*                       - SysTick_Counter_Disable: 禁止计数器
*                       - SysTick_Counter_Enable: 允许计数器
*                       - SysTick_Counter_Clear: 计数器清零
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SysTick_CounterCmd(u32 SysTick_Counter)
{
  /* Check the parameters [检查参数]*/
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
* 函数名称: SysTick_ITConfig
* 功能描述: 使能或取消SysTick中断.
* 输入参数: NewState:SysTick中断的新状态。这个参数可以是ENABLE或是DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SysTick_ITConfig(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
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
* 函数名称: SysTick_GetCounter
* 功能描述: 获取SysTick计数器的值.
* 输入参数: 无
* 输出参数: 无
* 返回参数: SysTick的当前值
*******************************************************************************/
u32 SysTick_GetCounter(void)
{
  return(SysTick->VAL);
}

/*******************************************************************************
* 函数名称: SysTick_GetFlagStatus
* 功能描述: 检查特定的SysTick标记是否被设置.
* 输入参数: SysTick_FLAG:将检查的标记.
*                    这个参数可以选下面的值之一:
*                       - SysTick_FLAG_COUNT
*                       - SysTick_FLAG_SKEW
*                       - SysTick_FLAG_NOREF
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
FlagStatus SysTick_GetFlagStatus(u8 SysTick_FLAG)
{
  u32 statusreg = 0, tmp = 0 ;
  FlagStatus bitstatus = RESET;

  /* Check the parameters [检查参数]*/
  assert_param(IS_SYSTICK_FLAG(SysTick_FLAG));

  /* Get the SysTick register index [取得SysTick寄存器标志]*/
  tmp = SysTick_FLAG >> 3;

  if (tmp == 2) /* The flag to check is in CTRL register [检查CTRL寄存器标志]*/
  {
    statusreg = SysTick->CTRL;
  }
  else          /* The flag to check is in CALIB register [检查CALIB寄存器标志]*/
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
