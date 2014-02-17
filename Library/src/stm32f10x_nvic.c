/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_nvic.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有NVIC的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_nvic.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
#define AIRCR_VECTKEY_MASK    ((u32)0x05FA0000)

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: NVIC_DeInit
* 功能描述: 复位NVIC外围设备寄存器为默认的复位值.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_DeInit(void)
{
  u32 index = 0;
  
  NVIC->ICER[0] = 0xFFFFFFFF;
  NVIC->ICER[1] = 0x0FFFFFFF;
  NVIC->ICPR[0] = 0xFFFFFFFF;
  NVIC->ICPR[1] = 0x0FFFFFFF;
  
  for(index = 0; index < 0x0F; index++)
  {
     NVIC->IPR[index] = 0x00000000;
  } 
}

/*******************************************************************************
* 函数名称: NVIC_SCBDeInit
* 功能描述: 复位SCB外围设备寄存器为默认的复位值.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_SCBDeInit(void)
{
  u32 index = 0x00;
  
  SCB->ICSR = 0x0A000000;
  SCB->VTOR = 0x00000000;
  SCB->AIRCR = AIRCR_VECTKEY_MASK;
  SCB->SCR = 0x00000000;
  SCB->CCR = 0x00000000;
  for(index = 0; index < 0x03; index++)
  {
     SCB->SHPR[index] = 0;
  }
  SCB->SHCSR = 0x00000000;
  SCB->CFSR = 0xFFFFFFFF;
  SCB->HFSR = 0xFFFFFFFF;
  SCB->DFSR = 0xFFFFFFFF;
}

/*******************************************************************************
* 函数名称: NVIC_PriorityGroupConfig
* 功能描述: 配置优先级组：优先级和子优先级.
* 输入参数: NVIC_PriorityGroup:优先级组位的长度。
*                    这个参数允许的值:
*                       - NVIC_PriorityGroup_0:0位优先级；4位子优先级
*                       - NVIC_PriorityGroup_1:1位优先级；3位子优先级
*                       - NVIC_PriorityGroup_2:2位优先级；2位子优先级
*                       - NVIC_PriorityGroup_3:3位优先级；1位子优先级
*                       - NVIC_PriorityGroup_4:4位优先级；0位子优先级
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_PriorityGroupConfig(u32 NVIC_PriorityGroup)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
  
  /* Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup value
  [依照NVIC_PriorityGroup的值设置PRIGROUP[10:8]的位]*/
  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;
}

/*******************************************************************************
* 函数名称: NVIC_Init
* 功能描述: 通过NVIC_InitStruct中的指定参数来初始化外围设备.
* 输入参数: NVIC_InitStruct:一个指向包含指定NVIC外围设备配置信息的NVIC_InitTypeDef结构的指针。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct)
{
  u32 tmppriority = 0x00, tmpreg = 0x00, tmpmask = 0x00;
  u32 tmppre = 0, tmpsub = 0x0F;

  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
  assert_param(IS_NVIC_IRQ_CHANNEL(NVIC_InitStruct->NVIC_IRQChannel));
  assert_param(IS_NVIC_PREEMPTION_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority));  
  assert_param(IS_NVIC_SUB_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelSubPriority));
    
  if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
  {
    /* Compute the Corresponding IRQ Priority [计算相应的终端优先级]*/
    tmppriority = (0x700 - (SCB->AIRCR & (u32)0x700))>> 0x08;
    tmppre = (0x4 - tmppriority);
    tmpsub = tmpsub >> tmppriority;
    
    tmppriority = (u32)NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority << tmppre;
    tmppriority |=  NVIC_InitStruct->NVIC_IRQChannelSubPriority & tmpsub;

    tmppriority = tmppriority << 0x04;
    tmppriority = ((u32)tmppriority) << ((NVIC_InitStruct->NVIC_IRQChannel & (u8)0x03) * 0x08);
    
    tmpreg = NVIC->IPR[(NVIC_InitStruct->NVIC_IRQChannel >> 0x02)];
    tmpmask = (u32)0xFF << ((NVIC_InitStruct->NVIC_IRQChannel & (u8)0x03) * 0x08);
    tmpreg &= ~tmpmask;
    tmppriority &= tmpmask;  
    tmpreg |= tmppriority;

    NVIC->IPR[(NVIC_InitStruct->NVIC_IRQChannel >> 0x02)] = tmpreg;
    
    /* Enable the Selected IRQ Channels [允许选择的中断通道]*/
    NVIC->ISER[(NVIC_InitStruct->NVIC_IRQChannel >> 0x05)] =
      (u32)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (u8)0x1F);
  }
  else
  {
    /* Disable the Selected IRQ Channels [禁止选择的中断通道]*/
    NVIC->ICER[(NVIC_InitStruct->NVIC_IRQChannel >> 0x05)] =
      (u32)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (u8)0x1F);
  }
}

