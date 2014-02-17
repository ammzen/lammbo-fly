/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_rtc.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有RTC的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_rtc.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
#define CRL_CNF_Set      ((u16)0x0010)      /* Configuration Flag Enable Mask */
#define CRL_CNF_Reset    ((u16)0xFFEF)      /* Configuration Flag Disable Mask */
#define RTC_LSB_Mask     ((u32)0x0000FFFF)  /* RTC LSB Mask */
#define PRLH_MSB_Mask    ((u32)0x000F0000)  /* RTC Prescaler MSB Mask */

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: RTC_ITConfig
* 功能描述: 使能或关闭指定的RTC中断.
* 输入参数: (1)RTC_IT:要使能或关闭的RTC中断源.
*                    这个参数可以是下面值的任意组合:
*                       - RTC_IT_OW: 溢出中断使能
*                       - RTC_IT_ALR: 警报中断使能
*                       - RTC_IT_SEC: 第二次中断使能
*           (2)NewState:指定RTC中断的新状态.这个参数可以是:ENALBLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RTC_ITConfig(u16 RTC_IT, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_RTC_IT(RTC_IT));  
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    RTC->CRH |= RTC_IT;
  }
  else
  {
    RTC->CRH &= (u16)~RTC_IT;
  }
}

/*******************************************************************************
* 函数名称: RTC_EnterConfigMode
* 功能描述: 进入RTC配置模式.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RTC_EnterConfigMode(void)
{
  /* Set the CNF flag to enter in the Configuration Mode [置位CNF标志以进入配置模式]*/
  RTC->CRL |= CRL_CNF_Set;
}

/*******************************************************************************
* 函数名称: RTC_ExitConfigMode
* 功能描述: 退出RTC配置模式.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RTC_ExitConfigMode(void)
{
  /* Reset the CNF flag to exit from the Configuration Mode [复位CNF标志以退出配置模式]*/
  RTC->CRL &= CRL_CNF_Reset;
}

/*******************************************************************************
* 函数名称: RTC_GetCounter
* 功能描述: 获得RTC计数器值.
* 输入参数: 无
* 输出参数: 无
* 返回参数: RTC计数值.
*******************************************************************************/
u32 RTC_GetCounter(void)
{
  u16 tmp = 0;
  tmp = RTC->CNTL;

  return (((u32)RTC->CNTH << 16 ) | tmp) ;
}

/*******************************************************************************
* 函数名称: RTC_SetCounter
* 功能描述: 设置RTC计数器值.
* 输入参数: CounterValue:RTC计数新值.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RTC_SetCounter(u32 CounterValue)
{ 
  RTC_EnterConfigMode();

  /* Set RTC COUNTER MSB word [设置RTC计数器高半字节]*/
  RTC->CNTH = CounterValue >> 16;
  /* Set RTC COUNTER LSB word [设置RTC计数器低半字节]*/
  RTC->CNTL = (CounterValue & RTC_LSB_Mask);

  RTC_ExitConfigMode();
}

/*******************************************************************************
* 函数名称: RTC_SetPrescaler
* 功能描述: 获得RTC预分频数值.
* 输入参数: PrescalerValue:RTC预分频数新值.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RTC_SetPrescaler(u32 PrescalerValue)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_RTC_PRESCALER(PrescalerValue));
  
  RTC_EnterConfigMode();

  /* Set RTC PRESCALER MSB word [设置RTC分频器高半字节]*/
  RTC->PRLH = (PrescalerValue & PRLH_MSB_Mask) >> 16;
  /* Set RTC PRESCALER LSB word [设置RTC分频器低半字节]*/
  RTC->PRLL = (PrescalerValue & RTC_LSB_Mask);

  RTC_ExitConfigMode();
}

/*******************************************************************************
* 函数名称: RTC_SetAlarm
* 功能描述: 设置RTC警报值.
* 输入参数: AlarmValue:RTC警报新值.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RTC_SetAlarm(u32 AlarmValue)
{  
  RTC_EnterConfigMode();

  /* Set the ALARM MSB word [设置ALARM高半字节]*/
  RTC->ALRH = AlarmValue >> 16;
  /* Set the ALARM LSB word [设置ALARM低半字节]*/
  RTC->ALRL = (AlarmValue & RTC_LSB_Mask);

  RTC_ExitConfigMode();
}

/*******************************************************************************
* 函数名称: RTC_GetDivider
* 功能描述: 获得RTC分割值.
* 输入参数: 无
* 输出参数: 无
* 返回参数: RTC分割值.
*******************************************************************************/
u32 RTC_GetDivider(void)
{
  u32 tmp = 0x00;

  tmp = ((u32)RTC->DIVH & (u32)0x000F) << 16;
  tmp |= RTC->DIVL;

  return tmp;
}

