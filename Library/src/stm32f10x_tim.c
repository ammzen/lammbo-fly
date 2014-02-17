/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_tim.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������TIM�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
/* ---------------------- TIM registers bit mask ------------------------ */
#define CR1_CEN_Set                 ((u16)0x0001)
#define CR1_CEN_Reset               ((u16)0x03FE)
#define CR1_UDIS_Set                ((u16)0x0002)
#define CR1_UDIS_Reset              ((u16)0x03FD)
#define CR1_URS_Set                 ((u16)0x0004)
#define CR1_URS_Reset               ((u16)0x03FB)
#define CR1_OPM_Reset               ((u16)0x03F7)
#define CR1_CounterMode_Mask        ((u16)0x038F)
#define CR1_ARPE_Set                ((u16)0x0080)
#define CR1_ARPE_Reset              ((u16)0x037F)
#define CR1_CKD_Mask                ((u16)0x00FF)

#define CR2_CCPC_Set                ((u16)0x0001)
#define CR2_CCPC_Reset              ((u16)0xFFFE)
#define CR2_CCUS_Set                ((u16)0x0004)
#define CR2_CCUS_Reset              ((u16)0xFFFB)
#define CR2_CCDS_Set                ((u16)0x0008)
#define CR2_CCDS_Reset              ((u16)0xFFF7)
#define CR2_MMS_Mask                ((u16)0xFF8F)
#define CR2_TI1S_Set                ((u16)0x0080)
#define CR2_TI1S_Reset              ((u16)0xFF7F)
#define CR2_OIS1_Reset              ((u16)0x7EFF)
#define CR2_OIS1N_Reset             ((u16)0x7DFF)
#define CR2_OIS2_Reset              ((u16)0x7BFF)
#define CR2_OIS2N_Reset             ((u16)0x77FF)
#define CR2_OIS3_Reset              ((u16)0x6FFF)
#define CR2_OIS3N_Reset             ((u16)0x5FFF)
#define CR2_OIS4_Reset              ((u16)0x3FFF)

#define SMCR_SMS_Mask               ((u16)0xFFF8)
#define SMCR_ETR_Mask               ((u16)0x00FF)
#define SMCR_TS_Mask                ((u16)0xFF8F)
#define SMCR_MSM_Reset              ((u16)0xFF7F)
#define SMCR_ECE_Set                ((u16)0x4000)

#define CCMR_CC13S_Mask             ((u16)0xFFFC)
#define CCMR_CC24S_Mask             ((u16)0xFCFF)
#define CCMR_TI13Direct_Set         ((u16)0x0001)
#define CCMR_TI24Direct_Set         ((u16)0x0100)
#define CCMR_OC13FE_Reset           ((u16)0xFFFB)
#define CCMR_OC24FE_Reset           ((u16)0xFBFF)
#define CCMR_OC13PE_Reset           ((u16)0xFFF7)
#define CCMR_OC24PE_Reset           ((u16)0xF7FF)
#define CCMR_OC13M_Mask             ((u16)0xFF8F)
#define CCMR_OC24M_Mask             ((u16)0x8FFF) 

#define CCMR_OC13CE_Reset           ((u16)0xFF7F)
#define CCMR_OC24CE_Reset           ((u16)0x7FFF)

#define CCMR_IC13PSC_Mask           ((u16)0xFFF3)
#define CCMR_IC24PSC_Mask           ((u16)0xF3FF)
#define CCMR_IC13F_Mask             ((u16)0xFF0F)
#define CCMR_IC24F_Mask             ((u16)0x0FFF)

#define CCMR_Offset                 ((u16)0x0018)
#define CCER_CCE_Set                ((u16)0x0001)
#define CCER_CCNE_Set               ((u16)0x0004)

#define CCER_CC1P_Reset             ((u16)0xFFFD)
#define CCER_CC2P_Reset             ((u16)0xFFDF)
#define CCER_CC3P_Reset             ((u16)0xFDFF)
#define CCER_CC4P_Reset             ((u16)0xDFFF)

#define CCER_CC1NP_Reset            ((u16)0xFFF7)
#define CCER_CC2NP_Reset            ((u16)0xFF7F)
#define CCER_CC3NP_Reset            ((u16)0xF7FF)

#define CCER_CC1E_Set               ((u16)0x0001)
#define CCER_CC1E_Reset             ((u16)0xFFFE)

#define CCER_CC1NE_Reset            ((u16)0xFFFB)

#define CCER_CC2E_Set               ((u16)0x0010)
#define CCER_CC2E_Reset             ((u16)0xFFEF)

#define CCER_CC2NE_Reset            ((u16)0xFFBF)

#define CCER_CC3E_Set               ((u16)0x0100)
#define CCER_CC3E_Reset             ((u16)0xFEFF)

#define CCER_CC3NE_Reset            ((u16)0xFBFF)

#define CCER_CC4E_Set               ((u16)0x1000)
#define CCER_CC4E_Reset             ((u16)0xEFFF)

#define BDTR_MOE_Set                ((u16)0x8000)
#define BDTR_MOE_Reset              ((u16)0x7FFF)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
static void TI1_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter);
static void TI2_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter);
static void TI3_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter);
static void TI4_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter);
/* ���ú��� ---------------------------------------------------------*/
/*******************************************************************************
* ��������: TIM_DeInit
* ��������: ����TIMx����Ĵ���Ϊ��Ĭ�ϸ�λֵ.
* �������: TIMx:�˴�x������1��8��ѡ������.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_DeInit(TIM_TypeDef* TIMx)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
 
  switch (*(u32*)&TIMx)
  {
    case TIM1_BASE:
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, DISABLE);  
      break; 
      
    case TIM2_BASE:
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE);
      break;
 
    case TIM3_BASE:
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, ENABLE);
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, DISABLE);
      break;
 
    case TIM4_BASE:
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, ENABLE);
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, DISABLE);
      break;
      
    case TIM5_BASE:
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM5, ENABLE);
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM5, DISABLE);
      break;
      
    case TIM6_BASE:
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM6, ENABLE);
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM6, DISABLE);
      break;
      
    case TIM7_BASE:
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM7, ENABLE);
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM7, DISABLE);
      break;
      
    case TIM8_BASE:
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM8, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM8, DISABLE);  
      break; 
      
    default:
      break;
  }
}

/*******************************************************************************
* ��������: TIM_TimeBaseInit
* ��������: ����TIM_TimeBaseInitStruct�е��ض�������ʼ��ĳ��TIM��ʱ�����Ԫ.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_BaseInitStruct:ָ��һ������TIMʱ���������Ϣ��TIM_BaseInitTypeDef�ṹ��ָ��..
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_COUNTER_MODE(TIM_TimeBaseInitStruct->TIM_CounterMode));
  assert_param(IS_TIM_CKD_DIV(TIM_TimeBaseInitStruct->TIM_ClockDivision));

  /* Select the Counter Mode and set the clock division [ѡ�����ģʽ������ʱ�ӷ�Ƶ]*/
  TIMx->CR1 &= CR1_CKD_Mask & CR1_CounterMode_Mask;
  TIMx->CR1 |= (u32)TIM_TimeBaseInitStruct->TIM_ClockDivision |
                TIM_TimeBaseInitStruct->TIM_CounterMode;
  /* Set the Autoreload value [�����Զ�װ��ֵ]*/
  TIMx->ARR = TIM_TimeBaseInitStruct->TIM_Period ;

  /* Set the Prescaler value [����Ԥ��Ƶ��ֵ]*/
  TIMx->PSC = TIM_TimeBaseInitStruct->TIM_Prescaler;

  /* Generate an update event to reload the Prescaler value immediatly [����һ�������¼���������װ��Ԥ��Ƶ��ֵ]*/
  TIMx->EGR = TIM_PSCReloadMode_Immediate;
    
  if (((*(u32*)&TIMx) == TIM1_BASE) || ((*(u32*)&TIMx) == TIM8_BASE))  
  {
    /* Set the Repetition Counter value [�����ظ�������ֵ]*/
    TIMx->RCR = TIM_TimeBaseInitStruct->TIM_RepetitionCounter;
  }        
}

