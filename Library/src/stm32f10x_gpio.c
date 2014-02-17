/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_gpio.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������GPIO�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
/* ------------ RCC registers bit address in the alias region ----------- */
#define AFIO_OFFSET                 (AFIO_BASE - PERIPH_BASE)

/* --- EVENTCR Register ---*/
/* Alias word address of EVOE bit */
#define EVCR_OFFSET                 (AFIO_OFFSET + 0x00)
#define EVOE_BitNumber              ((u8)0x07)
#define EVCR_EVOE_BB                (PERIPH_BB_BASE + (EVCR_OFFSET * 32) + (EVOE_BitNumber * 4))

#define EVCR_PORTPINCONFIG_MASK     ((u16)0xFF80)
#define LSB_MASK                    ((u16)0xFFFF)
#define DBGAFR_POSITION_MASK        ((u32)0x000F0000)
#define DBGAFR_SWJCFG_MASK          ((u32)0xF0FFFFFF)
#define DBGAFR_LOCATION_MASK        ((u32)0x00200000)
#define DBGAFR_NUMBITS_MASK         ((u32)0x00100000)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: GPIO_DeInit
* ��������: ��GPIO�����ļĴ�����λΪĬ��ֵ
* �������: GPIOx��x��ΪA��G��ѡ���ض���GPIO�豸
* �������: ��
* ���ز���: ��
*******************************************************************************/
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters [������]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  switch (*(u32*)&GPIOx)
  {
    case GPIOA_BASE:
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, DISABLE);
      break;

    case GPIOB_BASE:
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, DISABLE);
      break;

    case GPIOC_BASE:
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, DISABLE);
      break;

    case GPIOD_BASE:
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, DISABLE);
      break;
      
    case GPIOE_BASE:
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, DISABLE);
      break; 

    case GPIOF_BASE:
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, DISABLE);
      break;

    case GPIOG_BASE:
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, DISABLE);
      break;                       

    default:
      break;
  }
}

/*******************************************************************************
* ��������: GPIO_AFIODeInit
* ��������: �����ù��ܣ���ӳ�䣬�¼����Ƽ�EXTI���ã��Ĵ�����λΪĬ��ֵ
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void GPIO_AFIODeInit(void)
{
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, DISABLE);
}

/*******************************************************************************
* ��������: GPIO_Init
* ��������: ����GPIO_InitStruct���ض�������ʼ��GPIO����
* �������: (1)GPIOx��x��ΪA��G��ѡ���ض���GPIO�豸
*           (2)GPIO_InitStruct��ָ��GPIO_InitTypeDef�ṹ��ָ�룬�������ض�GPIO������������Ϣ.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
  u32 currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
  u32 tmpreg = 0x00, pinmask = 0x00;

  /* Check the parameters [������]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
  assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));  
  
/*---------------------------- GPIO Mode Configuration [GPIOģʽ����]-----------------------*/
  currentmode = ((u32)GPIO_InitStruct->GPIO_Mode) & ((u32)0x0F);

  if ((((u32)GPIO_InitStruct->GPIO_Mode) & ((u32)0x10)) != 0x00)
  { 
    /* Check the parameters [������]*/
    assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
    /* Output mode [���ģʽ]*/
    currentmode |= (u32)GPIO_InitStruct->GPIO_Speed;
  }

