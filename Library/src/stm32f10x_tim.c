/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_tim.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有TIM的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
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

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
static void TI1_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter);
static void TI2_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter);
static void TI3_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter);
static void TI4_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter);
/* 自用函数 ---------------------------------------------------------*/
/*******************************************************************************
* 函数名称: TIM_DeInit
* 功能描述: 重置TIMx外设寄存器为其默认复位值.
* 输入参数: TIMx:此处x可以是1到8以选择外设.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_DeInit(TIM_TypeDef* TIMx)
{
  /* Check the parameters [检查参数]*/
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
* 函数名称: TIM_TimeBaseInit
* 功能描述: 根据TIM_TimeBaseInitStruct中的特定参数初始化某个TIM的时间基单元.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_BaseInitStruct:指向一个包含TIM时间基配置信息的TIM_BaseInitTypeDef结构的指针..
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_COUNTER_MODE(TIM_TimeBaseInitStruct->TIM_CounterMode));
  assert_param(IS_TIM_CKD_DIV(TIM_TimeBaseInitStruct->TIM_ClockDivision));

  /* Select the Counter Mode and set the clock division [选择计数模式和设置时钟分频]*/
  TIMx->CR1 &= CR1_CKD_Mask & CR1_CounterMode_Mask;
  TIMx->CR1 |= (u32)TIM_TimeBaseInitStruct->TIM_ClockDivision |
                TIM_TimeBaseInitStruct->TIM_CounterMode;
  /* Set the Autoreload value [设置自动装载值]*/
  TIMx->ARR = TIM_TimeBaseInitStruct->TIM_Period ;

  /* Set the Prescaler value [设置预分频器值]*/
  TIMx->PSC = TIM_TimeBaseInitStruct->TIM_Prescaler;

  /* Generate an update event to reload the Prescaler value immediatly [产生一个更新事件立即重新装载预分频器值]*/
  TIMx->EGR = TIM_PSCReloadMode_Immediate;
    
  if (((*(u32*)&TIMx) == TIM1_BASE) || ((*(u32*)&TIMx) == TIM8_BASE))  
  {
    /* Set the Repetition Counter value [设置重复计数器值]*/
    TIMx->RCR = TIM_TimeBaseInitStruct->TIM_RepetitionCounter;
  }        
}

/*******************************************************************************
* 函数名称: TIM_OC1Init
* 功能描述: 根据TIM_OCInitStruct中的特定参数初始化TIMx通道1.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_OCInitStruct:指向一个包含特定TIMx外设的配置信息的TIM_OCInitTypeDef结构的指针.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  u16 tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   

  /* Disable the Channel 1: Reset the CC1E Bit [禁止通道1：复位CC1E位]*/
  TIMx->CCER &= CCER_CC1E_Reset;
  
  /* Get the TIMx CCER register value [取得TIMx 捕获/比较使能寄存器的寄存器值]*/
  tmpccer = TIMx->CCER;

  /* Get the TIMx CR2 register value [取得TIMx 控制2的寄存器值]*/
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR1 register value [取得TIMx 捕获/比较模式寄存器1的寄存器值]*/
  tmpccmrx = TIMx->CCMR1;
    
  /* Reset the Output Compare Mode Bits [复位输出比较模式位]*/
  tmpccmrx &= CCMR_OC13M_Mask;
  
  /* Select the Output Compare Mode [选择输出比较模式位]*/
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* Reset the Output Polarity level [复位输出极性级别]*/
  tmpccer &= CCER_CC1P_Reset;

  /* Set the Output Compare Polarity [设置输出比较极性]*/
  tmpccer |= TIM_OCInitStruct->TIM_OCPolarity;
  
  /* Set the Output State [设置输出状态]*/
  tmpccer |= TIM_OCInitStruct->TIM_OutputState;
  
  /* Set the Capture Compare Register value [设置捕获比较寄存器值]*/
  TIMx->CCR1 = TIM_OCInitStruct->TIM_Pulse;
  
  if((*(u32*)&TIMx == TIM1_BASE) || (*(u32*)&TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* Reset the Output N Polarity level [复位输出N极性等级]*/
    tmpccer &= CCER_CC1NP_Reset;

    /* Set the Output N Polarity [设置输出N极性]*/
    tmpccer |= TIM_OCInitStruct->TIM_OCNPolarity;

    /* Reset the Output N State [复位输出N状态]*/
    tmpccer &= CCER_CC1NE_Reset;
    
    /* Set the Output N State [设置输出N状态]*/
    tmpccer |= TIM_OCInitStruct->TIM_OutputNState;

    /* Reset the Ouput Compare and Output Compare N IDLE State [复位输出比较和输出比较N IDLE状态]*/
    tmpcr2 &= CR2_OIS1_Reset;
    tmpcr2 &= CR2_OIS1N_Reset;

    /* Set the Output Idle state [设置输出IDLE状态]*/
    tmpcr2 |= TIM_OCInitStruct->TIM_OCIdleState;

    /* Set the Output N Idle state [设置输出N IDLE状态]*/
    tmpcr2 |= TIM_OCInitStruct->TIM_OCNIdleState;
  }
  /* Write to TIMx CR2 [写TIMx配置寄存器2]*/
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR1 [写TIMx捕获/比较模式寄存器1]*/
  TIMx->CCMR1 = tmpccmrx;
  
  /* Write to TIMx CCER [写TIMx捕获/比较使能寄存器]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TIM_OC2Init
* 功能描述: 根据TIM_OCInitStruct中的特定参数初始化TIMx通道2。
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_OCInitStruct:指向一个包含特定TIMx外设的配置信息的TIM_OCInitTypeDef结构的指针
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  u16 tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   

  /* Disable the Channel 2: Reset the CC2E Bit [禁止通道2：复位CC2E位]*/
  TIMx->CCER &= CCER_CC2E_Reset;
  
  /* Get the TIMx CCER register value [取得TIMx 捕获/比较使能寄存器的寄存器值]*/  
  tmpccer = TIMx->CCER;

  /* Get the TIMx CR2 register value [取得TIMx 控制2的寄存器值]*/
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR1 register value [取得TIMx 捕获/比较模式寄存器1的寄存器值]*/
  tmpccmrx = TIMx->CCMR1;
    
  /* Reset the Output Compare Mode Bits [复位输出比较模式位]*/
  tmpccmrx &= CCMR_OC24M_Mask;
  
  /* Select the Output Compare Mode [选择输出比较模式位]*/
  tmpccmrx |= (u16)(TIM_OCInitStruct->TIM_OCMode << 8);
  
  /* Reset the Output Polarity level [复位输出极性级别]*/
  tmpccer &= CCER_CC2P_Reset;

  /* Set the Output Compare Polarity [设置输出比较极性]*/
  tmpccer |= (u16)(TIM_OCInitStruct->TIM_OCPolarity << 4);
  
  /* Set the Output State [设置输出状态]*/
  tmpccer |= (u16)(TIM_OCInitStruct->TIM_OutputState << 4);
  
  /* Set the Capture Compare Register value [设置捕获比较寄存器值]*/
  TIMx->CCR2 = TIM_OCInitStruct->TIM_Pulse;
  
  if((*(u32*)&TIMx == TIM1_BASE) || (*(u32*)&TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* Reset the Output N Polarity level [复位输出N极性等级]*/
    tmpccer &= CCER_CC2NP_Reset;

    /* Set the Output N Polarity [设置输出N极性]*/
    tmpccer |= (u16)(TIM_OCInitStruct->TIM_OCNPolarity << 4);

    /* Reset the Output N State [复位输出N状态]*/
    tmpccer &= CCER_CC2NE_Reset;
    
    /* Set the Output N State [设置输出N状态]*/
    tmpccer |= (u16)(TIM_OCInitStruct->TIM_OutputNState << 4);

    /* Reset the Ouput Compare and Output Compare N IDLE State [复位输出比较和输出比较N IDLE状态]*/
    tmpcr2 &= CR2_OIS2_Reset;
    tmpcr2 &= CR2_OIS2N_Reset;

    /* Set the Output Idle state [设置输出IDLE状态]*/
    tmpcr2 |= (u16)(TIM_OCInitStruct->TIM_OCIdleState << 2);

    /* Set the Output N Idle state [设置输出N IDLE状态]*/
    tmpcr2 |= (u16)(TIM_OCInitStruct->TIM_OCNIdleState << 2);
  }

  /* Write to TIMx CR2 [写TIMx配置寄存器2]*/
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR1 [写TIMx捕获/比较模式寄存器1]*/
  TIMx->CCMR1 = tmpccmrx;
  
  /* Write to TIMx CCER [写TIMx捕获/比较使能寄存器]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TIM_OC3Init
* 功能描述: 根据TIM_OCInitStruct中的特定参数初始化TIMx通道3。
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_OCInitStruct:指向一个包含特定TIMx外设的配置信息的TIM_OCInitTypeDef结构的指针
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC3Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  u16 tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   

  /* Disable the Channel 3: Reset the CC3E Bit [禁止通道3：复位CC3E位]*/
  TIMx->CCER &= CCER_CC3E_Reset;
  
  /* Get the TIMx CCER register value [取得TIMx 捕获/比较使能寄存器的寄存器值]*/
  tmpccer = TIMx->CCER;

  /* Get the TIMx CR2 register value [取得TIMx 控制2的寄存器值]*/
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR2 register value [取得TIMx 捕获/比较模式寄存器2的寄存器值]*/
  tmpccmrx = TIMx->CCMR2;
    
  /* Reset the Output Compare Mode Bits [复位输出比较模式位]*/
  tmpccmrx &= CCMR_OC13M_Mask;
  
  /* Select the Output Compare Mode [选择输出比较模式位]*/
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* Reset the Output Polarity level [复位输出极性级别]*/
  tmpccer &= CCER_CC3P_Reset;

  /* Set the Output Compare Polarity [设置输出比较极性]*/
  tmpccer |= (u16)(TIM_OCInitStruct->TIM_OCPolarity << 8);
  
  /* Set the Output State [设置输出状态]*/
  tmpccer |= (u16)(TIM_OCInitStruct->TIM_OutputState << 8);
  
  /* Set the Capture Compare Register value [设置捕获比较寄存器值]*/
  TIMx->CCR3 = TIM_OCInitStruct->TIM_Pulse;
  
  if((*(u32*)&TIMx == TIM1_BASE) || (*(u32*)&TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* Reset the Output N Polarity level [复位输出N极性等级]*/
    tmpccer &= CCER_CC3NP_Reset;

    /* Set the Output N Polarity [设置输出N极性]*/
    tmpccer |= (u16)(TIM_OCInitStruct->TIM_OCNPolarity << 8);

    /* Reset the Output N State [复位输出N状态]*/
    tmpccer &= CCER_CC3NE_Reset;
    
    /* Set the Output N State [设置输出N状态]*/
    tmpccer |= (u16)(TIM_OCInitStruct->TIM_OutputNState << 8);

    /* Reset the Ouput Compare and Output Compare N IDLE State [复位输出比较和输出比较N IDLE状态]*/
    tmpcr2 &= CR2_OIS3_Reset;
    tmpcr2 &= CR2_OIS3N_Reset;

    /* Set the Output Idle state [设置输出IDLE状态]*/
    tmpcr2 |= (u16)(TIM_OCInitStruct->TIM_OCIdleState << 4);

    /* Set the Output N Idle state [设置输出N IDLE状态]*/
    tmpcr2 |= (u16)(TIM_OCInitStruct->TIM_OCNIdleState << 4);
  }

  /* Write to TIMx CR2 [写TIMx配置寄存器2]*/
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR2 [写TIMx捕获/比较模式寄存器2]*/
  TIMx->CCMR2 = tmpccmrx;
  
  /* Write to TIMx CCER [写TIMx捕获/比较使能寄存器]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TIM_OC4Init
* 功能描述: 根据TIM_OCInitStruct中的特定参数初始化TIMx通道4。
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_OCInitStruct:指向一个包含特定TIMx外设的配置信息的TIM_OCInitTypeDef结构的指针.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC4Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  u16 tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   

  /* Disable the Channel 2: Reset the CC4E Bit [禁止通道2：复位CC4E位]*/
  TIMx->CCER &= CCER_CC4E_Reset;
  
  /* Get the TIMx CCER register value [取得TIMx 捕获/比较使能寄存器的寄存器值]*/
  tmpccer = TIMx->CCER;

  /* Get the TIMx CR2 register value [取得TIMx 控制2的寄存器值]*/
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR2 register value [取得TIMx 捕获/比较模式寄存器2的寄存器值]*/
  tmpccmrx = TIMx->CCMR2;
    
  /* Reset the Output Compare Mode Bits [复位输出比较模式位]*/
  tmpccmrx &= CCMR_OC24M_Mask;
  
  /* Select the Output Compare Mode [选择输出比较模式位]*/
  tmpccmrx |= (u16)(TIM_OCInitStruct->TIM_OCMode << 8);
  
  /* Reset the Output Polarity level [复位输出极性级别]*/
  tmpccer &= CCER_CC4P_Reset;

  /* Set the Output Compare Polarity [设置输出比较极性]*/
  tmpccer |= (u16)(TIM_OCInitStruct->TIM_OCPolarity << 12);
  
  /* Set the Output State [设置输出状态]*/
  tmpccer |= (u16)(TIM_OCInitStruct->TIM_OutputState << 12);
  
  /* Set the Capture Compare Register value [设置捕获比较寄存器值]*/
  TIMx->CCR4 = TIM_OCInitStruct->TIM_Pulse;
  
  if((*(u32*)&TIMx == TIM1_BASE) || (*(u32*)&TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));

    /* Reset the Ouput Compare IDLE State [设置输出比较IDLE状态]*/
    tmpcr2 &= CR2_OIS4_Reset;

    /* Set the Output Idle state [设置输出IDLE状态]*/
    tmpcr2 |= (u16)(TIM_OCInitStruct->TIM_OCIdleState << 6);
  }

  /* Write to TIMx CR2 [写TIMx配置寄存器2]*/
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR2 [写TIMx捕获/比较模式寄存器2]*/  
  TIMx->CCMR2 = tmpccmrx;
  
  /* Write to TIMx CCER [写TIMx捕获/比较使能寄存器]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TIM_ICInit
* 功能描述: 根据TIM_ ICInitStruct中的特定参数初始化TIMx.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_OCInitStruct:指向一个包含特定TIMx外设的配置信息的TIM_OCInitTypeDef结构的指针.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_ICInitStruct->TIM_Channel));
  assert_param(IS_TIM_IC_POLARITY(TIM_ICInitStruct->TIM_ICPolarity));
  assert_param(IS_TIM_IC_SELECTION(TIM_ICInitStruct->TIM_ICSelection));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICInitStruct->TIM_ICPrescaler));
  assert_param(IS_TIM_IC_FILTER(TIM_ICInitStruct->TIM_ICFilter));
  
  if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1)
  {
    /* TI1 Configuration [TI1配置]*/
    TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [设置输入捕捉预分频器值]*/
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_2)
  {
    /* TI2 Configuration [TI2配置]*/
    TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [设置输入捕捉预分频器值]*/
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_3)
  {
    /* TI3 Configuration [TI2配置]*/
    TI3_Config(TIMx,  TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [设置输入捕捉预分频器值]*/
    TIM_SetIC3Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else
  {
    /* TI4 Configuration [TI2配置]*/
    TI4_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [设置输入捕捉预分频器值]*/
    TIM_SetIC4Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
}

/*******************************************************************************
* 函数名称: TIM_PWMIConfig
* 功能描述: 依照TIM_ICInitStruct指定的参数设置TIM设备去测量外部的PWM信号。
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_ICInitStruct:指向一个包含特定TIMx外设的配置信息的TIM_ICInitTypeDef结构的指针.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_PWMIConfig(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  u16 icoppositepolarity = TIM_ICPolarity_Rising;
  u16 icoppositeselection = TIM_ICSelection_DirectTI;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Select the Opposite Input Polarity [选择相反的输入极性]*/
  if (TIM_ICInitStruct->TIM_ICPolarity == TIM_ICPolarity_Rising)
  {
    icoppositepolarity = TIM_ICPolarity_Falling;
  }
  else
  {
    icoppositepolarity = TIM_ICPolarity_Rising;
  }

  /* Select the Opposite Input [选择相反的输入]*/
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
    /* TI1 Configuration [TI1配置]*/
    TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [设置输入捕捉预分频器值]*/
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);

    /* TI2 Configuration [TI2配置]*/
    TI2_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [设置输入捕捉预分频器值]*/
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else
  { 
    /* TI2 Configuration [TI2配置]*/
    TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [设置输入捕捉预分频器值]*/
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);

    /* TI1 Configuration [TI1配置]*/
    TI1_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value [设置输入捕捉预分频器值]*/
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
}

/*******************************************************************************
* 函数名称: TIM_BDTRConfig
* 功能描述: 配置：间断特征，滞后时间，锁级别，OSSI，OSSR状态和AOE（自动输出使能）.
* 输入参数:(1)TIMx:此处x可以是1到8以选择外设.
*          (2)TIM_BDTRInitStruct:指向一个包含特定TIMx外设的配置信息的TIM_BDTRInitTypeDef结构的指针.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_BDTRConfig(TIM_TypeDef* TIMx, TIM_BDTRInitTypeDef *TIM_BDTRInitStruct)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OSSR_STATE(TIM_BDTRInitStruct->TIM_OSSRState));
  assert_param(IS_TIM_OSSI_STATE(TIM_BDTRInitStruct->TIM_OSSIState));
  assert_param(IS_TIM_LOCK_LEVEL(TIM_BDTRInitStruct->TIM_LOCKLevel));
  assert_param(IS_TIM_BREAK_STATE(TIM_BDTRInitStruct->TIM_Break));
  assert_param(IS_TIM_BREAK_POLARITY(TIM_BDTRInitStruct->TIM_BreakPolarity));
  assert_param(IS_TIM_AUTOMATIC_OUTPUT_STATE(TIM_BDTRInitStruct->TIM_AutomaticOutput));

  /* Set the Lock level, the Break enable Bit and the Polarity, the OSSR State,
  the OSSI State, the dead time value and the Automatic Output Enable Bit [配置：锁级别，间断允许位和极性，OSSR状态，OSSI状态，滞后时间和自动输出使能位]*/

  TIMx->BDTR = (u32)TIM_BDTRInitStruct->TIM_OSSRState | TIM_BDTRInitStruct->TIM_OSSIState |
             TIM_BDTRInitStruct->TIM_LOCKLevel | TIM_BDTRInitStruct->TIM_DeadTime |
             TIM_BDTRInitStruct->TIM_Break | TIM_BDTRInitStruct->TIM_BreakPolarity |
             TIM_BDTRInitStruct->TIM_AutomaticOutput;

}

