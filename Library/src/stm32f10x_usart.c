/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_usart.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有USART的固件函数
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
/* USART UE Mask */
#define CR1_UE_Set                ((u16)0x2000)  /* USART Enable Mask */
#define CR1_UE_Reset              ((u16)0xDFFF)  /* USART Disable Mask */

/* USART WakeUp Method  */
#define CR1_WAKE_Mask             ((u16)0xF7FF)  /* USART WakeUp Method Mask */

/* USART RWU Mask */
#define CR1_RWU_Set               ((u16)0x0002)  /* USART mute mode Enable Mask */
#define CR1_RWU_Reset             ((u16)0xFFFD)  /* USART mute mode Enable Mask */

#define CR1_SBK_Set               ((u16)0x0001)  /* USART Break Character send Mask */

#define CR1_CLEAR_Mask            ((u16)0xE9F3)  /* USART CR1 Mask */

#define CR2_Address_Mask          ((u16)0xFFF0)  /* USART address Mask */

/* USART LIN Mask */
#define CR2_LINEN_Set              ((u16)0x4000)  /* USART LIN Enable Mask */
#define CR2_LINEN_Reset            ((u16)0xBFFF)  /* USART LIN Disable Mask */

/* USART LIN Break detection */
#define CR2_LBDL_Mask             ((u16)0xFFDF)  /* USART LIN Break detection Mask */

#define CR2_STOP_CLEAR_Mask       ((u16)0xCFFF)  /* USART CR2 STOP Bits Mask */
#define CR2_CLOCK_CLEAR_Mask      ((u16)0xF0FF)  /* USART CR2 Clock Mask */

/* USART SC Mask */
#define CR3_SCEN_Set              ((u16)0x0020)  /* USART SC Enable Mask */
#define CR3_SCEN_Reset            ((u16)0xFFDF)  /* USART SC Disable Mask */

/* USART SC NACK Mask */
#define CR3_NACK_Set              ((u16)0x0010)  /* USART SC NACK Enable Mask */
#define CR3_NACK_Reset            ((u16)0xFFEF)  /* USART SC NACK Disable Mask */

/* USART Half-Duplex Mask */
#define CR3_HDSEL_Set             ((u16)0x0008)  /* USART Half-Duplex Enable Mask */
#define CR3_HDSEL_Reset           ((u16)0xFFF7)  /* USART Half-Duplex Disable Mask */

/* USART IrDA Mask */
#define CR3_IRLP_Mask             ((u16)0xFFFB)  /* USART IrDA LowPower mode Mask */

#define CR3_CLEAR_Mask            ((u16)0xFCFF)  /* USART CR3 Mask */

/* USART IrDA Mask */
#define CR3_IREN_Set              ((u16)0x0002)  /* USART IrDA Enable Mask */
#define CR3_IREN_Reset            ((u16)0xFFFD)  /* USART IrDA Disable Mask */

#define GTPR_LSB_Mask             ((u16)0x00FF)  /* Guard Time Register LSB Mask */
#define GTPR_MSB_Mask             ((u16)0xFF00)  /* Guard Time Register MSB Mask */

#define IT_Mask                   ((u16)0x001F)  /* USART Interrupt Mask */

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: USART_DeInit
* 功能描述: 重置USARTx外围寄存器为默认重置值.
* 输入参数: USARTx: 选择这个USART 或者这个UART外设.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_DeInit(USART_TypeDef* USARTx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));

  switch (*(u32*)&USARTx)
  {
    case USART1_BASE:
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);
      break;

    case USART2_BASE:
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);
      break;

    case USART3_BASE:
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);
      break;
    
    case UART4_BASE:
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4, ENABLE);
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4, DISABLE);
      break;
    
    case UART5_BASE:
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, ENABLE);
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, DISABLE);
      break;            

    default:
      break;
  }
}

