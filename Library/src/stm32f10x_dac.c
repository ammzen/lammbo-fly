/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_dac.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有DAC的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_dac.h"
#include "stm32f10x_rcc.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
/* DAC EN mask */
#define CR_EN_Set                  ((u32)0x00000001)

/* DAC DMAEN mask */
#define CR_DMAEN_Set               ((u32)0x00001000)

/* CR register Mask */
#define CR_CLEAR_Mask              ((u32)0x00000FFE)

/* DAC SWTRIG mask */
#define SWTRIGR_SWTRIG_Set         ((u32)0x00000001)

/* DAC Dual Channels SWTRIG masks */
#define DUAL_SWTRIG_Set            ((u32)0x00000003)
#define DUAL_SWTRIG_Reset          ((u32)0xFFFFFFFC)

/* DHR registers offsets */
#define DHR12R1_Offset             ((u32)0x00000008)
#define DHR12R2_Offset             ((u32)0x00000014)
#define DHR12RD_Offset             ((u32)0x00000020)

/* DOR register offset */
#define DOR_Offset                 ((u32)0x0000002C)

/* 自用宏 -----------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: DAC_DeInit
* 功能描述: Deinitializes the DAC peripheral registers to their default
*                  reset values[还原DAC外设寄存器到默认复位值].
* 输入参数: 无.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DAC_DeInit(void)
{
  /* Enable DAC reset state [使能DAC复位状态]*/
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, ENABLE);
  /* Release DAC from reset state [解除DAC复位状态]*/
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, DISABLE);
}

/*******************************************************************************
* 函数名称: DAC_Init
* 功能描述: Initializes the DAC peripheral according to the specified 
*                  parameters in the DAC_InitStruct[依照DAC_InitStruct指定的参数初始化DAC外部设备].
* 输入参数: (1) DAC_Channel: the selected DAC channel[选择DAC通道]. 
*                    This parameter can be one of the following values[这个参数可以是下面的值之一]:
*                       - DAC_Channel_1: DAC Channel1 selected[选择DAC通道1]
*                       - DAC_Channel_2: DAC Channel2 selected[选择DAC通道2]
*           (2) DAC_InitStruct: pointer to a DAC_InitTypeDef structure that
*                    contains the configuration information for the specified
*                    DAC channel.[指向包含了指定DAC通道配置信息的DAC_InitTypeDef结构指针]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DAC_Init(u32 DAC_Channel, DAC_InitTypeDef* DAC_InitStruct)
{
  u32 tmpreg1 = 0, tmpreg2 = 0;

  /* Check the DAC parameters [检查DAC参数]*/
  assert_param(IS_DAC_TRIGGER(DAC_InitStruct->DAC_Trigger));
  assert_param(IS_DAC_GENERATE_WAVE(DAC_InitStruct->DAC_WaveGeneration));
  assert_param(IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(DAC_InitStruct->DAC_LFSRUnmask_TriangleAmplitude));
  assert_param(IS_DAC_OUTPUT_BUFFER_STATE(DAC_InitStruct->DAC_OutputBuffer));

/*---------------------------- DAC CR Configuration --------------------------*/
  /* Get the DAC CR value [获得DAC CR的值]*/
  tmpreg1 = DAC->CR;
  /* Clear BOFFx, TENx, TSELx, WAVEx and MAMPx bits [清BOFFx, TENx, TSELx, WAVEx和MAMPx位]*/
  tmpreg1 &= ~(CR_CLEAR_Mask << DAC_Channel);
  /* Configure for the selected DAC channel: buffer output, trigger, wave generation,
     mask/amplitude for wave generation [配置选择的DAC通道：缓存输出，触发器，波形发生器，波形发生器的时标/振幅]*/
  /* Set TSELx and TENx bits according to DAC_Trigger value [依照DAC_Trigger的值设置TSELx和TENx位]*/
  /* Set WAVEx bits according to DAC_WaveGeneration value [依照DAC_WaveGeneration的值设置WAVEx位]*/
  /* Set MAMPx bits according to DAC_LFSRUnmask_TriangleAmplitude value [依照DAC_LFSRUnmask_TriangleAmplitude的值设置MAMPx位]*/ 
  /* Set BOFFx bit according to DAC_OutputBuffer value [依照DAC_OutputBuffer的值设置BOFFx位]*/   
  tmpreg2 = (DAC_InitStruct->DAC_Trigger | DAC_InitStruct->DAC_WaveGeneration |
             DAC_InitStruct->DAC_LFSRUnmask_TriangleAmplitude | DAC_InitStruct->DAC_OutputBuffer);
  /* Calculate CR register value depending on DAC_Channel [根据DAC_Channel计算CR寄存器值]*/
  tmpreg1 |= tmpreg2 << DAC_Channel;
  /* Write to DAC CR [写DAC CR]*/
  DAC->CR = tmpreg1;
}