/*******************************************************************************
* 函数名称: TIM_TimeBaseStructInit
* 功能描述: 使用缺省值填充TIM_TimeBaseInitStruct的每一个成员.
* 输入参数: TIM_TimeBaseInitStruct:指向一个将被初始化的TIM_TimeBaseInitTypeDef结构的指针
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  /* Set the default configuration [设置成默认配置]*/
  TIM_TimeBaseInitStruct->TIM_Period = 0xFFFF;
  TIM_TimeBaseInitStruct->TIM_Prescaler = 0x0000;
  TIM_TimeBaseInitStruct->TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct->TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct->TIM_RepetitionCounter = 0x0000;
}

/*******************************************************************************
* 函数名称: TIM_OCStructInit
* 功能描述: 使用缺省值填充TIM_OCInitStruct的每一个成员
* 输入参数: TIM_OCInitStruct:指向一个将被初始化的TIM_OCInitStruct结构的指针.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OCStructInit(TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  /* Set the default configuration [设置成默认配置]*/
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
* 函数名称: TIM_ICStructInit
* 功能描述: 使用缺省值填充TIM_ICInitStruct的每一个成员.
* 输入参数: TIM_ICInitStruct:指向一个将被初始化的TIM_ICInitStruct结构的指针.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ICStructInit(TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  /* Set the default configuration [设置成默认配置]*/
  TIM_ICInitStruct->TIM_Channel = TIM_Channel_1;
  TIM_ICInitStruct->TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStruct->TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStruct->TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStruct->TIM_ICFilter = 0x00;
}

/*******************************************************************************
* 函数名称: TIM_BDTRStructInit
* 功能描述: 将TIM1_BDTRStruct中的成员赋为默认值.
* 输入参数: TIM1_BDTRInitStruct:指向一个将被初始化的TIM1_BDTRInitStruct结构体。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_BDTRStructInit(TIM_BDTRInitTypeDef* TIM_BDTRInitStruct)
{
  /* Set the default configuration [设置成默认配置]*/
  TIM_BDTRInitStruct->TIM_OSSRState = TIM_OSSRState_Disable;
  TIM_BDTRInitStruct->TIM_OSSIState = TIM_OSSIState_Disable;
  TIM_BDTRInitStruct->TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStruct->TIM_DeadTime = 0x00;
  TIM_BDTRInitStruct->TIM_Break = TIM_Break_Disable;
  TIM_BDTRInitStruct->TIM_BreakPolarity = TIM_BreakPolarity_Low;
  TIM_BDTRInitStruct->TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
}

/*******************************************************************************
* 函数名称: TIM_Cmd
* 功能描述: 使能或关闭特定的TIM外设.
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)NewState:此TIM外设的新状态，该参数可以是ENABLE或DISABLE。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the TIM Counter [允许tim计数器]*/
    TIMx->CR1 |= CR1_CEN_Set;
  }
  else
  {
    /* Disable the TIM Counter [禁止tim计数器]*/
    TIMx->CR1 &= CR1_CEN_Reset;
  }
}

/*******************************************************************************
* 函数名称: TIM_CtrlPWMOutputs
* 功能描述: 使能或禁用TIM外围主输出.
* 输入参数:(1)TIMx:此处x可以是1到8以选择外设.
*          (2)Newstate：TIM1外围设备主输出的新状态。这个参数能够取：ENABLE或者DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_CtrlPWMOutputs(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the TIM Main Output [允许TIM主输出]*/
    TIMx->BDTR |= BDTR_MOE_Set;
  }
  else
  {
    /* Disable the TIM Main Output [禁止TIM主输出]*/
    TIMx->BDTR &= BDTR_MOE_Reset;
  }  
}

/*******************************************************************************
* 函数名称: TIM_ITConfig
* 功能描述: 使能或关闭特定的TIM中断.
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)TIM_IT: 将被使能或关闭的TIM中断源。
*                    TIM_IT这个参数允许的值:
*                       - TIM_IT_Update: TIM更新中断源
*                       - TIM_IT_CC1: TIM捕获/比较1中断源
*                       - TIM_IT_CC2: TIM捕获/比较2中断源
*                       - TIM_IT_CC3: TIM捕获/比较3中断源
*                       - TIM_IT_CC4: TIM捕获/比较4中断源
*                       - TIM_IT_COM: TIM 交换中断源
*                       - TIM_IT_Trigger: TIM触发器中断源
*                       - TIM_IT_Break: TIM1间断中断源
*           (3)NewState:特定TIM1中断的新状态。这个参数能够取：ENABLE或者DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ITConfig(TIM_TypeDef* TIMx, u16 TIM_IT, FunctionalState NewState)
{  
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_IT(TIM_IT));
  assert_param(IS_TIM_PERIPH_IT((TIMx), (TIM_IT)));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the Interrupt sources [允许中断源]*/
    TIMx->DIER |= TIM_IT;
  }
  else
  {
    /* Disable the Interrupt sources [禁止中断源]*/
    TIMx->DIER &= (u16)~TIM_IT;
  }
}