/*******************************************************************************
* ��������: TIM_OC1Init
* ��������: ����TIM_OCInitStruct�е��ض�������ʼ��TIMxͨ��1.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_OCInitStruct:ָ��һ�������ض�TIMx�����������Ϣ��TIM_OCInitTypeDef�ṹ��ָ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  u16 tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   

  /* Disable the Channel 1: Reset the CC1E Bit [��ֹͨ��1����λCC1Eλ]*/
  TIMx->CCER &= CCER_CC1E_Reset;
  
  /* Get the TIMx CCER register value [ȡ��TIMx ����/�Ƚ�ʹ�ܼĴ����ļĴ���ֵ]*/
  tmpccer = TIMx->CCER;

  /* Get the TIMx CR2 register value [ȡ��TIMx ����2�ļĴ���ֵ]*/
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR1 register value [ȡ��TIMx ����/�Ƚ�ģʽ�Ĵ���1�ļĴ���ֵ]*/
  tmpccmrx = TIMx->CCMR1;
    
  /* Reset the Output Compare Mode Bits [��λ����Ƚ�ģʽλ]*/
  tmpccmrx &= CCMR_OC13M_Mask;
  
  /* Select the Output Compare Mode [ѡ������Ƚ�ģʽλ]*/
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* Reset the Output Polarity level [��λ������Լ���]*/
  tmpccer &= CCER_CC1P_Reset;

  /* Set the Output Compare Polarity [��������Ƚϼ���]*/
  tmpccer |= TIM_OCInitStruct->TIM_OCPolarity;
  
  /* Set the Output State [�������״̬]*/
  tmpccer |= TIM_OCInitStruct->TIM_OutputState;
  
  /* Set the Capture Compare Register value [���ò���ȽϼĴ���ֵ]*/
  TIMx->CCR1 = TIM_OCInitStruct->TIM_Pulse;
  
  if((*(u32*)&TIMx == TIM1_BASE) || (*(u32*)&TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* Reset the Output N Polarity level [��λ���N���Եȼ�]*/
    tmpccer &= CCER_CC1NP_Reset;

    /* Set the Output N Polarity [�������N����]*/
    tmpccer |= TIM_OCInitStruct->TIM_OCNPolarity;

    /* Reset the Output N State [��λ���N״̬]*/
    tmpccer &= CCER_CC1NE_Reset;
    
    /* Set the Output N State [�������N״̬]*/
    tmpccer |= TIM_OCInitStruct->TIM_OutputNState;

    /* Reset the Ouput Compare and Output Compare N IDLE State [��λ����ȽϺ�����Ƚ�N IDLE״̬]*/
    tmpcr2 &= CR2_OIS1_Reset;
    tmpcr2 &= CR2_OIS1N_Reset;

    /* Set the Output Idle state [�������IDLE״̬]*/
    tmpcr2 |= TIM_OCInitStruct->TIM_OCIdleState;

    /* Set the Output N Idle state [�������N IDLE״̬]*/
    tmpcr2 |= TIM_OCInitStruct->TIM_OCNIdleState;
  }
  /* Write to TIMx CR2 [дTIMx���üĴ���2]*/
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR1 [дTIMx����/�Ƚ�ģʽ�Ĵ���1]*/
  TIMx->CCMR1 = tmpccmrx;
  
  /* Write to TIMx CCER [дTIMx����/�Ƚ�ʹ�ܼĴ���]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TIM_OC2Init
* ��������: ����TIM_OCInitStruct�е��ض�������ʼ��TIMxͨ��2��
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_OCInitStruct:ָ��һ�������ض�TIMx�����������Ϣ��TIM_OCInitTypeDef�ṹ��ָ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  u16 tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   

  /* Disable the Channel 2: Reset the CC2E Bit [��ֹͨ��2����λCC2Eλ]*/
  TIMx->CCER &= CCER_CC2E_Reset;
  
  /* Get the TIMx CCER register value [ȡ��TIMx ����/�Ƚ�ʹ�ܼĴ����ļĴ���ֵ]*/  
  tmpccer = TIMx->CCER;

  /* Get the TIMx CR2 register value [ȡ��TIMx ����2�ļĴ���ֵ]*/
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR1 register value [ȡ��TIMx ����/�Ƚ�ģʽ�Ĵ���1�ļĴ���ֵ]*/
  tmpccmrx = TIMx->CCMR1;
    
  /* Reset the Output Compare Mode Bits [��λ����Ƚ�ģʽλ]*/
  tmpccmrx &= CCMR_OC24M_Mask;
  
  /* Select the Output Compare Mode [ѡ������Ƚ�ģʽλ]*/
  tmpccmrx |= (u16)(TIM_OCInitStruct->TIM_OCMode << 8);
  
  /* Reset the Output Polarity level [��λ������Լ���]*/
  tmpccer &= CCER_CC2P_Reset;

  /* Set the Output Compare Polarity [��������Ƚϼ���]*/
  tmpccer |= (u16)(TIM_OCInitStruct->TIM_OCPolarity << 4);
  
  /* Set the Output State [�������״̬]*/
  tmpccer |= (u16)(TIM_OCInitStruct->TIM_OutputState << 4);
  
  /* Set the Capture Compare Register value [���ò���ȽϼĴ���ֵ]*/
  TIMx->CCR2 = TIM_OCInitStruct->TIM_Pulse;
  
  if((*(u32*)&TIMx == TIM1_BASE) || (*(u32*)&TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* Reset the Output N Polarity level [��λ���N���Եȼ�]*/
    tmpccer &= CCER_CC2NP_Reset;

    /* Set the Output N Polarity [�������N����]*/
    tmpccer |= (u16)(TIM_OCInitStruct->TIM_OCNPolarity << 4);

    /* Reset the Output N State [��λ���N״̬]*/
    tmpccer &= CCER_CC2NE_Reset;
    
    /* Set the Output N State [�������N״̬]*/
    tmpccer |= (u16)(TIM_OCInitStruct->TIM_OutputNState << 4);

    /* Reset the Ouput Compare and Output Compare N IDLE State [��λ����ȽϺ�����Ƚ�N IDLE״̬]*/
    tmpcr2 &= CR2_OIS2_Reset;
    tmpcr2 &= CR2_OIS2N_Reset;

    /* Set the Output Idle state [�������IDLE״̬]*/
    tmpcr2 |= (u16)(TIM_OCInitStruct->TIM_OCIdleState << 2);

    /* Set the Output N Idle state [�������N IDLE״̬]*/
    tmpcr2 |= (u16)(TIM_OCInitStruct->TIM_OCNIdleState << 2);
  }

  /* Write to TIMx CR2 [дTIMx���üĴ���2]*/
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR1 [дTIMx����/�Ƚ�ģʽ�Ĵ���1]*/
  TIMx->CCMR1 = tmpccmrx;
  
  /* Write to TIMx CCER [дTIMx����/�Ƚ�ʹ�ܼĴ���]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TIM_OC3Init
* ��������: ����TIM_OCInitStruct�е��ض�������ʼ��TIMxͨ��3��
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_OCInitStruct:ָ��һ�������ض�TIMx�����������Ϣ��TIM_OCInitTypeDef�ṹ��ָ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC3Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  u16 tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   

  /* Disable the Channel 3: Reset the CC3E Bit [��ֹͨ��3����λCC3Eλ]*/
  TIMx->CCER &= CCER_CC3E_Reset;
  
  /* Get the TIMx CCER register value [ȡ��TIMx ����/�Ƚ�ʹ�ܼĴ����ļĴ���ֵ]*/
  tmpccer = TIMx->CCER;

  /* Get the TIMx CR2 register value [ȡ��TIMx ����2�ļĴ���ֵ]*/
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR2 register value [ȡ��TIMx ����/�Ƚ�ģʽ�Ĵ���2�ļĴ���ֵ]*/
  tmpccmrx = TIMx->CCMR2;
    
  /* Reset the Output Compare Mode Bits [��λ����Ƚ�ģʽλ]*/
  tmpccmrx &= CCMR_OC13M_Mask;
  
  /* Select the Output Compare Mode [ѡ������Ƚ�ģʽλ]*/
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* Reset the Output Polarity level [��λ������Լ���]*/
  tmpccer &= CCER_CC3P_Reset;

  /* Set the Output Compare Polarity [��������Ƚϼ���]*/
  tmpccer |= (u16)(TIM_OCInitStruct->TIM_OCPolarity << 8);
  
  /* Set the Output State [�������״̬]*/
  tmpccer |= (u16)(TIM_OCInitStruct->TIM_OutputState << 8);
  
  /* Set the Capture Compare Register value [���ò���ȽϼĴ���ֵ]*/
  TIMx->CCR3 = TIM_OCInitStruct->TIM_Pulse;
  
  if((*(u32*)&TIMx == TIM1_BASE) || (*(u32*)&TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* Reset the Output N Polarity level [��λ���N���Եȼ�]*/
    tmpccer &= CCER_CC3NP_Reset;

    /* Set the Output N Polarity [�������N����]*/
    tmpccer |= (u16)(TIM_OCInitStruct->TIM_OCNPolarity << 8);

    /* Reset the Output N State [��λ���N״̬]*/
    tmpccer &= CCER_CC3NE_Reset;
    
    /* Set the Output N State [�������N״̬]*/
    tmpccer |= (u16)(TIM_OCInitStruct->TIM_OutputNState << 8);

    /* Reset the Ouput Compare and Output Compare N IDLE State [��λ����ȽϺ�����Ƚ�N IDLE״̬]*/
    tmpcr2 &= CR2_OIS3_Reset;
    tmpcr2 &= CR2_OIS3N_Reset;

    /* Set the Output Idle state [�������IDLE״̬]*/
    tmpcr2 |= (u16)(TIM_OCInitStruct->TIM_OCIdleState << 4);

    /* Set the Output N Idle state [�������N IDLE״̬]*/
    tmpcr2 |= (u16)(TIM_OCInitStruct->TIM_OCNIdleState << 4);
  }

  /* Write to TIMx CR2 [дTIMx���üĴ���2]*/
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR2 [дTIMx����/�Ƚ�ģʽ�Ĵ���2]*/
  TIMx->CCMR2 = tmpccmrx;
  
  /* Write to TIMx CCER [дTIMx����/�Ƚ�ʹ�ܼĴ���]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TIM_OC4Init
* ��������: ����TIM_OCInitStruct�е��ض�������ʼ��TIMxͨ��4��
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_OCInitStruct:ָ��һ�������ض�TIMx�����������Ϣ��TIM_OCInitTypeDef�ṹ��ָ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC4Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  u16 tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   

  /* Disable the Channel 2: Reset the CC4E Bit [��ֹͨ��2����λCC4Eλ]*/
  TIMx->CCER &= CCER_CC4E_Reset;
  
  /* Get the TIMx CCER register value [ȡ��TIMx ����/�Ƚ�ʹ�ܼĴ����ļĴ���ֵ]*/
  tmpccer = TIMx->CCER;

  /* Get the TIMx CR2 register value [ȡ��TIMx ����2�ļĴ���ֵ]*/
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR2 register value [ȡ��TIMx ����/�Ƚ�ģʽ�Ĵ���2�ļĴ���ֵ]*/
  tmpccmrx = TIMx->CCMR2;
    
  /* Reset the Output Compare Mode Bits [��λ����Ƚ�ģʽλ]*/
  tmpccmrx &= CCMR_OC24M_Mask;
  
  /* Select the Output Compare Mode [ѡ������Ƚ�ģʽλ]*/
  tmpccmrx |= (u16)(TIM_OCInitStruct->TIM_OCMode << 8);
  
  /* Reset the Output Polarity level [��λ������Լ���]*/
  tmpccer &= CCER_CC4P_Reset;

  /* Set the Output Compare Polarity [��������Ƚϼ���]*/
  tmpccer |= (u16)(TIM_OCInitStruct->TIM_OCPolarity << 12);
  
  /* Set the Output State [�������״̬]*/
  tmpccer |= (u16)(TIM_OCInitStruct->TIM_OutputState << 12);
  
  /* Set the Capture Compare Register value [���ò���ȽϼĴ���ֵ]*/
  TIMx->CCR4 = TIM_OCInitStruct->TIM_Pulse;
  
  if((*(u32*)&TIMx == TIM1_BASE) || (*(u32*)&TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));

    /* Reset the Ouput Compare IDLE State [��������Ƚ�IDLE״̬]*/
    tmpcr2 &= CR2_OIS4_Reset;

    /* Set the Output Idle state [�������IDLE״̬]*/
    tmpcr2 |= (u16)(TIM_OCInitStruct->TIM_OCIdleState << 6);
  }

  /* Write to TIMx CR2 [дTIMx���üĴ���2]*/
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR2 [дTIMx����/�Ƚ�ģʽ�Ĵ���2]*/  
  TIMx->CCMR2 = tmpccmrx;
  
  /* Write to TIMx CCER [дTIMx����/�Ƚ�ʹ�ܼĴ���]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TIM_ICInit
* ��������: ����TIM_ ICInitStruct�е��ض�������ʼ��TIMx.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_OCInitStruct:ָ��һ�������ض�TIMx�����������Ϣ��TIM_OCInitTypeDef�ṹ��ָ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_ICInitStruct->TIM_Channel));
  assert_param(IS_TIM_IC_POLARITY(TIM_ICInitStruct->TIM_ICPolarity));
  assert_param(IS_TIM_IC_SELECTION(TIM_ICInitStruct->TIM_ICSelection));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICInitStruct->TIM_ICPrescaler));
  assert_param(IS_TIM_IC_FILTER(TIM_ICInitStruct->TIM_ICFilter));
  
  if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1)
  {
    /* TI1 Configuration [TI1����]*/
    TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [�������벶׽Ԥ��Ƶ��ֵ]*/
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_2)
  {
    /* TI2 Configuration [TI2����]*/
    TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [�������벶׽Ԥ��Ƶ��ֵ]*/
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_3)
  {
    /* TI3 Configuration [TI2����]*/
    TI3_Config(TIMx,  TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [�������벶׽Ԥ��Ƶ��ֵ]*/
    TIM_SetIC3Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else
  {
    /* TI4 Configuration [TI2����]*/
    TI4_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [�������벶׽Ԥ��Ƶ��ֵ]*/
    TIM_SetIC4Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
}

/*******************************************************************************
* ��������: TIM_PWMIConfig
* ��������: ����TIM_ICInitStructָ���Ĳ�������TIM�豸ȥ�����ⲿ��PWM�źš�
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_ICInitStruct:ָ��һ�������ض�TIMx�����������Ϣ��TIM_ICInitTypeDef�ṹ��ָ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_PWMIConfig(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  u16 icoppositepolarity = TIM_ICPolarity_Rising;
  u16 icoppositeselection = TIM_ICSelection_DirectTI;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Select the Opposite Input Polarity [ѡ���෴�����뼫��]*/
  if (TIM_ICInitStruct->TIM_ICPolarity == TIM_ICPolarity_Rising)
  {
    icoppositepolarity = TIM_ICPolarity_Falling;
  }
  else
  {
    icoppositepolarity = TIM_ICPolarity_Rising;
  }

  /* Select the Opposite Input [ѡ���෴������]*/
  if (TIM_ICInitStruct->TIM_ICSelection == TIM_ICSelection_DirectTI)
  {
    icoppositeselection = TIM_ICSelection_IndirectTI;
  }
  else
  {
    icoppositeselection = TIM_ICSelection_DirectTI;
  }

  if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1)
  {
    /* TI1 Configuration [TI1����]*/
    TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [�������벶׽Ԥ��Ƶ��ֵ]*/
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);

    /* TI2 Configuration [TI2����]*/
    TI2_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [�������벶׽Ԥ��Ƶ��ֵ]*/
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else
  { 
    /* TI2 Configuration [TI2����]*/
    TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [�������벶׽Ԥ��Ƶ��ֵ]*/
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);

    /* TI1 Configuration [TI1����]*/
    TI1_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [�������벶׽Ԥ��Ƶ��ֵ]*/
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
}

/*******************************************************************************
* ��������: TIM_BDTRConfig
* ��������: ���ã�����������ͺ�ʱ�䣬������OSSI��OSSR״̬��AOE���Զ����ʹ�ܣ�.
* �������:(1)TIMx:�˴�x������1��8��ѡ������.
*          (2)TIM_BDTRInitStruct:ָ��һ�������ض�TIMx�����������Ϣ��TIM_BDTRInitTypeDef�ṹ��ָ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_BDTRConfig(TIM_TypeDef* TIMx, TIM_BDTRInitTypeDef *TIM_BDTRInitStruct)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OSSR_STATE(TIM_BDTRInitStruct->TIM_OSSRState));
  assert_param(IS_TIM_OSSI_STATE(TIM_BDTRInitStruct->TIM_OSSIState));
  assert_param(IS_TIM_LOCK_LEVEL(TIM_BDTRInitStruct->TIM_LOCKLevel));
  assert_param(IS_TIM_BREAK_STATE(TIM_BDTRInitStruct->TIM_Break));
  assert_param(IS_TIM_BREAK_POLARITY(TIM_BDTRInitStruct->TIM_BreakPolarity));
  assert_param(IS_TIM_AUTOMATIC_OUTPUT_STATE(TIM_BDTRInitStruct->TIM_AutomaticOutput));

  /* Set the Lock level, the Break enable Bit and the Polarity, the OSSR State,
  the OSSI State, the dead time value and the Automatic Output Enable Bit [���ã������𣬼������λ�ͼ��ԣ�OSSR״̬��OSSI״̬���ͺ�ʱ����Զ����ʹ��λ]*/

  TIMx->BDTR = (u32)TIM_BDTRInitStruct->TIM_OSSRState | TIM_BDTRInitStruct->TIM_OSSIState |
             TIM_BDTRInitStruct->TIM_LOCKLevel | TIM_BDTRInitStruct->TIM_DeadTime |
             TIM_BDTRInitStruct->TIM_Break | TIM_BDTRInitStruct->TIM_BreakPolarity |
             TIM_BDTRInitStruct->TIM_AutomaticOutput;

}