/*******************************************************************************
* 函数名称: DAC_StructInit
* 功能描述: Fills each DAC_InitStruct member with its default value[用默认值填充DAC_InitStruct结构的每一个成员].
* 输入参数: - DAC_InitStruct : pointer to a DAC_InitTypeDef structure
*                    which will be initialized[指向将要被初始化的DAC_InitTypeDef结构指针].
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct)
{
/*------- Reset DAC init structure parameters values [复位DAC初始化结构参数值]*/
  /* Initialize the DAC_Trigger member [初始化DAC_Trigger成员]*/
  DAC_InitStruct->DAC_Trigger = DAC_Trigger_None;

  /* Initialize the DAC_WaveGeneration member [初始化DAC_WaveGeneration成员]*/
  DAC_InitStruct->DAC_WaveGeneration = DAC_WaveGeneration_None;

  /* Initialize the DAC_LFSRUnmask_TriangleAmplitude member [初始化DAC_LFSRUnmask_TriangleAmplitude成员]*/
  DAC_InitStruct->DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;

  /* Initialize the DAC_OutputBuffer member [初始化DAC_OutputBuffer成员]*/
  DAC_InitStruct->DAC_OutputBuffer = DAC_OutputBuffer_Enable;
}

/*******************************************************************************
* 函数名称: DAC_Cmd
* 功能描述: Enables or disables the specified DAC channel[使能或禁止指定的DAC通道].
* 输入参数  (1) DAC_Channel: the selected DAC channel[选择的DAC通道]. 
*                    This parameter can be one of the following values:[这个参数可以选择下面的值之一]
*                       - DAC_Channel_1: DAC Channel1 selected[选择DAC通道1]
*                       - DAC_Channel_2: DAC Channel2 selected[选择DAC通道2]
*           (2) NewState: new state of the DAC channel. [DAC通道的新状态]
*                    This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DAC_Cmd(u32 DAC_Channel, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected DAC channel [使能选择的DAC通道]*/
    DAC->CR |= CR_EN_Set << DAC_Channel;
  }
  else
  {
    /* Disable the selected DAC channel [禁止选择的DAC通道]*/
    DAC->CR &= ~(CR_EN_Set << DAC_Channel);
  }
}

/*******************************************************************************
* 函数名称: DAC_DMACmd
* 功能描述: Enables or disables the specified DAC channel DMA request[使能或禁止指定的DAC通道请求DMA].
* 输入参数  (1) DAC_Channel: the selected DAC channel[选择的DAC通道]. 
*                    This parameter can be one of the following values:[这个参数可以选择下面的值之一]
*                       - DAC_Channel_1: DAC Channel1 selected[选择DAC通道1]
*                       - DAC_Channel_2: DAC Channel2 selected[选择DAC通道2]
*           (2) NewState: new state of the selected DAC channel DMA request[选择的DAC通道DMA请求新状态].
*                    This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DAC_DMACmd(u32 DAC_Channel, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected DAC channel DMA request [使能选择的DAC通道DMA请求]*/
    DAC->CR |= CR_DMAEN_Set << DAC_Channel;
  }
  else
  {
    /* Disable the selected DAC channel DMA request [禁止选择的DAC通道DMA请求]*/
    DAC->CR &= ~(CR_DMAEN_Set << DAC_Channel);
  }
}

