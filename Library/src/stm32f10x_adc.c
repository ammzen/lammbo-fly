/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_adc.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������ADC�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ----------------------------------------------------------------*/
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"

/* �������� ------------------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------------*/
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

/* ���ú� -----------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: ADC_DeInit
* ��������: ��λADCx��Χģ��Ĵ�����ʹ��ΪĬ��ֵ��
* �������: ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_DeInit(ADC_TypeDef* ADCx)
{
  /* Check the parameters [������ ������]*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  switch (*(u32*)&ADCx)
  {
    case ADC1_BASE:
      /* Enable ADC1 reset state [����ADC1��λ״̬]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
      /* Release ADC1 from reset state [���ADC1��λ״̬]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
      break;
    
    case ADC2_BASE:
      /* Enable ADC2 reset state [����ADC2��λ״̬]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);
      /* Release ADC2 from reset state [���ADC2��λ״̬]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);
      break;
      
    case ADC3_BASE:
      /* Enable ADC3 reset state [����ADC3��λ״̬]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);
      /* Release ADC3 from reset state [���ADC3��λ״̬]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);
      break; 

    default:
      break;
  }
}

/*******************************************************************************
* ��������: ADC_Init
* ��������: ����ADC_InitStruct ��ָ���Ĳ�����ʼ��ADCx ��Χģ�顣
* �������: (1)ADCx: ����x ������1 2��3������ѡ��ADC ��Χģ��.
*           (2)ADC_InitStruc��ָ��ṹ��ADC_InitTypeDef ��ָ��,�ýṹ������ָ��ADC��Χģ���������Ϣ��
* �������: ��
* ���ز���: ��
******************************************************************************/
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
{
  u32 tmpreg1 = 0;
  u8 tmpreg2 = 0;

  /* Check the parameters [������ ������]*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_MODE(ADC_InitStruct->ADC_Mode));
  assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ScanConvMode));
  assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ContinuousConvMode));            
  assert_param(IS_ADC_EXT_TRIG(ADC_InitStruct->ADC_ExternalTrigConv));   
  assert_param(IS_ADC_DATA_ALIGN(ADC_InitStruct->ADC_DataAlign)); 
  assert_param(IS_ADC_REGULAR_LENGTH(ADC_InitStruct->ADC_NbrOfChannel));

  /*---------------------------- ADCx CR1 Configuration [ADCx CR1����]-----------------*/
  /* Get the ADCx CR1 value [�õ�ADCx CR1��ֵ]*/
  tmpreg1 = ADCx->CR1;
  /* Clear DUALMOD and SCAN bits [���DUALMOD��SCANλ]*/
  tmpreg1 &= CR1_CLEAR_Mask;
  /* Configure ADCx: Dual mode and scan conversion mode [����ADCx:˫��ģʽ��ɨ��ת��ģʽ]*/
  /* Set DUALMOD bits according to ADC_Mode value [����DUALMODλ����ADCģʽֵ]*/
  /* Set SCAN bit according to ADC_ScanConvMode value */
  tmpreg1 |= (u32)(ADC_InitStruct->ADC_Mode | ((u32)ADC_InitStruct->ADC_ScanConvMode << 8));
  /* Write to ADCx CR1 [д��ADCx CR1]*/
  ADCx->CR1 = tmpreg1;

  /*---------------------------- ADCx CR2 Configuration [ADCx CR2����]-----------------*/
  /* Get the ADCx CR2 value [�õ�ADCx CR2��ֵ]*/
  tmpreg1 = ADCx->CR2;
  /* Clear CONT, ALIGN and EXTSEL bits [���CONT,ALIGN��EXTSELλ]*/
  tmpreg1 &= CR2_CLEAR_Mask;
  /* Configure ADCx: external trigger event and continuous conversion mode [����ADCx:������ʱ�������ת��ģʽ]*/
  /* Set ALIGN bit according to ADC_DataAlign value [����ADC_DataAlign��ֵ����ALIGNλ]*/
  /* Set EXTSEL bits according to ADC_ExternalTrigConv value [����ADC_ExternalTrigConv��ֵ����EXTSELλ]*/
  /* Set CONT bit according to ADC_ContinuousConvMode value [����ADC_ContinuousConvMode��ֵ����CONTλ]*/
  tmpreg1 |= (u32)(ADC_InitStruct->ADC_DataAlign | ADC_InitStruct->ADC_ExternalTrigConv |
            ((u32)ADC_InitStruct->ADC_ContinuousConvMode << 1));
  /* Write to ADCx CR2 [д��ADCx CR2]*/
  ADCx->CR2 = tmpreg1;

  /*---------------------------- ADCx SQR1 Configuration [ADCx SQR1����]-----------------*/
  /* Get the ADCx SQR1 value [ȡ��ADCx SQR1��ֵ]*/
  tmpreg1 = ADCx->SQR1;
  /* Clear L bits [���Lλ]*/
  tmpreg1 &= SQR1_CLEAR_Mask;
  /* Configure ADCx: regular channel sequence length [����ADCx:�����ŵ�����]*/
  /* Set L bits according to ADC_NbrOfChannel value [����ADC_NbrOfChannel��ֵ����Lλ]*/
  tmpreg2 |= (ADC_InitStruct->ADC_NbrOfChannel - 1);
  tmpreg1 |= ((u32)tmpreg2 << 20);
  /* Write to ADCx SQR1 [д��ADCx SQR1]*/
  ADCx->SQR1 = tmpreg1;
}

/*******************************************************************************
* ��������: ADC_StructInit
* ��������: ʹ��Ĭ��ֵ��ʼ��ADC_InitStruct����Ա������
* �������: ADC_InitStruct:ָ��ṹ��ADC_InitTypeDef��ָ�룬������ʼ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
  /* Reset ADC init structure parameters values [��λADC��ʼ�ṹ����]*/
  /* Initialize the ADC_Mode member [��ʼ��ADC_Mode��Ա]*/
  ADC_InitStruct->ADC_Mode = ADC_Mode_Independent;

  /* initialize the ADC_ScanConvMode member [��ʼ��ADC_ScanConvMode��Ա]*/
  ADC_InitStruct->ADC_ScanConvMode = DISABLE;

  /* Initialize the ADC_ContinuousConvMode member [��ʼ��ADC_ContinuousConvMode��Ա]*/
  ADC_InitStruct->ADC_ContinuousConvMode = DISABLE;

  /* Initialize the ADC_ExternalTrigConv member [��ʼ��ADC_ExternalTrigConve��Ա]*/
  ADC_InitStruct->ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;

  /* Initialize the ADC_DataAlign member [��ʼ��ADC_DataAlign��Ա]*/
  ADC_InitStruct->ADC_DataAlign = ADC_DataAlign_Right;

  /* Initialize the ADC_NbrOfChannel member [��ʼ��ADC_NbrOfChannel��Ա]*/
  ADC_InitStruct->ADC_NbrOfChannel = 1;
}

