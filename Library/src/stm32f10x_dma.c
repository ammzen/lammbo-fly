/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_dma.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������DMA�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
/* DMA ENABLE mask */
#define CCR_ENABLE_Set          ((u32)0x00000001)
#define CCR_ENABLE_Reset        ((u32)0xFFFFFFFE)

/* DMA1 Channelx interrupt pending bit masks */
#define DMA1_Channel1_IT_Mask    ((u32)0x0000000F)
#define DMA1_Channel2_IT_Mask    ((u32)0x000000F0)
#define DMA1_Channel3_IT_Mask    ((u32)0x00000F00)
#define DMA1_Channel4_IT_Mask    ((u32)0x0000F000)
#define DMA1_Channel5_IT_Mask    ((u32)0x000F0000)
#define DMA1_Channel6_IT_Mask    ((u32)0x00F00000)
#define DMA1_Channel7_IT_Mask    ((u32)0x0F000000)

/* DMA2 Channelx interrupt pending bit masks */
#define DMA2_Channel1_IT_Mask    ((u32)0x0000000F)
#define DMA2_Channel2_IT_Mask    ((u32)0x000000F0)
#define DMA2_Channel3_IT_Mask    ((u32)0x00000F00)
#define DMA2_Channel4_IT_Mask    ((u32)0x0000F000)
#define DMA2_Channel5_IT_Mask    ((u32)0x000F0000)

/* DMA2 FLAG mask */
#define FLAG_Mask                ((u32)0x10000000)

/* DMA registers Masks */
#define CCR_CLEAR_Mask           ((u32)0xFFFF800F)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: DMA_DeInit
* ��������: ��DMA��ͨ���Ĵ�����λΪĬ�ϵĸ�λֵ.
* �������: DMAy_Channelx��y������1����2��DMA1��x������1��7 ��DMA2��x������1��5��ѡ���ͨ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DMA_DeInit(DMA_Channel_TypeDef* DMAy_Channelx)
{
  /* Check the parameters [������]*/
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));

  /* Disable the selected DMAy Channelx [��ֹѡ���DMAy ͨ��x]*/
  DMAy_Channelx->CCR &= CCR_ENABLE_Reset;

  /* Reset DMAy Channelx control register [��λDMAy Channelx�Ŀ��ƼĴ���]*/
  DMAy_Channelx->CCR  = 0;
  
  /* Reset DMAy Channelx remaining bytes register [��λDMAy Channelx��ʣ���ֽڼĴ���]*/
  DMAy_Channelx->CNDTR = 0;
  
  /* Reset DMAy Channelx peripheral address register [��λDMAy Channelx��Χ�豸��ַ�Ĵ���]*/
  DMAy_Channelx->CPAR  = 0;
  
  /* Reset DMAy Channelx memory address register [��λDMAy Channelx�ڴ��ַ�Ĵ���]*/
  DMAy_Channelx->CMAR = 0;

  switch (*(u32*)&DMAy_Channelx)
  {
    case DMA1_Channel1_BASE:
      /* Reset interrupt pending bits for DMA1 Channel1 [��λDMA1 Channel1�жϹ���λ]*/
      DMA1->IFCR |= DMA1_Channel1_IT_Mask;
      break;

    case DMA1_Channel2_BASE:
      /* Reset interrupt pending bits for DMA1 Channel2 [��λDMA1 Channel2�жϹ���λ]*/
      DMA1->IFCR |= DMA1_Channel2_IT_Mask;
      break;

    case DMA1_Channel3_BASE:
      /* Reset interrupt pending bits for DMA1 Channel3 [��λDMA1 Channel3�жϹ���λ]*/
      DMA1->IFCR |= DMA1_Channel3_IT_Mask;
      break;

    case DMA1_Channel4_BASE:
      /* Reset interrupt pending bits for DMA1 Channel4 [��λDMA1 Channel4�жϹ���λ]*/
      DMA1->IFCR |= DMA1_Channel4_IT_Mask;
      break;

    case DMA1_Channel5_BASE:
      /* Reset interrupt pending bits for DMA1 Channel5 [��λDMA1 Channel5�жϹ���λ]*/
      DMA1->IFCR |= DMA1_Channel5_IT_Mask;
      break;

    case DMA1_Channel6_BASE:
      /* Reset interrupt pending bits for DMA1 Channel6 [��λDMA1 Channel6�жϹ���λ]*/
      DMA1->IFCR |= DMA1_Channel6_IT_Mask;
      break;

    case DMA1_Channel7_BASE:
      /* Reset interrupt pending bits for DMA1 Channel7 [��λDMA1 Channel7�жϹ���λ]*/
      DMA1->IFCR |= DMA1_Channel7_IT_Mask;
      break;

    case DMA2_Channel1_BASE:
      /* Reset interrupt pending bits for DMA2 Channel1 [��λDMA2 Channel1�жϹ���λ]*/
      DMA2->IFCR |= DMA2_Channel1_IT_Mask;
      break;

    case DMA2_Channel2_BASE:
      /* Reset interrupt pending bits for DMA2 Channel2 [��λDMA2 Channel2�жϹ���λ]*/
      DMA2->IFCR |= DMA2_Channel2_IT_Mask;
      break;

    case DMA2_Channel3_BASE:
      /* Reset interrupt pending bits for DMA2 Channel3 [��λDMA2 Channel3�жϹ���λ]*/
      DMA2->IFCR |= DMA2_Channel3_IT_Mask;
      break;

    case DMA2_Channel4_BASE:
      /* Reset interrupt pending bits for DMA2 Channel4 [��λDMA2 Channel4�жϹ���λ]*/
      DMA2->IFCR |= DMA2_Channel4_IT_Mask;
      break;

    case DMA2_Channel5_BASE:
      /* Reset interrupt pending bits for DMA2 Channel5 [��λDMA2 Channel5�жϹ���λ]*/
      DMA2->IFCR |= DMA2_Channel5_IT_Mask;
      break;
      
    default:
      break;
  }
}

