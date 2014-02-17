/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* ÎÄ¼şÃû³Æ: stm32f10x_adc.c
* ³ÌĞò×÷Õß: MCD Application Team
* ³ÌĞò°æ±¾: V2.0.2
* ±àÖÆÈÕÆÚ: 07/11/2008
* ¹¦ÄÜÃèÊö: Õâ¸öÎÄ¼şÌá¹©ÁËËùÓĞADCµÄ¹Ì¼şº¯Êı.
********************************************************************************
* Ä¿Ç°µÄ¹Ì¼şµÄÄ¿µÄÊÇÎª¿Í»§Ìá¹©¹ØÓÚËûÃÇµÄ²úÆ·µÄ±àÂëĞÅÏ¢ÒÔ±ãÓÚ½ÚÊ¡ËûÃÇÊ±¼ä¡£
* ½á¹û, Òâ·¨°ëµ¼Ìå½«²»»áÎª´Ë³Ğµ£ÈÎºÎÖ±½Ó£¬¼ä½Ó»òÏàÓ¦µÄËğº¦Åâ³¥·½ÃæµÄÈÎºÎË÷ÅâÒªÇó£¬
* ÀıÈç¹Ì¼ş»òÊ¹ÓÃËù×÷µÄ¿Í»§±àÂëÖĞËûÉæÏÓ°üº¬ÓëËûÃÇµÄÏà¹Ø²úÆ·µÄĞÅÏ¢¡£
*******************************************************************************/

/* °üº­µÄÎÄ¼ş ----------------------------------------------------------------*/
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"

/* ×ÔÓÃÀàĞÍ ------------------------------------------------------------------*/
/* ×ÔÓÃ¶¨Òå ------------------------------------------------------------------*/
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