/*******************************************************************************
* ��������: ADC_Cmd
* ��������: ʹ��/��ָֹ����ADC��Χģ��.
* �������: (1) ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)ADCx��Χģ�����״̬������������ǣ�ENABLE��DISABLE��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters ������ ������*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the ADON bit to wake up the ADC from power down mode [�ӵ���ģʽ��λADON������]*/
    ADCx->CR2 |= CR2_ADON_Set;
  }
  else
  {
    /* Disable the selected ADC peripheral [����ѡ���ADC�豸]*/
    ADCx->CR2 &= CR2_ADON_Reset;
  }
}

/*******************************************************************************
* ��������: ADC_DMACmd
* ��������: ʹ��/��ָֹ����ADC DMA����.
* �������: (1)ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
*                    ע��: ADC2 û�� DMA ����.
*           (2)ADC DMAת��������״̬������������ǣ�ENABLE��DISABLE��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters ������ ������*/
  assert_param(IS_ADC_DMA_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC DMA request [����ѡ���ADC DMA����]*/
    ADCx->CR2 |= CR2_DMA_Set;
  }
  else
  {
    /* Disable the selected ADC DMA request [����ѡ���ADC DMA����]*/
    ADCx->CR2 &= CR2_DMA_Reset;
  }
}

/*******************************************************************************
* ��������: ADC_ITConfig
* ��������: ʹ��/��ָֹ����ADC�ж�.
* �������: (1)ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)ADC_IT: ָ��ADC�ж�Դ��ʹ�ܵĻ��ֹ��.
*                    ADC_IT ����ʹ�ܻ��߽�ֹADC �ж�.����ʹ������ֵ��һ�����߼���ֵ�����:
*                       - ADC_IT_EOC: EOC�ж�����
*                       - ADC_IT_AWD: AWDOG�ж�����
*                       - ADC_IT_JEOC: JEOC�ж�����
*           (3)NewState: ָ����ADC�жϵ���״̬������������ǣ�ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_ITConfig(ADC_TypeDef* ADCx, u16 ADC_IT, FunctionalState NewState)
{
  u8 itmask = 0;

  /* Check the parameters [������]*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_ADC_IT(ADC_IT));

  /* Get the ADC IT index [ȡ��ADC IT�жϺ�]*/
  itmask = (u8)ADC_IT;

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC interrupts [����ѡ���ADC �ж�]*/
    ADCx->CR1 |= itmask;
  }
  else
  {
    /* Disable the selected ADC interrupts [����ѡ���ADC �ж�]*/
    ADCx->CR1 &= (~(u32)itmask);
  }
}

/*******************************************************************************
* ��������: ADC_ResetCalibration
* ��������: ����ѡ�е�ADCУ׼�Ĵ���
* �������: ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_ResetCalibration(ADC_TypeDef* ADCx)
{
  /* Check the parameters [������]*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Resets the selected ADC calibration registers [��λѡ���ADCУ׼�Ĵ���]*/
  ADCx->CR2 |= CR2_RSTCAL_Set;
}

/*******************************************************************************
* ��������: ADC_GetResetCalibrationStatus
* ��������: �õ�ѡ�е�ADC����У׼�Ĵ�����״̬.
* �������: ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
* �������: ��
* ���ز���: ADC����У׼�Ĵ�������״̬��SET��RESET��.
*******************************************************************************/
FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Check the status of RSTCAL bit [���״̬λRSTCAL]*/
  if ((ADCx->CR2 & CR2_RSTCAL_Set) != (u32)RESET)
  {
    /* RSTCAL bit is set [��λRSTCAL]*/
    bitstatus = SET;
  }
  else
  {
    /* RSTCAL bit is reset [��λRSTCAL]*/
    bitstatus = RESET;
  }

  /* Return the RSTCAL bit status [����״̬λRSTCAL]*/
  return  bitstatus;
}

/*******************************************************************************
* ��������: ADC_StartCalibration
* ��������: ��ʼѡ�е�ADCУ׼����.
* �������: ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_StartCalibration(ADC_TypeDef* ADCx)
{
  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Enable the selected ADC calibration process [����ѡ���ADCУ׼����]*/
  ADCx->CR2 |= CR2_CAL_Set;
}

/*******************************************************************************
* ��������: ADC_GetCalibrationStatus
* ��������: �õ�ѡ�е�ADC У׼��״̬
* �������: ADCx:����x������1��2��3������ѡ��ADC��Χģ��..
* �������: ��
* ���ز���: ADC У׼����״̬��SET ��RESET��.
*******************************************************************************/
FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Check the status of CAL bit [���״̬λCAL]*/
  if ((ADCx->CR2 & CR2_CAL_Set) != (u32)RESET)
  {
    /* CAL bit is set: calibration on going [CAL����:У׼��ʼ]*/
    bitstatus = SET;
  }
  else
  {
    /* CAL bit is reset: end of calibration [CAL��λ:����У׼]*/
    bitstatus = RESET;
  }

  /* Return the CAL bit status [����״̬λCAL]*/
  return  bitstatus;
}

/*******************************************************************************
* ��������: ADC_SoftwareStartConvCmd
* ��������: ʹ��/��ֹѡ�е�ADC��������ƿ�ʼת�� .
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)NewState:ѡ�е������������ʼ�źŵ�ADC����״̬�������������:ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC conversion on external event and start the selected
       ADC conversion [����ѡ���ADCת���ⲿ�¼�������ѡ���ADCת��]*/
    ADCx->CR2 |= CR2_EXTTRIG_SWSTART_Set;
  }
  else
  {
    /* Disable the selected ADC conversion on external event and stop the selected
       ADC conversion [��ֹѡ���ADCת���ⲿ�¼���ֹͣѡ���ADCת��]*/
    ADCx->CR2 &= CR2_EXTTRIG_SWSTART_Reset;
  }
}

