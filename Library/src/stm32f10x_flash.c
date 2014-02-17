/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_flash.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有FLASH的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_flash.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
/* Flash Access Control Register bits */
#define ACR_LATENCY_Mask         ((u32)0x00000038)
#define ACR_HLFCYA_Mask          ((u32)0xFFFFFFF7)
#define ACR_PRFTBE_Mask          ((u32)0xFFFFFFEF)

#ifdef _FLASH_PROG
/* Flash Access Control Register bits */
#define ACR_PRFTBS_Mask          ((u32)0x00000020) 

/* Flash Control Register bits */
#define CR_PG_Set                ((u32)0x00000001)
#define CR_PG_Reset              ((u32)0x00001FFE) 

#define CR_PER_Set               ((u32)0x00000002)
#define CR_PER_Reset             ((u32)0x00001FFD)

#define CR_MER_Set               ((u32)0x00000004)
#define CR_MER_Reset             ((u32)0x00001FFB)

#define CR_OPTPG_Set             ((u32)0x00000010)
#define CR_OPTPG_Reset           ((u32)0x00001FEF)

#define CR_OPTER_Set             ((u32)0x00000020)
#define CR_OPTER_Reset           ((u32)0x00001FDF)

#define CR_STRT_Set              ((u32)0x00000040)
                			 
#define CR_LOCK_Set              ((u32)0x00000080)

/* FLASH Mask */
#define RDPRT_Mask               ((u32)0x00000002)
#define WRP0_Mask                ((u32)0x000000FF)
#define WRP1_Mask                ((u32)0x0000FF00)
#define WRP2_Mask                ((u32)0x00FF0000)
#define WRP3_Mask                ((u32)0xFF000000)

/* FLASH Keys */
#define RDP_Key                  ((u16)0x00A5)
#define FLASH_KEY1               ((u32)0x45670123)
#define FLASH_KEY2               ((u32)0xCDEF89AB)

/* Delay definition */   
#define EraseTimeout             ((u32)0x00000FFF)
#define ProgramTimeout           ((u32)0x0000000F)
#endif

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型-----------------------------------------------*/
#ifdef _FLASH_PROG
static void delay(void);
#endif

/* 自用函数 ---------------------------------------------------------*/
/*******************************************************************************
* 函数名称: FLASH_SetLatency
* 功能描述: 设置代码延时值.
* 输入参数: FLASH_Latency指定了FLASH延时值。
*                    这个参数可以是下面的值之一:
*                       - FLASH_Latency_0: 0个延时周期
*                       - FLASH_Latency_1: 1个延时周期
*                       - FLASH_Latency_2: 2个延时周期
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FLASH_SetLatency(u32 FLASH_Latency)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FLASH_LATENCY(FLASH_Latency));
  
  /* Sets the Latency value [设置延时值]*/
  FLASH->ACR &= ACR_LATENCY_Mask;
  FLASH->ACR |= FLASH_Latency;
}

/*******************************************************************************
* 函数名称: FLASH_HalfCycleAccessCmd
* 功能描述: 使能（或禁能）半周期Flash访问
* 输入参数: FLASH_HalfCycleAccess：FLASH半周期模式。
*                    这个参数可以是下面的值之一:
*                       - FLASH_HalfCycleAccess_Enable: 使能半周期访问
*                       - FLASH_HalfCycleAccess_Disable: 禁能半周期访问
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FLASH_HalfCycleAccessCmd(u32 FLASH_HalfCycleAccess)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FLASH_HALFCYCLEACCESS_STATE(FLASH_HalfCycleAccess));
  
  /* Enable or disable the Half cycle access [使能或者禁止半周期访问]*/
  FLASH->ACR &= ACR_HLFCYA_Mask;
  FLASH->ACR |= FLASH_HalfCycleAccess;
}

