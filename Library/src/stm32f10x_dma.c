/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_dma.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有DMA的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
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

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: DMA_DeInit
* 功能描述: 将DMA各通道寄存器复位为默认的复位值.
* 输入参数: DMAy_Channelx：y可以是1或者2，DMA1的x可以是1到7 和DMA2的x可以是1到5来选择各通道
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DMA_DeInit(DMA_Channel_TypeDef* DMAy_Channelx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));

  /* Disable the selected DMAy Channelx [禁止选择的DMAy 通道x]*/
  DMAy_Channelx->CCR &= CCR_ENABLE_Reset;

  /* Reset DMAy Channelx control register [复位DMAy Channelx的控制寄存器]*/
  DMAy_Channelx->CCR  = 0;
  
  /* Reset DMAy Channelx remaining bytes register [复位DMAy Channelx的剩余字节寄存器]*/
  DMAy_Channelx->CNDTR = 0;
  
  /* Reset DMAy Channelx peripheral address register [复位DMAy Channelx外围设备地址寄存器]*/
  DMAy_Channelx->CPAR  = 0;
  
  /* Reset DMAy Channelx memory address register [复位DMAy Channelx内存地址寄存器]*/
  DMAy_Channelx->CMAR = 0;

  switch (*(u32*)&DMAy_Channelx)
  {
    case DMA1_Channel1_BASE:
      /* Reset interrupt pending bits for DMA1 Channel1 [复位DMA1 Channel1中断挂起位]*/
      DMA1->IFCR |= DMA1_Channel1_IT_Mask;
      break;

    case DMA1_Channel2_BASE:
      /* Reset interrupt pending bits for DMA1 Channel2 [复位DMA1 Channel2中断挂起位]*/
      DMA1->IFCR |= DMA1_Channel2_IT_Mask;
      break;

    case DMA1_Channel3_BASE:
      /* Reset interrupt pending bits for DMA1 Channel3 [复位DMA1 Channel3中断挂起位]*/
      DMA1->IFCR |= DMA1_Channel3_IT_Mask;
      break;

    case DMA1_Channel4_BASE:
      /* Reset interrupt pending bits for DMA1 Channel4 [复位DMA1 Channel4中断挂起位]*/
      DMA1->IFCR |= DMA1_Channel4_IT_Mask;
      break;

    case DMA1_Channel5_BASE:
      /* Reset interrupt pending bits for DMA1 Channel5 [复位DMA1 Channel5中断挂起位]*/
      DMA1->IFCR |= DMA1_Channel5_IT_Mask;
      break;

    case DMA1_Channel6_BASE:
      /* Reset interrupt pending bits for DMA1 Channel6 [复位DMA1 Channel6中断挂起位]*/
      DMA1->IFCR |= DMA1_Channel6_IT_Mask;
      break;

    case DMA1_Channel7_BASE:
      /* Reset interrupt pending bits for DMA1 Channel7 [复位DMA1 Channel7中断挂起位]*/
      DMA1->IFCR |= DMA1_Channel7_IT_Mask;
      break;

    case DMA2_Channel1_BASE:
      /* Reset interrupt pending bits for DMA2 Channel1 [复位DMA2 Channel1中断挂起位]*/
      DMA2->IFCR |= DMA2_Channel1_IT_Mask;
      break;

    case DMA2_Channel2_BASE:
      /* Reset interrupt pending bits for DMA2 Channel2 [复位DMA2 Channel2中断挂起位]*/
      DMA2->IFCR |= DMA2_Channel2_IT_Mask;
      break;

    case DMA2_Channel3_BASE:
      /* Reset interrupt pending bits for DMA2 Channel3 [复位DMA2 Channel3中断挂起位]*/
      DMA2->IFCR |= DMA2_Channel3_IT_Mask;
      break;

    case DMA2_Channel4_BASE:
      /* Reset interrupt pending bits for DMA2 Channel4 [复位DMA2 Channel4中断挂起位]*/
      DMA2->IFCR |= DMA2_Channel4_IT_Mask;
      break;

    case DMA2_Channel5_BASE:
      /* Reset interrupt pending bits for DMA2 Channel5 [复位DMA2 Channel5中断挂起位]*/
      DMA2->IFCR |= DMA2_Channel5_IT_Mask;
      break;
      
    default:
      break;
  }
}