/*******************************************************************************
* ��������: ADC_GetSoftwareStartConvStatus
* ��������: ���ѡ��ADC�����ʼת��״̬��
* �������: ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
* �������: ��
* ���ز���: ADC�����ʼת������״̬��SET��RESET��.
*******************************************************************************/
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Check the status of SWSTART bit [���״̬λSWSTART]*/
  if ((ADCx->CR2 & CR2_SWSTART_Set) != (u32)RESET)
  {
    /* SWSTART bit is set [��λSWSTART]*/
    bitstatus = SET;
  }
  else
  {
    /* SWSTART bit is reset [��λSWSTART]*/
    bitstatus = RESET;
  }

  /* Return the SWSTART bit status [����״̬λSWSTART]*/
  return  bitstatus;
}

/*******************************************************************************
* ��������: ADC_DiscModeChannelCountConfig
* ��������: ����ѡ�е�ADC������Ϊ������ģʽ.
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)Number:������ģʽ�³����ŵ�����ֵ����ֵ��ΧΪ1��8��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, u8 Number)
{
  u32 tmpreg1 = 0;
  u32 tmpreg2 = 0;

  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_REGULAR_DISC_NUMBER(Number));

  /* Get the old register value [ȡ�ù�ȥ�ļĴ���ֵ]*/
  tmpreg1 = ADCx->CR1;
  /* Clear the old discontinuous mode channel count [�����ȥ�Ĳ����ģʽͨ��������]*/
  tmpreg1 &= CR1_DISCNUM_Reset;
  /* Set the discontinuous mode channel count [���ò����ģʽͨ��������]*/
  tmpreg2 = Number - 1;
  tmpreg1 |= tmpreg2 << 13;
  /* Store the new register value [�洢�µļĴ���ֵ]*/
  ADCx->CR1 = tmpreg1;
}

/*******************************************************************************
* ��������: ADC_DiscModeCmd
* ��������: ʹ��/��ָֹ����ADC�������ŵ��ķ�����ģʽ
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)NewState: ADC�������ŵ��·�����ģʽ����״̬������������ǣ�ENABLE��DISABLE��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC regular discontinuous mode [����ѡ���ADC������ģʽ]*/
    ADCx->CR1 |= CR1_DISCEN_Set;
  }
  else
  {
    /* Disable the selected ADC regular discontinuous mode [��ֹѡ���ADC������ģʽ]*/
    ADCx->CR1 &= CR1_DISCEN_Reset;
  }
}

/*******************************************************************************
* ��������: ADC_RegularChannelConfig
* ��������: Ϊѡ�е�ADC�������ŵ�������ص���������sequencer���ȼ��Ͳ���ʱ�䡣
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)ADC_Channel:��Ҫ���õ�ADC�ŵ�
*                    ADC_Channel ���ܵ�ȡֵ��
*                       - ADC_Channel_0: ADC�ŵ�0��ѡ��
*                       - ADC_Channel_1: ADC�ŵ�1��ѡ��
*                       - ADC_Channel_2: ADC�ŵ�2��ѡ��
*                       - ADC_Channel_3: ADC�ŵ�3��ѡ��
*                       - ADC_Channel_4: ADC�ŵ�4��ѡ��
*                       - ADC_Channel_5: ADC�ŵ�5��ѡ��
*                       - ADC_Channel_6: ADC�ŵ�6��ѡ��
*                       - ADC_Channel_7: ADC�ŵ�7��ѡ��
*                       - ADC_Channel_8: ADC�ŵ�8��ѡ��
*                       - ADC_Channel_9: ADC�ŵ�9��ѡ��
*                       - ADC_Channel_10: ADC�ŵ�10��ѡ��
*                       - ADC_Channel_11: ADC�ŵ�11��ѡ��
*                       - ADC_Channel_12: ADC�ŵ�12��ѡ��
*                       - ADC_Channel_13: ADC�ŵ�13��ѡ��
*                       - ADC_Channel_14: ADC�ŵ�14��ѡ��
*                       - ADC_Channel_15: ADC�ŵ�15��ѡ��
*                       - ADC_Channel_16: ADC�ŵ�16��ѡ��
*                       - ADC_Channel_17: ADC�ŵ�17��ѡ��
*           (3)Rank:��������������sequencer���ĵȼ���������Χ��1��16��
*           (4)ADC_SampleTime:��ҪΪ��ѡ���ŵ����õĲ���ʱ��
*                    ADC_SampleTime.ȡֵ:
*                       - ADC_SampleTime_1Cycles5: ����ʱ�����1.5������
*                       - ADC_SampleTime_7Cycles5: ����ʱ�����7.5������
*                       - ADC_SampleTime_13Cycles5: ����ʱ�����13.5������
*                       - ADC_SampleTime_28Cycles5: ����ʱ�����28.5������
*                       - ADC_SampleTime_41Cycles5: ����ʱ�����41.5������
*                       - ADC_SampleTime_55Cycles5: ����ʱ�����55.5������
*                       - ADC_SampleTime_71Cycles5: ����ʱ�����71.5������
*                       - ADC_SampleTime_239Cycles5: ����ʱ�����239.5������
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, u8 ADC_Channel, u8 Rank, u8 ADC_SampleTime)
{
  u32 tmpreg1 = 0, tmpreg2 = 0;

  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  assert_param(IS_ADC_REGULAR_RANK(Rank));
  assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));

  /* if ADC_Channel_10 ... ADC_Channel_17 is selected [���ADC_Channel_10 ... ADC_Channel_17��ѡ��]*/
  if (ADC_Channel > ADC_Channel_9)
  {
    /* Get the old register value [ȡ�þɵļĴ���ֵ]*/
    tmpreg1 = ADCx->SMPR1;
    /* Calculate the mask to clear [������Ҫ����ı�־]*/
    tmpreg2 = SMPR1_SMP_Set << (3 * (ADC_Channel - 10));
    /* Clear the old discontinuous mode channel count [�����ȥ�Ĳ����ģʽͨ��������]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [������Ҫ��λ�ı�־]*/
    tmpreg2 = (u32)ADC_SampleTime << (3 * (ADC_Channel - 10));
    /* Set the discontinuous mode channel count [���ò����ģʽͨ��������]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [�洢�µļĴ���ֵ]*/
    ADCx->SMPR1 = tmpreg1;
  }
  else /* ADC_Channel include in ADC_Channel_[0..9] [ADC_Channel �� ADC_Channel_[0..9]֮��]*/
  {
    /* Get the old register value [ȡ�þɵļĴ���ֵ]*/
    tmpreg1 = ADCx->SMPR2;
    /* Calculate the mask to clear [������Ҫ����ı�־]*/
    tmpreg2 = SMPR2_SMP_Set << (3 * ADC_Channel);
    /* Clear the old discontinuous mode channel count [�����ȥ�Ĳ����ģʽͨ��������]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [������Ҫ��λ�ı�־]*/
    tmpreg2 = (u32)ADC_SampleTime << (3 * ADC_Channel);
    /* Set the discontinuous mode channel count [���ò����ģʽͨ��������]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [�洢�µļĴ���ֵ]*/
    ADCx->SMPR2 = tmpreg1;
  }
  /* For Rank 1 to 6 [������1��6֮��]*/
  if (Rank < 7)
  {
    /* Get the old register value [ȡ�þɵļĴ���ֵ]*/
    tmpreg1 = ADCx->SQR3;
    /* Calculate the mask to clear [������Ҫ����ı�־]*/
    tmpreg2 = SQR3_SQ_Set << (5 * (Rank - 1));
    /* Clear the old SQx bits for the selected rank [����ѡ�����������ɵ�SQxλ]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [������Ҫ��λ�ı�־]*/
    tmpreg2 = (u32)ADC_Channel << (5 * (Rank - 1));
    /* Set the SQx bits for the selected rank [����ѡ���������λ�µ�SQxλ]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [�洢�µļĴ���ֵ]*/
    ADCx->SQR3 = tmpreg1;
  }
  /* For Rank 7 to 12 [������7��12֮��]*/
  else if (Rank < 13)
  {
    /* Get the old register value [ȡ�þɵļĴ���ֵ]*/
    tmpreg1 = ADCx->SQR2;
    /* Calculate the mask to clear [������Ҫ����ı�־]*/
    tmpreg2 = SQR2_SQ_Set << (5 * (Rank - 7));
    /* Clear the old SQx bits for the selected rank [����ѡ�����������ɵ�SQxλ]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [������Ҫ��λ�ı�־]*/
    tmpreg2 = (u32)ADC_Channel << (5 * (Rank - 7));
    /* Set the SQx bits for the selected rank [����ѡ���������λ�µ�SQxλ]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [�洢�µļĴ���ֵ]*/
    ADCx->SQR2 = tmpreg1;
  }
  /* For Rank 13 to 16 [������13��16֮��]*/
  else
  {
    /* Get the old register value [ȡ�þɵļĴ���ֵ]*/
    tmpreg1 = ADCx->SQR1;
    /* Calculate the mask to clear [������Ҫ����ı�־]*/
    tmpreg2 = SQR1_SQ_Set << (5 * (Rank - 13));
    /* Clear the old SQx bits for the selected rank [����ѡ�����������ɵ�SQxλ]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [������Ҫ��λ�ı�־]*/
    tmpreg2 = (u32)ADC_Channel << (5 * (Rank - 13));
    /* Set the SQx bits for the selected rank [����ѡ���������λ�µ�SQxλ]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [�洢�µļĴ���ֵ]*/
    ADCx->SQR1 = tmpreg1;
  }
}

