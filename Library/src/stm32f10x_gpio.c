/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_gpio.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有GPIO的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
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

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: GPIO_DeInit
* 功能描述: 将GPIO部件的寄存器复位为默认值
* 输入参数: GPIOx：x可为A到G来选择特定的GPIO设备
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters [检查参数]*/
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
* 函数名称: GPIO_AFIODeInit
* 功能描述: 将备用功能（重映射，事件控制及EXTI配置）寄存器复位为默认值
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void GPIO_AFIODeInit(void)
{
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, DISABLE);
}

/*******************************************************************************
* 函数名称: GPIO_Init
* 功能描述: 按照GPIO_InitStruct的特定参数初始化GPIO部件
* 输入参数: (1)GPIOx：x可为A到G来选择特定的GPIO设备
*           (2)GPIO_InitStruct：指向GPIO_InitTypeDef结构的指针，它包含特定GPIO部件的配置信息.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
  u32 currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
  u32 tmpreg = 0x00, pinmask = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
  assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));  
  
/*---------------------------- GPIO Mode Configuration [GPIO模式配置]-----------------------*/
  currentmode = ((u32)GPIO_InitStruct->GPIO_Mode) & ((u32)0x0F);

  if ((((u32)GPIO_InitStruct->GPIO_Mode) & ((u32)0x10)) != 0x00)
  { 
    /* Check the parameters [检查参数]*/
    assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
    /* Output mode [输出模式]*/
    currentmode |= (u32)GPIO_InitStruct->GPIO_Speed;
  }

/*---------------------------- GPIO CRL Configuration [GPIO端口控制寄存器低字节配置]------------------------*/
  /* Configure the eight low port pins [配置低8位端口管脚]*/
  if (((u32)GPIO_InitStruct->GPIO_Pin & ((u32)0x00FF)) != 0x00)
  {
    tmpreg = GPIOx->CRL;

    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = ((u32)0x01) << pinpos;
      /* Get the port pins position [取得端口引脚的位置]*/
      currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;

      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* Clear the corresponding low control register bits [清除相应的低位控制寄存器]*/
        pinmask = ((u32)0x0F) << pos;
        tmpreg &= ~pinmask;

        /* Write the mode configuration in the corresponding bits [写相应的模式配置位]*/
        tmpreg |= (currentmode << pos);

        /* Reset the corresponding ODR bit [复位相应的输出数据寄存器位]*/
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((u32)0x01) << pinpos);
        }
        else
        {
          /* Set the corresponding ODR bit [置位相应的输出数据寄存器位]*/
          if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
          {
            GPIOx->BSRR = (((u32)0x01) << pinpos);
          }
        }
      }
    }
    GPIOx->CRL = tmpreg;
  }

/*---------------------------- GPIO CRH Configuration [GPIO端口控制寄存器高字节配置]------------------------*/
  /* Configure the eight high port pins [配置高8位端口引脚]*/
  if (GPIO_InitStruct->GPIO_Pin > 0x00FF)
  {
    tmpreg = GPIOx->CRH;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = (((u32)0x01) << (pinpos + 0x08));
      /* Get the port pins position [取得端口引脚的位置]*/
      currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* Clear the corresponding high control register bits [清除相应的高位控制寄存器]*/
        pinmask = ((u32)0x0F) << pos;
        tmpreg &= ~pinmask;

        /* Write the mode configuration in the corresponding bits [写相应的模式配置位]*/
        tmpreg |= (currentmode << pos);

        /* Reset the corresponding ODR bit [复位相应的输出数据寄存器位]*/
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((u32)0x01) << (pinpos + 0x08));
        }
        /* Set the corresponding ODR bit [置位相应的输出数据寄存器位]*/
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
* 函数名称: GPIO_StructInit
* 功能描述: 为GPIO_InitStruct各成员赋缺省值。
* 输入参数: GPIO_InitStruct：指向GPIO_InitTypeDef结构，该结构将被初始化.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
  /* Reset GPIO init structure parameters values [复位GPIO初始化结构参数值]*/
  GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_All;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
}

/*******************************************************************************
* 函数名称: GPIO_ReadInputDataBit
* 功能描述: 读取特定的输入引脚
* 输入参数: (1)GPIOx：x可为A到G来选择特定的GPIO设备
*           (2)GPIO_Pin：被读取的引脚位。 这个参数GPIO_Pin_x可以选择0至15。
* 输出参数: 无
* 返回参数: 输入端口引脚值.
*******************************************************************************/
u8 GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
  u8 bitstatus = 0x00;
  
  /* Check the parameters [检查参数]*/
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
* 函数名称: GPIO_ReadInputData
* 功能描述: 从特定的GPIO输入数据端口读取.
* 输入参数: GPIOx：x可为A到G来选择特定的GPIO设备.
* 输出参数: 无
* 返回参数: GPIO输入数据端口值.
*******************************************************************************/
u16 GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  return ((u16)GPIOx->IDR);
}