/*******************************************************************************
* ��������: TIM_TimeBaseStructInit
* ��������: ʹ��ȱʡֵ���TIM_TimeBaseInitStruct��ÿһ����Ա.
* �������: TIM_TimeBaseInitStruct:ָ��һ��������ʼ����TIM_TimeBaseInitTypeDef�ṹ��ָ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  /* Set the default configuration [���ó�Ĭ������]*/
  TIM_TimeBaseInitStruct->TIM_Period = 0xFFFF;
  TIM_TimeBaseInitStruct->TIM_Prescaler = 0x0000;
  TIM_TimeBaseInitStruct->TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct->TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct->TIM_RepetitionCounter = 0x0000;
}

/*******************************************************************************
* ��������: TIM_OCStructInit
* ��������: ʹ��ȱʡֵ���TIM_OCInitStruct��ÿһ����Ա
* �������: TIM_OCInitStruct:ָ��һ��������ʼ����TIM_OCInitStruct�ṹ��ָ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OCStructInit(TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  /* Set the default configuration [���ó�Ĭ������]*/
  TIM_OCInitStruct->TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStruct->TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStruct->TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStruct->TIM_Pulse = 0x0000;
  TIM_OCInitStruct->TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct->TIM_OCNPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct->TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStruct->TIM_OCNIdleState = TIM_OCNIdleState_Reset;
}

/*******************************************************************************
* ��������: TIM_ICStructInit
* ��������: ʹ��ȱʡֵ���TIM_ICInitStruct��ÿһ����Ա.
* �������: TIM_ICInitStruct:ָ��һ��������ʼ����TIM_ICInitStruct�ṹ��ָ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ICStructInit(TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  /* Set the default configuration [���ó�Ĭ������]*/
  TIM_ICInitStruct->TIM_Channel = TIM_Channel_1;
  TIM_ICInitStruct->TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStruct->TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStruct->TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStruct->TIM_ICFilter = 0x00;
}

/*******************************************************************************
* ��������: TIM_BDTRStructInit
* ��������: ��TIM1_BDTRStruct�еĳ�Ա��ΪĬ��ֵ.
* �������: TIM1_BDTRInitStruct:ָ��һ��������ʼ����TIM1_BDTRInitStruct�ṹ�塣
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_BDTRStructInit(TIM_BDTRInitTypeDef* TIM_BDTRInitStruct)
{
  /* Set the default configuration [���ó�Ĭ������]*/
  TIM_BDTRInitStruct->TIM_OSSRState = TIM_OSSRState_Disable;
  TIM_BDTRInitStruct->TIM_OSSIState = TIM_OSSIState_Disable;
  TIM_BDTRInitStruct->TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStruct->TIM_DeadTime = 0x00;
  TIM_BDTRInitStruct->TIM_Break = TIM_Break_Disable;
  TIM_BDTRInitStruct->TIM_BreakPolarity = TIM_BreakPolarity_Low;
  TIM_BDTRInitStruct->TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
}

/*******************************************************************************
* ��������: TIM_Cmd
* ��������: ʹ�ܻ�ر��ض���TIM����.
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)NewState:��TIM�������״̬���ò���������ENABLE��DISABLE��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the TIM Counter [����tim������]*/
    TIMx->CR1 |= CR1_CEN_Set;
  }
  else
  {
    /* Disable the TIM Counter [��ֹtim������]*/
    TIMx->CR1 &= CR1_CEN_Reset;
  }
}

/*******************************************************************************
* ��������: TIM_CtrlPWMOutputs
* ��������: ʹ�ܻ����TIM��Χ�����.
* �������:(1)TIMx:�˴�x������1��8��ѡ������.
*          (2)Newstate��TIM1��Χ�豸���������״̬����������ܹ�ȡ��ENABLE����DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_CtrlPWMOutputs(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the TIM Main Output [����TIM�����]*/
    TIMx->BDTR |= BDTR_MOE_Set;
  }
  else
  {
    /* Disable the TIM Main Output [��ֹTIM�����]*/
    TIMx->BDTR &= BDTR_MOE_Reset;
  }  
}

/*******************************************************************************
* ��������: TIM_ITConfig
* ��������: ʹ�ܻ�ر��ض���TIM�ж�.
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)TIM_IT: ����ʹ�ܻ�رյ�TIM�ж�Դ��
*                    TIM_IT������������ֵ:
*                       - TIM_IT_Update: TIM�����ж�Դ
*                       - TIM_IT_CC1: TIM����/�Ƚ�1�ж�Դ
*                       - TIM_IT_CC2: TIM����/�Ƚ�2�ж�Դ
*                       - TIM_IT_CC3: TIM����/�Ƚ�3�ж�Դ
*                       - TIM_IT_CC4: TIM����/�Ƚ�4�ж�Դ
*                       - TIM_IT_COM: TIM �����ж�Դ
*                       - TIM_IT_Trigger: TIM�������ж�Դ
*                       - TIM_IT_Break: TIM1����ж�Դ
*           (3)NewState:�ض�TIM1�жϵ���״̬����������ܹ�ȡ��ENABLE����DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ITConfig(TIM_TypeDef* TIMx, u16 TIM_IT, FunctionalState NewState)
{  
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_IT(TIM_IT));
  assert_param(IS_TIM_PERIPH_IT((TIMx), (TIM_IT)));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the Interrupt sources [�����ж�Դ]*/
    TIMx->DIER |= TIM_IT;
  }
  else
  {
    /* Disable the Interrupt sources [��ֹ�ж�Դ]*/
    TIMx->DIER &= (u16)~TIM_IT;
  }
}

/*******************************************************************************
* ��������: TIM_GenerateEvent
* ��������: ���ý������������TIM�¼�.
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           TIM1_EventSource��ָ��TIM1����¼�Դ��
*                    �����������ȡ�����ֵ֮һ:�����������ȡ�����ֵ֮һ��    
*                       - TIM_EventSource_Update: ��ʱ�������¼�Դ
*                       - TIM_EventSource_CC1: ��ʱ���Ƚϲ�׽1�¼�Դ
*                       - TIM_EventSource_CC2: ��ʱ���Ƚϲ�׽2�¼�Դ
*                       - TIM_EventSource_CC3: ��ʱ���Ƚϲ�׽3�¼�Դ
*                       - TIM_EventSource_CC4: ��ʱ���Ƚϲ�׽4�¼�Դ
*                       - TIM_EventSource_Trigger: ��ʱ���������¼�Դ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_GenerateEvent(TIM_TypeDef* TIMx, u16 TIM_EventSource)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_EVENT_SOURCE(TIM_EventSource));
  assert_param(IS_TIM_PERIPH_EVENT((TIMx), (TIM_EventSource)));

  /* Set the event sources [�����¼�Դ]*/
  TIMx->EGR = TIM_EventSource;
}

/*******************************************************************************
* ��������: TIM_DMAConfig
* ��������: ����ĳ��TIM��DMA�ӿ�.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_DMABase: DMA��ַ��
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_DMABase_CR, TIM_DMABase_CR2, TIM_DMABase_SMCR,
*                         TIM_DMABase_DIER, TIM1_DMABase_SR, TIM_DMABase_EGR,
*                         TIM_DMABase_CCMR1, TIM_DMABase_CCMR2, TIM_DMABase_CCER,
*                         TIM_DMABase_CNT, TIM_DMABase_PSC, TIM_DMABase_ARR,
*                         TIM_DMABase_RCR, TIM_DMABase_CCR1, TIM_DMABase_CCR2,
*                         TIM_DMABase_CCR3, TIM_DMABase_CCR4, TIM_DMABase_BDTR,
*                         TIM_DMABase_DCR.
*           (3)TIM_DMABurstLength: DMA⧷����ȡ�
*                     �����������ȡ��������ֵ֮һ:
*                     TIM_DMABurstLength_1Byte and TIM_DMABurstLength_18Bytes.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_DMAConfig(TIM_TypeDef* TIMx, u16 TIM_DMABase, u16 TIM_DMABurstLength)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_DMA_BASE(TIM_DMABase));
  assert_param(IS_TIM_DMA_LENGTH(TIM_DMABurstLength));

  /* Set the DMA Base and the DMA Burst Length [����DMA��ַ��DMA⧷�����]*/
  TIMx->DCR = TIM_DMABase | TIM_DMABurstLength;
}

/*******************************************************************************
* ��������: TIM_DMACmd
* ��������: ʹ�ܻ�ر�TIMx��DMA����.
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)TIM_DMASource: DMA����Դ��
*                    ������������������κ�ֵ����ϣ�
*                       - TIM_DMA_Update: TIM �����ж�Դ
*                       - TIM_DMA_CC1: TIM ����Ƚ� 1 DMA Դ
*                       - TIM_DMA_CC2: TIM ����Ƚ� 2 DMA Դ
*                       - TIM_DMA_CC3: TIM ����Ƚ� 3 DMA Դ
*                       - TIM_DMA_CC4: TIM ����Ƚ� 4 DMA Դ
*                       - TIM_DMA_COM: TIM ���� DMA Դ
*                       - TIM_DMA_Trigger: TIM ���� DMA Դ
*           (3)NewState:��TIM DMA����Դ����״̬���ò���������ENABLE��DISABLE��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_DMACmd(TIM_TypeDef* TIMx, u16 TIM_DMASource, FunctionalState NewState)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_DMA_SOURCE(TIM_DMASource));
  assert_param(IS_TIM_PERIPH_DMA(TIMx, TIM_DMASource));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the DMA sources [����DMAԴ]*/
    TIMx->DIER |= TIM_DMASource; 
  }
  else
  {
    /* Disable the DMA sources [��ֹDMAԴ]*/
    TIMx->DIER &= (u16)~TIM_DMASource;
  }
}

/*******************************************************************************
* ��������: TIM_InternalClockConfig
* ��������: ����TIM���ڲ�ʱ��
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_InternalClockConfig(TIM_TypeDef* TIMx)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Disable slave mode to clock the prescaler directly with the internal clock [��ֹ�ڲ�ʱ��Ԥ��Ƶ����ģʽ]*/
  TIMx->SMCR &=  SMCR_SMS_Mask;
}
/*******************************************************************************
* ��������: TIM_ITRxExternalClockConfig
* ��������: ����TIM���ڲ���������Ϊ�ⲿʱ��
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_InputTriggerSource:���봥��Դ��
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_TS_ITR0: �ڲ������� 0
*                       - TIM_TS_ITR1: �ڲ������� 1
*                       - TIM_TS_ITR2: �ڲ������� 2
*                       - TIM_TS_ITR3: �ڲ������� 3
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, u16 TIM_InputTriggerSource)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_INTERNAL_TRIGGER_SELECTION(TIM_InputTriggerSource));

  /* Select the Internal Trigger [ѡ���ڲ�������]*/
  TIM_SelectInputTrigger(TIMx, TIM_InputTriggerSource);

  /* Select the External clock mode1 [ѡ���ⲿʱ��ģʽ1]*/
  TIMx->SMCR |= TIM_SlaveMode_External1;
}
/*******************************************************************************
* ��������: TIM_TIxExternalClockConfig
* ��������: ����TIM�Ĵ�������Ϊ�ⲿʱ��
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_ TIxExternalCLKSource: ������Դ��
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_TIxExternalCLK1Source_TI1ED: TI1 ��Ե�����
*                       - TIM_TIxExternalCLK1Source_TI1: ��ɸѡ�Ķ�ʱ������1
*                       - TIM_TIxExternalCLK1Source_TI2: ��ɸѡ�Ķ�ʱ������2
*           (3)TIM_ICPolarity: ָ��TI�ļ���
*                    �������������:
*                       - TIM_ICPolarity_Rising[������]
*                       - TIM_ICPolarity_Falling[�½���]
*           (4)ICFilter:ָ�����벶����������ò���������0x0��0xF֮���ֵ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, u16 TIM_TIxExternalCLKSource,
                                u16 TIM_ICPolarity, u16 ICFilter)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_TIXCLK_SOURCE(TIM_TIxExternalCLKSource));
  assert_param(IS_TIM_IC_POLARITY(TIM_ICPolarity));
  assert_param(IS_TIM_IC_FILTER(ICFilter));

  /* Configure the Timer Input Clock Source [���ö�ʱ������ʱ��Դ]*/
  if (TIM_TIxExternalCLKSource == TIM_TIxExternalCLK1Source_TI2)
  {
    TI2_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
  }
  else
  {
    TI1_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
  }

  /* Select the Trigger source [ѡ�񴥷���Դ]*/
  TIM_SelectInputTrigger(TIMx, TIM_TIxExternalCLKSource);

  /* Select the External clock mode1 [ѡ���ⲿʱ��ģʽ1]*/
  TIMx->SMCR |= TIM_SlaveMode_External1;
}