/* ×ÔÓÃºê -----------------------------------------------------------*/
/* ×ÔÓÃ±äÁ¿ ---------------------------------------------------------*/
/* ×ÔÓÃº¯ÊıÔ­ĞÍ -----------------------------------------------------*/
/* ×ÔÓÃº¯Êı ---------------------------------------------------------*/

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_DeInit
* ¹¦ÄÜÃèÊö: ¸´Î»ADCxÍâÎ§Ä£¿é¼Ä´æÆ÷£¬Ê¹ÆäÎªÄ¬ÈÏÖµ¡£
* ÊäÈë²ÎÊı: ADCx: ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_DeInit(ADC_TypeDef* ADCx)
{
  /* Check the parameters [¼ì²é²ÎÊı ¼ì²é²ÎÊı]*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  switch (*(u32*)&ADCx)
  {
    case ADC1_BASE:
      /* Enable ADC1 reset state [ÔÊĞíADC1¸´Î»×´Ì¬]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
      /* Release ADC1 from reset state [½â³ıADC1¸´Î»×´Ì¬]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
      break;
    
    case ADC2_BASE:
      /* Enable ADC2 reset state [ÔÊĞíADC2¸´Î»×´Ì¬]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);
      /* Release ADC2 from reset state [½â³ıADC2¸´Î»×´Ì¬]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);
      break;
      
    case ADC3_BASE:
      /* Enable ADC3 reset state [ÔÊĞíADC3¸´Î»×´Ì¬]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);
      /* Release ADC3 from reset state [½â³ıADC3¸´Î»×´Ì¬]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);
      break; 

    default:
      break;
  }
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_Init
* ¹¦ÄÜÃèÊö: ¸ù¾İADC_InitStruct ÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ADCx ÍâÎ§Ä£¿é¡£
* ÊäÈë²ÎÊı: (1)ADCx: ÆäÖĞx ¿ÉÒÔÊÇ1 2»ò3£¬ÓÃÀ´Ñ¡ÔñADC ÍâÎ§Ä£¿é.
*           (2)ADC_InitStruc£ºÖ¸Ïò½á¹¹ÌåADC_InitTypeDef µÄÖ¸Õë,¸Ã½á¹¹°üÀ¨ÁËÖ¸¶¨ADCÍâÎ§Ä£¿éµÄÅäÖÃĞÅÏ¢¡£
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
******************************************************************************/
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
{
  u32 tmpreg1 = 0;
  u8 tmpreg2 = 0;

  /* Check the parameters [¼ì²é²ÎÊı ¼ì²é²ÎÊı]*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_MODE(ADC_InitStruct->ADC_Mode));
  assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ScanConvMode));
  assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ContinuousConvMode));            
  assert_param(IS_ADC_EXT_TRIG(ADC_InitStruct->ADC_ExternalTrigConv));   
  assert_param(IS_ADC_DATA_ALIGN(ADC_InitStruct->ADC_DataAlign)); 
  assert_param(IS_ADC_REGULAR_LENGTH(ADC_InitStruct->ADC_NbrOfChannel));

  /*---------------------------- ADCx CR1 Configuration [ADCx CR1ÅäÖÃ]-----------------*/
  /* Get the ADCx CR1 value [µÃµ½ADCx CR1µÄÖµ]*/
  tmpreg1 = ADCx->CR1;
  /* Clear DUALMOD and SCAN bits [Çå³ıDUALMODºÍSCANÎ»]*/
  tmpreg1 &= CR1_CLEAR_Mask;
  /* Configure ADCx: Dual mode and scan conversion mode [ÅäÖÃADCx:Ë«ÖØÄ£Ê½ºÍÉ¨Ãè×ª»»Ä£Ê½]*/
  /* Set DUALMOD bits according to ADC_Mode value [ÉèÖÃDUALMODÎ»ÒÀÕÕADCÄ£Ê½Öµ]*/
  /* Set SCAN bit according to ADC_ScanConvMode value */
  tmpreg1 |= (u32)(ADC_InitStruct->ADC_Mode | ((u32)ADC_InitStruct->ADC_ScanConvMode << 8));
  /* Write to ADCx CR1 [Ğ´µ½ADCx CR1]*/
  ADCx->CR1 = tmpreg1;

  /*---------------------------- ADCx CR2 Configuration [ADCx CR2ÅäÖÃ]-----------------*/
  /* Get the ADCx CR2 value [µÃµ½ADCx CR2µÄÖµ]*/
  tmpreg1 = ADCx->CR2;
  /* Clear CONT, ALIGN and EXTSEL bits [Çå³ıCONT,ALIGNºÍEXTSELÎ»]*/
  tmpreg1 &= CR2_CLEAR_Mask;
  /* Configure ADCx: external trigger event and continuous conversion mode [ÅäÖÃADCx:´¥·¢Æ÷Ê±¼äºÍÁ¬Ğø×ª»»Ä£Ê½]*/
  /* Set ALIGN bit according to ADC_DataAlign value [ÒÀÕÕADC_DataAlignµÄÖµÉèÖÃALIGNÎ»]*/
  /* Set EXTSEL bits according to ADC_ExternalTrigConv value [ÒÀÕÕADC_ExternalTrigConvµÄÖµÉèÖÃEXTSELÎ»]*/
  /* Set CONT bit according to ADC_ContinuousConvMode value [ÒÀÕÕADC_ContinuousConvModeµÄÖµÉèÖÃCONTÎ»]*/
  tmpreg1 |= (u32)(ADC_InitStruct->ADC_DataAlign | ADC_InitStruct->ADC_ExternalTrigConv |
            ((u32)ADC_InitStruct->ADC_ContinuousConvMode << 1));
  /* Write to ADCx CR2 [Ğ´ÈëADCx CR2]*/
  ADCx->CR2 = tmpreg1;

  /*---------------------------- ADCx SQR1 Configuration [ADCx SQR1ÅäÖÃ]-----------------*/
  /* Get the ADCx SQR1 value [È¡µÃADCx SQR1µÄÖµ]*/
  tmpreg1 = ADCx->SQR1;
  /* Clear L bits [Çå³ıLÎ»]*/
  tmpreg1 &= SQR1_CLEAR_Mask;
  /* Configure ADCx: regular channel sequence length [ÅäÖÃADCx:¹æÔòĞÅµÀ³¤¶È]*/
  /* Set L bits according to ADC_NbrOfChannel value [ÒÀÕÕADC_NbrOfChannelµÄÖµÉèÖÃLÎ»]*/
  tmpreg2 |= (ADC_InitStruct->ADC_NbrOfChannel - 1);
  tmpreg1 |= ((u32)tmpreg2 << 20);
  /* Write to ADCx SQR1 [Ğ´ÈëADCx SQR1]*/
  ADCx->SQR1 = tmpreg1;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_StructInit
* ¹¦ÄÜÃèÊö: Ê¹ÓÃÄ¬ÈÏÖµ³õÊ¼»¯ADC_InitStruct¸÷³ÉÔ±±äÁ¿¡£
* ÊäÈë²ÎÊı: ADC_InitStruct:Ö¸Ïò½á¹¹ÌåADC_InitTypeDefµÄÖ¸Õë£¬ÓÃÀ´³õÊ¼»¯
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
  /* Reset ADC init structure parameters values [¸´Î»ADC³õÊ¼½á¹¹±äÁ¿]*/
  /* Initialize the ADC_Mode member [³õÊ¼»¯ADC_Mode³ÉÔ±]*/
  ADC_InitStruct->ADC_Mode = ADC_Mode_Independent;

  /* initialize the ADC_ScanConvMode member [³õÊ¼»¯ADC_ScanConvMode³ÉÔ±]*/
  ADC_InitStruct->ADC_ScanConvMode = DISABLE;

  /* Initialize the ADC_ContinuousConvMode member [³õÊ¼»¯ADC_ContinuousConvMode³ÉÔ±]*/
  ADC_InitStruct->ADC_ContinuousConvMode = DISABLE;

  /* Initialize the ADC_ExternalTrigConv member [³õÊ¼»¯ADC_ExternalTrigConve³ÉÔ±]*/
  ADC_InitStruct->ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;

  /* Initialize the ADC_DataAlign member [³õÊ¼»¯ADC_DataAlign³ÉÔ±]*/
  ADC_InitStruct->ADC_DataAlign = ADC_DataAlign_Right;

  /* Initialize the ADC_NbrOfChannel member [³õÊ¼»¯ADC_NbrOfChannel³ÉÔ±]*/
  ADC_InitStruct->ADC_NbrOfChannel = 1;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_Cmd
* ¹¦ÄÜÃèÊö: Ê¹ÄÜ/½ûÖ¹Ö¸¶¨µÄADCÍâÎ§Ä£¿é.
* ÊäÈë²ÎÊı: (1) ADCx: ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)ADCxÍâÎ§Ä£¿éµÄĞÂ×´Ì¬Õâ¸ö²ÎÊı¿ÉÒÔÊÇ£ºENABLE»òDISABLE¡£
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters ¼ì²é²ÎÊı ¼ì²é²ÎÊı*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the ADON bit to wake up the ADC from power down mode [´ÓµôµçÄ£Ê½ÖÃÎ»ADONµ½»½ĞÑ]*/
    ADCx->CR2 |= CR2_ADON_Set;
  }
  else
  {
    /* Disable the selected ADC peripheral [½ûÓÃÑ¡ÔñµÄADCÉè±¸]*/
    ADCx->CR2 &= CR2_ADON_Reset;
  }
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_DMACmd
* ¹¦ÄÜÃèÊö: Ê¹ÄÜ/½ûÖ¹Ö¸¶¨µÄADC DMAÇëÇó.
* ÊäÈë²ÎÊı: (1)ADCx: ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*                    ×¢Òâ: ADC2 Ã»ÓĞ DMA ÄÜÁ¦.
*           (2)ADC DMA×ªÒÆÆ÷µÄĞÂ×´Ì¬Õâ¸ö²ÎÊı¿ÉÒÔÊÇ£ºENABLE»òDISABLE¡£
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters ¼ì²é²ÎÊı ¼ì²é²ÎÊı*/
  assert_param(IS_ADC_DMA_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC DMA request [ÔÊĞíÑ¡ÔñµÄADC DMAÇëÇó]*/
    ADCx->CR2 |= CR2_DMA_Set;
  }
  else
  {
    /* Disable the selected ADC DMA request [½ûÓÃÑ¡ÔñµÄADC DMAÇëÇó]*/
    ADCx->CR2 &= CR2_DMA_Reset;
  }
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_ITConfig
* ¹¦ÄÜÃèÊö: Ê¹ÄÜ/½ûÖ¹Ö¸¶¨µÄADCÖĞ¶Ï.
* ÊäÈë²ÎÊı: (1)ADCx: ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)ADC_IT: Ö¸¶¨ADCÖĞ¶ÏÔ´ÊÇÊ¹ÄÜµÄ»ò½ûÖ¹µÄ.
*                    ADC_IT ÓÃÀ´Ê¹ÄÜ»òÕß½ûÖ¹ADC ÖĞ¶Ï.¿ÉÒÔÊ¹ÓÃÏÂÊöÖµµÄÒ»¸ö»òÕß¼¸¸öÖµµÄ×éºÏ:
*                       - ADC_IT_EOC: EOCÖĞ¶ÏÆÁ±Î
*                       - ADC_IT_AWD: AWDOGÖĞ¶ÏÆÁ±Î
*                       - ADC_IT_JEOC: JEOCÖĞ¶ÏÆÁ±Î
*           (3)NewState: Ö¸¶¨µÄADCÖĞ¶ÏµÄĞÂ×´Ì¬Õâ¸ö²ÎÊı¿ÉÒÔÊÇ£ºENABLE»òDISABLE
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_ITConfig(ADC_TypeDef* ADCx, u16 ADC_IT, FunctionalState NewState)
{
  u8 itmask = 0;

  /* Check the parameters [¼ì²é²ÎÊı]*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_ADC_IT(ADC_IT));

  /* Get the ADC IT index [È¡µÃADC ITÖĞ¶ÏºÅ]*/
  itmask = (u8)ADC_IT;

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC interrupts [ÔÊĞíÑ¡ÔñµÄADC ÖĞ¶Ï]*/
    ADCx->CR1 |= itmask;
  }
  else
  {
    /* Disable the selected ADC interrupts [½ûÓÃÑ¡ÔñµÄADC ÖĞ¶Ï]*/
    ADCx->CR1 &= (~(u32)itmask);
  }
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_ResetCalibration
* ¹¦ÄÜÃèÊö: ÖØÖÃÑ¡ÖĞµÄADCĞ£×¼¼Ä´æÆ÷
* ÊäÈë²ÎÊı: ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_ResetCalibration(ADC_TypeDef* ADCx)
{
  /* Check the parameters [¼ì²é²ÎÊı]*/
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Resets the selected ADC calibration registers [¸´Î»Ñ¡ÔñµÄADCĞ£×¼¼Ä´æÆ÷]*/
  ADCx->CR2 |= CR2_RSTCAL_Set;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_GetResetCalibrationStatus
* ¹¦ÄÜÃèÊö: µÃµ½Ñ¡ÖĞµÄADCÖØÖÃĞ£×¼¼Ä´æÆ÷µÄ×´Ì¬.
* ÊäÈë²ÎÊı: ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ADCÖØÖÃĞ£×¼¼Ä´æÆ÷µÄĞÂ×´Ì¬£¨SET»òRESET£©.
*******************************************************************************/
FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Check the status of RSTCAL bit [¼ì²é×´Ì¬Î»RSTCAL]*/
  if ((ADCx->CR2 & CR2_RSTCAL_Set) != (u32)RESET)
  {
    /* RSTCAL bit is set [ÖÃÎ»RSTCAL]*/
    bitstatus = SET;
  }
  else
  {
    /* RSTCAL bit is reset [¸´Î»RSTCAL]*/
    bitstatus = RESET;
  }

  /* Return the RSTCAL bit status [·µ»Ø×´Ì¬Î»RSTCAL]*/
  return  bitstatus;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_StartCalibration
* ¹¦ÄÜÃèÊö: ¿ªÊ¼Ñ¡ÖĞµÄADCĞ£×¼¹ı³Ì.
* ÊäÈë²ÎÊı: ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_StartCalibration(ADC_TypeDef* ADCx)
{
  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Enable the selected ADC calibration process [ÔÊĞí±»Ñ¡ÔñµÄADCĞ£×¼¹ı³Ì]*/
  ADCx->CR2 |= CR2_CAL_Set;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_GetCalibrationStatus
* ¹¦ÄÜÃèÊö: µÃµ½Ñ¡ÖĞµÄADC Ğ£×¼µÄ×´Ì¬
* ÊäÈë²ÎÊı: ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é..
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ADC Ğ£×¼µÄĞÂ×´Ì¬£¨SET »òRESET£©.
*******************************************************************************/
FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Check the status of CAL bit [¼ì²é×´Ì¬Î»CAL]*/
  if ((ADCx->CR2 & CR2_CAL_Set) != (u32)RESET)
  {
    /* CAL bit is set: calibration on going [CALÉèÖÃ:Ğ£×¼¿ªÊ¼]*/
    bitstatus = SET;
  }
  else
  {
    /* CAL bit is reset: end of calibration [CAL¸´Î»:½áÊøĞ£×¼]*/
    bitstatus = RESET;
  }

  /* Return the CAL bit status [·µ»Ø×´Ì¬Î»CAL]*/
  return  bitstatus;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_SoftwareStartConvCmd
* ¹¦ÄÜÃèÊö: Ê¹ÄÜ/½ûÖ¹Ñ¡ÖĞµÄADCÓÉÈí¼ş¿ØÖÆ¿ªÊ¼×ª»» .
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)NewState:Ñ¡ÖĞµÄÓÉÈí¼ş·¢³ö¿ªÊ¼ĞÅºÅµÄADCµÄĞÂ×´Ì¬Õâ¸ö²ÎÊı¿ÉÒÔÊÇ:ENABLE»òDISABLE
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC conversion on external event and start the selected
       ADC conversion [ÔÊĞíÑ¡ÔñµÄADC×ª»»Íâ²¿ÊÂ¼şºÍÆô¶¯Ñ¡ÔñµÄADC×ª»»]*/
    ADCx->CR2 |= CR2_EXTTRIG_SWSTART_Set;
  }
  else
  {
    /* Disable the selected ADC conversion on external event and stop the selected
       ADC conversion [½ûÖ¹Ñ¡ÔñµÄADC×ª»»Íâ²¿ÊÂ¼şºÍÍ£Ö¹Ñ¡ÔñµÄADC×ª»»]*/
    ADCx->CR2 &= CR2_EXTTRIG_SWSTART_Reset;
  }
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_GetSoftwareStartConvStatus
* ¹¦ÄÜÃèÊö: »ñµÃÑ¡ÖĞADCÈí¼ş¿ªÊ¼×ª»»×´Ì¬¡£
* ÊäÈë²ÎÊı: ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ADCÈí¼ş¿ªÊ¼×ª»»µÄĞÂ×´Ì¬£¨SET»òRESET£©.
*******************************************************************************/
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Check the status of SWSTART bit [¼ì²é×´Ì¬Î»SWSTART]*/
  if ((ADCx->CR2 & CR2_SWSTART_Set) != (u32)RESET)
  {
    /* SWSTART bit is set [ÖÃÎ»SWSTART]*/
    bitstatus = SET;
  }
  else
  {
    /* SWSTART bit is reset [¸´Î»SWSTART]*/
    bitstatus = RESET;
  }

  /* Return the SWSTART bit status [·µ»Ø×´Ì¬Î»SWSTART]*/
  return  bitstatus;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_DiscModeChannelCountConfig
