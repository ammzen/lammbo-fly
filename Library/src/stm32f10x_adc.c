/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_adc.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有ADC的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ----------------------------------------------------------------*/
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"

/* 自用类型 ------------------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------------*/
/* ADC DISCNUM mask */
#define CR1_DISCNUM_Reset           ((u32)0xFFFF1FFF)

/* ADC DISCEN mask */
#define CR1_DISCEN_Set              ((u32)0x00000800)
#define CR1_DISCEN_Reset            ((u32)0xFFFFF7FF)

/* ADC JAUTO mask */
#define CR1_JAUTO_Set               ((u32)0x00000400)
#define CR1_JAUTO_Reset             ((u32)0xFFFFFBFF)

/* ADC JDISCEN mask */
#define CR1_JDISCEN_Set             ((u32)0x00001000)
#define CR1_JDISCEN_Reset           ((u32)0xFFFFEFFF)

/* ADC AWDCH mask */
#define CR1_AWDCH_Reset             ((u32)0xFFFFFFE0)

/* ADC Analog watchdog enable mode mask */
#define CR1_AWDMode_Reset           ((u32)0xFF3FFDFF)

/* CR1 register Mask */
#define CR1_CLEAR_Mask              ((u32)0xFFF0FEFF)

/* ADC ADON mask */
#define CR2_ADON_Set                ((u32)0x00000001)
#define CR2_ADON_Reset              ((u32)0xFFFFFFFE)

/* ADC DMA mask */
#define CR2_DMA_Set                 ((u32)0x00000100)
#define CR2_DMA_Reset               ((u32)0xFFFFFEFF)

/* ADC RSTCAL mask */
#define CR2_RSTCAL_Set              ((u32)0x00000008)

/* ADC CAL mask */
#define CR2_CAL_Set                 ((u32)0x00000004)

/* ADC SWSTART mask */
#define CR2_SWSTART_Set             ((u32)0x00400000)

/* ADC EXTTRIG mask */
#define CR2_EXTTRIG_Set             ((u32)0x00100000)
#define CR2_EXTTRIG_Reset           ((u32)0xFFEFFFFF)

/* ADC Software start mask */
#define CR2_EXTTRIG_SWSTART_Set     ((u32)0x00500000)
#define CR2_EXTTRIG_SWSTART_Reset   ((u32)0xFFAFFFFF)

/* ADC JEXTSEL mask */
#define CR2_JEXTSEL_Reset           ((u32)0xFFFF8FFF)

/* ADC JEXTTRIG mask */
#define CR2_JEXTTRIG_Set            ((u32)0x00008000)
#define CR2_JEXTTRIG_Reset          ((u32)0xFFFF7FFF)

/* ADC JSWSTART mask */
#define CR2_JSWSTART_Set            ((u32)0x00200000)

/* ADC injected software start mask */
#define CR2_JEXTTRIG_JSWSTART_Set   ((u32)0x00208000)
#define CR2_JEXTTRIG_JSWSTART_Reset ((u32)0xFFDF7FFF)

/* ADC TSPD mask */
#define CR2_TSVREFE_Set             ((u32)0x00800000)
#define CR2_TSVREFE_Reset           ((u32)0xFF7FFFFF)

/* CR2 register Mask */
#define CR2_CLEAR_Mask              ((u32)0xFFF1F7FD)

/* ADC SQx mask */
#define SQR3_SQ_Set                 ((u32)0x0000001F)
#define SQR2_SQ_Set                 ((u32)0x0000001F)
#define SQR1_SQ_Set                 ((u32)0x0000001F)

/* SQR1 register Mask */
#define SQR1_CLEAR_Mask             ((u32)0xFF0FFFFF)

/* ADC JSQx mask */
#define JSQR_JSQ_Set                ((u32)0x0000001F)

/* ADC JL mask */
#define JSQR_JL_Set                 ((u32)0x00300000)
#define JSQR_JL_Reset               ((u32)0xFFCFFFFF)

/* ADC SMPx mask */
#define SMPR1_SMP_Set               ((u32)0x00000007)
#define SMPR2_SMP_Set               ((u32)0x00000007)

/* ADC JDRx registers offset */
#define JDR_Offset                  ((u8)0x28)

/* ADC1 DR register base address */
#define DR_ADDRESS                  ((u32)0x4001244C)

