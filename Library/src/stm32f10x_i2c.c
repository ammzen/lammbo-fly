/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_i2c.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有I2C的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_i2c.h"
#include "stm32f10x_rcc.h"
     
/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
/* I2C SPE mask */
#define CR1_PE_Set              ((u16)0x0001)
#define CR1_PE_Reset            ((u16)0xFFFE)

/* I2C START mask */
#define CR1_START_Set           ((u16)0x0100)
#define CR1_START_Reset         ((u16)0xFEFF)

/* I2C STOP mask */
#define CR1_STOP_Set            ((u16)0x0200)
#define CR1_STOP_Reset          ((u16)0xFDFF)

/* I2C ACK mask */
#define CR1_ACK_Set             ((u16)0x0400)
#define CR1_ACK_Reset           ((u16)0xFBFF)

/* I2C ENGC mask */
#define CR1_ENGC_Set            ((u16)0x0040)
#define CR1_ENGC_Reset          ((u16)0xFFBF)

/* I2C SWRST mask */
#define CR1_SWRST_Set           ((u16)0x8000)
#define CR1_SWRST_Reset         ((u16)0x7FFF)

/* I2C PEC mask */
#define CR1_PEC_Set             ((u16)0x1000)
#define CR1_PEC_Reset           ((u16)0xEFFF)

/* I2C ENPEC mask */
#define CR1_ENPEC_Set           ((u16)0x0020)
#define CR1_ENPEC_Reset         ((u16)0xFFDF)

/* I2C ENARP mask */
#define CR1_ENARP_Set           ((u16)0x0010)
#define CR1_ENARP_Reset         ((u16)0xFFEF)

/* I2C NOSTRETCH mask */
#define CR1_NOSTRETCH_Set       ((u16)0x0080)
#define CR1_NOSTRETCH_Reset     ((u16)0xFF7F)

/* I2C registers Masks */
#define CR1_CLEAR_Mask          ((u16)0xFBF5)

/* I2C DMAEN mask */
#define CR2_DMAEN_Set           ((u16)0x0800)
#define CR2_DMAEN_Reset         ((u16)0xF7FF)

/* I2C LAST mask */
#define CR2_LAST_Set            ((u16)0x1000)
#define CR2_LAST_Reset          ((u16)0xEFFF)

/* I2C FREQ mask */
#define CR2_FREQ_Reset          ((u16)0xFFC0)

/* I2C ADD0 mask */
#define OAR1_ADD0_Set           ((u16)0x0001)
#define OAR1_ADD0_Reset         ((u16)0xFFFE)

/* I2C ENDUAL mask */
#define OAR2_ENDUAL_Set         ((u16)0x0001)
#define OAR2_ENDUAL_Reset       ((u16)0xFFFE)

/* I2C ADD2 mask */
#define OAR2_ADD2_Reset         ((u16)0xFF01)

/* I2C F/S mask */
#define CCR_FS_Set              ((u16)0x8000)

/* I2C CCR mask */
#define CCR_CCR_Set             ((u16)0x0FFF)

/* I2C FLAG mask */
#define FLAG_Mask               ((u32)0x00FFFFFF)