/*******************************************************************************
* ��������: TIM_ETRClockMode1Config
* ��������: �����ⲿʱ��ģʽ1
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_ExtTRGPrescaler:�ⲿ������Ԥ��Ƶ����
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ExtTRGPSC_OFF
*                       - TIM_ExtTRGPSC_DIV2
*                       - TIM_ExtTRGPSC_DIV4
*                       - TIM_ExtTRGPSC_DIV8.
*           (3)TIM_ExtTRGPolarity:�ⲿ���������ԡ�
*                     �����������ȡ�����ֵ֮һ:
*                       - TIM_ExtTRGPolarity_Inverted
*                       - TIM_ExtTRGPolarity_NonInverted
*           (4)ExtTRGFilter:�ⲿ���������������ò���������0x0��0xF֮���ֵ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, u16 TIM_ExtTRGPrescaler, u16 TIM_ExtTRGPolarity,
                             u16 ExtTRGFilter)
{
  u16 tmpsmcr = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));

  /* Configure the ETR Clock source [�����ⲿ������ʱ��Դ]*/
  TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);
  
  /* Get the TIMx SMCR register value [ȡ��TIMx SMCR�Ĵ���ֵ]*/
  tmpsmcr = TIMx->SMCR;

  /* Reset the SMS Bits [��λSMS]*/
  tmpsmcr &= SMCR_SMS_Mask;
  /* Select the External clock mode1 [ѡ���ⲿʱ��ģʽ1]*/
  tmpsmcr |= TIM_SlaveMode_External1;

  /* Select the Trigger selection : ETRF [ѡ�񴥷�ѡ��: ETRF]*/
  tmpsmcr &= SMCR_TS_Mask;
  tmpsmcr |= TIM_TS_ETRF;

  /* Write to TIMx SMCR [дTIMx SMCR]*/
  TIMx->SMCR = tmpsmcr;
}

/*******************************************************************************
* ��������: TIM_ETRClockMode2Config
* ��������: ����TIMx�ⲿʱ��Ϊģʽ2
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_ExtTRGPrescaler:�ⲿ������Ԥ��Ƶ��.
*                    �������������ֵ֮һ:
*                       - TIM_ExtTRGPSC_OFF
*                       - TIM_ExtTRGPSC_DIV2
*                       - TIM_ExtTRGPSC_DIV4
*                       - TIM_ExtTRGPSC_DIV8
*           (3)TIM_ExtTRGPolarity:�ⲿ����������.
*                    �������������ֵ֮һ:
*                       - TIM_ExtTRGPolarity_Inverted
*                       - TIM_ExtTRGPolarity_NonInverted
*           (4)ExtTRGFilter:�ⲿ���������������ò���������0x0��0xF֮���ֵ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, u16 TIM_ExtTRGPrescaler, 
                             u16 TIM_ExtTRGPolarity, u16 ExtTRGFilter)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));

  /* Configure the ETR Clock source [����ETR��ʱ��Դ]*/
  TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);

  /* Enable the External clock mode2 [ʹ���ⲿʱ��ģʽ2]*/
  TIMx->SMCR |= SMCR_ECE_Set;
}

/*******************************************************************************
* ��������: TIM_ETRConfig
* ��������: ����TIMx���ⲿ����(ETR).
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_ExtTRGPrescaler:�ⲿ������Ԥ��Ƶ��.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ExtTRGPSC_OFF
*                       - TIM_ExtTRGPSC_DIV2
*                       - TIM_ExtTRGPSC_DIV4
*                       - TIM_ExtTRGPSC_DIV8
*           (3)TIM_ExtTRGPolarity:�ⲿ����������.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ExtTRGPolarity_Inverted
*                       - TIM_ExtTRGPolarity_NonInverted
*           (4)ExtTRGFilter:�ⲿ���������������ò���������0x0��0xF֮���ֵ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ETRConfig(TIM_TypeDef* TIMx, u16 TIM_ExtTRGPrescaler, u16 TIM_ExtTRGPolarity,
                   u16 ExtTRGFilter)
{
  u16 tmpsmcr = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));

  tmpsmcr = TIMx->SMCR;

  /* Reset the ETR Bits [��λETRλ]*/
  tmpsmcr &= SMCR_ETR_Mask;

  /* Set the Prescaler, the Filter value and the Polarity [����Ԥ��Ƶ����������ֵ�ͼ���]*/
  tmpsmcr |= TIM_ExtTRGPrescaler | TIM_ExtTRGPolarity | (u16)(ExtTRGFilter << 8);

  /* Write to TIMx SMCR [дTIMx SMCR]*/
  TIMx->SMCR = tmpsmcr;
}

/*******************************************************************************
* ��������: TIM_PrescalerConfig
* ��������: Configures the TIMx Prescaler.
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)Prescaler:TIMxԤ��Ƶ������ֵ
*           (3)TIM_PSCReloadMode:TIMԤ��Ƶ������ģʽ.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_PSCReloadMode_Update: ���������¼�ʱԤ��Ƶ������.
*                       - TIM_PSCReloadMode_Immediate: Ԥ��Ƶ����������.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, u16 Prescaler, u16 TIM_PSCReloadMode)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_PRESCALER_RELOAD(TIM_PSCReloadMode));

  /* Set the Prescaler value [����Ԥ��Ƶֵ]*/
  TIMx->PSC = Prescaler;

  /* Set or reset the UG Bit [��λ��λUG]*/
  TIMx->EGR = TIM_PSCReloadMode;
}

/*******************************************************************************
* ��������: TIM_CounterModeConfig
* ��������: ָ��TIMxʹ�ü���ģʽ.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_CounterMode:��ʹ�õļ���ģʽ.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_CounterMode_Up: TIM���ϼ���ģʽ
*                       - TIM_CounterMode_Down: TIM���¼���ģʽ
*                       - TIM_CounterMode_CenterAligned1: TIM��������ģʽ1
*                       - TIM_CounterMode_CenterAligned2: TIM��������ģʽ2
*                       - TIM_CounterMode_CenterAligned3: TIM��������ģʽ3
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_CounterModeConfig(TIM_TypeDef* TIMx, u16 TIM_CounterMode)
{
  u16 tmpcr1 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_COUNTER_MODE(TIM_CounterMode));

  tmpcr1 = TIMx->CR1;

  /* Reset the CMS and DIR Bits [��λCMS��DIR]*/
  tmpcr1 &= CR1_CounterMode_Mask;

  /* Set the Counter Mode [���ü���ģʽ]*/
  tmpcr1 |= TIM_CounterMode;

  /* Write to TIMx CR1 register [дTIMx CR1 �Ĵ���]*/
  TIMx->CR1 = tmpcr1;
}

/*******************************************************************************
* ��������: TIM_SelectInputTrigger
* ��������: ѡ�����봥��Դ
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_InputTriggerSource: ���봥��Դ.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_TS_ITR0: �ڲ����� 0
*                       - TIM_TS_ITR1: �ڲ����� 1
*                       - TIM_TS_ITR2: �ڲ����� 2
*                       - TIM_TS_ITR3: �ڲ����� 3
*                       - TIM_TS_TI1F_ED: TI1 ���ؼ��
*                       - TIM_TS_TI1FP1: ������ʱ������ 1
*                       - TIM_TS_TI2FP2: ������ʱ������ 2
*                       - TIM_TS_ETRF: ��չ��������
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, u16 TIM_InputTriggerSource)
{
  u16 tmpsmcr = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_TRIGGER_SELECTION(TIM_InputTriggerSource));

  /* Get the TIMx SMCR register value [ȡ��TIMx SMCR�Ĵ���ֵ]*/
  tmpsmcr = TIMx->SMCR;

  /* Reset the TS Bits [��λTS]*/
  tmpsmcr &= SMCR_TS_Mask;

  /* Set the Input Trigger source [���������ж�Դ]*/
  tmpsmcr |= TIM_InputTriggerSource;

  /* Write to TIMx SMCR [дTIMx SMCR]*/
  TIMx->SMCR = tmpsmcr;
}

/*******************************************************************************
* ��������: TIM_EncoderInterfaceConfig
* ��������: ����TIM�ı������ӿ�
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)IM_EncoderMode:������ģʽ.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_EncoderMode_TI1: TIM������ʹ��ģʽ1.
*                       - TIM_EncoderMode_TI2: TIM������ʹ��ģʽ2.
*                       - TIM_EncoderMode_TI12:TIM������ʹ��ģʽ3.
*            (3)TIM_IC1Polarity: TI1�ļ���.
*                    �����������ȡ�����ֵ֮һ:
*                        - TIM_ICPolarity_Falling: ���벶���½���.
*                        - TIM_ICPolarity_Rising: ���벶��������.
*            (4)TIM_IC2Polarity:TI2�ļ���.
*                    �����������ȡ�����ֵ֮һ:
*                        - TIM_ICPolarity_Falling: ���벶���½���.
*                        - TIM_ICPolarity_Rising: ���벶��������.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_EncoderInterfaceConfig(TIM_TypeDef* TIMx, u16 TIM_EncoderMode,
                                u16 TIM_IC1Polarity, u16 TIM_IC2Polarity)
{
  u16 tmpsmcr = 0;
  u16 tmpccmr1 = 0;
  u16 tmpccer = 0;
    
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_ENCODER_MODE(TIM_EncoderMode));
  assert_param(IS_TIM_IC_POLARITY(TIM_IC1Polarity));
  assert_param(IS_TIM_IC_POLARITY(TIM_IC2Polarity));

  /* Get the TIMx SMCR register value [ȡ��TIMx SMCR�Ĵ���ֵ]*/
  tmpsmcr = TIMx->SMCR;

  /* Get the TIMx CCMR1 register value [ȡ��TIMx ����/�Ƚ�ģʽ�Ĵ���1�ļĴ���ֵ]*/
  tmpccmr1 = TIMx->CCMR1;

  /* Get the TIMx CCER register value [ȡ��TIMx ����/�Ƚ�ʹ�ܼĴ����ļĴ���ֵ]*/
  tmpccer = TIMx->CCER;

  /* Set the encoder Mode [���ñ���ģʽ]*/
  tmpsmcr &= SMCR_SMS_Mask;
  tmpsmcr |= TIM_EncoderMode;

  /* Select the Capture Compare 1 and the Capture Compare 2 as input [ѡ��Ƚϲ�׽1�ͽϲ�׽2��Ϊ����]*/
  tmpccmr1 &= CCMR_CC13S_Mask & CCMR_CC24S_Mask;
  tmpccmr1 |= CCMR_TI13Direct_Set | CCMR_TI24Direct_Set;

  /* Set the TI1 and the TI2 Polarities [����TI1��TI2�ļ���]*/
  tmpccer &= CCER_CC1P_Reset & CCER_CC2P_Reset;
  tmpccer |= (TIM_IC1Polarity | (u16)(TIM_IC2Polarity << 4));

  /* Write to TIMx SMCR [дTIMx SMCR]*/
  TIMx->SMCR = tmpsmcr;

  /* Write to TIMx CCMR1 [дTIMx����/�Ƚ�ģʽ�Ĵ���1]*/
  TIMx->CCMR1 = tmpccmr1;

  /* Write to TIMx CCER [дTIMx����/�Ƚ�ʹ�ܼĴ���]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TIM_ForcedOC1Config
* ��������: ǿ��TIMx�ŵ�1��������ε���Ч��ƽ������Ч��ƽ.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM1_ForcedAction��ָ����������ε�ǿ����Ϊ..
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ForcedAction_Active: ǿ��OCxREFΪ��Ч��ƽ
*                       - TIM_ForcedAction_InActive: ǿ��OCxREFΪ��Ч��ƽ.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ForcedOC1Config(TIM_TypeDef* TIMx, u16 TIM_ForcedAction)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC1M Bits [��λOC1M]*/
  tmpccmr1 &= CCMR_OC13M_Mask;

  /* Configure The Forced output Mode [����ǿ�����ģʽ]*/
  tmpccmr1 |= TIM_ForcedAction;

  /* Write to TIMx CCMR1 register [дTIMx CCMR1�Ĵ���]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* ��������: TIM_ForcedOC2Config
* ��������: ǿ��TIMx�ŵ�2��������ε���Ч����Ч��ƽ��.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIMx_ForcedAction��ָ����������ε�ǿ����Ϊ.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ForcedAction_Active: ǿ��OC2REFΪ��Ч��ƽ
*                       - TIM_ForcedAction_InActive:ǿ��OC2REFΪ��Ч��ƽ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ForcedOC2Config(TIM_TypeDef* TIMx, u16 TIM_ForcedAction)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC2M Bits [��λOC2M]*/
  tmpccmr1 &= CCMR_OC24M_Mask;

  /* Configure The Forced output Mode [����ǿ�����ģʽ]*/
  tmpccmr1 |= (u16)(TIM_ForcedAction << 8);

  /* Write to TIMx CCMR1 register [дTIMx CCMR1�Ĵ���]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* ��������: TIM_ForcedOC3Config
* ��������: ǿ��TIMx�ŵ�3��������ε���Ч����Ч��ƽ��
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIMx_ForcedAction��ָ����������ε�ǿ����Ϊ.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ForcedAction_Active: ǿ��OC3REFΪ��Ч��ƽ
*                       - TIM_ForcedAction_InActive:ǿ��OC3REFΪ��Ч��ƽ.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ForcedOC3Config(TIM_TypeDef* TIMx, u16 TIM_ForcedAction)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC1M Bits [��λOC1M]*/
  tmpccmr2 &= CCMR_OC13M_Mask;

  /* Configure The Forced output Mode [����ǿ�����ģʽ]*/
  tmpccmr2 |= TIM_ForcedAction;

  /* Write to TIMx CCMR2 register [дTIMx CCMR2�Ĵ���]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* ��������: TIM_ForcedOC4Config
* ��������: ǿ��TIMx�ŵ�4��������ε���Ч����Ч��ƽ��
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIMx_ForcedAction��ָ����������ε�ǿ����Ϊ.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ForcedAction_Active: ǿ��OC4REFΪ��Ч��ƽ
*                       - TIM_ForcedAction_InActive:ǿ��OC4REFΪ��Ч��ƽ.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ForcedOC4Config(TIM_TypeDef* TIMx, u16 TIM_ForcedAction)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC2M Bits [��λOC2M]*/
  tmpccmr2 &= CCMR_OC24M_Mask;

  /* Configure The Forced output Mode [����ǿ�����ģʽ]*/
  tmpccmr2 |= (u16)(TIM_ForcedAction << 8);

  /* Write to TIMx CCMR2 register [дTIMx CCMR2�Ĵ���]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* ��������: TIM_ARRPreloadConfig
* ��������: ʹ�ܻ����TIMx����ARRԤ�ؼĴ���.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)Newstate:��TIM1_CR1�Ĵ����е�ARPEλ����״̬.��������ܹ�ȡ:ENABLE����DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the ARR Preload Bit [��λARR��Ԥװ��λ]*/
    TIMx->CR1 |= CR1_ARPE_Set;
  }
  else
  {
    /* Reset the ARR Preload Bit [��λARR��Ԥװ��λ]*/
    TIMx->CR1 &= CR1_ARPE_Reset;
  }
}

