/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_usart.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������USART�Ĺ̼�����
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
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

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: USART_DeInit
* ��������: ����USARTx��Χ�Ĵ���ΪĬ������ֵ.
* �������: USARTx: ѡ�����USART �������UART����.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_DeInit(USART_TypeDef* USARTx)
{
  /* Check the parameters [������]*/
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
* ��������: USART_Init
* ��������: ����USART_InitStruct�е�ָ���Ĳ�����ʼ��USARTx��Χ�豸 .
* �������: (1)USARTx: ѡ�����USART �������UART����.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_InitStruct��ָ������ض���USART��Χ�豸������Ϣ��USART_InitTypeDef�ṹ��..
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct)
{
  u32 tmpreg = 0x00, apbclock = 0x00;
  u32 integerdivider = 0x00;
  u32 fractionaldivider = 0x00;
  u32 usartxbase = 0;
  RCC_ClocksTypeDef RCC_ClocksStatus;

  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));  
  assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));
  assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));
  assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));
  assert_param(IS_USART_MODE(USART_InitStruct->USART_Mode));
  assert_param(IS_USART_HARDWARE_FLOW_CONTROL(USART_InitStruct->USART_HardwareFlowControl));
  /* The hardware flow control is available only for USART1, USART2 and USART3 [���Ӳ��������ֻ������USART1,USART2��USART3]*/          
  assert_param(IS_USART_PERIPH_HFC(USARTx, USART_InitStruct->USART_HardwareFlowControl));
  
  usartxbase = (*(u32*)&USARTx);

/*---------------------------- USART CR2 Configuration [USART CR2����]*/
  tmpreg = USARTx->CR2;
  /* Clear STOP[13:12] bits [���STOP[13:12]λ]*/
  tmpreg &= CR2_STOP_CLEAR_Mask;

  /* Configure the USART Stop Bits, Clock, CPOL, CPHA and LastBit [����USART STOPλ,Clock, CPOL, CPHA �� LastBit]*/
  /* Set STOP[13:12] bits according to USART_StopBits value [����USART_StopBits��ֵ����STOP[13:12]λ]*/
  tmpreg |= (u32)USART_InitStruct->USART_StopBits;
  
  /* Write to USART CR2 [дUSART CR2]*/
  USARTx->CR2 = (u16)tmpreg;

/*---------------------------- USART CR1 Configuration [USART CR1����]--------*/
  tmpreg = USARTx->CR1;
  /* Clear M, PCE, PS, TE and RE bits [��M, PCE, PS, TE��REλ]*/
  tmpreg &= CR1_CLEAR_Mask;

  /* Configure the USART Word Length, Parity and mode [����USART���ֳ�,��żУ���ģʽ] */
  /* Set the M bits according to USART_WordLength value [����USART_WordLengthֵ����Mλ]*/
  /* Set PCE and PS bits according to USART_Parity value [����USART_Parityֵ����PCE��PSλ]*/
  /* Set TE and RE bits according to USART_Mode value [����USART_Modeֵ����TE��REλ]*/
  tmpreg |= (u32)USART_InitStruct->USART_WordLength | USART_InitStruct->USART_Parity |
            USART_InitStruct->USART_Mode;

  /* Write to USART CR1 [дUSART CR1]*/
  USARTx->CR1 = (u16)tmpreg;

/*---------------------------- USART CR3 Configuration [USART CR3����]--------*/  
  tmpreg = USARTx->CR3;
  /* Clear CTSE and RTSE bits [��CTSE �� RTSEλ]*/
  tmpreg &= CR3_CLEAR_Mask;

  /* Configure the USART HFC [����USART HFC]----------------------------------*/
  /* Set CTSE and RTSE bits according to USART_HardwareFlowControl value [����USART_HardwareFlowControlֵ����CTSE��RTSEλ]*/
  tmpreg |= USART_InitStruct->USART_HardwareFlowControl;

  /* Write to USART CR3 [дUSART CR3]*/
  USARTx->CR3 = (u16)tmpreg;