/*******************************************************************************
* 函数名称: USART_Init
* 功能描述: 根据USART_InitStruct中的指定的参数初始化USARTx外围设备 .
* 输入参数: (1)USARTx: 选择这个USART 或者这个UART外设.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_InitStruct：指向包含特定的USART外围设备配置信息的USART_InitTypeDef结构体..
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct)
{
  u32 tmpreg = 0x00, apbclock = 0x00;
  u32 integerdivider = 0x00;
  u32 fractionaldivider = 0x00;
  u32 usartxbase = 0;
  RCC_ClocksTypeDef RCC_ClocksStatus;

  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));  
  assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));
  assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));
  assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));
  assert_param(IS_USART_MODE(USART_InitStruct->USART_Mode));
  assert_param(IS_USART_HARDWARE_FLOW_CONTROL(USART_InitStruct->USART_HardwareFlowControl));
  /* The hardware flow control is available only for USART1, USART2 and USART3 [这个硬件流控制只能用于USART1,USART2和USART3]*/          
  assert_param(IS_USART_PERIPH_HFC(USARTx, USART_InitStruct->USART_HardwareFlowControl));
  
  usartxbase = (*(u32*)&USARTx);

/*---------------------------- USART CR2 Configuration [USART CR2配置]*/
  tmpreg = USARTx->CR2;
  /* Clear STOP[13:12] bits [清除STOP[13:12]位]*/
  tmpreg &= CR2_STOP_CLEAR_Mask;

  /* Configure the USART Stop Bits, Clock, CPOL, CPHA and LastBit [配置USART STOP位,Clock, CPOL, CPHA 和 LastBit]*/
  /* Set STOP[13:12] bits according to USART_StopBits value [依照USART_StopBits的值设置STOP[13:12]位]*/
  tmpreg |= (u32)USART_InitStruct->USART_StopBits;
  
  /* Write to USART CR2 [写USART CR2]*/
  USARTx->CR2 = (u16)tmpreg;

/*---------------------------- USART CR1 Configuration [USART CR1配置]--------*/
  tmpreg = USARTx->CR1;
  /* Clear M, PCE, PS, TE and RE bits [清M, PCE, PS, TE和RE位]*/
  tmpreg &= CR1_CLEAR_Mask;

  /* Configure the USART Word Length, Parity and mode [配置USART的字长,奇偶校验和模式] */
  /* Set the M bits according to USART_WordLength value [依照USART_WordLength值设置M位]*/
  /* Set PCE and PS bits according to USART_Parity value [依照USART_Parity值设置PCE和PS位]*/
  /* Set TE and RE bits according to USART_Mode value [依照USART_Mode值设置TE和RE位]*/
  tmpreg |= (u32)USART_InitStruct->USART_WordLength | USART_InitStruct->USART_Parity |
            USART_InitStruct->USART_Mode;

  /* Write to USART CR1 [写USART CR1]*/
  USARTx->CR1 = (u16)tmpreg;

/*---------------------------- USART CR3 Configuration [USART CR3配置]--------*/  
  tmpreg = USARTx->CR3;
  /* Clear CTSE and RTSE bits [清CTSE 和 RTSE位]*/
  tmpreg &= CR3_CLEAR_Mask;

  /* Configure the USART HFC [配置USART HFC]----------------------------------*/
  /* Set CTSE and RTSE bits according to USART_HardwareFlowControl value [依照USART_HardwareFlowControl值设置CTSE和RTSE位]*/
  tmpreg |= USART_InitStruct->USART_HardwareFlowControl;

  /* Write to USART CR3 [写USART CR3]*/
  USARTx->CR3 = (u16)tmpreg;

/*---------------------------- USART BRR Configuration [USART BRR配置]--------*/
  /* Configure the USART Baud Rate [配置USART 波特率]-------------------*/
  RCC_GetClocksFreq(&RCC_ClocksStatus);
  if (usartxbase == USART1_BASE)
  {
    apbclock = RCC_ClocksStatus.PCLK2_Frequency;
  }
  else
  {
    apbclock = RCC_ClocksStatus.PCLK1_Frequency;
  }

  /* Determine the integer part [确定整数部分]*/
  integerdivider = ((0x19 * apbclock) / (0x04 * (USART_InitStruct->USART_BaudRate)));
  tmpreg = (integerdivider / 0x64) << 0x04;

  /* Determine the fractional part [确定小数部分]*/
  fractionaldivider = integerdivider - (0x64 * (tmpreg >> 0x04));
  tmpreg |= ((((fractionaldivider * 0x10) + 0x32) / 0x64)) & ((u8)0x0F);

  /* Write to USART BRR [写USART BRR]*/
  USARTx->BRR = (u16)tmpreg;
}