/*******************************************************************************
* 函数名称: TIM_GenerateEvent
* 功能描述: 配置将由软件引发的TIM事件.
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           TIM1_EventSource：指明TIM1软件事件源。
*                    这个参数可以取下面的值之一:这个参数可以取下面的值之一：    
*                       - TIM_EventSource_Update: 定时器更新事件源
*                       - TIM_EventSource_CC1: 定时器比较捕捉1事件源
*                       - TIM_EventSource_CC2: 定时器比较捕捉2事件源
*                       - TIM_EventSource_CC3: 定时器比较捕捉3事件源
*                       - TIM_EventSource_CC4: 定时器比较捕捉4事件源
*                       - TIM_EventSource_Trigger: 定时器触发器事件源
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_GenerateEvent(TIM_TypeDef* TIMx, u16 TIM_EventSource)
{ 
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_EVENT_SOURCE(TIM_EventSource));
  assert_param(IS_TIM_PERIPH_EVENT((TIMx), (TIM_EventSource)));

  /* Set the event sources [设置事件源]*/
  TIMx->EGR = TIM_EventSource;
}

/*******************************************************************************
* 函数名称: TIM_DMAConfig
* 功能描述: 配置某个TIM的DMA接口.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_DMABase: DMA基址。
*                    这个参数可以取下面的值之一:
*                       - TIM_DMABase_CR, TIM_DMABase_CR2, TIM_DMABase_SMCR,
*                         TIM_DMABase_DIER, TIM1_DMABase_SR, TIM_DMABase_EGR,
*                         TIM_DMABase_CCMR1, TIM_DMABase_CCMR2, TIM_DMABase_CCER,
*                         TIM_DMABase_CNT, TIM_DMABase_PSC, TIM_DMABase_ARR,
*                         TIM_DMABase_RCR, TIM_DMABase_CCR1, TIM_DMABase_CCR2,
*                         TIM_DMABase_CCR3, TIM_DMABase_CCR4, TIM_DMABase_BDTR,
*                         TIM_DMABase_DCR.
*           (3)TIM_DMABurstLength: DMA猝发长度。
*                     这个参数可以取下面两个值之一:
*                     TIM_DMABurstLength_1Byte and TIM_DMABurstLength_18Bytes.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_DMAConfig(TIM_TypeDef* TIMx, u16 TIM_DMABase, u16 TIM_DMABurstLength)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_DMA_BASE(TIM_DMABase));
  assert_param(IS_TIM_DMA_LENGTH(TIM_DMABurstLength));

  /* Set the DMA Base and the DMA Burst Length [设置DMA基址和DMA猝发长度]*/
  TIMx->DCR = TIM_DMABase | TIM_DMABurstLength;
}

/*******************************************************************************
* 函数名称: TIM_DMACmd
* 功能描述: 使能或关闭TIMx的DMA请求.
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)TIM_DMASource: DMA请求源。
*                    这个参数可以是下面任何值的组合：
*                       - TIM_DMA_Update: TIM 更新中断源
*                       - TIM_DMA_CC1: TIM 捕获比较 1 DMA 源
*                       - TIM_DMA_CC2: TIM 捕获比较 2 DMA 源
*                       - TIM_DMA_CC3: TIM 捕获比较 3 DMA 源
*                       - TIM_DMA_CC4: TIM 捕获比较 4 DMA 源
*                       - TIM_DMA_COM: TIM 交换 DMA 源
*                       - TIM_DMA_Trigger: TIM 触发 DMA 源
*           (3)NewState:此TIM DMA请求源的新状态，该参数可以是ENABLE或DISABLE。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_DMACmd(TIM_TypeDef* TIMx, u16 TIM_DMASource, FunctionalState NewState)
{ 
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_DMA_SOURCE(TIM_DMASource));
  assert_param(IS_TIM_PERIPH_DMA(TIMx, TIM_DMASource));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the DMA sources [允许DMA源]*/
    TIMx->DIER |= TIM_DMASource; 
  }
  else
  {
    /* Disable the DMA sources [禁止DMA源]*/
    TIMx->DIER &= (u16)~TIM_DMASource;
  }
}

/*******************************************************************************
* 函数名称: TIM_InternalClockConfig
* 功能描述: 配置TIM的内部时钟
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_InternalClockConfig(TIM_TypeDef* TIMx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Disable slave mode to clock the prescaler directly with the internal clock [禁止内部时钟预分频器从模式]*/
  TIMx->SMCR &=  SMCR_SMS_Mask;
}
/*******************************************************************************
* 函数名称: TIM_ITRxExternalClockConfig
* 功能描述: 配置TIM的内部触发器作为外部时钟
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_InputTriggerSource:输入触发源。
*                    这个参数可以取下面的值之一:
*                       - TIM_TS_ITR0: 内部触发器 0
*                       - TIM_TS_ITR1: 内部触发器 1
*                       - TIM_TS_ITR2: 内部触发器 2
*                       - TIM_TS_ITR3: 内部触发器 3
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, u16 TIM_InputTriggerSource)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_INTERNAL_TRIGGER_SELECTION(TIM_InputTriggerSource));

  /* Select the Internal Trigger [选择内部触发器]*/
  TIM_SelectInputTrigger(TIMx, TIM_InputTriggerSource);

  /* Select the External clock mode1 [选择外部时钟模式1]*/
  TIMx->SMCR |= TIM_SlaveMode_External1;
}
/*******************************************************************************
* 函数名称: TIM_TIxExternalClockConfig
* 功能描述: 配置TIM的触发器作为外部时钟
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_ TIxExternalCLKSource: 触发器源。
*                    这个参数可以取下面的值之一:
*                       - TIM_TIxExternalCLK1Source_TI1ED: TI1 边缘检测器
*                       - TIM_TIxExternalCLK1Source_TI1: 已筛选的定时器输入1
*                       - TIM_TIxExternalCLK1Source_TI2: 已筛选的定时器输入2
*           (3)TIM_ICPolarity: 指定TI的极性
*                    这个参数可以是:
*                       - TIM_ICPolarity_Rising[上升沿]
*                       - TIM_ICPolarity_Falling[下降沿]
*           (4)ICFilter:指定输入捕获过滤器。该参数可以是0x0到0xF之间的值。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, u16 TIM_TIxExternalCLKSource,
                                u16 TIM_ICPolarity, u16 ICFilter)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_TIXCLK_SOURCE(TIM_TIxExternalCLKSource));
  assert_param(IS_TIM_IC_POLARITY(TIM_ICPolarity));
  assert_param(IS_TIM_IC_FILTER(ICFilter));

  /* Configure the Timer Input Clock Source [配置定时器输入时钟源]*/
  if (TIM_TIxExternalCLKSource == TIM_TIxExternalCLK1Source_TI2)
  {
    TI2_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
  }
  else
  {
    TI1_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
  }

  /* Select the Trigger source [选择触发器源]*/
  TIM_SelectInputTrigger(TIMx, TIM_TIxExternalCLKSource);

  /* Select the External clock mode1 [选择外部时钟模式1]*/
  TIMx->SMCR |= TIM_SlaveMode_External1;
}

/*******************************************************************************
* 函数名称: TIM_ETRClockMode1Config
* 功能描述: 配置外部时钟模式1
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_ExtTRGPrescaler:外部触发器预分频数。
*                    这个参数可以取下面的值之一:
*                       - TIM_ExtTRGPSC_OFF
*                       - TIM_ExtTRGPSC_DIV2
*                       - TIM_ExtTRGPSC_DIV4
*                       - TIM_ExtTRGPSC_DIV8.
*           (3)TIM_ExtTRGPolarity:外部触发器极性。
*                     这个参数可以取下面的值之一:
*                       - TIM_ExtTRGPolarity_Inverted
*                       - TIM_ExtTRGPolarity_NonInverted
*           (4)ExtTRGFilter:外部触发器过滤器。该参数可以是0x0到0xF之间的值。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, u16 TIM_ExtTRGPrescaler, u16 TIM_ExtTRGPolarity,
                             u16 ExtTRGFilter)
{
  u16 tmpsmcr = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));

  /* Configure the ETR Clock source [配置外部触发器时钟源]*/
  TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);
  
  /* Get the TIMx SMCR register value [取得TIMx SMCR寄存器值]*/
  tmpsmcr = TIMx->SMCR;

  /* Reset the SMS Bits [复位SMS]*/
  tmpsmcr &= SMCR_SMS_Mask;
  /* Select the External clock mode1 [选择外部时钟模式1]*/
  tmpsmcr |= TIM_SlaveMode_External1;

  /* Select the Trigger selection : ETRF [选择触发选项: ETRF]*/
  tmpsmcr &= SMCR_TS_Mask;
  tmpsmcr |= TIM_TS_ETRF;

  /* Write to TIMx SMCR [写TIMx SMCR]*/
  TIMx->SMCR = tmpsmcr;
}

/*******************************************************************************
* 函数名称: TIM_ETRClockMode2Config
* 功能描述: 配置TIMx外部时钟为模式2
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_ExtTRGPrescaler:外部触发器预分频数.
*                    它可以是下面的值之一:
*                       - TIM_ExtTRGPSC_OFF
*                       - TIM_ExtTRGPSC_DIV2
*                       - TIM_ExtTRGPSC_DIV4
*                       - TIM_ExtTRGPSC_DIV8
*           (3)TIM_ExtTRGPolarity:外部触发器极性.
*                    它可以是下面的值之一:
*                       - TIM_ExtTRGPolarity_Inverted
*                       - TIM_ExtTRGPolarity_NonInverted
*           (4)ExtTRGFilter:外部触发器过滤器。该参数可以是0x0到0xF之间的值。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, u16 TIM_ExtTRGPrescaler, 
                             u16 TIM_ExtTRGPolarity, u16 ExtTRGFilter)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));

  /* Configure the ETR Clock source [配置ETR的时钟源]*/
  TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);

  /* Enable the External clock mode2 [使能外部时钟模式2]*/
  TIMx->SMCR |= SMCR_ECE_Set;
}

/*******************************************************************************
* 函数名称: TIM_ETRConfig
* 功能描述: 配置TIMx的外部触发(ETR).
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_ExtTRGPrescaler:外部触发器预分频数.
*                    这个参数可以取下面的值之一:
*                       - TIM_ExtTRGPSC_OFF
*                       - TIM_ExtTRGPSC_DIV2
*                       - TIM_ExtTRGPSC_DIV4
*                       - TIM_ExtTRGPSC_DIV8
*           (3)TIM_ExtTRGPolarity:外部触发器极性.
*                    这个参数可以取下面的值之一:
*                       - TIM_ExtTRGPolarity_Inverted
*                       - TIM_ExtTRGPolarity_NonInverted
*           (4)ExtTRGFilter:外部触发器过滤器。该参数可以是0x0到0xF之间的值。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ETRConfig(TIM_TypeDef* TIMx, u16 TIM_ExtTRGPrescaler, u16 TIM_ExtTRGPolarity,
                   u16 ExtTRGFilter)
{
  u16 tmpsmcr = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));

  tmpsmcr = TIMx->SMCR;

  /* Reset the ETR Bits [复位ETR位]*/
  tmpsmcr &= SMCR_ETR_Mask;

  /* Set the Prescaler, the Filter value and the Polarity [设置预分频器，过滤器值和极性]*/
  tmpsmcr |= TIM_ExtTRGPrescaler | TIM_ExtTRGPolarity | (u16)(ExtTRGFilter << 8);

  /* Write to TIMx SMCR [写TIMx SMCR]*/
  TIMx->SMCR = tmpsmcr;
}