/*******************************************************************************
* 函数名称: DMA_Init
* 功能描述: 根据DMA_InitStruct指定的参数初始化DMA各通道.
* 输入参数: (1)DMAy_Channelx：y可以是1或者2，DMA1的x可以是1到7 和DMA2的x可以是1到5来选择各通道.
*           (2)DMA_InitStruct：指向DMA_InitTypeDef，它包含特定DMA频道的配置信息。
* 输出参数: 无
* 返回参数: 无
******************************************************************************/
void DMA_Init(DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct)
{
  u32 tmpreg = 0;

  /* Check the parameters [检查参数]*/
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

/*--------------------------- DMAy Channelx CCR Configuration [DMAy 通道x CCR配置]-----------------*/
  /* Get the DMAy_Channelx CCR value [取得DMAy_Channelx CCR的值]*/
  tmpreg = DMAy_Channelx->CCR;
  /* Clear MEM2MEM, PL, MSIZE, PSIZE, MINC, PINC, CIRC and DIR bits [清MEM2MEM, PL, MSIZE, PSIZE, MINC, PINC, CIRC 和 DIR位]*/
  tmpreg &= CCR_CLEAR_Mask;
  /* Configure DMAy Channelx: data transfer, data size, priority level and mode [配置DMAy 通道x:数据传输，数据类型，优先级和模式]*/
  /* Set DIR bit according to DMA_DIR value [依照DMA_DIR的值设置DIR位]*/
  /* Set CIRC bit according to DMA_Mode value [依照DMA_Mode的值设置CIRC位]*/
  /* Set PINC bit according to DMA_PeripheralInc value [依照DMA_PeripheralInc的值设置PINC位]*/
  /* Set MINC bit according to DMA_MemoryInc value [依照DMA_MemoryInc的值设置MINC位]*/
  /* Set PSIZE bits according to DMA_PeripheralDataSize value [依照DMA_PeripheralDataSize的值设置PSIZE位]*/
  /* Set MSIZE bits according to DMA_MemoryDataSize value [依照DMA_MemoryDataSize的值设置MSIZE位]*/
  /* Set PL bits according to DMA_Priority value [依照DMA_Priority的值设置PL位]*/
  /* Set the MEM2MEM bit according to DMA_M2M value [依照DMA_M2M的值设置MEM2MEM位]*/
  tmpreg |= DMA_InitStruct->DMA_DIR | DMA_InitStruct->DMA_Mode |
            DMA_InitStruct->DMA_PeripheralInc | DMA_InitStruct->DMA_MemoryInc |
            DMA_InitStruct->DMA_PeripheralDataSize | DMA_InitStruct->DMA_MemoryDataSize |
            DMA_InitStruct->DMA_Priority | DMA_InitStruct->DMA_M2M;
  /* Write to DMAy Channelx CCR [写DMAy通道x CCR]*/
  DMAy_Channelx->CCR = tmpreg;

/*--------------------------- DMAy Channelx CNDTR Configuration [DMAy 通道x CNDTR配置]---------------*/
  /* Write to DMAy Channelx CNDTR [写DMAy通道x CNDTR]*/
  DMAy_Channelx->CNDTR = DMA_InitStruct->DMA_BufferSize;

/*--------------------------- DMAy Channelx CPAR Configuration [DMAy 通道x CPAR配置]----------------*/
  /* Write to DMAy Channelx CPAR [写DMAy通道x CPAR]*/
  DMAy_Channelx->CPAR = DMA_InitStruct->DMA_PeripheralBaseAddr;

/*--------------------------- DMAy Channelx CMAR Configuration [DMAy 通道x CMAR配置]----------------*/
  /* Write to DMAy Channelx CMAR [写DMAy通道x CMAR]*/
  DMAy_Channelx->CMAR = DMA_InitStruct->DMA_MemoryBaseAddr;
}