* ¹¦ÄÜÃèÊö: ÅäÖÃÑ¡ÖĞµÄADC³£¹æ×éÎª·ÇÁ¬ĞøÄ£Ê½.
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)Number:·ÇÁ¬ĞøÄ£Ê½ÏÂ³£¹æĞÅµÀ¼ÆÊıÖµ¡£¸ÃÖµ·¶Î§Îª1£­8¡£
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, u8 Number)
{
  u32 tmpreg1 = 0;
  u32 tmpreg2 = 0;

  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_REGULAR_DISC_NUMBER(Number));

  /* Get the old register value [È¡µÃ¹ıÈ¥µÄ¼Ä´æÆ÷Öµ]*/
  tmpreg1 = ADCx->CR1;
  /* Clear the old discontinuous mode channel count [Çå³ı¹ıÈ¥µÄ²»¼ä¶ÏÄ£Ê½Í¨µÀ¼ÆÊıÆ÷]*/
  tmpreg1 &= CR1_DISCNUM_Reset;
  /* Set the discontinuous mode channel count [ÉèÖÃ²»¼ä¶ÏÄ£Ê½Í¨µÀ¼ÆÊıÆ÷]*/
  tmpreg2 = Number - 1;
  tmpreg1 |= tmpreg2 << 13;
  /* Store the new register value [´æ´¢ĞÂµÄ¼Ä´æÆ÷Öµ]*/
  ADCx->CR1 = tmpreg1;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_DiscModeCmd