/*---------------------------- GPIO CRL Configuration [GPIO�˿ڿ��ƼĴ������ֽ�����]------------------------*/
  /* Configure the eight low port pins [���õ�8λ�˿ڹܽ�]*/
  if (((u32)GPIO_InitStruct->GPIO_Pin & ((u32)0x00FF)) != 0x00)
  {
    tmpreg = GPIOx->CRL;

    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = ((u32)0x01) << pinpos;
      /* Get the port pins position [ȡ�ö˿����ŵ�λ��]*/
      currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;

      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* Clear the corresponding low control register bits [�����Ӧ�ĵ�λ���ƼĴ���]*/
        pinmask = ((u32)0x0F) << pos;
        tmpreg &= ~pinmask;

        /* Write the mode configuration in the corresponding bits [д��Ӧ��ģʽ����λ]*/
        tmpreg |= (currentmode << pos);

        /* Reset the corresponding ODR bit [��λ��Ӧ��������ݼĴ���λ]*/
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((u32)0x01) << pinpos);
        }
        else
        {
          /* Set the corresponding ODR bit [��λ��Ӧ��������ݼĴ���λ]*/
          if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
          {
            GPIOx->BSRR = (((u32)0x01) << pinpos);
          }
        }
      }
    }
    GPIOx->CRL = tmpreg;
  }

/*---------------------------- GPIO CRH Configuration [GPIO�˿ڿ��ƼĴ������ֽ�����]------------------------*/
  /* Configure the eight high port pins [���ø�8λ�˿�����]*/
  if (GPIO_InitStruct->GPIO_Pin > 0x00FF)
  {
    tmpreg = GPIOx->CRH;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = (((u32)0x01) << (pinpos + 0x08));
      /* Get the port pins position [ȡ�ö˿����ŵ�λ��]*/
      currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* Clear the corresponding high control register bits [�����Ӧ�ĸ�λ���ƼĴ���]*/
        pinmask = ((u32)0x0F) << pos;
        tmpreg &= ~pinmask;

        /* Write the mode configuration in the corresponding bits [д��Ӧ��ģʽ����λ]*/
        tmpreg |= (currentmode << pos);

        /* Reset the corresponding ODR bit [��λ��Ӧ��������ݼĴ���λ]*/
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((u32)0x01) << (pinpos + 0x08));
        }
        /* Set the corresponding ODR bit [��λ��Ӧ��������ݼĴ���λ]*/
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
        {
          GPIOx->BSRR = (((u32)0x01) << (pinpos + 0x08));
        }
      }
    }
    GPIOx->CRH = tmpreg;
  }
}

/*******************************************************************************
* ��������: GPIO_StructInit
* ��������: ΪGPIO_InitStruct����Ա��ȱʡֵ��
* �������: GPIO_InitStruct��ָ��GPIO_InitTypeDef�ṹ���ýṹ������ʼ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
  /* Reset GPIO init structure parameters values [��λGPIO��ʼ���ṹ����ֵ]*/
  GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_All;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
}

/*******************************************************************************
* ��������: GPIO_ReadInputDataBit
* ��������: ��ȡ�ض�����������
* �������: (1)GPIOx��x��ΪA��G��ѡ���ض���GPIO�豸
*           (2)GPIO_Pin������ȡ������λ�� �������GPIO_Pin_x����ѡ��0��15��
* �������: ��
* ���ز���: ����˿�����ֵ.
*******************************************************************************/
u8 GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
  u8 bitstatus = 0x00;
  
  /* Check the parameters [������]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
  
  if ((GPIOx->IDR & GPIO_Pin) != (u32)Bit_RESET)
  {
    bitstatus = (u8)Bit_SET;
  }
  else
  {
    bitstatus = (u8)Bit_RESET;
  }
  return bitstatus;
}

/*******************************************************************************
* ��������: GPIO_ReadInputData
* ��������: ���ض���GPIO�������ݶ˿ڶ�ȡ.
* �������: GPIOx��x��ΪA��G��ѡ���ض���GPIO�豸.
* �������: ��
* ���ز���: GPIO�������ݶ˿�ֵ.
*******************************************************************************/
u16 GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters [������]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  return ((u16)GPIOx->IDR);
}