/* I2C Interrupt Enable mask */
#define ITEN_Mask               ((u32)0x07000000)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: I2C_DeInit
* 功能描述: 将I2Cx外设寄存器复位到它们的默认值.
* 输入参数: I2Cx：x可为1或2来选择I2C外设.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_DeInit(I2C_TypeDef* I2Cx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  switch (*(u32*)&I2Cx)
  {
    case I2C1_BASE:
      /* Enable I2C1 reset state [允许I2C1复位状态]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
      /* Release I2C1 from reset state [解除I2C1复位状态]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
      break;

    case I2C2_BASE:
      /* Enable I2C2 reset state [允许I2C2复位状态]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
      /* Release I2C2 from reset state [解除I2C2复位状态]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
      break;

    default:
      break;
  }
}

/*******************************************************************************
* 函数名称: I2C_Init
* 功能描述: 按照I2C_InitStruct的特定参数初始化I2C部件.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C部件.
*           (2)I2C_InitStruct：指向I2C_InitTypeDef结构，该结构包含了特定I2C部件的配置信息。
* 输出参数: 无
* 返回参数: 无
******************************************************************************/
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct)
{
  u16 tmpreg = 0, freqrange = 0;
  u16 result = 0x04;
  u32 pclk1 = 8000000;
  RCC_ClocksTypeDef  rcc_clocks;

  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_MODE(I2C_InitStruct->I2C_Mode));
  assert_param(IS_I2C_DUTY_CYCLE(I2C_InitStruct->I2C_DutyCycle));
  assert_param(IS_I2C_OWN_ADDRESS1(I2C_InitStruct->I2C_OwnAddress1));
  assert_param(IS_I2C_ACK_STATE(I2C_InitStruct->I2C_Ack));
  assert_param(IS_I2C_ACKNOWLEDGE_ADDRESS(I2C_InitStruct->I2C_AcknowledgedAddress));
  assert_param(IS_I2C_CLOCK_SPEED(I2C_InitStruct->I2C_ClockSpeed));

/*---------------------------- I2Cx CR2 Configuration ------------------------*/
  /* Get the I2Cx CR2 value */
  tmpreg = I2Cx->CR2;
  /* Clear frequency FREQ[5:0] bits */
  tmpreg &= CR2_FREQ_Reset;
  /* Get pclk1 frequency value */
  RCC_GetClocksFreq(&rcc_clocks);
  pclk1 = rcc_clocks.PCLK1_Frequency;
  /* Set frequency bits depending on pclk1 value */
  freqrange = (u16)(pclk1 / 1000000);
  tmpreg |= freqrange;
  /* Write to I2Cx CR2 */
  I2Cx->CR2 = tmpreg;

/*---------------------------- I2Cx CCR Configuration ------------------------*/
  /* Disable the selected I2C peripheral to configure TRISE */
  I2Cx->CR1 &= CR1_PE_Reset;

  /* Reset tmpreg value */
  /* Clear F/S, DUTY and CCR[11:0] bits */
  tmpreg = 0;

  /* Configure speed in standard mode */
  if (I2C_InitStruct->I2C_ClockSpeed <= 100000)
  {
    /* Standard mode speed calculate */
    result = (u16)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed << 1));
    /* Test if CCR value is under 0x4*/
    if (result < 0x04)
    {
      /* Set minimum allowed value */
      result = 0x04;  
    }
    /* Set speed value for standard mode */
    tmpreg |= result;     
    /* Set Maximum Rise Time for standard mode */
    I2Cx->TRISE = freqrange + 1; 
  }
  /* Configure speed in fast mode */
  else /*(I2C_InitStruct->I2C_ClockSpeed <= 400000)*/
  {
    if (I2C_InitStruct->I2C_DutyCycle == I2C_DutyCycle_2)
    {
      /* Fast mode speed calculate: Tlow/Thigh = 2 */
      result = (u16)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed * 3));
    }
    else /*I2C_InitStruct->I2C_DutyCycle == I2C_DutyCycle_16_9*/
    {
      /* Fast mode speed calculate: Tlow/Thigh = 16/9 */
      result = (u16)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed * 25));
      /* Set DUTY bit */
      result |= I2C_DutyCycle_16_9;
    }
    /* Test if CCR value is under 0x1*/
    if ((result & CCR_CCR_Set) == 0)
    {
      /* Set minimum allowed value */
      result |= (u16)0x0001;  
    }
    /* Set speed value and set F/S bit for fast mode */
    tmpreg |= result | CCR_FS_Set;
    /* Set Maximum Rise Time for fast mode */
    I2Cx->TRISE = (u16)(((freqrange * 300) / 1000) + 1);  
  }
  /* Write to I2Cx CCR */
  I2Cx->CCR = tmpreg;

  /* Enable the selected I2C peripheral */
  I2Cx->CR1 |= CR1_PE_Set;

/*---------------------------- I2Cx CR1 Configuration ------------------------*/
  /* Get the I2Cx CR1 value */
  tmpreg = I2Cx->CR1;
  /* Clear ACK, SMBTYPE and  SMBUS bits */
  tmpreg &= CR1_CLEAR_Mask;
  /* Configure I2Cx: mode and acknowledgement */
  /* Set SMBTYPE and SMBUS bits according to I2C_Mode value */
  /* Set ACK bit according to I2C_Ack value */
  tmpreg |= (u16)((u32)I2C_InitStruct->I2C_Mode | I2C_InitStruct->I2C_Ack);
  /* Write to I2Cx CR1 */
  I2Cx->CR1 = tmpreg;

/*---------------------------- I2Cx OAR1 Configuration -----------------------*/
  /* Set I2Cx Own Address1 and acknowledged address */
  I2Cx->OAR1 = (I2C_InitStruct->I2C_AcknowledgedAddress | I2C_InitStruct->I2C_OwnAddress1);
}

/*******************************************************************************
* 函数名称: I2C_StructInit
* 功能描述: 为I2C_InitStruct的成员赋默认值.
* 输入参数: I2C_InitStruct：指向必须初始化的I2C_InitTypeDef结构.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
{
/*---------------- Reset I2C init structure parameters values ----------------*/
  /* Initialize the I2C_Mode member */
  I2C_InitStruct->I2C_Mode = I2C_Mode_I2C;

  /* Initialize the I2C_DutyCycle member */
  I2C_InitStruct->I2C_DutyCycle = I2C_DutyCycle_2;

  /* Initialize the I2C_OwnAddress1 member */
  I2C_InitStruct->I2C_OwnAddress1 = 0;

  /* Initialize the I2C_Ack member */
  I2C_InitStruct->I2C_Ack = I2C_Ack_Disable;

  /* Initialize the I2C_AcknowledgedAddress member */
  I2C_InitStruct->I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

  /* initialize the I2C_ClockSpeed member */
  I2C_InitStruct->I2C_ClockSpeed = 5000;
}