/*******************************************************************************
* ��������: DMA_Init
* ��������: ����DMA_InitStructָ���Ĳ�����ʼ��DMA��ͨ��.
* �������: (1)DMAy_Channelx��y������1����2��DMA1��x������1��7 ��DMA2��x������1��5��ѡ���ͨ��.
*           (2)DMA_InitStruct��ָ��DMA_InitTypeDef���������ض�DMAƵ����������Ϣ��
* �������: ��
* ���ز���: ��
******************************************************************************/
void DMA_Init(DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct)
{
  u32 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_DMA_DIR(DMA_InitStruct->DMA_DIR));
  assert_param(IS_DMA_BUFFER_SIZE(DMA_InitStruct->DMA_BufferSize));
  assert_param(IS_DMA_PERIPHERAL_INC_STATE(DMA_InitStruct->DMA_PeripheralInc));
  assert_param(IS_DMA_MEMORY_INC_STATE(DMA_InitStruct->DMA_MemoryInc));   
  assert_param(IS_DMA_PERIPHERAL_DATA_SIZE(DMA_InitStruct->DMA_PeripheralDataSize));
  assert_param(IS_DMA_MEMORY_DATA_SIZE(DMA_InitStruct->DMA_MemoryDataSize));
  assert_param(IS_DMA_MODE(DMA_InitStruct->DMA_Mode));
  assert_param(IS_DMA_PRIORITY(DMA_InitStruct->DMA_Priority));
  assert_param(IS_DMA_M2M_STATE(DMA_InitStruct->DMA_M2M));

/*--------------------------- DMAy Channelx CCR Configuration [DMAy ͨ��x CCR����]-----------------*/
  /* Get the DMAy_Channelx CCR value [ȡ��DMAy_Channelx CCR��ֵ]*/
  tmpreg = DMAy_Channelx->CCR;
  /* Clear MEM2MEM, PL, MSIZE, PSIZE, MINC, PINC, CIRC and DIR bits [��MEM2MEM, PL, MSIZE, PSIZE, MINC, PINC, CIRC �� DIRλ]*/
  tmpreg &= CCR_CLEAR_Mask;
  /* Configure DMAy Channelx: data transfer, data size, priority level and mode [����DMAy ͨ��x:���ݴ��䣬�������ͣ����ȼ���ģʽ]*/
  /* Set DIR bit according to DMA_DIR value [����DMA_DIR��ֵ����DIRλ]*/
  /* Set CIRC bit according to DMA_Mode value [����DMA_Mode��ֵ����CIRCλ]*/
  /* Set PINC bit according to DMA_PeripheralInc value [����DMA_PeripheralInc��ֵ����PINCλ]*/
  /* Set MINC bit according to DMA_MemoryInc value [����DMA_MemoryInc��ֵ����MINCλ]*/
  /* Set PSIZE bits according to DMA_PeripheralDataSize value [����DMA_PeripheralDataSize��ֵ����PSIZEλ]*/
  /* Set MSIZE bits according to DMA_MemoryDataSize value [����DMA_MemoryDataSize��ֵ����MSIZEλ]*/
  /* Set PL bits according to DMA_Priority value [����DMA_Priority��ֵ����PLλ]*/
  /* Set the MEM2MEM bit according to DMA_M2M value [����DMA_M2M��ֵ����MEM2MEMλ]*/
  tmpreg |= DMA_InitStruct->DMA_DIR | DMA_InitStruct->DMA_Mode |
            DMA_InitStruct->DMA_PeripheralInc | DMA_InitStruct->DMA_MemoryInc |
            DMA_InitStruct->DMA_PeripheralDataSize | DMA_InitStruct->DMA_MemoryDataSize |
            DMA_InitStruct->DMA_Priority | DMA_InitStruct->DMA_M2M;
  /* Write to DMAy Channelx CCR [дDMAyͨ��x CCR]*/
  DMAy_Channelx->CCR = tmpreg;

