/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_dac.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������DAC�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_dac.h"
#include "stm32f10x_rcc.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
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

/* ���ú� -----------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: DAC_DeInit
* ��������: Deinitializes the DAC peripheral registers to their default
*                  reset values[��ԭDAC����Ĵ�����Ĭ�ϸ�λֵ].
* �������: ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DAC_DeInit(void)
{
  /* Enable DAC reset state [ʹ��DAC��λ״̬]*/
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, ENABLE);
  /* Release DAC from reset state [���DAC��λ״̬]*/
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, DISABLE);
}

/*******************************************************************************
* ��������: DAC_Init
* ��������: Initializes the DAC peripheral according to the specified 
*                  parameters in the DAC_InitStruct[����DAC_InitStructָ���Ĳ�����ʼ��DAC�ⲿ�豸].
* �������: (1) DAC_Channel: the selected DAC channel[ѡ��DACͨ��]. 
*                    This parameter can be one of the following values[������������������ֵ֮һ]:
*                       - DAC_Channel_1: DAC Channel1 selected[ѡ��DACͨ��1]
*                       - DAC_Channel_2: DAC Channel2 selected[ѡ��DACͨ��2]
*           (2) DAC_InitStruct: pointer to a DAC_InitTypeDef structure that
*                    contains the configuration information for the specified
*                    DAC channel.[ָ�������ָ��DACͨ��������Ϣ��DAC_InitTypeDef�ṹָ��]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DAC_Init(u32 DAC_Channel, DAC_InitTypeDef* DAC_InitStruct)
{
  u32 tmpreg1 = 0, tmpreg2 = 0;

  /* Check the DAC parameters [���DAC����]*/
  assert_param(IS_DAC_TRIGGER(DAC_InitStruct->DAC_Trigger));
  assert_param(IS_DAC_GENERATE_WAVE(DAC_InitStruct->DAC_WaveGeneration));
  assert_param(IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(DAC_InitStruct->DAC_LFSRUnmask_TriangleAmplitude));
  assert_param(IS_DAC_OUTPUT_BUFFER_STATE(DAC_InitStruct->DAC_OutputBuffer));

/*---------------------------- DAC CR Configuration --------------------------*/
  /* Get the DAC CR value [���DAC CR��ֵ]*/
  tmpreg1 = DAC->CR;
  /* Clear BOFFx, TENx, TSELx, WAVEx and MAMPx bits [��BOFFx, TENx, TSELx, WAVEx��MAMPxλ]*/
  tmpreg1 &= ~(CR_CLEAR_Mask << DAC_Channel);
  /* Configure for the selected DAC channel: buffer output, trigger, wave generation,
     mask/amplitude for wave generation [����ѡ���DACͨ������������������������η����������η�������ʱ��/���]*/
  /* Set TSELx and TENx bits according to DAC_Trigger value [����DAC_Trigger��ֵ����TSELx��TENxλ]*/
  /* Set WAVEx bits according to DAC_WaveGeneration value [����DAC_WaveGeneration��ֵ����WAVExλ]*/
  /* Set MAMPx bits according to DAC_LFSRUnmask_TriangleAmplitude value [����DAC_LFSRUnmask_TriangleAmplitude��ֵ����MAMPxλ]*/ 
  /* Set BOFFx bit according to DAC_OutputBuffer value [����DAC_OutputBuffer��ֵ����BOFFxλ]*/   
  tmpreg2 = (DAC_InitStruct->DAC_Trigger | DAC_InitStruct->DAC_WaveGeneration |
             DAC_InitStruct->DAC_LFSRUnmask_TriangleAmplitude | DAC_InitStruct->DAC_OutputBuffer);
  /* Calculate CR register value depending on DAC_Channel [����DAC_Channel����CR�Ĵ���ֵ]*/
  tmpreg1 |= tmpreg2 << DAC_Channel;
  /* Write to DAC CR [дDAC CR]*/
  DAC->CR = tmpreg1;
}