/*******************************************************************************
* 函数名称: DMA_StructInit
* 功能描述: 将DMA_InitStruct的每个成员复位为默认值.
* 输入参数: DMA_InitStruc：指向需要初始化的DMA_InitTypeDef结构。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct)
{
/*-------------- Reset DMA init structure parameters values ------------------*/
  /* Initialize the DMA_PeripheralBaseAddr member [初始化DMA_PeripheralBaseAddr成员]*/
  DMA_InitStruct->DMA_PeripheralBaseAddr = 0;

  /* Initialize the DMA_MemoryBaseAddr member [初始化DMA_MemoryBaseAddr成员]*/
  DMA_InitStruct->DMA_MemoryBaseAddr = 0;

  /* Initialize the DMA_DIR member [初始化DMA_DIR成员]*/
  DMA_InitStruct->DMA_DIR = DMA_DIR_PeripheralSRC;

  /* Initialize the DMA_BufferSize member[初始化DMA_BufferSize成员] */
  DMA_InitStruct->DMA_BufferSize = 0;

  /* Initialize the DMA_PeripheralInc member [初始化DMA_PeripheralInc成员]*/
  DMA_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Disable;

  /* Initialize the DMA_MemoryInc member [初始化DMA_MemoryInc成员]*/
  DMA_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Disable;

  /* Initialize the DMA_PeripheralDataSize member [初始化DMA_PeripheralDataSize成员]*/
  DMA_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

  /* Initialize the DMA_MemoryDataSize member [初始化DMA_MemoryDataSize成员]*/
  DMA_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

  /* Initialize the DMA_Mode member [初始化DMA_DMA_Mode成员]*/
  DMA_InitStruct->DMA_Mode = DMA_Mode_Normal;

  /* Initialize the DMA_Priority member [初始化DMA_Priority成员]*/
  DMA_InitStruct->DMA_Priority = DMA_Priority_Low;

  /* Initialize the DMA_M2M member [初始化DMA_M2M成员]*/
  DMA_InitStruct->DMA_M2M = DMA_M2M_Disable;
}

/*******************************************************************************
* 函数名称: DMA_Cmd
* 功能描述: 使能（或禁能）特定的DMAy通道x .
* 输入参数: (1)DMAy_Channelx：y可以是1或者2，DMA1的x可以是1到7 和DMA2的x可以是1到5来选择各通道.
*           (2)NewState：DMA通道的新状态。该参数可为：ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DMA_Cmd(DMA_Channel_TypeDef* DMAy_Channelx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected DMAy Channelx [使能选择的DMAy通道x]*/
    DMAy_Channelx->CCR |= CCR_ENABLE_Set;
  }
  else
  {
    /* Disable the selected DMAy Channelx [禁止选择的DMAy通道x]*/
    DMAy_Channelx->CCR &= CCR_ENABLE_Reset;
  }
}

/*******************************************************************************
* 函数名称: DMA_ITConfig
* 功能描述: 使能（或禁能）特定的DMAy通道x的中断.
* 输入参数: (1)DMAy_Channelx：y可以是1或者2，DMA1的x可以是1到7 和DMA2的x可以是1到5来选择各通道.
*           (2)DMA_IT: 确定DMA通道的中断源是否被使能。 
*                    这个参数可以是下面值的任意组合:
*                       - DMA_IT_TC:  传输结束中断屏蔽 mask
*                       - DMA_IT_HT:  半传输中断屏蔽 mask
*                       - DMA_IT_TE:  传输错误中断屏蔽 mask
*           (3)NewState：指定的DMA通道中断的新状态。该参数可为：ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DMA_ITConfig(DMA_Channel_TypeDef* DMAy_Channelx, u32 DMA_IT, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_DMA_CONFIG_IT(DMA_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected DMA interrupts [使能选择的DMA中断]*/
    DMAy_Channelx->CCR |= DMA_IT;
  }
  else
  {
    /* Disable the selected DMA interrupts [禁止选择的DMA中断]*/
    DMAy_Channelx->CCR &= ~DMA_IT;
  }
}

/*******************************************************************************
* 函数名称: DMA_GetCurrDataCounter
* 功能描述: 返回当前DMAy通道x传输剩余的数据单元.
* 输入参数: DMAy_Channelx：y可以是1或者2，DMA1的x可以是1到7 和DMA2的x可以是1到5来选择各通道.
* 输出参数: 无
* 返回参数: 当前DMA通道传输中剩余的数据单元的数量.
*******************************************************************************/
u16 DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));

  /* Return the number of remaining data units for DMAy Channelx [返回DMAy Channelx剩余数据单元的数字]*/
  return ((u16)(DMAy_Channelx->CNDTR));
}

