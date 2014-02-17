/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_bkp.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有BKP的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_bkp.h"
#include "stm32f10x_rcc.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
/* ------------ BKP registers bit address in the alias region ----------- */
#define BKP_OFFSET        (BKP_BASE - PERIPH_BASE)

/* --- CR Register ---*/
/* Alias word address of TPAL bit */
#define CR_OFFSET         (BKP_OFFSET + 0x30)
#define TPAL_BitNumber    0x01
#define CR_TPAL_BB        (PERIPH_BB_BASE + (CR_OFFSET * 32) + (TPAL_BitNumber * 4))

/* Alias word address of TPE bit */
#define TPE_BitNumber     0x00
#define CR_TPE_BB         (PERIPH_BB_BASE + (CR_OFFSET * 32) + (TPE_BitNumber * 4))

/* --- CSR Register ---*/
/* Alias word address of TPIE bit */
#define CSR_OFFSET        (BKP_OFFSET + 0x34)
#define TPIE_BitNumber    0x02
#define CSR_TPIE_BB       (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TPIE_BitNumber * 4))

/* Alias word address of TIF bit */
#define TIF_BitNumber     0x09
#define CSR_TIF_BB        (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TIF_BitNumber * 4))

/* Alias word address of TEF bit */
#define TEF_BitNumber     0x08
#define CSR_TEF_BB        (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TEF_BitNumber * 4))


/* ---------------------- BKP registers bit mask ------------------------ */
/* RTCCR register bit mask */
#define RTCCR_CAL_Mask    ((u16)0xFF80)
#define RTCCR_Mask        ((u16)0xFC7F)

/* CSR register bit mask */
#define CSR_CTE_Set       ((u16)0x0001)
#define CSR_CTI_Set       ((u16)0x0002)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: BKP_DeInit
* 功能描述: 将BKP外设寄存器复位到它们的默认值
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void BKP_DeInit(void)
{
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);
}

/*******************************************************************************
* 函数名称: BKP_TamperPinLevelConfig
* 功能描述: 配置窜改引脚的有效电平.
* 输入参数: BKP_TamperPinLevel: 窜改引脚的有效电平.
*                    这个参数可以取下面中的一个:
*                       - BKP_TamperPinLevel_High: 窜改引脚高有效
*                       - BKP_TamperPinLevel_Low: 窜改引脚低有效
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void BKP_TamperPinLevelConfig(u16 BKP_TamperPinLevel)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_BKP_TAMPER_PIN_LEVEL(BKP_TamperPinLevel));

  *(vu32 *) CR_TPAL_BB = BKP_TamperPinLevel;
}

/*******************************************************************************
* 函数名称: BKP_TamperPinCmd
* 功能描述: 使能/禁止窜改引脚激活.
* 输入参数: NewState: 窜改引脚激活的新状态该参数取值：ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void BKP_TamperPinCmd(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CR_TPE_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: BKP_ITConfig
* 功能描述: 使能/禁止窜改引脚中断.
* 输入参数: NewState: 窜改引脚中断的新状态该参数取值：ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void BKP_ITConfig(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CSR_TPIE_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: BKP_RTCOutputConfig
* 功能描述: 使能/禁止校准时钟的输出
* 输入参数: NewState: 校准时钟输出的新状态。该参数取值：ENABLE或DISABLE.
*                    这个参数可以取下面的值之一:
*                       - BKP_RTCOutputSource_None: 窜改的引脚上没有实时时钟输出.
*                       - BKP_RTCOutputSource_CalibClock: 窜改的引脚上输出的实时时钟等于频率除以64。
*                       - BKP_RTCOutputSource_Alarm: 窜改的引脚上输出实时时钟告警脉冲.
*                       - BKP_RTCOutputSource_Second: 窜改的引脚上输出实时时钟秒脉冲.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void BKP_RTCOutputConfig(u16 BKP_RTCOutputSource)
{
  u16 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_BKP_RTC_OUTPUT_SOURCE(BKP_RTCOutputSource));

  tmpreg = BKP->RTCCR;

  /* Clear CCO, ASOE and ASOS bits [清除CCO, ASOE 和 ASOS位]*/
  tmpreg &= RTCCR_Mask;
  
  /* Set CCO, ASOE and ASOS bits according to BKP_RTCOutputSource value [依照备份实时时钟源的值置位CCO, ASOE 和 ASOS位]*/
  tmpreg |= BKP_RTCOutputSource;

  /* Store the new value [保存新的值]*/
  BKP->RTCCR = tmpreg;
}