/*******************************************************************************
* 函数名称: I2C_Cmd
* 功能描述: 使能（或禁能）特定的I2C外设.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)NewState：I2C部件的新状态.该参数可为：ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C peripheral [使能选择的I2C设备]*/
    I2Cx->CR1 |= CR1_PE_Set;
  }
  else
  {
    /* Disable the selected I2C peripheral [禁止选择的I2C设备]*/
    I2Cx->CR1 &= CR1_PE_Reset;
  }
}

/*******************************************************************************
* 函数名称: I2C_DMACmd
* 功能描述: 使能（或禁能）特定的I2C的DMA请求.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C部件.
*           (2)NewState：I2C的DMA传输的新状态.该参数可为：ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_DMACmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C DMA requests [使能选则的I2C DMA请求]*/
    I2Cx->CR2 |= CR2_DMAEN_Set;
  }
  else
  {
    /* Disable the selected I2C DMA requests [禁用选则的I2C DMA请求]*/
    I2Cx->CR2 &= CR2_DMAEN_Reset;
  }
}

/*******************************************************************************
* 函数名称: I2C_DMALastTransferCmd
* 功能描述: 指出下一个DMA传输是最后一个.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C部件.
*           (2)NewState：I2C的最后DMA传输的新状态.该参数为：ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_DMALastTransferCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Next DMA transfer is the last transfer [下一个传输是最后一个传输]*/
    I2Cx->CR2 |= CR2_LAST_Set;
  }
  else
  {
    /* Next DMA transfer is not the last transfer [下一个传输不是最后一个传输]*/
    I2Cx->CR2 &= CR2_LAST_Reset;
  }
}

/*******************************************************************************
* 函数名称: I2C_GenerateSTART
* 功能描述: 产生I2C通信的起始条件.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)NewState：I2C起始条件生成的新状态.该参数为：ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无.
*******************************************************************************/
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Generate a START condition [产生一个起始条件]*/
    I2Cx->CR1 |= CR1_START_Set;
  }
  else
  {
    /* Disable the START condition generation [禁止一个起始条件产生]*/
    I2Cx->CR1 &= CR1_START_Reset;
  }
}

/*******************************************************************************
* 函数名称: I2C_GenerateSTOP
* 功能描述: 产生I2C通信的停止条件.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)NewState：I2C停止条件生成的新状态.该参数为：ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无.
*******************************************************************************/
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Generate a STOP condition [产生一个停止条件]*/
    I2Cx->CR1 |= CR1_STOP_Set;
  }
  else
  {
    /* Disable the STOP condition generation [禁止一个停止条件产生]*/
    I2Cx->CR1 &= CR1_STOP_Reset;
  }
}

/*******************************************************************************
* 函数名称: I2C_AcknowledgeConfig
* 功能描述: 使能（或禁能）特定的I2C的确认特性.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)NewState：I2C确认特性的新状态。该参数为：ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无.
*******************************************************************************/
void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the acknowledgement [允许确认]*/
    I2Cx->CR1 |= CR1_ACK_Set;
  }
  else
  {
    /* Disable the acknowledgement [禁止确认]*/
    I2Cx->CR1 &= CR1_ACK_Reset;
  }
}

/*******************************************************************************
* 函数名称: I2C_OwnAddress2Config
* 功能描述: 配置特定的I2C自地址2.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)地址：确认7位I2C自地址2.
* 输出参数: 无
* 返回参数: 无.
*******************************************************************************/
void I2C_OwnAddress2Config(I2C_TypeDef* I2Cx, u8 Address)
{
  u16 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Get the old register value [取得旧的寄存器值]*/
  tmpreg = I2Cx->OAR2;
  /* Reset I2Cx Own address2 bit [7:1] [复位I2Cx自己地址位[7:1]]*/
  tmpreg &= OAR2_ADD2_Reset;
  /* Set I2Cx Own address2 [设置I2Cx自己地址]*/
  tmpreg |= (u16)(Address & (u16)0x00FE);
  /* Store the new register value [保存新的寄存器值]*/
  I2Cx->OAR2 = tmpreg;
}

/*******************************************************************************
* 函数名称: I2C_DualAddressCmd
* 功能描述: 使能（或禁能）特定的I2C双重地址模式.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)NewState：I2C双重地址模式的新状态。该参数为：ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_DualAddressCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable dual addressing mode [允许双地址模式]*/
    I2Cx->OAR2 |= OAR2_ENDUAL_Set;
  }
  else
  {
    /* Disable dual addressing mode [禁止双地址模式]*/
    I2Cx->OAR2 &= OAR2_ENDUAL_Reset;
  }
}

/*******************************************************************************
* 函数名称: I2C_GeneralCallCmd
* 功能描述: 使能（或禁能）特定I2C的通用调用特性.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)NewState：I2C通用调用的新状态。该参数为：ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_GeneralCallCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable generall call [允许通用调用]*/
    I2Cx->CR1 |= CR1_ENGC_Set;
  }
  else
  {
    /* Disable generall call [禁止通用调用]*/
    I2Cx->CR1 &= CR1_ENGC_Reset;
  }
}