/*******************************************************************************
* 函数名称: NVIC_StructInit
* 功能描述: 给每一个NVIC_InitStruct成员填上默认值.
* 输入参数: NVIC_InitStruct:指向将被初始化的NVIC_InitTypeDef结构的指针.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_StructInit(NVIC_InitTypeDef* NVIC_InitStruct)
{
  /* NVIC_InitStruct members default value [NVIC_InitStruct成员默认值]*/
  NVIC_InitStruct->NVIC_IRQChannel = 0x00;
  NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStruct->NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStruct->NVIC_IRQChannelCmd = DISABLE;
}

/*******************************************************************************
* 函数名称: NVIC_SETPRIMASK
* 功能描述: 使能PRIMASK优先级：把执行的优先级提升为0.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_SETPRIMASK(void)
{
  __SETPRIMASK();
}

/*******************************************************************************
* 函数名称: NVIC_RESETPRIMASK
* 功能描述: 使PRIMASK优先级无效.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_RESETPRIMASK(void)
{
  __RESETPRIMASK();
}

/*******************************************************************************
* 函数名称: NVIC_SETFAULTMASK
* 功能描述: 使能FAULTMASK优先级：把执行的优先级提升为－1.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_SETFAULTMASK(void)
{
  __SETFAULTMASK();
}

/*******************************************************************************
* 函数名称: NVIC_RESETFAULTMASK
* 功能描述: 使FAULTMASK优先级无效。
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_RESETFAULTMASK(void)
{
  __RESETFAULTMASK();
}

/*******************************************************************************
* 函数名称: NVIC_BASEPRICONFIG
* 功能描述: 执行优先级能够从15(可配置的最低优先级)改变为1.写入0将禁止优先级.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_BASEPRICONFIG(u32 NewPriority)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_NVIC_BASE_PRI(NewPriority));
  
  __BASEPRICONFIG(NewPriority << 0x04);
}

/*******************************************************************************
* 函数名称: NVIC_GetBASEPRI
* 功能描述: 返回BASEPRI掩码值.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 返回BASEPRI寄存器值
*******************************************************************************/
u32 NVIC_GetBASEPRI(void)
{
  return (__GetBASEPRI());
}

/*******************************************************************************
* 函数名称: NVIC_GetCurrentPendingIRQChannel
* 功能描述: 返回当前挂起的IRQ通道（channel）标识符.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 挂起IRQ通道的标识符.
*******************************************************************************/
u16 NVIC_GetCurrentPendingIRQChannel(void)
{
  return ((u16)((SCB->ICSR & (u32)0x003FF000) >> 0x0C));
}

/*******************************************************************************
* 函数名称: NVIC_GetIRQChannelPendingBitStatus
* 功能描述: 检查指定IRQ通道挂起位是否置位。
* 输入参数: NVIC_IRQChannel:要检查的的中断挂起位.
* 输出参数: 无
* 返回参数: IRQ通道挂起位的新状态(SET or RESET).
*******************************************************************************/
ITStatus NVIC_GetIRQChannelPendingBitStatus(u8 NVIC_IRQChannel)
{
  ITStatus pendingirqstatus = RESET;
  u32 tmp = 0x00;
  
  /* Check the parameters [检查参数]*/
  assert_param(IS_NVIC_IRQ_CHANNEL(NVIC_IRQChannel));
  
  tmp = ((u32)0x01 << (NVIC_IRQChannel & (u32)0x1F));

  if (((NVIC->ISPR[(NVIC_IRQChannel >> 0x05)]) & tmp) == tmp)
  {
    pendingirqstatus = SET;
  }
  else
  {
    pendingirqstatus = RESET;
  }
  return pendingirqstatus;
}