/*******************************************************************************
* 函数名称: GPIO_ReadOutputDataBit
* 功能描述: 从特定的数据输出端口位读取.
* 输入参数: (1)GPIOx：x可为A到G来选择特定的GPIO设备.
*           (2)GPIO_Pin：需要读取的端口位。这个参数GPIO_Pin_x可以选择0至15。
* 输出参数: 无
* 返回参数: 输出端口引脚值.
*******************************************************************************/
u8 GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
  u8 bitstatus = 0x00;

  /* Check the parameters [检查参数]*/
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
* 函数名称: GPIO_ReadOutputData
* 功能描述: 从特定的GPIO输出数据端口读取数据.
* 输入参数: GPIOx：x可为A到G来选择特定的GPIO设备.
* 输出参数: 无
* 返回参数: GPIO输出数据端口值.
*******************************************************************************/
u16 GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    
  return ((u16)GPIOx->ODR);
}

/*******************************************************************************
* 函数名称: GPIO_SetBits
* 功能描述: 置位选定的数据端口位.
* 输入参数: (1)GPIOx：x可为A到G来选择特定的GPIO设备.
*           (2)GPIO_Pin:确定要写入的端口位。该参数可为GPIO_Pin_x的任意组合,其中x为0到15。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void GPIO_SetBits(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BSRR = GPIO_Pin;
}

/*******************************************************************************
* 函数名称: GPIO_ResetBits
* 功能描述: 清除选定的数据端口位.
* 输入参数: (1)GPIOx：x可为A到G来选择特定的GPIO设备.
*           (2)GPIO_Pin：指定被写入的端口位。该参数可为GPIO_Pin_x的任意组合,其中x为0到15。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BRR = GPIO_Pin;
}

/*******************************************************************************
* 函数名称: GPIO_WriteBit
* 功能描述: 置位或清除选定的数据端口位
* 输入参数: (1)GPIOx:x可为A到G来选择特定的GPIO设备.
*           (2)GPIO_Pin:指定被写入的端口位。该参数可为GPIO_Pin_x的任意组合,其中x为0到15。
*           (3)BitVal:该参数指定被写入到选定位的值。
*              BitVal必须为BitAction枚举类型值: Bit_RESET:清除端口引脚，Bit_SET:置位端口引脚
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, BitAction BitVal)
{
  /* Check the parameters [检查参数]*/
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
* 函数名称: GPIO_Write
* 功能描述: 写数据到指定的GPIO端口数据寄存器.
* 输入参数: (1)GPIOx:x可为A到G来选择特定的GPIO设备.
*           (2)PortVal：写入到数据端口寄存器的值.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void GPIO_Write(GPIO_TypeDef* GPIOx, u16 PortVal)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  GPIOx->ODR = PortVal;
}

/*******************************************************************************
* 函数名称: GPIO_PinLockConfig
* 功能描述: 锁定GPIO引脚配置寄存器.
* 输入参数: (1)GPIOx:x可为A到G来选择特定的GPIO设备.
*           (2)GPIO_Pin:指定被写入的端口位。该参数可为GPIO_Pin_x的任意组合,其中x为0到15.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
  u32 tmp = 0x00010000;
  
  /* Check the parameters [检查参数]*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  tmp |= GPIO_Pin;
  /* Set LCKK bit [置位锁定寄存器位]*/
  GPIOx->LCKR = tmp;
  /* Reset LCKK bit [复位锁定寄存器位]*/
  GPIOx->LCKR =  GPIO_Pin;
  /* Set LCKK bit [置位锁定寄存器位]*/
  GPIOx->LCKR = tmp;
  /* Read LCKK bit[读锁定寄存器位]*/
  tmp = GPIOx->LCKR;
  /* Read LCKK bit[读锁定寄存器位]*/
  tmp = GPIOx->LCKR;
}

/*******************************************************************************
* 函数名称: GPIO_EventOutputConfig
* 功能描述: Selects the GPIO pin used as Event output.
* 输入参数: (1)GPIO_PortSource：选择GPIO端口用作事件输出源。GPIOx:x可为A到E来选择特定的GPIO设备.
*           (2)GPIO_PinSource：事件输出引脚。该参数可为GPIO_PinSourcex,这里x可为0到15.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void GPIO_EventOutputConfig(u8 GPIO_PortSource, u8 GPIO_PinSource)
{
  u32 tmpreg = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_GPIO_EVENTOUT_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
    
  tmpreg = AFIO->EVCR;
  /* Clear the PORT[6:4] and PIN[3:0] bits [清除PORT[6:4] 和 PIN[3:0]位]*/
  tmpreg &= EVCR_PORTPINCONFIG_MASK;
  tmpreg |= (u32)GPIO_PortSource << 0x04;
  tmpreg |= GPIO_PinSource;

  AFIO->EVCR = tmpreg;
}