/*--------------------------- DMAy Channelx CNDTR Configuration [DMAy ͨ��x CNDTR����]---------------*/
  /* Write to DMAy Channelx CNDTR [дDMAyͨ��x CNDTR]*/
  DMAy_Channelx->CNDTR = DMA_InitStruct->DMA_BufferSize;

/*--------------------------- DMAy Channelx CPAR Configuration [DMAy ͨ��x CPAR����]----------------*/
  /* Write to DMAy Channelx CPAR [дDMAyͨ��x CPAR]*/
  DMAy_Channelx->CPAR = DMA_InitStruct->DMA_PeripheralBaseAddr;

/*--------------------------- DMAy Channelx CMAR Configuration [DMAy ͨ��x CMAR����]----------------*/
  /* Write to DMAy Channelx CMAR [дDMAyͨ��x CMAR]*/
  DMAy_Channelx->CMAR = DMA_InitStruct->DMA_MemoryBaseAddr;
}

/*******************************************************************************
* ��������: DMA_StructInit
* ��������: ��DMA_InitStruct��ÿ����Ա��λΪĬ��ֵ.
* �������: DMA_InitStruc��ָ����Ҫ��ʼ����DMA_InitTypeDef�ṹ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct)
{
/*-------------- Reset DMA init structure parameters values ------------------*/
  /* Initialize the DMA_PeripheralBaseAddr member [��ʼ��DMA_PeripheralBaseAddr��Ա]*/
  DMA_InitStruct->DMA_PeripheralBaseAddr = 0;

  /* Initialize the DMA_MemoryBaseAddr member [��ʼ��DMA_MemoryBaseAddr��Ա]*/
  DMA_InitStruct->DMA_MemoryBaseAddr = 0;

  /* Initialize the DMA_DIR member [��ʼ��DMA_DIR��Ա]*/
  DMA_InitStruct->DMA_DIR = DMA_DIR_PeripheralSRC;

  /* Initialize the DMA_BufferSize member[��ʼ��DMA_BufferSize��Ա] */
  DMA_InitStruct->DMA_BufferSize = 0;

  /* Initialize the DMA_PeripheralInc member [��ʼ��DMA_PeripheralInc��Ա]*/
  DMA_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Disable;

  /* Initialize the DMA_MemoryInc member [��ʼ��DMA_MemoryInc��Ա]*/
  DMA_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Disable;

  /* Initialize the DMA_PeripheralDataSize member [��ʼ��DMA_PeripheralDataSize��Ա]*/
  DMA_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

  /* Initialize the DMA_MemoryDataSize member [��ʼ��DMA_MemoryDataSize��Ա]*/
  DMA_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

  /* Initialize the DMA_Mode member [��ʼ��DMA_DMA_Mode��Ա]*/
  DMA_InitStruct->DMA_Mode = DMA_Mode_Normal;

  /* Initialize the DMA_Priority member [��ʼ��DMA_Priority��Ա]*/
  DMA_InitStruct->DMA_Priority = DMA_Priority_Low;

  /* Initialize the DMA_M2M member [��ʼ��DMA_M2M��Ա]*/
  DMA_InitStruct->DMA_M2M = DMA_M2M_Disable;
}

/*******************************************************************************
* ��������: DMA_Cmd
* ��������: ʹ�ܣ�����ܣ��ض���DMAyͨ��x .
* �������: (1)DMAy_Channelx��y������1����2��DMA1��x������1��7 ��DMA2��x������1��5��ѡ���ͨ��.
*           (2)NewState��DMAͨ������״̬���ò�����Ϊ��ENABLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DMA_Cmd(DMA_Channel_TypeDef* DMAy_Channelx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected DMAy Channelx [ʹ��ѡ���DMAyͨ��x]*/
    DMAy_Channelx->CCR |= CCR_ENABLE_Set;
  }
  else
  {
    /* Disable the selected DMAy Channelx [��ֹѡ���DMAyͨ��x]*/
    DMAy_Channelx->CCR &= CCR_ENABLE_Reset;
  }
}

/*******************************************************************************
* ��������: DMA_ITConfig
* ��������: ʹ�ܣ�����ܣ��ض���DMAyͨ��x���ж�.
* �������: (1)DMAy_Channelx��y������1����2��DMA1��x������1��7 ��DMA2��x������1��5��ѡ���ͨ��.
*           (2)DMA_IT: ȷ��DMAͨ�����ж�Դ�Ƿ�ʹ�ܡ� 
*                    �����������������ֵ���������:
*                       - DMA_IT_TC:  ��������ж����� mask
*                       - DMA_IT_HT:  �봫���ж����� mask
*                       - DMA_IT_TE:  ��������ж����� mask
*           (3)NewState��ָ����DMAͨ���жϵ���״̬���ò�����Ϊ��ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DMA_ITConfig(DMA_Channel_TypeDef* DMAy_Channelx, u32 DMA_IT, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_DMA_CONFIG_IT(DMA_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected DMA interrupts [ʹ��ѡ���DMA�ж�]*/
    DMAy_Channelx->CCR |= DMA_IT;
  }
  else
  {
    /* Disable the selected DMA interrupts [��ֹѡ���DMA�ж�]*/
    DMAy_Channelx->CCR &= ~DMA_IT;
  }
}