* ¹¦ÄÜÃèÊö: Ê¹ÄÜ/½ûÖ¹Ö¸¶¨µÄADC³£¹æ×éĞÅµÀµÄ·ÇÁ¬ĞøÄ£Ê½
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)NewState: ADC³£¹æ×éĞÅµÀÏÂ·ÇÁ¬ĞøÄ£Ê½µÄĞÂ×´Ì¬Õâ¸ö²ÎÊı¿ÉÒÔÊÇ£ºENABLE»òDISABLE¡£
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC regular discontinuous mode [ÔÊĞíÑ¡ÔñµÄADC¹æÔò¼ä¶ÏÄ£Ê½]*/
    ADCx->CR1 |= CR1_DISCEN_Set;
  }
  else
  {
    /* Disable the selected ADC regular discontinuous mode [½ûÖ¹Ñ¡ÔñµÄADC¹æÔò¼ä¶ÏÄ£Ê½]*/
    ADCx->CR1 &= CR1_DISCEN_Reset;
  }
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_RegularChannelConfig
* ¹¦ÄÜÃèÊö: ÎªÑ¡ÖĞµÄADC³£¹æ×éĞÅµÀÅäÖÃÏà¹ØµÄÒôĞòÆ÷£¨sequencer£©µÈ¼¶ºÍ²ÉÑùÊ±¼ä¡£
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)ADC_Channel:ĞèÒªÅäÖÃµÄADCĞÅµÀ
*                    ADC_Channel ¿ÉÄÜµÄÈ¡Öµ£º
*                       - ADC_Channel_0: ADCĞÅµÀ0±»Ñ¡Ôñ
*                       - ADC_Channel_1: ADCĞÅµÀ1±»Ñ¡Ôñ
*                       - ADC_Channel_2: ADCĞÅµÀ2±»Ñ¡Ôñ
*                       - ADC_Channel_3: ADCĞÅµÀ3±»Ñ¡Ôñ
*                       - ADC_Channel_4: ADCĞÅµÀ4±»Ñ¡Ôñ
*                       - ADC_Channel_5: ADCĞÅµÀ5±»Ñ¡Ôñ
*                       - ADC_Channel_6: ADCĞÅµÀ6±»Ñ¡Ôñ
*                       - ADC_Channel_7: ADCĞÅµÀ7±»Ñ¡Ôñ
*                       - ADC_Channel_8: ADCĞÅµÀ8±»Ñ¡Ôñ
*                       - ADC_Channel_9: ADCĞÅµÀ9±»Ñ¡Ôñ
*                       - ADC_Channel_10: ADCĞÅµÀ10±»Ñ¡Ôñ
*                       - ADC_Channel_11: ADCĞÅµÀ11±»Ñ¡Ôñ
*                       - ADC_Channel_12: ADCĞÅµÀ12±»Ñ¡Ôñ
*                       - ADC_Channel_13: ADCĞÅµÀ13±»Ñ¡Ôñ
*                       - ADC_Channel_14: ADCĞÅµÀ14±»Ñ¡Ôñ
*                       - ADC_Channel_15: ADCĞÅµÀ15±»Ñ¡Ôñ
*                       - ADC_Channel_16: ADCĞÅµÀ16±»Ñ¡Ôñ
*                       - ADC_Channel_17: ADCĞÅµÀ17±»Ñ¡Ôñ
*           (3)Rank:³£¹æ×éÒôĞòÆ÷£¨sequencer£©µÄµÈ¼¶¡£²ÎÊı·¶Î§ÊÇ1£­16¡£
*           (4)ADC_SampleTime:½«ÒªÎªËùÑ¡µÄĞÅµÀÉèÖÃµÄ²ÉÑùÊ±¼ä
*                    ADC_SampleTime.È¡Öµ:
*                       - ADC_SampleTime_1Cycles5: ²ÉÑùÊ±¼äµÈÓÚ1.5¸öÖÜÆÚ
*                       - ADC_SampleTime_7Cycles5: ²ÉÑùÊ±¼äµÈÓÚ7.5¸öÖÜÆÚ
*                       - ADC_SampleTime_13Cycles5: ²ÉÑùÊ±¼äµÈÓÚ13.5¸öÖÜÆÚ
*                       - ADC_SampleTime_28Cycles5: ²ÉÑùÊ±¼äµÈÓÚ28.5¸öÖÜÆÚ
*                       - ADC_SampleTime_41Cycles5: ²ÉÑùÊ±¼äµÈÓÚ41.5¸öÖÜÆÚ
*                       - ADC_SampleTime_55Cycles5: ²ÉÑùÊ±¼äµÈÓÚ55.5¸öÖÜÆÚ
*                       - ADC_SampleTime_71Cycles5: ²ÉÑùÊ±¼äµÈÓÚ71.5¸öÖÜÆÚ
*                       - ADC_SampleTime_239Cycles5: ²ÉÑùÊ±¼äµÈÓÚ239.5¸öÖÜÆÚ
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, u8 ADC_Channel, u8 Rank, u8 ADC_SampleTime)
{
  u32 tmpreg1 = 0, tmpreg2 = 0;

  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  assert_param(IS_ADC_REGULAR_RANK(Rank));
  assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));

  /* if ADC_Channel_10 ... ADC_Channel_17 is selected [Èç¹ûADC_Channel_10 ... ADC_Channel_17±»Ñ¡Ôñ]*/
  if (ADC_Channel > ADC_Channel_9)
  {
    /* Get the old register value [È¡µÃ¾ÉµÄ¼Ä´æÆ÷Öµ]*/
    tmpreg1 = ADCx->SMPR1;
    /* Calculate the mask to clear [¼ÆËãĞèÒªÇå³ıµÄ±êÖ¾]*/
    tmpreg2 = SMPR1_SMP_Set << (3 * (ADC_Channel - 10));
    /* Clear the old discontinuous mode channel count [Çå³ı¹ıÈ¥µÄ²»¼ä¶ÏÄ£Ê½Í¨µÀ¼ÆÊıÆ÷]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [¼ÆËãĞèÒªÖÃÎ»µÄ±êÖ¾]*/
    tmpreg2 = (u32)ADC_SampleTime << (3 * (ADC_Channel - 10));
    /* Set the discontinuous mode channel count [ÉèÖÃ²»¼ä¶ÏÄ£Ê½Í¨µÀ¼ÆÊıÆ÷]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [´æ´¢ĞÂµÄ¼Ä´æÆ÷Öµ]*/
    ADCx->SMPR1 = tmpreg1;
  }
  else /* ADC_Channel include in ADC_Channel_[0..9] [ADC_Channel ÔÚ ADC_Channel_[0..9]Ö®¼ä]*/
  {
    /* Get the old register value [È¡µÃ¾ÉµÄ¼Ä´æÆ÷Öµ]*/
    tmpreg1 = ADCx->SMPR2;
    /* Calculate the mask to clear [¼ÆËãĞèÒªÇå³ıµÄ±êÖ¾]*/
    tmpreg2 = SMPR2_SMP_Set << (3 * ADC_Channel);
    /* Clear the old discontinuous mode channel count [Çå³ı¹ıÈ¥µÄ²»¼ä¶ÏÄ£Ê½Í¨µÀ¼ÆÊıÆ÷]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [¼ÆËãĞèÒªÖÃÎ»µÄ±êÖ¾]*/
    tmpreg2 = (u32)ADC_SampleTime << (3 * ADC_Channel);
    /* Set the discontinuous mode channel count [ÉèÖÃ²»¼ä¶ÏÄ£Ê½Í¨µÀ¼ÆÊıÆ÷]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [´æ´¢ĞÂµÄ¼Ä´æÆ÷Öµ]*/
    ADCx->SMPR2 = tmpreg1;
  }
  /* For Rank 1 to 6 [ĞòÁĞÔÚ1µ½6Ö®¼ä]*/
  if (Rank < 7)
  {
    /* Get the old register value [È¡µÃ¾ÉµÄ¼Ä´æÆ÷Öµ]*/
    tmpreg1 = ADCx->SQR3;
    /* Calculate the mask to clear [¼ÆËãĞèÒªÇå³ıµÄ±êÖ¾]*/
    tmpreg2 = SQR3_SQ_Set << (5 * (Rank - 1));
    /* Clear the old SQx bits for the selected rank [¸ù¾İÑ¡ÔñµÄĞòÁĞÇå³ı¾ÉµÄSQxÎ»]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [¼ÆËãĞèÒªÖÃÎ»µÄ±êÖ¾]*/
    tmpreg2 = (u32)ADC_Channel << (5 * (Rank - 1));
    /* Set the SQx bits for the selected rank [¸ù¾İÑ¡ÔñµÄĞòÁĞÖÃÎ»ĞÂµÄSQxÎ»]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [´æ´¢ĞÂµÄ¼Ä´æÆ÷Öµ]*/
    ADCx->SQR3 = tmpreg1;
  }
  /* For Rank 7 to 12 [ĞòÁĞÔÚ7µ½12Ö®¼ä]*/
  else if (Rank < 13)
  {
    /* Get the old register value [È¡µÃ¾ÉµÄ¼Ä´æÆ÷Öµ]*/
    tmpreg1 = ADCx->SQR2;
    /* Calculate the mask to clear [¼ÆËãĞèÒªÇå³ıµÄ±êÖ¾]*/
    tmpreg2 = SQR2_SQ_Set << (5 * (Rank - 7));
    /* Clear the old SQx bits for the selected rank [¸ù¾İÑ¡ÔñµÄĞòÁĞÇå³ı¾ÉµÄSQxÎ»]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [¼ÆËãĞèÒªÖÃÎ»µÄ±êÖ¾]*/
    tmpreg2 = (u32)ADC_Channel << (5 * (Rank - 7));
    /* Set the SQx bits for the selected rank [¸ù¾İÑ¡ÔñµÄĞòÁĞÖÃÎ»ĞÂµÄSQxÎ»]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [´æ´¢ĞÂµÄ¼Ä´æÆ÷Öµ]*/
    ADCx->SQR2 = tmpreg1;
  }
  /* For Rank 13 to 16 [ĞòÁĞÔÚ13µ½16Ö®¼ä]*/
  else
  {
    /* Get the old register value [È¡µÃ¾ÉµÄ¼Ä´æÆ÷Öµ]*/
    tmpreg1 = ADCx->SQR1;
    /* Calculate the mask to clear [¼ÆËãĞèÒªÇå³ıµÄ±êÖ¾]*/
    tmpreg2 = SQR1_SQ_Set << (5 * (Rank - 13));
    /* Clear the old SQx bits for the selected rank [¸ù¾İÑ¡ÔñµÄĞòÁĞÇå³ı¾ÉµÄSQxÎ»]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [¼ÆËãĞèÒªÖÃÎ»µÄ±êÖ¾]*/
    tmpreg2 = (u32)ADC_Channel << (5 * (Rank - 13));
    /* Set the SQx bits for the selected rank [¸ù¾İÑ¡ÔñµÄĞòÁĞÖÃÎ»ĞÂµÄSQxÎ»]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [´æ´¢ĞÂµÄ¼Ä´æÆ÷Öµ]*/
    ADCx->SQR1 = tmpreg1;
  }
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_ExternalTrigConvCmd
* ¹¦ÄÜÃèÊö: Ê¹ÄÜ»ò½ûÖ¹Íâ²¿´¥·¢ADCx×ª»»¡£
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)NewState:Ñ¡ÖĞµÄÓÉÍâ²¿´¥·¢×ª»»µÄADCµÄĞÂ×´Ì¬Õâ¸ö²ÎÊı¿ÉÒÔÊÇ£ºENABLE»òDISABLE¡£
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_ExternalTrigConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC conversion on external event [ÔÊĞíÍâ²¿ÊÂ¼ş´¥·¢Ñ¡ÔñµÄADC×ª»»]*/
    ADCx->CR2 |= CR2_EXTTRIG_Set;
  }
  else
  {
    /* Disable the selected ADC conversion on external event [½ûÖ¹Íâ²¿ÊÂ¼ş´¥·¢Ñ¡ÔñµÄADC×ª»»]*/
    ADCx->CR2 &= CR2_EXTTRIG_Reset;
  }
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_GetConversionValue
* ¹¦ÄÜÃèÊö: ·µ»Ø³£¹æĞÅµÀ×îºóµÄADCx×ª»»½á¹û¡£
* ÊäÈë²ÎÊı: ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: Êı¾İ×ª»»½á¹û.
*******************************************************************************/
u16 ADC_GetConversionValue(ADC_TypeDef* ADCx)
{
  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Return the selected ADC conversion value [·µ»ØÑ¡ÔñµÄADC×ª»»Öµ]*/
  return (u16) ADCx->DR;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_GetDualModeConversionValue
* ¹¦ÄÜÃèÊö: ·µ»ØË«ÖØÄ£Ê½ÏÂµÄADC1ºÍADC2µÄ×îºó×ª»»½á¹û
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: Êı¾İ×ª»»½á¹û.
*******************************************************************************/
u32 ADC_GetDualModeConversionValue(void)
{
  /* Return the dual mode conversion value [·µ»ØË«Ä£Ê½×ª»»Öµ]*/
  return (*(vu32 *) DR_ADDRESS);
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_AutoInjectedConvCmd
* ¹¦ÄÜÃèÊö: Ê¹ÄÜ/½ûÖ¹ÔÚ³£¹æ×é×ª»»ºó£¬±»Ñ¡µÄADC½øĞĞ×Ô¶¯×¢Èë×é×ª»»¡£
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)NewState:Ñ¡ÖĞµÄ½øĞĞ×Ô¶¯×¢Èë×ª»»µÄADCĞÂ×´Ì¬Õâ¸ö²ÎÊı¿ÉÒÔÊÇ£ºENABLE»òDISABLE¡£
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_AutoInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC automatic injected group conversion [ÔÊĞíÑ¡ÔñµÄADC×Ô¶¯×¢ÈëÍ¨µÀ×é×ª»»]*/
    ADCx->CR1 |= CR1_JAUTO_Set;
  }
  else
  {
    /* Disable the selected ADC automatic injected group conversion [½ûÖ¹Ñ¡ÔñµÄADC×Ô¶¯×¢ÈëÍ¨µÀ×é×ª»»]*/
    ADCx->CR1 &= CR1_JAUTO_Reset;
  }
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_InjectedDiscModeCmd
* ¹¦ÄÜÃèÊö: Ê¹ÄÜ/½ûÖ¹Ö¸¶¨ADC×¢Èë×éĞÅµÀµÄ·ÇÁ¬ĞøÄ£Ê½¡£
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)NewState:±»Ñ¡ÖĞµÄ£¬×¢Èë×éĞÅµÀÉÏÎª·ÇÁ¬ĞøÄ£Ê½µÄADCµÄĞÂ×´Ì¬Õâ¸ö²ÎÊı¿ÉÒÔÊÇ£ºENABLE»òDISABLE¡£
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_InjectedDiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC injected discontinuous mode [ÔÊĞíÑ¡µÄADCÍ¨µÀ×¢ÈëÁ¬Ğø×ª»»Ä£Ê½]*/
    ADCx->CR1 |= CR1_JDISCEN_Set;
  }
  else
  {
    /* Disable the selected ADC injected discontinuous mode [½ûÖ¹Ñ¡µÄADCÍ¨µÀ×¢ÈëÁ¬Ğø×ª»»Ä£Ê½]*/
    ADCx->CR1 &= CR1_JDISCEN_Reset;
  }
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_ExternalTrigInjectedConvConfig
* ¹¦ÄÜÃèÊö: Îª×¢ÈëĞÅµÀ×ª»»ÅäÖÃADCxÍâ²¿´¥·¢
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)ADC_ExternalTrigInjecConv:¿ªÊ¼×¢Èë×ª»»µÄADC´¥·¢Æ÷
*                    ADC_ExternalTrigInjecConv È¡Öµ:
*                       - ADC_ExternalTrigInjecConv_T1_TRGO: ¶¨Ê±Æ÷1TRGOÊÂ¼ş×÷Îª×¢Èë×ª»»(ADC1,ADC2,ADC3)µÄÍâ²¿´¥·¢Æ÷
*                       - ADC_ExternalTrigInjecConv_T1_CC4: ¶¨Ê±Æ÷1²¶»ñ±È½Ï4×÷Îª×¢Èë×ª»»(ADC1,ADC2,ADC3)µÄÍâ²¿´¥·¢Æ÷
*                       - ADC_ExternalTrigInjecConv_T2_TRGO: ¶¨Ê±Æ÷2TRGOÊÂ¼ş×÷Îª×¢Èë×ª»»(ADC1,ADC2,ADC3)µÄÍâ²¿´¥·¢Æ÷
*                       - ADC_External TrigInjecConv_T2_CC1: ¶¨Ê±Æ÷2²¶»ñ±È½Ï1×÷Îª×¢Èë×ª»»(ADC1,ADC2,ADC3)µÄÍâ²¿´¥·¢Æ÷
*                       - ADC_ExternalTrigInjecConv_T3_CC4: ¶¨Ê±Æ÷3²¶»ñ±È½Ï4×÷Îª×¢Èë×ª»»(ADC1,ADC2,ADC3)µÄÍâ²¿´¥·¢Æ÷
*                       - ADC_ExternalTrigInjecConv_T4_TRGO: ¶¨Ê±Æ÷4TRGOÊÂ¼ş×÷Îª×¢Èë×ª»»(ADC1,ADC2,ADC3)µÄÍâ²¿´¥·¢Æ÷
*                       - ADC_ExternalTrigInjecConv_Ext_IT15_TIM8_CC4: Íâ²¿ÖĞ¶Ï15»òÕßtimer8±È½ÏÊÂ¼ş×÷Îª×¢Èë×ª»»(ADC1,ADC2,ADC3)µÄÍâ²¿´¥·¢Æ÷
*                       - ADC_External TrigInjecConv_T4_CC3: timer4±È½Ï3ÊÂ¼ş×÷Îª×¢Èë×ª»»(Ö»ADC3)µÄÍâ²¿´¥·¢Æ÷
*                       - ADC_External TrigInjecConv_T8_CC2: timer8±È½Ï2ÊÂ¼ş×÷Îª×¢Èë×ª»»(Ö»ADC3)µÄÍâ²¿´¥·¢Æ÷
*                       - ADC_External TrigInjecConv_T8_CC4: timer8±È½Ï4ÊÂ¼ş×÷Îª×¢Èë×ª»»(Ö»ADC3)µÄÍâ²¿´¥·¢Æ÷
*                       - ADC_ExternalTrigInjecConv_T5_TRGO: ¶¨Ê±Æ÷5 TRGO ÊÂ¼ş×÷Îª×¢Èë×ª»»(Ö»ADC3)µÄÍâ²¿´¥·¢Æ÷
*                       - ADC_External TrigInjecConv_T5_CC4: timer5±È½Ï4ÊÂ¼ş×÷Îª×¢Èë×ª»»(Ö»ADC3)µÄÍâ²¿´¥·¢Æ÷
*                       - ADC_ExternalTrigInjecConv_ÎŞ: ×¢Èë×ª»»ÓÉÈí¼şÆô¶¯£¬¶ø²»ÊÇÓÉÍâ²¿´¥·¢Æ÷Æô¶¯
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, u32 ADC_ExternalTrigInjecConv)
{
  u32 tmpreg = 0;

  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_EXT_INJEC_TRIG(ADC_ExternalTrigInjecConv));

  /* Get the old register value [È¡µÃ¾ÉµÄ¼Ä´æÆ÷Öµ]*/
  tmpreg = ADCx->CR2;
  /* Clear the old external event selection for injected group [Çå³ıÑ¡ÔñµÄ×¢ÈëÍ¨µÀÈºµÄÍâ²¿´¥·¢ÊÂ¼ş]*/
  tmpreg &= CR2_JEXTSEL_Reset;
  /* Set the external event selection for injected group [ÖÃÎ»Ñ¡ÔñµÄ×¢ÈëÍ¨µÀÈºµÄÍâ²¿´¥·¢ÊÂ¼ş]*/
  tmpreg |= ADC_ExternalTrigInjecConv;
  /* Store the new register value [´æ´¢ĞÂµÄ¼Ä´æÆ÷Öµ]*/
  ADCx->CR2 = tmpreg;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_ExternalTrigInjectedConvCmd