/*******************************************************************************
* 函数名称: USART_StructInit
* 功能描述: 用默认值填充每一个USART_InitStruct成员.
* 输入参数: USART_InitStruct: 指向会被初始化的USART_InitTypeDef结构体.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_StructInit(USART_InitTypeDef* USART_InitStruct)
{
  /* USART_InitStruct members default value [USART_InitStruct结构成员的默认值]*/
  USART_InitStruct->USART_BaudRate = 9600;
  USART_InitStruct->USART_WordLength = USART_WordLength_8b;
  USART_InitStruct->USART_StopBits = USART_StopBits_1;
  USART_InitStruct->USART_Parity = USART_Parity_No ;
  USART_InitStruct->USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStruct->USART_HardwareFlowControl = USART_HardwareFlowControl_None;
}

/*******************************************************************************
* 函数名称: USART_ClockInit
* 功能描述: 依照USART_ClockInitStruct指定参数初始化 USARTx 外围设备时钟  .
* 输入参数: (1)USARTx: x可以是 1, 2, 3 来选择USART外设.
*                    注意:智能卡模式不能使用 UART4 和 UART5.
*           (2)USART_ClockInitStruct: USART_ClockInitTypeDef的结构指针包含USART设备的配置信息。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct)
{
  u32 tmpreg = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_123_PERIPH(USARTx));
  assert_param(IS_USART_CLOCK(USART_ClockInitStruct->USART_Clock));
  assert_param(IS_USART_CPOL(USART_ClockInitStruct->USART_CPOL));
  assert_param(IS_USART_CPHA(USART_ClockInitStruct->USART_CPHA));
  assert_param(IS_USART_LASTBIT(USART_ClockInitStruct->USART_LastBit));              
  
/*---------------------------- USART CR2 Configuration [USART CR2配置]--------*/
  tmpreg = USARTx->CR2;
  /* Clear CLKEN, CPOL, CPHA and LBCL bits [清CLKEN,CPOL,CPHA和LBCL位]*/
  tmpreg &= CR2_CLOCK_CLEAR_Mask;

  /* Configure the USART Clock, CPOL, CPHA and LastBit [配置USART STOP位,Clock, CPOL, CPHA 和 LastBit]*/
  /* Set CLKEN bit according to USART_Clock value [依照USART_Clock值设置CLKEN位]*/
  /* Set CPOL bit according to USART_CPOL value [依照USART_CPOL值设置CPOL位]*/
  /* Set CPHA bit according to USART_CPHA value [依照USART_CPHA值设置CPHA位]*/
  /* Set LBCL bit according to USART_LastBit value [依照USART_LastBit值设置LBCL位]*/
  tmpreg |= (u32)USART_ClockInitStruct->USART_Clock | USART_ClockInitStruct->USART_CPOL | 
                 USART_ClockInitStruct->USART_CPHA | USART_ClockInitStruct->USART_LastBit;

  /* Write to USART CR2 [写USART CR2]*/
  USARTx->CR2 = (u16)tmpreg;
}

/*******************************************************************************
* 函数名称: USART_ClockStructInit
* 功能描述: 用默认值填充每一个USART_ClockInitStruct的成员.
* 输入参数: - USART_ClockInitStruct: 将要初始化指向USART_ClockInitTypeDef结构的指针.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_ClockStructInit(USART_ClockInitTypeDef* USART_ClockInitStruct)
{
  /* USART_ClockInitStruct members default value [USART_ClockInitStruct成员默认值]*/
  USART_ClockInitStruct->USART_Clock = USART_Clock_Disable;
  USART_ClockInitStruct->USART_CPOL = USART_CPOL_Low;
  USART_ClockInitStruct->USART_CPHA = USART_CPHA_1Edge;
  USART_ClockInitStruct->USART_LastBit = USART_LastBit_Disable;
}