/*******************************************************************************
* ��������: DAC_StructInit
* ��������: Fills each DAC_InitStruct member with its default value[��Ĭ��ֵ���DAC_InitStruct�ṹ��ÿһ����Ա].
* �������: - DAC_InitStruct : pointer to a DAC_InitTypeDef structure
*                    which will be initialized[ָ��Ҫ����ʼ����DAC_InitTypeDef�ṹָ��].
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct)
{
/*------- Reset DAC init structure parameters values [��λDAC��ʼ���ṹ����ֵ]*/
  /* Initialize the DAC_Trigger member [��ʼ��DAC_Trigger��Ա]*/
  DAC_InitStruct->DAC_Trigger = DAC_Trigger_None;

  /* Initialize the DAC_WaveGeneration member [��ʼ��DAC_WaveGeneration��Ա]*/
  DAC_InitStruct->DAC_WaveGeneration = DAC_WaveGeneration_None;

  /* Initialize the DAC_LFSRUnmask_TriangleAmplitude member [��ʼ��DAC_LFSRUnmask_TriangleAmplitude��Ա]*/
  DAC_InitStruct->DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;

  /* Initialize the DAC_OutputBuffer member [��ʼ��DAC_OutputBuffer��Ա]*/
  DAC_InitStruct->DAC_OutputBuffer = DAC_OutputBuffer_Enable;
}

/*******************************************************************************
* ��������: DAC_Cmd
* ��������: Enables or disables the specified DAC channel[ʹ�ܻ��ָֹ����DACͨ��].
* �������  (1) DAC_Channel: the selected DAC channel[ѡ���DACͨ��]. 
*                    This parameter can be one of the following values:[�����������ѡ�������ֵ֮һ]
*                       - DAC_Channel_1: DAC Channel1 selected[ѡ��DACͨ��1]
*                       - DAC_Channel_2: DAC Channel2 selected[ѡ��DACͨ��2]
*           (2) NewState: new state of the DAC channel. [DACͨ������״̬]
*                    This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DAC_Cmd(u32 DAC_Channel, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected DAC channel [ʹ��ѡ���DACͨ��]*/
    DAC->CR |= CR_EN_Set << DAC_Channel;
  }
  else
  {
    /* Disable the selected DAC channel [��ֹѡ���DACͨ��]*/
    DAC->CR &= ~(CR_EN_Set << DAC_Channel);
  }
}

/*******************************************************************************
* ��������: DAC_DMACmd
* ��������: Enables or disables the specified DAC channel DMA request[ʹ�ܻ��ָֹ����DACͨ������DMA].
* �������  (1) DAC_Channel: the selected DAC channel[ѡ���DACͨ��]. 
*                    This parameter can be one of the following values:[�����������ѡ�������ֵ֮һ]
*                       - DAC_Channel_1: DAC Channel1 selected[ѡ��DACͨ��1]
*                       - DAC_Channel_2: DAC Channel2 selected[ѡ��DACͨ��2]
*           (2) NewState: new state of the selected DAC channel DMA request[ѡ���DACͨ��DMA������״̬].
*                    This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DAC_DMACmd(u32 DAC_Channel, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected DAC channel DMA request [ʹ��ѡ���DACͨ��DMA����]*/
    DAC->CR |= CR_DMAEN_Set << DAC_Channel;
  }
  else
  {
    /* Disable the selected DAC channel DMA request [��ֹѡ���DACͨ��DMA����]*/
    DAC->CR &= ~(CR_DMAEN_Set << DAC_Channel);
  }
}

/*******************************************************************************
* ��������: DAC_SoftwareTriggerCmd
* ��������: Enables or disables the selected DAC channel software trigger[ʹ�ܻ��ֹѡ���DACͨ���������].
* �������  (1) DAC_Channel: the selected DAC channel[ѡ���DACͨ��]. 
*                    This parameter can be one of the following values[�����������ѡ�������ֵ֮һ]:
*                       - DAC_Channel_1: DAC Channel1 selected[ѡ��DACͨ��1]
*                       - DAC_Channel_2: DAC Channel2 selected[ѡ��DACͨ��2]
*           (2) NewState: new state of the selected DAC channel software trigger[ѡ���DACͨ�����������״̬].
*                    This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DAC_SoftwareTriggerCmd(u32 DAC_Channel, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable software trigger for the selected DAC channel [ʹ��ѡ���DACͨ���������]*/
    DAC->SWTRIGR |= SWTRIGR_SWTRIG_Set << (DAC_Channel >> 4);
  }
  else
  {
    /* Disable software trigger for the selected DAC channel [��ֹѡ���DACͨ���������]*/
    DAC->SWTRIGR &= ~(SWTRIGR_SWTRIG_Set << (DAC_Channel >> 4));
  }
}