/* 自用宏 -----------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: ADC_DeInit
* 功能描述: 复位ADCx外围模块寄存器，使其为默认值。
* 输入参数: ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_DeInit(ADC_TypeDef* ADCx)
{
  /* Check the parameters [检查参数 检查参数]*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  switch (*(u32*)&ADCx)
  {
    case ADC1_BASE:
      /* Enable ADC1 reset state [允许ADC1复位状态]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
      /* Release ADC1 from reset state [解除ADC1复位状态]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
      break;
    
    case ADC2_BASE:
      /* Enable ADC2 reset state [允许ADC2复位状态]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);
      /* Release ADC2 from reset state [解除ADC2复位状态]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);
      break;
      
    case ADC3_BASE:
      /* Enable ADC3 reset state [允许ADC3复位状态]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);
      /* Release ADC3 from reset state [解除ADC3复位状态]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);
      break; 

    default:
      break;
  }
}

/*******************************************************************************
* 函数名称: ADC_Init
* 功能描述: 根据ADC_InitStruct 中指定的参数初始化ADCx 外围模块。
* 输入参数: (1)ADCx: 其中x 可以是1 2或3，用来选择ADC 外围模块.
*           (2)ADC_InitStruc：指向结构体ADC_InitTypeDef 的指针,该结构包括了指定ADC外围模块的配置信息。
* 输出参数: 无
* 返回参数: 无
******************************************************************************/
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
{
  u32 tmpreg1 = 0;
  u8 tmpreg2 = 0;

  /* Check the parameters [检查参数 检查参数]*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_MODE(ADC_InitStruct->ADC_Mode));
  assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ScanConvMode));
  assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ContinuousConvMode));            
  assert_param(IS_ADC_EXT_TRIG(ADC_InitStruct->ADC_ExternalTrigConv));   
  assert_param(IS_ADC_DATA_ALIGN(ADC_InitStruct->ADC_DataAlign)); 
  assert_param(IS_ADC_REGULAR_LENGTH(ADC_InitStruct->ADC_NbrOfChannel));

  /*---------------------------- ADCx CR1 Configuration [ADCx CR1配置]-----------------*/
  /* Get the ADCx CR1 value [得到ADCx CR1的值]*/
  tmpreg1 = ADCx->CR1;
  /* Clear DUALMOD and SCAN bits [清除DUALMOD和SCAN位]*/
  tmpreg1 &= CR1_CLEAR_Mask;
  /* Configure ADCx: Dual mode and scan conversion mode [配置ADCx:双重模式和扫描转换模式]*/
  /* Set DUALMOD bits according to ADC_Mode value [设置DUALMOD位依照ADC模式值]*/
  /* Set SCAN bit according to ADC_ScanConvMode value */
  tmpreg1 |= (u32)(ADC_InitStruct->ADC_Mode | ((u32)ADC_InitStruct->ADC_ScanConvMode << 8));
  /* Write to ADCx CR1 [写到ADCx CR1]*/
  ADCx->CR1 = tmpreg1;

  /*---------------------------- ADCx CR2 Configuration [ADCx CR2配置]-----------------*/
  /* Get the ADCx CR2 value [得到ADCx CR2的值]*/
  tmpreg1 = ADCx->CR2;
  /* Clear CONT, ALIGN and EXTSEL bits [清除CONT,ALIGN和EXTSEL位]*/
  tmpreg1 &= CR2_CLEAR_Mask;
  /* Configure ADCx: external trigger event and continuous conversion mode [配置ADCx:触发器时间和连续转换模式]*/
  /* Set ALIGN bit according to ADC_DataAlign value [依照ADC_DataAlign的值设置ALIGN位]*/
  /* Set EXTSEL bits according to ADC_ExternalTrigConv value [依照ADC_ExternalTrigConv的值设置EXTSEL位]*/
  /* Set CONT bit according to ADC_ContinuousConvMode value [依照ADC_ContinuousConvMode的值设置CONT位]*/
  tmpreg1 |= (u32)(ADC_InitStruct->ADC_DataAlign | ADC_InitStruct->ADC_ExternalTrigConv |
            ((u32)ADC_InitStruct->ADC_ContinuousConvMode << 1));
  /* Write to ADCx CR2 [写入ADCx CR2]*/
  ADCx->CR2 = tmpreg1;

  /*---------------------------- ADCx SQR1 Configuration [ADCx SQR1配置]-----------------*/
  /* Get the ADCx SQR1 value [取得ADCx SQR1的值]*/
  tmpreg1 = ADCx->SQR1;
  /* Clear L bits [清除L位]*/
  tmpreg1 &= SQR1_CLEAR_Mask;
  /* Configure ADCx: regular channel sequence length [配置ADCx:规则信道长度]*/
  /* Set L bits according to ADC_NbrOfChannel value [依照ADC_NbrOfChannel的值设置L位]*/
  tmpreg2 |= (ADC_InitStruct->ADC_NbrOfChannel - 1);
  tmpreg1 |= ((u32)tmpreg2 << 20);
  /* Write to ADCx SQR1 [写入ADCx SQR1]*/
  ADCx->SQR1 = tmpreg1;
}

/*******************************************************************************
* 函数名称: ADC_StructInit
* 功能描述: 使用默认值初始化ADC_InitStruct各成员变量。
* 输入参数: ADC_InitStruct:指向结构体ADC_InitTypeDef的指针，用来初始化
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
  /* Reset ADC init structure parameters values [复位ADC初始结构变量]*/
  /* Initialize the ADC_Mode member [初始化ADC_Mode成员]*/
  ADC_InitStruct->ADC_Mode = ADC_Mode_Independent;

  /* initialize the ADC_ScanConvMode member [初始化ADC_ScanConvMode成员]*/
  ADC_InitStruct->ADC_ScanConvMode = DISABLE;

  /* Initialize the ADC_ContinuousConvMode member [初始化ADC_ContinuousConvMode成员]*/
  ADC_InitStruct->ADC_ContinuousConvMode = DISABLE;

  /* Initialize the ADC_ExternalTrigConv member [初始化ADC_ExternalTrigConve成员]*/
  ADC_InitStruct->ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;

  /* Initialize the ADC_DataAlign member [初始化ADC_DataAlign成员]*/
  ADC_InitStruct->ADC_DataAlign = ADC_DataAlign_Right;

  /* Initialize the ADC_NbrOfChannel member [初始化ADC_NbrOfChannel成员]*/
  ADC_InitStruct->ADC_NbrOfChannel = 1;
}

/*******************************************************************************
* 函数名称: ADC_Cmd
* 功能描述: 使能/禁止指定的ADC外围模块.
* 输入参数: (1) ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)ADCx外围模块的新状态这个参数可以是：ENABLE或DISABLE。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters 检查参数 检查参数*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the ADON bit to wake up the ADC from power down mode [从掉电模式置位ADON到唤醒]*/
    ADCx->CR2 |= CR2_ADON_Set;
  }
  else
  {
    /* Disable the selected ADC peripheral [禁用选择的ADC设备]*/
    ADCx->CR2 &= CR2_ADON_Reset;
  }
}

/*******************************************************************************
* 函数名称: ADC_DMACmd
* 功能描述: 使能/禁止指定的ADC DMA请求.
* 输入参数: (1)ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
*                    注意: ADC2 没有 DMA 能力.
*           (2)ADC DMA转移器的新状态这个参数可以是：ENABLE或DISABLE。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters 检查参数 检查参数*/
  assert_param(IS_ADC_DMA_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC DMA request [允许选择的ADC DMA请求]*/
    ADCx->CR2 |= CR2_DMA_Set;
  }
  else
  {
    /* Disable the selected ADC DMA request [禁用选择的ADC DMA请求]*/
    ADCx->CR2 &= CR2_DMA_Reset;
  }
}

/*******************************************************************************
* 函数名称: ADC_ITConfig
* 功能描述: 使能/禁止指定的ADC中断.
* 输入参数: (1)ADCx: 其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)ADC_IT: 指定ADC中断源是使能的或禁止的.
*                    ADC_IT 用来使能或者禁止ADC 中断.可以使用下述值的一个或者几个值的组合:
*                       - ADC_IT_EOC: EOC中断屏蔽
*                       - ADC_IT_AWD: AWDOG中断屏蔽
*                       - ADC_IT_JEOC: JEOC中断屏蔽
*           (3)NewState: 指定的ADC中断的新状态这个参数可以是：ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_ITConfig(ADC_TypeDef* ADCx, u16 ADC_IT, FunctionalState NewState)
{
  u8 itmask = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_ADC_IT(ADC_IT));

  /* Get the ADC IT index [取得ADC IT中断号]*/
  itmask = (u8)ADC_IT;

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC interrupts [允许选择的ADC 中断]*/
    ADCx->CR1 |= itmask;
  }
  else
  {
    /* Disable the selected ADC interrupts [禁用选择的ADC 中断]*/
    ADCx->CR1 &= (~(u32)itmask);
  }
}