/*******************************************************************************
* 函数名称: GPIO_EventOutputCmd
* 功能描述: 使能（或禁能）事件输出
* 输入参数: NewState：事件输出的新状态。这个参数可为：ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void GPIO_EventOutputCmd(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) EVCR_EVOE_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: GPIO_PinRemapConfig
* 功能描述: 改变特定引脚的映射.
* 输入参数: (1)GPIO_Remap：选择引脚进行重映射。
*                    GPIO_Remap 用来改变备用功能映射,它的可用值:
*                       - GPIO_Remap_SPI1    [SPI1备用功能映射]
*                       - GPIO_Remap_I2C1    [I2C1备用功能映射]
*                       - GPIO_Remap_USART1  [USART1备用功能映射]
*                       - GPIO_Remap_USART2  [USART2备用功能映射]
*                       - GPIO_PartialRemap_USART3  [USART3备用功能映射]
*                       - GPIO_FullRemap_USART3     [USART3全部备用功能映射]
*                       - GPIO_PartialRemap_TIM1 [TIM1部分备用功能映射]
*                       - GPIO_FullRemap_TIM1    [TIM1全部备用功能映射]
*                       - GPIO_PartialRemap1_TIM2   [TIM2部分1备用功能映射]
*                       - GPIO_PartialRemap2_TIM2   [TIM2部分2备用功能映射]
*                       - GPIO_FullRemap_TIM2       [TIM2全部备用功能映射]
*                       - GPIO_PartialRemap_TIM3    [TIM3部分备用功能映射]
*                       - GPIO_FullRemap_TIM3       [TIM3全部备用功能映射]
*                       - GPIO_Remap_TIM4   [TIM4备用功能映射]
*                       - GPIO_Remap1_CAN   [CAN部分1备用功能映射]
*                       - GPIO_Remap2_CAN   [CAN部分2备用功能映射]
*                       - GPIO_Remap_PD01   [PD01备用功能映射]
*                       - GPIO_Remap_TIM5CH4_LSI   [TIM5CH4_LSI备用功能映射]
*                       - GPIO_Remap_ADC1_ETRGINJ   [ADC1_ETRGINJ备用功能映射]
*                       - GPIO_Remap_ADC1_ETRGREG   [ADC1_ETRGREG备用功能映射]
*                       - GPIO_Remap_ADC2_ETRGINJ   [ADC2_ETRGINJ备用功能映射]
*                       - GPIO_Remap_ADC2_ETRGREG   [ADC2_ETRGREG备用功能映射]
*                       - GPIO_Remap_SWJ_NoJTRST   [全部SWJ使能(JTAG-DP+ SW-DP)但不包括JTRST]
*                       - GPIO_Remap_SWJ_JTAGDisable [JTAG-DP禁能但SW-DP使能]
*                       - GPIO_Remap_SWJ_Disable   [全部SWJ禁能(JTAG-DP SW-DP)]
*          (2)NewState：端口引脚重映射的新状态该参数可为：ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void GPIO_PinRemapConfig(u32 GPIO_Remap, FunctionalState NewState)
{
  u32 tmp = 0x00, tmp1 = 0x00, tmpreg = 0x00, tmpmask = 0x00;

  /* Check the parameters [检查参数]*/
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
* 函数名称: GPIO_EXTILineConfig
* 功能描述: 选择GPIO引脚作为EXTI线.
* 输入参数: (1)GPIO_PortSource:选择将要用作EXTI线的源的GPIO端口。GPIOx:x可为A到G来选择特定的GPIO设备.
*           (2)GPIO_PinSource:需要配置的EXTI线。这个参数可以是GPIO_PinSourcex,其中x为0到15.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void GPIO_EXTILineConfig(u8 GPIO_PortSource, u8 GPIO_PinSource)
{
  u32 tmp = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_GPIO_EXTI_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
  
  tmp = ((u32)0x0F) << (0x04 * (GPIO_PinSource & (u8)0x03));

  AFIO->EXTICR[GPIO_PinSource >> 0x02] &= ~tmp;
  AFIO->EXTICR[GPIO_PinSource >> 0x02] |= (((u32)GPIO_PortSource) << (0x04 * (GPIO_PinSource & (u8)0x03)));
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