/*******************************************************************************
* 函数名称: I2C_ITConfig
* 功能描述: 使能（或禁能）特定的I2C中断.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)I2C_IT：需要使能(或禁能)的I2C中断源。. 
*                    这个参数可以是下面任意值得组合:
*                       - I2C_IT_BUF: 缓冲区中断屏蔽
*                       - I2C_IT_EVT: 事件中断屏蔽
*                       - I2C_IT_ERR: 错误中断屏蔽
*           (3)NewState：指定的I2C中断的新状态。该参数为：ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_ITConfig(I2C_TypeDef* I2Cx, u16 I2C_IT, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_I2C_CONFIG_IT(I2C_IT));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected I2C interrupts [使能选择的I2C中断]*/
    I2Cx->CR2 |= I2C_IT;
  }
  else
  {
    /* Disable the selected I2C interrupts [禁止选择的I2C中断]*/
    I2Cx->CR2 &= (u16)~I2C_IT;
  }
}

/*******************************************************************************
* 函数名称: I2C_SendData
* 功能描述: 通过I2C发送一字节数据.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)Data：需要传送的字节.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_SendData(I2C_TypeDef* I2Cx, u8 Data)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Write in the DR register the data to be sent [写发送的数据到DR寄存器]*/
  I2Cx->DR = Data;
}

/*******************************************************************************
* 函数名称: I2C_ReceiveData
* 功能描述: 返回I2Cx外设最近接收的数据.
* 输入参数: I2Cx：x可为1或2来选择I2C外设.
* 输出参数: 无
* 返回参数: 接收数据值.
*******************************************************************************/
u8 I2C_ReceiveData(I2C_TypeDef* I2Cx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Return the data in the DR register [返回DR寄存器的数据]*/
  return (u8)I2Cx->DR;
}

/*******************************************************************************
* 函数名称: I2C_Send7bitAddress
* 功能描述: 传输地址字节来选择从设备.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)Address：将要被发送的从设备地址
*           (3)I2C_Direction:确定I2C设备为主设备或从设备. 
*                    这个参数可以是下面的值之一
*                       - I2C_Direction_Transmitter: 选择发送模式
*                       - I2C_Direction_Receiver: 选择接收模式
* 输出参数: 无
* 返回参数: 无.
*******************************************************************************/
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, u8 Address, u8 I2C_Direction)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_DIRECTION(I2C_Direction));

  /* Test on the direction to set/reset the read/write bit [测试读/写位的状态是置位还是复位]*/
  if (I2C_Direction != I2C_Direction_Transmitter)
  {
    /* Set the address bit0 for read [置位地址位的0位到读状态]*/
    Address |= OAR1_ADD0_Set;
  }
  else
  {
    /* Reset the address bit0 for write [复位地址位的0位到写状态]*/
    Address &= OAR1_ADD0_Reset;
  }
  /* Send the address [发送地址]*/
  I2Cx->DR = Address;
}

/*******************************************************************************
* 函数名称: I2C_ReadRegister
* 功能描述: 读取特定的I2C寄存器并返回其值.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C部件
*           (2)I2C_Register：将要读取的寄存器.
*                    这个参数可以是下面的值之一:
*                       - I2C_Register_CR1:  CR1 寄存器.
*                       - I2C_Register_CR2:   CR2 寄存器.
*                       - I2C_Register_OAR1:  OAR1 寄存器.
*                       - I2C_Register_OAR2:  OAR2 寄存器.
*                       - I2C_Register_DR:    DR 寄存器.
*                       - I2C_Register_SR1:   SR1 寄存器.
*                       - I2C_Register_SR2:   SR2 寄存器.
*                       - I2C_Register_CCR:   CCR 寄存器.
*                       - I2C_Register_TRISE: TRISE 寄存器.
* 输出参数: 无
* 返回参数: 选择寄存器的值.
*******************************************************************************/
u16 I2C_ReadRegister(I2C_TypeDef* I2Cx, u8 I2C_Register)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_REGISTER(I2C_Register));

  /* Return the selected register value [返回选择寄存器的值]*/
  return (*(vu16 *)(*((vu32 *)&I2Cx) + I2C_Register));
}

/*******************************************************************************
* 函数名称: I2C_SoftwareResetCmd
* 功能描述: 使能(或禁止)特定I2C软件复位.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)NewState：I2C软件复位的新状态.
*                    该参数为：ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_SoftwareResetCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Peripheral under reset [外设下复位]*/
    I2Cx->CR1 |= CR1_SWRST_Set;
  }
  else
  {
    /* Peripheral not under reset [外设不下复位]*/
    I2Cx->CR1 &= CR1_SWRST_Reset;
  }
}