/*******************************************************************************
* ��������: DAC_DualSoftwareTriggerCmd
* ��������: Enables or disables simultaneously the two DAC channels software
*                  triggers[ʹ�ܻ��ֹ����DACͨ��ͬ���������].
* �������  - NewState: new state of the DAC channels software triggers[DACͨ�����������״̬].
*                    This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DAC_DualSoftwareTriggerCmd(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable software trigger for both DAC channels [ʹ�������������DACͨ��]*/
    DAC->SWTRIGR |= DUAL_SWTRIG_Set ;
  }
  else
  {
    /* Disable software trigger for both DAC channels [��ֹ�����������DACͨ��]*/
    DAC->SWTRIGR &= DUAL_SWTRIG_Reset;
  }
}

/*******************************************************************************
* ��������: DAC_WaveGenerationCmd
* ��������: Enables or disables the selected DAC channel wave generation[ʹ�ܻ��ֹѡ���DACͨ�����η���].
* �������  (1) DAC_Channel: the selected DAC channel[ѡ���DACͨ��]. 
*                    This parameter can be one of the following values[�����������ѡ�������ֵ֮һ]:
*                       - DAC_Channel_1: DAC Channel1 selected[ѡ��DACͨ��1]
*                       - DAC_Channel_2: DAC Channel2 selected[ѡ��DACͨ��2]
*           (2) DAC_Wave: Specifies the wave type to enable or disable[ָ���Ĳ�������].
*                    This parameter can be one of the following values[�����������ѡ�������ֵ֮һ]:
*                       - DAC_Wave_Noise: noise wave generation[�������η���]
*                       - DAC_Wave_Triangle: triangle wave generation[���ǲ��η���]
*                  - NewState: new state of the selected DAC channel wave generation[DACͨ�����η�����״̬].
*                    This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DAC_WaveGenerationCmd(u32 DAC_Channel, u32 DAC_Wave, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_DAC_WAVE(DAC_Wave)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected wave generation for the selected DAC channel [ʹ��ѡ���DACͨ��ѡ��Ĳ���]*/
    DAC->CR |= DAC_Wave << DAC_Channel;
  }
  else
  {
    /* Disable the selected wave generation for the selected DAC channel [��ֹѡ���DACͨ��ѡ��Ĳ���]*/
    DAC->CR &= ~(DAC_Wave << DAC_Channel);
  }
}

/*******************************************************************************
* ��������: DAC_SetChannel1Data
* ��������: Set the specified data holding register value for DAC channel1[����DACͨ��1ָ�������ݱ��ּĴ���ֵ].
* �������: (1) DAC_Align: Specifies the data alignment for DAC channel1[DACͨ��1ָ�������ݶ���].
*                    This parameter can be one of the following values[�����������ѡ�������ֵ֮һ]:
*                       - DAC_Align_8b_R: 8bit right data alignement selected[ѡ��8λ�����Ҷ���]
*                       - DAC_Align_12b_L: 12bit left data alignement selected[ѡ��12λ���������]
*                       - DAC_Align_12b_R: 12bit right data alignement selected[ѡ��12λ�����Ҷ���]
*           (2) Data : Data to be loaded in the selected data holding 
*                    register[װ��ѡ������ݱ��ּĴ���������].
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DAC_SetChannel1Data(u32 DAC_Align, u16 Data)
{
  /* Check the parameters [������]*/
  assert_param(IS_DAC_ALIGN(DAC_Align));
  assert_param(IS_DAC_DATA(Data));

  /* Set the DAC channel1 selected data holding register [����DACͨ��1ѡ������ݱ��ּĴ���]*/
  *((vu32 *)(DAC_BASE + DHR12R1_Offset + DAC_Align)) = (u32)Data;
}