/*******************************************************************************
* 函数名称: FLASH_PrefetchBufferCmd
* 功能描述: 使能（或禁能）预取缓冲区
* 输入参数: FLASH_PrefetchBuffer：领取缓冲区状态.
*                    这个参数可以是下面的值之一:
*                       - FLASH_PrefetchBuffer_Enable: FLASH 预取缓冲区使能
*                       - FLASH_PrefetchBuffer_Disable: FLASH 预取缓冲区禁能
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FLASH_PrefetchBufferCmd(u32 FLASH_PrefetchBuffer)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FLASH_PREFETCHBUFFER_STATE(FLASH_PrefetchBuffer));
  
  /* Enable or disable the Prefetch Buffer [使能或者禁能预取缓冲区]*/
  FLASH->ACR &= ACR_PRFTBE_Mask;
  FLASH->ACR |= FLASH_PrefetchBuffer;
}

#ifdef _FLASH_PROG
/*******************************************************************************
* 函数名称: FLASH_Unlock
* 功能描述: 解锁FLASH程序擦除控制器.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FLASH_Unlock(void)
{
  /* Authorize the FPEC Access [授权FPEC存取]*/
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}

/*******************************************************************************
* 函数名称: FLASH_Lock
* 功能描述: 锁定FLASH程序擦除控制器.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FLASH_Lock(void)
{
  /* Set the Lock Bit to lock the FPEC and the FCR [置位锁定位来锁定FPEC和FCR]*/
  FLASH->CR |= CR_LOCK_Set;
}

/*******************************************************************************
* 函数名称: FLASH_ErasePage
* 功能描述: 擦除一个FLASH页.
* 输入参数: FLASH_Page：需要擦除的页.
* 输出参数: 无
* 返回参数: FLASH 状态: 返回值可以是: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_ErasePage(u32 Page_Address)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters [检查参数]*/
  assert_param(IS_FLASH_ADDRESS(Page_Address));

  /* Wait for last operation to be completed [等待最后一个操作完成]*/
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  { 
    /* if the previous operation is completed, proceed to erase the page [如果前一个操作已经完成，]*/
    FLASH->CR|= CR_PER_Set;
    FLASH->AR = Page_Address; 
    FLASH->CR|= CR_STRT_Set;
    
    /* Wait for last operation to be completed [等待最后一个操作完成]*/
    status = FLASH_WaitForLastOperation(EraseTimeout);

    if(status != FLASH_BUSY)
    {
      /* if the erase operation is completed, disable the PER Bit [如果擦除操作完成，禁止PER位]*/
      FLASH->CR &= CR_PER_Reset;
    }
  }
  /* Return the Erase Status [返回擦除状态]*/
  return status;
}

/*******************************************************************************
* 函数名称: FLASH_EraseAllPages
* 功能描述: 擦除所有的FLASH页.
* 输入参数: 无
* 输出参数: 无
* 返回参数: FLASH 状态: 返回值可以是: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_EraseAllPages(void)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Wait for last operation to be completed [等待最后一个操作完成]*/
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to erase all pages [如果前一个操作已经完成，擦除所有的页]*/
     FLASH->CR |= CR_MER_Set;
     FLASH->CR |= CR_STRT_Set;
    
    /* Wait for last operation to be completed [等待最后一个操作完成]*/
    status = FLASH_WaitForLastOperation(EraseTimeout);

    if(status != FLASH_BUSY)
    {
      /* if the erase operation is completed, disable the MER Bit [如果擦除操作完成，禁止MER位]*/
      FLASH->CR &= CR_MER_Reset;
    }
  }    
  /* Return the Erase Status [返回擦除状态]*/
  return status;
}