/*******************************************************************************
* 函数名称: ADC_ResetCalibration
* 功能描述: 重置选中的ADC校准寄存器
* 输入参数: ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_ResetCalibration(ADC_TypeDef* ADCx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Resets the selected ADC calibration registers [复位选择的ADC校准寄存器]*/
  ADCx->CR2 |= CR2_RSTCAL_Set;
}

/*******************************************************************************
* 函数名称: ADC_GetResetCalibrationStatus
* 功能描述: 得到选中的ADC重置校准寄存器的状态.
* 输入参数: ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
* 输出参数: 无
* 返回参数: ADC重置校准寄存器的新状态（SET或RESET）.
*******************************************************************************/
FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Check the status of RSTCAL bit [检查状态位RSTCAL]*/
  if ((ADCx->CR2 & CR2_RSTCAL_Set) != (u32)RESET)
  {
    /* RSTCAL bit is set [置位RSTCAL]*/
    bitstatus = SET;
  }
  else
  {
    /* RSTCAL bit is reset [复位RSTCAL]*/
    bitstatus = RESET;
  }

  /* Return the RSTCAL bit status [返回状态位RSTCAL]*/
  return  bitstatus;
}

/*******************************************************************************
* 函数名称: ADC_StartCalibration
* 功能描述: 开始选中的ADC校准过程.
* 输入参数: ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_StartCalibration(ADC_TypeDef* ADCx)
{
  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Enable the selected ADC calibration process [允许被选择的ADC校准过程]*/
  ADCx->CR2 |= CR2_CAL_Set;
}

/*******************************************************************************
* 函数名称: ADC_GetCalibrationStatus
* 功能描述: 得到选中的ADC 校准的状态
* 输入参数: ADCx:其中x可以是1、2或3，用来选择ADC外围模块..
* 输出参数: 无
* 返回参数: ADC 校准的新状态（SET 或RESET）.
*******************************************************************************/
FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Check the status of CAL bit [检查状态位CAL]*/
  if ((ADCx->CR2 & CR2_CAL_Set) != (u32)RESET)
  {
    /* CAL bit is set: calibration on going [CAL设置:校准开始]*/
    bitstatus = SET;
  }
  else
  {
    /* CAL bit is reset: end of calibration [CAL复位:结束校准]*/
    bitstatus = RESET;
  }

  /* Return the CAL bit status [返回状态位CAL]*/
  return  bitstatus;
}

/*******************************************************************************
* 函数名称: ADC_SoftwareStartConvCmd
* 功能描述: 使能/禁止选中的ADC由软件控制开始转换 .
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)NewState:选中的由软件发出开始信号的ADC的新状态这个参数可以是:ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC conversion on external event and start the selected
       ADC conversion [允许选择的ADC转换外部事件和启动选择的ADC转换]*/
    ADCx->CR2 |= CR2_EXTTRIG_SWSTART_Set;
  }
  else
  {
    /* Disable the selected ADC conversion on external event and stop the selected
       ADC conversion [禁止选择的ADC转换外部事件和停止选择的ADC转换]*/
    ADCx->CR2 &= CR2_EXTTRIG_SWSTART_Reset;
  }
}

/*******************************************************************************
* 函数名称: ADC_GetSoftwareStartConvStatus
* 功能描述: 获得选中ADC软件开始转换状态。
* 输入参数: ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
* 输出参数: 无
* 返回参数: ADC软件开始转换的新状态（SET或RESET）.
*******************************************************************************/
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Check the status of SWSTART bit [检查状态位SWSTART]*/
  if ((ADCx->CR2 & CR2_SWSTART_Set) != (u32)RESET)
  {
    /* SWSTART bit is set [置位SWSTART]*/
    bitstatus = SET;
  }
  else
  {
    /* SWSTART bit is reset [复位SWSTART]*/
    bitstatus = RESET;
  }

  /* Return the SWSTART bit status [返回状态位SWSTART]*/
  return  bitstatus;
}

/*******************************************************************************
* 函数名称: ADC_DiscModeChannelCountConfig
* 功能描述: 配置选中的ADC常规组为非连续模式.
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)Number:非连续模式下常规信道计数值。该值范围为1－8。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, u8 Number)
{
  u32 tmpreg1 = 0;
  u32 tmpreg2 = 0;

  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_REGULAR_DISC_NUMBER(Number));

  /* Get the old register value [取得过去的寄存器值]*/
  tmpreg1 = ADCx->CR1;
  /* Clear the old discontinuous mode channel count [清除过去的不间断模式通道计数器]*/
  tmpreg1 &= CR1_DISCNUM_Reset;
  /* Set the discontinuous mode channel count [设置不间断模式通道计数器]*/
  tmpreg2 = Number - 1;
  tmpreg1 |= tmpreg2 << 13;
  /* Store the new register value [存储新的寄存器值]*/
  ADCx->CR1 = tmpreg1;
}

/*******************************************************************************
* 函数名称: ADC_DiscModeCmd
* 功能描述: 使能/禁止指定的ADC常规组信道的非连续模式
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)NewState: ADC常规组信道下非连续模式的新状态这个参数可以是：ENABLE或DISABLE。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC regular discontinuous mode [允许选择的ADC规则间断模式]*/
    ADCx->CR1 |= CR1_DISCEN_Set;
  }
  else
  {
    /* Disable the selected ADC regular discontinuous mode [禁止选择的ADC规则间断模式]*/
    ADCx->CR1 &= CR1_DISCEN_Reset;
  }
}