/*******************************************************************************
* ��������: DMA_GetCurrDataCounter
* ��������: ���ص�ǰDMAyͨ��x����ʣ������ݵ�Ԫ.
* �������: DMAy_Channelx��y������1����2��DMA1��x������1��7 ��DMA2��x������1��5��ѡ���ͨ��.
* �������: ��
* ���ز���: ��ǰDMAͨ��������ʣ������ݵ�Ԫ������.
*******************************************************************************/
u16 DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx)
{
  /* Check the parameters [������]*/
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));

  /* Return the number of remaining data units for DMAy Channelx [����DMAy Channelxʣ�����ݵ�Ԫ������]*/
  return ((u16)(DMAy_Channelx->CNDTR));
}

/*******************************************************************************
* ��������: DMA_GetFlagStatus
* ��������: ����ض���DMAyͨ��x�ı�־�Ƿ���λ��
* �������: DMA_FLAG��ָ����Ҫ���ı�־��
*                    ������������������ֵ֮һ:
*                       - DMA1_FLAG_GL1: DMA1ͨ��1ȫ�ֱ�־
*                       - DMA1_FLAG_TC1: DMA1ͨ��1���������־
*                       - DMA1_FLAG_HT1: DMA1ͨ��1�봫���־
*                       - DMA1_FLAG_TE1: DMA1ͨ��1��������־
*                       - DMA1_FLAG_GL2: DMA1 ͨ��2 ȫ�ֱ�־.
*                       - DMA1_FLAG_TC2: DMA1 ͨ��2 ���������־.
*                       - DMA1_FLAG_HT2: DMA1 ͨ��2 �봫���־.
*                       - DMA1_FLAG_TE2: DMA1 ͨ��2 ��������־.
*                       - DMA1_FLAG_GL3: DMA1 ͨ��3 ȫ�ֱ�־.
*                       - DMA1_FLAG_TC3: DMA1 ͨ��3 ���������־.
*                       - DMA1_FLAG_HT3: DMA1 ͨ��3 �봫���־.
*                       - DMA1_FLAG_TE3: DMA1 ͨ��3 ��������־.
*                       - DMA1_FLAG_GL4: DMA1 ͨ��4 ȫ�ֱ�־.
*                       - DMA1_FLAG_TC4: DMA1 ͨ��4 ���������־.
*                       - DMA1_FLAG_HT4: DMA1 ͨ��4 �봫���־.
*                       - DMA1_FLAG_TE4: DMA1 ͨ��4 ��������־.
*                       - DMA1_FLAG_GL5: DMA1 ͨ��5 ȫ�ֱ�־.
*                       - DMA1_FLAG_TC5: DMA1 ͨ��5 ���������־.
*                       - DMA1_FLAG_HT5: DMA1 ͨ��5 �봫���־.
*                       - DMA1_FLAG_TE5: DMA1 ͨ��5 ��������־.
*                       - DMA1_FLAG_GL6: DMA1 ͨ��6 ȫ�ֱ�־.
*                       - DMA1_FLAG_TC6: DMA1 ͨ��6 ���������־.
*                       - DMA1_FLAG_HT6: DMA1 ͨ��6 �봫���־.
*                       - DMA1_FLAG_TE6: DMA1 ͨ��6 ��������־.
*                       - DMA1_FLAG_GL7: DMA1 ͨ��7 ȫ�ֱ�־.
*                       - DMA1_FLAG_TC7: DMA1 ͨ��7 ���������־.
*                       - DMA1_FLAG_HT7: DMA1 ͨ��7 �봫���־.
*                       - DMA1_FLAG_TE7: DMA1 ͨ��7 ��������־.
*                       - DMA2_FLAG_GL1: DMA2 ͨ��1 ȫ�ֱ�־.
*                       - DMA2_FLAG_TC1: DMA2 ͨ��1 ���������־.
*                       - DMA2_FLAG_HT1: DMA2 ͨ��1 �봫���־.
*                       - DMA2_FLAG_TE1: DMA2 ͨ��1 ��������־.
*                       - DMA2_FLAG_GL2: DMA2 ͨ��2 ȫ�ֱ�־.
*                       - DMA2_FLAG_TC2: DMA2 ͨ��2 ���������־.
*                       - DMA2_FLAG_HT2: DMA2 ͨ��2 �봫���־.
*                       - DMA2_FLAG_TE2: DMA2 ͨ��2 ��������־.
*                       - DMA2_FLAG_GL3: DMA2 ͨ��3 ȫ�ֱ�־.
*                       - DMA2_FLAG_TC3: DMA2 ͨ��3 ���������־.
*                       - DMA2_FLAG_HT3: DMA2 ͨ��3 �봫���־.
*                       - DMA2_FLAG_TE3: DMA2 ͨ��3 ��������־.
*                       - DMA2_FLAG_GL4: DMA2 ͨ��4 ȫ�ֱ�־.
*                       - DMA2_FLAG_TC4: DMA2 ͨ��4 ���������־.
*                       - DMA2_FLAG_HT4: DMA2 ͨ��4 �봫���־.
*                       - DMA2_FLAG_TE4: DMA2 ͨ��4 ��������־.
*                       - DMA2_FLAG_GL5: DMA2 ͨ��5 ȫ�ֱ�־.
*                       - DMA2_FLAG_TC5: DMA2 ͨ��5 ���������־.
*                       - DMA2_FLAG_HT5: DMA2 ͨ��5 �봫���־.
*                       - DMA2_FLAG_TE5: DMA2 ͨ��5 ��������־.
* �������: ��
* ���ز���: DMA_FLAG����״̬��SET��RESET��.
*******************************************************************************/
FlagStatus DMA_GetFlagStatus(u32 DMA_FLAG)
{
  FlagStatus bitstatus = RESET;
  u32 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_DMA_GET_FLAG(DMA_FLAG));

  /* Calculate the used DMA [����ʹ�õ�DMA]*/
  if ((DMA_FLAG & FLAG_Mask) != (u32)RESET)
  {
    /* Get DMA2 ISR register value [ȡ��DMA2 ISR�Ĵ���ֵ]*/
    tmpreg = DMA2->ISR ;
  }
  else
  {
    /* Get DMA1 ISR register value [ȡ��DMA1 ISR�Ĵ���ֵ]*/
    tmpreg = DMA1->ISR ;
  }

  /* Check the status of the specified DMA flag [���ָ����DMA��־״̬]*/
  if ((tmpreg & DMA_FLAG) != (u32)RESET)
  {
    /* DMA_FLAG is set [��λDMA_FLAG]*/
    bitstatus = SET;
  }
  else
  {
    /* DMA_FLAG is reset [��λDMA_FLAG]*/
    bitstatus = RESET;
  }
  
  /* Return the DMA_FLAG status [����DMA_FLAG״̬]*/
  return  bitstatus;
}