/*******************************************************************************
* ��������: TIM_SelectCOM
* ��������: ѡ��TIM��Χ�豸�����¼�.
* �������:(1)TIMx:�˴�x������1��8��ѡ������.
*          (2)Newstate�������¼�����״̬.��������ܹ�ȡ��ENABLE����DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SelectCOM(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the COM Bit [��λCOMλ]*/
    TIMx->CR2 |= CR2_CCUS_Set;
  }
  else
  {
    /* Reset the COM Bit [��λCOMλ]*/
    TIMx->CR2 &= CR2_CCUS_Reset;
  }
}

/*******************************************************************************
* ��������: TIM_SelectCCDMA
* ��������: ѡ��TIMx��Χ�豸����Ƚ�DMAԴ.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)Newstate������Ƚ�DMAԴ����״̬.��������ܹ�ȡ��ENABLE����DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SelectCCDMA(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the CCDS Bit [��λCCDSλ]*/
    TIMx->CR2 |= CR2_CCDS_Set;
  }
  else
  {
    /* Reset the CCDS Bit [��λCCDSλ]*/
    TIMx->CR2 &= CR2_CCDS_Reset;
  }
}

/*******************************************************************************
* ��������: TIM_CCPreloadControl
* ��������: ���û�����TIM��Χ�豸����Ƚ�Ԥ�ؿ���λ.
* �������:(1)TIMx:�˴�x������1��8��ѡ������.
*          (2)Newstate������Ƚ�Ԥ�ؿ���λ����״̬.��������ܹ�ȡ��ENABLE����DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_CCPreloadControl(TIM_TypeDef* TIMx, FunctionalState NewState)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the CCPC Bit [��λCCPC]*/
    TIMx->CR2 |= CR2_CCPC_Set;
  }
  else
  {
    /* Reset the CCPC Bit [��λCCPC]*/
    TIMx->CR2 &= CR2_CCPC_Reset;
  }
}

/*******************************************************************************
* ��������: TIM_OC1PreloadConfig
* ��������: ʹ�ܻ����CCR1��TIMxԤ�ؼĴ���.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM1_OCPreload������Ƚ�����״̬,
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCPreload_Enable
*                       - TIM_OCPreload_Disable
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, u16 TIM_OCPreload)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC1PE Bit [��λOC1PE]*/
  tmpccmr1 &= CCMR_OC13PE_Reset;

  /* Enable or Disable the Output Compare Preload feature [ʹ�ܻ��ֹ����Ƚ�Ԥװ������]*/
  tmpccmr1 |= TIM_OCPreload;

  /* Write to TIMx CCMR1 register [дTIMx CCMR1�Ĵ���]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* ��������: TIM_OC2PreloadConfig
* ��������: ʹ�ܻ����CCR2��TIMxԤ�ؼĴ���.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM1_OCPreload������Ƚ�����״̬,
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCPreload_Enable
*                       - TIM_OCPreload_Disable
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC2PreloadConfig(TIM_TypeDef* TIMx, u16 TIM_OCPreload)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC2PE Bit [��λOC2PE]*/
  tmpccmr1 &= CCMR_OC24PE_Reset;

  /* Enable or Disable the Output Compare Preload feature [ʹ�ܻ��ֹ����Ƚ�Ԥװ������]*/
  tmpccmr1 |= (u16)(TIM_OCPreload << 8);

  /* Write to TIMx CCMR1 register [дTIMx CCMR1�Ĵ���]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* ��������: TIM_OC3PreloadConfig
* ��������: ʹ�ܻ����CCR3��TIMxԤ�ؼĴ���.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM1_OCPreload������Ƚ�����״̬,
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCPreload_Enable
*                       - TIM_OCPreload_Disable
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC3PreloadConfig(TIM_TypeDef* TIMx, u16 TIM_OCPreload)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC3PE Bit [��λOC3PE]*/
  tmpccmr2 &= CCMR_OC13PE_Reset;

  /* Enable or Disable the Output Compare Preload feature [ʹ�ܻ��ֹ����Ƚ�Ԥװ������]*/
  tmpccmr2 |= TIM_OCPreload;

  /* Write to TIMx CCMR2 register [дTIMx CCMR2�Ĵ���]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* ��������: TIM_OC4PreloadConfig
* ��������: ʹ�ܻ����CCR4��TIMxԤ�ؼĴ���.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM1_OCPreload������Ƚ�����״̬,
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCPreload_Enable
*                       - TIM_OCPreload_Disable
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC4PreloadConfig(TIM_TypeDef* TIMx, u16 TIM_OCPreload)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC4PE Bit [��λOC4PE]*/
  tmpccmr2 &= CCMR_OC24PE_Reset;

  /* Enable or Disable the Output Compare Preload feature [ʹ�ܻ��ֹ����Ƚ�Ԥװ������]*/
  tmpccmr2 |= (u16)(TIM_OCPreload << 8);

  /* Write to TIMx CCMR2 register [дTIMx CCMR2�Ĵ���]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* ��������: TIM_OC1FastConfig
* ��������: ����TIMx����Ƚ�1��������.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM1_OCFast:����ȽϿ�������״̬.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCFast_Enable: TIM����ȽϿ�������ʹ��
*                       - TIM_OCFast_Disable: TIM����ȽϿ������ܽ���
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC1FastConfig(TIM_TypeDef* TIMx, u16 TIM_OCFast)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));

  /* Get the TIMx CCMR1 register value [ȡ��TIMx ����/�Ƚ�ģʽ�Ĵ���1�ļĴ���ֵ]*/
  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC1FE Bit [��λOC1PE]*/
  tmpccmr1 &= CCMR_OC13FE_Reset;

  /* Enable or Disable the Output Compare Fast Bit */
  tmpccmr1 |= TIM_OCFast;

  /* Write to TIMx CCMR1 [дTIMx����/�Ƚ�ģʽ�Ĵ���1]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* ��������: TIM_OC2FastConfig
* ��������: ����TIMx����Ƚ�2��������.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM1_OCFast:����ȽϿ�������״̬.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCFast_Enable: TIM����ȽϿ�������ʹ��
*                       - TIM_OCFast_Disable: TIM����ȽϿ������ܽ���
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC2FastConfig(TIM_TypeDef* TIMx, u16 TIM_OCFast)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));

  /* Get the TIMx CCMR1 register value [ȡ��TIMx ����/�Ƚ�ģʽ�Ĵ���1�ļĴ���ֵ]*/
  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC2FE Bit [��λOC2PE]*/
  tmpccmr1 &= CCMR_OC24FE_Reset;

  /* Enable or Disable the Output Compare Fast Bit [ʹ�ܻ��ֹ��������Ƚ�λ]*/
  tmpccmr1 |= (u16)(TIM_OCFast << 8);

  /* Write to TIMx CCMR1 [дTIMx����/�Ƚ�ģʽ�Ĵ���1]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* ��������: TIM_OC3FastConfig
* ��������: ����TIMx����Ƚ�3��������.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM1_OCFast:����ȽϿ�������״̬.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCFast_Enable: TIM����ȽϿ�������ʹ��
*                       - TIM_OCFast_Disable: TIM����ȽϿ������ܽ���
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC3FastConfig(TIM_TypeDef* TIMx, u16 TIM_OCFast)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));

  /* Get the TIMx CCMR2 register value [ȡ��TIMx CCMR2�Ĵ���ֵ]*/
  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC3FE Bit [��λOC3PE]*/
  tmpccmr2 &= CCMR_OC13FE_Reset;

  /* Enable or Disable the Output Compare Fast Bit [ʹ�ܻ��ֹ��������Ƚ�λ]*/
  tmpccmr2 |= TIM_OCFast;

  /* Write to TIMx CCMR2 [дTIMx CCMR2]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* ��������: TIM_OC4FastConfig
* ��������: ����TIMx����Ƚ�4��������.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM1_OCFast:����ȽϿ�������״̬.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCFast_Enable: TIM����ȽϿ�������ʹ��
*                       - TIM_OCFast_Disable: TIM����ȽϿ������ܽ���
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC4FastConfig(TIM_TypeDef* TIMx, u16 TIM_OCFast)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));

  /* Get the TIMx CCMR2 register value [ȡ��TIMx CCMR2�Ĵ���ֵ]*/
  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC4FE Bit [��λOC4PE]*/
  tmpccmr2 &= CCMR_OC24FE_Reset;

  /* Enable or Disable the Output Compare Fast Bit [ʹ�ܻ��ֹ��������Ƚ�λ]*/
  tmpccmr2 |= (u16)(TIM_OCFast << 8);

  /* Write to TIMx CCMR2 [дTIMx CCMR2]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* ��������: TIM_ClearOC1Ref
* ��������: �����ά�����ⲿ�¼��е�OCREF1�ź�
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM1_OCClear:����Ƚ����ʹ��λ����״̬.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCClear_Enable: TIM����Ƚ����ʹ��
*                       - TIM_OCClear_Disable: ����Ƚ��������
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ClearOC1Ref(TIM_TypeDef* TIMx, u16 TIM_OCClear)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC1CE Bit [��λOC1CE]*/
  tmpccmr1 &= CCMR_OC13CE_Reset;

  /* Enable or Disable the Output Compare Clear Bit [ʹ�ܻ��ֹ��������Ƚ�λ]*/
  tmpccmr1 |= TIM_OCClear;

  /* Write to TIMx CCMR1 register [дTIMx CCMR1�Ĵ���]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* ��������: TIM_ClearOC2Ref
* ��������: �����ά�����ⲿ�¼��е�OCREF2�ź�
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM1_OCClear:����Ƚ����ʹ��λ����״̬.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCClear_Enable: TIM����Ƚ����ʹ��
*                       - TIM_OCClear_Disable: ����Ƚ��������
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ClearOC2Ref(TIM_TypeDef* TIMx, u16 TIM_OCClear)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC2CE Bit [��λOC2CE]*/
  tmpccmr1 &= CCMR_OC24CE_Reset;

  /* Enable or Disable the Output Compare Clear Bit [ʹ�ܻ��ֹ��������Ƚ�λ]*/
  tmpccmr1 |= (u16)(TIM_OCClear << 8);

  /* Write to TIMx CCMR1 register [дTIMx CCMR1�Ĵ���]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* ��������: TIM_ClearOC3Ref
* ��������: �����ά�����ⲿ�¼��е�OCREF3�ź�
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM1_OCClear:����Ƚ����ʹ��λ����״̬.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCClear_Enable: TIM����Ƚ����ʹ��
*                       - TIM_OCClear_Disable: ����Ƚ��������
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ClearOC3Ref(TIM_TypeDef* TIMx, u16 TIM_OCClear)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC3CE Bit [��λOC3CE]*/
  tmpccmr2 &= CCMR_OC13CE_Reset;

  /* Enable or Disable the Output Compare Clear Bit [ʹ�ܻ��ֹ��������Ƚ�λ]*/
  tmpccmr2 |= TIM_OCClear;

  /* Write to TIMx CCMR2 register [дTIMx CCMR2�Ĵ���]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* ��������: TIM_ClearOC4Ref
* ��������: �����ά�����ⲿ�¼��е�OCREF4�ź�
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM1_OCClear:����Ƚ����ʹ��λ����״̬.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCClear_Enable:����Ƚ����ʹ��
*                       - TIM_OCClear_Disable:����Ƚ��������
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ClearOC4Ref(TIM_TypeDef* TIMx, u16 TIM_OCClear)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC4CE Bit [��λOC4CE]*/
  tmpccmr2 &= CCMR_OC24CE_Reset;

  /* Enable or Disable the Output Compare Clear Bit [ʹ�ܻ��ֹ��������Ƚ�λ]*/
  tmpccmr2 |= (u16)(TIM_OCClear << 8);

  /* Write to TIMx CCMR2 register [дTIMx CCMR2�Ĵ���]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* ��������: TIM_OC1PolarityConfig
* ��������: ����TIMͨ��1����.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_OCPolarity: ָ���� OC1 ����
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCPolarity_High: ������Ը�
*                       - TIM_OCPolarity_Low: ������Ե�
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC1PolarityConfig(TIM_TypeDef* TIMx, u16 TIM_OCPolarity)
{
  u16 tmpccer = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));

  tmpccer = TIMx->CCER;

  /* Set or Reset the CC1P Bit [��λ��λCC1P]*/
  tmpccer &= CCER_CC1P_Reset;
  tmpccer |= TIM_OCPolarity;

  /* Write to TIMx CCER register [дTIMx CCER�Ĵ���]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TIM_OC1NPolarityConfig
* ��������: ����TIMͨ��1N����.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_OCPolarity: ָ���� OC1N ����
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCNPolarity_High: ������Ը�
*                       - TIM_OCNPolarity_Low: ������Ե�
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC1NPolarityConfig(TIM_TypeDef* TIMx, u16 TIM_OCNPolarity)
{
  u16 tmpccer = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
   
  tmpccer = TIMx->CCER;

  /* Set or Reset the CC1NP Bit [��λ��λCC1NP]*/
  tmpccer &= CCER_CC1NP_Reset;
  tmpccer |= TIM_OCNPolarity;

  /* Write to TIMx CCER register [дTIMx CCER�Ĵ���]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TIM_OC2PolarityConfig
* ��������: ����TIMͨ��2����
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_OCPolarity: ָ���� OC2 ����
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCPolarity_High: ������Ը�
*                       - TIM_OCPolarity_Low: ������Ե�
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC2PolarityConfig(TIM_TypeDef* TIMx, u16 TIM_OCPolarity)
{
  u16 tmpccer = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));

  tmpccer = TIMx->CCER;

  /* Set or Reset the CC2P Bit [��λ��λCC2P]*/
  tmpccer &= CCER_CC2P_Reset;
  tmpccer |= (u16)(TIM_OCPolarity << 4);

  /* Write to TIMx CCER register [дTIMx CCER�Ĵ���]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TIM_OC2NPolarityConfig
* ��������: ����TIMͨ��2N����
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)TIM_OCPolarity: ָ���� OC2N����
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCNPolarity_High: ������Ը�
*                       - TIM_OCNPolarity_Low: ������Ե�
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC2NPolarityConfig(TIM_TypeDef* TIMx, u16 TIM_OCNPolarity)
{
  u16 tmpccer = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
  
  tmpccer = TIMx->CCER;

  /* Set or Reset the CC2NP Bit [��λ��λCC2NP]*/
  tmpccer &= CCER_CC2NP_Reset;
  tmpccer |= (u16)(TIM_OCNPolarity << 4);

  /* Write to TIMx CCER register [дTIMx CCER�Ĵ���]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TIM_OC3PolarityConfig
* ��������: ����TIMͨ��3����
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_OCPolarity: ָ���� OC3 ����
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCPolarity_High: ������Ը�
*                       - TIM_OCPolarity_Low: ������Ե�
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC3PolarityConfig(TIM_TypeDef* TIMx, u16 TIM_OCPolarity)
{
  u16 tmpccer = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));

  tmpccer = TIMx->CCER;

  /* Set or Reset the CC3P Bit [��λ��λCC3P]*/
  tmpccer &= CCER_CC3P_Reset;
  tmpccer |= (u16)(TIM_OCPolarity << 8);

  /* Write to TIMx CCER register [дTIMx CCER�Ĵ���]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TIM_OC3NPolarityConfig
* ��������: ����TIMͨ��3N����
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)TIM_OCPolarity: ָ���� OC3N ����
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCNPolarity_High: ������Ը�
*                       - TIM_OCNPolarity_Low: ������Ե�
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC3NPolarityConfig(TIM_TypeDef* TIMx, u16 TIM_OCNPolarity)
{
  u16 tmpccer = 0;
 
  /* Check the parameters [������]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
    
  tmpccer = TIMx->CCER;

  /* Set or Reset the CC3NP Bit [��λ��λCC3NP]*/
  tmpccer &= CCER_CC3NP_Reset;
  tmpccer |= (u16)(TIM_OCNPolarity << 8);

  /* Write to TIMx CCER register [дTIMx CCER�Ĵ���]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TIM_OC4PolarityConfig
* ��������: ����TIMͨ��1����
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_OCPolarity: ָ���� OC4 ����
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCPolarity_High: ������Ը�
*                       - TIM_OCPolarity_Low: ������Ե�
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_OC4PolarityConfig(TIM_TypeDef* TIMx, u16 TIM_OCPolarity)
{
  u16 tmpccer = 0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));

  tmpccer = TIMx->CCER;

  /* Set or Reset the CC4P Bit [��λ��λCC4P]*/
  tmpccer &= CCER_CC4P_Reset;
  tmpccer |= (u16)(TIM_OCPolarity << 12);

  /* Write to TIMx CCER register [дTIMx CCER�Ĵ���]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TIM_CCxCmd
* ��������: ʹ�ܻ����TIM����Ƚ�ͨ��x
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_Channel: ָ����TIMͨ��
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_Channel_1: TIMͨ��1
*                       - TIM_Channel_2: TIMͨ��2
*                       - TIM_Channel_3: TIMͨ��3
*                       - TIM_Channel_4: TIMͨ��4
*           (3)Newstate��ָ��TIM1ͨ��CCxEλ��״̬.��������ܹ�ȡ��ENABLE����DISABLE 
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_CCxCmd(TIM_TypeDef* TIMx, u16 TIM_Channel, u16 TIM_CCx)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_CCX(TIM_CCx));

  /* Reset the CCxE Bit [��λCCxE]*/
  TIMx->CCER &= (u16)(~((u16)(CCER_CCE_Set << TIM_Channel)));

  /* Set or reset the CCxE Bit [��λ��λCCxE]*/ 
  TIMx->CCER |=  (u16)(TIM_CCx << TIM_Channel);
}