/*******************************************************************************
* ��������: GPIO_ReadOutputDataBit
* ��������: ���ض�����������˿�λ��ȡ.
* �������: (1)GPIOx��x��ΪA��G��ѡ���ض���GPIO�豸.
*           (2)GPIO_Pin����Ҫ��ȡ�Ķ˿�λ���������GPIO_Pin_x����ѡ��0��15��
* �������: ��
* ���ز���: ����˿�����ֵ.
*******************************************************************************/
u8 GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
  u8 bitstatus = 0x00;

  /* Check the parameters [������]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
  
  if ((GPIOx->ODR & GPIO_Pin) != (u32)Bit_RESET)
  {
    bitstatus = (u8)Bit_SET;
  }
  else
  {
    bitstatus = (u8)Bit_RESET;
  }
  return bitstatus;
}

/*******************************************************************************
* ��������: GPIO_ReadOutputData
* ��������: ���ض���GPIO������ݶ˿ڶ�ȡ����.
* �������: GPIOx��x��ΪA��G��ѡ���ض���GPIO�豸.
* �������: ��
* ���ز���: GPIO������ݶ˿�ֵ.
*******************************************************************************/
u16 GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters [������]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    
  return ((u16)GPIOx->ODR);
}

/*******************************************************************************
* ��������: GPIO_SetBits
* ��������: ��λѡ�������ݶ˿�λ.
* �������: (1)GPIOx��x��ΪA��G��ѡ���ض���GPIO�豸.
*           (2)GPIO_Pin:ȷ��Ҫд��Ķ˿�λ���ò�����ΪGPIO_Pin_x���������,����xΪ0��15��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void GPIO_SetBits(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
  /* Check the parameters [������]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BSRR = GPIO_Pin;
}

/*******************************************************************************
* ��������: GPIO_ResetBits
* ��������: ���ѡ�������ݶ˿�λ.
* �������: (1)GPIOx��x��ΪA��G��ѡ���ض���GPIO�豸.
*           (2)GPIO_Pin��ָ����д��Ķ˿�λ���ò�����ΪGPIO_Pin_x���������,����xΪ0��15��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
  /* Check the parameters [������]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BRR = GPIO_Pin;
}

/*******************************************************************************
* ��������: GPIO_WriteBit
* ��������: ��λ�����ѡ�������ݶ˿�λ
* �������: (1)GPIOx:x��ΪA��G��ѡ���ض���GPIO�豸.
*           (2)GPIO_Pin:ָ����д��Ķ˿�λ���ò�����ΪGPIO_Pin_x���������,����xΪ0��15��
*           (3)BitVal:�ò���ָ����д�뵽ѡ��λ��ֵ��
*              BitVal����ΪBitActionö������ֵ: Bit_RESET:����˿����ţ�Bit_SET:��λ�˿�����
* �������: ��
* ���ز���: ��
*******************************************************************************/
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, BitAction BitVal)
{
  /* Check the parameters [������]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
  assert_param(IS_GPIO_BIT_ACTION(BitVal)); 
  
  if (BitVal != Bit_RESET)
  {
    GPIOx->BSRR = GPIO_Pin;
  }
  else
  {
    GPIOx->BRR = GPIO_Pin;
  }
}

/*******************************************************************************
* ��������: GPIO_Write
* ��������: д���ݵ�ָ����GPIO�˿����ݼĴ���.
* �������: (1)GPIOx:x��ΪA��G��ѡ���ض���GPIO�豸.
*           (2)PortVal��д�뵽���ݶ˿ڼĴ�����ֵ.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void GPIO_Write(GPIO_TypeDef* GPIOx, u16 PortVal)
{
  /* Check the parameters [������]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  GPIOx->ODR = PortVal;
}

/*******************************************************************************
* ��������: GPIO_PinLockConfig
* ��������: ����GPIO�������üĴ���.
* �������: (1)GPIOx:x��ΪA��G��ѡ���ض���GPIO�豸.
*           (2)GPIO_Pin:ָ����д��Ķ˿�λ���ò�����ΪGPIO_Pin_x���������,����xΪ0��15.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
  u32 tmp = 0x00010000;
  
  /* Check the parameters [������]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  tmp |= GPIO_Pin;
  /* Set LCKK bit [��λ�����Ĵ���λ]*/
  GPIOx->LCKR = tmp;
  /* Reset LCKK bit [��λ�����Ĵ���λ]*/
  GPIOx->LCKR =  GPIO_Pin;
  /* Set LCKK bit [��λ�����Ĵ���λ]*/
  GPIOx->LCKR = tmp;
  /* Read LCKK bit[�������Ĵ���λ]*/
  tmp = GPIOx->LCKR;
  /* Read LCKK bit[�������Ĵ���λ]*/
  tmp = GPIOx->LCKR;
}