/*******************************************************************************
* ��������: DMA_ClearFlag
* ��������: ��� DMAy ͨ��x �����־.
* �������: DMA_FLAG����Ҫ����ı�־��ʹ��"|"�����������������־��
*                    �����������������ֵ��������� (ͬһ�� DMA) :
*                       - DMA1_FLAG_GL1: DMA1 ͨ��1 ȫ�ֱ�־.
*                       - DMA1_FLAG_TC1: DMA1 ͨ��1 ���������־.
*                       - DMA1_FLAG_HT1: DMA1 ͨ��1 �봫���־.
*                       - DMA1_FLAG_TE1: DMA1 ͨ��1 ��������־.
*                       - DMA1_FLAG_GL2: DMA1 ͨ��2 ȫ�ֱ�־.
*                       - DMA1_FLAG_TC2: DMA1 ͨ��2 ���������־.
*                       - DMA1_FLAG_HT2: DMA1 ͨ��2 �봫���־.
*                       - DMA1_FLAG_TE2: DMA1 ͨ��2 ��������־.
*                       - DMA1_FLAG_GL3: DMA1 ͨ��3 ȫ�ֱ�־.
*                       - DMA1_FLAG_TC3: DMA1 ͨ��3 ���������־.
*                       - DMA1_FLAG_HT3: DMA1 ͨ��3 �봫���־.
*                       - DMA1_FLAG_TE3: DMA1 ͨ��3 ��������־.
*                       - DMA1_FLAG_GL4: DMA1 ͨ��4 ȫ�ֱ�־.
*                       - DMA1_FLAG_TC4: DMA1 ͨ��4 ���������־.
*                       - DMA1_FLAG_HT4: DMA1 ͨ��4 �봫���־.
*                       - DMA1_FLAG_TE4: DMA1 ͨ��4 ��������־.
*                       - DMA1_FLAG_GL5: DMA1 ͨ��5 ȫ�ֱ�־.
*                       - DMA1_FLAG_TC5: DMA1 ͨ��5 ���������־.
*                       - DMA1_FLAG_HT5: DMA1 ͨ��5 �봫���־.
*                       - DMA1_FLAG_TE5: DMA1 ͨ��5 ��������־.
*                       - DMA1_FLAG_GL6: DMA1 ͨ��6 ȫ�ֱ�־.
*                       - DMA1_FLAG_TC6: DMA1 ͨ��6 ���������־.
*                       - DMA1_FLAG_HT6: DMA1 ͨ��6 �봫���־.
*                       - DMA1_FLAG_TE6: DMA1 ͨ��6 ��������־.
*                       - DMA1_FLAG_GL7: DMA1 ͨ��7 ȫ�ֱ�־.
*                       - DMA1_FLAG_TC7: DMA1 ͨ��7 ���������־.
*                       - DMA1_FLAG_HT7: DMA1 ͨ��7 �봫���־.
*                       - DMA1_FLAG_TE7: DMA1 ͨ��7 ��������־.
*                       - DMA2_FLAG_GL1: DMA2 ͨ��1 ȫ�ֱ�־.
*                       - DMA2_FLAG_TC1: DMA2 ͨ��1 ���������־.
*                       - DMA2_FLAG_HT1: DMA2 ͨ��1 �봫���־.
*                       - DMA2_FLAG_TE1: DMA2 ͨ��1 ��������־.
*                       - DMA2_FLAG_GL2: DMA2 ͨ��2 ȫ�ֱ�־.
*                       - DMA2_FLAG_TC2: DMA2 ͨ��2 ���������־.
*                       - DMA2_FLAG_HT2: DMA2 ͨ��2 �봫���־.
*                       - DMA2_FLAG_TE2: DMA2 ͨ��2 ��������־.
*                       - DMA2_FLAG_GL3: DMA2 ͨ��3 ȫ�ֱ�־.
*                       - DMA2_FLAG_TC3: DMA2 ͨ��3 ���������־.
*                       - DMA2_FLAG_HT3: DMA2 ͨ��3 �봫���־.
*                       - DMA2_FLAG_TE3: DMA2 ͨ��3 ��������־.
*                       - DMA2_FLAG_GL4: DMA2 ͨ��4 ȫ�ֱ�־.
*                       - DMA2_FLAG_TC4: DMA2 ͨ��4 ���������־.
*                       - DMA2_FLAG_HT4: DMA2 ͨ��4 �봫���־.
*                       - DMA2_FLAG_TE4: DMA2 ͨ��4 ��������־.
*                       - DMA2_FLAG_GL5: DMA2 ͨ��5 ȫ�ֱ�־.
*                       - DMA2_FLAG_TC5: DMA2 ͨ��5 ���������־.
*                       - DMA2_FLAG_HT5: DMA2 ͨ��5 �봫���־.
*                       - DMA2_FLAG_TE5: DMA2 ͨ��5 ��������־.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DMA_ClearFlag(u32 DMA_FLAG)
{
  /* Check the parameters [������]*/
  assert_param(IS_DMA_CLEAR_FLAG(DMA_FLAG));

  /* Calculate the used DMA [����ʹ�õ�DMA]*/
  if ((DMA_FLAG & FLAG_Mask) != (u32)RESET)
  {
    /* Clear the selected DMA flags [���ѡ���DMA��־]*/
    DMA2->IFCR = DMA_FLAG;
  }
  else
  {
    /* Clear the selected DMA flags [���ѡ���DMA��־]*/
    DMA1->IFCR = DMA_FLAG;
  }
}