/*******************************************************************************
* 函数名称: I2C_SMBusAlertConfig
* 功能描述: 为特定的I2C驱动SMBAlert引脚为高或低.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)I2C_SMBusAlert：SMBusAlert引脚电平。 
*                    这个参数可以是下面的值之一:
*                       - I2C_SMBusAlert_Low: SMBusAlert引脚驱动为低
*                       - I2C_SMBusAlert_High: SMBusAlert引脚驱动为高
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_SMBusAlertConfig(I2C_TypeDef* I2Cx, u16 I2C_SMBusAlert)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_SMBUS_ALERT(I2C_SMBusAlert));

  if (I2C_SMBusAlert == I2C_SMBusAlert_Low)
  {
    /* Drive the SMBusAlert pin Low [驱动SMBusAlert引脚为低]*/
    I2Cx->CR1 |= I2C_SMBusAlert_Low;
  }
  else
  {
    /* Drive the SMBusAlert pin High  [驱动SMBusAlert引脚为高]*/
    I2Cx->CR1 &= I2C_SMBusAlert_High;
  }
}

/*******************************************************************************
* 函数名称: I2C_TransmitPEC
* 功能描述: 使能（或禁止）特定I2C的PEC传输
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)NewState：I2C的PEC传输的新状态.
*                    该参数为：ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_TransmitPEC(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C PEC transmission [使能选择I2C的PEC传输]*/
    I2Cx->CR1 |= CR1_PEC_Set;
  }
  else
  {
    /* Disable the selected I2C PEC transmission [禁止选择I2C的PEC传输]*/
    I2Cx->CR1 &= CR1_PEC_Reset;
  }
}

/*******************************************************************************
* 函数名称: I2C_PECPositionConfig
* 功能描述: 选择特定的I2C的PEC位置.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)I2C_PECPosition：PEC位置. 
*                    这个参数可以是下面的值之一:
*                       - I2C_PECPosition_Next: 下一字节就是PEC
*                       - I2C_PECPosition_Current: 当前字节就是PEC
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_PECPositionConfig(I2C_TypeDef* I2Cx, u16 I2C_PECPosition)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_PEC_POSITION(I2C_PECPosition));

  if (I2C_PECPosition == I2C_PECPosition_Next)
  {
    /* Next byte in shift register is PEC [在移位寄存器中的下一个字节是PEC]*/
    I2Cx->CR1 |= I2C_PECPosition_Next;
  }
  else
  {
    /* Current byte in shift register is PEC [在移位寄存器中的当前字节是PEC]*/
    I2Cx->CR1 &= I2C_PECPosition_Current;
  }
}

/*******************************************************************************
* 函数名称: I2C_CalculatePEC
* 功能描述: 使能(或禁止)已传输字节的PEC计算值.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)NewState：PEC计算值的新状态. 该参数为：ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_CalculatePEC(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C PEC calculation [使能选择的I2C PEC计算值]*/
    I2Cx->CR1 |= CR1_ENPEC_Set;
  }
  else
  {
    /* Disable the selected I2C PEC calculation [禁止选择的I2C PEC计算值]*/
    I2Cx->CR1 &= CR1_ENPEC_Reset;
  }
}

/*******************************************************************************
* 函数名称: I2C_GetPEC
* 功能描述: 为特定I2C接口返回PEC值.
* 输入参数: I2Cx：x可为1或2来选择I2C外设.
* 输出参数: 无
* 返回参数: PEC值.
*******************************************************************************/
u8 I2C_GetPEC(I2C_TypeDef* I2Cx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Return the selected I2C PEC value [返回选择的I2C PEC值]*/
  return ((I2Cx->SR2) >> 8);
}

/*******************************************************************************
* 函数名称: I2C_ARPCmd
* 功能描述: 使能（或禁止）特定的I2C的ARP
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)NewState：I2Cx的ARP的新状态.该参数为：ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_ARPCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C ARP [使能选择的I2C ARP]*/
    I2Cx->CR1 |= CR1_ENARP_Set;
  }
  else
  {
    /* Disable the selected I2C ARP [禁止选择的I2C ARP]*/
    I2Cx->CR1 &= CR1_ENARP_Reset;
  }
}

/*******************************************************************************
* 函数名称: I2C_StretchClockCmd
* 功能描述: 使能（或禁止）特定的I2C时钟扩展
* 输入参数: I(1)2Cx：x可为1或2来选择I2C外设.
*           (2)NewState：时钟伸展的新状态.该参数为：ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_StretchClockCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState == DISABLE)
  {
    /* Enable the selected I2C Clock stretching [使能选择的I2C时钟扩展]*/
    I2Cx->CR1 |= CR1_NOSTRETCH_Set;
  }
  else
  {
    /* Disable the selected I2C Clock stretching [禁止选择的I2C时钟扩展]*/
    I2Cx->CR1 &= CR1_NOSTRETCH_Reset;
  }
}