/*---------------------------- USART BRR Configuration [USART BRR����]--------*/
  /* Configure the USART Baud Rate [����USART ������]-------------------*/
  RCC_GetClocksFreq(&RCC_ClocksStatus);
  if (usartxbase == USART1_BASE)
  {
    apbclock = RCC_ClocksStatus.PCLK2_Frequency;
  }
  else
  {
    apbclock = RCC_ClocksStatus.PCLK1_Frequency;
  }

  /* Determine the integer part [ȷ����������]*/
  integerdivider = ((0x19 * apbclock) / (0x04 * (USART_InitStruct->USART_BaudRate)));
  tmpreg = (integerdivider / 0x64) << 0x04;

  /* Determine the fractional part [ȷ��С������]*/
  fractionaldivider = integerdivider - (0x64 * (tmpreg >> 0x04));
  tmpreg |= ((((fractionaldivider * 0x10) + 0x32) / 0x64)) & ((u8)0x0F);

  /* Write to USART BRR [дUSART BRR]*/
  USARTx->BRR = (u16)tmpreg;
}

/*******************************************************************************
* ��������: USART_StructInit
* ��������: ��Ĭ��ֵ���ÿһ��USART_InitStruct��Ա.
* �������: USART_InitStruct: ָ��ᱻ��ʼ����USART_InitTypeDef�ṹ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_StructInit(USART_InitTypeDef* USART_InitStruct)
{
  /* USART_InitStruct members default value [USART_InitStruct�ṹ��Ա��Ĭ��ֵ]*/
  USART_InitStruct->USART_BaudRate = 9600;
  USART_InitStruct->USART_WordLength = USART_WordLength_8b;
  USART_InitStruct->USART_StopBits = USART_StopBits_1;
  USART_InitStruct->USART_Parity = USART_Parity_No ;
  USART_InitStruct->USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStruct->USART_HardwareFlowControl = USART_HardwareFlowControl_None;
}

/*******************************************************************************
* ��������: USART_ClockInit
* ��������: ����USART_ClockInitStructָ��������ʼ�� USARTx ��Χ�豸ʱ��  .
* �������: (1)USARTx: x������ 1, 2, 3 ��ѡ��USART����.
*                    ע��:���ܿ�ģʽ����ʹ�� UART4 �� UART5.
*           (2)USART_ClockInitStruct: USART_ClockInitTypeDef�Ľṹָ�����USART�豸��������Ϣ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct)
{
  u32 tmpreg = 0x00;

  /* Check the parameters [������]*/
  assert_param(IS_USART_123_PERIPH(USARTx));
  assert_param(IS_USART_CLOCK(USART_ClockInitStruct->USART_Clock));
  assert_param(IS_USART_CPOL(USART_ClockInitStruct->USART_CPOL));
  assert_param(IS_USART_CPHA(USART_ClockInitStruct->USART_CPHA));
  assert_param(IS_USART_LASTBIT(USART_ClockInitStruct->USART_LastBit));              
  
/*---------------------------- USART CR2 Configuration [USART CR2����]--------*/
  tmpreg = USARTx->CR2;
  /* Clear CLKEN, CPOL, CPHA and LBCL bits [��CLKEN,CPOL,CPHA��LBCLλ]*/
  tmpreg &= CR2_CLOCK_CLEAR_Mask;

  /* Configure the USART Clock, CPOL, CPHA and LastBit [����USART STOPλ,Clock, CPOL, CPHA �� LastBit]*/
  /* Set CLKEN bit according to USART_Clock value [����USART_Clockֵ����CLKENλ]*/
  /* Set CPOL bit according to USART_CPOL value [����USART_CPOLֵ����CPOLλ]*/
  /* Set CPHA bit according to USART_CPHA value [����USART_CPHAֵ����CPHAλ]*/
  /* Set LBCL bit according to USART_LastBit value [����USART_LastBitֵ����LBCLλ]*/
  tmpreg |= (u32)USART_ClockInitStruct->USART_Clock | USART_ClockInitStruct->USART_CPOL | 
                 USART_ClockInitStruct->USART_CPHA | USART_ClockInitStruct->USART_LastBit;

  /* Write to USART CR2 [дUSART CR2]*/
  USARTx->CR2 = (u16)tmpreg;
}