/*******************************************************************************
* ��������: DMA_GetITStatus
* ��������: ����ض���DMAy ͨ��x���ж��Ƿ񱻷�����
* �������: DMA_IT����Ҫ����DMAͨ���ж�Դ��
*                    ������������������ֵ֮һ:
*                       - DMA1_IT_GL1: DMA1 ͨ��1 ȫ���ж�.
*                       - DMA1_IT_TC1: DMA1 ͨ��1 ��������ж�.
*                       - DMA1_IT_HT1: DMA1 ͨ��1 �봫���ж�.
*                       - DMA1_IT_TE1: DMA1 ͨ��1 ��������ж�.
*                       - DMA1_IT_GL2: DMA1 ͨ��2 ȫ���ж�.
*                       - DMA1_IT_TC2: DMA1 ͨ��2 ��������ж�.
*                       - DMA1_IT_HT2: DMA1 ͨ��2 �봫���ж�.
*                       - DMA1_IT_TE2: DMA1 ͨ��2 ��������ж�.
*                       - DMA1_IT_GL3: DMA1 ͨ��3 ȫ���ж�.
*                       - DMA1_IT_TC3: DMA1 ͨ��3 ��������ж�.
*                       - DMA1_IT_HT3: DMA1 ͨ��3 �봫���ж�.
*                       - DMA1_IT_TE3: DMA1 ͨ��3 ��������ж�.
*                       - DMA1_IT_GL4: DMA1 ͨ��4 ȫ���ж�.
*                       - DMA1_IT_TC4: DMA1 ͨ��4 ��������ж�.
*                       - DMA1_IT_HT4: DMA1 ͨ��4 �봫���ж�.
*                       - DMA1_IT_TE4: DMA1 ͨ��4 ��������ж�.
*                       - DMA1_IT_GL5: DMA1 ͨ��5 ȫ���ж�.
*                       - DMA1_IT_TC5: DMA1 ͨ��5 ��������ж�.
*                       - DMA1_IT_HT5: DMA1 ͨ��5 �봫���ж�.
*                       - DMA1_IT_TE5: DMA1 ͨ��5 ��������ж�.
*                       - DMA1_IT_GL6: DMA1 ͨ��6 ȫ���ж�.
*                       - DMA1_IT_TC6: DMA1 ͨ��6 ��������ж�.
*                       - DMA1_IT_HT6: DMA1 ͨ��6 �봫���ж�.
*                       - DMA1_IT_TE6: DMA1 ͨ��6 ��������ж�.
*                       - DMA1_IT_GL7: DMA1 ͨ��7 ȫ���ж�.
*                       - DMA1_IT_TC7: DMA1 ͨ��7 ��������ж�.
*                       - DMA1_IT_HT7: DMA1 ͨ��7 �봫���ж�.
*                       - DMA1_IT_TE7: DMA1 ͨ��7 ��������ж�.
*                       - DMA2_IT_GL1: DMA2 ͨ��1 ȫ���ж�.
*                       - DMA2_IT_TC1: DMA2 ͨ��1 ��������ж�.
*                       - DMA2_IT_HT1: DMA2 ͨ��1 �봫���ж�.
*                       - DMA2_IT_TE1: DMA2 ͨ��1 ��������ж�.
*                       - DMA2_IT_GL2: DMA2 ͨ��2 ȫ���ж�.
*                       - DMA2_IT_TC2: DMA2 ͨ��2 ��������ж�.
*                       - DMA2_IT_HT2: DMA2 ͨ��2 �봫���ж�.
*                       - DMA2_IT_TE2: DMA2 ͨ��2 ��������ж�.
*                       - DMA2_IT_GL3: DMA2 ͨ��3 ȫ���ж�.
*                       - DMA2_IT_TC3: DMA2 ͨ��3 ��������ж�.
*                       - DMA2_IT_HT3: DMA2 ͨ��3 �봫���ж�.
*                       - DMA2_IT_TE3: DMA2 ͨ��3 ��������ж�.
*                       - DMA2_IT_GL4: DMA2 ͨ��4 ȫ���ж�.
*                       - DMA2_IT_TC4: DMA2 ͨ��4 ��������ж�.
*                       - DMA2_IT_HT4: DMA2 ͨ��4 �봫���ж�.
*                       - DMA2_IT_TE4: DMA2 ͨ��4 ��������ж�.
*                       - DMA2_IT_GL5: DMA2 ͨ��5 ȫ���ж�.
*                       - DMA2_IT_TC5: DMA2 ͨ��5 ��������ж�.
*                       - DMA2_IT_HT5: DMA2 ͨ��5 �봫���ж�.
*                       - DMA2_IT_TE5: DMA2 ͨ��5 ��������ж�.
* �������: ��
* ���ز���: DMA_IT����״̬��SET��RESET��.
*******************************************************************************/
ITStatus DMA_GetITStatus(u32 DMA_IT)
{
  ITStatus bitstatus = RESET;
  u32 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_DMA_GET_IT(DMA_IT));

  /* Calculate the used DMA [����ʹ�õ�DMA]*/
  if ((DMA_IT & FLAG_Mask) != (u32)RESET)
  {
    /* Get DMA2 ISR register value [ȡ��DMA2 ISR�Ĵ���ֵ]*/
    tmpreg = DMA2->ISR ;
  }
  else
  {
    /* Get DMA1 ISR register value [ȡ��DMA1 ISR�Ĵ���ֵ]*/
    tmpreg = DMA1->ISR ;
  }

  /* Check the status of the specified DMA interrupt [���ָ����DMA�ж�״̬]*/
  if ((tmpreg & DMA_IT) != (u32)RESET)
  {
    /* DMA_IT is set [��λDMA_IT]*/
    bitstatus = SET;
  }
  else
  {
    /* DMA_IT is reset [��λDMA_IT]*/
    bitstatus = RESET;
  }
  /* Return the DMA_IT status [����DMA_IT��״̬]*/
  return  bitstatus;
}