/*******************************************************************************
* ��������: TIM_CCxNCmd
* ��������: ʹ�ܻ����TIM����Ƚ�ͨ��xN
* �������:(1)TIMx:�˴�x������1��8��ѡ������.
*          (2)TIM_Channel: ָ����TIMͨ��
*                    �����������ȡ�����ֵ֮һ::
*                       - TIM_Channel_1: TIMͨ��1
*                       - TIM_Channel_2: TIMͨ��2
*                       - TIM_Channel_3: TIMͨ��3
*           (3)Newstate��ָ��TIM1ͨ��CCxNEλ��״̬.��������ܹ�ȡ��ENABLE����DISABLE 
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_CCxNCmd(TIM_TypeDef* TIMx, u16 TIM_Channel, u16 TIM_CCxN)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_COMPLEMENTARY_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_CCXN(TIM_CCxN));

  /* Reset the CCxNE Bit [��λCCxNE]*/
  TIMx->CCER &= (u16)(~((u16)(CCER_CCNE_Set << TIM_Channel)));

  /* Set or reset the CCxNE Bit [��λ��λCCxNE]*/ 
  TIMx->CCER |=  (u16)(TIM_CCxN << TIM_Channel);
}

/*******************************************************************************
* ��������: TIM_SelectOCxM
* ��������: ѡ��TIM����Ƚ�ģʽ��
*                  �ڸı�����Ƚ�ģʽ֮ǰ���ú�������ѡ���ͨ�����û���Ҫʹ�� 
*                  TIM1_CCxCmd��TIM1_CCxNCmd����ʹ��ͨ����
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_Channel: ָ����TIMͨ��
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_Channel_1: TIMͨ��1
*                       - TIM_Channel_2: TIMͨ��2
*                       - TIM_Channel_3: TIMͨ��3
*                       - TIM_Channel_4: TIMͨ��4
*           (2)TIM_OCMode: ָ����TIM����Ƚ�ģʽ.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OCMode_Timing
*                       - TIM_OCMode_Active
*                       - TIM_OCMode_Toggle
*                       - TIM_OCMode_PWM1
*                       - TIM_OCMode_PWM2
*                       - TIM_ForcedAction_Active
*                       - TIM_ForcedAction_InActive
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SelectOCxM(TIM_TypeDef* TIMx, u16 TIM_Channel, u16 TIM_OCMode)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_OCM(TIM_OCMode));
  
  /* Disable the Channel: Reset the CCxE Bit [��ֹͨ������λCCxE]*/
  TIMx->CCER &= (u16)(~((u16)(CCER_CCE_Set << TIM_Channel)));

  if((TIM_Channel == TIM_Channel_1) ||(TIM_Channel == TIM_Channel_3))
  {
    /* Reset the OCxM bits in the CCMRx register [��λCCMRx�Ĵ����е�OCxM]*/
    *((vu32 *)((*(u32*)&TIMx) + CCMR_Offset + (TIM_Channel>>1))) &= CCMR_OC13M_Mask;
   
    /* Configure the OCxM bits in the CCMRx register [����CCMRx�Ĵ����е�OCxM]*/
    *((vu32 *)((*(u32*)&TIMx) + CCMR_Offset + (TIM_Channel>>1))) = TIM_OCMode;

  }
  else
  {
    /* Reset the OCxM bits in the CCMRx register [��λCCMRx�Ĵ����е�OCxM]*/
    *((vu32 *)((*(u32*)&TIMx) + CCMR_Offset + ((u16)(TIM_Channel - 4)>> 1))) &= CCMR_OC24M_Mask;
    
    /* Configure the OCxM bits in the CCMRx register [����CCMRx�Ĵ����е�OCxM]*/
    *((vu32 *)((*(u32*)&TIMx) + CCMR_Offset + ((u16)(TIM_Channel - 4)>> 1))) = (u16)(TIM_OCMode << 8);
  }
}

/*******************************************************************************
* ��������: TIM_UpdateDisableConfig
* ��������: ʹ�ܻ�ر�TIM�����¼�
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)NewState: TIMx_CR1�Ĵ�����UDISλ����״̬.�������������ENABLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_UpdateDisableConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the Update Disable Bit [��λ���½�ֹλ]*/
    TIMx->CR1 |= CR1_UDIS_Set;
  }
  else
  {
    /* Reset the Update Disable Bit [��λ���½�ֹλ]*/
    TIMx->CR1 &= CR1_UDIS_Reset;
  }
}

/*******************************************************************************
* ��������: TIM_UpdateRequestConfig
* ��������: ����ĳ��TIM�ĸ��������ж�Դ.
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)TIM_UpdateSource:��������Դ.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_UpdateSource_Regular
*                       - TIM_UpdateSource_Global
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_UpdateRequestConfig(TIM_TypeDef* TIMx, u16 TIM_UpdateSource)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_UPDATE_SOURCE(TIM_UpdateSource));

  if (TIM_UpdateSource != TIM_UpdateSource_Global)
  {
    /* Set the URS Bit [��λURS]*/
    TIMx->CR1 |= CR1_URS_Set;
  }
  else
  {
    /* Reset the URS Bit [��λURS]*/
    TIMx->CR1 &= CR1_URS_Reset;
  }
}

/*******************************************************************************
* ��������: TIM_SelectHallSensor
* ��������: ʹ�ܻ�ر�ĳ��TIMx�Ļ�����Ӧ���ӿ�.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)NewState:������Ӧ������״̬.�������������ENABLE��DISABLE��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SelectHallSensor(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the TI1S Bit [��λTI1S]*/
    TIMx->CR2 |= CR2_TI1S_Set;
  }
  else
  {
    /* Reset the TI1S Bit [��λTI1S]*/
    TIMx->CR2 &= CR2_TI1S_Reset;
  }
}