/*******************************************************************************
* 函数名称: I2C_FastModeDutyCycleConfig
* 功能描述: 选择特定I2C的快速模式占空周期.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)I2C_DutyCycle：快速模式下占空周期。
*                    这个参数可以是下面的值之一:
*                       - I2C_DutyCycle_2: I2C快模式Tlow/Thigh=2
*                       - I2C_DutyCycle_16_9: I2C快模式Tlow/Thigh=16/9
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_FastModeDutyCycleConfig(I2C_TypeDef* I2Cx, u16 I2C_DutyCycle)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_DUTY_CYCLE(I2C_DutyCycle));

  if (I2C_DutyCycle != I2C_DutyCycle_16_9)
  {
    /* I2C fast mode Tlow/Thigh=2 [I2C快模式Tlow/Thigh=2]*/
    I2Cx->CCR &= I2C_DutyCycle_2;
  }
  else
  {
    /* I2C fast mode Tlow/Thigh=16/9 [I2C快模式Tlow/Thigh=16/9]*/
    I2Cx->CCR |= I2C_DutyCycle_16_9;
  }
}

/*******************************************************************************
* 函数名称: I2C_GetLastEvent
* 功能描述: 返回最近I2Cx事件.
* 输入参数: I2Cx：x可为1或2来选择I2C外设.
* 输出参数: 无
* 返回参数: 最近的I2Cx事件
*******************************************************************************/
u32 I2C_GetLastEvent(I2C_TypeDef* I2Cx)
{
  u32 lastevent = 0;
  u32 flag1 = 0, flag2 = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Read the I2Cx status register [读I2Cx状态寄存器]*/
  flag1 = I2Cx->SR1;
  flag2 = I2Cx->SR2;
  flag2 = flag2 << 16;

  /* Get the last event value from I2C status register [从I2Cx状态寄存器中取得最后的事件值]*/
  lastevent = (flag1 | flag2) & FLAG_Mask;

  /* Return status [返回状态内容]*/
  return lastevent;
}

/*******************************************************************************
* 函数名称: I2C_CheckEvent
* 功能描述: 检查最近的I2C事件是否等于那个已传输的参数.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)I2C_EVENT：指定被检查的事件。
*                    这个参数可以是下面的值之一:
*                       - I2C_EVENT_SLAVE_ADDRESS_MATCHED   : EV1
*                       - I2C_EVENT_SLAVE_BYTE_RECEIVED     : EV2
*                       - I2C_EVENT_SLAVE_BYTE_TRANSMITTED  : EV3
*                       - I2C_EVENT_SLAVE_ACK_FAILURE       : EV3-2
*                       - I2C_EVENT_MASTER_MODE_SELECT      : EV5
*                       - I2C_EVENT_MASTER_MODE_SELECTED    : EV6
*                       - I2C_EVENT_MASTER_BYTE_RECEIVED    : EV7
*                       - I2C_EVENT_MASTER_BYTE_TRANSMITTED : EV8
*                       - I2C_EVENT_MASTER_MODE_ADDRESS10   : EV9
*                       - I2C_EVENT_SLAVE_STOP_DETECTED     : EV4
* 输出参数: 无
* 返回参数: 一个ErrorStatus枚举值：SUCCESS：最近的事件与I2C_EVENT相同；
*                                  ERROR：最近的事件与I2C_EVENT不同
*
*******************************************************************************/
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, u32 I2C_EVENT)
{
  u32 lastevent = 0;
  u32 flag1 = 0, flag2 = 0;
  ErrorStatus status = ERROR;

  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_EVENT(I2C_EVENT));

  /* Read the I2Cx status register [读I2Cx状态寄存器]*/
  flag1 = I2Cx->SR1;
  flag2 = I2Cx->SR2;
  flag2 = flag2 << 16;

  /* Get the last event value from I2C status register [从I2Cx状态寄存器中取得最后的事件值]*/
  lastevent = (flag1 | flag2) & FLAG_Mask;

  /* Check whether the last event is equal to I2C_EVENT [检查最后的事件值是否等于I2C_EVENT]*/
  if (lastevent == I2C_EVENT )
  {
    /* SUCCESS: last event is equal to I2C_EVENT [SUCCESS：最近的事件与I2C_EVENT相同]*/
    status = SUCCESS;
  }
  else
  {
    /* ERROR: last event is different from I2C_EVENT [ERROR：最近的事件与I2C_EVENT不同]*/
    status = ERROR;
  }

  /* Return status [返回状态内容]*/
  return status;
}