/*******************************************************************************
* 函数名称: RTC_WaitForLastTask
* 功能描述: 等待直到RTC寄存写操作完成。
*               这个函数一定要在任何对RTC寄存器写操作之前调用.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RTC_WaitForLastTask(void)
{
  /* Loop until RTOFF flag is set [循环到RTOFF标志置位]*/
  while ((RTC->CRL & RTC_FLAG_RTOFF) == (u16)RESET)
  {
  }
}

/*******************************************************************************
* 函数名称: RTC_WaitForSynchro
* 功能描述: 等待RTC寄存器(RTC_CNT,RTC_ALR和RTC_PRL)与RTC APB时钟同步。
*               这个函数一定要在任何对APB复位或APB时钟停止后的写操作之前调用。.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RTC_WaitForSynchro(void)
{
  /* Clear RSF flag [清RSF标志]*/
  RTC->CRL &= (u16)~RTC_FLAG_RSF;

  /* Loop until RSF flag is set [循环到RSF标志置位]*/
  while ((RTC->CRL & RTC_FLAG_RSF) == (u16)RESET)
  {
  }
}

/*******************************************************************************
* 函数名称: RTC_GetFlagStatus
* 功能描述: 检查指定的RTC标志设置与否.
* 输入参数: RTC_FLAG:指定要检查的标志.
*                    这个参数可以选下面的值之一:
*                       - RTC_FLAG_RTOFF: RTC操作关闭标志
*                       - RTC_FLAG_RSF: 寄存器同步标志
*                       - RTC_FLAG_OW: 溢出中断标志
*                       - RTC_FLAG_ALR: 警报中断标志
*                       - RTC_FLAG_SEC: 第二中断标志
* 输出参数: 无
* 返回参数: RTC_FLAG的新状态(SET或RESET).
*******************************************************************************/
FlagStatus RTC_GetFlagStatus(u16 RTC_FLAG)
{
  FlagStatus bitstatus = RESET;
  
  /* Check the parameters [检查参数]*/
  assert_param(IS_RTC_GET_FLAG(RTC_FLAG)); 
  
  if ((RTC->CRL & RTC_FLAG) != (u16)RESET)
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
* 函数名称: RTC_ClearFlag
* 功能描述: 清除RTC挂起标志.
* 输入参数: RTC_FLAG:将被清除的标志。
*                    这个参数可以是下面值的任意组合:
*                       - RTC_FLAG_RSF: 同步寄存器标志. 只有在SPB复位或APB时钟停止之后清除..
*                       - RTC_FLAG_OW: 溢出标志
*                       - RTC_FLAG_ALR: 报警标志
*                       - RTC_FLAG_SEC: 第二次标志
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RTC_ClearFlag(u16 RTC_FLAG)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_RTC_CLEAR_FLAG(RTC_FLAG)); 
    
  /* Clear the coressponding RTC flag [清除coressponding RTC标志]*/
  RTC->CRL &= (u16)~RTC_FLAG;
}

/*******************************************************************************
* 函数名称: RTC_GetITStatus
* 功能描述: 检查指定的RTC中断发生与否..
* 输入参数: RTC_IT:要检查的RTC中断源..
*                    这个参数可以选下面的值之一:
*                       - RTC_IT_OW: 溢出中断
*                       - RTC_IT_ALR: 报警中断
*                       - RTC_IT_SEC: 第二次中断
* 输出参数: 无
* 返回参数: RTC_IT的新状态(SET或RESET).
*******************************************************************************/
ITStatus RTC_GetITStatus(u16 RTC_IT)
{
  ITStatus bitstatus = RESET;

  /* Check the parameters [检查参数]*/
  assert_param(IS_RTC_GET_IT(RTC_IT)); 
  
  bitstatus = (ITStatus)(RTC->CRL & RTC_IT);

  if (((RTC->CRH & RTC_IT) != (u16)RESET) && (bitstatus != (u16)RESET))
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
* 函数名称: RTC_ClearITPendingBit
* 功能描述: 清除RTC中断挂起位.
* 输入参数: RTC_IT:将要清除的中断挂起位..
*                    这个参数可以是下面值的任意组合:
*                       - RTC_IT_OW: 溢出中断
*                       - RTC_IT_ALR: 报警中断
*                       - RTC_IT_SEC: 第二次中断
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RTC_ClearITPendingBit(u16 RTC_IT)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_RTC_IT(RTC_IT));  
  
  /* Clear the coressponding RTC pending bit [清除coressponding RTC挂起标志]*/
  RTC->CRL &= (u16)~RTC_IT;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
