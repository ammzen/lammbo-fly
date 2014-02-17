/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_pwr.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: This file provides all the PWR firmware functions.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
/* --------- PWR registers bit address in the alias region ---------- */
#define PWR_OFFSET               (PWR_BASE - PERIPH_BASE)

/* --- CR Register ---*/
/* Alias word address of DBP bit */
#define CR_OFFSET                (PWR_OFFSET + 0x00)
#define DBP_BitNumber            0x08
#define CR_DBP_BB                (PERIPH_BB_BASE + (CR_OFFSET * 32) + (DBP_BitNumber * 4))

/* Alias word address of PVDE bit */
#define PVDE_BitNumber           0x04
#define CR_PVDE_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PVDE_BitNumber * 4))

/* --- CSR Register ---*/
/* Alias word address of EWUP bit */
#define CSR_OFFSET               (PWR_OFFSET + 0x04)
#define EWUP_BitNumber           0x08
#define CSR_EWUP_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (EWUP_BitNumber * 4))

/* ------------------ PWR registers bit mask ------------------------ */
/* CR register bit mask */
#define CR_PDDS_Set              ((u32)0x00000002)
#define CR_DS_Mask               ((u32)0xFFFFFFFC)
#define CR_CWUF_Set              ((u32)0x00000004)
#define CR_PLS_Mask              ((u32)0xFFFFFF1F)

/* --------- Cortex System Control register bit mask ---------------- */
/* Cortex System Control register address */
#define SCB_SysCtrl              ((u32)0xE000ED10)
/* SLEEPDEEP bit mask */
#define SysCtrl_SLEEPDEEP_Set    ((u32)0x00000004)

/* 自用宏 --------------------------------------------------------------------*/
/* 自用变量 ------------------------------------------------------------------*/
/* 自用函数原型 --------------------------------------------------------------*/
/* 自用函数 ------------------------------------------------------------------*/

/*******************************************************************************
* 函数名称: PWR_DeInit
* 功能描述: 复位PWR外围寄存器为默认复位值.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void PWR_DeInit(void)
{
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, DISABLE);
}

/*******************************************************************************
* 函数名称: PWR_BackupAccessCmd
* 功能描述: 使能或关闭对RTC和备份寄存器的访问。
* 输入参数: NewState:访问RTC和备份寄存器的新状态。这个参数可以是：ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void PWR_BackupAccessCmd(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CR_DBP_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: PWR_PVDCmd
* 功能描述: 使能或关闭电源电压探测器（PVD）。
* 输入参数: NewState:PVD的新状态。这个参数可以是：ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void PWR_PVDCmd(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CR_PVDE_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: PWR_PVDLevelConfig
* 功能描述: 配置由电源电压探测器探测的电压门限值（PVD）.
* 输入参数: PWR_PVDLevel:PVD探测电平。
*                    这个参数可以取下面的值之一:
*                       - PWR_PVDLevel_2V2: PVD探测电平设置为 2.2V
*                       - PWR_PVDLevel_2V3: PVD探测电平设置为 2.3V
*                       - PWR_PVDLevel_2V4: PVD探测电平设置为 2.4V
*                       - PWR_PVDLevel_2V5: PVD探测电平设置为 2.5V
*                       - PWR_PVDLevel_2V6: PVD探测电平设置为 2.6V
*                       - PWR_PVDLevel_2V7: PVD探测电平设置为 2.7V
*                       - PWR_PVDLevel_2V8: PVD探测电平设置为 2.8V
*                       - PWR_PVDLevel_2V9: PVD探测电平设置为 2.9V
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void PWR_PVDLevelConfig(u32 PWR_PVDLevel)
{
  u32 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_PWR_PVD_LEVEL(PWR_PVDLevel));

  tmpreg = PWR->CR;

  /* Clear PLS[7:5] bits [清零PLS[7:5]位]*/
  tmpreg &= CR_PLS_Mask;

  /* Set PLS[7:5] bits according to PWR_PVDLevel value [依照PWR_PVDLevel的值置位PLS[7:5]]*/
  tmpreg |= PWR_PVDLevel;

  /* Store the new value [保存新的值]*/
  PWR->CR = tmpreg;
}