/*******************************************************************************
* 函数名称: I2C_GetFlagStatus
* 功能描述: 检查特定的I2C标志是否被置位了.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)I2C_FLAG：确定将要被检查的标志。 
*                    这个参数可以是下面的值之一:
*                       - I2C_FLAG_DUALF: 双重标志（从模式）
*                       - I2C_FLAG_SMBHOST: SMBus主机头（从模式）
*                       - I2C_FLAG_SMBDEFAULT: SMBus默认头（从模式）
*                       - I2C_FLAG_GENCALL: 通用调用头标志（从模式）
*                       - I2C_FLAG_TRA: 发送器/接收器标志
*                       - I2C_FLAG_BUSY: 总线忙的标志
*                       - I2C_FLAG_MSL: 主机/从机标志
*                       - I2C_FLAG_SMBALERT: SMBus警告标志
*                       - I2C_FLAG_TIMEOUT: 超时或Tlow错误标志
*                       - I2C_FLAG_PECERR: 接收PEC错误标志
*                       - I2C_FLAG_OVR: 上溢/下溢标志（从模式）
*                       - I2C_FLAG_AF: 确认失败标志
*                       - I2C_FLAG_ARLO: 失去总裁标志（主模式）
*                       - I2C_FLAG_BERR: 总线错误标志
*                       - I2C_FLAG_TXE: 数据寄存器空的标志（发送器）
*                       - I2C_FLAG_RXNE: 数据寄存器非空的标志（接收器）
*                       - I2C_FLAG_STOPF: 停止检测寄存器（从模式）
*                       - I2C_FLAG_ADD10: 10位头被发送标志（主模式）
*                       - I2C_FLAG_BTF: 字节发送结束标志
*                       - I2C_FLAG_ADDR: 地址发送标志（主模式）"ADSL"；地址匹配标志（从模式）"ENDAD"                                      Address matched flag (Slave mode)擡NDAD?*                       - I2C_FLAG_SB: Start bit flag (Master mode)
* 输出参数: 无
* 返回参数: I2C_FLAG的新状态（SET或RESET）.
*******************************************************************************/
FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, u32 I2C_FLAG)
{
  FlagStatus bitstatus = RESET;
  u32 i2creg = 0, i2cxbase = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_GET_FLAG(I2C_FLAG));

  /* Get the I2Cx peripheral base address [取得I2Cx外设基地址]*/
  i2cxbase = (*(u32*)&(I2Cx));
  
  /* Read flag register index [读标志寄存器引索]*/
  i2creg = I2C_FLAG >> 28;
  
  /* Get bit[23:0] of the flag [取得标志的bit[23:0]位]*/
  I2C_FLAG &= FLAG_Mask;
  
  if(i2creg != 0)
  {
    /* Get the I2Cx SR1 register address [取得I2Cx SR1寄存器地址]*/
    i2cxbase += 0x14;
  }
  else
  {
    /* Flag in I2Cx SR2 Register [标记I2Cx SR2的寄存器]*/
    I2C_FLAG = (u32)(I2C_FLAG >> 16);
    /* Get the I2Cx SR2 register address [取得I2Cx SR2寄存器地址]*/
    i2cxbase += 0x18;
  }
  
  if(((*(vu32 *)i2cxbase) & I2C_FLAG) != (u32)RESET)
  {
    /* I2C_FLAG is set [置位I2C_FLAG]*/
    bitstatus = SET;
  }
  else
  {
    /* I2C_FLAG is reset [复位I2C_FLAG]*/
    bitstatus = RESET;
  }
  
  /* Return the I2C_FLAG status [返回I2C_FLAG的状态]*/
  return  bitstatus;
}

/*******************************************************************************
* 函数名称: I2C_ClearFlag
* 功能描述: 清除I2Cx的挂起标志.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)I2C_FLAG：需清除的标志。
*                    这个参数可以是下面值的任意组合:
*                       - I2C_FLAG_SMBALERT: SMBus警告标志
*                       - I2C_FLAG_TIMEOUT: 超时或Tlow错误标志
*                       - I2C_FLAG_PECERR: 接收的PEC错误标志
*                       - I2C_FLAG_OVR: 上溢/下溢标志（从模式）
*                       - I2C_FLAG_AF: 确认失败标志
*                       - I2C_FLAG_ARLO: 失去总裁标志（主模式）
*                       - I2C_FLAG_BERR: 总线错误标志
*                       
*                  注意: 
*                        - STOPF (停止检测)只能用软件按次序操作清除：  
*                          读I2C_SR1寄存器操作(I2C_GetFlagStatus())后面跟着写
*                          I2C_CR1 寄存器操作 (I2C_Cmd() to 重新使能I2C外设). 
*                        - ADD10 (10-位头发送)只能用软件按次序操作清除：
*                          读I2C_SR1寄存器操作(I2C_GetFlagStatus())后面跟着写
*                          第二个字节地址到DR寄存器。
*                        - BTF (字节发送完成) 只能用软件按次序操作清除： 
*                          读I2C_SR1寄存器操作(I2C_GetFlagStatus())后面跟着读/写
*                          I2C_DR寄存器 (I2C_SendData()).
*                        - ADDR (地址发送)只能用软件按次序操作清除：
*                          读I2C_SR1寄存器操作(I2C_GetFlagStatus())后面跟着读
*                          I2C_SR2寄存器操作 ((void)(I2Cx->SR2)).
*                        - SB (起始位) 只能用软件按次序操作清除:读I2C_SR1 
*                          寄存器操作(I2C_GetFlagStatus())后面跟着写I2C_DR寄存器 
*                          操作(I2C_SendData()). 
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_ClearFlag(I2C_TypeDef* I2Cx, u32 I2C_FLAG)
{
  u32 flagpos = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_CLEAR_FLAG(I2C_FLAG));

  /* Get the I2C flag position [取得I2C标志位置]*/
  flagpos = I2C_FLAG & FLAG_Mask;

  /* Clear the selected I2C flag [清除选择的I2C标志]*/
  I2Cx->SR1 = (u16)~flagpos;
}