/*******************************************************************************
* 函数名称: ADC_RegularChannelConfig
* 功能描述: 为选中的ADC常规组信道配置相关的音序器（sequencer）等级和采样时间。
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)ADC_Channel:需要配置的ADC信道
*                    ADC_Channel 可能的取值：
*                       - ADC_Channel_0: ADC信道0被选择
*                       - ADC_Channel_1: ADC信道1被选择
*                       - ADC_Channel_2: ADC信道2被选择
*                       - ADC_Channel_3: ADC信道3被选择
*                       - ADC_Channel_4: ADC信道4被选择
*                       - ADC_Channel_5: ADC信道5被选择
*                       - ADC_Channel_6: ADC信道6被选择
*                       - ADC_Channel_7: ADC信道7被选择
*                       - ADC_Channel_8: ADC信道8被选择
*                       - ADC_Channel_9: ADC信道9被选择
*                       - ADC_Channel_10: ADC信道10被选择
*                       - ADC_Channel_11: ADC信道11被选择
*                       - ADC_Channel_12: ADC信道12被选择
*                       - ADC_Channel_13: ADC信道13被选择
*                       - ADC_Channel_14: ADC信道14被选择
*                       - ADC_Channel_15: ADC信道15被选择
*                       - ADC_Channel_16: ADC信道16被选择
*                       - ADC_Channel_17: ADC信道17被选择
*           (3)Rank:常规组音序器（sequencer）的等级。参数范围是1－16。
*           (4)ADC_SampleTime:将要为所选的信道设置的采样时间
*                    ADC_SampleTime.取值:
*                       - ADC_SampleTime_1Cycles5: 采样时间等于1.5个周期
*                       - ADC_SampleTime_7Cycles5: 采样时间等于7.5个周期
*                       - ADC_SampleTime_13Cycles5: 采样时间等于13.5个周期
*                       - ADC_SampleTime_28Cycles5: 采样时间等于28.5个周期
*                       - ADC_SampleTime_41Cycles5: 采样时间等于41.5个周期
*                       - ADC_SampleTime_55Cycles5: 采样时间等于55.5个周期
*                       - ADC_SampleTime_71Cycles5: 采样时间等于71.5个周期
*                       - ADC_SampleTime_239Cycles5: 采样时间等于239.5个周期
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, u8 ADC_Channel, u8 Rank, u8 ADC_SampleTime)
{
  u32 tmpreg1 = 0, tmpreg2 = 0;

  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  assert_param(IS_ADC_REGULAR_RANK(Rank));
  assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));

  /* if ADC_Channel_10 ... ADC_Channel_17 is selected [如果ADC_Channel_10 ... ADC_Channel_17被选择]*/
  if (ADC_Channel > ADC_Channel_9)
  {
    /* Get the old register value [取得旧的寄存器值]*/
    tmpreg1 = ADCx->SMPR1;
    /* Calculate the mask to clear [计算需要清除的标志]*/
    tmpreg2 = SMPR1_SMP_Set << (3 * (ADC_Channel - 10));
    /* Clear the old discontinuous mode channel count [清除过去的不间断模式通道计数器]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [计算需要置位的标志]*/
    tmpreg2 = (u32)ADC_SampleTime << (3 * (ADC_Channel - 10));
    /* Set the discontinuous mode channel count [设置不间断模式通道计数器]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [存储新的寄存器值]*/
    ADCx->SMPR1 = tmpreg1;
  }
  else /* ADC_Channel include in ADC_Channel_[0..9] [ADC_Channel 在 ADC_Channel_[0..9]之间]*/
  {
    /* Get the old register value [取得旧的寄存器值]*/
    tmpreg1 = ADCx->SMPR2;
    /* Calculate the mask to clear [计算需要清除的标志]*/
    tmpreg2 = SMPR2_SMP_Set << (3 * ADC_Channel);
    /* Clear the old discontinuous mode channel count [清除过去的不间断模式通道计数器]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [计算需要置位的标志]*/
    tmpreg2 = (u32)ADC_SampleTime << (3 * ADC_Channel);
    /* Set the discontinuous mode channel count [设置不间断模式通道计数器]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [存储新的寄存器值]*/
    ADCx->SMPR2 = tmpreg1;
  }
  /* For Rank 1 to 6 [序列在1到6之间]*/
  if (Rank < 7)
  {
    /* Get the old register value [取得旧的寄存器值]*/
    tmpreg1 = ADCx->SQR3;
    /* Calculate the mask to clear [计算需要清除的标志]*/
    tmpreg2 = SQR3_SQ_Set << (5 * (Rank - 1));
    /* Clear the old SQx bits for the selected rank [根据选择的序列清除旧的SQx位]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [计算需要置位的标志]*/
    tmpreg2 = (u32)ADC_Channel << (5 * (Rank - 1));
    /* Set the SQx bits for the selected rank [根据选择的序列置位新的SQx位]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [存储新的寄存器值]*/
    ADCx->SQR3 = tmpreg1;
  }
  /* For Rank 7 to 12 [序列在7到12之间]*/
  else if (Rank < 13)
  {
    /* Get the old register value [取得旧的寄存器值]*/
    tmpreg1 = ADCx->SQR2;
    /* Calculate the mask to clear [计算需要清除的标志]*/
    tmpreg2 = SQR2_SQ_Set << (5 * (Rank - 7));
    /* Clear the old SQx bits for the selected rank [根据选择的序列清除旧的SQx位]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [计算需要置位的标志]*/
    tmpreg2 = (u32)ADC_Channel << (5 * (Rank - 7));
    /* Set the SQx bits for the selected rank [根据选择的序列置位新的SQx位]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [存储新的寄存器值]*/
    ADCx->SQR2 = tmpreg1;
  }
  /* For Rank 13 to 16 [序列在13到16之间]*/
  else
  {
    /* Get the old register value [取得旧的寄存器值]*/
    tmpreg1 = ADCx->SQR1;
    /* Calculate the mask to clear [计算需要清除的标志]*/
    tmpreg2 = SQR1_SQ_Set << (5 * (Rank - 13));
    /* Clear the old SQx bits for the selected rank [根据选择的序列清除旧的SQx位]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [计算需要置位的标志]*/
    tmpreg2 = (u32)ADC_Channel << (5 * (Rank - 13));
    /* Set the SQx bits for the selected rank [根据选择的序列置位新的SQx位]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [存储新的寄存器值]*/
    ADCx->SQR1 = tmpreg1;
  }
}

/*******************************************************************************
* 函数名称: ADC_ExternalTrigConvCmd
* 功能描述: 使能或禁止外部触发ADCx转换。
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)NewState:选中的由外部触发转换的ADC的新状态这个参数可以是：ENABLE或DISABLE。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_ExternalTrigConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC conversion on external event [允许外部事件触发选择的ADC转换]*/
    ADCx->CR2 |= CR2_EXTTRIG_Set;
  }
  else
  {
    /* Disable the selected ADC conversion on external event [禁止外部事件触发选择的ADC转换]*/
    ADCx->CR2 &= CR2_EXTTRIG_Reset;
  }
}