/*******************************************************************************
* 函数名称: FLASH_EraseOptionBytes
* 功能描述: 擦除FLASH选项字节.
* 输入参数: 无
* 输出参数: 无
* 返回参数: FLASH 状态: 返回值可以是: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_EraseOptionBytes(void)
{
  FLASH_Status status = FLASH_COMPLETE;
  
  /* Wait for last operation to be completed [等待最后一个操作完成]*/
  status = FLASH_WaitForLastOperation(EraseTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* Authorize the small information block programming [授权小信息块编程]*/
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    
    /* if the previous operation is completed, proceed to erase the option bytes [如果前一个操作完成，擦除选项字节]*/
    FLASH->CR |= CR_OPTER_Set;
    FLASH->CR |= CR_STRT_Set;

    /* Wait for last operation to be completed [等待最后一个操作完成]*/
    status = FLASH_WaitForLastOperation(EraseTimeout);
    
    if(status == FLASH_COMPLETE)
    {
      /* if the erase operation is completed, disable the OPTER Bit [如果擦除操作完成，禁止OPTER位]*/
      FLASH->CR &= CR_OPTER_Reset;
       
      /* Enable the Option Bytes Programming operation [使能字节编程操作选项]*/
      FLASH->CR |= CR_OPTPG_Set;

      /* Enable the readout access [使能读出访问]*/
      OB->RDP= RDP_Key; 

      /* Wait for last operation to be completed [等待最后一个操作完成]*/
      status = FLASH_WaitForLastOperation(ProgramTimeout);
 
      if(status != FLASH_BUSY)
      {
        /* if the program operation is completed, disable the OPTPG Bit [如果编程操作完成，禁止OPTPG位]*/
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else
    {
      if (status != FLASH_BUSY)
      {
        /* Disable the OPTPG Bit [禁止OPTPG位]*/
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }  
  }
  /* Return the erase status [返回擦除状态]*/
  return status;
}

/*******************************************************************************
* 函数名称: FLASH_ProgramWord
* 功能描述: 在特定地址编程一个字.
* 输入参数: (1)Address：将要编程的地址.
*           (2)Data：指定被编程的数据.
* 输出参数: 无
* 返回参数: FLASH状态:这个返回值可以是: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT. 
*******************************************************************************/
FLASH_Status FLASH_ProgramWord(u32 Address, u32 Data)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters [检查参数]*/
  assert_param(IS_FLASH_ADDRESS(Address));

  /* Wait for last operation to be completed [等待最后一个操作完成]*/
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new first 
    half word [如果前一个操作完成，编程新的第一个半字]*/
    FLASH->CR |= CR_PG_Set;
  
    *(vu16*)Address = (u16)Data;

    /* Wait for last operation to be completed [等待最后一个操作完成]*/
    status = FLASH_WaitForLastOperation(ProgramTimeout);
 
    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new second 
      half word [如果前一个操作完成，编程新的第二个半字]*/
      *(vu16*)(Address + 2) = Data >> 16;
    
      /* Wait for last operation to be completed [等待最后一个操作完成]*/
      status = FLASH_WaitForLastOperation(ProgramTimeout);
        
      if(status != FLASH_BUSY)
      {
        /* Disable the PG Bit [禁止PG位]*/
        FLASH->CR &= CR_PG_Reset;
      }
    }
    else
    {
      if (status != FLASH_BUSY)
      {
        /* Disable the PG Bit [禁止PG位]*/
        FLASH->CR &= CR_PG_Reset;
      }
     }
  }
  /* Return the Program Status[返回编程状态] */
  return status;
}

/*******************************************************************************
* 函数名称: FLASH_ProgramHalfWord
* 功能描述: 在特定地址编程一个半字.
* 输入参数: (1)Address：被编程的地址.
*           (2)Data：指定被编程的半字数据.
* 输出参数: 无
* 返回参数: FLASH状态:这个返回值可以是: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT. 
*******************************************************************************/
FLASH_Status FLASH_ProgramHalfWord(u32 Address, u16 Data)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters [检查参数]*/
  assert_param(IS_FLASH_ADDRESS(Address));

  /* Wait for last operation to be completed [等待最后一个操作完成]*/
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new data [如果前一个操作完成，编程新的数据]*/
    FLASH->CR |= CR_PG_Set;
  
    *(vu16*)Address = Data;
    /* Wait for last operation to be completed [等待最后一个操作完成]*/
    status = FLASH_WaitForLastOperation(ProgramTimeout);

    if(status != FLASH_BUSY)
    {
      /* if the program operation is completed, disable the PG Bit [如果编程操作完成，禁止PG位]*/
      FLASH->CR &= CR_PG_Reset;
    }
  } 
  /* Return the Program Status [返回编程状态]*/
  return status;
}