/*******************************************************************************
* 函数名称: TIM_PrescalerConfig
* 功能描述: Configures the TIMx Prescaler.
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)Prescaler:TIMx预分频数的新值
*           (3)TIM_PSCReloadMode:TIM预分频数重载模式.
*                    这个参数可以取下面的值之一:
*                       - TIM_PSCReloadMode_Update: 发生更新事件时预分频数载入.
*                       - TIM_PSCReloadMode_Immediate: 预分频数立即载入.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, u16 Prescaler, u16 TIM_PSCReloadMode)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_PRESCALER_RELOAD(TIM_PSCReloadMode));

  /* Set the Prescaler value [设置预分频值]*/
  TIMx->PSC = Prescaler;

  /* Set or reset the UG Bit [置位或复位UG]*/
  TIMx->EGR = TIM_PSCReloadMode;
}

/*******************************************************************************
* 函数名称: TIM_CounterModeConfig
* 功能描述: 指定TIMx使用计数模式.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_CounterMode:将使用的计数模式.
*                    这个参数可以取下面的值之一:
*                       - TIM_CounterMode_Up: TIM向上计数模式
*                       - TIM_CounterMode_Down: TIM向下计数模式
*                       - TIM_CounterMode_CenterAligned1: TIM计数对齐模式1
*                       - TIM_CounterMode_CenterAligned2: TIM计数对齐模式2
*                       - TIM_CounterMode_CenterAligned3: TIM计数对齐模式3
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_CounterModeConfig(TIM_TypeDef* TIMx, u16 TIM_CounterMode)
{
  u16 tmpcr1 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_COUNTER_MODE(TIM_CounterMode));

  tmpcr1 = TIMx->CR1;

  /* Reset the CMS and DIR Bits [复位CMS和DIR]*/
  tmpcr1 &= CR1_CounterMode_Mask;

  /* Set the Counter Mode [设置计数模式]*/
  tmpcr1 |= TIM_CounterMode;

  /* Write to TIMx CR1 register [写TIMx CR1 寄存器]*/
  TIMx->CR1 = tmpcr1;
}

/*******************************************************************************
* 函数名称: TIM_SelectInputTrigger
* 功能描述: 选择输入触发源
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_InputTriggerSource: 输入触发源.
*                    这个参数可以取下面的值之一:
*                       - TIM_TS_ITR0: 内部触发 0
*                       - TIM_TS_ITR1: 内部触发 1
*                       - TIM_TS_ITR2: 内部触发 2
*                       - TIM_TS_ITR3: 内部触发 3
*                       - TIM_TS_TI1F_ED: TI1 边沿检测
*                       - TIM_TS_TI1FP1: 过滤器时钟输入 1
*                       - TIM_TS_TI2FP2: 过滤器时钟输入 2
*                       - TIM_TS_ETRF: 扩展触发输入
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, u16 TIM_InputTriggerSource)
{
  u16 tmpsmcr = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_TRIGGER_SELECTION(TIM_InputTriggerSource));

  /* Get the TIMx SMCR register value [取得TIMx SMCR寄存器值]*/
  tmpsmcr = TIMx->SMCR;

  /* Reset the TS Bits [复位TS]*/
  tmpsmcr &= SMCR_TS_Mask;

  /* Set the Input Trigger source [设置输入中断源]*/
  tmpsmcr |= TIM_InputTriggerSource;

  /* Write to TIMx SMCR [写TIMx SMCR]*/
  TIMx->SMCR = tmpsmcr;
}

/*******************************************************************************
* 函数名称: TIM_EncoderInterfaceConfig
* 功能描述: 配置TIM的编码器接口
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)IM_EncoderMode:编码器模式.
*                    这个参数可以取下面的值之一:
*                       - TIM_EncoderMode_TI1: TIM编码器使用模式1.
*                       - TIM_EncoderMode_TI2: TIM编码器使用模式2.
*                       - TIM_EncoderMode_TI12:TIM编码器使用模式3.
*            (3)TIM_IC1Polarity: TI1的极性.
*                    这个参数可以取下面的值之一:
*                        - TIM_ICPolarity_Falling: 输入捕获下降沿.
*                        - TIM_ICPolarity_Rising: 输入捕获上升沿.
*            (4)TIM_IC2Polarity:TI2的极性.
*                    这个参数可以取下面的值之一:
*                        - TIM_ICPolarity_Falling: 输入捕获下降沿.
*                        - TIM_ICPolarity_Rising: 输入捕获上升沿.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_EncoderInterfaceConfig(TIM_TypeDef* TIMx, u16 TIM_EncoderMode,
                                u16 TIM_IC1Polarity, u16 TIM_IC2Polarity)
{
  u16 tmpsmcr = 0;
  u16 tmpccmr1 = 0;
  u16 tmpccer = 0;
    
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_ENCODER_MODE(TIM_EncoderMode));
  assert_param(IS_TIM_IC_POLARITY(TIM_IC1Polarity));
  assert_param(IS_TIM_IC_POLARITY(TIM_IC2Polarity));

  /* Get the TIMx SMCR register value [取得TIMx SMCR寄存器值]*/
  tmpsmcr = TIMx->SMCR;

  /* Get the TIMx CCMR1 register value [取得TIMx 捕获/比较模式寄存器1的寄存器值]*/
  tmpccmr1 = TIMx->CCMR1;

  /* Get the TIMx CCER register value [取得TIMx 捕获/比较使能寄存器的寄存器值]*/
  tmpccer = TIMx->CCER;

  /* Set the encoder Mode [设置编码模式]*/
  tmpsmcr &= SMCR_SMS_Mask;
  tmpsmcr |= TIM_EncoderMode;

  /* Select the Capture Compare 1 and the Capture Compare 2 as input [选择比较捕捉1和较捕捉2作为输入]*/
  tmpccmr1 &= CCMR_CC13S_Mask & CCMR_CC24S_Mask;
  tmpccmr1 |= CCMR_TI13Direct_Set | CCMR_TI24Direct_Set;

  /* Set the TI1 and the TI2 Polarities [设置TI1和TI2的极性]*/
  tmpccer &= CCER_CC1P_Reset & CCER_CC2P_Reset;
  tmpccer |= (TIM_IC1Polarity | (u16)(TIM_IC2Polarity << 4));

  /* Write to TIMx SMCR [写TIMx SMCR]*/
  TIMx->SMCR = tmpsmcr;

  /* Write to TIMx CCMR1 [写TIMx捕获/比较模式寄存器1]*/
  TIMx->CCMR1 = tmpccmr1;

  /* Write to TIMx CCER [写TIMx捕获/比较使能寄存器]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TIM_ForcedOC1Config
* 功能描述: 强制TIMx信道1的输出波形到有效电平或者无效电平.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM1_ForcedAction：指定对输出波形的强制行为..
*                    这个参数可以取下面的值之一:
*                       - TIM_ForcedAction_Active: 强制OCxREF为有效电平
*                       - TIM_ForcedAction_InActive: 强制OCxREF为无效电平.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ForcedOC1Config(TIM_TypeDef* TIMx, u16 TIM_ForcedAction)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC1M Bits [复位OC1M]*/
  tmpccmr1 &= CCMR_OC13M_Mask;

  /* Configure The Forced output Mode [配置强制输出模式]*/
  tmpccmr1 |= TIM_ForcedAction;

  /* Write to TIMx CCMR1 register [写TIMx CCMR1寄存器]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* 函数名称: TIM_ForcedOC2Config
* 功能描述: 强制TIMx信道2的输出波形到有效或无效电平。.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIMx_ForcedAction：指定对输出波形的强制行为.
*                    这个参数可以取下面的值之一:
*                       - TIM_ForcedAction_Active: 强制OC2REF为有效电平
*                       - TIM_ForcedAction_InActive:强制OC2REF为无效电平
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ForcedOC2Config(TIM_TypeDef* TIMx, u16 TIM_ForcedAction)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC2M Bits [复位OC2M]*/
  tmpccmr1 &= CCMR_OC24M_Mask;

  /* Configure The Forced output Mode [配置强制输出模式]*/
  tmpccmr1 |= (u16)(TIM_ForcedAction << 8);

  /* Write to TIMx CCMR1 register [写TIMx CCMR1寄存器]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* 函数名称: TIM_ForcedOC3Config
* 功能描述: 强制TIMx信道3的输出波形到有效或无效电平。
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIMx_ForcedAction：指定对输出波形的强制行为.
*                    这个参数可以取下面的值之一:
*                       - TIM_ForcedAction_Active: 强制OC3REF为有效电平
*                       - TIM_ForcedAction_InActive:强制OC3REF为无效电平.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ForcedOC3Config(TIM_TypeDef* TIMx, u16 TIM_ForcedAction)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC1M Bits [复位OC1M]*/
  tmpccmr2 &= CCMR_OC13M_Mask;

  /* Configure The Forced output Mode [配置强制输出模式]*/
  tmpccmr2 |= TIM_ForcedAction;

  /* Write to TIMx CCMR2 register [写TIMx CCMR2寄存器]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* 函数名称: TIM_ForcedOC4Config
* 功能描述: 强制TIMx信道4的输出波形到有效或无效电平。
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIMx_ForcedAction：指定对输出波形的强制行为.
*                    这个参数可以取下面的值之一:
*                       - TIM_ForcedAction_Active: 强制OC4REF为有效电平
*                       - TIM_ForcedAction_InActive:强制OC4REF为无效电平.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ForcedOC4Config(TIM_TypeDef* TIMx, u16 TIM_ForcedAction)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC2M Bits [复位OC2M]*/
  tmpccmr2 &= CCMR_OC24M_Mask;

  /* Configure The Forced output Mode [配置强制输出模式]*/
  tmpccmr2 |= (u16)(TIM_ForcedAction << 8);

  /* Write to TIMx CCMR2 register [写TIMx CCMR2寄存器]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* 函数名称: TIM_ARRPreloadConfig
* 功能描述: 使能或禁用TIMx外设ARR预载寄存器.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)Newstate:在TIM1_CR1寄存器中的ARPE位的新状态.这个参数能够取:ENABLE或者DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the ARR Preload Bit [置位ARR的预装载位]*/
    TIMx->CR1 |= CR1_ARPE_Set;
  }
  else
  {
    /* Reset the ARR Preload Bit [复位ARR的预装载位]*/
    TIMx->CR1 &= CR1_ARPE_Reset;
  }
}

/*******************************************************************************
* 函数名称: TIM_SelectCOM
* 功能描述: 选择TIM外围设备交换事件.
* 输入参数:(1)TIMx:此处x可以是1到8以选择外设.
*          (2)Newstate：交换事件的新状态.这个参数能够取：ENABLE或者DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SelectCOM(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the COM Bit [置位COM位]*/
    TIMx->CR2 |= CR2_CCUS_Set;
  }
  else
  {
    /* Reset the COM Bit [复位COM位]*/
    TIMx->CR2 &= CR2_CCUS_Reset;
  }
}

/*******************************************************************************
* 函数名称: TIM_SelectCCDMA
* 功能描述: 选择TIMx外围设备捕获比较DMA源.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)Newstate：捕获比较DMA源的新状态.这个参数能够取：ENABLE或者DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SelectCCDMA(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the CCDS Bit [置位CCDS位]*/
    TIMx->CR2 |= CR2_CCDS_Set;
  }
  else
  {
    /* Reset the CCDS Bit [复位CCDS位]*/
    TIMx->CR2 &= CR2_CCDS_Reset;
  }
}

/*******************************************************************************
* 函数名称: TIM_CCPreloadControl
* 功能描述: 设置或重置TIM外围设备捕获比较预载控制位.
* 输入参数:(1)TIMx:此处x可以是1到8以选择外设.
*          (2)Newstate：捕获比较预载控制位的新状态.这个参数能够取：ENABLE或者DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_CCPreloadControl(TIM_TypeDef* TIMx, FunctionalState NewState)
{ 
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the CCPC Bit [置位CCPC]*/
    TIMx->CR2 |= CR2_CCPC_Set;
  }
  else
  {
    /* Reset the CCPC Bit [复位CCPC]*/
    TIMx->CR2 &= CR2_CCPC_Reset;
  }
}