* ¹¦ÄÜÃèÊö: Í¨¹ıÍâ²¿´¥·¢Æ÷Ê¹ÄÜ/½ûÖ¹ADCx×¢ÈëĞÅµÀ×ª»»
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)NewState:Ñ¡ÖĞµÄÓÃÓÚÆô¶¯×¢Èë×ª»»µÄADCÍâ²¿´¥·¢Æ÷¡£Õâ¸ö²ÎÊı¿ÉÒÔÊÇ:ENABLE»òDISABLE¡
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC external event selection for injected group [Ê¹ÄÜÑ¡ÔñµÄADCx×¢ÈëĞÅµÀÍâ²¿ÊÂ¼ş´¥·¢]*/
    ADCx->CR2 |= CR2_JEXTTRIG_Set;
  }
  else
  {
    /* Disable the selected ADC external event selection for injected group [½ûÖ¹Ñ¡ÔñµÄADCx×¢ÈëĞÅµÀÍâ²¿ÊÂ¼ş´¥·¢]*/
    ADCx->CR2 &= CR2_JEXTTRIG_Reset;
  }
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_SoftwareStartInjectedConvCmd
* ¹¦ÄÜÃèÊö: Ê¹ÄÜ/½ûÖ¹Ñ¡ÖĞµÄADC¿ªÊ¼×¢ÈëĞÅµÀ×ª»»¡£
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)NewState:±»Ñ¡ÖĞµÄÓÃÓÚÆô¶¯×¢Èë×ª»»µÄADCÈí¼şµÄĞÂ×´Ì¬¡£Õâ¸ö²ÎÊı¿ÉÒÔÊÇ:ENABLE»òDISABLE¡£
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC conversion for injected group on external event and start the selected
       ADC injected conversion [ÔÊĞíÍâ²¿ÊÂ¼ş´¥·¢Ñ¡ÔñµÄADC×¢ÈëÍ¨µÀÈºÄ£Ê½×ª»»²¢ÇÒÆô¶¯×¢ÈëÑ¡Ôñ]*/
    ADCx->CR2 |= CR2_JEXTTRIG_JSWSTART_Set;
  }
  else
  {
    /* Disable the selected ADC conversion on external event for injected group and stop the selected
       ADC injected conversion [½ûÖ¹Íâ²¿ÊÂ¼ş´¥·¢Ñ¡ÔñµÄADC×¢ÈëÍ¨µÀÈºÄ£Ê½×ª»»²¢ÇÒÆô¶¯×¢ÈëÑ¡Ôñ]*/
    ADCx->CR2 &= CR2_JEXTTRIG_JSWSTART_Reset;
  }
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_GetSoftwareStartInjectedConvCmdStatus
* ¹¦ÄÜÃèÊö: »ñµÃÑ¡ÖĞµÄÓÉÈí¼şÆô¶¯×¢Èë×ª»»µÄADCµÄ×´Ì¬¡£
* ÊäÈë²ÎÊı: ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÓÉÈí¼şÆô¶¯×¢Èë×ª»»µÄADCĞÂ×´Ì¬£¨SET»òÕßRESET£©
*******************************************************************************/
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Check the status of JSWSTART bit [¼ì²é×´Ì¬Î»JSWSTART]*/
  if ((ADCx->CR2 & CR2_JSWSTART_Set) != (u32)RESET)
  {
    /* JSWSTART bit is set [ÖÃÎ»JSWSTART]*/
    bitstatus = SET;
  }
  else
  {
    /* JSWSTART bit is reset [¸´Î»JSWSTART]*/
    bitstatus = RESET;
  }

  /* Return the JSWSTART bit status [·µ»Ø×´Ì¬Î»JSWSTART]*/
  return  bitstatus;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_InjectedChannelConfig