/*******************************************************************************
* 函数名称: NVIC_SetIRQChannelPendingBit
* 功能描述: 设置NVIC中断挂起位。
* 输入参数: NVIC_IRQChannel:指定了要置位的中断挂起位。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_SetIRQChannelPendingBit(u8 NVIC_IRQChannel)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_NVIC_IRQ_CHANNEL(NVIC_IRQChannel));
  
  *(vu32*) 0xE000EF00 = (u32)NVIC_IRQChannel;
}

/*******************************************************************************
* 函数名称: NVIC_ClearIRQChannelPendingBit
* 功能描述: 清除NVIC中断挂起位。
* 输入参数: NVIC_IRQChannel:指定了要清除的中断挂起位。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_ClearIRQChannelPendingBit(u8 NVIC_IRQChannel)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_NVIC_IRQ_CHANNEL(NVIC_IRQChannel));
  
  NVIC->ICPR[(NVIC_IRQChannel >> 0x05)] = (u32)0x01 << (NVIC_IRQChannel & (u32)0x1F);
}

/*******************************************************************************
* 函数名称: NVIC_GetCurrentActiveHandler
* 功能描述: 返回当前活动处理程序（IRQ通道和系统处理程序）的标识符
* 输入参数: 无
* 输出参数: 无
* 返回参数: 活动处理程序标识符
*******************************************************************************/
u16 NVIC_GetCurrentActiveHandler(void)
{
  return ((u16)(SCB->ICSR & (u32)0x3FF));
}

/*******************************************************************************
* 函数名称: NVIC_GetIRQChannelActiveBitStatus
* 功能描述: 检查指定IRQ通道活动位设置与否。
* 输入参数: NVIC_IRQChannel:指定了要检查的中断活动位.
* 输出参数: 无
* 返回参数: IRQ通道活动位的新状态(SET or RESET)
*******************************************************************************/
ITStatus NVIC_GetIRQChannelActiveBitStatus(u8 NVIC_IRQChannel)
{
  ITStatus activeirqstatus = RESET;
  u32 tmp = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_NVIC_IRQ_CHANNEL(NVIC_IRQChannel));
  
  tmp = ((u32)0x01 << (NVIC_IRQChannel & (u32)0x1F));

  if (((NVIC->IABR[(NVIC_IRQChannel >> 0x05)]) & tmp) == tmp )
  {
    activeirqstatus = SET;
  }
  else
  {
    activeirqstatus = RESET;
  }
  return activeirqstatus;
}

/*******************************************************************************
* 函数名称: NVIC_GetCPUID
* 功能描述: 返回Cortex-M3核的ID号，版本号和实现细节。
* 输入参数: 无
* 输出参数: 无
* 返回参数: CPU ID.
*******************************************************************************/
u32 NVIC_GetCPUID(void)
{
  return (SCB->CPUID);
}

/*******************************************************************************
* 函数名称: NVIC_SetVectorTable
* 功能描述: 设置向量表的位置和偏移量.
* 输入参数: (1)NVIC_VectTab:指定中断向量表在RAM还是在FLASH存储器中.
*                    这个参数可以取下面的值之一:
*                       - NVIC_VectTab_RAM
*                       - NVIC_VectTab_FLASH
*           (2)Offset:向量表的基址偏移，这个值一定要比0x0100高.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)
{ 
  /* Check the parameters [检查参数]*/
  assert_param(IS_NVIC_VECTTAB(NVIC_VectTab));
  assert_param(IS_NVIC_OFFSET(Offset));  
   
  SCB->VTOR = NVIC_VectTab | (Offset & (u32)0x1FFFFF80);
}