/*******************************************************************************
* 函数名称: USART_Cmd
* 功能描述: E使能或禁用特定的USART外围设备.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)Newstate：USARTx外围设备的新状态.这个参数能够取：ENABLE或者DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected USART by setting the UE bit in the CR1 register [置位CR1寄存器中的UE位,来使能选择的USART]*/
    USARTx->CR1 |= CR1_UE_Set;
  }
  else
  {
    /* Disable the selected USART by clearing the UE bit in the CR1 register [清零CR1寄存器中的UE位,来禁止选择的USART]*/
    USARTx->CR1 &= CR1_UE_Reset;
  }
}

/*******************************************************************************
* 函数名称: USART_ITConfig
* 功能描述: 使能或禁用特定的USART中断.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_IT：指明要使能或禁用的USART中断源。
*                    这个参数可以是下面的值之一 :
*                       - USART_IT_CTS:  CTS 改变中断 (对于UART4和UART5无法使用)
*                       - USART_IT_LBD:  LIN 间隔侦测中断
*                       - USART_IT_TXE:  发送寄存器空中断
*                       - USART_IT_TC:   发送完成中断
*                       - USART_IT_RXNE: 接收寄存器全满中断
*                       - USART_IT_IDLE: IDLE线性中断
*                       - USART_IT_PE:   奇偶错误中断
*                       - USART_IT_ERR:  错误中断(桢错误, 噪声错误, 超速错误)
*           (3)指定USARTx中断的新状态.这个参数能够取:ENABLE或者DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_ITConfig(USART_TypeDef* USARTx, u16 USART_IT, FunctionalState NewState)
{
  u32 usartreg = 0x00, itpos = 0x00, itmask = 0x00;
  u32 usartxbase = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CONFIG_IT(USART_IT));
  assert_param(IS_USART_PERIPH_IT(USARTx, USART_IT)); /* The CTS interrupt is not available for UART4 and UART5 [CTS中断对于UART4和UART5无法使用]*/     
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  usartxbase = (*(u32*)&(USARTx));

  /* Get the USART register index [取得USART寄存器变址]*/
  usartreg = (((u8)USART_IT) >> 0x05);

  /* Get the interrupt position [取得中断位置]*/
  itpos = USART_IT & IT_Mask;

  itmask = (((u32)0x01) << itpos);
    
  if (usartreg == 0x01) /* The IT is in CR1 register [中断在CR1寄存器中]*/
  {
    usartxbase += 0x0C;
  }
  else if (usartreg == 0x02) /* The IT is in CR2 register [中断在CR2寄存器中]*/
  {
    usartxbase += 0x10;
  }
  else /* The IT is in CR3 register [中断在CR3寄存器中]*/
  {
    usartxbase += 0x14; 
  }
  if (NewState != DISABLE)
  {
    *(vu32*)usartxbase  |= itmask;
  }
  else
  {
    *(vu32*)usartxbase &= ~itmask;
  }
}

/*******************************************************************************
* 函数名称: USART_DMACmd
* 功能描述: 使能或禁用特定的USART DMA接口.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3 or UART4.
*                    注意: 这个DMA不能用于UART5.
*           (2)USART_DMAReq: 指定DMA请求.
*                    这个参数可以是下面任意值的组合:
*                       - USART_DMAReq_Tx: USART DMA 传输请求
*                       - USART_DMAReq_Rx: USART DMA 接收请求
*           (3)NewState: DMA请求源的新状态.这个参数能够取:ENABLE或者DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_DMACmd(USART_TypeDef* USARTx, u16 USART_DMAReq, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_1234_PERIPH(USARTx));
  assert_param(IS_USART_DMAREQ(USART_DMAReq));  
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 

  if (NewState != DISABLE)
  {
    /* Enable the DMA transfer for selected requests by setting the DMAT and/or
       DMAR bits in the USART CR3 register [通过设置USART CR3 寄存器的DMAT、DMAT位组合,来使能选择DMA传输请求]*/
    USARTx->CR3 |= USART_DMAReq;
  }
  else
  {
    /* Disable the DMA transfer for selected requests by clearing the DMAT and/or
       DMAR bits in the USART CR3 register [通过设置USART CR3 寄存器的DMAT、DMAT位组合,来禁止选择DMA传输请求]*/
    USARTx->CR3 &= (u16)~USART_DMAReq;
  }
}