/*******************************************************************************
* 函数名称: FLASH_ProgramOptionByteData
* 功能描述: 在指定的选项字节数据地址上编写一个半字.
* 输入参数: (1)Address：被编程的地址，该参数可为：0x1FFFF804或0x1FFFF806. 
*           (2)Data：被编写的数据.
* 输出参数: 无
* 返回参数: FLASH状态:这个返回值可以是: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT. 
*******************************************************************************/
FLASH_Status FLASH_ProgramOptionByteData(u32 Address, u8 Data)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters [检查参数]*/
  assert_param(IS_OB_DATA_ADDRESS(Address));

  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* Authorize the small information block programming [授权小信息块编程]*/
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;

    /* Enables the Option Bytes Programming operation [使能字节编程操作选项]*/
    FLASH->CR |= CR_OPTPG_Set; 
    *(vu16*)Address = Data;
    
    /* Wait for last operation to be completed [等待最后一个操作完成]*/
    status = FLASH_WaitForLastOperation(ProgramTimeout);

    if(status != FLASH_BUSY)
    {
      /* if the program operation is completed, disable the OPTPG Bit [如果编程操作完成，禁止OPTPG位]*/
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }    
  /* Return the Option Byte Data Program Status [返回选项字节数据的编程状态]*/
  return status;      
}