/*******************************************************************************
* 函数名称: DAC_SoftwareTriggerCmd
* 功能描述: Enables or disables the selected DAC channel software trigger[使能或禁止选择的DAC通道软件触发].
* 输入参数  (1) DAC_Channel: the selected DAC channel[选择的DAC通道]. 
*                    This parameter can be one of the following values[这个参数可以选择下面的值之一]:
*                       - DAC_Channel_1: DAC Channel1 selected[选择DAC通道1]
*                       - DAC_Channel_2: DAC Channel2 selected[选择DAC通道2]
*           (2) NewState: new state of the selected DAC channel software trigger[选择的DAC通道软件触发新状态].
*                    This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DAC_SoftwareTriggerCmd(u32 DAC_Channel, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable software trigger for the selected DAC channel [使能选择的DAC通道软件触发]*/
    DAC->SWTRIGR |= SWTRIGR_SWTRIG_Set << (DAC_Channel >> 4);
  }
  else
  {
    /* Disable software trigger for the selected DAC channel [禁止选择的DAC通道软件触发]*/
    DAC->SWTRIGR &= ~(SWTRIGR_SWTRIG_Set << (DAC_Channel >> 4));
  }
}

/*******************************************************************************
* 函数名称: DAC_DualSoftwareTriggerCmd
* 功能描述: Enables or disables simultaneously the two DAC channels software
*                  triggers[使能或禁止两个DAC通道同步软件触发].
* 输入参数  - NewState: new state of the DAC channels software triggers[DAC通道软件触发新状态].
*                    This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DAC_DualSoftwareTriggerCmd(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable software trigger for both DAC channels [使能软件触发两个DAC通道]*/
    DAC->SWTRIGR |= DUAL_SWTRIG_Set ;
  }
  else
  {
    /* Disable software trigger for both DAC channels [禁止软件触发两个DAC通道]*/
    DAC->SWTRIGR &= DUAL_SWTRIG_Reset;
  }
}

/*******************************************************************************
* 函数名称: DAC_WaveGenerationCmd
* 功能描述: Enables or disables the selected DAC channel wave generation[使能或禁止选择的DAC通道波形发生].
* 输入参数  (1) DAC_Channel: the selected DAC channel[选择的DAC通道]. 
*                    This parameter can be one of the following values[这个参数可以选择下面的值之一]:
*                       - DAC_Channel_1: DAC Channel1 selected[选择DAC通道1]
*                       - DAC_Channel_2: DAC Channel2 selected[选择DAC通道2]
*           (2) DAC_Wave: Specifies the wave type to enable or disable[指定的波形类型].
*                    This parameter can be one of the following values[这个参数可以选择下面的值之一]:
*                       - DAC_Wave_Noise: noise wave generation[噪音波形发生]
*                       - DAC_Wave_Triangle: triangle wave generation[三角波形发生]
*                  - NewState: new state of the selected DAC channel wave generation[DAC通道波形发生新状态].
*                    This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DAC_WaveGenerationCmd(u32 DAC_Channel, u32 DAC_Wave, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_DAC_WAVE(DAC_Wave)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected wave generation for the selected DAC channel [使能选择的DAC通道选择的波形]*/
    DAC->CR |= DAC_Wave << DAC_Channel;
  }
  else
  {
    /* Disable the selected wave generation for the selected DAC channel [禁止选择的DAC通道选择的波形]*/
    DAC->CR &= ~(DAC_Wave << DAC_Channel);
  }
}

/*******************************************************************************
* 函数名称: DAC_SetChannel1Data
* 功能描述: Set the specified data holding register value for DAC channel1[设置DAC通道1指定的数据保持寄存器值].
* 输入参数: (1) DAC_Align: Specifies the data alignment for DAC channel1[DAC通道1指定的数据对齐].
*                    This parameter can be one of the following values[这个参数可以选择下面的值之一]:
*                       - DAC_Align_8b_R: 8bit right data alignement selected[选择8位数据右对齐]
*                       - DAC_Align_12b_L: 12bit left data alignement selected[选择12位数据左对齐]
*                       - DAC_Align_12b_R: 12bit right data alignement selected[选择12位数据右对齐]
*           (2) Data : Data to be loaded in the selected data holding 
*                    register[装入选择的数据保持寄存器的数据].
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DAC_SetChannel1Data(u32 DAC_Align, u16 Data)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_DAC_ALIGN(DAC_Align));
  assert_param(IS_DAC_DATA(Data));

  /* Set the DAC channel1 selected data holding register [设置DAC通道1选择的数据保持寄存器]*/
  *((vu32 *)(DAC_BASE + DHR12R1_Offset + DAC_Align)) = (u32)Data;
}