/*******************************************************************************
* 函数名称: NVIC_GenerateSystemReset
* 功能描述: 生成一个系统复位.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_GenerateSystemReset(void)
{
  SCB->AIRCR = AIRCR_VECTKEY_MASK | (u32)0x04;
}

/*******************************************************************************
* 函数名称: NVIC_GenerateCoreReset
* 功能描述: 生成一个核（Core+NVIC）复位.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_GenerateCoreReset(void)
{
  SCB->AIRCR = AIRCR_VECTKEY_MASK | (u32)0x01;
}

/*******************************************************************************
* 函数名称: NVIC_SystemLPConfig
* 功能描述: 选择系统进入低功耗模式的条件。
* 输入参数: (1)LowPowerMode:系统为进入低功耗模式的新模式.
*                    这个参数可以取下面的值之一:
*                       - NVIC_LP_SEVONPEND
*                       - NVIC_LP_SLEEPDEEP
*                       - NVIC_LP_SLEEPONEXIT
*           (2)NewState:LP条件的新状态.这个参数可以是:ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_SystemLPConfig(u8 LowPowerMode, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_NVIC_LP(LowPowerMode));
  assert_param(IS_FUNCTIONAL_STATE(NewState));  
  
  if (NewState != DISABLE)
  {
    SCB->SCR |= LowPowerMode;
  }
  else
  {
    SCB->SCR &= (u32)(~(u32)LowPowerMode);
  }
}

/*******************************************************************************
* 函数名称: NVIC_SystemHandlerConfig
* 功能描述: 使能或关闭指定的系统处理程序。
* 输入参数: (1)SystemHandler:系统处理程序有效或无效..
*                    这个参数可以取下面的值之一:
*                       - SystemHandler_MemoryManage
*                       - SystemHandler_BusFault
*                       - SystemHandler_UsageFault
*           (2)NewState:指定系统处理程序的新状态.这个参数可以是:ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_SystemHandlerConfig(u32 SystemHandler, FunctionalState NewState)
{
  u32 tmpreg = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_CONFIG_SYSTEM_HANDLER(SystemHandler));
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  
  tmpreg =  (u32)0x01 << (SystemHandler & (u32)0x1F);

  if (NewState != DISABLE)
  {
    SCB->SHCSR |= tmpreg;
  }
  else
  {
    SCB->SHCSR &= ~tmpreg;
  }
}

/*******************************************************************************
* 函数名称: NVIC_SystemHandlerPriorityConfig
* 功能描述: 配置指定系统处理程序的优先级。
* 输入参数: (1)SystemHandler:使能或关闭系统处理程序.
*                    这个参数可以取下面的值之一:
*                       - SystemHandler_MemoryManage
*                       - SystemHandler_BusFault
*                       - SystemHandler_UsageFault
*                       - SystemHandler_SVCall
*                       - SystemHandler_DebugMonitor
*                       - SystemHandler_PSV
*                       - SystemHandler_SysTick
*            (2)SystemHandlerPreemptionPriority:指定系统处理程序的新的优先级组.
*            (3)SystemHandlerSubPriority:指定系统处理程序的新子优先级.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_SystemHandlerPriorityConfig(u32 SystemHandler, u8 SystemHandlerPreemptionPriority,
                                      u8 SystemHandlerSubPriority)
{
  u32 tmp1 = 0x00, tmp2 = 0xFF, handlermask = 0x00;
  u32 tmppriority = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_PRIORITY_SYSTEM_HANDLER(SystemHandler));
  assert_param(IS_NVIC_PREEMPTION_PRIORITY(SystemHandlerPreemptionPriority));  
  assert_param(IS_NVIC_SUB_PRIORITY(SystemHandlerSubPriority));
    
  tmppriority = (0x700 - (SCB->AIRCR & (u32)0x700))>> 0x08;
  tmp1 = (0x4 - tmppriority);
  tmp2 = tmp2 >> tmppriority;
    
  tmppriority = (u32)SystemHandlerPreemptionPriority << tmp1;
  tmppriority |=  SystemHandlerSubPriority & tmp2;

  tmppriority = tmppriority << 0x04;
  tmp1 = SystemHandler & (u32)0xC0;
  tmp1 = tmp1 >> 0x06; 
  tmp2 = (SystemHandler >> 0x08) & (u32)0x03;
  tmppriority = tmppriority << (tmp2 * 0x08);
  handlermask = (u32)0xFF << (tmp2 * 0x08);
  
  SCB->SHPR[tmp1] &= ~handlermask;
  SCB->SHPR[tmp1] |= tmppriority;
}

/*******************************************************************************
* 函数名称: NVIC_GetSystemHandlerPendingBitStatus
* 功能描述: 检查指定系统处理程序挂起位设置与否。
* 输入参数: SystemHandler:要检查的系统处理程序挂起位.
*                    这个参数可以取下面的值之一:
*                       - SystemHandler_MemoryManage
*                       - SystemHandler_BusFault
*                       - SystemHandler_SVCall
* 输出参数: 无
* 返回参数: 系统处理程序挂起位的新状态(SET or RESET).
*******************************************************************************/
ITStatus NVIC_GetSystemHandlerPendingBitStatus(u32 SystemHandler)
{
  ITStatus bitstatus  = RESET;
  u32 tmp = 0x00, tmppos = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_GET_PENDING_SYSTEM_HANDLER(SystemHandler));
  
  tmppos = (SystemHandler >> 0x0A);
  tmppos &= (u32)0x0F;

  tmppos = (u32)0x01 << tmppos;

  tmp = SCB->SHCSR & tmppos;

  if (tmp == tmppos)
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
* 函数名称: NVIC_SetSystemHandlerPendingBit
* 功能描述: 设置系统处理程序挂起位。
* 输入参数: SystemHandler:要设置的系统处理程序挂起位.
*                    这个参数可以取下面的值之一:
*                       - SystemHandler_NMI
*                       - SystemHandler_PSV
*                       - SystemHandler_SysTick
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_SetSystemHandlerPendingBit(u32 SystemHandler)
{
  u32 tmp = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_SET_PENDING_SYSTEM_HANDLER(SystemHandler));
  
  /* Get the System Handler pending bit position [取得系统处理程序挂起位位置]*/
  tmp = SystemHandler & (u32)0x1F;
  /* Set the corresponding System Handler pending bit [置位相应的系统处理程序挂起位]*/
  SCB->ICSR |= ((u32)0x01 << tmp);
}