/*******************************************************************************
* ��������: GPIO_EventOutputConfig
* ��������: Selects the GPIO pin used as Event output.
* �������: (1)GPIO_PortSource��ѡ��GPIO�˿������¼����Դ��GPIOx:x��ΪA��E��ѡ���ض���GPIO�豸.
*           (2)GPIO_PinSource���¼�������š��ò�����ΪGPIO_PinSourcex,����x��Ϊ0��15.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void GPIO_EventOutputConfig(u8 GPIO_PortSource, u8 GPIO_PinSource)
{
  u32 tmpreg = 0x00;

  /* Check the parameters [������]*/
  assert_param(IS_GPIO_EVENTOUT_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
    
  tmpreg = AFIO->EVCR;
  /* Clear the PORT[6:4] and PIN[3:0] bits [���PORT[6:4] �� PIN[3:0]λ]*/
  tmpreg &= EVCR_PORTPINCONFIG_MASK;
  tmpreg |= (u32)GPIO_PortSource << 0x04;
  tmpreg |= GPIO_PinSource;

  AFIO->EVCR = tmpreg;
}

/*******************************************************************************
* ��������: GPIO_EventOutputCmd
* ��������: ʹ�ܣ�����ܣ��¼����
* �������: NewState���¼��������״̬�����������Ϊ��ENABLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void GPIO_EventOutputCmd(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) EVCR_EVOE_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: GPIO_PinRemapConfig
* ��������: �ı��ض����ŵ�ӳ��.
* �������: (1)GPIO_Remap��ѡ�����Ž�����ӳ�䡣
*                    GPIO_Remap �����ı䱸�ù���ӳ��,���Ŀ���ֵ:
*                       - GPIO_Remap_SPI1    [SPI1���ù���ӳ��]
*                       - GPIO_Remap_I2C1    [I2C1���ù���ӳ��]
*                       - GPIO_Remap_USART1  [USART1���ù���ӳ��]
*                       - GPIO_Remap_USART2  [USART2���ù���ӳ��]
*                       - GPIO_PartialRemap_USART3  [USART3���ù���ӳ��]
*                       - GPIO_FullRemap_USART3     [USART3ȫ�����ù���ӳ��]
*                       - GPIO_PartialRemap_TIM1 [TIM1���ֱ��ù���ӳ��]
*                       - GPIO_FullRemap_TIM1    [TIM1ȫ�����ù���ӳ��]
*                       - GPIO_PartialRemap1_TIM2   [TIM2����1���ù���ӳ��]
*                       - GPIO_PartialRemap2_TIM2   [TIM2����2���ù���ӳ��]
*                       - GPIO_FullRemap_TIM2       [TIM2ȫ�����ù���ӳ��]
*                       - GPIO_PartialRemap_TIM3    [TIM3���ֱ��ù���ӳ��]
*                       - GPIO_FullRemap_TIM3       [TIM3ȫ�����ù���ӳ��]
*                       - GPIO_Remap_TIM4   [TIM4���ù���ӳ��]
*                       - GPIO_Remap1_CAN   [CAN����1���ù���ӳ��]
*                       - GPIO_Remap2_CAN   [CAN����2���ù���ӳ��]
*                       - GPIO_Remap_PD01   [PD01���ù���ӳ��]
*                       - GPIO_Remap_TIM5CH4_LSI   [TIM5CH4_LSI���ù���ӳ��]
*                       - GPIO_Remap_ADC1_ETRGINJ   [ADC1_ETRGINJ���ù���ӳ��]
*                       - GPIO_Remap_ADC1_ETRGREG   [ADC1_ETRGREG���ù���ӳ��]
*                       - GPIO_Remap_ADC2_ETRGINJ   [ADC2_ETRGINJ���ù���ӳ��]
*                       - GPIO_Remap_ADC2_ETRGREG   [ADC2_ETRGREG���ù���ӳ��]
*                       - GPIO_Remap_SWJ_NoJTRST   [ȫ��SWJʹ��(JTAG-DP+ SW-DP)��������JTRST]
*                       - GPIO_Remap_SWJ_JTAGDisable [JTAG-DP���ܵ�SW-DPʹ��]
*                       - GPIO_Remap_SWJ_Disable   [ȫ��SWJ����(JTAG-DP SW-DP)]
*          (2)NewState���˿�������ӳ�����״̬�ò�����Ϊ��ENABLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void GPIO_PinRemapConfig(u32 GPIO_Remap, FunctionalState NewState)
{
  u32 tmp = 0x00, tmp1 = 0x00, tmpreg = 0x00, tmpmask = 0x00;

  /* Check the parameters [������]*/
  assert_param(IS_GPIO_REMAP(GPIO_Remap));
  assert_param(IS_FUNCTIONAL_STATE(NewState));  
  
  tmpreg = AFIO->MAPR;

  tmpmask = (GPIO_Remap & DBGAFR_POSITION_MASK) >> 0x10;
  tmp = GPIO_Remap & LSB_MASK;

  if ((GPIO_Remap & (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK)) == (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK))
  {
    tmpreg &= DBGAFR_SWJCFG_MASK;
    AFIO->MAPR &= DBGAFR_SWJCFG_MASK;
  }
  else if ((GPIO_Remap & DBGAFR_NUMBITS_MASK) == DBGAFR_NUMBITS_MASK)
  {
    tmp1 = ((u32)0x03) << tmpmask;
    tmpreg &= ~tmp1;
    tmpreg |= ~DBGAFR_SWJCFG_MASK;
  }
  else
  {
    tmpreg &= ~(tmp << ((GPIO_Remap >> 0x15)*0x10));
    tmpreg |= ~DBGAFR_SWJCFG_MASK;
  }

  if (NewState != DISABLE)
  {
    tmpreg |= (tmp << ((GPIO_Remap >> 0x15)*0x10));
  }

  AFIO->MAPR = tmpreg;
}

/*******************************************************************************
* ��������: GPIO_EXTILineConfig
* ��������: ѡ��GPIO������ΪEXTI��.
* �������: (1)GPIO_PortSource:ѡ��Ҫ����EXTI�ߵ�Դ��GPIO�˿ڡ�GPIOx:x��ΪA��G��ѡ���ض���GPIO�豸.
*           (2)GPIO_PinSource:��Ҫ���õ�EXTI�ߡ��������������GPIO_PinSourcex,����xΪ0��15.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void GPIO_EXTILineConfig(u8 GPIO_PortSource, u8 GPIO_PinSource)
{
  u32 tmp = 0x00;

  /* Check the parameters [������]*/
  assert_param(IS_GPIO_EXTI_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
  
  tmp = ((u32)0x0F) << (0x04 * (GPIO_PinSource & (u8)0x03));

  AFIO->EXTICR[GPIO_PinSource >> 0x02] &= ~tmp;
  AFIO->EXTICR[GPIO_PinSource >> 0x02] |= (((u32)GPIO_PortSource) << (0x04 * (GPIO_PinSource & (u8)0x03)));
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