* ¹¦ÄÜÃèÊö: ÅäÖÃÑ¡ÖĞµÄADC×¢ÈëĞÅµÀÏàÓ¦µÄÒôĞòÆ÷£¨sequencer£©µÈ¼¶ºÍ²ÉÑùÊ±¼ä¡£
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)ADC_Channel: ĞèÒªÅäÖÃµÄADCĞÅµÀ.
*                    ADC_Channel ¿ÉÄÜµÄÈ¡Öµ£º
*                       - ADC_Channel_0: ADCĞÅµÀ0±»Ñ¡Ôñ
*                       - ADC_Channel_1: ADCĞÅµÀ1±»Ñ¡Ôñ
*                       - ADC_Channel_2: ADCĞÅµÀ2±»Ñ¡Ôñ
*                       - ADC_Channel_3: ADCĞÅµÀ3±»Ñ¡Ôñ
*                       - ADC_Channel_4: ADCĞÅµÀ4±»Ñ¡Ôñ
*                       - ADC_Channel_5: ADCĞÅµÀ5±»Ñ¡Ôñ
*                       - ADC_Channel_6: ADCĞÅµÀ6±»Ñ¡Ôñ
*                       - ADC_Channel_7: ADCĞÅµÀ7±»Ñ¡Ôñ
*                       - ADC_Channel_8: ADCĞÅµÀ8±»Ñ¡Ôñ
*                       - ADC_Channel_9: ADCĞÅµÀ9±»Ñ¡Ôñ
*                       - ADC_Channel_10: ADCĞÅµÀ10±»Ñ¡Ôñ
*                       - ADC_Channel_11: ADCĞÅµÀ11±»Ñ¡Ôñ
*                       - ADC_Channel_12: ADCĞÅµÀ12±»Ñ¡Ôñ
*                       - ADC_Channel_13: ADCĞÅµÀ13±»Ñ¡Ôñ
*                       - ADC_Channel_14: ADCĞÅµÀ14±»Ñ¡Ôñ
*                       - ADC_Channel_15: ADCĞÅµÀ15±»Ñ¡Ôñ
*                       - ADC_Channel_16: ADCĞÅµÀ16±»Ñ¡Ôñ
*                       - ADC_Channel_17: ADCĞÅµÀ17±»Ñ¡Ôñ
*           (3)Rank:×¢Èë×éÒôĞòÆ÷£¨sequencer£©µÄµÈ¼¶,Ñ¡Ôñ·¶Î§±ØĞëÔÚ1-4Ö®¼ä
*           (4)ADC_SampleTime: ADC_SampleTime:½«ÒªÎªÑ¡ÖĞĞÅµÀÉèÖÃµÄ²ÉÑùÊ±¼äÖµ
*                    ADC_SampleTime.È¡Öµ:
*                       - ADC_SampleTime_1Cycles5: ²ÉÑùÊ±¼äµÈÓÚ1.5¸öÖÜÆÚ
*                       - ADC_SampleTime_7Cycles5: ²ÉÑùÊ±¼äµÈÓÚ7.5¸öÖÜÆÚ
*                       - ADC_SampleTime_13Cycles5: ²ÉÑùÊ±¼äµÈÓÚ13.5¸öÖÜÆÚ
*                       - ADC_SampleTime_28Cycles5: ²ÉÑùÊ±¼äµÈÓÚ28.5¸öÖÜÆÚ
*                       - ADC_SampleTime_41Cycles5: ²ÉÑùÊ±¼äµÈÓÚ41.5¸öÖÜÆÚ
*                       - ADC_SampleTime_55Cycles5: ²ÉÑùÊ±¼äµÈÓÚ55.5¸öÖÜÆÚ
*                       - ADC_SampleTime_71Cycles5: ²ÉÑùÊ±¼äµÈÓÚ71.5¸öÖÜÆÚ
*                       - ADC_SampleTime_239Cycles5: ²ÉÑùÊ±¼äµÈÓÚ239.5¸öÖÜÆÚ
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, u8 ADC_Channel, u8 Rank, u8 ADC_SampleTime)
{
  u32 tmpreg1 = 0, tmpreg2 = 0, tmpreg3 = 0;

  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  assert_param(IS_ADC_INJECTED_RANK(Rank));
  assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));

  /* if ADC_Channel_10 ... ADC_Channel_17 is selected [Èç¹ûADC_Channel_10 ... ADC_Channel_17±»Ñ¡Ôñ]*/
  if (ADC_Channel > ADC_Channel_9)
  {
    /* Get the old register value [È¡µÃ¾ÉµÄ¼Ä´æÆ÷Öµ]*/
    tmpreg1 = ADCx->SMPR1;
    /* Calculate the mask to clear [¼ÆËãĞèÒªÇå³ıµÄ±êÖ¾]*/
    tmpreg2 = SMPR1_SMP_Set << (3*(ADC_Channel - 10));
    /* Clear the old discontinuous mode channel count [Çå³ı¹ıÈ¥µÄ²»¼ä¶ÏÄ£Ê½Í¨µÀ¼ÆÊıÆ÷]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [¼ÆËãĞèÒªÖÃÎ»µÄ±êÖ¾]*/
    tmpreg2 = (u32)ADC_SampleTime << (3*(ADC_Channel - 10));
    /* Set the discontinuous mode channel count [ÉèÖÃ²»¼ä¶ÏÄ£Ê½Í¨µÀ¼ÆÊıÆ÷]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [´æ´¢ĞÂµÄ¼Ä´æÆ÷Öµ]*/
    ADCx->SMPR1 = tmpreg1;
  }
  else /* ADC_Channel include in ADC_Channel_[0..9] [ADC_Channel ÔÚ ADC_Channel_[0..9]Ö®¼ä]*/
  {
    /* Get the old register value [È¡µÃ¾ÉµÄ¼Ä´æÆ÷Öµ]*/
    tmpreg1 = ADCx->SMPR2;
    /* Calculate the mask to clear [¼ÆËãĞèÒªÇå³ıµÄ±êÖ¾]*/
    tmpreg2 = SMPR2_SMP_Set << (3 * ADC_Channel);
    /* Clear the old discontinuous mode channel count [Çå³ı¹ıÈ¥µÄ²»¼ä¶ÏÄ£Ê½Í¨µÀ¼ÆÊıÆ÷]*/
    tmpreg1 &= ~tmpreg2;
    /* Calculate the mask to set [¼ÆËãĞèÒªÖÃÎ»µÄ±êÖ¾]*/
    tmpreg2 = (u32)ADC_SampleTime << (3 * ADC_Channel);
    /* Set the discontinuous mode channel count [ÉèÖÃ²»¼ä¶ÏÄ£Ê½Í¨µÀ¼ÆÊıÆ÷]*/
    tmpreg1 |= tmpreg2;
    /* Store the new register value [´æ´¢ĞÂµÄ¼Ä´æÆ÷Öµ]*/
    ADCx->SMPR2 = tmpreg1;
  }

  /* Rank configuration [ĞòÁĞÅäÖÃ]*/
  /* Get the old register value [È¡µÃ¾ÉµÄ¼Ä´æÆ÷Öµ]*/
  tmpreg1 = ADCx->JSQR;
  /* Get JL value[È¡µÃJLÖµ]: Number = JL+1 */
  tmpreg3 =  (tmpreg1 & JSQR_JL_Set)>> 20;
  /* Calculate the mask to clear[¼ÆËãĞèÒªÇå³ıµÄ±êÖ¾]: ((Rank-1)+(4-JL-1)) */
  tmpreg2 = JSQR_JSQ_Set << (5 * (u8)((Rank + 3) - (tmpreg3 + 1)));
  /* Clear the old JSQx bits for the selected rank [Çå³ıÑ¡ÖĞĞòÁĞ¾ÉµÄJSQxÎ»]*/
  tmpreg1 &= ~tmpreg2;
  /* Calculate the mask to set[¼ÆËãĞèÒªÖÃÎ»µÄ±êÖ¾]: ((Rank-1)+(4-JL-1)) */
  tmpreg2 = (u32)ADC_Channel << (5 * (u8)((Rank + 3) - (tmpreg3 + 1)));
  /* Set the JSQx bits for the selected rank [ÖÃÎ»Ñ¡ÖĞĞòÁĞ¾ÉµÄJSQxÎ»]*/
  tmpreg1 |= tmpreg2;
  /* Store the new register value [´æ´¢ĞÂµÄ¼Ä´æÆ÷Öµ]*/
  ADCx->JSQR = tmpreg1;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_InjectedSequencerLengthConfig
* ¹¦ÄÜÃèÊö: ÅäÖÃ×¢ÈëĞÅµÀÒôĞòÆ÷£¨sequencer£©µÄ³¤¶È
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)Length:ÒôĞòÆ÷£¨sequencer£©µÄ³¤¶È¸Ã²ÎÊıµÄ·¶Î§ÊÇ1£­4.
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef* ADCx, u8 Length)
{
  u32 tmpreg1 = 0;
  u32 tmpreg2 = 0;

  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_LENGTH(Length));
  
  /* Get the old register value [È¡µÃ¾ÉµÄ¼Ä´æÆ÷Öµ]*/
  tmpreg1 = ADCx->JSQR;
  /* Clear the old injected sequnence lenght JL bits [Çå³ı×¢ÈëĞÅµÀÒôĞòÆ÷µÄ³¤¶ÈJLÎ»]*/
  tmpreg1 &= JSQR_JL_Reset;
  /* Set the injected sequnence lenght JL bits [ÖÃÎ»×¢ÈëĞÅµÀÒôĞòÆ÷µÄ³¤¶ÈJLÎ»]*/
  tmpreg2 = Length - 1; 
  tmpreg1 |= tmpreg2 << 20;
  /* Store the new register value [´æ´¢ĞÂµÄ¼Ä´æÆ÷Öµ]*/
  ADCx->JSQR = tmpreg1;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_SetInjectedOffset
