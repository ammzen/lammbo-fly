/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_iwdg.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: This file provides all the IWDG firmware functions.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_iwdg.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
/* ---------------------- IWDG registers bit mask ------------------------ */
/* KR register bit mask */
#define KR_KEY_Reload    ((u16)0xAAAA)
#define KR_KEY_Enable    ((u16)0xCCCC)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: IWDG_WriteAccessCmd
* 功能描述: 使能（或禁能）对IWDG_PR和IWDG_RLR寄存器的写访问.
* 输入参数: IWDG_WriteAccess: 对IWDG_PR和IWDG_RLR寄存器的写访问的新的状态。
*                    这个参数可以是下面的值之一:
*                       - IWDG_WriteAccess_Enable: 对IWDG_PR和IWDG_RLR寄存器的写访问被使能
*                       - IWDG_WriteAccess_Disable: 对IWDG_PR和IWDG_RLR寄存器的写访问被禁止
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void IWDG_WriteAccessCmd(u16 IWDG_WriteAccess)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_IWDG_WRITE_ACCESS(IWDG_WriteAccess));

  IWDG->KR = IWDG_WriteAccess;
}

/*******************************************************************************
* 函数名称: IWDG_SetPrescaler
* 功能描述: 设置IWDG预分频值.
* 输入参数: IWDG_Prescaler: IWDG预分频寄存器的值.
*                    这个参数可以是下面的值之一:
*                       - IWDG_Prescaler_4: IWDG预分频因子设置为 4
*                       - IWDG_Prescaler_8: IWDG预分频因子设置为 8
*                       - IWDG_Prescaler_16: IWDG预分频因子设置为 16
*                       - IWDG_Prescaler_32: IWDG预分频因子设置为 32
*                       - IWDG_Prescaler_64: IWDG预分频因子设置为 64
*                       - IWDG_Prescaler_128: IWDG预分频因子设置为 128
*                       - IWDG_Prescaler_256: IWDG预分频因子设置为 256
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void IWDG_SetPrescaler(u8 IWDG_Prescaler)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_IWDG_PRESCALER(IWDG_Prescaler));

  IWDG->PR = IWDG_Prescaler;
}

/*******************************************************************************
* 函数名称: IWDG_SetReload
* 功能描述: 设置IWDG的重载值.
* 输入参数: Reload: IWDG重载寄存器的值。
*                    这个参数一定是在'0'到'0x0FFF'之间的数.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void IWDG_SetReload(u16 Reload)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_IWDG_RELOAD(Reload));

  IWDG->RLR = Reload;
}

/*******************************************************************************
* 函数名称: IWDG_ReloadCounter
* 功能描述: 通过在重载寄存器中定义了的值重载IWDG计数器（对IWDG_PR和IWDG_RLR寄存器的写访问被禁能）.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void IWDG_ReloadCounter(void)
{
  IWDG->KR = KR_KEY_Reload;
}

/*******************************************************************************
* 函数名称: IWDG_Enable
* 功能描述: 使能IWDG（禁能对IWDG_PR和IWDG_RLR寄存器的写访问）.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void IWDG_Enable(void)
{
  IWDG->KR = KR_KEY_Enable;
}

/*******************************************************************************
* 函数名称: IWDG_GetFlagStatus
* 功能描述: 检验指定的IWDG标志是否置位.
* 输入参数: IWDG_FLAG:检查的标志。
*                    这个参数可以是下面的值之一:
*                       - IWDG_FLAG_PVU: 预分频因子值更新了
*                       - IWDG_FLAG_RVU: 重载值更新了
* 输出参数: 无
* 返回参数: IWDG_FLAG新的状态（置位或复位）.
*******************************************************************************/
FlagStatus IWDG_GetFlagStatus(u16 IWDG_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [检查参数]*/
  assert_param(IS_IWDG_FLAG(IWDG_FLAG));

  if ((IWDG->SR & IWDG_FLAG) != (u32)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* Return the flag status [返回状态标志]*/
  return bitstatus;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