/*******************************************************************************
* ��������: USART_ClockStructInit
* ��������: ��Ĭ��ֵ���ÿһ��USART_ClockInitStruct�ĳ�Ա.
* �������: - USART_ClockInitStruct: ��Ҫ��ʼ��ָ��USART_ClockInitTypeDef�ṹ��ָ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_ClockStructInit(USART_ClockInitTypeDef* USART_ClockInitStruct)
{
  /* USART_ClockInitStruct members default value [USART_ClockInitStruct��ԱĬ��ֵ]*/
  USART_ClockInitStruct->USART_Clock = USART_Clock_Disable;
  USART_ClockInitStruct->USART_CPOL = USART_CPOL_Low;
  USART_ClockInitStruct->USART_CPHA = USART_CPHA_1Edge;
  USART_ClockInitStruct->USART_LastBit = USART_LastBit_Disable;
}

/*******************************************************************************
* ��������: USART_Cmd
* ��������: Eʹ�ܻ�����ض���USART��Χ�豸.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)Newstate��USARTx��Χ�豸����״̬.��������ܹ�ȡ��ENABLE����DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected USART by setting the UE bit in the CR1 register [��λCR1�Ĵ����е�UEλ,��ʹ��ѡ���USART]*/
    USARTx->CR1 |= CR1_UE_Set;
  }
  else
  {
    /* Disable the selected USART by clearing the UE bit in the CR1 register [����CR1�Ĵ����е�UEλ,����ֹѡ���USART]*/
    USARTx->CR1 &= CR1_UE_Reset;
  }
}

/*******************************************************************************
* ��������: USART_ITConfig
* ��������: ʹ�ܻ�����ض���USART�ж�.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_IT��ָ��Ҫʹ�ܻ���õ�USART�ж�Դ��
*                    ������������������ֵ֮һ :
*                       - USART_IT_CTS:  CTS �ı��ж� (����UART4��UART5�޷�ʹ��)
*                       - USART_IT_LBD:  LIN �������ж�
*                       - USART_IT_TXE:  ���ͼĴ������ж�
*                       - USART_IT_TC:   ��������ж�
*                       - USART_IT_RXNE: ���ռĴ���ȫ���ж�
*                       - USART_IT_IDLE: IDLE�����ж�
*                       - USART_IT_PE:   ��ż�����ж�
*                       - USART_IT_ERR:  �����ж�(�����, ��������, ���ٴ���)
*           (3)ָ��USARTx�жϵ���״̬.��������ܹ�ȡ:ENABLE����DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_ITConfig(USART_TypeDef* USARTx, u16 USART_IT, FunctionalState NewState)
{
  u32 usartreg = 0x00, itpos = 0x00, itmask = 0x00;
  u32 usartxbase = 0x00;

  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CONFIG_IT(USART_IT));
  assert_param(IS_USART_PERIPH_IT(USARTx, USART_IT)); /* The CTS interrupt is not available for UART4 and UART5 [CTS�ж϶���UART4��UART5�޷�ʹ��]*/     
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  usartxbase = (*(u32*)&(USARTx));

  /* Get the USART register index [ȡ��USART�Ĵ�����ַ]*/
  usartreg = (((u8)USART_IT) >> 0x05);

  /* Get the interrupt position [ȡ���ж�λ��]*/
  itpos = USART_IT & IT_Mask;

  itmask = (((u32)0x01) << itpos);
    
  if (usartreg == 0x01) /* The IT is in CR1 register [�ж���CR1�Ĵ�����]*/
  {
    usartxbase += 0x0C;
  }
  else if (usartreg == 0x02) /* The IT is in CR2 register [�ж���CR2�Ĵ�����]*/
  {
    usartxbase += 0x10;
  }
  else /* The IT is in CR3 register [�ж���CR3�Ĵ�����]*/
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
* ��������: USART_DMACmd
* ��������: ʹ�ܻ�����ض���USART DMA�ӿ�.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3 or UART4.
*                    ע��: ���DMA��������UART5.
*           (2)USART_DMAReq: ָ��DMA����.
*                    ���������������������ֵ�����:
*                       - USART_DMAReq_Tx: USART DMA ��������
*                       - USART_DMAReq_Rx: USART DMA ��������
*           (3)NewState: DMA����Դ����״̬.��������ܹ�ȡ:ENABLE����DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_DMACmd(USART_TypeDef* USARTx, u16 USART_DMAReq, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_1234_PERIPH(USARTx));
  assert_param(IS_USART_DMAREQ(USART_DMAReq));  
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 

  if (NewState != DISABLE)
  {
    /* Enable the DMA transfer for selected requests by setting the DMAT and/or
       DMAR bits in the USART CR3 register [ͨ������USART CR3 �Ĵ�����DMAT��DMATλ���,��ʹ��ѡ��DMA��������]*/
    USARTx->CR3 |= USART_DMAReq;
  }
  else
  {
    /* Disable the DMA transfer for selected requests by clearing the DMAT and/or
       DMAR bits in the USART CR3 register [ͨ������USART CR3 �Ĵ�����DMAT��DMATλ���,����ֹѡ��DMA��������]*/
    USARTx->CR3 &= (u16)~USART_DMAReq;
  }
}