/*******************************************************************************
* 函数名称: ADC_GetConversionValue
* 功能描述: 返回常规信道最后的ADCx转换结果。
* 输入参数: ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
* 输出参数: 无
* 返回参数: 数据转换结果.
*******************************************************************************/
u16 ADC_GetConversionValue(ADC_TypeDef* ADCx)
{
  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Return the selected ADC conversion value [返回选择的ADC转换值]*/
  return (u16) ADCx->DR;
}

/*******************************************************************************
* 函数名称: ADC_GetDualModeConversionValue
* 功能描述: 返回双重模式下的ADC1和ADC2的最后转换结果
* 输出参数: 无
* 返回参数: 数据转换结果.
*******************************************************************************/
u32 ADC_GetDualModeConversionValue(void)
{
  /* Return the dual mode conversion value [返回双模式转换值]*/
  return (*(vu32 *) DR_ADDRESS);
}

/*******************************************************************************
* 函数名称: ADC_AutoInjectedConvCmd
* 功能描述: 使能/禁止在常规组转换后，被选的ADC进行自动注入组转换。
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)NewState:选中的进行自动注入转换的ADC新状态这个参数可以是：ENABLE或DISABLE。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_AutoInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC automatic injected group conversion [允许选择的ADC自动注入通道组转换]*/
    ADCx->CR1 |= CR1_JAUTO_Set;
  }
  else
  {
    /* Disable the selected ADC automatic injected group conversion [禁止选择的ADC自动注入通道组转换]*/
    ADCx->CR1 &= CR1_JAUTO_Reset;
  }
}

/*******************************************************************************
* 函数名称: ADC_InjectedDiscModeCmd
* 功能描述: 使能/禁止指定ADC注入组信道的非连续模式。
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)NewState:被选中的，注入组信道上为非连续模式的ADC的新状态这个参数可以是：ENABLE或DISABLE。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_InjectedDiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC injected discontinuous mode [允许选的ADC通道注入连续转换模式]*/
    ADCx->CR1 |= CR1_JDISCEN_Set;
  }
  else
  {
    /* Disable the selected ADC injected discontinuous mode [禁止选的ADC通道注入连续转换模式]*/
    ADCx->CR1 &= CR1_JDISCEN_Reset;
  }
}

/*******************************************************************************
* 函数名称: ADC_ExternalTrigInjectedConvConfig
* 功能描述: 为注入信道转换配置ADCx外部触发
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)ADC_ExternalTrigInjecConv:开始注入转换的ADC触发器
*                    ADC_ExternalTrigInjecConv 取值:
*                       - ADC_ExternalTrigInjecConv_T1_TRGO: 定时器1TRGO事件作为注入转换(ADC1,ADC2,ADC3)的外部触发器
*                       - ADC_ExternalTrigInjecConv_T1_CC4: 定时器1捕获比较4作为注入转换(ADC1,ADC2,ADC3)的外部触发器
*                       - ADC_ExternalTrigInjecConv_T2_TRGO: 定时器2TRGO事件作为注入转换(ADC1,ADC2,ADC3)的外部触发器
*                       - ADC_External TrigInjecConv_T2_CC1: 定时器2捕获比较1作为注入转换(ADC1,ADC2,ADC3)的外部触发器
*                       - ADC_ExternalTrigInjecConv_T3_CC4: 定时器3捕获比较4作为注入转换(ADC1,ADC2,ADC3)的外部触发器
*                       - ADC_ExternalTrigInjecConv_T4_TRGO: 定时器4TRGO事件作为注入转换(ADC1,ADC2,ADC3)的外部触发器
*                       - ADC_ExternalTrigInjecConv_Ext_IT15_TIM8_CC4: 外部中断15或者timer8比较事件作为注入转换(ADC1,ADC2,ADC3)的外部触发器
*                       - ADC_External TrigInjecConv_T4_CC3: timer4比较3事件作为注入转换(只ADC3)的外部触发器
*                       - ADC_External TrigInjecConv_T8_CC2: timer8比较2事件作为注入转换(只ADC3)的外部触发器
*                       - ADC_External TrigInjecConv_T8_CC4: timer8比较4事件作为注入转换(只ADC3)的外部触发器
*                       - ADC_ExternalTrigInjecConv_T5_TRGO: 定时器5 TRGO 事件作为注入转换(只ADC3)的外部触发器
*                       - ADC_External TrigInjecConv_T5_CC4: timer5比较4事件作为注入转换(只ADC3)的外部触发器
*                       - ADC_ExternalTrigInjecConv_无: 注入转换由软件启动，而不是由外部触发器启动
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, u32 ADC_ExternalTrigInjecConv)
{
  u32 tmpreg = 0;

  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_EXT_INJEC_TRIG(ADC_ExternalTrigInjecConv));

  /* Get the old register value [取得旧的寄存器值]*/
  tmpreg = ADCx->CR2;
  /* Clear the old external event selection for injected group [清除选择的注入通道群的外部触发事件]*/
  tmpreg &= CR2_JEXTSEL_Reset;
  /* Set the external event selection for injected group [置位选择的注入通道群的外部触发事件]*/
  tmpreg |= ADC_ExternalTrigInjecConv;
  /* Store the new register value [存储新的寄存器值]*/
  ADCx->CR2 = tmpreg;
}

/*******************************************************************************
* 函数名称: ADC_ExternalTrigInjectedConvCmd
* 功能描述: 通过外部触发器使能/禁止ADCx注入信道转换
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)NewState:选中的用于启动注入转换的ADC外部触发器。这个参数可以是:ENABLE或DISABLE�
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC external event selection for injected group [使能选择的ADCx注入信道外部事件触发]*/
    ADCx->CR2 |= CR2_JEXTTRIG_Set;
  }
  else
  {
    /* Disable the selected ADC external event selection for injected group [禁止选择的ADCx注入信道外部事件触发]*/
    ADCx->CR2 &= CR2_JEXTTRIG_Reset;
  }
}

/*******************************************************************************
* 函数名称: ADC_SoftwareStartInjectedConvCmd
* 功能描述: 使能/禁止选中的ADC开始注入信道转换。
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)NewState:被选中的用于启动注入转换的ADC软件的新状态。这个参数可以是:ENABLE或DISABLE。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC conversion for injected group on external event and start the selected
       ADC injected conversion [允许外部事件触发选择的ADC注入通道群模式转换并且启动注入选择]*/
    ADCx->CR2 |= CR2_JEXTTRIG_JSWSTART_Set;
  }
  else
  {
    /* Disable the selected ADC conversion on external event for injected group and stop the selected
       ADC injected conversion [禁止外部事件触发选择的ADC注入通道群模式转换并且启动注入选择]*/
    ADCx->CR2 &= CR2_JEXTTRIG_JSWSTART_Reset;
  }
}