/*******************************************************************************
* ��������: ADC_ExternalTrigConvCmd
* ��������: ʹ�ܻ��ֹ�ⲿ����ADCxת����
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)NewState:ѡ�е����ⲿ����ת����ADC����״̬������������ǣ�ENABLE��DISABLE��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_ExternalTrigConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC conversion on external event [�����ⲿ�¼�����ѡ���ADCת��]*/
    ADCx->CR2 |= CR2_EXTTRIG_Set;
  }
  else
  {
    /* Disable the selected ADC conversion on external event [��ֹ�ⲿ�¼�����ѡ���ADCת��]*/
    ADCx->CR2 &= CR2_EXTTRIG_Reset;
  }
}

/*******************************************************************************
* ��������: ADC_GetConversionValue
* ��������: ���س����ŵ�����ADCxת�������
* �������: ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
* �������: ��
* ���ز���: ����ת�����.
*******************************************************************************/
u16 ADC_GetConversionValue(ADC_TypeDef* ADCx)
{
  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Return the selected ADC conversion value [����ѡ���ADCת��ֵ]*/
  return (u16) ADCx->DR;
}

/*******************************************************************************
* ��������: ADC_GetDualModeConversionValue
* ��������: ����˫��ģʽ�µ�ADC1��ADC2�����ת�����
* �������: ��
* ���ز���: ����ת�����.
*******************************************************************************/
u32 ADC_GetDualModeConversionValue(void)
{
  /* Return the dual mode conversion value [����˫ģʽת��ֵ]*/
  return (*(vu32 *) DR_ADDRESS);
}

/*******************************************************************************
* ��������: ADC_AutoInjectedConvCmd
* ��������: ʹ��/��ֹ�ڳ�����ת���󣬱�ѡ��ADC�����Զ�ע����ת����
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)NewState:ѡ�еĽ����Զ�ע��ת����ADC��״̬������������ǣ�ENABLE��DISABLE��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_AutoInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC automatic injected group conversion [����ѡ���ADC�Զ�ע��ͨ����ת��]*/
    ADCx->CR1 |= CR1_JAUTO_Set;
  }
  else
  {
    /* Disable the selected ADC automatic injected group conversion [��ֹѡ���ADC�Զ�ע��ͨ����ת��]*/
    ADCx->CR1 &= CR1_JAUTO_Reset;
  }
}

/*******************************************************************************
* ��������: ADC_InjectedDiscModeCmd
* ��������: ʹ��/��ָֹ��ADCע�����ŵ��ķ�����ģʽ��
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)NewState:��ѡ�еģ�ע�����ŵ���Ϊ������ģʽ��ADC����״̬������������ǣ�ENABLE��DISABLE��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_InjectedDiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC injected discontinuous mode [����ѡ��ADCͨ��ע������ת��ģʽ]*/
    ADCx->CR1 |= CR1_JDISCEN_Set;
  }
  else
  {
    /* Disable the selected ADC injected discontinuous mode [��ֹѡ��ADCͨ��ע������ת��ģʽ]*/
    ADCx->CR1 &= CR1_JDISCEN_Reset;
  }
}