/*******************************************************************************
* ��������: USART_SetAddress
* ��������: ����USART�ڵ�ĵ�ַ
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_Address��ָ����USART�ڵ�ĵ�ַ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_SetAddress(USART_TypeDef* USARTx, u8 USART_Address)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_ADDRESS(USART_Address)); 
    
  /* Clear the USART address [��� USART��ַ]*/
  USARTx->CR2 &= CR2_Address_Mask;
  /* Set the USART address node [����USART��ַ]*/
  USARTx->CR2 |= USART_Address;
}

/*******************************************************************************
* ��������: USART_WakeUpConfig
* ��������: ѡ��USART�Ļ��ѷ���.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_WakeUp:ָ����USART�Ļ��ѷ���.
*                    ������������������ֵ֮һ :
*                        - USART_WakeUp_IdleLine: IDLE��·����
*                        - USART_WakeUp_AddressMark: ��ַ��ǻ���
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_WakeUpConfig(USART_TypeDef* USARTx, u16 USART_WakeUp)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_WAKEUP(USART_WakeUp));
  
  USARTx->CR1 &= CR1_WAKE_Mask;
  USARTx->CR1 |= USART_WakeUp;
}

/*******************************************************************************
* ��������: USART_ReceiverWakeUpCmd
* ��������: ����USART�Ƿ�������ģʽ.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)Newstate��USARTģʽ����״̬.��������ܹ�ȡ��ENABLE����DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_ReceiverWakeUpCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  
  if (NewState != DISABLE)
  {
    /* Enable the USART mute mode  by setting the RWU bit in the CR1 register [ͨ����λCR1�Ĵ�����RWUλʹ��USART����ģʽ]*/
    USARTx->CR1 |= CR1_RWU_Set;
  }
  else
  {
    /* Disable the USART mute mode by clearing the RWU bit in the CR1 register [ͨ������CR1�Ĵ�����RWUλ��ֹUSART����ģʽ]*/
    USARTx->CR1 &= CR1_RWU_Reset;
  }
}