/*******************************************************************************
* 函数名称: TIM_OC1PreloadConfig
* 功能描述: 使能或禁用CCR1上TIMx预载寄存器.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM1_OCPreload：输出比较重载状态,
*                    这个参数可以取下面的值之一:
*                       - TIM_OCPreload_Enable
*                       - TIM_OCPreload_Disable
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, u16 TIM_OCPreload)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC1PE Bit [复位OC1PE]*/
  tmpccmr1 &= CCMR_OC13PE_Reset;

  /* Enable or Disable the Output Compare Preload feature [使能或禁止输出比较预装载特征]*/
  tmpccmr1 |= TIM_OCPreload;

  /* Write to TIMx CCMR1 register [写TIMx CCMR1寄存器]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* 函数名称: TIM_OC2PreloadConfig
* 功能描述: 使能或禁用CCR2上TIMx预载寄存器.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM1_OCPreload：输出比较重载状态,
*                    这个参数可以取下面的值之一:
*                       - TIM_OCPreload_Enable
*                       - TIM_OCPreload_Disable
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC2PreloadConfig(TIM_TypeDef* TIMx, u16 TIM_OCPreload)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC2PE Bit [复位OC2PE]*/
  tmpccmr1 &= CCMR_OC24PE_Reset;

  /* Enable or Disable the Output Compare Preload feature [使能或禁止输出比较预装载特征]*/
  tmpccmr1 |= (u16)(TIM_OCPreload << 8);

  /* Write to TIMx CCMR1 register [写TIMx CCMR1寄存器]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* 函数名称: TIM_OC3PreloadConfig
* 功能描述: 使能或禁用CCR3上TIMx预载寄存器.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM1_OCPreload：输出比较重载状态,
*                    这个参数可以取下面的值之一:
*                       - TIM_OCPreload_Enable
*                       - TIM_OCPreload_Disable
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC3PreloadConfig(TIM_TypeDef* TIMx, u16 TIM_OCPreload)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC3PE Bit [复位OC3PE]*/
  tmpccmr2 &= CCMR_OC13PE_Reset;

  /* Enable or Disable the Output Compare Preload feature [使能或禁止输出比较预装载特征]*/
  tmpccmr2 |= TIM_OCPreload;

  /* Write to TIMx CCMR2 register [写TIMx CCMR2寄存器]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* 函数名称: TIM_OC4PreloadConfig
* 功能描述: 使能或禁用CCR4上TIMx预载寄存器.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM1_OCPreload：输出比较重载状态,
*                    这个参数可以取下面的值之一:
*                       - TIM_OCPreload_Enable
*                       - TIM_OCPreload_Disable
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC4PreloadConfig(TIM_TypeDef* TIMx, u16 TIM_OCPreload)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC4PE Bit [复位OC4PE]*/
  tmpccmr2 &= CCMR_OC24PE_Reset;

  /* Enable or Disable the Output Compare Preload feature [使能或禁止输出比较预装载特征]*/
  tmpccmr2 |= (u16)(TIM_OCPreload << 8);

  /* Write to TIMx CCMR2 register [写TIMx CCMR2寄存器]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* 函数名称: TIM_OC1FastConfig
* 功能描述: 配置TIMx捕获比较1快速性能.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM1_OCFast:输出比较快速特性状态.
*                    这个参数可以取下面的值之一:
*                       - TIM_OCFast_Enable: TIM输出比较快速性能使能
*                       - TIM_OCFast_Disable: TIM输出比较快速性能禁用
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC1FastConfig(TIM_TypeDef* TIMx, u16 TIM_OCFast)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));

  /* Get the TIMx CCMR1 register value [取得TIMx 捕获/比较模式寄存器1的寄存器值]*/
  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC1FE Bit [复位OC1PE]*/
  tmpccmr1 &= CCMR_OC13FE_Reset;

  /* Enable or Disable the Output Compare Fast Bit */
  tmpccmr1 |= TIM_OCFast;

  /* Write to TIMx CCMR1 [写TIMx捕获/比较模式寄存器1]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* 函数名称: TIM_OC2FastConfig
* 功能描述: 配置TIMx捕获比较2快速性能.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM1_OCFast:输出比较快速特性状态.
*                    这个参数可以取下面的值之一:
*                       - TIM_OCFast_Enable: TIM输出比较快速性能使能
*                       - TIM_OCFast_Disable: TIM输出比较快速性能禁用
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC2FastConfig(TIM_TypeDef* TIMx, u16 TIM_OCFast)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));

  /* Get the TIMx CCMR1 register value [取得TIMx 捕获/比较模式寄存器1的寄存器值]*/
  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC2FE Bit [复位OC2PE]*/
  tmpccmr1 &= CCMR_OC24FE_Reset;

  /* Enable or Disable the Output Compare Fast Bit [使能或禁止快速输出比较位]*/
  tmpccmr1 |= (u16)(TIM_OCFast << 8);

  /* Write to TIMx CCMR1 [写TIMx捕获/比较模式寄存器1]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* 函数名称: TIM_OC3FastConfig
* 功能描述: 配置TIMx捕获比较3快速性能.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM1_OCFast:输出比较快速特性状态.
*                    这个参数可以取下面的值之一:
*                       - TIM_OCFast_Enable: TIM输出比较快速性能使能
*                       - TIM_OCFast_Disable: TIM输出比较快速性能禁用
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC3FastConfig(TIM_TypeDef* TIMx, u16 TIM_OCFast)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));

  /* Get the TIMx CCMR2 register value [取得TIMx CCMR2寄存器值]*/
  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC3FE Bit [复位OC3PE]*/
  tmpccmr2 &= CCMR_OC13FE_Reset;

  /* Enable or Disable the Output Compare Fast Bit [使能或禁止快速输出比较位]*/
  tmpccmr2 |= TIM_OCFast;

  /* Write to TIMx CCMR2 [写TIMx CCMR2]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* 函数名称: TIM_OC4FastConfig
* 功能描述: 配置TIMx捕获比较4快速性能.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM1_OCFast:输出比较快速特性状态.
*                    这个参数可以取下面的值之一:
*                       - TIM_OCFast_Enable: TIM输出比较快速性能使能
*                       - TIM_OCFast_Disable: TIM输出比较快速性能禁用
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC4FastConfig(TIM_TypeDef* TIMx, u16 TIM_OCFast)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));

  /* Get the TIMx CCMR2 register value [取得TIMx CCMR2寄存器值]*/
  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC4FE Bit [复位OC4PE]*/
  tmpccmr2 &= CCMR_OC24FE_Reset;

  /* Enable or Disable the Output Compare Fast Bit [使能或禁止快速输出比较位]*/
  tmpccmr2 |= (u16)(TIM_OCFast << 8);

  /* Write to TIMx CCMR2 [写TIMx CCMR2]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* 函数名称: TIM_ClearOC1Ref
* 功能描述: 清除或维护在外部事件中的OCREF1信号
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM1_OCClear:输出比较清除使能位的新状态.
*                    这个参数可以取下面的值之一:
*                       - TIM_OCClear_Enable: TIM输出比较清除使能
*                       - TIM_OCClear_Disable: 输出比较清除禁用
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ClearOC1Ref(TIM_TypeDef* TIMx, u16 TIM_OCClear)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC1CE Bit [复位OC1CE]*/
  tmpccmr1 &= CCMR_OC13CE_Reset;

  /* Enable or Disable the Output Compare Clear Bit [使能或禁止快速输出比较位]*/
  tmpccmr1 |= TIM_OCClear;

  /* Write to TIMx CCMR1 register [写TIMx CCMR1寄存器]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* 函数名称: TIM_ClearOC2Ref
* 功能描述: 清除或维护在外部事件中的OCREF2信号
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM1_OCClear:输出比较清除使能位的新状态.
*                    这个参数可以取下面的值之一:
*                       - TIM_OCClear_Enable: TIM输出比较清除使能
*                       - TIM_OCClear_Disable: 输出比较清除禁用
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ClearOC2Ref(TIM_TypeDef* TIMx, u16 TIM_OCClear)
{
  u16 tmpccmr1 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC2CE Bit [复位OC2CE]*/
  tmpccmr1 &= CCMR_OC24CE_Reset;

  /* Enable or Disable the Output Compare Clear Bit [使能或禁止快速输出比较位]*/
  tmpccmr1 |= (u16)(TIM_OCClear << 8);

  /* Write to TIMx CCMR1 register [写TIMx CCMR1寄存器]*/
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* 函数名称: TIM_ClearOC3Ref
* 功能描述: 清除或维护在外部事件中的OCREF3信号
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM1_OCClear:输出比较清除使能位的新状态.
*                    这个参数可以取下面的值之一:
*                       - TIM_OCClear_Enable: TIM输出比较清除使能
*                       - TIM_OCClear_Disable: 输出比较清除禁用
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ClearOC3Ref(TIM_TypeDef* TIMx, u16 TIM_OCClear)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC3CE Bit [复位OC3CE]*/
  tmpccmr2 &= CCMR_OC13CE_Reset;

  /* Enable or Disable the Output Compare Clear Bit [使能或禁止快速输出比较位]*/
  tmpccmr2 |= TIM_OCClear;

  /* Write to TIMx CCMR2 register [写TIMx CCMR2寄存器]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* 函数名称: TIM_ClearOC4Ref
* 功能描述: 清除或维护在外部事件中的OCREF4信号
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM1_OCClear:输出比较清除使能位的新状态.
*                    这个参数可以取下面的值之一:
*                       - TIM_OCClear_Enable:输出比较清除使能
*                       - TIM_OCClear_Disable:输出比较清除禁用
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ClearOC4Ref(TIM_TypeDef* TIMx, u16 TIM_OCClear)
{
  u16 tmpccmr2 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC4CE Bit [复位OC4CE]*/
  tmpccmr2 &= CCMR_OC24CE_Reset;

  /* Enable or Disable the Output Compare Clear Bit [使能或禁止快速输出比较位]*/
  tmpccmr2 |= (u16)(TIM_OCClear << 8);

  /* Write to TIMx CCMR2 register [写TIMx CCMR2寄存器]*/
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* 函数名称: TIM_OC1PolarityConfig
* 功能描述: 配置TIM通道1极性.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_OCPolarity: 指定的 OC1 极性
*                    这个参数可以取下面的值之一:
*                       - TIM_OCPolarity_High: 输出极性高
*                       - TIM_OCPolarity_Low: 输出极性低
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC1PolarityConfig(TIM_TypeDef* TIMx, u16 TIM_OCPolarity)
{
  u16 tmpccer = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));

  tmpccer = TIMx->CCER;

  /* Set or Reset the CC1P Bit [置位或复位CC1P]*/
  tmpccer &= CCER_CC1P_Reset;
  tmpccer |= TIM_OCPolarity;

  /* Write to TIMx CCER register [写TIMx CCER寄存器]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TIM_OC1NPolarityConfig
* 功能描述: 配置TIM通道1N极性.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_OCPolarity: 指定的 OC1N 极性
*                    这个参数可以取下面的值之一:
*                       - TIM_OCNPolarity_High: 输出极性高
*                       - TIM_OCNPolarity_Low: 输出极性低
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC1NPolarityConfig(TIM_TypeDef* TIMx, u16 TIM_OCNPolarity)
{
  u16 tmpccer = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
   
  tmpccer = TIMx->CCER;

  /* Set or Reset the CC1NP Bit [置位或复位CC1NP]*/
  tmpccer &= CCER_CC1NP_Reset;
  tmpccer |= TIM_OCNPolarity;

  /* Write to TIMx CCER register [写TIMx CCER寄存器]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TIM_OC2PolarityConfig
* 功能描述: 配置TIM通道2极性
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_OCPolarity: 指定的 OC2 极性
*                    这个参数可以取下面的值之一:
*                       - TIM_OCPolarity_High: 输出极性高
*                       - TIM_OCPolarity_Low: 输出极性低
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC2PolarityConfig(TIM_TypeDef* TIMx, u16 TIM_OCPolarity)
{
  u16 tmpccer = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));

  tmpccer = TIMx->CCER;

  /* Set or Reset the CC2P Bit [置位或复位CC2P]*/
  tmpccer &= CCER_CC2P_Reset;
  tmpccer |= (u16)(TIM_OCPolarity << 4);

  /* Write to TIMx CCER register [写TIMx CCER寄存器]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TIM_OC2NPolarityConfig
* 功能描述: 配置TIM通道2N极性
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)TIM_OCPolarity: 指定的 OC2N极性
*                    这个参数可以取下面的值之一:
*                       - TIM_OCNPolarity_High: 输出极性高
*                       - TIM_OCNPolarity_Low: 输出极性低
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC2NPolarityConfig(TIM_TypeDef* TIMx, u16 TIM_OCNPolarity)
{
  u16 tmpccer = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
  
  tmpccer = TIMx->CCER;

  /* Set or Reset the CC2NP Bit [置位或复位CC2NP]*/
  tmpccer &= CCER_CC2NP_Reset;
  tmpccer |= (u16)(TIM_OCNPolarity << 4);

  /* Write to TIMx CCER register [写TIMx CCER寄存器]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TIM_OC3PolarityConfig
* 功能描述: 配置TIM通道3极性
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_OCPolarity: 指定的 OC3 极性
*                    这个参数可以取下面的值之一:
*                       - TIM_OCPolarity_High: 输出极性高
*                       - TIM_OCPolarity_Low: 输出极性低
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC3PolarityConfig(TIM_TypeDef* TIMx, u16 TIM_OCPolarity)
{
  u16 tmpccer = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));

  tmpccer = TIMx->CCER;

  /* Set or Reset the CC3P Bit [置位或复位CC3P]*/
  tmpccer &= CCER_CC3P_Reset;
  tmpccer |= (u16)(TIM_OCPolarity << 8);

  /* Write to TIMx CCER register [写TIMx CCER寄存器]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TIM_OC3NPolarityConfig
* 功能描述: 配置TIM通道3N极性
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)TIM_OCPolarity: 指定的 OC3N 极性
*                    这个参数可以取下面的值之一:
*                       - TIM_OCNPolarity_High: 输出极性高
*                       - TIM_OCNPolarity_Low: 输出极性低
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC3NPolarityConfig(TIM_TypeDef* TIMx, u16 TIM_OCNPolarity)
{
  u16 tmpccer = 0;
 
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
    
  tmpccer = TIMx->CCER;

  /* Set or Reset the CC3NP Bit [置位或复位CC3NP]*/
  tmpccer &= CCER_CC3NP_Reset;
  tmpccer |= (u16)(TIM_OCNPolarity << 8);

  /* Write to TIMx CCER register [写TIMx CCER寄存器]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TIM_OC4PolarityConfig
* 功能描述: 配置TIM通道1极性
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_OCPolarity: 指定的 OC4 极性
*                    这个参数可以取下面的值之一:
*                       - TIM_OCPolarity_High: 输出极性高
*                       - TIM_OCPolarity_Low: 输出极性低
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_OC4PolarityConfig(TIM_TypeDef* TIMx, u16 TIM_OCPolarity)
{
  u16 tmpccer = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));

  tmpccer = TIMx->CCER;

  /* Set or Reset the CC4P Bit [置位或复位CC4P]*/
  tmpccer &= CCER_CC4P_Reset;
  tmpccer |= (u16)(TIM_OCPolarity << 12);

  /* Write to TIMx CCER register [写TIMx CCER寄存器]*/
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TIM_CCxCmd
* 功能描述: 使能或禁用TIM捕获比较通道x
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_Channel: 指定的TIM通道
*                    这个参数可以取下面的值之一:
*                       - TIM_Channel_1: TIM通道1
*                       - TIM_Channel_2: TIM通道2
*                       - TIM_Channel_3: TIM通道3
*                       - TIM_Channel_4: TIM通道4
*           (3)Newstate：指明TIM1通道CCxE位新状态.这个参数能够取：ENABLE或者DISABLE 
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_CCxCmd(TIM_TypeDef* TIMx, u16 TIM_Channel, u16 TIM_CCx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_CCX(TIM_CCx));

  /* Reset the CCxE Bit [置位CCxE]*/
  TIMx->CCER &= (u16)(~((u16)(CCER_CCE_Set << TIM_Channel)));

  /* Set or reset the CCxE Bit [置位或复位CCxE]*/ 
  TIMx->CCER |=  (u16)(TIM_CCx << TIM_Channel);
}