/*******************************************************************************
* ��������: ADC_ExternalTrigInjectedConvConfig
* ��������: Ϊע���ŵ�ת������ADCx�ⲿ����
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)ADC_ExternalTrigInjecConv:��ʼע��ת����ADC������
*                    ADC_ExternalTrigInjecConv ȡֵ:
*                       - ADC_ExternalTrigInjecConv_T1_TRGO: ��ʱ��1TRGO�¼���Ϊע��ת��(ADC1,ADC2,ADC3)���ⲿ������
*                       - ADC_ExternalTrigInjecConv_T1_CC4: ��ʱ��1����Ƚ�4��Ϊע��ת��(ADC1,ADC2,ADC3)���ⲿ������
*                       - ADC_ExternalTrigInjecConv_T2_TRGO: ��ʱ��2TRGO�¼���Ϊע��ת��(ADC1,ADC2,ADC3)���ⲿ������
*                       - ADC_External TrigInjecConv_T2_CC1: ��ʱ��2����Ƚ�1��Ϊע��ת��(ADC1,ADC2,ADC3)���ⲿ������
*                       - ADC_ExternalTrigInjecConv_T3_CC4: ��ʱ��3����Ƚ�4��Ϊע��ת��(ADC1,ADC2,ADC3)���ⲿ������
*                       - ADC_ExternalTrigInjecConv_T4_TRGO: ��ʱ��4TRGO�¼���Ϊע��ת��(ADC1,ADC2,ADC3)���ⲿ������
*                       - ADC_ExternalTrigInjecConv_Ext_IT15_TIM8_CC4: �ⲿ�ж�15����timer8�Ƚ��¼���Ϊע��ת��(ADC1,ADC2,ADC3)���ⲿ������
*                       - ADC_External TrigInjecConv_T4_CC3: timer4�Ƚ�3�¼���Ϊע��ת��(ֻADC3)���ⲿ������
*                       - ADC_External TrigInjecConv_T8_CC2: timer8�Ƚ�2�¼���Ϊע��ת��(ֻADC3)���ⲿ������
*                       - ADC_External TrigInjecConv_T8_CC4: timer8�Ƚ�4�¼���Ϊע��ת��(ֻADC3)���ⲿ������
*                       - ADC_ExternalTrigInjecConv_T5_TRGO: ��ʱ��5 TRGO �¼���Ϊע��ת��(ֻADC3)���ⲿ������
*                       - ADC_External TrigInjecConv_T5_CC4: timer5�Ƚ�4�¼���Ϊע��ת��(ֻADC3)���ⲿ������
*                       - ADC_ExternalTrigInjecConv_��: ע��ת����������������������ⲿ����������
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, u32 ADC_ExternalTrigInjecConv)
{
  u32 tmpreg = 0;

  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_EXT_INJEC_TRIG(ADC_ExternalTrigInjecConv));

  /* Get the old register value [ȡ�þɵļĴ���ֵ]*/
  tmpreg = ADCx->CR2;
  /* Clear the old external event selection for injected group [���ѡ���ע��ͨ��Ⱥ���ⲿ�����¼�]*/
  tmpreg &= CR2_JEXTSEL_Reset;
  /* Set the external event selection for injected group [��λѡ���ע��ͨ��Ⱥ���ⲿ�����¼�]*/
  tmpreg |= ADC_ExternalTrigInjecConv;
  /* Store the new register value [�洢�µļĴ���ֵ]*/
  ADCx->CR2 = tmpreg;
}

/*******************************************************************************
* ��������: ADC_ExternalTrigInjectedConvCmd
* ��������: ͨ���ⲿ������ʹ��/��ֹADCxע���ŵ�ת��
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)NewState:ѡ�е���������ע��ת����ADC�ⲿ���������������������:ENABLE��DISABLE�
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC external event selection for injected group [ʹ��ѡ���ADCxע���ŵ��ⲿ�¼�����]*/
    ADCx->CR2 |= CR2_JEXTTRIG_Set;
  }
  else
  {
    /* Disable the selected ADC external event selection for injected group [��ֹѡ���ADCxע���ŵ��ⲿ�¼�����]*/
    ADCx->CR2 &= CR2_JEXTTRIG_Reset;
  }
}

/*******************************************************************************
* ��������: ADC_SoftwareStartInjectedConvCmd
* ��������: ʹ��/��ֹѡ�е�ADC��ʼע���ŵ�ת����
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)NewState:��ѡ�е���������ע��ת����ADC�������״̬���������������:ENABLE��DISABLE��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC conversion for injected group on external event and start the selected
       ADC injected conversion [�����ⲿ�¼�����ѡ���ADCע��ͨ��Ⱥģʽת����������ע��ѡ��]*/
    ADCx->CR2 |= CR2_JEXTTRIG_JSWSTART_Set;
  }
  else
  {
    /* Disable the selected ADC conversion on external event for injected group and stop the selected
       ADC injected conversion [��ֹ�ⲿ�¼�����ѡ���ADCע��ͨ��Ⱥģʽת����������ע��ѡ��]*/
    ADCx->CR2 &= CR2_JEXTTRIG_JSWSTART_Reset;
  }
}

/*******************************************************************************
* ��������: ADC_GetSoftwareStartInjectedConvCmdStatus
* ��������: ���ѡ�е����������ע��ת����ADC��״̬��
* �������: ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
* �������: ��
* ���ز���: ���������ע��ת����ADC��״̬��SET����RESET��
*******************************************************************************/
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Check the status of JSWSTART bit [���״̬λJSWSTART]*/
  if ((ADCx->CR2 & CR2_JSWSTART_Set) != (u32)RESET)
  {
    /* JSWSTART bit is set [��λJSWSTART]*/
    bitstatus = SET;
  }
  else
  {
    /* JSWSTART bit is reset [��λJSWSTART]*/
    bitstatus = RESET;
  }

  /* Return the JSWSTART bit status [����״̬λJSWSTART]*/
  return  bitstatus;
}