/*******************************************************************************
* 函数名称: NVIC_ClearSystemHandlerPendingBit
* 功能描述: 清除系统处理程序挂起位。
* 输入参数: SystemHandler:要复位的系统处理程序挂起位.
*                    这个参数可以取下面的值之一:
*                       - SystemHandler_PSV
*                       - SystemHandler_SysTick
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void NVIC_ClearSystemHandlerPendingBit(u32 SystemHandler)
{
  u32 tmp = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_CLEAR_SYSTEM_HANDLER(SystemHandler));
  
  /* Get the System Handler pending bit position [取得系统处理程序挂起位位置]*/
  tmp = SystemHandler & (u32)0x1F;
  /* Clear the corresponding System Handler pending bit [清除相应的系统处理程序挂起位]*/
  SCB->ICSR |= ((u32)0x01 << (tmp - 0x01));
}

/*******************************************************************************
* 函数名称: NVIC_GetSystemHandlerActiveBitStatus
* 功能描述: 检查指定系统处理程序活动位设置与否。
* 输入参数: SystemHandler:要检查的系统处理程序活动位.
*                    这个参数可以取下面的值之一:
*                       - SystemHandler_MemoryManage
*                       - SystemHandler_BusFault
*                       - SystemHandler_UsageFault
*                       - SystemHandler_SVCall
*                       - SystemHandler_DebugMonitor
*                       - SystemHandler_PSV
*                       - SystemHandler_SysTick
* 输出参数: 无
* 返回参数: 系统处理程序活动位的新状态(SET or RESET).
*******************************************************************************/
ITStatus NVIC_GetSystemHandlerActiveBitStatus(u32 SystemHandler)
{
  ITStatus bitstatus  = RESET;

  u32 tmp = 0x00, tmppos = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_GET_ACTIVE_SYSTEM_HANDLER(SystemHandler));
  
  tmppos = (SystemHandler >> 0x0E) & (u32)0x0F;

  tmppos = (u32)0x01 << tmppos;

  tmp = SCB->SHCSR & tmppos;

  if (tmp == tmppos)
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
* 函数名称: NVIC_GetFaultHandlerSources
* 功能描述: 返回系统处理程序故障源.
* 输入参数: SystemHandler:将要返回故障源的系统处理程序
*                    这个参数可以取下面的值之一:
*                       - SystemHandler_HardFault
*                       - SystemHandler_MemoryManage
*                       - SystemHandler_BusFault
*                       - SystemHandler_UsageFault
*                       - SystemHandler_DebugMonitor
* 输出参数: 无
* 返回参数: 故障处理程序源.
*******************************************************************************/
u32 NVIC_GetFaultHandlerSources(u32 SystemHandler)
{
  u32 faultsources = 0x00;
  u32 tmpreg = 0x00, tmppos = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_FAULT_SOURCE_SYSTEM_HANDLER(SystemHandler));
  
  tmpreg = (SystemHandler >> 0x12) & (u32)0x03;
  tmppos = (SystemHandler >> 0x14) & (u32)0x03;

  if (tmpreg == 0x00)
  {
    faultsources = SCB->HFSR;
  }
  else if (tmpreg == 0x01)
  {
    faultsources = SCB->CFSR >> (tmppos * 0x08);
    if (tmppos != 0x02)
    {
      faultsources &= (u32)0x0F;
    }
    else
    {
      faultsources &= (u32)0xFF;
    }
  }
  else
  {
    faultsources = SCB->DFSR;
  }
  return faultsources;
}

/*******************************************************************************
* 函数名称: NVIC_GetFaultAddress
* 功能描述: 返回生成故障处理程序位置的地址.
* 输入参数: SystemHandler：将要返回故障地址的系统处理程序.
*                    这个参数可以取下面的值之一:
*                       - SystemHandler_MemoryManage
*                       - SystemHandler_BusFault
* 输出参数: 无
* 返回参数: Fault address.
*******************************************************************************/
u32 NVIC_GetFaultAddress(u32 SystemHandler)
{
  u32 faultaddress = 0x00;
  u32 tmp = 0x00;

  /* Check the parameters [检查参数]*/
  assert_param(IS_FAULT_ADDRESS_SYSTEM_HANDLER(SystemHandler));
  
  tmp = (SystemHandler >> 0x16) & (u32)0x01;

  if (tmp == 0x00)
  {
    faultaddress = SCB->MMFAR;
  }
  else
  {
    faultaddress = SCB->BFAR;
  }
  return faultaddress;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