* ¹¦ÄÜÃèÊö: ÉèÖÃ×¢ÈëĞÅµÀ×ª»»µÄÆ«ÒÆÖµ¡£
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)ADC_InjectedChannel:ĞèÒªÉèÖÃÆ«ÒÆÖµµÄADC×¢ÈëĞÅµÀ¡£
*                    ADC_InjectedChannel Ö¸¶¨ÁËĞèÒªÉèÖÃÆ«ÒÆÖµµÄADC ×¢ÈëĞÅµÀ:
*                       - ADC_InjectedChannel_1: Ñ¡Ôñ×¢ÈëĞÅµÀ1
*                       - ADC_InjectedChannel_2: Ñ¡Ôñ×¢ÈëĞÅµÀ2
*                       - ADC_InjectedChannel_3: Ñ¡Ôñ×¢ÈëĞÅµÀ3
*                       - ADC_InjectedChannel_4: Ñ¡Ôñ×¢ÈëĞÅµÀ4
*           (3)Offset:Ñ¡ÖĞµÄADC×¢ÈëĞÅµÀµÄÆ«ÒÆÖµ¡£¸Ã²ÎÊıÊÇÒ»¸ö12Î»Öµ
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_SetInjectedOffset(ADC_TypeDef* ADCx, u8 ADC_InjectedChannel, u16 Offset)
{
  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_CHANNEL(ADC_InjectedChannel));
  assert_param(IS_ADC_OFFSET(Offset));  

  /* Set the selected injected channel data offset [ÉèÖÃÑ¡ÔñµÄ×¢ÈëÍ¨µÀÊı¾İÆ«ÒÆ]*/
  *((vu32 *)((*(u32*)&ADCx) + ADC_InjectedChannel)) = (u32)Offset;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_GetInjectedConversionValue
* ¹¦ÄÜÃèÊö: ·µ»ØÑ¡ÖĞµÄ×¢ÈëĞÅµÀµÄADC×ª»»½á¹û¡£
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)ADC_InjectedChannel:ĞèÒªÉèÖÃÆ«ÒÆÖµµÄADC×¢ÈëĞÅµÀ
*              ADC_InjectedChannel Ö¸¶¨ÁËĞèÒªÉèÖÃÆ«ÒÆÖµµÄADC ×¢ÈëĞÅµÀ:
*                       - ADC_InjectedChannel_1: Ñ¡Ôñ×¢ÈëĞÅµÀ1
*                       - ADC_InjectedChannel_2: Ñ¡Ôñ×¢ÈëĞÅµÀ2
*                       - ADC_InjectedChannel_3: Ñ¡Ôñ×¢ÈëĞÅµÀ3
*                       - ADC_InjectedChannel_4: Ñ¡Ôñ×¢ÈëĞÅµÀ4
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: Êı¾İ×ª»»½á¹û.
*******************************************************************************/
u16 ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx, u8 ADC_InjectedChannel)
{
  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_CHANNEL(ADC_InjectedChannel));

  /* Returns the selected injected channel conversion data value [·µ»ØÑ¡ÔñµÄ×¢ÈëÍ¨µÀ×ª»»Êı¾İÖµ]*/
  return (u16) (*(vu32*) (((*(u32*)&ADCx) + ADC_InjectedChannel + JDR_Offset)));
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_AnalogWatchdogCmd
* ¹¦ÄÜÃèÊö: Ê¹ÄÜ/½ûÖ¹Ò»¸ö/ËùÓĞ³£¹æĞÅµÀ»òÕß×¢ÈëĞÅµÀµÄÄ£Äâ¿´ÃÅ¹·¡£
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)ADC_AnalogWatchdog: ADCÄ£Äâ¿´ÃÅ¹·ÅäÖÃ
*                    ¸Ã²ÎÊı¿ÉÄÜµÄÈ¡Öµ:
*                       - ADC_AnalogWatchdog_SingleRegEnable: µ¥³£¹æĞÅµÀÉÏµÄÄ£Äâ¿´ÃÅ¹·
*                       - ADC_AnalogWatchdog_SingleInjecEnable: µ¥×¢ÈëĞÅµÀÉÏµÄÄ£Äâ¿´ÃÅ¹·
*                       - ADC_AnalogWatchdog_SingleRegOrInjecEnable: µ¥³£¹æĞÅµÀ»òÕß×¢ÈëĞÅµÀµÄÄ£Äâ¿´ÃÅ¹·
*                       - ADC_AnalogWatchdog_AllRegEnable: ËùÓĞ³£¹æĞÅµÀÉÏµÄÄ£Äâ¿´ÃÅ¹·
*                       - ADC_AnalogWatchdog_AllInjecEnable: ËùÓĞ×¢ÈëĞÅµÀÉÏµÄÄ£Äâ¿´ÃÅ¹·
*                       - ADC_AnalogWatchdog_AllRegAllInjecEnable: ËùÓĞ³£¹æĞÅµÀºÍ×¢ÈëĞÅµÀÉÏµÄÄ£Äâ¿´ÃÅ¹·
*                       - ADC_AnalogWatchdog_ÎŞ: Ã»ÓĞĞÅµÀĞèÒªÄ£Äâ¿´ÃÅ¹·¼àÊÓ
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, u32 ADC_AnalogWatchdog)
{
  u32 tmpreg = 0;

  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_ANALOG_WATCHDOG(ADC_AnalogWatchdog));

  /* Get the old register value [È¡µÃ¾ÉµÄ¼Ä´æÆ÷Öµ]*/
  tmpreg = ADCx->CR1;
  /* Clear AWDEN, AWDENJ and AWDSGL bits [Çå³ıAWDEN,AWDENJºÍAWDSGLÎ»]*/
  tmpreg &= CR1_AWDMode_Reset;
  /* Set the analog watchdog enable mode [ÖÃÎ»Ä£Äâ¿´ÃÅ¹·ÔÊĞíÄ£Ê½]*/
  tmpreg |= ADC_AnalogWatchdog;
  /* Store the new register value [´æ´¢ĞÂµÄ¼Ä´æÆ÷Öµ]*/
  ADCx->CR1 = tmpreg;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_AnalogWatchdogThresholdsConfig
* ¹¦ÄÜÃèÊö: ÅäÖÃÄ£Äâ¿´ÃÅ¹·µÄ¸ß¡¢µÍ·§Öµ.
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)HighThreshold: ADCÄ£Äâ¿´ÃÅ¹·×î¸ß·§Öµ¡£¸Ã²ÎÊıÊÇÒ»¸ö12Î»µÄÊıÖµ
*           (3)LowThreshold: ADCÄ£Äâ¿´ÃÅ¹·×îµÍ·§Öµ¡£¸Ã²ÎÊıÊÇÒ»¸ö12Î»µÄÊıÖµ
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, u16 HighThreshold,
                                        u16 LowThreshold)
{
  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_THRESHOLD(HighThreshold));
  assert_param(IS_ADC_THRESHOLD(LowThreshold));

  /* Set the ADCx high threshold [ÉèÖÃADCxµÄ¸ß·§Öµ]*/
  ADCx->HTR = HighThreshold;
  /* Set the ADCx low threshold [ÉèÖÃADCxµÄµÍ·§Öµ]*/
  ADCx->LTR = LowThreshold;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_AnalogWatchdogSingleChannelConfig
* ¹¦ÄÜÃèÊö: ÅäÖÃ¼àÊÓµ¥ĞÅµÀµÄÄ£Äâ¿´ÃÅ¹·
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)ADC_Channel:½«ÒªÎªÆäÅäÖÃÄ£Äâ¿´ÃÅ¹·µÄĞÅµÀ.
*                    ADC_Channel ¿ÉÄÜµÄÈ¡Öµ£º
*                       - ADC_Channel_0: ADCĞÅµÀ0±»Ñ¡Ôñ
*                       - ADC_Channel_1: ADCĞÅµÀ1±»Ñ¡Ôñ
*                       - ADC_Channel_2: ADCĞÅµÀ2±»Ñ¡Ôñ
*                       - ADC_Channel_3: ADCĞÅµÀ3±»Ñ¡Ôñ
*                       - ADC_Channel_4: ADCĞÅµÀ4±»Ñ¡Ôñ
*                       - ADC_Channel_5: ADCĞÅµÀ5±»Ñ¡Ôñ
*                       - ADC_Channel_6: ADCĞÅµÀ6±»Ñ¡Ôñ
*                       - ADC_Channel_7: ADCĞÅµÀ7±»Ñ¡Ôñ
*                       - ADC_Channel_8: ADCĞÅµÀ8±»Ñ¡Ôñ
*                       - ADC_Channel_9: ADCĞÅµÀ9±»Ñ¡Ôñ
*                       - ADC_Channel_10: ADCĞÅµÀ10±»Ñ¡Ôñ
*                       - ADC_Channel_11: ADCĞÅµÀ11±»Ñ¡Ôñ
*                       - ADC_Channel_12: ADCĞÅµÀ12±»Ñ¡Ôñ
*                       - ADC_Channel_13: ADCĞÅµÀ13±»Ñ¡Ôñ
*                       - ADC_Channel_14: ADCĞÅµÀ14±»Ñ¡Ôñ
*                       - ADC_Channel_15: ADCĞÅµÀ15±»Ñ¡Ôñ
*                       - ADC_Channel_16: ADCĞÅµÀ16±»Ñ¡Ôñ
*                       - ADC_Channel_17: ADCĞÅµÀ17±»Ñ¡Ôñ
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCx, u8 ADC_Channel)
{
  u32 tmpreg = 0;

  /* Check the parameters ¼ì²é²ÎÊı */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));

  /* Get the old register value [È¡µÃ¾ÉµÄ¼Ä´æÆ÷Öµ]*/
  tmpreg = ADCx->CR1;
  /* Clear the Analog watchdog channel select bits [Çå³ıÄ£Äâ¿´ÃÅ¹·Í¨µÀÑ¡ÔñÎ»]*/
  tmpreg &= CR1_AWDCH_Reset;
  /* Set the Analog watchdog channel [ÖÃÎ»Ä£Äâ¿´ÃÅ¹·Í¨µÀÑ¡ÔñÎ»]*/
  tmpreg |= ADC_Channel;
  /* Store the new register value [´æ´¢ĞÂµÄ¼Ä´æÆ÷Öµ]*/
  ADCx->CR1 = tmpreg;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_TempSensorVrefintCmd