/*******************************************************************************
* 函数名称: ADC_GetSoftwareStartInjectedConvCmdStatus
* 功能描述: 获得选中的由软件启动注入转换的ADC的状态。
* 输入参数: ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
* 输出参数: 无
* 返回参数: 由软件启动注入转换的ADC新状态（SET或者RESET）
*******************************************************************************/
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Check the status of JSWSTART bit [检查状态位JSWSTART]*/
  if ((ADCx->CR2 & CR2_JSWSTART_Set) != (u32)RESET)
  {
    /* JSWSTART bit is set [置位JSWSTART]*/
    bitstatus = SET;
  }
  else
  {
    /* JSWSTART bit is reset [复位JSWSTART]*/
    bitstatus = RESET;
  }

  /* Return the JSWSTART bit status [返回状态位JSWSTART]*/
  return  bitstatus;
}

/*******************************************************************************
* 函数名称: ADC_InjectedChannelConfig
* 功能描述: 配置选中的ADC注入信道相应的音序器（sequencer）等级和采样时间。
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)ADC_Channel: 需要配置的ADC信道.
*                    ADC_Channel 可能的取值：
*                       - ADC_Channel_0: ADC信道0被选择
*                       - ADC_Channel_1: ADC信道1被选择
*                       - ADC_Channel_2: ADC信道2被选择
*                       - ADC_Channel_3: ADC信道3被选择
*                       - ADC_Channel_4: ADC信道4被选择
*                       - ADC_Channel_5: ADC信道5被选择
*                       - ADC_Channel_6: ADC信道6被选择
*                       - ADC_Channel_7: ADC信道7被选择
*                       - ADC_Channel_8: ADC信道8被选择
*                       - ADC_Channel_9: ADC信道9被选择
*                       - ADC_Channel_10: ADC信道10被选择
*                       - ADC_Channel_11: ADC信道11被选择
*                       - ADC_Channel_12: ADC信道12被选择
*                       - ADC_Channel_13: ADC信道13被选择
*                       - ADC_Channel_14: ADC信道14被选择
*                       - ADC_Channel_15: ADC信道15被选择
*                       - ADC_Channel_16: ADC信道16被选择
*                       - ADC_Channel_17: ADC信道17被选择
*           (3)Rank:注入组音序器（sequencer）的等级,选择范围必须在1-4之间
*           (4)ADC_SampleTime: ADC_SampleTime:将要为选中信道设置的采样时间值
*                    ADC_SampleTime.取值:
*                       - ADC_SampleTime_1Cycles5: 采样时间等于1.5个周期
*                       - ADC_SampleTime_7Cycles5: 采样时间等于7.5个周期
*                       - ADC_SampleTime_13Cycles5: 采样时间等于13.5个周期
*                       - ADC_SampleTime_28Cycles5: 采样时间等于28.5个周期
*                       - ADC_SampleTime_41Cycles5: 采样时间等于41.5个周期
*                       - ADC_SampleTime_55Cycles5: 采样时间等于55.5个周期
*                       - ADC_SampleTime_71Cycles5: 采样时间等于71.5个周期
*                       - ADC_SampleTime_239Cycles5: 采样时间等于239.5个周期
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, u8 ADC_Channel, u8 Rank, u8 ADC_SampleTime)
{
  u32 tmpreg1 = 0, tmpreg2 = 0, tmpreg3 = 0;

  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  assert_param(IS_ADC_INJECTED_RANK(Rank));
  assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));

  /* if ADC_Channel_10 ... ADC_Channel_17 is selected [如果ADC_Channel_10 ... ADC_Channel_17被选择]*/
  if (ADC_Channel > ADC_Channel_9)
  {
    /* Get the old register value [取得旧的寄存器值]*/
    tmpreg1 = ADCx->SMPR1;
    /* Calculate the mask to clear [计算需要清除的标志]*/
    tmpreg2 = SMPR1_SMP_Set << (3*(ADC_Channel - 10));
    /* Clear the old discontinuous mode channel count [清除过去的不间断模式通道计数器]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [计算需要置位的标志]*/
    tmpreg2 = (u32)ADC_SampleTime << (3*(ADC_Channel - 10));
    /* Set the discontinuous mode channel count [设置不间断模式通道计数器]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [存储新的寄存器值]*/
    ADCx->SMPR1 = tmpreg1;
  }
  else /* ADC_Channel include in ADC_Channel_[0..9] [ADC_Channel 在 ADC_Channel_[0..9]之间]*/
  {
    /* Get the old register value [取得旧的寄存器值]*/
    tmpreg1 = ADCx->SMPR2;
    /* Calculate the mask to clear [计算需要清除的标志]*/
    tmpreg2 = SMPR2_SMP_Set << (3 * ADC_Channel);
    /* Clear the old discontinuous mode channel count [清除过去的不间断模式通道计数器]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [计算需要置位的标志]*/
    tmpreg2 = (u32)ADC_SampleTime << (3 * ADC_Channel);
    /* Set the discontinuous mode channel count [设置不间断模式通道计数器]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [存储新的寄存器值]*/
    ADCx->SMPR2 = tmpreg1;
  }

  /* Rank configuration [序列配置]*/
  /* Get the old register value [取得旧的寄存器值]*/
  tmpreg1 = ADCx->JSQR;
  /* Get JL value[取得JL值]: Number = JL+1 */
  tmpreg3 =  (tmpreg1 & JSQR_JL_Set)>> 20;
  /* Calculate the mask to clear[计算需要清除的标志]: ((Rank-1)+(4-JL-1)) */
  tmpreg2 = JSQR_JSQ_Set << (5 * (u8)((Rank + 3) - (tmpreg3 + 1)));
  /* Clear the old JSQx bits for the selected rank [清除选中序列旧的JSQx位]*/
  tmpreg1 &= ~tmpreg2;
  /* Calculate the mask to set[计算需要置位的标志]: ((Rank-1)+(4-JL-1)) */
  tmpreg2 = (u32)ADC_Channel << (5 * (u8)((Rank + 3) - (tmpreg3 + 1)));
  /* Set the JSQx bits for the selected rank [置位选中序列旧的JSQx位]*/
  tmpreg1 |= tmpreg2;
  /* Store the new register value [存储新的寄存器值]*/
  ADCx->JSQR = tmpreg1;
}