/*******************************************************************************
* ��������: USART_LINBreakDetectLengthConfig
* ��������: ����USART LIN����첨����.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_LINBreakDetectLengthָ����LIN��ϼ�ⳤ��.
*                    ������������������ֵ֮һ :
*                       - USART_LINBreakDetectLength_10b: 10λ��ϼ��
*                       - USART_LINBreakDetectLength_11b: 11λ��ϼ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, u16 USART_LINBreakDetectLength)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_LIN_BREAK_DETECT_LENGTH(USART_LINBreakDetectLength));
  
  USARTx->CR2 &= CR2_LBDL_Mask;
  USARTx->CR2 |= USART_LINBreakDetectLength;  
}

/*******************************************************************************
* ��������: USART_LINCmd
* ��������: ʹ�ܻ����USART LINģʽ.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)Newstate:USART LINģʽ����״̬.��������ܹ�ȡ:ENABLE����DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_LINCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the LIN mode by setting the LINEN bit in the CR2 register [ͨ����λCR2�Ĵ�����LINENλʹ��LINģʽ]*/
    USARTx->CR2 |= CR2_LINEN_Set;
  }
  else
  {
    /* Disable the LIN mode by clearing the LINEN bit in the CR2 register [ͨ������CR2�Ĵ�����LINENλ��ֹLINģʽ]*/
    USARTx->CR2 &= CR2_LINEN_Reset;
  }
}

/*******************************************************************************
* ��������: USART_SendData
* ��������: ���䵥������ͨ��USARTx��Χ�豸.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)Data:Ҫ�����������.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_SendData(USART_TypeDef* USARTx, u16 Data)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_DATA(Data)); 
    
  /* Transmit Data */
  USARTx->DR = (Data & (u16)0x01FF);
}

/*******************************************************************************
* ��������: USART_ReceiveData
* ��������: ���������USARTx��Χ�豸���յ�����.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
* �������: ��
* ���ز���: ���յ�����.
*******************************************************************************/
u16 USART_ReceiveData(USART_TypeDef* USARTx)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  
  /* Receive Data */
  return (u16)(USARTx->DR & (u16)0x01FF);
}

/*******************************************************************************
* ��������: USART_SendBreak
* ��������: �����Ϸ�.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_SendBreak(USART_TypeDef* USARTx)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  
  /* Send break characters [���ͼ���ַ�]*/
  USARTx->CR1 |= CR1_SBK_Set;
}

/*******************************************************************************
* ��������: USART_SetGuardTime
* ��������: ����ָ����USART����ʱ��.
* �������: (1)USARTx:x����Ϊ1,2��3��ѡ��USART����Χ�豸.
*                  ע��: �������ʱ��λ��������UART4��UART5.
*           (2)USART_GuardTime:ָ������ʱ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_SetGuardTime(USART_TypeDef* USARTx, u8 USART_GuardTime)
{    
  /* Check the parameters [������]*/
  assert_param(IS_USART_123_PERIPH(USARTx));
  
  /* Clear the USART Guard time [���USART����ʱ��]*/
  USARTx->GTPR &= GTPR_LSB_Mask;
  /* Set the USART guard time [����USART����ʱ��*/
  USARTx->GTPR |= (u16)((u16)USART_GuardTime << 0x08);
}

/*******************************************************************************
* ��������: USART_SetPrescaler
* ��������: ����USARTʱ��Ԥ��Ƶ��.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*                  ע��:�������������UART4��UART5��IrDAģʽ .
*           (2)USART_Prescaler:ָ��Ԥ��Ƶ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_SetPrescaler(USART_TypeDef* USARTx, u8 USART_Prescaler)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  
  /* Clear the USART prescaler [���USARTԤ��Ƶ��]*/
  USARTx->GTPR &= GTPR_MSB_Mask;
  /* Set the USART prescaler [����USARTԤ��Ƶ��]*/
  USARTx->GTPR |= USART_Prescaler;
}