/*******************************************************************************
* ��������: TIM_SelectOnePulseMode
* ��������: ѡ��ĳ��TIMxΪ������ģʽ
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)TIM_OPMode:ָ��ʹ�õ�����ģʽ.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_OPMode_Single
*                       - TIM_OPMode_Repetitive
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SelectOnePulseMode(TIM_TypeDef* TIMx, u16 TIM_OPMode)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OPM_MODE(TIM_OPMode));

  /* Reset the OPM Bit [��λOPM]*/
  TIMx->CR1 &= CR1_OPM_Reset;

  /* Configure the OPM Mode [����OPMģʽ]*/
  TIMx->CR1 |= TIM_OPMode;
}

/*******************************************************************************
* ��������: TIM_SelectOutputTrigger
* ��������: ѡ��ĳ��TIMx�Ĵ��������ģʽ
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)TIM_TRGOSource:���������Դ.
*                    ������������������ֵ:
*                      1/ For TIM1 to TIM8:
*                       - TIM_TRGOSource_Reset 
*                       - TIM_TRGOSource_Enable
*                       - TIM_TRGOSource_Update
*                      2/ These parameters are available for all TIMx except 
*                         TIM6 and TIM7:
*                       - TIM_TRGOSource_OC1
*                       - TIM_TRGOSource_OC1Ref
*                       - TIM_TRGOSource_OC2Ref
*                       - TIM_TRGOSource_OC3Ref
*                       - TIM_TRGOSource_OC4Ref
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx, u16 TIM_TRGOSource)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_TRGO_SOURCE(TIM_TRGOSource));
  assert_param(IS_TIM_PERIPH_TRGO(TIMx, TIM_TRGOSource));

  /* Reset the MMS Bits [��λMMS]*/
  TIMx->CR2 &= CR2_MMS_Mask;

  /* Select the TRGO source [ѡ��TRGOԴ]*/
  TIMx->CR2 |=  TIM_TRGOSource;
}

/*******************************************************************************
* ��������: TIM_SelectSlaveMode
* ��������: ѡ��ĳ��TIMxΪ��ģʽ.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_SlaveMode:ָ����TIM��ģʽ.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_SlaveMode_Reset
*                       - TIM_SlaveMode_Gated
*                       - TIM_SlaveMode_Trigger
*                       - TIM_SlaveMode_External1
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, u16 TIM_SlaveMode)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_SLAVE_MODE(TIM_SlaveMode));

  /* Reset the SMS Bits [��λSMS]*/
  TIMx->SMCR &= SMCR_SMS_Mask;

  /* Select the Slave Mode [ѡ���ģʽ]*/
  TIMx->SMCR |= TIM_SlaveMode;
}

/*******************************************************************************
* ��������: TIM_SelectMasterSlaveMode
* ��������: ����/����TIMΪ��/��ģʽ
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_MasterSlaveMode:��ʱ������ģʽ.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_MasterSlaveMode_Enable: ʹ������ģʽ.
*                       - TIM_MasterSlaveMode_Disable: û������
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SelectMasterSlaveMode(TIM_TypeDef* TIMx, u16 TIM_MasterSlaveMode)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_MSM_STATE(TIM_MasterSlaveMode));

  /* Reset the MSM Bit [��λMSM]*/
  TIMx->SMCR &= SMCR_MSM_Reset;
  
  /* Set or Reset the MSM Bit [��λ��λMSM]*/
  TIMx->SMCR |= TIM_MasterSlaveMode;
}

/*******************************************************************************
* ��������: TIM_SetCounter
* ��������: ����ĳ��TIM�������Ĵ�����ֵ
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)Counter:�������Ĵ�������ֵ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SetCounter(TIM_TypeDef* TIMx, u16 Counter)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));

  /* Set the Counter Register value [���ü������Ĵ���ֵ]*/
  TIMx->CNT = Counter;
}

/*******************************************************************************
* ��������: TIM_SetAutoreload
* ��������: ����ĳ��TIMx�Զ����ؼĴ�����ֵ
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)Autoreload:�Զ����ؼĴ�������ֵ.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SetAutoreload(TIM_TypeDef* TIMx, u16 Autoreload)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));

  /* Set the Autoreload Register value [�����Զ�װ�ؼĴ���ֵ]*/
  TIMx->ARR = Autoreload;
}

/*******************************************************************************
* ��������: TIM_SetCompare1
* ��������: ����ĳ��TIMx����Ƚ�1�Ĵ�����ֵ
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)Compare1: ����Ƚ�1�Ĵ�������ֵ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SetCompare1(TIM_TypeDef* TIMx, u16 Compare1)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Set the Capture Compare1 Register value [���ò���Ƚ�1�Ĵ�����ֵ]*/
  TIMx->CCR1 = Compare1;
}

/*******************************************************************************
* ��������: TIM_SetCompare2
* ��������: ����ĳ��TIMx����Ƚ�2�Ĵ�����ֵ
* �������:  (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*            (2)Compare2: ����Ƚ�2�Ĵ�������ֵ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SetCompare2(TIM_TypeDef* TIMx, u16 Compare2)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Set the Capture Compare2 Register value [���ò���Ƚ�2�Ĵ�����ֵ]*/
  TIMx->CCR2 = Compare2;
}

/*******************************************************************************
* ��������: TIM_SetCompare3
* ��������: ����ĳ��TIMx����Ƚ�3�Ĵ�����ֵ
* �������:  (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*            (2)Compare3: ����Ƚ�3�Ĵ�������ֵ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SetCompare3(TIM_TypeDef* TIMx, u16 Compare3)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Set the Capture Compare3 Register value [���ò���Ƚ�3�Ĵ�����ֵ]*/
  TIMx->CCR3 = Compare3;
}

/*******************************************************************************
* ��������: TIM_SetCompare4
* ��������: ����ĳ��TIMx����Ƚ�4�Ĵ�����ֵ
* �������:  (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*            (2)Compare4:����Ƚ�4�Ĵ�������ֵ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SetCompare4(TIM_TypeDef* TIMx, u16 Compare4)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Set the Capture Compare4 Register value [���ò���Ƚ�4�Ĵ�����ֵ]*/
  TIMx->CCR4 = Compare4;
}

/*******************************************************************************
* ��������: TIM_SetIC1Prescaler
* ��������: ����ĳ��TIMx���벶��1��Ԥ��Ƶ��
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_ICPSC:���벶��1��Ԥ��Ƶ��.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ICPSC_DIV1: û��Ԥ��Ƶ��
*                       - TIM_ICPSC_DIV2: ÿ2���¼�TIM����ִ��һ��
*                       - TIM_ICPSC_DIV4: ÿ4���¼�TIM����ִ��һ��
*                       - TIM_ICPSC_DIV8: ÿ8���¼�TIM����ִ��һ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SetIC1Prescaler(TIM_TypeDef* TIMx, u16 TIM_ICPSC)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));

  /* Reset the IC1PSC Bits [��λIC1PSC]*/
  TIMx->CCMR1 &= CCMR_IC13PSC_Mask;

  /* Set the IC1PSC value [����IC1PSCֵ]*/
  TIMx->CCMR1 |= TIM_ICPSC;
}

/*******************************************************************************
* ��������: TIM_SetIC2Prescaler
* ��������: ����ĳ��TIMx���벶��2��Ԥ��Ƶ��
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_ICPSC:���벶��2��Ԥ��Ƶ��.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ICPSC_DIV1: û��Ԥ��Ƶ��
*                       - TIM_ICPSC_DIV2: ÿ2���¼�TIM����ִ��һ��
*                       - TIM_ICPSC_DIV4: ÿ4���¼�TIM����ִ��һ��
*                       - TIM_ICPSC_DIV8: ÿ8���¼�TIM����ִ��һ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SetIC2Prescaler(TIM_TypeDef* TIMx, u16 TIM_ICPSC)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));

  /* Reset the IC2PSC Bits [��λIC2PSC]*/
  TIMx->CCMR1 &= CCMR_IC24PSC_Mask;

  /* Set the IC2PSC value [����IC2PSCֵ]*/
  TIMx->CCMR1 |= (u16)(TIM_ICPSC << 8);
}

/*******************************************************************************
* ��������: TIM_SetIC3Prescaler
* ��������: ����ĳ��TIMx���벶��3��Ԥ��Ƶ��
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_ICPSC:���벶��3��Ԥ��Ƶ��.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ICPSC_DIV1: û��Ԥ��Ƶ��
*                       - TIM_ICPSC_DIV2: ÿ2���¼�TIM����ִ��һ��
*                       - TIM_ICPSC_DIV4: ÿ4���¼�TIM����ִ��һ��
*                       - TIM_ICPSC_DIV8: ÿ8���¼�TIM����ִ��һ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SetIC3Prescaler(TIM_TypeDef* TIMx, u16 TIM_ICPSC)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));

  /* Reset the IC3PSC Bits [��λIC3PSC]*/
  TIMx->CCMR2 &= CCMR_IC13PSC_Mask;

  /* Set the IC3PSC value [����IC3PSCֵ]*/
  TIMx->CCMR2 |= TIM_ICPSC;
}

/*******************************************************************************
* ��������: TIM_SetIC4Prescaler
* ��������: ����ĳ��TIMx���벶��4��Ԥ��Ƶ��
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_ICPSC:���벶��4��Ԥ��Ƶ��.
*                    �����������ȡ�����ֵ֮һ:
*                      - TIM_ICPSC_DIV1: û��Ԥ��Ƶ��
*                      - TIM_ICPSC_DIV2: ÿ2���¼�TIM����ִ��һ��
*                      - TIM_ICPSC_DIV4: ÿ4���¼�TIM����ִ��һ��
*                      - TIM_ICPSC_DIV8: ÿ8���¼�TIM����ִ��һ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SetIC4Prescaler(TIM_TypeDef* TIMx, u16 TIM_ICPSC)
{  
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));

  /* Reset the IC4PSC Bits [��λIC4PSC]*/
  TIMx->CCMR2 &= CCMR_IC24PSC_Mask;

  /* Set the IC4PSC value [����IC4PSCֵ]*/
  TIMx->CCMR2 |= (u16)(TIM_ICPSC << 8);
}

/*******************************************************************************
* ��������: TIM_SetClockDivision
* ��������: ����ĳ��TIMxʱ�ӷָ�ֵ
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_CKD:ʱ�ӷָ�ֵ.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_CKD_DIV1: TDTS = Tck_tim
*                       - TIM_CKD_DIV2: TDTS = 2*Tck_tim
*                       - TIM_CKD_DIV4: TDTS = 4*Tck_tim
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_SetClockDivision(TIM_TypeDef* TIMx, u16 TIM_CKD)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CKD_DIV(TIM_CKD));

  /* Reset the CKD Bits [��λCKD]*/
  TIMx->CR1 &= CR1_CKD_Mask;

  /* Set the CKD value [����CKDֵ]*/
  TIMx->CR1 |= TIM_CKD;
}
/*******************************************************************************
* ��������: TIM_GetCapture1
* ��������: ��ȡĳ��TIMx�����벶��1��ֵ
* �������: TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
* �������: ��
* ���ز���: �Ƚϲ���Ĵ���1��ֵ.
*******************************************************************************/
u16 TIM_GetCapture1(TIM_TypeDef* TIMx)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Get the Capture 1 Register value [ȡ�ò���Ĵ���1��ֵ]*/
  return TIMx->CCR1;
}

/*******************************************************************************
* ��������: TIM_GetCapture2
* ��������: ��ȡĳ��TIMx�����벶��2��ֵ
* �������: TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
* �������: ��
* ���ز���: �Ƚϲ���Ĵ���2��ֵ��
*******************************************************************************/
u16 TIM_GetCapture2(TIM_TypeDef* TIMx)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Get the Capture 2 Register value [ȡ�ò���Ĵ���2��ֵ]*/
  return TIMx->CCR2;
}

/*******************************************************************************
* ��������: TIM_GetCapture3
* ��������: ��ȡĳ��TIMx�����벶��3��ֵ
* �������: TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
* �������: ��
* ���ز���: �Ƚϲ���Ĵ���3��ֵ��
*******************************************************************************/
u16 TIM_GetCapture3(TIM_TypeDef* TIMx)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 

  /* Get the Capture 3 Register value [ȡ�ò���Ĵ���3��ֵ]*/
  return TIMx->CCR3;
}

/*******************************************************************************
* ��������: TIM_GetCapture4
* ��������: ��ȡĳ��TIMx�����벶��4��ֵ
* �������: TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
* �������: ��
* ���ز���: �Ƚϲ���Ĵ���4��ֵ��
*******************************************************************************/
u16 TIM_GetCapture4(TIM_TypeDef* TIMx)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Get the Capture 4 Register value [ȡ�ò���Ĵ���4��ֵ]*/
  return TIMx->CCR4;
}