/*******************************************************************************
* 函数名称: ADC_InjectedSequencerLengthConfig
* 功能描述: 配置注入信道音序器（sequencer）的长度
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)Length:音序器（sequencer）的长度该参数的范围是1－4.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef* ADCx, u8 Length)
{
  u32 tmpreg1 = 0;
  u32 tmpreg2 = 0;

  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_LENGTH(Length));
  
  /* Get the old register value [取得旧的寄存器值]*/
  tmpreg1 = ADCx->JSQR;
  /* Clear the old injected sequnence lenght JL bits [清除注入信道音序器的长度JL位]*/
  tmpreg1 &= JSQR_JL_Reset;
  /* Set the injected sequnence lenght JL bits [置位注入信道音序器的长度JL位]*/
  tmpreg2 = Length - 1; 
  tmpreg1 |= tmpreg2 << 20;
  /* Store the new register value [存储新的寄存器值]*/
  ADCx->JSQR = tmpreg1;
}

/*******************************************************************************
* 函数名称: ADC_SetInjectedOffset
* 功能描述: 设置注入信道转换的偏移值。
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)ADC_InjectedChannel:需要设置偏移值的ADC注入信道。
*                    ADC_InjectedChannel 指定了需要设置偏移值的ADC 注入信道:
*                       - ADC_InjectedChannel_1: 选择注入信道1
*                       - ADC_InjectedChannel_2: 选择注入信道2
*                       - ADC_InjectedChannel_3: 选择注入信道3
*                       - ADC_InjectedChannel_4: 选择注入信道4
*           (3)Offset:选中的ADC注入信道的偏移值。该参数是一个12位值
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_SetInjectedOffset(ADC_TypeDef* ADCx, u8 ADC_InjectedChannel, u16 Offset)
{
  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_CHANNEL(ADC_InjectedChannel));
  assert_param(IS_ADC_OFFSET(Offset));  

  /* Set the selected injected channel data offset [设置选择的注入通道数据偏移]*/
  *((vu32 *)((*(u32*)&ADCx) + ADC_InjectedChannel)) = (u32)Offset;
}

/*******************************************************************************
* 函数名称: ADC_GetInjectedConversionValue
* 功能描述: 返回选中的注入信道的ADC转换结果。
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)ADC_InjectedChannel:需要设置偏移值的ADC注入信道
*              ADC_InjectedChannel 指定了需要设置偏移值的ADC 注入信道:
*                       - ADC_InjectedChannel_1: 选择注入信道1
*                       - ADC_InjectedChannel_2: 选择注入信道2
*                       - ADC_InjectedChannel_3: 选择注入信道3
*                       - ADC_InjectedChannel_4: 选择注入信道4
* 输出参数: 无
* 返回参数: 数据转换结果.
*******************************************************************************/
u16 ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx, u8 ADC_InjectedChannel)
{
  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_CHANNEL(ADC_InjectedChannel));

  /* Returns the selected injected channel conversion data value [返回选择的注入通道转换数据值]*/
  return (u16) (*(vu32*) (((*(u32*)&ADCx) + ADC_InjectedChannel + JDR_Offset)));
}

/*******************************************************************************
* 函数名称: ADC_AnalogWatchdogCmd
* 功能描述: 使能/禁止一个/所有常规信道或者注入信道的模拟看门狗。
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)ADC_AnalogWatchdog: ADC模拟看门狗配置
*                    该参数可能的取值:
*                       - ADC_AnalogWatchdog_SingleRegEnable: 单常规信道上的模拟看门狗
*                       - ADC_AnalogWatchdog_SingleInjecEnable: 单注入信道上的模拟看门狗
*                       - ADC_AnalogWatchdog_SingleRegOrInjecEnable: 单常规信道或者注入信道的模拟看门狗
*                       - ADC_AnalogWatchdog_AllRegEnable: 所有常规信道上的模拟看门狗
*                       - ADC_AnalogWatchdog_AllInjecEnable: 所有注入信道上的模拟看门狗
*                       - ADC_AnalogWatchdog_AllRegAllInjecEnable: 所有常规信道和注入信道上的模拟看门狗
*                       - ADC_AnalogWatchdog_无: 没有信道需要模拟看门狗监视
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, u32 ADC_AnalogWatchdog)
{
  u32 tmpreg = 0;

  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_ANALOG_WATCHDOG(ADC_AnalogWatchdog));

  /* Get the old register value [取得旧的寄存器值]*/
  tmpreg = ADCx->CR1;
  /* Clear AWDEN, AWDENJ and AWDSGL bits [清除AWDEN,AWDENJ和AWDSGL位]*/
  tmpreg &= CR1_AWDMode_Reset;
  /* Set the analog watchdog enable mode [置位模拟看门狗允许模式]*/
  tmpreg |= ADC_AnalogWatchdog;
  /* Store the new register value [存储新的寄存器值]*/
  ADCx->CR1 = tmpreg;
}

/*******************************************************************************
* 函数名称: ADC_AnalogWatchdogThresholdsConfig
* 功能描述: 配置模拟看门狗的高、低阀值.
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)HighThreshold: ADC模拟看门狗最高阀值。该参数是一个12位的数值
*           (3)LowThreshold: ADC模拟看门狗最低阀值。该参数是一个12位的数值
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, u16 HighThreshold,
                                        u16 LowThreshold)
{
  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_THRESHOLD(HighThreshold));
  assert_param(IS_ADC_THRESHOLD(LowThreshold));

  /* Set the ADCx high threshold [设置ADCx的高阀值]*/
  ADCx->HTR = HighThreshold;
  /* Set the ADCx low threshold [设置ADCx的低阀值]*/
  ADCx->LTR = LowThreshold;
}