/*******************************************************************************
* 函数名称: TIM_CCxNCmd
* 功能描述: 使能或禁用TIM捕获比较通道xN
* 输入参数:(1)TIMx:此处x可以是1到8以选择外设.
*          (2)TIM_Channel: 指定的TIM通道
*                    这个参数可以取下面的值之一::
*                       - TIM_Channel_1: TIM通道1
*                       - TIM_Channel_2: TIM通道2
*                       - TIM_Channel_3: TIM通道3
*           (3)Newstate：指明TIM1通道CCxNE位新状态.这个参数能够取：ENABLE或者DISABLE 
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_CCxNCmd(TIM_TypeDef* TIMx, u16 TIM_Channel, u16 TIM_CCxN)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_COMPLEMENTARY_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_CCXN(TIM_CCxN));

  /* Reset the CCxNE Bit [置位CCxNE]*/
  TIMx->CCER &= (u16)(~((u16)(CCER_CCNE_Set << TIM_Channel)));

  /* Set or reset the CCxNE Bit [置位或复位CCxNE]*/ 
  TIMx->CCER |=  (u16)(TIM_CCxN << TIM_Channel);
}

/*******************************************************************************
* 函数名称: TIM_SelectOCxM
* 功能描述: 选择TIM输出比较模式。
*                  在改变输出比较模式之前，该函数禁用选择的通道，用户需要使用 
*                  TIM1_CCxCmd和TIM1_CCxNCmd函数使能通道。
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_Channel: 指定的TIM通道
*                    这个参数可以取下面的值之一:
*                       - TIM_Channel_1: TIM通道1
*                       - TIM_Channel_2: TIM通道2
*                       - TIM_Channel_3: TIM通道3
*                       - TIM_Channel_4: TIM通道4
*           (2)TIM_OCMode: 指定的TIM输出比较模式.
*                    这个参数可以取下面的值之一:
*                       - TIM_OCMode_Timing
*                       - TIM_OCMode_Active
*                       - TIM_OCMode_Toggle
*                       - TIM_OCMode_PWM1
*                       - TIM_OCMode_PWM2
*                       - TIM_ForcedAction_Active
*                       - TIM_ForcedAction_InActive
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SelectOCxM(TIM_TypeDef* TIMx, u16 TIM_Channel, u16 TIM_OCMode)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_OCM(TIM_OCMode));
  
  /* Disable the Channel: Reset the CCxE Bit [禁止通道：复位CCxE]*/
  TIMx->CCER &= (u16)(~((u16)(CCER_CCE_Set << TIM_Channel)));

  if((TIM_Channel == TIM_Channel_1) ||(TIM_Channel == TIM_Channel_3))
  {
    /* Reset the OCxM bits in the CCMRx register [复位CCMRx寄存器中的OCxM]*/
    *((vu32 *)((*(u32*)&TIMx) + CCMR_Offset + (TIM_Channel>>1))) &= CCMR_OC13M_Mask;
   
    /* Configure the OCxM bits in the CCMRx register [配置CCMRx寄存器中的OCxM]*/
    *((vu32 *)((*(u32*)&TIMx) + CCMR_Offset + (TIM_Channel>>1))) = TIM_OCMode;

  }
  else
  {
    /* Reset the OCxM bits in the CCMRx register [复位CCMRx寄存器中的OCxM]*/
    *((vu32 *)((*(u32*)&TIMx) + CCMR_Offset + ((u16)(TIM_Channel - 4)>> 1))) &= CCMR_OC24M_Mask;
    
    /* Configure the OCxM bits in the CCMRx register [配置CCMRx寄存器中的OCxM]*/
    *((vu32 *)((*(u32*)&TIMx) + CCMR_Offset + ((u16)(TIM_Channel - 4)>> 1))) = (u16)(TIM_OCMode << 8);
  }
}

/*******************************************************************************
* 函数名称: TIM_UpdateDisableConfig
* 功能描述: 使能或关闭TIM更新事件
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)NewState: TIMx_CR1寄存器中UDIS位的新状态.这个参数可以是ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_UpdateDisableConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the Update Disable Bit [置位更新禁止位]*/
    TIMx->CR1 |= CR1_UDIS_Set;
  }
  else
  {
    /* Reset the Update Disable Bit [复位更新禁止位]*/
    TIMx->CR1 &= CR1_UDIS_Reset;
  }
}

/*******************************************************************************
* 函数名称: TIM_UpdateRequestConfig
* 功能描述: 配置某个TIM的更新请求中断源.
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)TIM_UpdateSource:更新请求源.
*                    这个参数可以取下面的值之一:
*                       - TIM_UpdateSource_Regular
*                       - TIM_UpdateSource_Global
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_UpdateRequestConfig(TIM_TypeDef* TIMx, u16 TIM_UpdateSource)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_UPDATE_SOURCE(TIM_UpdateSource));

  if (TIM_UpdateSource != TIM_UpdateSource_Global)
  {
    /* Set the URS Bit [置位URS]*/
    TIMx->CR1 |= CR1_URS_Set;
  }
  else
  {
    /* Reset the URS Bit [复位URS]*/
    TIMx->CR1 &= CR1_URS_Reset;
  }
}

/*******************************************************************************
* 函数名称: TIM_SelectHallSensor
* 功能描述: 使能或关闭某个TIMx的霍尔感应器接口.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)NewState:霍尔感应器的新状态.这个参数可以是ENABLE或DISABLE。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SelectHallSensor(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the TI1S Bit [置位TI1S]*/
    TIMx->CR2 |= CR2_TI1S_Set;
  }
  else
  {
    /* Reset the TI1S Bit [复位TI1S]*/
    TIMx->CR2 &= CR2_TI1S_Reset;
  }
}

/*******************************************************************************
* 函数名称: TIM_SelectOnePulseMode
* 功能描述: 选择某个TIMx为单脉冲模式
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)TIM_OPMode:指定使用单脉冲模式.
*                    这个参数可以取下面的值之一:
*                       - TIM_OPMode_Single
*                       - TIM_OPMode_Repetitive
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SelectOnePulseMode(TIM_TypeDef* TIMx, u16 TIM_OPMode)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OPM_MODE(TIM_OPMode));

  /* Reset the OPM Bit [复位OPM]*/
  TIMx->CR1 &= CR1_OPM_Reset;

  /* Configure the OPM Mode [配置OPM模式]*/
  TIMx->CR1 |= TIM_OPMode;
}

/*******************************************************************************
* 函数名称: TIM_SelectOutputTrigger
* 功能描述: 选择某个TIMx的触发器输出模式
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)TIM_TRGOSource:触发器输出源.
*                    这个参数可以是下面的值:
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
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx, u16 TIM_TRGOSource)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_TRGO_SOURCE(TIM_TRGOSource));
  assert_param(IS_TIM_PERIPH_TRGO(TIMx, TIM_TRGOSource));

  /* Reset the MMS Bits [复位MMS]*/
  TIMx->CR2 &= CR2_MMS_Mask;

  /* Select the TRGO source [选择TRGO源]*/
  TIMx->CR2 |=  TIM_TRGOSource;
}