/*******************************************************************************
* ��������: DAC_SetChannel2Data
* ��������: Set the specified data holding register value for DAC channel2[����DACͨ��2ָ�������ݱ��ּĴ���ֵ].
* �������: (1) DAC_Align: Specifies the data alignement for DAC channel2[DACͨ��2ָ�������ݶ���].
*                    This parameter can be one of the following values:[�����������ѡ�������ֵ֮һ]
*                       - DAC_Align_8b_R: 8bit right data alignement selected[ѡ��8λ�����Ҷ���]
*                       - DAC_Align_12b_L: 12bit left data alignement selected[ѡ��12λ���������]
*                       - DAC_Align_12b_R: 12bit right data alignement selected[ѡ��12λ�����Ҷ���]
*           (2) Data : Data to be loaded in the selected data holding 
*                    register.[װ��ѡ������ݱ��ּĴ���������]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DAC_SetChannel2Data(u32 DAC_Align, u16 Data)
{
  /* Check the parameters [������]*/
  assert_param(IS_DAC_ALIGN(DAC_Align));
  assert_param(IS_DAC_DATA(Data));

  /* Set the DAC channel2 selected data holding register [����DACͨ��2ѡ������ݱ��ּĴ���]*/
  *((vu32 *)(DAC_BASE + DHR12R2_Offset + DAC_Align)) = (u32)Data;
}

/*******************************************************************************
* ��������: DAC_SetDualChannelData
* ��������: Set the specified data holding register value for dual channel
*                  DAC[Ϊ˫ͨ��DAC����ָ�������ݱ��ּĴ���ֵ].
* �������: (1) DAC_Align: Specifies the data alignment for dual channel DAC[Ϊ˫ͨ��DACָ�����ݶ���].
*                    This parameter can be one of the following values:[�����������ѡ�������ֵ֮һ]
*                       - DAC_Align_8b_R: 8bit right data alignement selected[ѡ��8λ�����Ҷ���]
*                       - DAC_Align_12b_L: 12bit left data alignement selected[ѡ��12λ���������]
*                       - DAC_Align_12b_R: 12bit right data alignement selected[ѡ��12λ�����Ҷ���]
*           (2) Data2: Data for DAC Channel2 to be loaded in the selected data 
*                    holding register[ΪDACͨ��2װ�ص�ѡ������ݱ��ּĴ���������].
*           (3) Data1: Data for DAC Channel1 to be loaded in the selected data 
*                    holding register[ΪDACͨ��1װ�ص�ѡ������ݱ��ּĴ���������].
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DAC_SetDualChannelData(u32 DAC_Align, u16 Data2, u16 Data1)
{
  u32 data = 0;

  /* Check the parameters [������]*/
  assert_param(IS_DAC_ALIGN(DAC_Align));
  assert_param(IS_DAC_DATA(Data1));
  assert_param(IS_DAC_DATA(Data2));
  
  /* Calculate and set dual DAC data holding register value [���㲢����˫DAC���ݱ��ּĴ�����ֵ]*/
  if (DAC_Align == DAC_Align_8b_R)
  {
    data = ((u32)Data2 << 8) | Data1; 
  }
  else
  {
    data = ((u32)Data2 << 16) | Data1;
  }

  /* Set the dual DAC selected data holding register [����˫DACѡ������ݱ��ּĴ���]*/
  *((vu32 *)(DAC_BASE + DHR12RD_Offset + DAC_Align)) = data;
}

/*******************************************************************************
* ��������: DAC_GetDataOutputValue
* ��������: Returns the last data output value of the selected DAC cahnnel[�������µ�DACͨ�����ݼĴ������ֵ].
* �������  - DAC_Channel: the selected DAC channel[ѡ���DACͨ��]. 
*                    This parameter can be one of the following values[�����������ѡ�������ֵ֮һ]:
*                       - DAC_Channel_1: DAC Channel1 selected[ѡ��DACͨ��1]
*                       - DAC_Channel_2: DAC Channel2 selected[ѡ��DACͨ��2]
* �������: ��
* ���ز���: The selected DAC channel data output value[ѡ���DACͨ���������ֵ].
*******************************************************************************/
u16 DAC_GetDataOutputValue(u32 DAC_Channel)
{
  /* Check the parameters [������]*/
  assert_param(IS_DAC_CHANNEL(DAC_Channel));

  /* Returns the DAC channel data output register value [����DACͨ�����ݼĴ������ֵ]*/
  return (u16) (*(vu32*)(DAC_BASE + DOR_Offset + ((u32)DAC_Channel >> 2)));
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
