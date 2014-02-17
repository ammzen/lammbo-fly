/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_dbgmcu.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有DBGMCU的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_dbgmcu.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
#define IDCODE_DEVID_Mask    ((u32)0x00000FFF)

/* 自用宏 -----------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: DBGMCU_GetREVID
* 功能描述: Returns the device revision identifier[返回设备的校正标示符].
* 输入参数: 无
* 输出参数: 无
* 返回参数: Device revision identifier[设备的校正标示符]
*******************************************************************************/
u32 DBGMCU_GetREVID(void)
{
   return(DBGMCU->IDCODE >> 16);
}

/*******************************************************************************
* 函数名称: DBGMCU_GetDEVID
* 功能描述: Returns the device identifier[返回设备的标示符].
* 输入参数: 无
* 输出参数: 无
* 返回参数: Device identifier[设备的标示符]
*******************************************************************************/
u32 DBGMCU_GetDEVID(void)
{
   return(DBGMCU->IDCODE & IDCODE_DEVID_Mask);
}

/*******************************************************************************
* 函数名称: DBGMCU_Config
* 功能描述: Configures the specified peripheral and low power mode behavior
*                  when the MCU under Debug mode[配置MCU在调试模式下指定外设和低功耗模式状态].
* 输入参数: (1) DBGMCU_Periph: specifies the peripheral and low power mode[指定的外设和低功耗模式].
*                    This parameter can be any combination of the following values:[可以使用下述任意值的组合]
*                       - DBGMCU_SLEEP: Keep debugger connection during SLEEP mode [在睡眠模式保持调试器的连接]             
*                       - DBGMCU_STOP: Keep debugger connection during STOP mode[在暂停模式保持调试器的连接]               
*                       - DBGMCU_STANDBY: Keep debugger connection during STANDBY mode[在停止模式保持调试器的连接]             
*                       - DBGMCU_IWDG_STOP: Debug IWDG stopped when Core is halted[当内核停止时停止调试IWDG]          
*                       - DBGMCU_WWDG_STOP: Debug WWDG stopped when Core is halted[当内核停止时停止调试WWDG]            
*                       - DBGMCU_TIM1_STOP: TIM1 counter stopped when Core is halted[当内核停止时停止TIM1计数器]           
*                       - DBGMCU_TIM2_STOP: TIM2 counter stopped when Core is halted[当内核停止时停止TIM2计数器]          
*                       - DBGMCU_TIM3_STOP: TIM3 counter stopped when Core is halted[当内核停止时停止TIM3计数器]          
*                       - DBGMCU_TIM4_STOP: TIM4 counter stopped when Core is halted[当内核停止时停止TIM4计数器]          
*                       - DBGMCU_CAN_STOP: Debug CAN stopped when Core is halted[当内核停止时停止调试CAN]           
*                       - DBGMCU_I2C1_SMBUS_TIMEOUT: I2C1 SMBUS timeout mode stopped
*                                                    when Core is halted[当内核停止时停止I2C1总线超时模式] 
*                       - DBGMCU_I2C2_SMBUS_TIMEOUT: I2C2 SMBUS timeout mode stopped
*                                                    when Core is halted[当内核停止时停止I2C2总线超时模式]
*                       - DBGMCU_TIM5_STOP: TIM5 counter stopped when Core is halted[当内核停止时停止TIM5计数器]          
*                       - DBGMCU_TIM6_STOP: TIM6 counter stopped when Core is halted[当内核停止时停止TIM6计数器]          
*                       - DBGMCU_TIM7_STOP: TIM7 counter stopped when Core is halted[当内核停止时停止TIM7计数器]          
*                       - DBGMCU_TIM8_STOP: TIM8 counter stopped when Core is halted[当内核停止时停止TIM8计数器]          
*           (2) NewState: new state of the specified peripheral in Debug mode.[指定的外设在低功耗模式的新状态]
*                    This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DBGMCU_Config(u32 DBGMCU_Periph, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_DBGMCU_PERIPH(DBGMCU_Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    DBGMCU->CR |= DBGMCU_Periph;
  }
  else
  {
    DBGMCU->CR &= ~DBGMCU_Periph;
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