/*******************************************************************************
* 函数名称: DMA_GetFlagStatus
* 功能描述: 检查特定的DMAy通道x的标志是否被置位了
* 输入参数: DMA_FLAG：指定需要检查的标志。
*                    这个参数可以是下面的值之一:
*                       - DMA1_FLAG_GL1: DMA1通道1全局标志
*                       - DMA1_FLAG_TC1: DMA1通道1传输结束标志
*                       - DMA1_FLAG_HT1: DMA1通道1半传输标志
*                       - DMA1_FLAG_TE1: DMA1通道1传输错误标志
*                       - DMA1_FLAG_GL2: DMA1 通道2 全局标志.
*                       - DMA1_FLAG_TC2: DMA1 通道2 传输结束标志.
*                       - DMA1_FLAG_HT2: DMA1 通道2 半传输标志.
*                       - DMA1_FLAG_TE2: DMA1 通道2 传输错误标志.
*                       - DMA1_FLAG_GL3: DMA1 通道3 全局标志.
*                       - DMA1_FLAG_TC3: DMA1 通道3 传输结束标志.
*                       - DMA1_FLAG_HT3: DMA1 通道3 半传输标志.
*                       - DMA1_FLAG_TE3: DMA1 通道3 传输错误标志.
*                       - DMA1_FLAG_GL4: DMA1 通道4 全局标志.
*                       - DMA1_FLAG_TC4: DMA1 通道4 传输结束标志.
*                       - DMA1_FLAG_HT4: DMA1 通道4 半传输标志.
*                       - DMA1_FLAG_TE4: DMA1 通道4 传输错误标志.
*                       - DMA1_FLAG_GL5: DMA1 通道5 全局标志.
*                       - DMA1_FLAG_TC5: DMA1 通道5 传输结束标志.
*                       - DMA1_FLAG_HT5: DMA1 通道5 半传输标志.
*                       - DMA1_FLAG_TE5: DMA1 通道5 传输错误标志.
*                       - DMA1_FLAG_GL6: DMA1 通道6 全局标志.
*                       - DMA1_FLAG_TC6: DMA1 通道6 传输结束标志.
*                       - DMA1_FLAG_HT6: DMA1 通道6 半传输标志.
*                       - DMA1_FLAG_TE6: DMA1 通道6 传输错误标志.
*                       - DMA1_FLAG_GL7: DMA1 通道7 全局标志.
*                       - DMA1_FLAG_TC7: DMA1 通道7 传输结束标志.
*                       - DMA1_FLAG_HT7: DMA1 通道7 半传输标志.
*                       - DMA1_FLAG_TE7: DMA1 通道7 传输错误标志.
*                       - DMA2_FLAG_GL1: DMA2 通道1 全局标志.
*                       - DMA2_FLAG_TC1: DMA2 通道1 传输结束标志.
*                       - DMA2_FLAG_HT1: DMA2 通道1 半传输标志.
*                       - DMA2_FLAG_TE1: DMA2 通道1 传输错误标志.
*                       - DMA2_FLAG_GL2: DMA2 通道2 全局标志.
*                       - DMA2_FLAG_TC2: DMA2 通道2 传输结束标志.
*                       - DMA2_FLAG_HT2: DMA2 通道2 半传输标志.
*                       - DMA2_FLAG_TE2: DMA2 通道2 传输错误标志.
*                       - DMA2_FLAG_GL3: DMA2 通道3 全局标志.
*                       - DMA2_FLAG_TC3: DMA2 通道3 传输结束标志.
*                       - DMA2_FLAG_HT3: DMA2 通道3 半传输标志.
*                       - DMA2_FLAG_TE3: DMA2 通道3 传输错误标志.
*                       - DMA2_FLAG_GL4: DMA2 通道4 全局标志.
*                       - DMA2_FLAG_TC4: DMA2 通道4 传输结束标志.
*                       - DMA2_FLAG_HT4: DMA2 通道4 半传输标志.
*                       - DMA2_FLAG_TE4: DMA2 通道4 传输错误标志.
*                       - DMA2_FLAG_GL5: DMA2 通道5 全局标志.
*                       - DMA2_FLAG_TC5: DMA2 通道5 传输结束标志.
*                       - DMA2_FLAG_HT5: DMA2 通道5 半传输标志.
*                       - DMA2_FLAG_TE5: DMA2 通道5 传输错误标志.
* 输出参数: 无
* 返回参数: DMA_FLAG的新状态（SET或RESET）.
*******************************************************************************/
FlagStatus DMA_GetFlagStatus(u32 DMA_FLAG)
{
  FlagStatus bitstatus = RESET;
  u32 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_DMA_GET_FLAG(DMA_FLAG));

  /* Calculate the used DMA [计算使用的DMA]*/
  if ((DMA_FLAG & FLAG_Mask) != (u32)RESET)
  {
    /* Get DMA2 ISR register value [取得DMA2 ISR寄存器值]*/
    tmpreg = DMA2->ISR ;
  }
  else
  {
    /* Get DMA1 ISR register value [取得DMA1 ISR寄存器值]*/
    tmpreg = DMA1->ISR ;
  }

  /* Check the status of the specified DMA flag [检查指定的DMA标志状态]*/
  if ((tmpreg & DMA_FLAG) != (u32)RESET)
  {
    /* DMA_FLAG is set [置位DMA_FLAG]*/
    bitstatus = SET;
  }
  else
  {
    /* DMA_FLAG is reset [复位DMA_FLAG]*/
    bitstatus = RESET;
  }
  
  /* Return the DMA_FLAG status [返回DMA_FLAG状态]*/
  return  bitstatus;
}