/*******************************************************************************
* ��������: USART_SmartCardCmd
* ��������: ʹ�ܻ����USART���ܿ�ģʽ.
* �������: (1)USARTx:x����Ϊ1,2��3��ѡ��USART����Χ�豸. 
*                    ע��:������ܿ�ģʽ��������UART4��UART5.
*           (2)Newstate:���ܿ�ģʽ����״̬.��������ܹ�ȡ:ENABLE����DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_SmartCardCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_123_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the SC mode by setting the SCEN bit in the CR3 register [ͨ����λCR3�Ĵ�����SCENλʹ��SCģʽ]*/
    USARTx->CR3 |= CR3_SCEN_Set;
  }
  else
  {
    /* Disable the SC mode by clearing the SCEN bit in the CR3 register [ͨ������CR3�Ĵ�����SCENλ��ֹSCģʽ]*/
    USARTx->CR3 &= CR3_SCEN_Reset;
  }
}

/*******************************************************************************
* ��������: USART_SmartCardNACKCmd
* ��������: ʹ�ܻ����NACK����.
* �������: (1)USARTx:x����Ϊ1,2��3��ѡ��USART����Χ�豸. 
*                    ע��:������ܿ�ģʽ��������UART4��UART5.
*           (2)Newstate:NACK�������״̬.��������ܹ�ȡ:ENABLE����DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_SmartCardNACKCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_123_PERIPH(USARTx));  
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the NACK transmission by setting the NACK bit in the CR3 register [ͨ����λCR3�Ĵ�����NACKλʹ��NACK����]*/
    USARTx->CR3 |= CR3_NACK_Set;
  }
  else
  {
    /* Disable the NACK transmission by clearing the NACK bit in the CR3 register [ͨ������CR3�Ĵ�����NACKλ��ֹNACK����]*/
    USARTx->CR3 &= CR3_NACK_Reset;
  }
}

/*******************************************************************************
* ��������: USART_HalfDuplexCmd
* ��������: ʹ�ܻ����USART��˫��ģʽ.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)Newstate:��˫��ģʽ����״̬.��������ܹ�ȡ��ENABLE����DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_HalfDuplexCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the Half-Duplex mode by setting the HDSEL bit in the CR3 register [ͨ����λCR3�Ĵ�����HDSELλʹ��Half-Duplexģʽ]*/
    USARTx->CR3 |= CR3_HDSEL_Set;
  }
  else
  {
    /* Disable the Half-Duplex mode by clearing the HDSEL bit in the CR3 register [ͨ������CR3�Ĵ�����HDSELλ��ֹHalf-Duplexģʽ]*/
    USARTx->CR3 &= CR3_HDSEL_Reset;
  }
}

/*******************************************************************************
* ��������: USART_IrDAConfig
* ��������: ����USART IrDAģʽ.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_IrDAMode:ָ��IrDAģʽ.
*                    ������������������ֵ֮һ :
*                       - USART_IrDAMode_LowPower
*                       - USART_IrDAMode_Normal
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_IrDAConfig(USART_TypeDef* USARTx, u16 USART_IrDAMode)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_IRDA_MODE(USART_IrDAMode));
    
  USARTx->CR3 &= CR3_IRLP_Mask;
  USARTx->CR3 |= USART_IrDAMode;
}

/*******************************************************************************
* ��������: USART_IrDACmd
* ��������: ʹ�ܻ����USART IrDA�ӿ�.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)Newstate:IrDAģʽ����״̬.��������ܹ�ȡ��ENABLE����DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_IrDACmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
    
  if (NewState != DISABLE)
  {
    /* Enable the IrDA mode by setting the IREN bit in the CR3 register [ͨ����λCR3�Ĵ�����IRENλʹ��IrDAģʽ]*/
    USARTx->CR3 |= CR3_IREN_Set;
  }
  else
  {
    /* Disable the IrDA mode by clearing the IREN bit in the CR3 register [ͨ������CR3�Ĵ�����IRENλ��ֹIrDAģʽ]*/
    USARTx->CR3 &= CR3_IREN_Reset;
  }
}