/*******************************************************************************
* ��������: TIM_GetCounter
* ��������: ȡ��TIMx�����Ĵ�����ֵ.
* �������: TIMx:�˴�x������1��8��ѡ������.
* �������: ��
* ���ز���: �����Ĵ�����ֵ.
*******************************************************************************/
u16 TIM_GetCounter(TIM_TypeDef* TIMx)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));

  /* Get the Counter Register value [ȡ�ü����Ĵ�����ֵ]*/
  return TIMx->CNT;
}

/*******************************************************************************
* ��������: TIM_GetPrescaler
* ��������: ȡ��TIMxԤ��Ƶ��ֵ.
* �������: TIMx:�˴�x������1��8��ѡ������.
* �������: ��
* ���ز���: Ԥ��Ƶ�Ĵ�����ֵ.
*******************************************************************************/
u16 TIM_GetPrescaler(TIM_TypeDef* TIMx)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));

  /* Get the Prescaler Register value [ȡ��Ԥ��Ƶ�Ĵ�����ֵ]*/
  return TIMx->PSC;
}

/*******************************************************************************
* ��������: TIM_GetFlagStatus
* ��������: ����Ƿ��ض���TIM��Ǳ�����.
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)TIM_FLAG:���ڼ��ı��.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_FLAG_Update: TIM���±��
*                       - TIM_FLAG_CC1: TIM����Ƚ�1���
*                       - TIM_FLAG_CC2: TIM����Ƚ�2���
*                       - TIM_FLAG_CC3: TIM����Ƚ�3���
*                       - TIM_FLAG_CC4: TIM����Ƚ�4���
*                       - TIM_FLAG_COM: TIM�������
*                       - TIM_FLAG_Trigger: TIM���������
*                       - TIM_FLAG_Break: TIMͣ�ٱ��
*                       - TIM_FLAG_CC1OF: TIM����Ƚ�1������
*                       - TIM_FLAG_CC2OF: TIM����Ƚ�2������
*                       - TIM_FLAG_CC3OF: TIM����Ƚ�3������
*                       - TIM_FLAG_CC4OF: TIM����Ƚ�4������
* �������: ��
* ���ز���: TIM_FLAG����״̬ (SET or RESET).
*******************************************************************************/
FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx, u16 TIM_FLAG)
{ 
  ITStatus bitstatus = RESET;  

  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_GET_FLAG(TIM_FLAG));
  assert_param(IS_TIM_PERIPH_FLAG(TIMx, TIM_FLAG));
  
  if ((TIMx->SR & TIM_FLAG) != (u16)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/*******************************************************************************
* ��������: TIM_ClearFlag
* ��������: ���TIMx�Ĺ�����
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)TIM_FLAG: ��������ı��.
*                    �����������������ֵ���������:
*                       - TIM_FLAG_Update: TIM���±��
*                       - TIM_FLAG_CC1: TIM����Ƚ�1���
*                       - TIM_FLAG_CC2: TIM����Ƚ�2���
*                       - TIM_FLAG_CC3: TIM����Ƚ�3���
*                       - TIM_FLAG_CC4: TIM����Ƚ�4���
*                       - TIM_FLAG_COM: TIM�������
*                       - TIM_FLAG_Trigger: TIM���������
*                       - TIM_FLAG_Break: TIMͣ�ٱ��
*                       - TIM_FLAG_CC1OF: TIM����Ƚ�1������
*                       - TIM_FLAG_CC2OF: TIM����Ƚ�2������
*                       - TIM_FLAG_CC3OF: TIM����Ƚ�3������
*                       - TIM_FLAG_CC4OF: TIM����Ƚ�4������
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ClearFlag(TIM_TypeDef* TIMx, u16 TIM_FLAG)
{  
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_CLEAR_FLAG(TIMx, TIM_FLAG));
   
  /* Clear the flags [�����־λ]*/
  TIMx->SR = (u16)~TIM_FLAG;
}

/*******************************************************************************
* ��������: TIM_GetITStatus
* ��������: ����ض�TIMx�ж��Ƿ���.
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)TIM_ IT:�涨Ҫ����TIM�ж�Դ.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_IT_Update: TIM�����ж�Դ
*                       - TIM_IT_CC1: TIM ����Ƚ�1�ж�Դ
*                       - TIM_IT_CC2: TIM ����Ƚ�2�ж�Դ
*                       - TIM_IT_CC3: TIM ����Ƚ�3�ж�Դ
*                       - TIM_IT_CC4: TIM ����Ƚ�4�ж�Դ
*                       - TIM_IT_COM: TIM �����ж�Դ
*                       - TIM_IT_Trigger: TIM�����ж�Դ
*                       - TIM_IT_Break: TIMͣ���ж�Դ
* �������: ��
* ���ز���: TIM_IT����״̬(SET or RESET).
*******************************************************************************/
ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, u16 TIM_IT)
{
  ITStatus bitstatus = RESET;  
  u16 itstatus = 0x0, itenable = 0x0;

  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_GET_IT(TIM_IT));
  assert_param(IS_TIM_PERIPH_IT(TIMx, TIM_IT));
   
  itstatus = TIMx->SR & TIM_IT;
  
  itenable = TIMx->DIER & TIM_IT;

  if ((itstatus != (u16)RESET) && (itenable != (u16)RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/*******************************************************************************
* ��������: TIM_ClearITPendingBit
* ��������: ���TIMx���жϹ���λ
* �������: (1)TIMx:�˴�x������1��8��ѡ������.
*           (2)TIM_ IT:ָ��Ҫ������жϹ���λ.
*                    �����������������ֵ���������:
*                       - TIM_IT_Update: TIM�����ж�Դ
*                       - TIM_IT_CC1: TIM ����Ƚ�1�ж�Դ
*                       - TIM_IT_CC2: TIM ����Ƚ�2�ж�Դ
*                       - TIM_IT_CC3: TIM ����Ƚ�3�ж�Դ
*                       - TIM_IT_CC4: TIM ����Ƚ�4�ж�Դ
*                       - TIM_IT_COM: TIM �����ж�Դ
*                       - TIM_IT_Trigger: TIM�����ж�Դ
*                       - TIM_IT_Break: TIMͣ���ж�Դ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, u16 TIM_IT)
{
  /* Check the parameters [������]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_PERIPH_IT(TIMx, TIM_IT));

  /* Clear the IT pending Bit [���IT����λ]*/
  TIMx->SR = (u16)~TIM_IT;
}

/*******************************************************************************
* ��������: TI1_Config
* ��������: ����TI1��Ϊ����.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_ICPolarity : ���뼫��.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ICPolarity_Rising
*                       - TIM_ICPolarity_Falling
*           (3)TIM_ICSelection: ʹ��ָ��������.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ICSelection_DirectTI: ѡ��TIM ����1����IC1.
*                       - TIM_ICSelection_IndirectTI: ѡ��TIM ����1����IC2.
*                       - TIM_ICSelection_TRC: ѡ��TIM ����1����TRC.
*           (4)TIM_ICFilter: ָ�������벶���˲���.
*                    ���������ֵ������ 0x00 �� 0x0F֮��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
static void TI1_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter)
{
  u16 tmpccmr1 = 0, tmpccer = 0;

  /* Disable the Channel 1: Reset the CC1E Bit [��ֹͨ��1����λCC1Eλ]*/
  TIMx->CCER &= CCER_CC1E_Reset;

  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;

  /* Select the Input and set the filter [ѡ��������˲���]*/
  tmpccmr1 &= CCMR_CC13S_Mask & CCMR_IC13F_Mask;
  tmpccmr1 |= TIM_ICSelection | (u16)(TIM_ICFilter << 4);

  /* Select the Polarity and set the CC1E Bit [ѡ���Ժ���λCC1E]*/
  tmpccer &= CCER_CC1P_Reset;
  tmpccer |= TIM_ICPolarity | CCER_CC1E_Set;

  /* Write to TIMx CCMR1 and CCER registers [дTIMx CCMR1��CCER�Ĵ���]*/
  TIMx->CCMR1 = tmpccmr1;
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TI2_Config
* ��������: Configure the TI2 as Input.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_ICPolarity : ���뼫��.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ICPolarity_Rising
*                       - TIM_ICPolarity_Falling
*           (3)TIM_ICSelection: ʹ��ָ��������.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ICSelection_DirectTI: ѡ��TIM ����2����IC2.
*                       - TIM_ICSelection_IndirectTI: ѡ��TIM ����2����IC1.
*                       - TIM_ICSelection_TRC: ѡ��TIM ����2����TRC.
*           (4)TIM_ICFilter: ָ�������벶���˲���.
*                    ���������ֵ������ 0x00 �� 0x0F֮��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
static void TI2_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter)
{
  u16 tmpccmr1 = 0, tmpccer = 0, tmp = 0;

  /* Disable the Channel 2: Reset the CC2E Bit [��ֹͨ��2����λCC2Eλ]*/
  TIMx->CCER &= CCER_CC2E_Reset;

  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;
  tmp = (u16)(TIM_ICPolarity << 4);

  /* Select the Input and set the filter [ѡ��������˲���]*/
  tmpccmr1 &= CCMR_CC24S_Mask & CCMR_IC24F_Mask;
  tmpccmr1 |= (u16)(TIM_ICFilter << 12);
  tmpccmr1 |= (u16)(TIM_ICSelection << 8);

  /* Select the Polarity and set the CC2E Bit [ѡ���Ժ���λCC2E]*/
  tmpccer &= CCER_CC2P_Reset;
  tmpccer |=  tmp | CCER_CC2E_Set;

  /* Write to TIMx CCMR1 and CCER registers [дTIMx CCMR1��CCER�Ĵ���]*/
  TIMx->CCMR1 = tmpccmr1 ;
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TI3_Config
* ��������: Configure the TI3 as Input.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_ICPolarity : ���뼫��.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ICPolarity_Rising
*                       - TIM_ICPolarity_Falling
*           (3)TIM_ICSelection: ʹ��ָ��������.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ICSelection_DirectTI: ѡ��TIM ����3����IC3.
*                       - TIM_ICSelection_IndirectTI: ѡ��TIM ����3����IC4.
*                       - TIM_ICSelection_TRC: ѡ��TIM ����3����TRC.
*           (4)TIM_ICFilter: ָ�������벶���˲���.
*                    ���������ֵ������ 0x00 �� 0x0F֮��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
static void TI3_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter)
{
  u16 tmpccmr2 = 0, tmpccer = 0, tmp = 0;

  /* Disable the Channel 3: Reset the CC3E Bit [��ֹͨ��3����λCC3Eλ]*/
  TIMx->CCER &= CCER_CC3E_Reset;

  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (u16)(TIM_ICPolarity << 8);

  /* Select the Input and set the filter [ѡ��������˲���]*/
  tmpccmr2 &= CCMR_CC13S_Mask & CCMR_IC13F_Mask;
  tmpccmr2 |= TIM_ICSelection | (u16)(TIM_ICFilter << 4);

  /* Select the Polarity and set the CC3E Bit [ѡ���Ժ���λCC3E]*/
  tmpccer &= CCER_CC3P_Reset;
  tmpccer |= tmp | CCER_CC3E_Set;

  /* Write to TIMx CCMR2 and CCER registers [дTIMx CCMR2��CCER�Ĵ���]*/
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* ��������: TI4_Config
* ��������: Configure the TI1 as Input.
* �������: (1)TIMx:�˴�x������1,2,3,4,5��8��ѡ������.
*           (2)TIM_ICPolarity : ���뼫��.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ICPolarity_Rising
*                       - TIM_ICPolarity_Falling
*           (3)TIM_ICSelection: ʹ��ָ��������.
*                    �����������ȡ�����ֵ֮һ:
*                       - TIM_ICSelection_DirectTI: ѡ��TIM ����4����IC4.
*                       - TIM_ICSelection_IndirectTI: ѡ��TIM ����4����IC3.
*                       - TIM_ICSelection_TRC: ѡ��TIM ����4����TRC.
*           (4)TIM_ICFilter: ָ�������벶���˲���.
*                    ���������ֵ������ 0x00 �� 0x0F֮��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
static void TI4_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter)
{
  u16 tmpccmr2 = 0, tmpccer = 0, tmp = 0;

  /* Disable the Channel 4: Reset the CC4E Bit [����ͨ��4����λCC4E]*/
  TIMx->CCER &= CCER_CC4E_Reset;

  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (u16)(TIM_ICPolarity << 12);

  /* Select the Input and set the filter [ѡ����������ù�����]*/
  tmpccmr2 &= CCMR_CC24S_Mask & CCMR_IC24F_Mask;
  tmpccmr2 |= (u16)(TIM_ICSelection << 8) | (u16)(TIM_ICFilter << 12);

  /* Select the Polarity and set the CC4E Bit [ѡ���Ժ���λCC4E]*/
  tmpccer &= CCER_CC4P_Reset;
  tmpccer |= tmp | CCER_CC4E_Set;

  /* Write to TIMx CCMR2 and CCER registers [дTIMx CCMR2��CCER�Ĵ���]*/
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer ;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