/*******************************************************************************
* 函数名称: USART_SetAddress
* 功能描述: 设置USART节点的地址
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_Address：指出了USART节点的地址
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_SetAddress(USART_TypeDef* USARTx, u8 USART_Address)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_ADDRESS(USART_Address)); 
    
  /* Clear the USART address [清除 USART地址]*/
  USARTx->CR2 &= CR2_Address_Mask;
  /* Set the USART address node [设置USART地址]*/
  USARTx->CR2 |= USART_Address;
}

/*******************************************************************************
* 函数名称: USART_WakeUpConfig
* 功能描述: 选择USART的唤醒方法.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_WakeUp:指明了USART的唤醒方法.
*                    这个参数可以是下面的值之一 :
*                        - USART_WakeUp_IdleLine: IDLE线路唤醒
*                        - USART_WakeUp_AddressMark: 地址标记唤醒
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_WakeUpConfig(USART_TypeDef* USARTx, u16 USART_WakeUp)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_WAKEUP(USART_WakeUp));
  
  USARTx->CR1 &= CR1_WAKE_Mask;
  USARTx->CR1 |= USART_WakeUp;
}

/*******************************************************************************
* 函数名称: USART_ReceiverWakeUpCmd
* 功能描述: 决定USART是否在无声模式.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)Newstate：USART模式的新状态.这个参数能够取：ENABLE或者DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_ReceiverWakeUpCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  
  if (NewState != DISABLE)
  {
    /* Enable the USART mute mode  by setting the RWU bit in the CR1 register [通过置位CR1寄存器的RWU位使能USART静音模式]*/
    USARTx->CR1 |= CR1_RWU_Set;
  }
  else
  {
    /* Disable the USART mute mode by clearing the RWU bit in the CR1 register [通过清零CR1寄存器的RWU位禁止USART静音模式]*/
    USARTx->CR1 &= CR1_RWU_Reset;
  }
}

/*******************************************************************************
* 函数名称: USART_LINBreakDetectLengthConfig
* 功能描述: 设置USART LIN间隔检波长度.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_LINBreakDetectLength指明了LIN间断检测长度.
*                    这个参数可以是下面的值之一 :
*                       - USART_LINBreakDetectLength_10b: 10位间断检测
*                       - USART_LINBreakDetectLength_11b: 11位间断检测
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, u16 USART_LINBreakDetectLength)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_LIN_BREAK_DETECT_LENGTH(USART_LINBreakDetectLength));
  
  USARTx->CR2 &= CR2_LBDL_Mask;
  USARTx->CR2 |= USART_LINBreakDetectLength;  
}

/*******************************************************************************
* 函数名称: USART_LINCmd
* 功能描述: 使能或禁用USART LIN模式.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)Newstate:USART LIN模式的新状态.这个参数能够取:ENABLE或者DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_LINCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the LIN mode by setting the LINEN bit in the CR2 register [通过置位CR2寄存器的LINEN位使能LIN模式]*/
    USARTx->CR2 |= CR2_LINEN_Set;
  }
  else
  {
    /* Disable the LIN mode by clearing the LINEN bit in the CR2 register [通过清零CR2寄存器的LINEN位禁止LIN模式]*/
    USARTx->CR2 &= CR2_LINEN_Reset;
  }
}

/*******************************************************************************
* 函数名称: USART_SendData
* 功能描述: 传输单个数据通过USARTx外围设备.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)Data:要被传输的数据.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_SendData(USART_TypeDef* USARTx, u16 Data)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_DATA(Data)); 
    
  /* Transmit Data */
  USARTx->DR = (Data & (u16)0x01FF);
}

/*******************************************************************************
* 函数名称: USART_ReceiveData
* 功能描述: 返回最近由USARTx外围设备接收的数据.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
* 输出参数: 无
* 返回参数: 接收的数据.
*******************************************************************************/
u16 USART_ReceiveData(USART_TypeDef* USARTx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  
  /* Receive Data */
  return (u16)(USARTx->DR & (u16)0x01FF);
}

/*******************************************************************************
* 函数名称: USART_SendBreak
* 功能描述: 传输间断符.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_SendBreak(USART_TypeDef* USARTx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  
  /* Send break characters [发送间断字符]*/
  USARTx->CR1 |= CR1_SBK_Set;
}