/*******************************************************************************
* ��������: USART_GetFlagStatus
* ��������: ���ָ����USART����Ƿ���λ.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_FLAG:ָ��Ҫ���ı��.
*                    ������������������ֵ֮һ :
*                       - USART_FLAG_CTS:  CTS �ı��־ (����UART4��UART5�޷�ʹ��)
*                       - USART_FLAG_LBD:  LIN �������־
*                       - USART_FLAG_TXE:  ���ͼĴ����ձ�־
*                       - USART_FLAG_TC:   ������ɱ�־
*                       - USART_FLAG_RXNE: ���ռĴ���ȫ����־
*                       - USART_FLAG_IDLE: IDLE���Ա�־
*                       - USART_FLAG_ORE:  ���ٴ����־
*                       - USART_FLAG_NE:   ���������־
*                       - USART_FLAG_FE:   ��֡����־
*                       - USART_FLAG_PE:   ��ż�����־
* �������: ��
* ���ز���: USART_FLAG��״̬.��������ܹ�ȡ:ENABLE����DISABLE.
*******************************************************************************/
FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, u16 USART_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_FLAG(USART_FLAG));
  assert_param(IS_USART_PERIPH_FLAG(USARTx, USART_FLAG)); /* �Ÿ�CTS��־��������UART4��UART5 */   

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
* ��������: USART_ClearFlag
* ��������: ���USARTx������.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_FLAG��ָ��Ҫ������ı��.
*                    ������������������ֵ���������:
*                       - USART_FLAG_CTS:  CTS �ı��־ (����UART4��UART5�޷�ʹ��).
*                       - USART_FLAG_LBD:  LIN �������־.
*                       - USART_FLAG_TC:   ������ɱ�־.
*                       - USART_FLAG_RXNE: ���ռĴ���ȫ����־.
*
*                  ע��:
*                        - PE (��żУ�����), FE (�����), NE (��������),
*                          ORE (���ٴ���) �� IDLE (�ҵ�������) ��־��־�������˳�����:
*                          һ����USART_SR�Ĵ���������USART_GetFlagStatus()��
*                          ����һ����USART_DR�Ĵ�������(USART_ReceiveData()).
*                        - RXNE��־Ҳ���Ա�һ����USART_DR�Ĵ����������(USART_ReceiveData()).
*                        - TC��־��־�������˳�����: һ����USART_SR�Ĵ���������USART_GetFlagStatus()��
*                          ����һ����USART_DR�Ĵ�������(USART_ReceiveData()).                                                      
*                        - TXE��־ֻ�ܿ�дUSART_DR�Ĵ������(USART_SendData()).                        
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_ClearFlag(USART_TypeDef* USARTx, u16 USART_FLAG)
{
  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CLEAR_FLAG(USART_FLAG));
  assert_param(IS_USART_PERIPH_FLAG(USARTx, USART_FLAG)); /* The CTS flag is not available for UART4 and UART5 [CTS �ı��ж� ����UART4��UART5�޷�ʹ��]*/   
   
  USARTx->SR = (u16)~USART_FLAG;
}