/*******************************************************************************
* ��������: ADC_InjectedChannelConfig
* ��������: ����ѡ�е�ADCע���ŵ���Ӧ����������sequencer���ȼ��Ͳ���ʱ�䡣
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)ADC_Channel: ��Ҫ���õ�ADC�ŵ�.
*                    ADC_Channel ���ܵ�ȡֵ��
*                       - ADC_Channel_0: ADC�ŵ�0��ѡ��
*                       - ADC_Channel_1: ADC�ŵ�1��ѡ��
*                       - ADC_Channel_2: ADC�ŵ�2��ѡ��
*                       - ADC_Channel_3: ADC�ŵ�3��ѡ��
*                       - ADC_Channel_4: ADC�ŵ�4��ѡ��
*                       - ADC_Channel_5: ADC�ŵ�5��ѡ��
*                       - ADC_Channel_6: ADC�ŵ�6��ѡ��
*                       - ADC_Channel_7: ADC�ŵ�7��ѡ��
*                       - ADC_Channel_8: ADC�ŵ�8��ѡ��
*                       - ADC_Channel_9: ADC�ŵ�9��ѡ��
*                       - ADC_Channel_10: ADC�ŵ�10��ѡ��
*                       - ADC_Channel_11: ADC�ŵ�11��ѡ��
*                       - ADC_Channel_12: ADC�ŵ�12��ѡ��
*                       - ADC_Channel_13: ADC�ŵ�13��ѡ��
*                       - ADC_Channel_14: ADC�ŵ�14��ѡ��
*                       - ADC_Channel_15: ADC�ŵ�15��ѡ��
*                       - ADC_Channel_16: ADC�ŵ�16��ѡ��
*                       - ADC_Channel_17: ADC�ŵ�17��ѡ��
*           (3)Rank:ע������������sequencer���ĵȼ�,ѡ��Χ������1-4֮��
*           (4)ADC_SampleTime: ADC_SampleTime:��ҪΪѡ���ŵ����õĲ���ʱ��ֵ
*                    ADC_SampleTime.ȡֵ:
*                       - ADC_SampleTime_1Cycles5: ����ʱ�����1.5������
*                       - ADC_SampleTime_7Cycles5: ����ʱ�����7.5������
*                       - ADC_SampleTime_13Cycles5: ����ʱ�����13.5������
*                       - ADC_SampleTime_28Cycles5: ����ʱ�����28.5������
*                       - ADC_SampleTime_41Cycles5: ����ʱ�����41.5������
*                       - ADC_SampleTime_55Cycles5: ����ʱ�����55.5������
*                       - ADC_SampleTime_71Cycles5: ����ʱ�����71.5������
*                       - ADC_SampleTime_239Cycles5: ����ʱ�����239.5������
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, u8 ADC_Channel, u8 Rank, u8 ADC_SampleTime)
{
  u32 tmpreg1 = 0, tmpreg2 = 0, tmpreg3 = 0;

  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  assert_param(IS_ADC_INJECTED_RANK(Rank));
  assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));

  /* if ADC_Channel_10 ... ADC_Channel_17 is selected [���ADC_Channel_10 ... ADC_Channel_17��ѡ��]*/
  if (ADC_Channel > ADC_Channel_9)
  {
    /* Get the old register value [ȡ�þɵļĴ���ֵ]*/
    tmpreg1 = ADCx->SMPR1;
    /* Calculate the mask to clear [������Ҫ����ı�־]*/
    tmpreg2 = SMPR1_SMP_Set << (3*(ADC_Channel - 10));
    /* Clear the old discontinuous mode channel count [�����ȥ�Ĳ����ģʽͨ��������]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [������Ҫ��λ�ı�־]*/
    tmpreg2 = (u32)ADC_SampleTime << (3*(ADC_Channel - 10));
    /* Set the discontinuous mode channel count [���ò����ģʽͨ��������]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [�洢�µļĴ���ֵ]*/
    ADCx->SMPR1 = tmpreg1;
  }
  else /* ADC_Channel include in ADC_Channel_[0..9] [ADC_Channel �� ADC_Channel_[0..9]֮��]*/
  {
    /* Get the old register value [ȡ�þɵļĴ���ֵ]*/
    tmpreg1 = ADCx->SMPR2;
    /* Calculate the mask to clear [������Ҫ����ı�־]*/
    tmpreg2 = SMPR2_SMP_Set << (3 * ADC_Channel);
    /* Clear the old discontinuous mode channel count [�����ȥ�Ĳ����ģʽͨ��������]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [������Ҫ��λ�ı�־]*/
    tmpreg2 = (u32)ADC_SampleTime << (3 * ADC_Channel);
    /* Set the discontinuous mode channel count [���ò����ģʽͨ��������]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [�洢�µļĴ���ֵ]*/
    ADCx->SMPR2 = tmpreg1;
  }

  /* Rank configuration [��������]*/
  /* Get the old register value [ȡ�þɵļĴ���ֵ]*/
  tmpreg1 = ADCx->JSQR;
  /* Get JL value[ȡ��JLֵ]: Number = JL+1 */
  tmpreg3 =  (tmpreg1 & JSQR_JL_Set)>> 20;
  /* Calculate the mask to clear[������Ҫ����ı�־]: ((Rank-1)+(4-JL-1)) */
  tmpreg2 = JSQR_JSQ_Set << (5 * (u8)((Rank + 3) - (tmpreg3 + 1)));
  /* Clear the old JSQx bits for the selected rank [���ѡ�����оɵ�JSQxλ]*/
  tmpreg1 &= ~tmpreg2;
  /* Calculate the mask to set[������Ҫ��λ�ı�־]: ((Rank-1)+(4-JL-1)) */
  tmpreg2 = (u32)ADC_Channel << (5 * (u8)((Rank + 3) - (tmpreg3 + 1)));
  /* Set the JSQx bits for the selected rank [��λѡ�����оɵ�JSQxλ]*/
  tmpreg1 |= tmpreg2;
  /* Store the new register value [�洢�µļĴ���ֵ]*/
  ADCx->JSQR = tmpreg1;
}

/*******************************************************************************
* ��������: ADC_InjectedSequencerLengthConfig
* ��������: ����ע���ŵ���������sequencer���ĳ���
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)Length:��������sequencer���ĳ��ȸò����ķ�Χ��1��4.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef* ADCx, u8 Length)
{
  u32 tmpreg1 = 0;
  u32 tmpreg2 = 0;

  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_LENGTH(Length));
  
  /* Get the old register value [ȡ�þɵļĴ���ֵ]*/
  tmpreg1 = ADCx->JSQR;
  /* Clear the old injected sequnence lenght JL bits [���ע���ŵ��������ĳ���JLλ]*/
  tmpreg1 &= JSQR_JL_Reset;
  /* Set the injected sequnence lenght JL bits [��λע���ŵ��������ĳ���JLλ]*/
  tmpreg2 = Length - 1; 
  tmpreg1 |= tmpreg2 << 20;
  /* Store the new register value [�洢�µļĴ���ֵ]*/
  ADCx->JSQR = tmpreg1;
}