/*******************************************************************************
* 函数名称: USART_SetGuardTime
* 功能描述: 设置指定的USART保护时间.
* 输入参数: (1)USARTx:x可以为1,2或3以选择USART的外围设备.
*                  注意: 这个保护时间位不能用于UART4和UART5.
*           (2)USART_GuardTime:指定保护时间.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_SetGuardTime(USART_TypeDef* USARTx, u8 USART_GuardTime)
{    
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_123_PERIPH(USARTx));
  
  /* Clear the USART Guard time [清除USART保护时间]*/
  USARTx->GTPR &= GTPR_LSB_Mask;
  /* Set the USART guard time [设置USART保护时间*/
  USARTx->GTPR |= (u16)((u16)USART_GuardTime << 0x08);
}

/*******************************************************************************
* 函数名称: USART_SetPrescaler
* 功能描述: 设置USART时钟预分频器.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*                  注意:这个函数是用于UART4和UART5的IrDA模式 .
*           (2)USART_Prescaler:指明预分频器.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_SetPrescaler(USART_TypeDef* USARTx, u8 USART_Prescaler)
{ 
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  
  /* Clear the USART prescaler [清除USART预分频器]*/
  USARTx->GTPR &= GTPR_MSB_Mask;
  /* Set the USART prescaler [设置USART预分频器]*/
  USARTx->GTPR |= USART_Prescaler;
}

/*******************************************************************************
* 函数名称: USART_SmartCardCmd
* 功能描述: 使能或禁用USART智能卡模式.
* 输入参数: (1)USARTx:x可以为1,2或3以选择USART的外围设备. 
*                    注意:这个智能卡模式不能用于UART4和UART5.
*           (2)Newstate:智能卡模式的新状态.这个参数能够取:ENABLE或者DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_SmartCardCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_123_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the SC mode by setting the SCEN bit in the CR3 register [通过置位CR3寄存器的SCEN位使能SC模式]*/
    USARTx->CR3 |= CR3_SCEN_Set;
  }
  else
  {
    /* Disable the SC mode by clearing the SCEN bit in the CR3 register [通过清零CR3寄存器的SCEN位禁止SC模式]*/
    USARTx->CR3 &= CR3_SCEN_Reset;
  }
}

/*******************************************************************************
* 函数名称: USART_SmartCardNACKCmd
* 功能描述: 使能或禁用NACK传输.
* 输入参数: (1)USARTx:x可以为1,2或3以选择USART的外围设备. 
*                    注意:这个智能卡模式不能用于UART4和UART5.
*           (2)Newstate:NACK传输的新状态.这个参数能够取:ENABLE或者DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_SmartCardNACKCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_123_PERIPH(USARTx));  
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the NACK transmission by setting the NACK bit in the CR3 register [通过置位CR3寄存器的NACK位使能NACK传输]*/
    USARTx->CR3 |= CR3_NACK_Set;
  }
  else
  {
    /* Disable the NACK transmission by clearing the NACK bit in the CR3 register [通过清零CR3寄存器的NACK位禁止NACK传输]*/
    USARTx->CR3 &= CR3_NACK_Reset;
  }
}

/*******************************************************************************
* 函数名称: USART_HalfDuplexCmd
* 功能描述: 使能或禁用USART半双工模式.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)Newstate:半双工模式的新状态.这个参数能够取：ENABLE或者DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_HalfDuplexCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the Half-Duplex mode by setting the HDSEL bit in the CR3 register [通过置位CR3寄存器的HDSEL位使能Half-Duplex模式]*/
    USARTx->CR3 |= CR3_HDSEL_Set;
  }
  else
  {
    /* Disable the Half-Duplex mode by clearing the HDSEL bit in the CR3 register [通过清零CR3寄存器的HDSEL位禁止Half-Duplex模式]*/
    USARTx->CR3 &= CR3_HDSEL_Reset;
  }
}