* ¹¦ÄÜÃèÊö: Ê¹ÄÜ/½ûÖ¹ÎÂ¶È´«¸ĞºÍ²Î¿¼µçÑ¹ĞÅµÀ¡£
* ÊäÈë²ÎÊı: NewState:ÎÂ¶È´«¸ĞºÍ²Î¿¼µçÑ¹ĞÅµÀµÄĞÂ×´Ì¬¡£¸Ã²ÎÊı¿ÉÒÔÈ¡Á½¸öÖµ£ºENABLE»òDISABLE¡£
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_TempSensorVrefintCmd(FunctionalState NewState)
{
  /* Check the parameters ¼ì²é²ÎÊı */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the temperature sensor and Vrefint channel[Ê¹ÄÜÎÂ¶È´«¸ĞºÍ²Î¿¼µçÑ¹ĞÅµÀ]*/
    ADC1->CR2 |= CR2_TSVREFE_Set;
  }
  else
  {
    /* Disable the temperature sensor and Vrefint channel[½ûÖ¹ÎÂ¶È´«¸ĞºÍ²Î¿¼µçÑ¹ĞÅµÀ]*/
    ADC1->CR2 &= CR2_TSVREFE_Reset;
  }
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_GetFlagStatus
* ¹¦ÄÜÃèÊö: ¼ì²éÖ¸¶¨µÄADC±êÖ¾ÊÇ·ñÖÃÎ».
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)ADC_FLAG:Ö¸¶¨ĞèÒª¼ì²éµÄ±êÖ¾¡£
*                    ¸ø³öÁËADC_FLAG µÄÈ¡Öµ:
*                       - ADC_FLAG_AWD: Ä£Äâ¿´ÃÅ¹·±êÖ¾
*                       - ADC_FLAG_EOC: ×ª»»½áÊø±êÖ¾
*                       - ADC_FLAG_JEOC: ×¢Èë×é×ª»»½áÊø±êÖ¾
*                       - ADC_FLAG_JSTRT: ×¢Èë×é×ª»»¿ªÊ¼±êÖ¾
*                       - ADC_FLAG_STRT: ³£¹æ×é×ª»»¿ªÊ¼±êÖ¾
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ADC_FLAGµÄĞÂ×´Ì¬(SET»òRESET).
*******************************************************************************/
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, u8 ADC_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_GET_FLAG(ADC_FLAG));

  /* Check the status of the specified ADC flag [¼ì²éÖ¸¶¨ADC×´Ì¬Î»]*/
  if ((ADCx->SR & ADC_FLAG) != (u8)RESET)
  {
    /* ADC_FLAG is set [ÖÃÎ»ADC_FLAG]*/
    bitstatus = SET;
  }
  else
  {
    /* ADC_FLAG is reset [¸´Î»ADC_FLAG]*/
    bitstatus = RESET;
  }

  /* Return the ADC_FLAG status [·µ»Ø×´Ì¬Î»ADC_FLAG]*/
  return  bitstatus;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_ClearFlag
* ¹¦ÄÜÃèÊö: Çå³ıADCxµÄ¹ÒÆğ±êÖ¾.
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)ADC_FLAG:ĞèÒªÇå³ıµÄ±êÖ¾¡£Ê¹ÓÃ"|"¿ÉÒÔÊ¹µÃÒ»¸ö»ò¶à¸ö±êÖ¾¿ÉÒÔÍ¬Ê±±»Çå³ı¡£
*                    ¸ø³öÁËADC_FLAG µÄÈ¡Öµ:
*                       - ADC_FLAG_AWD: Ä£Äâ¿´ÃÅ¹·±êÖ¾
*                       - ADC_FLAG_EOC: ×ª»»½áÊø±êÖ¾
*                       - ADC_FLAG_JEOC: ×¢Èë×é×ª»»½áÊø±êÖ¾
*                       - ADC_FLAG_JSTRT: ×¢Èë×é×ª»»¿ªÊ¼±êÖ¾
*                       - ADC_FLAG_STRT: ³£¹æ×é×ª»»¿ªÊ¼±êÖ¾
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_ClearFlag(ADC_TypeDef* ADCx, u8 ADC_FLAG)
{
  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CLEAR_FLAG(ADC_FLAG));

  /* Clear the selected ADC flags [Çå³ıÑ¡ÔñµÄADC±êÖ¾]*/
  ADCx->SR = ~(u32)ADC_FLAG;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_GetITStatus
* ¹¦ÄÜÃèÊö: ¼ì²éÖ¸¶¨ADCµÄÖĞ¶ÏÊÇ·ñ·¢Éú.
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)ADC_IT:ĞèÒª¼ì²éµÄADCÖĞ¶ÏÔ´.
*                    ¿ÉÒÔÊ¹ÓÃÏÂÊöÖµµÄÒ»¸ö»òÕß¼¸¸öÖµµÄ×éºÏ:
*                       - ADC_IT_EOC: EOCÖĞ¶ÏÆÁ±Î
*                       - ADC_IT_AWD: AWDOGÖĞ¶ÏÆÁ±Î
*                       - ADC_IT_JEOC: JEOCÖĞ¶ÏÆÁ±Î
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ADC_ITµÄĞÂ×´Ì¬£¨SET»òRESET£©.
*******************************************************************************/
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, u16 ADC_IT)
{
  ITStatus bitstatus = RESET;
  u32 itmask = 0, enablestatus = 0;

  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_GET_IT(ADC_IT));

  /* Get the ADC IT index [È¡µÃADCÖĞ¶ÏºÅ]*/
  itmask = ADC_IT >> 8;

  /* Get the ADC_IT enable bit status [È¡µÃADCÖĞ¶ÏÔÊĞí±êÖ¾Î»]*/
  enablestatus = (ADCx->CR1 & (u8)ADC_IT) ;

  /* Check the status of the specified ADC interrupt [¼ì²éÖ¸¶¨ADCÖĞ¶Ï±êÖ¾Î»]*/
  if (((ADCx->SR & itmask) != (u32)RESET) && enablestatus)
  {
    /* ADC_IT is set [ÖÃÎ»ADC_IT]*/
    bitstatus = SET;
  }
  else
  {
    /* ADC_IT is reset [¸´Î»ADC_IT]*/
    bitstatus = RESET;
  }

  /* Return the ADC_IT status [·µ»ØADC_IT×´Ì¬]*/
  return  bitstatus;
}

/*******************************************************************************
* º¯ÊıÃû³Æ: ADC_ClearITPendingBit
* ¹¦ÄÜÃèÊö: Çå³ıADCxµÄÖĞ¶Ï¹ÒÆğÎ».
* ÊäÈë²ÎÊı: (1)ADCx:ÆäÖĞx¿ÉÒÔÊÇ1¡¢2»ò3£¬ÓÃÀ´Ñ¡ÔñADCÍâÎ§Ä£¿é.
*           (2)ADC_IT:ĞèÒª¼ì²éµÄADCÖĞ¶ÏÔ´.
*                    ¿ÉÒÔÊ¹ÓÃÏÂÊöÖµµÄÒ»¸ö»òÕß¼¸¸öÖµµÄ×éºÏ:
*                       - ADC_IT_EOC: EOCÖĞ¶ÏÆÁ±Î
*                       - ADC_IT_AWD: AWDOGÖĞ¶ÏÆÁ±Î
*                       - ADC_IT_JEOC: JEOCÖĞ¶ÏÆÁ±Î
* Êä³ö²ÎÊı: ÎŞ
* ·µ»Ø²ÎÊı: ÎŞ
*******************************************************************************/
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, u16 ADC_IT)
{
  u8 itmask = 0;

  /* Check the parameters [¼ì²é²ÎÊı] */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_IT(ADC_IT));

  /* Get the ADC IT index [È¡µÃADCÖĞ¶ÏºÅ]*/
  itmask = (u8)(ADC_IT >> 8);

  /* Clear the selected ADC interrupt pending bits [Çå³ıÑ¡ÔñµÄADCÖĞ¶Ï¹ÒÆğÎ»]*/
  ADCx->SR = ~(u32)itmask;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****ÎÄ¼ş½áÊø****/