/*******************************************************************************
* 函数名称: TIM_SelectSlaveMode
* 功能描述: 选择某个TIMx为从模式.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_SlaveMode:指定的TIM从模式.
*                    这个参数可以取下面的值之一:
*                       - TIM_SlaveMode_Reset
*                       - TIM_SlaveMode_Gated
*                       - TIM_SlaveMode_Trigger
*                       - TIM_SlaveMode_External1
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, u16 TIM_SlaveMode)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_SLAVE_MODE(TIM_SlaveMode));

  /* Reset the SMS Bits [复位SMS]*/
  TIMx->SMCR &= SMCR_SMS_Mask;

  /* Select the Slave Mode [选择从模式]*/
  TIMx->SMCR |= TIM_SlaveMode;
}

/*******************************************************************************
* 函数名称: TIM_SelectMasterSlaveMode
* 功能描述: 设置/重置TIM为主/从模式
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_MasterSlaveMode:计时器主从模式.
*                    这个参数可以取下面的值之一:
*                       - TIM_MasterSlaveMode_Enable: 使能主从模式.
*                       - TIM_MasterSlaveMode_Disable: 没有作用
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SelectMasterSlaveMode(TIM_TypeDef* TIMx, u16 TIM_MasterSlaveMode)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_MSM_STATE(TIM_MasterSlaveMode));

  /* Reset the MSM Bit [复位MSM]*/
  TIMx->SMCR &= SMCR_MSM_Reset;
  
  /* Set or Reset the MSM Bit [置位或复位MSM]*/
  TIMx->SMCR |= TIM_MasterSlaveMode;
}

/*******************************************************************************
* 函数名称: TIM_SetCounter
* 功能描述: 设置某个TIM计数器寄存器的值
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)Counter:计数器寄存器的新值
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SetCounter(TIM_TypeDef* TIMx, u16 Counter)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));

  /* Set the Counter Register value [设置计数器寄存器值]*/
  TIMx->CNT = Counter;
}

/*******************************************************************************
* 函数名称: TIM_SetAutoreload
* 功能描述: 设置某个TIMx自动重载寄存器的值
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)Autoreload:自动重载寄存器的新值.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SetAutoreload(TIM_TypeDef* TIMx, u16 Autoreload)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));

  /* Set the Autoreload Register value [设置自动装载寄存器值]*/
  TIMx->ARR = Autoreload;
}

/*******************************************************************************
* 函数名称: TIM_SetCompare1
* 功能描述: 设置某个TIMx捕获比较1寄存器的值
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)Compare1: 捕获比较1寄存器的新值
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SetCompare1(TIM_TypeDef* TIMx, u16 Compare1)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Set the Capture Compare1 Register value [设置捕获比较1寄存器的值]*/
  TIMx->CCR1 = Compare1;
}

/*******************************************************************************
* 函数名称: TIM_SetCompare2
* 功能描述: 设置某个TIMx捕获比较2寄存器的值
* 输入参数:  (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*            (2)Compare2: 捕获比较2寄存器的新值
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SetCompare2(TIM_TypeDef* TIMx, u16 Compare2)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Set the Capture Compare2 Register value [设置捕获比较2寄存器的值]*/
  TIMx->CCR2 = Compare2;
}

/*******************************************************************************
* 函数名称: TIM_SetCompare3
* 功能描述: 设置某个TIMx捕获比较3寄存器的值
* 输入参数:  (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*            (2)Compare3: 捕获比较3寄存器的新值
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SetCompare3(TIM_TypeDef* TIMx, u16 Compare3)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Set the Capture Compare3 Register value [设置捕获比较3寄存器的值]*/
  TIMx->CCR3 = Compare3;
}

/*******************************************************************************
* 函数名称: TIM_SetCompare4
* 功能描述: 设置某个TIMx捕获比较4寄存器的值
* 输入参数:  (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*            (2)Compare4:捕获比较4寄存器的新值
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SetCompare4(TIM_TypeDef* TIMx, u16 Compare4)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Set the Capture Compare4 Register value [设置捕获比较4寄存器的值]*/
  TIMx->CCR4 = Compare4;
}

/*******************************************************************************
* 函数名称: TIM_SetIC1Prescaler
* 功能描述: 设置某个TIMx输入捕获1的预分频数
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_ICPSC:输入捕获1的预分频数.
*                    这个参数可以取下面的值之一:
*                       - TIM_ICPSC_DIV1: 没有预分频数
*                       - TIM_ICPSC_DIV2: 每2次事件TIM捕获执行一次
*                       - TIM_ICPSC_DIV4: 每4次事件TIM捕获执行一次
*                       - TIM_ICPSC_DIV8: 每8次事件TIM捕获执行一次
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SetIC1Prescaler(TIM_TypeDef* TIMx, u16 TIM_ICPSC)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));

  /* Reset the IC1PSC Bits [复位IC1PSC]*/
  TIMx->CCMR1 &= CCMR_IC13PSC_Mask;

  /* Set the IC1PSC value [设置IC1PSC值]*/
  TIMx->CCMR1 |= TIM_ICPSC;
}

/*******************************************************************************
* 函数名称: TIM_SetIC2Prescaler
* 功能描述: 设置某个TIMx输入捕获2的预分频数
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_ICPSC:输入捕获2的预分频数.
*                    这个参数可以取下面的值之一:
*                       - TIM_ICPSC_DIV1: 没有预分频数
*                       - TIM_ICPSC_DIV2: 每2次事件TIM捕获执行一次
*                       - TIM_ICPSC_DIV4: 每4次事件TIM捕获执行一次
*                       - TIM_ICPSC_DIV8: 每8次事件TIM捕获执行一次
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SetIC2Prescaler(TIM_TypeDef* TIMx, u16 TIM_ICPSC)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));

  /* Reset the IC2PSC Bits [复位IC2PSC]*/
  TIMx->CCMR1 &= CCMR_IC24PSC_Mask;

  /* Set the IC2PSC value [设置IC2PSC值]*/
  TIMx->CCMR1 |= (u16)(TIM_ICPSC << 8);
}

/*******************************************************************************
* 函数名称: TIM_SetIC3Prescaler
* 功能描述: 设置某个TIMx输入捕获3的预分频数
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_ICPSC:输入捕获3的预分频数.
*                    这个参数可以取下面的值之一:
*                       - TIM_ICPSC_DIV1: 没有预分频数
*                       - TIM_ICPSC_DIV2: 每2次事件TIM捕获执行一次
*                       - TIM_ICPSC_DIV4: 每4次事件TIM捕获执行一次
*                       - TIM_ICPSC_DIV8: 每8次事件TIM捕获执行一次
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SetIC3Prescaler(TIM_TypeDef* TIMx, u16 TIM_ICPSC)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));

  /* Reset the IC3PSC Bits [复位IC3PSC]*/
  TIMx->CCMR2 &= CCMR_IC13PSC_Mask;

  /* Set the IC3PSC value [设置IC3PSC值]*/
  TIMx->CCMR2 |= TIM_ICPSC;
}

/*******************************************************************************
* 函数名称: TIM_SetIC4Prescaler
* 功能描述: 设置某个TIMx输入捕获4的预分频数
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_ICPSC:输入捕获4的预分频数.
*                    这个参数可以取下面的值之一:
*                      - TIM_ICPSC_DIV1: 没有预分频数
*                      - TIM_ICPSC_DIV2: 每2次事件TIM捕获执行一次
*                      - TIM_ICPSC_DIV4: 每4次事件TIM捕获执行一次
*                      - TIM_ICPSC_DIV8: 每8次事件TIM捕获执行一次
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SetIC4Prescaler(TIM_TypeDef* TIMx, u16 TIM_ICPSC)
{  
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));

  /* Reset the IC4PSC Bits [复位IC4PSC]*/
  TIMx->CCMR2 &= CCMR_IC24PSC_Mask;

  /* Set the IC4PSC value [设置IC4PSC值]*/
  TIMx->CCMR2 |= (u16)(TIM_ICPSC << 8);
}

/*******************************************************************************
* 函数名称: TIM_SetClockDivision
* 功能描述: 设置某个TIMx时钟分割值
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_CKD:时钟分割值.
*                    这个参数可以取下面的值之一:
*                       - TIM_CKD_DIV1: TDTS = Tck_tim
*                       - TIM_CKD_DIV2: TDTS = 2*Tck_tim
*                       - TIM_CKD_DIV4: TDTS = 4*Tck_tim
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_SetClockDivision(TIM_TypeDef* TIMx, u16 TIM_CKD)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CKD_DIV(TIM_CKD));

  /* Reset the CKD Bits [复位CKD]*/
  TIMx->CR1 &= CR1_CKD_Mask;

  /* Set the CKD value [设置CKD值]*/
  TIMx->CR1 |= TIM_CKD;
}
/*******************************************************************************
* 函数名称: TIM_GetCapture1
* 功能描述: 获取某个TIMx的输入捕获1的值
* 输入参数: TIMx:此处x可以是1,2,3,4,5或8以选择外设.
* 输出参数: 无
* 返回参数: 比较捕获寄存器1的值.
*******************************************************************************/
u16 TIM_GetCapture1(TIM_TypeDef* TIMx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Get the Capture 1 Register value [取得捕获寄存器1的值]*/
  return TIMx->CCR1;
}

/*******************************************************************************
* 函数名称: TIM_GetCapture2
* 功能描述: 获取某个TIMx的输入捕获2的值
* 输入参数: TIMx:此处x可以是1,2,3,4,5或8以选择外设.
* 输出参数: 无
* 返回参数: 比较捕获寄存器2的值。
*******************************************************************************/
u16 TIM_GetCapture2(TIM_TypeDef* TIMx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Get the Capture 2 Register value [取得捕获寄存器2的值]*/
  return TIMx->CCR2;
}

/*******************************************************************************
* 函数名称: TIM_GetCapture3
* 功能描述: 获取某个TIMx的输入捕获3的值
* 输入参数: TIMx:此处x可以是1,2,3,4,5或8以选择外设.
* 输出参数: 无
* 返回参数: 比较捕获寄存器3的值。
*******************************************************************************/
u16 TIM_GetCapture3(TIM_TypeDef* TIMx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 

  /* Get the Capture 3 Register value [取得捕获寄存器3的值]*/
  return TIMx->CCR3;
}

/*******************************************************************************
* 函数名称: TIM_GetCapture4
* 功能描述: 获取某个TIMx的输入捕获4的值
* 输入参数: TIMx:此处x可以是1,2,3,4,5或8以选择外设.
* 输出参数: 无
* 返回参数: 比较捕获寄存器4的值。
*******************************************************************************/
u16 TIM_GetCapture4(TIM_TypeDef* TIMx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_123458_PERIPH(TIMx));

  /* Get the Capture 4 Register value [取得捕获寄存器4的值]*/
  return TIMx->CCR4;
}

/*******************************************************************************
* 函数名称: TIM_GetCounter
* 功能描述: 取得TIMx计数寄存器的值.
* 输入参数: TIMx:此处x可以是1到8以选择外设.
* 输出参数: 无
* 返回参数: 计数寄存器的值.
*******************************************************************************/
u16 TIM_GetCounter(TIM_TypeDef* TIMx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));

  /* Get the Counter Register value [取得计数寄存器的值]*/
  return TIMx->CNT;
}

/*******************************************************************************
* 函数名称: TIM_GetPrescaler
* 功能描述: 取得TIMx预分频的值.
* 输入参数: TIMx:此处x可以是1到8以选择外设.
* 输出参数: 无
* 返回参数: 预分频寄存器的值.
*******************************************************************************/
u16 TIM_GetPrescaler(TIM_TypeDef* TIMx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));

  /* Get the Prescaler Register value [取得预分频寄存器的值]*/
  return TIMx->PSC;
}