/*******************************************************************************
* 函数名称: USART_IrDAConfig
* 功能描述: 配置USART IrDA模式.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_IrDAMode:指定IrDA模式.
*                    这个参数可以是下面的值之一 :
*                       - USART_IrDAMode_LowPower
*                       - USART_IrDAMode_Normal
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_IrDAConfig(USART_TypeDef* USARTx, u16 USART_IrDAMode)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_IRDA_MODE(USART_IrDAMode));
    
  USARTx->CR3 &= CR3_IRLP_Mask;
  USARTx->CR3 |= USART_IrDAMode;
}

/*******************************************************************************
* 函数名称: USART_IrDACmd
* 功能描述: 使能或禁用USART IrDA接口.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)Newstate:IrDA模式的新状态.这个参数能够取：ENABLE或者DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_IrDACmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
    
  if (NewState != DISABLE)
  {
    /* Enable the IrDA mode by setting the IREN bit in the CR3 register [通过置位CR3寄存器的IREN位使能IrDA模式]*/
    USARTx->CR3 |= CR3_IREN_Set;
  }
  else
  {
    /* Disable the IrDA mode by clearing the IREN bit in the CR3 register [通过清零CR3寄存器的IREN位禁止IrDA模式]*/
    USARTx->CR3 &= CR3_IREN_Reset;
  }
}

/*******************************************************************************
* 函数名称: USART_GetFlagStatus
* 功能描述: 检测指明的USART标记是否被置位.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_FLAG:指明要检测的标记.
*                    这个参数可以是下面的值之一 :
*                       - USART_FLAG_CTS:  CTS 改变标志 (对于UART4和UART5无法使用)
*                       - USART_FLAG_LBD:  LIN 间隔侦测标志
*                       - USART_FLAG_TXE:  发送寄存器空标志
*                       - USART_FLAG_TC:   发送完成标志
*                       - USART_FLAG_RXNE: 接收寄存器全满标志
*                       - USART_FLAG_IDLE: IDLE线性标志
*                       - USART_FLAG_ORE:  过速错误标志
*                       - USART_FLAG_NE:   噪声错误标志
*                       - USART_FLAG_FE:   成帧误差标志
*                       - USART_FLAG_PE:   奇偶错误标志
* 输出参数: 无
* 返回参数: USART_FLAG新状态.这个参数能够取:ENABLE或者DISABLE.
*******************************************************************************/
FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, u16 USART_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_FLAG(USART_FLAG));
  assert_param(IS_USART_PERIPH_FLAG(USARTx, USART_FLAG)); /* 着个CTS标志不能用于UART4和UART5 */   

  if ((USARTx->SR & USART_FLAG) != (u16)RESET)
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
* 函数名称: USART_ClearFlag
* 功能描述: 清除USARTx挂起标记.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_FLAG：指定要被清除的标记.
*                    这个参数可以是下面的值的任意组合:
*                       - USART_FLAG_CTS:  CTS 改变标志 (对于UART4和UART5无法使用).
*                       - USART_FLAG_LBD:  LIN 间隔侦测标志.
*                       - USART_FLAG_TC:   发送完成标志.
*                       - USART_FLAG_RXNE: 接收寄存器全满标志.
*
*                  注意:
*                        - PE (奇偶校验错误), FE (桢错误), NE (噪声错误),
*                          ORE (过速错误) 和 IDLE (找到空闲线) 标志标志被软件按顺序清除:
*                          一个读USART_SR寄存器操作（USART_GetFlagStatus()）
*                          接着一个读USART_DR寄存器操作(USART_ReceiveData()).
*                        - RXNE标志也可以被一个读USART_DR寄存器操作清除(USART_ReceiveData()).
*                        - TC标志标志被软件按顺序清除: 一个读USART_SR寄存器操作（USART_GetFlagStatus()）
*                          接着一个读USART_DR寄存器操作(USART_ReceiveData()).                                                      
*                        - TXE标志只能靠写USART_DR寄存器清除(USART_SendData()).                        
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_ClearFlag(USART_TypeDef* USARTx, u16 USART_FLAG)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CLEAR_FLAG(USART_FLAG));
  assert_param(IS_USART_PERIPH_FLAG(USARTx, USART_FLAG)); /* The CTS flag is not available for UART4 and UART5 [CTS 改变中断 对于UART4和UART5无法使用]*/   
   
  USARTx->SR = (u16)~USART_FLAG;
}

