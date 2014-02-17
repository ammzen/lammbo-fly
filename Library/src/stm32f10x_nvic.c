/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_nvic.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������NVIC�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_nvic.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
#define AIRCR_VECTKEY_MASK    ((u32)0x05FA0000)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: NVIC_DeInit
* ��������: ��λNVIC��Χ�豸�Ĵ���ΪĬ�ϵĸ�λֵ.
* �������: ��
* �������: ��
* ���ز���: ��
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
* ��������: NVIC_SCBDeInit
* ��������: ��λSCB��Χ�豸�Ĵ���ΪĬ�ϵĸ�λֵ.
* �������: ��
* �������: ��
* ���ز���: ��
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
* ��������: NVIC_PriorityGroupConfig
* ��������: �������ȼ��飺���ȼ��������ȼ�.
* �������: NVIC_PriorityGroup:���ȼ���λ�ĳ��ȡ�
*                    ������������ֵ:
*                       - NVIC_PriorityGroup_0:0λ���ȼ���4λ�����ȼ�
*                       - NVIC_PriorityGroup_1:1λ���ȼ���3λ�����ȼ�
*                       - NVIC_PriorityGroup_2:2λ���ȼ���2λ�����ȼ�
*                       - NVIC_PriorityGroup_3:3λ���ȼ���1λ�����ȼ�
*                       - NVIC_PriorityGroup_4:4λ���ȼ���0λ�����ȼ�
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_PriorityGroupConfig(u32 NVIC_PriorityGroup)
{
  /* Check the parameters [������]*/
  assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
  
  /* Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup value
  [����NVIC_PriorityGroup��ֵ����PRIGROUP[10:8]��λ]*/
  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;
}

/*******************************************************************************
* ��������: NVIC_Init
* ��������: ͨ��NVIC_InitStruct�е�ָ����������ʼ����Χ�豸.
* �������: NVIC_InitStruct:һ��ָ�����ָ��NVIC��Χ�豸������Ϣ��NVIC_InitTypeDef�ṹ��ָ�롣
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct)
{
  u32 tmppriority = 0x00, tmpreg = 0x00, tmpmask = 0x00;
  u32 tmppre = 0, tmpsub = 0x0F;

  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
  assert_param(IS_NVIC_IRQ_CHANNEL(NVIC_InitStruct->NVIC_IRQChannel));
  assert_param(IS_NVIC_PREEMPTION_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority));  
  assert_param(IS_NVIC_SUB_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelSubPriority));
    
  if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
  {
    /* Compute the Corresponding IRQ Priority [������Ӧ���ն����ȼ�]*/
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
    
    /* Enable the Selected IRQ Channels [����ѡ����ж�ͨ��]*/
    NVIC->ISER[(NVIC_InitStruct->NVIC_IRQChannel >> 0x05)] =
      (u32)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (u8)0x1F);
  }
  else
  {
    /* Disable the Selected IRQ Channels [��ֹѡ����ж�ͨ��]*/
    NVIC->ICER[(NVIC_InitStruct->NVIC_IRQChannel >> 0x05)] =
      (u32)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (u8)0x1F);
  }
}

/*******************************************************************************
* ��������: NVIC_StructInit
* ��������: ��ÿһ��NVIC_InitStruct��Ա����Ĭ��ֵ.
* �������: NVIC_InitStruct:ָ�򽫱���ʼ����NVIC_InitTypeDef�ṹ��ָ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_StructInit(NVIC_InitTypeDef* NVIC_InitStruct)
{
  /* NVIC_InitStruct members default value [NVIC_InitStruct��ԱĬ��ֵ]*/
  NVIC_InitStruct->NVIC_IRQChannel = 0x00;
  NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStruct->NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStruct->NVIC_IRQChannelCmd = DISABLE;
}