/*******************************************************************************
* 函数名称: PWR_WakeUpPinCmd
* 功能描述: 使能唤醒引脚的功能
* 输入参数: NewState:唤醒引脚功能的新状态。这个参数可以是：ENALBLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void PWR_WakeUpPinCmd(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CSR_EWUP_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: PWR_EnterSTOPMode
* 功能描述: 进入STOP模式.
* 输入参数: PWR_Regulator:STOP模式的校准状态。
*                    这个参数可以是下面的值之一:
*                       - PWR_Regulator_ON: 校准器开启的STOP模式
*                       - PWR_Regulator_LowPower: 在低功耗模式下带有校准器的STOP模式。
*                  - PWR_STOPEntry: 这个参数定义了STOP进入模式.
*                    这个参数可以是下面的值之一:
*                       - PWR_STOPEntry_WFI: 进入有WFI指令的STOP模式
*                       - PWR_STOPEntry_WFE: 进入有WFE指令的STOP模式
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void PWR_EnterSTOPMode(u32 PWR_Regulator, u8 PWR_STOPEntry)
{
  u32 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_PWR_REGULATOR(PWR_Regulator));
  assert_param(IS_PWR_STOP_ENTRY(PWR_STOPEntry));
  
  /* Select the regulator state in STOP mode [在停止模式选择校准状态]*/
  tmpreg = PWR->CR;

  /* Clear PDDS and LPDS bits [清PDDS和LPDS位]*/
  tmpreg &= CR_DS_Mask;

  /* Set LPDS bit according to PWR_Regulator value [依照PWR_Regulator值置位LPDS]*/
  tmpreg |= PWR_Regulator;

  /* Store the new value [保存新的值]*/
  PWR->CR = tmpreg;

  /* Set SLEEPDEEP bit of Cortex System Control Register [置位Cortex系统的控制寄存器SLEEPDEEP位]*/
  *(vu32 *) SCB_SysCtrl |= SysCtrl_SLEEPDEEP_Set;
  
  /* Select STOP mode entry [选择停止模式进入]*/
  if(PWR_STOPEntry == PWR_STOPEntry_WFI)
  {   
    /* Request Wait For Interrupt [请求等待中断]*/
    __WFI();
  }
  else
  {
    /* Request Wait For Event [请求等待事件]*/
    __WFE();
  }
}

/*******************************************************************************
* 函数名称: PWR_EnterSTANDBYMode
* 功能描述: 进入STANDBY模式.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void PWR_EnterSTANDBYMode(void)
{
  /* Clear Wake-up flag [清除Wake-up标志]*/
  PWR->CR |= CR_CWUF_Set;

  /* Select STANDBY mode [选择STANDBY模式]*/
  PWR->CR |= CR_PDDS_Set;

  /* Set SLEEPDEEP bit of Cortex System Control Register [置位Cortex系统的控制寄存器SLEEPDEEP位]*/
  *(vu32 *) SCB_SysCtrl |= SysCtrl_SLEEPDEEP_Set;

  /* Request Wait For Interrupt [请求等待中断]*/
  __WFI();
}

/*******************************************************************************
* 函数名称: PWR_GetFlagStatus
* 功能描述: 检查指定的PWR标志位设置与否.
* 输入参数: PWR_FLAG:要检查的标志。
*                    这个参数可以是下面的值之一:
*                       - PWR_FLAG_WU: Wake Up 标志
*                       - PWR_FLAG_SB: StandBy 标志
*                       - PWR_FLAG_PVDO: PVD 输出
* 输出参数: 无
* 返回参数: PWR_FLAG的新状态（SET或RESET）.
*******************************************************************************/
FlagStatus PWR_GetFlagStatus(u32 PWR_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [检查参数]*/
  assert_param(IS_PWR_GET_FLAG(PWR_FLAG));
  
  if ((PWR->CSR & PWR_FLAG) != (u32)RESET)
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

/*******************************************************************************
* 函数名称: PWR_ClearFlag
* 功能描述: 清空PWR挂起标志位。
* 输入参数: PWR_FLAG:要清空的标志。
*                    这个参数可以是下面的值之一:
*                       - PWR_FLAG_WU: Wake Up 标志
*                       - PWR_FLAG_SB: StandBy 标志
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void PWR_ClearFlag(u32 PWR_FLAG)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_PWR_CLEAR_FLAG(PWR_FLAG));
         
  PWR->CR |=  PWR_FLAG << 2;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