/*******************************************************************************
* 函数名称: BKP_SetRTCCalibrationValue
* 功能描述: 设置RTC时钟校准值.
* 输入参数: CalibrationValue: RTC时钟校准值。该参数取值从0到0x7F.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void BKP_SetRTCCalibrationValue(u8 CalibrationValue)
{
  u16 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_BKP_CALIBRATION_VALUE(CalibrationValue));

  tmpreg = BKP->RTCCR;

  /* Clear CAL[6:0] bits [清除CAL[6:0]位]*/
  tmpreg &= RTCCR_CAL_Mask;

  /* Set CAL[6:0] bits according to CalibrationValue value [依照校准值置位CAL[6:0]]*/
  tmpreg |= CalibrationValue;

  /* Store the new value */
  BKP->RTCCR = tmpreg;
}

/*******************************************************************************
* 函数名称: BKP_WriteBackupRegister
* 功能描述: 将用户数据写入指定的数据备份寄存器.
* 输入参数: (1)BKP_DR: 数据备份寄存器.这个参数可以是BKP_DRx，x取值在1，42。
*           (2)Data: 需要写入的数据
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void BKP_WriteBackupRegister(u16 BKP_DR, u16 Data)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_BKP_DR(BKP_DR));

  *(vu16 *) (BKP_BASE + BKP_DR) = Data;
}

/*******************************************************************************
* 函数名称: BKP_ReadBackupRegister
* 功能描述: 从指定的数据备份寄存器中读出数据.
* 输入参数: BKP_DR: 数据备份寄存器.这个参数可以是BKP_DRx，x取值在1，42。
* 输出参数: 无
* 返回参数: 指定的数据备份寄存器的内容
*******************************************************************************/
u16 BKP_ReadBackupRegister(u16 BKP_DR)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_BKP_DR(BKP_DR));

  return (*(vu16 *) (BKP_BASE + BKP_DR));
}

/*******************************************************************************
* 函数名称: BKP_GetFlagStatus
* 功能描述: 检查窜改引脚事件标志是否置位.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 窜改引脚事件标志的新状态（SET或RESET）.
*******************************************************************************/
FlagStatus BKP_GetFlagStatus(void)
{
  return (FlagStatus)(*(vu32 *) CSR_TEF_BB);
}

/*******************************************************************************
* 函数名称: BKP_ClearFlag
* 功能描述: 清除窜改引脚事件挂起标志.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void BKP_ClearFlag(void)
{
  /* Set CTE bit to clear Tamper Pin Event flag [置位CTE位清除窜改引脚事件标志]*/
  BKP->CSR |= CSR_CTE_Set;
}

/*******************************************************************************
* 函数名称: BKP_GetITStatus
* 功能描述: 检查窜改引脚中断是否发生.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 窜改引脚中断的新状态（SET或RESET）.
*******************************************************************************/
ITStatus BKP_GetITStatus(void)
{
  return (ITStatus)(*(vu32 *) CSR_TIF_BB);
}

/*******************************************************************************
* 函数名称: BKP_ClearITPendingBit
* 功能描述: 清除窜改引脚中断挂起位.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void BKP_ClearITPendingBit(void)
{
  /* Set CTI bit to clear Tamper Pin Interrupt pending bit [清除窜改引脚中断挂起位]*/
  BKP->CSR |= CSR_CTI_Set;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