/*******************************************************************************
* ��������: DMA_ClearITPendingBit
* ��������: ��� DMAy ͨ��x�жϹ���λ.
* �������: DMA_IT����Ҫ�����DMAͨ���Ĺ���λ��ʹ��"|"����������������жϡ�
*                     �����������������ֵ��������� (ͬһ�� DMA) :
*                       - DMA1_IT_GL1: DMA1 ͨ��1 ȫ���ж�.
*                       - DMA1_IT_TC1: DMA1 ͨ��1 ��������ж�.
*                       - DMA1_IT_HT1: DMA1 ͨ��1 �봫���ж�.
*                       - DMA1_IT_TE1: DMA1 ͨ��1 ��������ж�.
*                       - DMA1_IT_GL2: DMA1 ͨ��2 ȫ���ж�.
*                       - DMA1_IT_TC2: DMA1 ͨ��2 ��������ж�.
*                       - DMA1_IT_HT2: DMA1 ͨ��2 �봫���ж�.
*                       - DMA1_IT_TE2: DMA1 ͨ��2 ��������ж�.
*                       - DMA1_IT_GL3: DMA1 ͨ��3 ȫ���ж�.
*                       - DMA1_IT_TC3: DMA1 ͨ��3 ��������ж�.
*                       - DMA1_IT_HT3: DMA1 ͨ��3 �봫���ж�.
*                       - DMA1_IT_TE3: DMA1 ͨ��3 ��������ж�.
*                       - DMA1_IT_GL4: DMA1 ͨ��4 ȫ���ж�.
*                       - DMA1_IT_TC4: DMA1 ͨ��4 ��������ж�.
*                       - DMA1_IT_HT4: DMA1 ͨ��4 �봫���ж�.
*                       - DMA1_IT_TE4: DMA1 ͨ��4 ��������ж�.
*                       - DMA1_IT_GL5: DMA1 ͨ��5 ȫ���ж�.
*                       - DMA1_IT_TC5: DMA1 ͨ��5 ��������ж�.
*                       - DMA1_IT_HT5: DMA1 ͨ��5 �봫���ж�.
*                       - DMA1_IT_TE5: DMA1 ͨ��5 ��������ж�.
*                       - DMA1_IT_GL6: DMA1 ͨ��6 ȫ���ж�.
*                       - DMA1_IT_TC6: DMA1 ͨ��6 ��������ж�.
*                       - DMA1_IT_HT6: DMA1 ͨ��6 �봫���ж�.
*                       - DMA1_IT_TE6: DMA1 ͨ��6 ��������ж�.
*                       - DMA1_IT_GL7: DMA1 ͨ��7 ȫ���ж�.
*                       - DMA1_IT_TC7: DMA1 ͨ��7 ��������ж�.
*                       - DMA1_IT_HT7: DMA1 ͨ��7 �봫���ж�.
*                       - DMA1_IT_TE7: DMA1 ͨ��7 ��������ж�.
*                       - DMA2_IT_GL1: DMA2 ͨ��1 ȫ���ж�.
*                       - DMA2_IT_TC1: DMA2 ͨ��1 ��������ж�.
*                       - DMA2_IT_HT1: DMA2 ͨ��1 �봫���ж�.
*                       - DMA2_IT_TE1: DMA2 ͨ��1 ��������ж�.
*                       - DMA2_IT_GL2: DMA2 ͨ��2 ȫ���ж�.
*                       - DMA2_IT_TC2: DMA2 ͨ��2 ��������ж�.
*                       - DMA2_IT_HT2: DMA2 ͨ��2 �봫���ж�.
*                       - DMA2_IT_TE2: DMA2 ͨ��2 ��������ж�.
*                       - DMA2_IT_GL3: DMA2 ͨ��3 ȫ���ж�.
*                       - DMA2_IT_TC3: DMA2 ͨ��3 ��������ж�.
*                       - DMA2_IT_HT3: DMA2 ͨ��3 �봫���ж�.
*                       - DMA2_IT_TE3: DMA2 ͨ��3 ��������ж�.
*                       - DMA2_IT_GL4: DMA2 ͨ��4 ȫ���ж�.
*                       - DMA2_IT_TC4: DMA2 ͨ��4 ��������ж�.
*                       - DMA2_IT_HT4: DMA2 ͨ��4 �봫���ж�.
*                       - DMA2_IT_TE4: DMA2 ͨ��4 ��������ж�.
*                       - DMA2_IT_GL5: DMA2 ͨ��5 ȫ���ж�.
*                       - DMA2_IT_TC5: DMA2 ͨ��5 ��������ж�.
*                       - DMA2_IT_HT5: DMA2 ͨ��5 �봫���ж�.
*                       - DMA2_IT_TE5: DMA2 ͨ��5 ��������ж�.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DMA_ClearITPendingBit(u32 DMA_IT)
{
  /* Check the parameters [������]*/
  assert_param(IS_DMA_CLEAR_IT(DMA_IT));

  /* Calculate the used DMA [����ʹ�õ�DMA]*/
  if ((DMA_IT & FLAG_Mask) != (u32)RESET)
  {
    /* Clear the selected DMA interrupt pending bits [���ѡ���DMA�жϹ���λ]*/
    DMA2->IFCR = DMA_IT;
  }
  else
  {
    /* Clear the selected DMA interrupt pending bits [���ѡ���DMA�жϹ���λ]*/
    DMA1->IFCR = DMA_IT;
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