/*******************************************************************************
* ��������: NVIC_SETPRIMASK
* ��������: ʹ��PRIMASK���ȼ�����ִ�е����ȼ�����Ϊ0.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_SETPRIMASK(void)
{
  __SETPRIMASK();
}

/*******************************************************************************
* ��������: NVIC_RESETPRIMASK
* ��������: ʹPRIMASK���ȼ���Ч.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_RESETPRIMASK(void)
{
  __RESETPRIMASK();
}

/*******************************************************************************
* ��������: NVIC_SETFAULTMASK
* ��������: ʹ��FAULTMASK���ȼ�����ִ�е����ȼ�����Ϊ��1.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_SETFAULTMASK(void)
{
  __SETFAULTMASK();
}

/*******************************************************************************
* ��������: NVIC_RESETFAULTMASK
* ��������: ʹFAULTMASK���ȼ���Ч��
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_RESETFAULTMASK(void)
{
  __RESETFAULTMASK();
}

/*******************************************************************************
* ��������: NVIC_BASEPRICONFIG
* ��������: ִ�����ȼ��ܹ���15(�����õ�������ȼ�)�ı�Ϊ1.д��0����ֹ���ȼ�.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_BASEPRICONFIG(u32 NewPriority)
{
  /* Check the parameters [������]*/
  assert_param(IS_NVIC_BASE_PRI(NewPriority));
  
  __BASEPRICONFIG(NewPriority << 0x04);
}

/*******************************************************************************
* ��������: NVIC_GetBASEPRI
* ��������: ����BASEPRI����ֵ.
* �������: ��
* �������: ��
* ���ز���: ����BASEPRI�Ĵ���ֵ
*******************************************************************************/
u32 NVIC_GetBASEPRI(void)
{
  return (__GetBASEPRI());
}

/*******************************************************************************
* ��������: NVIC_GetCurrentPendingIRQChannel
* ��������: ���ص�ǰ�����IRQͨ����channel����ʶ��.
* �������: ��
* �������: ��
* ���ز���: ����IRQͨ���ı�ʶ��.
*******************************************************************************/
u16 NVIC_GetCurrentPendingIRQChannel(void)
{
  return ((u16)((SCB->ICSR & (u32)0x003FF000) >> 0x0C));
}

/*******************************************************************************
* ��������: NVIC_GetIRQChannelPendingBitStatus
* ��������: ���ָ��IRQͨ������λ�Ƿ���λ��
* �������: NVIC_IRQChannel:Ҫ���ĵ��жϹ���λ.
* �������: ��
* ���ز���: IRQͨ������λ����״̬(SET or RESET).
*******************************************************************************/
ITStatus NVIC_GetIRQChannelPendingBitStatus(u8 NVIC_IRQChannel)
{
  ITStatus pendingirqstatus = RESET;
  u32 tmp = 0x00;
  
  /* Check the parameters [������]*/
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
* ��������: NVIC_SetIRQChannelPendingBit
* ��������: ����NVIC�жϹ���λ��
* �������: NVIC_IRQChannel:ָ����Ҫ��λ���жϹ���λ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_SetIRQChannelPendingBit(u8 NVIC_IRQChannel)
{
  /* Check the parameters [������]*/
  assert_param(IS_NVIC_IRQ_CHANNEL(NVIC_IRQChannel));
  
  *(vu32*) 0xE000EF00 = (u32)NVIC_IRQChannel;
}

/*******************************************************************************
* ��������: NVIC_ClearIRQChannelPendingBit
* ��������: ���NVIC�жϹ���λ��
* �������: NVIC_IRQChannel:ָ����Ҫ������жϹ���λ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_ClearIRQChannelPendingBit(u8 NVIC_IRQChannel)
{
  /* Check the parameters [������]*/
  assert_param(IS_NVIC_IRQ_CHANNEL(NVIC_IRQChannel));
  
  NVIC->ICPR[(NVIC_IRQChannel >> 0x05)] = (u32)0x01 << (NVIC_IRQChannel & (u32)0x1F);
}

/*******************************************************************************
* ��������: NVIC_GetCurrentActiveHandler
* ��������: ���ص�ǰ��������IRQͨ����ϵͳ������򣩵ı�ʶ��
* �������: ��
* �������: ��
* ���ز���: ���������ʶ��
*******************************************************************************/
u16 NVIC_GetCurrentActiveHandler(void)
{
  return ((u16)(SCB->ICSR & (u32)0x3FF));
}