/*******************************************************************************
* 函数名称: DMA_ClearFlag
* 功能描述: 清除 DMAy 通道x 挂起标志.
* 输入参数: DMA_FLAG：需要清除的标志。使用"|"运算符可以清除多个标志。
*                    这个参数可以是下面值的任意组合 (同一个 DMA) :
*                       - DMA1_FLAG_GL1: DMA1 通道1 全局标志.
*                       - DMA1_FLAG_TC1: DMA1 通道1 传输结束标志.
*                       - DMA1_FLAG_HT1: DMA1 通道1 半传输标志.
*                       - DMA1_FLAG_TE1: DMA1 通道1 传输错误标志.
*                       - DMA1_FLAG_GL2: DMA1 通道2 全局标志.
*                       - DMA1_FLAG_TC2: DMA1 通道2 传输结束标志.
*                       - DMA1_FLAG_HT2: DMA1 通道2 半传输标志.
*                       - DMA1_FLAG_TE2: DMA1 通道2 传输错误标志.
*                       - DMA1_FLAG_GL3: DMA1 通道3 全局标志.
*                       - DMA1_FLAG_TC3: DMA1 通道3 传输结束标志.
*                       - DMA1_FLAG_HT3: DMA1 通道3 半传输标志.
*                       - DMA1_FLAG_TE3: DMA1 通道3 传输错误标志.
*                       - DMA1_FLAG_GL4: DMA1 通道4 全局标志.
*                       - DMA1_FLAG_TC4: DMA1 通道4 传输结束标志.
*                       - DMA1_FLAG_HT4: DMA1 通道4 半传输标志.
*                       - DMA1_FLAG_TE4: DMA1 通道4 传输错误标志.
*                       - DMA1_FLAG_GL5: DMA1 通道5 全局标志.
*                       - DMA1_FLAG_TC5: DMA1 通道5 传输结束标志.
*                       - DMA1_FLAG_HT5: DMA1 通道5 半传输标志.
*                       - DMA1_FLAG_TE5: DMA1 通道5 传输错误标志.
*                       - DMA1_FLAG_GL6: DMA1 通道6 全局标志.
*                       - DMA1_FLAG_TC6: DMA1 通道6 传输结束标志.
*                       - DMA1_FLAG_HT6: DMA1 通道6 半传输标志.
*                       - DMA1_FLAG_TE6: DMA1 通道6 传输错误标志.
*                       - DMA1_FLAG_GL7: DMA1 通道7 全局标志.
*                       - DMA1_FLAG_TC7: DMA1 通道7 传输结束标志.
*                       - DMA1_FLAG_HT7: DMA1 通道7 半传输标志.
*                       - DMA1_FLAG_TE7: DMA1 通道7 传输错误标志.
*                       - DMA2_FLAG_GL1: DMA2 通道1 全局标志.
*                       - DMA2_FLAG_TC1: DMA2 通道1 传输结束标志.
*                       - DMA2_FLAG_HT1: DMA2 通道1 半传输标志.
*                       - DMA2_FLAG_TE1: DMA2 通道1 传输错误标志.
*                       - DMA2_FLAG_GL2: DMA2 通道2 全局标志.
*                       - DMA2_FLAG_TC2: DMA2 通道2 传输结束标志.
*                       - DMA2_FLAG_HT2: DMA2 通道2 半传输标志.
*                       - DMA2_FLAG_TE2: DMA2 通道2 传输错误标志.
*                       - DMA2_FLAG_GL3: DMA2 通道3 全局标志.
*                       - DMA2_FLAG_TC3: DMA2 通道3 传输结束标志.
*                       - DMA2_FLAG_HT3: DMA2 通道3 半传输标志.
*                       - DMA2_FLAG_TE3: DMA2 通道3 传输错误标志.
*                       - DMA2_FLAG_GL4: DMA2 通道4 全局标志.
*                       - DMA2_FLAG_TC4: DMA2 通道4 传输结束标志.
*                       - DMA2_FLAG_HT4: DMA2 通道4 半传输标志.
*                       - DMA2_FLAG_TE4: DMA2 通道4 传输错误标志.
*                       - DMA2_FLAG_GL5: DMA2 通道5 全局标志.
*                       - DMA2_FLAG_TC5: DMA2 通道5 传输结束标志.
*                       - DMA2_FLAG_HT5: DMA2 通道5 半传输标志.
*                       - DMA2_FLAG_TE5: DMA2 通道5 传输错误标志.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DMA_ClearFlag(u32 DMA_FLAG)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_DMA_CLEAR_FLAG(DMA_FLAG));

  /* Calculate the used DMA [计算使用的DMA]*/
  if ((DMA_FLAG & FLAG_Mask) != (u32)RESET)
  {
    /* Clear the selected DMA flags [清除选择的DMA标志]*/
    DMA2->IFCR = DMA_FLAG;
  }
  else
  {
    /* Clear the selected DMA flags [清除选择的DMA标志]*/
    DMA1->IFCR = DMA_FLAG;
  }
}

