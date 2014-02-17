/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_wwdg.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: This file provides all the WWDG firmware functions.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_wwdg.h"
#include "stm32f10x_rcc.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
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

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: WWDG_DeInit
* 功能描述: 重置WWDG外围设备寄存器为他们的默认重置值.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void WWDG_DeInit(void)
{
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, DISABLE);
}

/*******************************************************************************
* 函数名称: WWDG_SetPrescaler
* 功能描述: 设置WWDG预分频器.
* 输入参数: WWDG_Prescaler：指明WWDG预分频器.
*                    这个参数可以是下面的值之一:
*                       - WWDG_Prescaler_1: WWDG计数器时钟 = (PCLK1/4096)/1
*                       - WWDG_Prescaler_2: WWDG计数器时钟 = (PCLK1/4096)/2
*                       - WWDG_Prescaler_4: WWDG计数器时钟 = (PCLK1/4096)/4
*                       - WWDG_Prescaler_8: WWDG计数器时钟 = (PCLK1/4096)/8
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void WWDG_SetPrescaler(u32 WWDG_Prescaler)
{
  u32 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_WWDG_PRESCALER(WWDG_Prescaler));

  /* Clear WDGTB[1:0] bits [清WDGTB[1:0]位]*/
  tmpreg = WWDG->CFR & CFR_WDGTB_Mask;

  /* Set WDGTB[1:0] bits according to WWDG_Prescaler value [依照WWDG预分频器的值设置WDGTB[1:0]位]*/
  tmpreg |= WWDG_Prescaler;

  /* Store the new value */
  WWDG->CFR = tmpreg;
}

/*******************************************************************************
* 函数名称: WWDG_SetWindowValue
* 功能描述: 设置WWDG窗口值.
* 输入参数: WindowValue：指明要和倒计数器比较的窗口值.这个个参数的值必须小于0x80.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void WWDG_SetWindowValue(u8 WindowValue)
{
  u32 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_WWDG_WINDOW_VALUE(WindowValue));

  /* Clear W[6:0] bits [清W[6:0]位]*/
  tmpreg = WWDG->CFR & CFR_W_Mask;

  /* Set W[6:0] bits according to WindowValue value [依照窗口值的值设置W[6:0]位]*/
  tmpreg |= WindowValue & BIT_Mask;

  /* Store the new value [保存新的值]*/
  WWDG->CFR = tmpreg;
}

/*******************************************************************************
* 函数名称: WWDG_EnableIT
* 功能描述: 使能WWDG提前唤醒中断.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void WWDG_EnableIT(void)
{
  *(vu32 *) CFR_EWI_BB = (u32)ENABLE;
}

/*******************************************************************************
* 函数名称: WWDG_SetCounter
* 功能描述: 设置WWDG计数器的值.
* 输入参数: Counter:：指定看门狗计数器的值这个参数必须是0x40到0x70之间的一个数.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void WWDG_SetCounter(u8 Counter)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_WWDG_COUNTER(Counter));

  /* Write to T[6:0] bits to configure the counter value[为配置计数器的值写T[6:0]位], no need to do
     a read-modify-write[不需要读-修改-写]; writing a 0 to WDGA bit does nothing [除了写0到WDGA位不用做任何事]*/
  WWDG->CR = Counter & BIT_Mask;
}

/*******************************************************************************
* 函数名称: WWDG_Enable
* 功能描述: 使能WWDG和载入计数器的值.
* 输入参数: 计数器：指定看门狗计数器的值这个参数必须是0x40到0x7F之间的一个数.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void WWDG_Enable(u8 Counter)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_WWDG_COUNTER(Counter));

  WWDG->CR = CR_WDGA_Set | Counter;
}

/*******************************************************************************
* 函数名称: WWDG_GetFlagStatus
* 功能描述: 检测提前唤醒中断标记是否被置位.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 提前唤醒中断比较的新状态（置位或复位）
*******************************************************************************/
FlagStatus WWDG_GetFlagStatus(void)
{
  return (FlagStatus)(WWDG->SR);
}

/*******************************************************************************
* 函数名称: WWDG_ClearFlag
* 功能描述: 清除提前唤醒中断标记
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void WWDG_ClearFlag(void)
{
  WWDG->SR = (u32)RESET;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