/*******************************************************************************
* ��������: ADC_SetInjectedOffset
* ��������: ����ע���ŵ�ת����ƫ��ֵ��
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)ADC_InjectedChannel:��Ҫ����ƫ��ֵ��ADCע���ŵ���
*                    ADC_InjectedChannel ָ������Ҫ����ƫ��ֵ��ADC ע���ŵ�:
*                       - ADC_InjectedChannel_1: ѡ��ע���ŵ�1
*                       - ADC_InjectedChannel_2: ѡ��ע���ŵ�2
*                       - ADC_InjectedChannel_3: ѡ��ע���ŵ�3
*                       - ADC_InjectedChannel_4: ѡ��ע���ŵ�4
*           (3)Offset:ѡ�е�ADCע���ŵ���ƫ��ֵ���ò�����һ��12λֵ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_SetInjectedOffset(ADC_TypeDef* ADCx, u8 ADC_InjectedChannel, u16 Offset)
{
  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_CHANNEL(ADC_InjectedChannel));
  assert_param(IS_ADC_OFFSET(Offset));  

  /* Set the selected injected channel data offset [����ѡ���ע��ͨ������ƫ��]*/
  *((vu32 *)((*(u32*)&ADCx) + ADC_InjectedChannel)) = (u32)Offset;
}

/*******************************************************************************
* ��������: ADC_GetInjectedConversionValue
* ��������: ����ѡ�е�ע���ŵ���ADCת�������
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)ADC_InjectedChannel:��Ҫ����ƫ��ֵ��ADCע���ŵ�
*              ADC_InjectedChannel ָ������Ҫ����ƫ��ֵ��ADC ע���ŵ�:
*                       - ADC_InjectedChannel_1: ѡ��ע���ŵ�1
*                       - ADC_InjectedChannel_2: ѡ��ע���ŵ�2
*                       - ADC_InjectedChannel_3: ѡ��ע���ŵ�3
*                       - ADC_InjectedChannel_4: ѡ��ע���ŵ�4
* �������: ��
* ���ز���: ����ת�����.
*******************************************************************************/
u16 ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx, u8 ADC_InjectedChannel)
{
  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_CHANNEL(ADC_InjectedChannel));

  /* Returns the selected injected channel conversion data value [����ѡ���ע��ͨ��ת������ֵ]*/
  return (u16) (*(vu32*) (((*(u32*)&ADCx) + ADC_InjectedChannel + JDR_Offset)));
}

/*******************************************************************************
* ��������: ADC_AnalogWatchdogCmd
* ��������: ʹ��/��ֹһ��/���г����ŵ�����ע���ŵ���ģ�⿴�Ź���
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)ADC_AnalogWatchdog: ADCģ�⿴�Ź�����
*                    �ò������ܵ�ȡֵ:
*                       - ADC_AnalogWatchdog_SingleRegEnable: �������ŵ��ϵ�ģ�⿴�Ź�
*                       - ADC_AnalogWatchdog_SingleInjecEnable: ��ע���ŵ��ϵ�ģ�⿴�Ź�
*                       - ADC_AnalogWatchdog_SingleRegOrInjecEnable: �������ŵ�����ע���ŵ���ģ�⿴�Ź�
*                       - ADC_AnalogWatchdog_AllRegEnable: ���г����ŵ��ϵ�ģ�⿴�Ź�
*                       - ADC_AnalogWatchdog_AllInjecEnable: ����ע���ŵ��ϵ�ģ�⿴�Ź�
*                       - ADC_AnalogWatchdog_AllRegAllInjecEnable: ���г����ŵ���ע���ŵ��ϵ�ģ�⿴�Ź�
*                       - ADC_AnalogWatchdog_��: û���ŵ���Ҫģ�⿴�Ź�����
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, u32 ADC_AnalogWatchdog)
{
  u32 tmpreg = 0;

  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_ANALOG_WATCHDOG(ADC_AnalogWatchdog));

  /* Get the old register value [ȡ�þɵļĴ���ֵ]*/
  tmpreg = ADCx->CR1;
  /* Clear AWDEN, AWDENJ and AWDSGL bits [���AWDEN,AWDENJ��AWDSGLλ]*/
  tmpreg &= CR1_AWDMode_Reset;
  /* Set the analog watchdog enable mode [��λģ�⿴�Ź�����ģʽ]*/
  tmpreg |= ADC_AnalogWatchdog;
  /* Store the new register value [�洢�µļĴ���ֵ]*/
  ADCx->CR1 = tmpreg;
}

/*******************************************************************************
* ��������: ADC_AnalogWatchdogThresholdsConfig
* ��������: ����ģ�⿴�Ź��ĸߡ��ͷ�ֵ.
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)HighThreshold: ADCģ�⿴�Ź���߷�ֵ���ò�����һ��12λ����ֵ
*           (3)LowThreshold: ADCģ�⿴�Ź���ͷ�ֵ���ò�����һ��12λ����ֵ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, u16 HighThreshold,
                                        u16 LowThreshold)
{
  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_THRESHOLD(HighThreshold));
  assert_param(IS_ADC_THRESHOLD(LowThreshold));

  /* Set the ADCx high threshold [����ADCx�ĸ߷�ֵ]*/
  ADCx->HTR = HighThreshold;
  /* Set the ADCx low threshold [����ADCx�ĵͷ�ֵ]*/
  ADCx->LTR = LowThreshold;
}

/*******************************************************************************
* ��������: ADC_AnalogWatchdogSingleChannelConfig
* ��������: ���ü��ӵ��ŵ���ģ�⿴�Ź�
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)ADC_Channel:��ҪΪ������ģ�⿴�Ź����ŵ�.
*                    ADC_Channel ���ܵ�ȡֵ��
*                       - ADC_Channel_0: ADC�ŵ�0��ѡ��
*                       - ADC_Channel_1: ADC�ŵ�1��ѡ��
*                       - ADC_Channel_2: ADC�ŵ�2��ѡ��
*                       - ADC_Channel_3: ADC�ŵ�3��ѡ��
*                       - ADC_Channel_4: ADC�ŵ�4��ѡ��
*                       - ADC_Channel_5: ADC�ŵ�5��ѡ��
*                       - ADC_Channel_6: ADC�ŵ�6��ѡ��
*                       - ADC_Channel_7: ADC�ŵ�7��ѡ��
*                       - ADC_Channel_8: ADC�ŵ�8��ѡ��
*                       - ADC_Channel_9: ADC�ŵ�9��ѡ��
*                       - ADC_Channel_10: ADC�ŵ�10��ѡ��
*                       - ADC_Channel_11: ADC�ŵ�11��ѡ��
*                       - ADC_Channel_12: ADC�ŵ�12��ѡ��
*                       - ADC_Channel_13: ADC�ŵ�13��ѡ��
*                       - ADC_Channel_14: ADC�ŵ�14��ѡ��
*                       - ADC_Channel_15: ADC�ŵ�15��ѡ��
*                       - ADC_Channel_16: ADC�ŵ�16��ѡ��
*                       - ADC_Channel_17: ADC�ŵ�17��ѡ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCx, u8 ADC_Channel)
{
  u32 tmpreg = 0;

  /* Check the parameters ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));

  /* Get the old register value [ȡ�þɵļĴ���ֵ]*/
  tmpreg = ADCx->CR1;
  /* Clear the Analog watchdog channel select bits [���ģ�⿴�Ź�ͨ��ѡ��λ]*/
  tmpreg &= CR1_AWDCH_Reset;
  /* Set the Analog watchdog channel [��λģ�⿴�Ź�ͨ��ѡ��λ]*/
  tmpreg |= ADC_Channel;
  /* Store the new register value [�洢�µļĴ���ֵ]*/
  ADCx->CR1 = tmpreg;
}