/*******************************************************************************
* 函数名称: FLASH_EnableWriteProtection
* 功能描述: 对需要写保护的页进行写保护
* 输入参数: FLASH_Pages：需要写保护的页的地址。
*                 这个参数可以是:
*                    - 对于STM32F10Xxx 中密度驱动器 (FLASH 页面大小等于1 KB)
*                       - 这个值在FLASH页写保护0到3 和FLASH页写保护124到127之间
*                    - 对于STM32F10Xxx 高密度驱动器 (FLASH页面大小等于2 KB) 
*                       - 这个值在 FLASH_WRProt_Pages0到1 和
*                         FLASH页写保护60到61或者FLASH页写保护62到255 之间
*                       - 所有页的写保护
* 输出参数: 无
* 返回参数: FLASH状态:这个返回值可以是: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_EnableWriteProtection(u32 FLASH_Pages)
{
  u16 WRP0_Data = 0xFFFF, WRP1_Data = 0xFFFF, WRP2_Data = 0xFFFF, WRP3_Data = 0xFFFF;
  
  FLASH_Status status = FLASH_COMPLETE;
  
  /* Check the parameters [检查参数]*/
  assert_param(IS_FLASH_WRPROT_PAGE(FLASH_Pages));
  
  FLASH_Pages = (u32)(~FLASH_Pages);
  WRP0_Data = (vu16)(FLASH_Pages & WRP0_Mask);
  WRP1_Data = (vu16)((FLASH_Pages & WRP1_Mask) >> 8);
  WRP2_Data = (vu16)((FLASH_Pages & WRP2_Mask) >> 16);
  WRP3_Data = (vu16)((FLASH_Pages & WRP3_Mask) >> 24);
  
  /* Wait for last operation to be completed [等待最后一个操作完成]*/
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* Authorizes the small information block programming [授权小信息块编程]*/
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    FLASH->CR |= CR_OPTPG_Set;

    if(WRP0_Data != 0xFF)
    {
      OB->WRP0 = WRP0_Data;
      
      /* Wait for last operation to be completed [等待最后一个操作完成]*/
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    if((status == FLASH_COMPLETE) && (WRP1_Data != 0xFF))
    {
      OB->WRP1 = WRP1_Data;
      
      /* Wait for last operation to be completed [等待最后一个操作完成]*/
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }

    if((status == FLASH_COMPLETE) && (WRP2_Data != 0xFF))
    {
      OB->WRP2 = WRP2_Data;
      
      /* Wait for last operation to be completed [等待最后一个操作完成]*/
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    
    if((status == FLASH_COMPLETE)&& (WRP3_Data != 0xFF))
    {
      OB->WRP3 = WRP3_Data;
     
      /* Wait for last operation to be completed [等待最后一个操作完成]*/
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
          
    if(status != FLASH_BUSY)
    {
      /* if the program operation is completed, disable the OPTPG Bit [如果编程操作完成，禁止OPTPG位]*/
      FLASH->CR &= CR_OPTPG_Reset;
    }
  } 
  /* Return the write protection operation Status [返回写保护操作状态]*/
  return status;       
}

/*******************************************************************************
* 函数名称: FLASH_ReadOutProtection
* 功能描述: 使能（或禁能）读出保护.
*                  如果在调用这个函数之前使用者已经编程了其它的选项字节，在这个
*                  函数擦除所有的选项字节以后他必须重新编程那些选项字节 .
* 输入参数: NewState：读出保护的新状态。这个参数可被设置为：ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: FLASH状态:这个返回值可以是: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_ReadOutProtection(FunctionalState NewState)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  status = FLASH_WaitForLastOperation(EraseTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* Authorizes the small information block programming [授权小信息块编程]*/
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;

    FLASH->CR |= CR_OPTER_Set;
    FLASH->CR |= CR_STRT_Set;

    /* Wait for last operation to be completed [等待最后一个操作完成]*/
    status = FLASH_WaitForLastOperation(EraseTimeout);

    if(status == FLASH_COMPLETE)
    {
      /* if the erase operation is completed, disable the OPTER Bit [如果擦除操作完成，禁止OPTER位]*/
      FLASH->CR &= CR_OPTER_Reset;

      /* Enable the Option Bytes Programming operation [使能字节编程操作选项]*/
      FLASH->CR |= CR_OPTPG_Set; 

      if(NewState != DISABLE)
      {
        OB->RDP = 0x00;
      }
      else
      {
        OB->RDP = RDP_Key;  
      }

      /* Wait for last operation to be completed [等待最后一个操作完成]*/
      status = FLASH_WaitForLastOperation(EraseTimeout); 
    
      if(status != FLASH_BUSY)
      {
        /* if the program operation is completed, disable the OPTPG Bit [如果编程操作完成，禁止OPTPG位]*/
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else 
    {
      if(status != FLASH_BUSY)
      {
        /* Disable the OPTER Bit [禁止OPTER位]*/
        FLASH->CR &= CR_OPTER_Reset;
      }
    }
  }
  /* Return the protection operation Status [返回写保护操作状态]*/
  return status;      
}
    
/*******************************************************************************
* 函数名称: FLASH_UserOptionByteConfig
* 功能描述: 编写FLASH用户选项字节：IWDG_SW/RST_STOP/RST_STDBY.
* 输入参数: (1)OB_IWDG：选择IWDG模式。
*                     这个参数可以是下面的值之一:
*                     - OB_IWDG_SW: 软件IWDG被选择
*                     - OB_IWDG_HW: 硬件IWDG被选择
*           (2)OB_STOP：当进入STOP模式时重置事件。
*                     这个参数可以是下面的值之一:
*                     - OB_STOP_NoRST: 当进入STOP模式时没有产生复位
*                     - OB_STOP_RST: 当进入STOP模式时产生复位
*           (3)OB_STDBY：当进入待命模式时重置事件。
*                    这个参数可以是下面的值之一:
*                     - OB_STDBY_NoRST: 当进入STOP模式时产生复位
*                     - OB_STDBY_RST: 当进入STANDBY模式时产生复位
* 输出参数: 无
* 返回参数: FLASH状态:这个返回值可以是: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_UserOptionByteConfig(u16 OB_IWDG, u16 OB_STOP, u16 OB_STDBY)
{
  FLASH_Status status = FLASH_COMPLETE; 

  /* Check the parameters [检查参数]*/
  assert_param(IS_OB_IWDG_SOURCE(OB_IWDG));
  assert_param(IS_OB_STOP_SOURCE(OB_STOP));
  assert_param(IS_OB_STDBY_SOURCE(OB_STDBY));

  /* Authorize the small information block programming [授权小信息块编程]*/
  FLASH->OPTKEYR = FLASH_KEY1;
  FLASH->OPTKEYR = FLASH_KEY2;
  
  /* Wait for last operation to be completed [等待最后一个操作完成]*/
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {  
    /* Enable the Option Bytes Programming operation [使能字节编程操作选项]*/
    FLASH->CR |= CR_OPTPG_Set; 
           
    OB->USER = ( OB_IWDG | OB_STOP |OB_STDBY) | (u16)0xF8; 
  
    /* Wait for last operation to be completed [等待最后一个操作完成]*/
    status = FLASH_WaitForLastOperation(ProgramTimeout);

    if(status != FLASH_BUSY)
    {
      /* if the program operation is completed, disable the OPTPG Bit [如果编程操作完成，禁止OPTPG位]*/
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }    
  /* Return the Option Byte program Status [返回选项字节编程状态]*/
  return status;
}

/*******************************************************************************
* 函数名称: FLASH_GetUserOptionByte
* 功能描述: 返回FLASH用户选项字节值.
* 输入参数: 无
* 输出参数: 无
* 返回参数: FLASH用户选项字节值：:IWDG_SW(Bit0), RST_STOP(Bit1)和RST_STDBY(Bit2).
*******************************************************************************/
u32 FLASH_GetUserOptionByte(void)
{
  /* Return the User Option Byte */
  return (u32)(FLASH->OBR >> 2);
}

/*******************************************************************************
* 函数名称: FLASH_GetWriteProtectionOptionByte
* 功能描述: 返回FLASH写保护选项字节寄存器值.
* 输入参数: 无
* 输出参数: 无
* 返回参数: FLASH写保护选项字节寄存器值
*******************************************************************************/
u32 FLASH_GetWriteProtectionOptionByte(void)
{
  /* Return the Flash write protection Register value [返回FLASH写保护寄存器值]*/
  return (u32)(FLASH->WRPR);
}

/*******************************************************************************
* 函数名称: FLASH_GetReadOutProtectionStatus
* 功能描述: 检查FLASH读取保护状态是否被置位了.
* 输入参数: 无
* 输出参数: 无
* 返回参数: FLASH读出保护状态（SET或RESET）
*******************************************************************************/
FlagStatus FLASH_GetReadOutProtectionStatus(void)
{
  FlagStatus readoutstatus = RESET;

  if ((FLASH->OBR & RDPRT_Mask) != (u32)RESET)
  {
    readoutstatus = SET;
  }
  else
  {
    readoutstatus = RESET;
  }
  return readoutstatus;
}

/*******************************************************************************
* 函数名称: FLASH_GetPrefetchBufferStatus
* 功能描述: 检查FLASH预取缓冲区状态是否被置位.
* 输入参数: 无
* 输出参数: 无
* 返回参数: FLASH预取缓冲区状态（SET或RESET）.
*******************************************************************************/
FlagStatus FLASH_GetPrefetchBufferStatus(void)
{
  FlagStatus bitstatus = RESET;
  
  if ((FLASH->ACR & ACR_PRFTBS_Mask) != (u32)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* Return the new state of FLASH Prefetch Buffer Status (SET or RESET) [返回flash预取缓冲状态的新状态(SET 或 RESET)]*/
  return bitstatus; 
}

/*******************************************************************************
* 函数名称: FLASH_ITConfig
* 功能描述: 使能（或禁能）特定的FLASH中断.
* 输入参数: FLASH_IT：被使能（或禁能）的FLASH中断源。
*                   这个参数可以是下面值的任意组合:
*                       - FLASH_IT_ERROR: FLASH 错误中断
*                       - FLASH_IT_EOP: FLASH 完成操作中断
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FLASH_ITConfig(u16 FLASH_IT, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FLASH_IT(FLASH_IT)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if(NewState != DISABLE)
  {
    /* Enable the interrupt sources [使能中断源]*/
    FLASH->CR |= FLASH_IT;
  }
  else
  {
    /* Disable the interrupt sources [禁止中断源]*/
    FLASH->CR &= ~(u32)FLASH_IT;
  }
}