/*******************************************************************************
* 函数名称: I2C_GetITStatus
* 功能描述: 检查特定的I2C中断是否产生.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)I2C_IT：确定需要检查的中断源. 
*                    这个参数可以是下面的值之一:
*                       - I2C_IT_SMBALERT: SMBus警告标志
*                       - I2C_IT_TIMEOUT: 超时或Tlow错误标志
*                       - I2C_IT_PECERR: 接收的PEC错误标志
*                       - I2C_IT_OVR: 上溢/下溢标志（从模式）
*                       - I2C_IT_AF: 确认失败标志
*                       - I2C_IT_ARLO: 失去总裁标志（主模式）
*                       - I2C_IT_BERR: 总线错误标志
*                       - I2C_IT_TXE: 数据寄存器空的标志（发送器）
*                       - I2C_IT_RXNE: 数据寄存器非空的标志（接收器）
*                       - I2C_IT_STOPF: 停止检测寄存器（从模式）
*                       - I2C_IT_ADD10: 10位头发送标志（主模式）
*                       - I2C_IT_BTF: 字节传送结束标志
*                       - I2C_IT_ADDR: 地址发送标志（主模式）"ADSL"；地址符合标志（从模式）"ENDAD"                                      Address matched flag (Slave mode)擡NDAD?*                       - I2C_IT_SB: Start bit flag (Master mode)
* 输出参数: 无
* 返回参数: I2C_IT的新状态（SET或RESET）.
*******************************************************************************/
ITStatus I2C_GetITStatus(I2C_TypeDef* I2Cx, u32 I2C_IT)
{
  ITStatus bitstatus = RESET;
  u32 enablestatus = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_GET_IT(I2C_IT));

  /* Check if the interrupt source is enabled or not [检查中断源是否被使能]*/
  enablestatus = (u32)(((I2C_IT & ITEN_Mask) >> 16) & (I2Cx->CR2)) ;  

  /* Get bit[23:0] of the flag [取得标志位的[23:0]位]*/
  I2C_IT &= FLAG_Mask;

  /* Check the status of the specified I2C flag [检查指定的I2C标志状态]*/
  if (((I2Cx->SR1 & I2C_IT) != (u32)RESET) && enablestatus)
  {
    /* I2C_IT is set [置位I2C_IT]*/
    bitstatus = SET;
  }
  else
  {
    /* I2C_IT is reset [复位I2C_IT]*/
    bitstatus = RESET;
  }
  /* Return the I2C_IT status [返回I2C_IT的状态]*/
  return  bitstatus;
}

/*******************************************************************************
* 函数名称: I2C_ClearITPendingBit
* 功能描述: 清除I2Cx的中断挂起位.
* 输入参数: (1)I2Cx：x可为1或2来选择I2C外设.
*           (2)I2C_IT：确定需要清除的中断挂起位。 
*                    这个参数可以是下面值的任意组合:
*                       - I2C_IT_SMBALERT: SMBus告警中断
*                       - I2C_IT_TIMEOUT: 超时或Tlow错误中断
*                       - I2C_IT_PECERR: 接收的PEC错误中断
*                       - I2C_IT_OVR: 上溢/下溢中断（从模式）
*                       - I2C_IT_AF: 确认失败中断
*                       - I2C_IT_ARLO: 失去总裁中断（主模式）
*                       - I2C_IT_BERR: 总线错误中断
*                       
*                  注意:
*                        - STOPF (停止检测)只能用软件按次序操作清除：  
*                          读I2C_SR1寄存器操作(I2C_GetFlagStatus())后面跟着写
*                          I2C_CR1 寄存器操作 (I2C_Cmd() to 重新使能I2C外设). 
*                        - ADD10 (10-位头发送)只能用软件按次序操作清除：
*                          读I2C_SR1寄存器操作(I2C_GetFlagStatus())后面跟着写
*                          第二个字节地址到DR寄存器。
*                        - BTF (字节发送完成) 只能用软件按次序操作清除： 
*                          读I2C_SR1寄存器操作(I2C_GetFlagStatus())后面跟着读/写
*                          I2C_DR寄存器 (I2C_SendData()).
*                        - ADDR (地址发送)只能用软件按次序操作清除：
*                          读I2C_SR1寄存器操作(I2C_GetFlagStatus())后面跟着读
*                          I2C_SR2寄存器操作 ((void)(I2Cx->SR2)).
*                        - SB (起始位) 只能用软件按次序操作清除:读I2C_SR1 
*                          寄存器操作(I2C_GetFlagStatus())后面跟着写I2C_DR寄存器 
*                          操作(I2C_SendData()).
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2C_ClearITPendingBit(I2C_TypeDef* I2Cx, u32 I2C_IT)
{
  u32 flagpos = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_CLEAR_IT(I2C_IT));

  /* Get the I2C flag position [取得I2C标志的位置]*/
  flagpos = I2C_IT & FLAG_Mask;

  /* Clear the selected I2C flag [清除选择的I2C标志]*/
  I2Cx->SR1 = (u16)~flagpos;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