/*******************************************************************************
* ��������: NVIC_GetIRQChannelActiveBitStatus
* ��������: ���ָ��IRQͨ���λ�������
* �������: NVIC_IRQChannel:ָ����Ҫ�����жϻλ.
* �������: ��
* ���ز���: IRQͨ���λ����״̬(SET or RESET)
*******************************************************************************/
ITStatus NVIC_GetIRQChannelActiveBitStatus(u8 NVIC_IRQChannel)
{
  ITStatus activeirqstatus = RESET;
  u32 tmp = 0x00;

  /* Check the parameters [������]*/
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
* ��������: NVIC_GetCPUID
* ��������: ����Cortex-M3�˵�ID�ţ��汾�ź�ʵ��ϸ�ڡ�
* �������: ��
* �������: ��
* ���ز���: CPU ID.
*******************************************************************************/
u32 NVIC_GetCPUID(void)
{
  return (SCB->CPUID);
}

/*******************************************************************************
* ��������: NVIC_SetVectorTable
* ��������: �����������λ�ú�ƫ����.
* �������: (1)NVIC_VectTab:ָ���ж���������RAM������FLASH�洢����.
*                    �����������ȡ�����ֵ֮һ:
*                       - NVIC_VectTab_RAM
*                       - NVIC_VectTab_FLASH
*           (2)Offset:������Ļ�ַƫ�ƣ����ֵһ��Ҫ��0x0100��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_NVIC_VECTTAB(NVIC_VectTab));
  assert_param(IS_NVIC_OFFSET(Offset));  
   
  SCB->VTOR = NVIC_VectTab | (Offset & (u32)0x1FFFFF80);
}

/*******************************************************************************
* ��������: NVIC_GenerateSystemReset
* ��������: ����һ��ϵͳ��λ.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_GenerateSystemReset(void)
{
  SCB->AIRCR = AIRCR_VECTKEY_MASK | (u32)0x04;
}

/*******************************************************************************
* ��������: NVIC_GenerateCoreReset
* ��������: ����һ���ˣ�Core+NVIC����λ.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_GenerateCoreReset(void)
{
  SCB->AIRCR = AIRCR_VECTKEY_MASK | (u32)0x01;
}

/*******************************************************************************
* ��������: NVIC_SystemLPConfig
* ��������: ѡ��ϵͳ����͹���ģʽ��������
* �������: (1)LowPowerMode:ϵͳΪ����͹���ģʽ����ģʽ.
*                    �����������ȡ�����ֵ֮һ:
*                       - NVIC_LP_SEVONPEND
*                       - NVIC_LP_SLEEPDEEP
*                       - NVIC_LP_SLEEPONEXIT
*           (2)NewState:LP��������״̬.�������������:ENABLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_SystemLPConfig(u8 LowPowerMode, FunctionalState NewState)
{
  /* Check the parameters [������]*/
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
* ��������: NVIC_SystemHandlerConfig
* ��������: ʹ�ܻ�ر�ָ����ϵͳ�������
* �������: (1)SystemHandler:ϵͳ���������Ч����Ч..
*                    �����������ȡ�����ֵ֮һ:
*                       - SystemHandler_MemoryManage
*                       - SystemHandler_BusFault
*                       - SystemHandler_UsageFault
*           (2)NewState:ָ��ϵͳ����������״̬.�������������:ENABLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_SystemHandlerConfig(u32 SystemHandler, FunctionalState NewState)
{
  u32 tmpreg = 0x00;

  /* Check the parameters [������]*/
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
* ��������: NVIC_SystemHandlerPriorityConfig
* ��������: ����ָ��ϵͳ�����������ȼ���
* �������: (1)SystemHandler:ʹ�ܻ�ر�ϵͳ�������.
*                    �����������ȡ�����ֵ֮һ:
*                       - SystemHandler_MemoryManage
*                       - SystemHandler_BusFault
*                       - SystemHandler_UsageFault
*                       - SystemHandler_SVCall
*                       - SystemHandler_DebugMonitor
*                       - SystemHandler_PSV
*                       - SystemHandler_SysTick
*            (2)SystemHandlerPreemptionPriority:ָ��ϵͳ���������µ����ȼ���.
*            (3)SystemHandlerSubPriority:ָ��ϵͳ���������������ȼ�.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_SystemHandlerPriorityConfig(u32 SystemHandler, u8 SystemHandlerPreemptionPriority,
                                      u8 SystemHandlerSubPriority)
{
  u32 tmp1 = 0x00, tmp2 = 0xFF, handlermask = 0x00;
  u32 tmppriority = 0x00;

  /* Check the parameters [������]*/
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
* ��������: NVIC_GetSystemHandlerPendingBitStatus
* ��������: ���ָ��ϵͳ����������λ�������
* �������: SystemHandler:Ҫ����ϵͳ����������λ.
*                    �����������ȡ�����ֵ֮һ:
*                       - SystemHandler_MemoryManage
*                       - SystemHandler_BusFault
*                       - SystemHandler_SVCall
* �������: ��
* ���ز���: ϵͳ����������λ����״̬(SET or RESET).
*******************************************************************************/
ITStatus NVIC_GetSystemHandlerPendingBitStatus(u32 SystemHandler)
{
  ITStatus bitstatus  = RESET;
  u32 tmp = 0x00, tmppos = 0x00;

  /* Check the parameters [������]*/
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
* ��������: NVIC_SetSystemHandlerPendingBit
* ��������: ����ϵͳ����������λ��
* �������: SystemHandler:Ҫ���õ�ϵͳ����������λ.
*                    �����������ȡ�����ֵ֮һ:
*                       - SystemHandler_NMI
*                       - SystemHandler_PSV
*                       - SystemHandler_SysTick
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_SetSystemHandlerPendingBit(u32 SystemHandler)
{
  u32 tmp = 0x00;

  /* Check the parameters [������]*/
  assert_param(IS_SET_PENDING_SYSTEM_HANDLER(SystemHandler));
  
  /* Get the System Handler pending bit position [ȡ��ϵͳ����������λλ��]*/
  tmp = SystemHandler & (u32)0x1F;
  /* Set the corresponding System Handler pending bit [��λ��Ӧ��ϵͳ����������λ]*/
  SCB->ICSR |= ((u32)0x01 << tmp);
}

/*******************************************************************************
* ��������: NVIC_ClearSystemHandlerPendingBit
* ��������: ���ϵͳ����������λ��
* �������: SystemHandler:Ҫ��λ��ϵͳ����������λ.
*                    �����������ȡ�����ֵ֮һ:
*                       - SystemHandler_PSV
*                       - SystemHandler_SysTick
* �������: ��
* ���ز���: ��
*******************************************************************************/
void NVIC_ClearSystemHandlerPendingBit(u32 SystemHandler)
{
  u32 tmp = 0x00;

  /* Check the parameters [������]*/
  assert_param(IS_CLEAR_SYSTEM_HANDLER(SystemHandler));
  
  /* Get the System Handler pending bit position [ȡ��ϵͳ����������λλ��]*/
  tmp = SystemHandler & (u32)0x1F;
  /* Clear the corresponding System Handler pending bit [�����Ӧ��ϵͳ����������λ]*/
  SCB->ICSR |= ((u32)0x01 << (tmp - 0x01));
}

/*******************************************************************************
* ��������: NVIC_GetSystemHandlerActiveBitStatus
* ��������: ���ָ��ϵͳ�������λ�������
* �������: SystemHandler:Ҫ����ϵͳ�������λ.
*                    �����������ȡ�����ֵ֮һ:
*                       - SystemHandler_MemoryManage
*                       - SystemHandler_BusFault
*                       - SystemHandler_UsageFault
*                       - SystemHandler_SVCall
*                       - SystemHandler_DebugMonitor
*                       - SystemHandler_PSV
*                       - SystemHandler_SysTick
* �������: ��
* ���ز���: ϵͳ�������λ����״̬(SET or RESET).
*******************************************************************************/
ITStatus NVIC_GetSystemHandlerActiveBitStatus(u32 SystemHandler)
{
  ITStatus bitstatus  = RESET;

  u32 tmp = 0x00, tmppos = 0x00;

  /* Check the parameters [������]*/
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
* ��������: NVIC_GetFaultHandlerSources
* ��������: ����ϵͳ����������Դ.
* �������: SystemHandler:��Ҫ���ع���Դ��ϵͳ�������
*                    �����������ȡ�����ֵ֮һ:
*                       - SystemHandler_HardFault
*                       - SystemHandler_MemoryManage
*                       - SystemHandler_BusFault
*                       - SystemHandler_UsageFault
*                       - SystemHandler_DebugMonitor
* �������: ��
* ���ز���: ���ϴ������Դ.
*******************************************************************************/
u32 NVIC_GetFaultHandlerSources(u32 SystemHandler)
{
  u32 faultsources = 0x00;
  u32 tmpreg = 0x00, tmppos = 0x00;

  /* Check the parameters [������]*/
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
* ��������: NVIC_GetFaultAddress
* ��������: �������ɹ��ϴ������λ�õĵ�ַ.
* �������: SystemHandler����Ҫ���ع��ϵ�ַ��ϵͳ�������.
*                    �����������ȡ�����ֵ֮һ:
*                       - SystemHandler_MemoryManage
*                       - SystemHandler_BusFault
* �������: ��
* ���ز���: Fault address.
*******************************************************************************/
u32 NVIC_GetFaultAddress(u32 SystemHandler)
{
  u32 faultaddress = 0x00;
  u32 tmp = 0x00;

  /* Check the parameters [������]*/
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