/*******************************************************************************
* 函数名称: FLASH_GetFlagStatus
* 功能描述: 检查特定的FLASH标志是否被置位.
* 输入参数: FLASH_FLAG：需要检查的标志。
*                     这个参数可以是下面的值之一:
*                    - FLASH_FLAG_BSY: FLASH忙标志           
*                    - FLASH_FLAG_PGERR: FLASH程序错误标志      
*                    - FLASH_FLAG_WRPRTERR: FLASH写保护错误标志     
*                    - FLASH_FLAG_EOP: FLASH操作结束标志          
*                    - FLASH_FLAG_OPTERR:  FLASH选项字节错误标志    
* 输出参数: 无
* 返回参数: FLASH标志寄存器新状态（置位或复位）。
*******************************************************************************/
FlagStatus FLASH_GetFlagStatus(u16 FLASH_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [检查参数]*/
  assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG)) ;

  if(FLASH_FLAG == FLASH_FLAG_OPTERR) 
  {
    if((FLASH->OBR & FLASH_FLAG_OPTERR) != (u32)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  else
  {
   if((FLASH->SR & FLASH_FLAG) != (u32)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  /* Return the new state of FLASH_FLAG (SET or RESET)[FLASH标志寄存器新状态(置位或复位)] */
  return bitstatus;
}

/*******************************************************************************
* 函数名称: FLASH_ClearFlag
* 功能描述: 清除FLASH挂起标志.
* 输入参数: FLASH_FLAG：需要清除的标志。
*                    这个参数可以是下面值的任意组合:
*                    - FLASH_FLAG_BSY: FLASH忙的标志           
*                    - FLASH_FLAG_PGERR: FLASH程序错误的标志      
*                    - FLASH_FLAG_WRPRTERR: FLASH页写保护错误的标志     
*                    - FLASH_FLAG_EOP: FLASH操作结束的标志          
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FLASH_ClearFlag(u16 FLASH_FLAG)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;
  
  /* Clear the flags [清除标志]*/
  FLASH->SR = FLASH_FLAG;
}

/*******************************************************************************
* 函数名称: FLASH_GetStatus
* 功能描述: 返回FLASH状态.
* 输入参数: 无
* 输出参数: 无
* 返回参数: FLASH状态:这个返回值可以是: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP or FLASH_COMPLETE
*******************************************************************************/
FLASH_Status FLASH_GetStatus(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;
  
  if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) 
  {
    flashstatus = FLASH_BUSY;
  }
  else 
  {  
    if(FLASH->SR & FLASH_FLAG_PGERR)
    { 
      flashstatus = FLASH_ERROR_PG;
    }
    else 
    {
      if(FLASH->SR & FLASH_FLAG_WRPRTERR)
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* Return the Flash Status [返回FLASH状态]*/
  return flashstatus;
}

/*******************************************************************************
* 函数名称: FLASH_WaitForLastOperation
* 功能描述: 等待一个Flash操作来结束或一个TIMEOUT事件发生.
* 输入参数: - Timeout: FLASH编程超时时间
* 输出参数: 无
* 返回参数: FLASH状态:这个返回值可以是: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_WaitForLastOperation(u32 Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* Check for the Flash Status [检查FLASH状态]*/
  status = FLASH_GetStatus();

  /* Wait for a Flash operation to complete or a TIMEOUT to occur [等待FLASH操作完成或者发生超时]*/
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    delay();
    status = FLASH_GetStatus();
    Timeout--;
  }

  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }

  /* Return the operation status [返回操作状态]*/
  return status;
}

/*******************************************************************************
* 函数名称: delay
* 功能描述: 插入一个延时.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
static void delay(void)
{
  vu32 i = 0;

  for(i = 0xFF; i != 0; i--)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