/*******************************************************************************
* 函数名称: TIM_GetFlagStatus
* 功能描述: 检查是否特定的TIM标记被设置.
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)TIM_FLAG:用于检查的标记.
*                    这个参数可以取下面的值之一:
*                       - TIM_FLAG_Update: TIM更新标记
*                       - TIM_FLAG_CC1: TIM捕获比较1标记
*                       - TIM_FLAG_CC2: TIM捕获比较2标记
*                       - TIM_FLAG_CC3: TIM捕获比较3标记
*                       - TIM_FLAG_CC4: TIM捕获比较4标记
*                       - TIM_FLAG_COM: TIM交换标记
*                       - TIM_FLAG_Trigger: TIM触发器标记
*                       - TIM_FLAG_Break: TIM停顿标记
*                       - TIM_FLAG_CC1OF: TIM捕获比较1溢出标记
*                       - TIM_FLAG_CC2OF: TIM捕获比较2溢出标记
*                       - TIM_FLAG_CC3OF: TIM捕获比较3溢出标记
*                       - TIM_FLAG_CC4OF: TIM捕获比较4溢出标记
* 输出参数: 无
* 返回参数: TIM_FLAG的新状态 (SET or RESET).
*******************************************************************************/
FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx, u16 TIM_FLAG)
{ 
  ITStatus bitstatus = RESET;  

  /* Check the parameters [检查参数]*/
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
* 函数名称: TIM_ClearFlag
* 功能描述: 清除TIMx的挂起标记
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)TIM_FLAG: 用于清除的标记.
*                    这个参数可以是下面值的任意组合:
*                       - TIM_FLAG_Update: TIM更新标记
*                       - TIM_FLAG_CC1: TIM捕获比较1标记
*                       - TIM_FLAG_CC2: TIM捕获比较2标记
*                       - TIM_FLAG_CC3: TIM捕获比较3标记
*                       - TIM_FLAG_CC4: TIM捕获比较4标记
*                       - TIM_FLAG_COM: TIM交换标记
*                       - TIM_FLAG_Trigger: TIM触发器标记
*                       - TIM_FLAG_Break: TIM停顿标记
*                       - TIM_FLAG_CC1OF: TIM捕获比较1溢出标记
*                       - TIM_FLAG_CC2OF: TIM捕获比较2溢出标记
*                       - TIM_FLAG_CC3OF: TIM捕获比较3溢出标记
*                       - TIM_FLAG_CC4OF: TIM捕获比较4溢出标记
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ClearFlag(TIM_TypeDef* TIMx, u16 TIM_FLAG)
{  
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_CLEAR_FLAG(TIMx, TIM_FLAG));
   
  /* Clear the flags [清除标志位]*/
  TIMx->SR = (u16)~TIM_FLAG;
}

/*******************************************************************************
* 函数名称: TIM_GetITStatus
* 功能描述: 检查特定TIMx中断是否发生.
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)TIM_ IT:规定要检查的TIM中断源.
*                    这个参数可以取下面的值之一:
*                       - TIM_IT_Update: TIM更新中断源
*                       - TIM_IT_CC1: TIM 捕获比较1中断源
*                       - TIM_IT_CC2: TIM 捕获比较2中断源
*                       - TIM_IT_CC3: TIM 捕获比较3中断源
*                       - TIM_IT_CC4: TIM 捕获比较4中断源
*                       - TIM_IT_COM: TIM 交换中断源
*                       - TIM_IT_Trigger: TIM触发中断源
*                       - TIM_IT_Break: TIM停顿中断源
* 输出参数: 无
* 返回参数: TIM_IT的新状态(SET or RESET).
*******************************************************************************/
ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, u16 TIM_IT)
{
  ITStatus bitstatus = RESET;  
  u16 itstatus = 0x0, itenable = 0x0;

  /* Check the parameters [检查参数]*/
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
* 函数名称: TIM_ClearITPendingBit
* 功能描述: 清除TIMx的中断挂起位
* 输入参数: (1)TIMx:此处x可以是1到8以选择外设.
*           (2)TIM_ IT:指定要清除的中断挂起位.
*                    这个参数可以是下面值的任意组合:
*                       - TIM_IT_Update: TIM更新中断源
*                       - TIM_IT_CC1: TIM 捕获比较1中断源
*                       - TIM_IT_CC2: TIM 捕获比较2中断源
*                       - TIM_IT_CC3: TIM 捕获比较3中断源
*                       - TIM_IT_CC4: TIM 捕获比较4中断源
*                       - TIM_IT_COM: TIM 交换中断源
*                       - TIM_IT_Trigger: TIM触发中断源
*                       - TIM_IT_Break: TIM停顿中断源
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, u16 TIM_IT)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_PERIPH_IT(TIMx, TIM_IT));

  /* Clear the IT pending Bit [清除IT挂起位]*/
  TIMx->SR = (u16)~TIM_IT;
}

/*******************************************************************************
* 函数名称: TI1_Config
* 功能描述: 配置TI1作为输入.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_ICPolarity : 输入极性.
*                    这个参数可以取下面的值之一:
*                       - TIM_ICPolarity_Rising
*                       - TIM_ICPolarity_Falling
*           (3)TIM_ICSelection: 使用指定的输入.
*                    这个参数可以取下面的值之一:
*                       - TIM_ICSelection_DirectTI: 选择TIM 输入1连接IC1.
*                       - TIM_ICSelection_IndirectTI: 选择TIM 输入1连接IC2.
*                       - TIM_ICSelection_TRC: 选择TIM 输入1连接TRC.
*           (4)TIM_ICFilter: 指定的输入捕获滤波器.
*                    这个参数的值必须在 0x00 和 0x0F之间.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
static void TI1_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter)
{
  u16 tmpccmr1 = 0, tmpccer = 0;

  /* Disable the Channel 1: Reset the CC1E Bit [禁止通道1：复位CC1E位]*/
  TIMx->CCER &= CCER_CC1E_Reset;

  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;

  /* Select the Input and set the filter [选择输入和滤波器]*/
  tmpccmr1 &= CCMR_CC13S_Mask & CCMR_IC13F_Mask;
  tmpccmr1 |= TIM_ICSelection | (u16)(TIM_ICFilter << 4);

  /* Select the Polarity and set the CC1E Bit [选择极性和置位CC1E]*/
  tmpccer &= CCER_CC1P_Reset;
  tmpccer |= TIM_ICPolarity | CCER_CC1E_Set;

  /* Write to TIMx CCMR1 and CCER registers [写TIMx CCMR1和CCER寄存器]*/
  TIMx->CCMR1 = tmpccmr1;
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TI2_Config
* 功能描述: Configure the TI2 as Input.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_ICPolarity : 输入极性.
*                    这个参数可以取下面的值之一:
*                       - TIM_ICPolarity_Rising
*                       - TIM_ICPolarity_Falling
*           (3)TIM_ICSelection: 使用指定的输入.
*                    这个参数可以取下面的值之一:
*                       - TIM_ICSelection_DirectTI: 选择TIM 输入2连接IC2.
*                       - TIM_ICSelection_IndirectTI: 选择TIM 输入2连接IC1.
*                       - TIM_ICSelection_TRC: 选择TIM 输入2连接TRC.
*           (4)TIM_ICFilter: 指定的输入捕获滤波器.
*                    这个参数的值必须在 0x00 和 0x0F之间.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
static void TI2_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter)
{
  u16 tmpccmr1 = 0, tmpccer = 0, tmp = 0;

  /* Disable the Channel 2: Reset the CC2E Bit [禁止通道2：复位CC2E位]*/
  TIMx->CCER &= CCER_CC2E_Reset;

  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;
  tmp = (u16)(TIM_ICPolarity << 4);

  /* Select the Input and set the filter [选择输入和滤波器]*/
  tmpccmr1 &= CCMR_CC24S_Mask & CCMR_IC24F_Mask;
  tmpccmr1 |= (u16)(TIM_ICFilter << 12);
  tmpccmr1 |= (u16)(TIM_ICSelection << 8);

  /* Select the Polarity and set the CC2E Bit [选择极性和置位CC2E]*/
  tmpccer &= CCER_CC2P_Reset;
  tmpccer |=  tmp | CCER_CC2E_Set;

  /* Write to TIMx CCMR1 and CCER registers [写TIMx CCMR1和CCER寄存器]*/
  TIMx->CCMR1 = tmpccmr1 ;
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TI3_Config
* 功能描述: Configure the TI3 as Input.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_ICPolarity : 输入极性.
*                    这个参数可以取下面的值之一:
*                       - TIM_ICPolarity_Rising
*                       - TIM_ICPolarity_Falling
*           (3)TIM_ICSelection: 使用指定的输入.
*                    这个参数可以取下面的值之一:
*                       - TIM_ICSelection_DirectTI: 选择TIM 输入3连接IC3.
*                       - TIM_ICSelection_IndirectTI: 选择TIM 输入3连接IC4.
*                       - TIM_ICSelection_TRC: 选择TIM 输入3连接TRC.
*           (4)TIM_ICFilter: 指定的输入捕获滤波器.
*                    这个参数的值必须在 0x00 和 0x0F之间.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
static void TI3_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter)
{
  u16 tmpccmr2 = 0, tmpccer = 0, tmp = 0;

  /* Disable the Channel 3: Reset the CC3E Bit [禁止通道3：复位CC3E位]*/
  TIMx->CCER &= CCER_CC3E_Reset;

  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (u16)(TIM_ICPolarity << 8);

  /* Select the Input and set the filter [选择输入和滤波器]*/
  tmpccmr2 &= CCMR_CC13S_Mask & CCMR_IC13F_Mask;
  tmpccmr2 |= TIM_ICSelection | (u16)(TIM_ICFilter << 4);

  /* Select the Polarity and set the CC3E Bit [选择极性和置位CC3E]*/
  tmpccer &= CCER_CC3P_Reset;
  tmpccer |= tmp | CCER_CC3E_Set;

  /* Write to TIMx CCMR2 and CCER registers [写TIMx CCMR2和CCER寄存器]*/
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* 函数名称: TI4_Config
* 功能描述: Configure the TI1 as Input.
* 输入参数: (1)TIMx:此处x可以是1,2,3,4,5或8以选择外设.
*           (2)TIM_ICPolarity : 输入极性.
*                    这个参数可以取下面的值之一:
*                       - TIM_ICPolarity_Rising
*                       - TIM_ICPolarity_Falling
*           (3)TIM_ICSelection: 使用指定的输入.
*                    这个参数可以取下面的值之一:
*                       - TIM_ICSelection_DirectTI: 选择TIM 输入4连接IC4.
*                       - TIM_ICSelection_IndirectTI: 选择TIM 输入4连接IC3.
*                       - TIM_ICSelection_TRC: 选择TIM 输入4连接TRC.
*           (4)TIM_ICFilter: 指定的输入捕获滤波器.
*                    这个参数的值必须在 0x00 和 0x0F之间.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
static void TI4_Config(TIM_TypeDef* TIMx, u16 TIM_ICPolarity, u16 TIM_ICSelection,
                       u16 TIM_ICFilter)
{
  u16 tmpccmr2 = 0, tmpccer = 0, tmp = 0;

  /* Disable the Channel 4: Reset the CC4E Bit [禁用通道4：复位CC4E]*/
  TIMx->CCER &= CCER_CC4E_Reset;

  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (u16)(TIM_ICPolarity << 12);

  /* Select the Input and set the filter [选择输入和设置过滤器]*/
  tmpccmr2 &= CCMR_CC24S_Mask & CCMR_IC24F_Mask;
  tmpccmr2 |= (u16)(TIM_ICSelection << 8) | (u16)(TIM_ICFilter << 12);

  /* Select the Polarity and set the CC4E Bit [选择极性和置位CC4E]*/
  tmpccer &= CCER_CC4P_Reset;
  tmpccer |= tmp | CCER_CC4E_Set;

  /* Write to TIMx CCMR2 and CCER registers [写TIMx CCMR2和CCER寄存器]*/
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer ;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