/*******************************************************************************
* 函数名称: DMA_GetITStatus
* 功能描述: 检查特定的DMAy 通道x的中断是否被发生。
* 输入参数: DMA_IT：需要检查的DMA通道中断源。
*                    这个参数可以是下面的值之一:
*                       - DMA1_IT_GL1: DMA1 通道1 全局中断.
*                       - DMA1_IT_TC1: DMA1 通道1 传输完成中断.
*                       - DMA1_IT_HT1: DMA1 通道1 半传输中断.
*                       - DMA1_IT_TE1: DMA1 通道1 传输错误中断.
*                       - DMA1_IT_GL2: DMA1 通道2 全局中断.
*                       - DMA1_IT_TC2: DMA1 通道2 传输完成中断.
*                       - DMA1_IT_HT2: DMA1 通道2 半传输中断.
*                       - DMA1_IT_TE2: DMA1 通道2 传输错误中断.
*                       - DMA1_IT_GL3: DMA1 通道3 全局中断.
*                       - DMA1_IT_TC3: DMA1 通道3 传输完成中断.
*                       - DMA1_IT_HT3: DMA1 通道3 半传输中断.
*                       - DMA1_IT_TE3: DMA1 通道3 传输错误中断.
*                       - DMA1_IT_GL4: DMA1 通道4 全局中断.
*                       - DMA1_IT_TC4: DMA1 通道4 传输完成中断.
*                       - DMA1_IT_HT4: DMA1 通道4 半传输中断.
*                       - DMA1_IT_TE4: DMA1 通道4 传输错误中断.
*                       - DMA1_IT_GL5: DMA1 通道5 全局中断.
*                       - DMA1_IT_TC5: DMA1 通道5 传输完成中断.
*                       - DMA1_IT_HT5: DMA1 通道5 半传输中断.
*                       - DMA1_IT_TE5: DMA1 通道5 传输错误中断.
*                       - DMA1_IT_GL6: DMA1 通道6 全局中断.
*                       - DMA1_IT_TC6: DMA1 通道6 传输完成中断.
*                       - DMA1_IT_HT6: DMA1 通道6 半传输中断.
*                       - DMA1_IT_TE6: DMA1 通道6 传输错误中断.
*                       - DMA1_IT_GL7: DMA1 通道7 全局中断.
*                       - DMA1_IT_TC7: DMA1 通道7 传输完成中断.
*                       - DMA1_IT_HT7: DMA1 通道7 半传输中断.
*                       - DMA1_IT_TE7: DMA1 通道7 传输错误中断.
*                       - DMA2_IT_GL1: DMA2 通道1 全局中断.
*                       - DMA2_IT_TC1: DMA2 通道1 传输完成中断.
*                       - DMA2_IT_HT1: DMA2 通道1 半传输中断.
*                       - DMA2_IT_TE1: DMA2 通道1 传输错误中断.
*                       - DMA2_IT_GL2: DMA2 通道2 全局中断.
*                       - DMA2_IT_TC2: DMA2 通道2 传输完成中断.
*                       - DMA2_IT_HT2: DMA2 通道2 半传输中断.
*                       - DMA2_IT_TE2: DMA2 通道2 传输错误中断.
*                       - DMA2_IT_GL3: DMA2 通道3 全局中断.
*                       - DMA2_IT_TC3: DMA2 通道3 传输完成中断.
*                       - DMA2_IT_HT3: DMA2 通道3 半传输中断.
*                       - DMA2_IT_TE3: DMA2 通道3 传输错误中断.
*                       - DMA2_IT_GL4: DMA2 通道4 全局中断.
*                       - DMA2_IT_TC4: DMA2 通道4 传输完成中断.
*                       - DMA2_IT_HT4: DMA2 通道4 半传输中断.
*                       - DMA2_IT_TE4: DMA2 通道4 传输错误中断.
*                       - DMA2_IT_GL5: DMA2 通道5 全局中断.
*                       - DMA2_IT_TC5: DMA2 通道5 传输完成中断.
*                       - DMA2_IT_HT5: DMA2 通道5 半传输中断.
*                       - DMA2_IT_TE5: DMA2 通道5 传输错误中断.
* 输出参数: 无
* 返回参数: DMA_IT的新状态（SET或RESET）.
*******************************************************************************/
ITStatus DMA_GetITStatus(u32 DMA_IT)
{
  ITStatus bitstatus = RESET;
  u32 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_DMA_GET_IT(DMA_IT));

  /* Calculate the used DMA [计算使用的DMA]*/
  if ((DMA_IT & FLAG_Mask) != (u32)RESET)
  {
    /* Get DMA2 ISR register value [取得DMA2 ISR寄存器值]*/
    tmpreg = DMA2->ISR ;
  }
  else
  {
    /* Get DMA1 ISR register value [取得DMA1 ISR寄存器值]*/
    tmpreg = DMA1->ISR ;
  }

  /* Check the status of the specified DMA interrupt [检查指定的DMA中断状态]*/
  if ((tmpreg & DMA_IT) != (u32)RESET)
  {
    /* DMA_IT is set [置位DMA_IT]*/
    bitstatus = SET;
  }
  else
  {
    /* DMA_IT is reset [复位DMA_IT]*/
    bitstatus = RESET;
  }
  /* Return the DMA_IT status [返回DMA_IT的状态]*/
  return  bitstatus;
}