/*******************************************************************************
* ��������: ADC_TempSensorVrefintCmd
* ��������: ʹ��/��ֹ�¶ȴ��кͲο���ѹ�ŵ���
* �������: NewState:�¶ȴ��кͲο���ѹ�ŵ�����״̬���ò�������ȡ����ֵ��ENABLE��DISABLE��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_TempSensorVrefintCmd(FunctionalState NewState)
{
  /* Check the parameters ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the temperature sensor and Vrefint channel[ʹ���¶ȴ��кͲο���ѹ�ŵ�]*/
    ADC1->CR2 |= CR2_TSVREFE_Set;
  }
  else
  {
    /* Disable the temperature sensor and Vrefint channel[��ֹ�¶ȴ��кͲο���ѹ�ŵ�]*/
    ADC1->CR2 &= CR2_TSVREFE_Reset;
  }
}

/*******************************************************************************
* ��������: ADC_GetFlagStatus
* ��������: ���ָ����ADC��־�Ƿ���λ.
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)ADC_FLAG:ָ����Ҫ���ı�־��
*                    ������ADC_FLAG ��ȡֵ:
*                       - ADC_FLAG_AWD: ģ�⿴�Ź���־
*                       - ADC_FLAG_EOC: ת��������־
*                       - ADC_FLAG_JEOC: ע����ת��������־
*                       - ADC_FLAG_JSTRT: ע����ת����ʼ��־
*                       - ADC_FLAG_STRT: ������ת����ʼ��־
* �������: ��
* ���ز���: ADC_FLAG����״̬(SET��RESET).
*******************************************************************************/
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, u8 ADC_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_GET_FLAG(ADC_FLAG));

  /* Check the status of the specified ADC flag [���ָ��ADC״̬λ]*/
  if ((ADCx->SR & ADC_FLAG) != (u8)RESET)
  {
    /* ADC_FLAG is set [��λADC_FLAG]*/
    bitstatus = SET;
  }
  else
  {
    /* ADC_FLAG is reset [��λADC_FLAG]*/
    bitstatus = RESET;
  }

  /* Return the ADC_FLAG status [����״̬λADC_FLAG]*/
  return  bitstatus;
}

/*******************************************************************************
* ��������: ADC_ClearFlag
* ��������: ���ADCx�Ĺ����־.
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)ADC_FLAG:��Ҫ����ı�־��ʹ��"|"����ʹ��һ��������־����ͬʱ�������
*                    ������ADC_FLAG ��ȡֵ:
*                       - ADC_FLAG_AWD: ģ�⿴�Ź���־
*                       - ADC_FLAG_EOC: ת��������־
*                       - ADC_FLAG_JEOC: ע����ת��������־
*                       - ADC_FLAG_JSTRT: ע����ת����ʼ��־
*                       - ADC_FLAG_STRT: ������ת����ʼ��־
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_ClearFlag(ADC_TypeDef* ADCx, u8 ADC_FLAG)
{
  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CLEAR_FLAG(ADC_FLAG));

  /* Clear the selected ADC flags [���ѡ���ADC��־]*/
  ADCx->SR = ~(u32)ADC_FLAG;
}

/*******************************************************************************
* ��������: ADC_GetITStatus
* ��������: ���ָ��ADC���ж��Ƿ���.
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)ADC_IT:��Ҫ����ADC�ж�Դ.
*                    ����ʹ������ֵ��һ�����߼���ֵ�����:
*                       - ADC_IT_EOC: EOC�ж�����
*                       - ADC_IT_AWD: AWDOG�ж�����
*                       - ADC_IT_JEOC: JEOC�ж�����
* �������: ��
* ���ز���: ADC_IT����״̬��SET��RESET��.
*******************************************************************************/
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, u16 ADC_IT)
{
  ITStatus bitstatus = RESET;
  u32 itmask = 0, enablestatus = 0;

  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_GET_IT(ADC_IT));

  /* Get the ADC IT index [ȡ��ADC�жϺ�]*/
  itmask = ADC_IT >> 8;

  /* Get the ADC_IT enable bit status [ȡ��ADC�ж������־λ]*/
  enablestatus = (ADCx->CR1 & (u8)ADC_IT) ;

  /* Check the status of the specified ADC interrupt [���ָ��ADC�жϱ�־λ]*/
  if (((ADCx->SR & itmask) != (u32)RESET) && enablestatus)
  {
    /* ADC_IT is set [��λADC_IT]*/
    bitstatus = SET;
  }
  else
  {
    /* ADC_IT is reset [��λADC_IT]*/
    bitstatus = RESET;
  }

  /* Return the ADC_IT status [����ADC_IT״̬]*/
  return  bitstatus;
}

/*******************************************************************************
* ��������: ADC_ClearITPendingBit
* ��������: ���ADCx���жϹ���λ.
* �������: (1)ADCx:����x������1��2��3������ѡ��ADC��Χģ��.
*           (2)ADC_IT:��Ҫ����ADC�ж�Դ.
*                    ����ʹ������ֵ��һ�����߼���ֵ�����:
*                       - ADC_IT_EOC: EOC�ж�����
*                       - ADC_IT_AWD: AWDOG�ж�����
*                       - ADC_IT_JEOC: JEOC�ж�����
* �������: ��
* ���ز���: ��
*******************************************************************************/
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, u16 ADC_IT)
{
  u8 itmask = 0;

  /* Check the parameters [������] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_IT(ADC_IT));

  /* Get the ADC IT index [ȡ��ADC�жϺ�]*/
  itmask = (u8)(ADC_IT >> 8);

  /* Clear the selected ADC interrupt pending bits [���ѡ���ADC�жϹ���λ]*/
  ADCx->SR = ~(u32)itmask;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****�ļ�����****/