/*******************************************************************************
* 函数名称: DAC_SetChannel2Data
* 功能描述: Set the specified data holding register value for DAC channel2[设置DAC通道2指定的数据保持寄存器值].
* 输入参数: (1) DAC_Align: Specifies the data alignement for DAC channel2[DAC通道2指定的数据对齐].
*                    This parameter can be one of the following values:[这个参数可以选择下面的值之一]
*                       - DAC_Align_8b_R: 8bit right data alignement selected[选择8位数据右对齐]
*                       - DAC_Align_12b_L: 12bit left data alignement selected[选择12位数据左对齐]
*                       - DAC_Align_12b_R: 12bit right data alignement selected[选择12位数据右对齐]
*           (2) Data : Data to be loaded in the selected data holding 
*                    register.[装入选择的数据保持寄存器的数据]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DAC_SetChannel2Data(u32 DAC_Align, u16 Data)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_DAC_ALIGN(DAC_Align));
  assert_param(IS_DAC_DATA(Data));

  /* Set the DAC channel2 selected data holding register [设置DAC通道2选择的数据保持寄存器]*/
  *((vu32 *)(DAC_BASE + DHR12R2_Offset + DAC_Align)) = (u32)Data;
}

/*******************************************************************************
* 函数名称: DAC_SetDualChannelData
* 功能描述: Set the specified data holding register value for dual channel
*                  DAC[为双通道DAC设置指定的数据保持寄存器值].
* 输入参数: (1) DAC_Align: Specifies the data alignment for dual channel DAC[为双通道DAC指定数据对齐].
*                    This parameter can be one of the following values:[这个参数可以选择下面的值之一]
*                       - DAC_Align_8b_R: 8bit right data alignement selected[选择8位数据右对齐]
*                       - DAC_Align_12b_L: 12bit left data alignement selected[选择12位数据左对齐]
*                       - DAC_Align_12b_R: 12bit right data alignement selected[选择12位数据右对齐]
*           (2) Data2: Data for DAC Channel2 to be loaded in the selected data 
*                    holding register[为DAC通道2装载到选择的数据保持寄存器的数据].
*           (3) Data1: Data for DAC Channel1 to be loaded in the selected data 
*                    holding register[为DAC通道1装载到选择的数据保持寄存器的数据].
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DAC_SetDualChannelData(u32 DAC_Align, u16 Data2, u16 Data1)
{
  u32 data = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_DAC_ALIGN(DAC_Align));
  assert_param(IS_DAC_DATA(Data1));
  assert_param(IS_DAC_DATA(Data2));
  
  /* Calculate and set dual DAC data holding register value [计算并设置双DAC数据保持寄存器的值]*/
  if (DAC_Align == DAC_Align_8b_R)
  {
    data = ((u32)Data2 << 8) | Data1; 
  }
  else
  {
    data = ((u32)Data2 << 16) | Data1;
  }

  /* Set the dual DAC selected data holding register [设置双DAC选择的数据保持寄存器]*/
  *((vu32 *)(DAC_BASE + DHR12RD_Offset + DAC_Align)) = data;
}

/*******************************************************************************
* 函数名称: DAC_GetDataOutputValue
* 功能描述: Returns the last data output value of the selected DAC cahnnel[返回最新的DAC通道数据寄存器输出值].
* 输入参数  - DAC_Channel: the selected DAC channel[选择的DAC通道]. 
*                    This parameter can be one of the following values[这个参数可以选择下面的值之一]:
*                       - DAC_Channel_1: DAC Channel1 selected[选择DAC通道1]
*                       - DAC_Channel_2: DAC Channel2 selected[选择DAC通道2]
* 输出参数: 无
* 返回参数: The selected DAC channel data output value[选择的DAC通道数据输出值].
*******************************************************************************/
u16 DAC_GetDataOutputValue(u32 DAC_Channel)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_DAC_CHANNEL(DAC_Channel));

  /* Returns the DAC channel data output register value [返回DAC通道数据寄存器输出值]*/
  return (u16) (*(vu32*)(DAC_BASE + DOR_Offset + ((u32)DAC_Channel >> 2)));
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