/*******************************************************************************
* 函数名称: DMA_ClearITPendingBit
* 功能描述: 清除 DMAy 通道x中断挂起位.
* 输入参数: DMA_IT：需要清除的DMA通道的挂起位。使用"|"运算符可以清除多个中断。
*                     这个参数可以是下面值的任意组合 (同一个 DMA) :
*                       - DMA1_IT_GL1: DMA1 通道1 全局中断.
*                       - DMA1_IT_TC1: DMA1 通道1 传输完成中断.
*                       - DMA1_IT_HT1: DMA1 通道1 半传输中断.
*                       - DMA1_IT_TE1: DMA1 通道1 传输错误中断.
*                       - DMA1_IT_GL2: DMA1 通道2 全局中断.
*                       - DMA1_IT_TC2: DMA1 通道2 传输完成中断.
*                       - DMA1_IT_HT2: DMA1 通道2 半传输中断.
*                       - DMA1_IT_TE2: DMA1 通道2 传输错误中断.
*                       - DMA1_IT_GL3: DMA1 通道3 全局中断.
*                       - DMA1_IT_TC3: DMA1 通道3 传输完成中断.
*                       - DMA1_IT_HT3: DMA1 通道3 半传输中断.
*                       - DMA1_IT_TE3: DMA1 通道3 传输错误中断.
*                       - DMA1_IT_GL4: DMA1 通道4 全局中断.
*                       - DMA1_IT_TC4: DMA1 通道4 传输完成中断.
*                       - DMA1_IT_HT4: DMA1 通道4 半传输中断.
*                       - DMA1_IT_TE4: DMA1 通道4 传输错误中断.
*                       - DMA1_IT_GL5: DMA1 通道5 全局中断.
*                       - DMA1_IT_TC5: DMA1 通道5 传输完成中断.
*                       - DMA1_IT_HT5: DMA1 通道5 半传输中断.
*                       - DMA1_IT_TE5: DMA1 通道5 传输错误中断.
*                       - DMA1_IT_GL6: DMA1 通道6 全局中断.
*                       - DMA1_IT_TC6: DMA1 通道6 传输完成中断.
*                       - DMA1_IT_HT6: DMA1 通道6 半传输中断.
*                       - DMA1_IT_TE6: DMA1 通道6 传输错误中断.
*                       - DMA1_IT_GL7: DMA1 通道7 全局中断.
*                       - DMA1_IT_TC7: DMA1 通道7 传输完成中断.
*                       - DMA1_IT_HT7: DMA1 通道7 半传输中断.
*                       - DMA1_IT_TE7: DMA1 通道7 传输错误中断.
*                       - DMA2_IT_GL1: DMA2 通道1 全局中断.
*                       - DMA2_IT_TC1: DMA2 通道1 传输完成中断.
*                       - DMA2_IT_HT1: DMA2 通道1 半传输中断.
*                       - DMA2_IT_TE1: DMA2 通道1 传输错误中断.
*                       - DMA2_IT_GL2: DMA2 通道2 全局中断.
*                       - DMA2_IT_TC2: DMA2 通道2 传输完成中断.
*                       - DMA2_IT_HT2: DMA2 通道2 半传输中断.
*                       - DMA2_IT_TE2: DMA2 通道2 传输错误中断.
*                       - DMA2_IT_GL3: DMA2 通道3 全局中断.
*                       - DMA2_IT_TC3: DMA2 通道3 传输完成中断.
*                       - DMA2_IT_HT3: DMA2 通道3 半传输中断.
*                       - DMA2_IT_TE3: DMA2 通道3 传输错误中断.
*                       - DMA2_IT_GL4: DMA2 通道4 全局中断.
*                       - DMA2_IT_TC4: DMA2 通道4 传输完成中断.
*                       - DMA2_IT_HT4: DMA2 通道4 半传输中断.
*                       - DMA2_IT_TE4: DMA2 通道4 传输错误中断.
*                       - DMA2_IT_GL5: DMA2 通道5 全局中断.
*                       - DMA2_IT_TC5: DMA2 通道5 传输完成中断.
*                       - DMA2_IT_HT5: DMA2 通道5 半传输中断.
*                       - DMA2_IT_TE5: DMA2 通道5 传输错误中断.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void DMA_ClearITPendingBit(u32 DMA_IT)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_DMA_CLEAR_IT(DMA_IT));

  /* Calculate the used DMA [计算使用的DMA]*/
  if ((DMA_IT & FLAG_Mask) != (u32)RESET)
  {
    /* Clear the selected DMA interrupt pending bits [清除选择的DMA中断挂起位]*/
    DMA2->IFCR = DMA_IT;
  }
  else
  {
    /* Clear the selected DMA interrupt pending bits [清除选择的DMA中断挂起位]*/
    DMA1->IFCR = DMA_IT;
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