/*******************************************************************************
* 函数名称: USART_GetITStatus
* 功能描述: 检测指定的USART中断是否发生.
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_IT:指明要检测的USART中断源.
*                    这个参数可以是下面的值之一 :
*                       - USART_IT_CTS:  CTS 改变中断 (对于UART4和UART5无法使用)
*                       - USART_IT_LBD:  LIN 间隔侦测中断
*                       - USART_IT_TXE:  发送寄存器空中断
*                       - USART_IT_TC:   发送完成中断
*                       - USART_IT_RXNE: 接收寄存器全满中断
*                       - USART_IT_IDLE: IDLE线性中断
*                       - USART_IT_ORE:  超速错误中断
*                       - USART_IT_NE:   噪声错误中断
*                       - USART_IT_FE:   桢错误中断
*                       - USART_IT_PE:   奇偶错误中断
* 输出参数: 无
* 返回参数: The new state of USART_IT (SET or RESET).
*******************************************************************************/
ITStatus USART_GetITStatus(USART_TypeDef* USARTx, u16 USART_IT)
{
  u32 bitpos = 0x00, itmask = 0x00, usartreg = 0x00;
  ITStatus bitstatus = RESET;

  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_GET_IT(USART_IT));
  assert_param(IS_USART_PERIPH_IT(USARTx, USART_IT)); /* The CTS interrupt is not available for UART4 and UART5 [CTS 改变中断 对于UART4和UART5无法使用]*/  
  
  /* Get the USART register index [取得USART寄存器引索]*/
  usartreg = (((u8)USART_IT) >> 0x05);

  /* Get the interrupt position [取得中断位置]*/
  itmask = USART_IT & IT_Mask;

  itmask = (u32)0x01 << itmask;
  
  if (usartreg == 0x01) /* The IT  is in CR1 register [CR1寄存器内的中断]*/
  {
    itmask &= USARTx->CR1;
  }
  else if (usartreg == 0x02) /* The IT  is in CR2 register [CR2寄存器内的中断]*/
  {
    itmask &= USARTx->CR2;
  }
  else /* The IT  is in CR3 register [CR3寄存器内的中断]*/
  {
    itmask &= USARTx->CR3;
  }
  
  bitpos = USART_IT >> 0x08;

  bitpos = (u32)0x01 << bitpos;
  bitpos &= USARTx->SR;

  if ((itmask != (u16)RESET)&&(bitpos != (u16)RESET))
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
* 函数名称: USART_ClearITPendingBit
* 功能描述: 清除USARTx中断挂起位
* 输入参数: (1)USARTx: 选择 USART 或者 UART的外围设备.
*                    这个参数可以是下面的值之一 :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_IT：指明要被清除的中断挂起位.
*                    这个参数可以是下面的值之一:
*                       - USART_IT_CTS:  CTS 改变中断 (对于UART4和UART5无法使用)
*                       - USART_IT_LBD:  LIN 间隔侦测中断
*                       - USART_IT_TC:   发送完成中断. 
*                       - USART_IT_RXNE: 接收寄存器全满中断.
*                    
*                  Notes:
*                        - PE (奇偶校验错误), FE (桢错误), NE (噪声错误),
*                          ORE (过速错误) 和 IDLE (找到空闲线)挂起位 
*                          被软件按顺序清除: 一个读USART_SR寄存器操作（USART_GetFlagStatus()）
*                          接着一个读USART_DR寄存器操作(USART_ReceiveData()).
*                        - RXNE挂起位也可以被一个读USART_DR寄存器操作清除(USART_ReceiveData()).
*                        - TC挂起位也可以被软件按顺序清除: 一个读USART_SR寄存器操作（USART_GetFlagStatus()）
*                          接着一个读USART_DR寄存器操作(USART_ReceiveData()).                                                      
*                        - TXE挂起位置可以被一个写USART_DR寄存器清除(USART_SendData()).  
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void USART_ClearITPendingBit(USART_TypeDef* USARTx, u16 USART_IT)
{
  u16 bitpos = 0x00, itmask = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CLEAR_IT(USART_IT));
  assert_param(IS_USART_PERIPH_IT(USARTx, USART_IT)); /* The CTS interrupt is not available for UART4 and UART5 [CTS 改变中断 对于UART4和UART5无法使用]*/
  
  bitpos = USART_IT >> 0x08;

  itmask = (u16)((u16)0x01 << bitpos);
  USARTx->SR = (u16)~itmask;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