/*******************************************************************************
* ��������: USART_GetITStatus
* ��������: ���ָ����USART�ж��Ƿ���.
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_IT:ָ��Ҫ����USART�ж�Դ.
*                    ������������������ֵ֮һ :
*                       - USART_IT_CTS:  CTS �ı��ж� (����UART4��UART5�޷�ʹ��)
*                       - USART_IT_LBD:  LIN �������ж�
*                       - USART_IT_TXE:  ���ͼĴ������ж�
*                       - USART_IT_TC:   ��������ж�
*                       - USART_IT_RXNE: ���ռĴ���ȫ���ж�
*                       - USART_IT_IDLE: IDLE�����ж�
*                       - USART_IT_ORE:  ���ٴ����ж�
*                       - USART_IT_NE:   ���������ж�
*                       - USART_IT_FE:   ������ж�
*                       - USART_IT_PE:   ��ż�����ж�
* �������: ��
* ���ز���: The new state of USART_IT (SET or RESET).
*******************************************************************************/
ITStatus USART_GetITStatus(USART_TypeDef* USARTx, u16 USART_IT)
{
  u32 bitpos = 0x00, itmask = 0x00, usartreg = 0x00;
  ITStatus bitstatus = RESET;

  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_GET_IT(USART_IT));
  assert_param(IS_USART_PERIPH_IT(USARTx, USART_IT)); /* The CTS interrupt is not available for UART4 and UART5 [CTS �ı��ж� ����UART4��UART5�޷�ʹ��]*/  
  
  /* Get the USART register index [ȡ��USART�Ĵ�������]*/
  usartreg = (((u8)USART_IT) >> 0x05);

  /* Get the interrupt position [ȡ���ж�λ��]*/
  itmask = USART_IT & IT_Mask;

  itmask = (u32)0x01 << itmask;
  
  if (usartreg == 0x01) /* The IT  is in CR1 register [CR1�Ĵ����ڵ��ж�]*/
  {
    itmask &= USARTx->CR1;
  }
  else if (usartreg == 0x02) /* The IT  is in CR2 register [CR2�Ĵ����ڵ��ж�]*/
  {
    itmask &= USARTx->CR2;
  }
  else /* The IT  is in CR3 register [CR3�Ĵ����ڵ��ж�]*/
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
* ��������: USART_ClearITPendingBit
* ��������: ���USARTx�жϹ���λ
* �������: (1)USARTx: ѡ�� USART ���� UART����Χ�豸.
*                    ������������������ֵ֮һ :
*                     - USART1, USART2, USART3, UART4 or UART5.
*           (2)USART_IT��ָ��Ҫ��������жϹ���λ.
*                    ������������������ֵ֮һ:
*                       - USART_IT_CTS:  CTS �ı��ж� (����UART4��UART5�޷�ʹ��)
*                       - USART_IT_LBD:  LIN �������ж�
*                       - USART_IT_TC:   ��������ж�. 
*                       - USART_IT_RXNE: ���ռĴ���ȫ���ж�.
*                    
*                  Notes:
*                        - PE (��żУ�����), FE (�����), NE (��������),
*                          ORE (���ٴ���) �� IDLE (�ҵ�������)����λ 
*                          �������˳�����: һ����USART_SR�Ĵ���������USART_GetFlagStatus()��
*                          ����һ����USART_DR�Ĵ�������(USART_ReceiveData()).
*                        - RXNE����λҲ���Ա�һ����USART_DR�Ĵ����������(USART_ReceiveData()).
*                        - TC����λҲ���Ա������˳�����: һ����USART_SR�Ĵ���������USART_GetFlagStatus()��
*                          ����һ����USART_DR�Ĵ�������(USART_ReceiveData()).                                                      
*                        - TXE����λ�ÿ��Ա�һ��дUSART_DR�Ĵ������(USART_SendData()).  
* �������: ��
* ���ز���: ��
*******************************************************************************/
void USART_ClearITPendingBit(USART_TypeDef* USARTx, u16 USART_IT)
{
  u16 bitpos = 0x00, itmask = 0x00;

  /* Check the parameters [������]*/
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CLEAR_IT(USART_IT));
  assert_param(IS_USART_PERIPH_IT(USARTx, USART_IT)); /* The CTS interrupt is not available for UART4 and UART5 [CTS �ı��ж� ����UART4��UART5�޷�ʹ��]*/
  
  bitpos = USART_IT >> 0x08;

  itmask = (u16)((u16)0x01 << bitpos);
  USARTx->SR = (u16)~itmask;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