/*******************************************************************************
* 函数名称: ADC_AnalogWatchdogSingleChannelConfig
* 功能描述: 配置监视单信道的模拟看门狗
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)ADC_Channel:将要为其配置模拟看门狗的信道.
*                    ADC_Channel 可能的取值：
*                       - ADC_Channel_0: ADC信道0被选择
*                       - ADC_Channel_1: ADC信道1被选择
*                       - ADC_Channel_2: ADC信道2被选择
*                       - ADC_Channel_3: ADC信道3被选择
*                       - ADC_Channel_4: ADC信道4被选择
*                       - ADC_Channel_5: ADC信道5被选择
*                       - ADC_Channel_6: ADC信道6被选择
*                       - ADC_Channel_7: ADC信道7被选择
*                       - ADC_Channel_8: ADC信道8被选择
*                       - ADC_Channel_9: ADC信道9被选择
*                       - ADC_Channel_10: ADC信道10被选择
*                       - ADC_Channel_11: ADC信道11被选择
*                       - ADC_Channel_12: ADC信道12被选择
*                       - ADC_Channel_13: ADC信道13被选择
*                       - ADC_Channel_14: ADC信道14被选择
*                       - ADC_Channel_15: ADC信道15被选择
*                       - ADC_Channel_16: ADC信道16被选择
*                       - ADC_Channel_17: ADC信道17被选择
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCx, u8 ADC_Channel)
{
  u32 tmpreg = 0;

  /* Check the parameters 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));

  /* Get the old register value [取得旧的寄存器值]*/
  tmpreg = ADCx->CR1;
  /* Clear the Analog watchdog channel select bits [清除模拟看门狗通道选择位]*/
  tmpreg &= CR1_AWDCH_Reset;
  /* Set the Analog watchdog channel [置位模拟看门狗通道选择位]*/
  tmpreg |= ADC_Channel;
  /* Store the new register value [存储新的寄存器值]*/
  ADCx->CR1 = tmpreg;
}

/*******************************************************************************
* 函数名称: ADC_TempSensorVrefintCmd
* 功能描述: 使能/禁止温度传感和参考电压信道。
* 输入参数: NewState:温度传感和参考电压信道的新状态。该参数可以取两个值：ENABLE或DISABLE。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_TempSensorVrefintCmd(FunctionalState NewState)
{
  /* Check the parameters 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the temperature sensor and Vrefint channel[使能温度传感和参考电压信道]*/
    ADC1->CR2 |= CR2_TSVREFE_Set;
  }
  else
  {
    /* Disable the temperature sensor and Vrefint channel[禁止温度传感和参考电压信道]*/
    ADC1->CR2 &= CR2_TSVREFE_Reset;
  }
}

/*******************************************************************************
* 函数名称: ADC_GetFlagStatus
* 功能描述: 检查指定的ADC标志是否置位.
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)ADC_FLAG:指定需要检查的标志。
*                    给出了ADC_FLAG 的取值:
*                       - ADC_FLAG_AWD: 模拟看门狗标志
*                       - ADC_FLAG_EOC: 转换结束标志
*                       - ADC_FLAG_JEOC: 注入组转换结束标志
*                       - ADC_FLAG_JSTRT: 注入组转换开始标志
*                       - ADC_FLAG_STRT: 常规组转换开始标志
* 输出参数: 无
* 返回参数: ADC_FLAG的新状态(SET或RESET).
*******************************************************************************/
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, u8 ADC_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_GET_FLAG(ADC_FLAG));

  /* Check the status of the specified ADC flag [检查指定ADC状态位]*/
  if ((ADCx->SR & ADC_FLAG) != (u8)RESET)
  {
    /* ADC_FLAG is set [置位ADC_FLAG]*/
    bitstatus = SET;
  }
  else
  {
    /* ADC_FLAG is reset [复位ADC_FLAG]*/
    bitstatus = RESET;
  }

  /* Return the ADC_FLAG status [返回状态位ADC_FLAG]*/
  return  bitstatus;
}

/*******************************************************************************
* 函数名称: ADC_ClearFlag
* 功能描述: 清除ADCx的挂起标志.
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)ADC_FLAG:需要清除的标志。使用"|"可以使得一个或多个标志可以同时被清除。
*                    给出了ADC_FLAG 的取值:
*                       - ADC_FLAG_AWD: 模拟看门狗标志
*                       - ADC_FLAG_EOC: 转换结束标志
*                       - ADC_FLAG_JEOC: 注入组转换结束标志
*                       - ADC_FLAG_JSTRT: 注入组转换开始标志
*                       - ADC_FLAG_STRT: 常规组转换开始标志
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_ClearFlag(ADC_TypeDef* ADCx, u8 ADC_FLAG)
{
  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CLEAR_FLAG(ADC_FLAG));

  /* Clear the selected ADC flags [清除选择的ADC标志]*/
  ADCx->SR = ~(u32)ADC_FLAG;
}

/*******************************************************************************
* 函数名称: ADC_GetITStatus
* 功能描述: 检查指定ADC的中断是否发生.
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)ADC_IT:需要检查的ADC中断源.
*                    可以使用下述值的一个或者几个值的组合:
*                       - ADC_IT_EOC: EOC中断屏蔽
*                       - ADC_IT_AWD: AWDOG中断屏蔽
*                       - ADC_IT_JEOC: JEOC中断屏蔽
* 输出参数: 无
* 返回参数: ADC_IT的新状态（SET或RESET）.
*******************************************************************************/
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, u16 ADC_IT)
{
  ITStatus bitstatus = RESET;
  u32 itmask = 0, enablestatus = 0;

  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_GET_IT(ADC_IT));

  /* Get the ADC IT index [取得ADC中断号]*/
  itmask = ADC_IT >> 8;

  /* Get the ADC_IT enable bit status [取得ADC中断允许标志位]*/
  enablestatus = (ADCx->CR1 & (u8)ADC_IT) ;

  /* Check the status of the specified ADC interrupt [检查指定ADC中断标志位]*/
  if (((ADCx->SR & itmask) != (u32)RESET) && enablestatus)
  {
    /* ADC_IT is set [置位ADC_IT]*/
    bitstatus = SET;
  }
  else
  {
    /* ADC_IT is reset [复位ADC_IT]*/
    bitstatus = RESET;
  }

  /* Return the ADC_IT status [返回ADC_IT状态]*/
  return  bitstatus;
}

/*******************************************************************************
* 函数名称: ADC_ClearITPendingBit
* 功能描述: 清除ADCx的中断挂起位.
* 输入参数: (1)ADCx:其中x可以是1、2或3，用来选择ADC外围模块.
*           (2)ADC_IT:需要检查的ADC中断源.
*                    可以使用下述值的一个或者几个值的组合:
*                       - ADC_IT_EOC: EOC中断屏蔽
*                       - ADC_IT_AWD: AWDOG中断屏蔽
*                       - ADC_IT_JEOC: JEOC中断屏蔽
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, u16 ADC_IT)
{
  u8 itmask = 0;

  /* Check the parameters [检查参数] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_IT(ADC_IT));

  /* Get the ADC IT index [取得ADC中断号]*/
  itmask = (u8)(ADC_IT >> 8);

  /* Clear the selected ADC interrupt pending bits [清除选择的ADC中断挂起位]*/
  ADCx->SR = ~(u32)itmask;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****文件结束****/
