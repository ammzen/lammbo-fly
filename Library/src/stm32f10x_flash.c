/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_flash.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������FLASH�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_flash.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
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

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ��-----------------------------------------------*/
#ifdef _FLASH_PROG
static void delay(void);
#endif

/* ���ú��� ---------------------------------------------------------*/
/*******************************************************************************
* ��������: FLASH_SetLatency
* ��������: ���ô�����ʱֵ.
* �������: FLASH_Latencyָ����FLASH��ʱֵ��
*                    ������������������ֵ֮һ:
*                       - FLASH_Latency_0: 0����ʱ����
*                       - FLASH_Latency_1: 1����ʱ����
*                       - FLASH_Latency_2: 2����ʱ����
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FLASH_SetLatency(u32 FLASH_Latency)
{
  /* Check the parameters [������]*/
  assert_param(IS_FLASH_LATENCY(FLASH_Latency));
  
  /* Sets the Latency value [������ʱֵ]*/
  FLASH->ACR &= ACR_LATENCY_Mask;
  FLASH->ACR |= FLASH_Latency;
}

/*******************************************************************************
* ��������: FLASH_HalfCycleAccessCmd
* ��������: ʹ�ܣ�����ܣ�������Flash����
* �������: FLASH_HalfCycleAccess��FLASH������ģʽ��
*                    ������������������ֵ֮һ:
*                       - FLASH_HalfCycleAccess_Enable: ʹ�ܰ����ڷ���
*                       - FLASH_HalfCycleAccess_Disable: ���ܰ����ڷ���
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FLASH_HalfCycleAccessCmd(u32 FLASH_HalfCycleAccess)
{
  /* Check the parameters [������]*/
  assert_param(IS_FLASH_HALFCYCLEACCESS_STATE(FLASH_HalfCycleAccess));
  
  /* Enable or disable the Half cycle access [ʹ�ܻ��߽�ֹ�����ڷ���]*/
  FLASH->ACR &= ACR_HLFCYA_Mask;
  FLASH->ACR |= FLASH_HalfCycleAccess;
}

/*******************************************************************************
* ��������: FLASH_PrefetchBufferCmd
* ��������: ʹ�ܣ�����ܣ�Ԥȡ������
* �������: FLASH_PrefetchBuffer����ȡ������״̬.
*                    ������������������ֵ֮һ:
*                       - FLASH_PrefetchBuffer_Enable: FLASH Ԥȡ������ʹ��
*                       - FLASH_PrefetchBuffer_Disable: FLASH Ԥȡ����������
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FLASH_PrefetchBufferCmd(u32 FLASH_PrefetchBuffer)
{
  /* Check the parameters [������]*/
  assert_param(IS_FLASH_PREFETCHBUFFER_STATE(FLASH_PrefetchBuffer));
  
  /* Enable or disable the Prefetch Buffer [ʹ�ܻ��߽���Ԥȡ������]*/
  FLASH->ACR &= ACR_PRFTBE_Mask;
  FLASH->ACR |= FLASH_PrefetchBuffer;
}

#ifdef _FLASH_PROG
/*******************************************************************************
* ��������: FLASH_Unlock
* ��������: ����FLASH�������������.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FLASH_Unlock(void)
{
  /* Authorize the FPEC Access [��ȨFPEC��ȡ]*/
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}

/*******************************************************************************
* ��������: FLASH_Lock
* ��������: ����FLASH�������������.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FLASH_Lock(void)
{
  /* Set the Lock Bit to lock the FPEC and the FCR [��λ����λ������FPEC��FCR]*/
  FLASH->CR |= CR_LOCK_Set;
}

/*******************************************************************************
* ��������: FLASH_ErasePage
* ��������: ����һ��FLASHҳ.
* �������: FLASH_Page����Ҫ������ҳ.
* �������: ��
* ���ز���: FLASH ״̬: ����ֵ������: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_ErasePage(u32 Page_Address)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters [������]*/
  assert_param(IS_FLASH_ADDRESS(Page_Address));

  /* Wait for last operation to be completed [�ȴ����һ���������]*/
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  { 
    /* if the previous operation is completed, proceed to erase the page [���ǰһ�������Ѿ���ɣ�]*/
    FLASH->CR|= CR_PER_Set;
    FLASH->AR = Page_Address; 
    FLASH->CR|= CR_STRT_Set;
    
    /* Wait for last operation to be completed [�ȴ����һ���������]*/
    status = FLASH_WaitForLastOperation(EraseTimeout);

    if(status != FLASH_BUSY)
    {
      /* if the erase operation is completed, disable the PER Bit [�������������ɣ���ֹPERλ]*/
      FLASH->CR &= CR_PER_Reset;
    }
  }
  /* Return the Erase Status [���ز���״̬]*/
  return status;
}

/*******************************************************************************
* ��������: FLASH_EraseAllPages
* ��������: �������е�FLASHҳ.
* �������: ��
* �������: ��
* ���ز���: FLASH ״̬: ����ֵ������: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_EraseAllPages(void)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Wait for last operation to be completed [�ȴ����һ���������]*/
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to erase all pages [���ǰһ�������Ѿ���ɣ��������е�ҳ]*/
     FLASH->CR |= CR_MER_Set;
     FLASH->CR |= CR_STRT_Set;
    
    /* Wait for last operation to be completed [�ȴ����һ���������]*/
    status = FLASH_WaitForLastOperation(EraseTimeout);

    if(status != FLASH_BUSY)
    {
      /* if the erase operation is completed, disable the MER Bit [�������������ɣ���ֹMERλ]*/
      FLASH->CR &= CR_MER_Reset;
    }
  }    
  /* Return the Erase Status [���ز���״̬]*/
  return status;
}

/*******************************************************************************
* ��������: FLASH_EraseOptionBytes
* ��������: ����FLASHѡ���ֽ�.
* �������: ��
* �������: ��
* ���ز���: FLASH ״̬: ����ֵ������: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_EraseOptionBytes(void)
{
  FLASH_Status status = FLASH_COMPLETE;
  
  /* Wait for last operation to be completed [�ȴ����һ���������]*/
  status = FLASH_WaitForLastOperation(EraseTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* Authorize the small information block programming [��ȨС��Ϣ����]*/
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    
    /* if the previous operation is completed, proceed to erase the option bytes [���ǰһ��������ɣ�����ѡ���ֽ�]*/
    FLASH->CR |= CR_OPTER_Set;
    FLASH->CR |= CR_STRT_Set;

    /* Wait for last operation to be completed [�ȴ����һ���������]*/
    status = FLASH_WaitForLastOperation(EraseTimeout);
    
    if(status == FLASH_COMPLETE)
    {
      /* if the erase operation is completed, disable the OPTER Bit [�������������ɣ���ֹOPTERλ]*/
      FLASH->CR &= CR_OPTER_Reset;
       
      /* Enable the Option Bytes Programming operation [ʹ���ֽڱ�̲���ѡ��]*/
      FLASH->CR |= CR_OPTPG_Set;

      /* Enable the readout access [ʹ�ܶ�������]*/
      OB->RDP= RDP_Key; 

      /* Wait for last operation to be completed [�ȴ����һ���������]*/
      status = FLASH_WaitForLastOperation(ProgramTimeout);
 
      if(status != FLASH_BUSY)
      {
        /* if the program operation is completed, disable the OPTPG Bit [�����̲�����ɣ���ֹOPTPGλ]*/
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else
    {
      if (status != FLASH_BUSY)
      {
        /* Disable the OPTPG Bit [��ֹOPTPGλ]*/
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }  
  }
  /* Return the erase status [���ز���״̬]*/
  return status;
}

/*******************************************************************************
* ��������: FLASH_ProgramWord
* ��������: ���ض���ַ���һ����.
* �������: (1)Address����Ҫ��̵ĵ�ַ.
*           (2)Data��ָ������̵�����.
* �������: ��
* ���ز���: FLASH״̬:�������ֵ������: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT. 
*******************************************************************************/
FLASH_Status FLASH_ProgramWord(u32 Address, u32 Data)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters [������]*/
  assert_param(IS_FLASH_ADDRESS(Address));

  /* Wait for last operation to be completed [�ȴ����һ���������]*/
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new first 
    half word [���ǰһ��������ɣ�����µĵ�һ������]*/
    FLASH->CR |= CR_PG_Set;
  
    *(vu16*)Address = (u16)Data;

    /* Wait for last operation to be completed [�ȴ����һ���������]*/
    status = FLASH_WaitForLastOperation(ProgramTimeout);
 
    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new second 
      half word [���ǰһ��������ɣ�����µĵڶ�������]*/
      *(vu16*)(Address + 2) = Data >> 16;
    
      /* Wait for last operation to be completed [�ȴ����һ���������]*/
      status = FLASH_WaitForLastOperation(ProgramTimeout);
        
      if(status != FLASH_BUSY)
      {
        /* Disable the PG Bit [��ֹPGλ]*/
        FLASH->CR &= CR_PG_Reset;
      }
    }
    else
    {
      if (status != FLASH_BUSY)
      {
        /* Disable the PG Bit [��ֹPGλ]*/
        FLASH->CR &= CR_PG_Reset;
      }
     }
  }
  /* Return the Program Status[���ر��״̬] */
  return status;
}

/*******************************************************************************
* ��������: FLASH_ProgramHalfWord
* ��������: ���ض���ַ���һ������.
* �������: (1)Address������̵ĵ�ַ.
*           (2)Data��ָ������̵İ�������.
* �������: ��
* ���ز���: FLASH״̬:�������ֵ������: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT. 
*******************************************************************************/
FLASH_Status FLASH_ProgramHalfWord(u32 Address, u16 Data)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters [������]*/
  assert_param(IS_FLASH_ADDRESS(Address));

  /* Wait for last operation to be completed [�ȴ����һ���������]*/
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new data [���ǰһ��������ɣ�����µ�����]*/
    FLASH->CR |= CR_PG_Set;
  
    *(vu16*)Address = Data;
    /* Wait for last operation to be completed [�ȴ����һ���������]*/
    status = FLASH_WaitForLastOperation(ProgramTimeout);

    if(status != FLASH_BUSY)
    {
      /* if the program operation is completed, disable the PG Bit [�����̲�����ɣ���ֹPGλ]*/
      FLASH->CR &= CR_PG_Reset;
    }
  } 
  /* Return the Program Status [���ر��״̬]*/
  return status;
}

/*******************************************************************************
* ��������: FLASH_ProgramOptionByteData
* ��������: ��ָ����ѡ���ֽ����ݵ�ַ�ϱ�дһ������.
* �������: (1)Address������̵ĵ�ַ���ò�����Ϊ��0x1FFFF804��0x1FFFF806. 
*           (2)Data������д������.
* �������: ��
* ���ز���: FLASH״̬:�������ֵ������: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT. 
*******************************************************************************/
FLASH_Status FLASH_ProgramOptionByteData(u32 Address, u8 Data)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters [������]*/
  assert_param(IS_OB_DATA_ADDRESS(Address));

  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* Authorize the small information block programming [��ȨС��Ϣ����]*/
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;

    /* Enables the Option Bytes Programming operation [ʹ���ֽڱ�̲���ѡ��]*/
    FLASH->CR |= CR_OPTPG_Set; 
    *(vu16*)Address = Data;
    
    /* Wait for last operation to be completed [�ȴ����һ���������]*/
    status = FLASH_WaitForLastOperation(ProgramTimeout);

    if(status != FLASH_BUSY)
    {
      /* if the program operation is completed, disable the OPTPG Bit [�����̲�����ɣ���ֹOPTPGλ]*/
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }    
  /* Return the Option Byte Data Program Status [����ѡ���ֽ����ݵı��״̬]*/
  return status;      
}

/*******************************************************************************
* ��������: FLASH_EnableWriteProtection
* ��������: ����Ҫд������ҳ����д����
* �������: FLASH_Pages����Ҫд������ҳ�ĵ�ַ��
*                 �������������:
*                    - ����STM32F10Xxx ���ܶ������� (FLASH ҳ���С����1 KB)
*                       - ���ֵ��FLASHҳд����0��3 ��FLASHҳд����124��127֮��
*                    - ����STM32F10Xxx ���ܶ������� (FLASHҳ���С����2 KB) 
*                       - ���ֵ�� FLASH_WRProt_Pages0��1 ��
*                         FLASHҳд����60��61����FLASHҳд����62��255 ֮��
*                       - ����ҳ��д����
* �������: ��
* ���ز���: FLASH״̬:�������ֵ������: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_EnableWriteProtection(u32 FLASH_Pages)
{
  u16 WRP0_Data = 0xFFFF, WRP1_Data = 0xFFFF, WRP2_Data = 0xFFFF, WRP3_Data = 0xFFFF;
  
  FLASH_Status status = FLASH_COMPLETE;
  
  /* Check the parameters [������]*/
  assert_param(IS_FLASH_WRPROT_PAGE(FLASH_Pages));
  
  FLASH_Pages = (u32)(~FLASH_Pages);
  WRP0_Data = (vu16)(FLASH_Pages & WRP0_Mask);
  WRP1_Data = (vu16)((FLASH_Pages & WRP1_Mask) >> 8);
  WRP2_Data = (vu16)((FLASH_Pages & WRP2_Mask) >> 16);
  WRP3_Data = (vu16)((FLASH_Pages & WRP3_Mask) >> 24);
  
  /* Wait for last operation to be completed [�ȴ����һ���������]*/
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* Authorizes the small information block programming [��ȨС��Ϣ����]*/
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    FLASH->CR |= CR_OPTPG_Set;

    if(WRP0_Data != 0xFF)
    {
      OB->WRP0 = WRP0_Data;
      
      /* Wait for last operation to be completed [�ȴ����һ���������]*/
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    if((status == FLASH_COMPLETE) && (WRP1_Data != 0xFF))
    {
      OB->WRP1 = WRP1_Data;
      
      /* Wait for last operation to be completed [�ȴ����һ���������]*/
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }

    if((status == FLASH_COMPLETE) && (WRP2_Data != 0xFF))
    {
      OB->WRP2 = WRP2_Data;
      
      /* Wait for last operation to be completed [�ȴ����һ���������]*/
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    
    if((status == FLASH_COMPLETE)&& (WRP3_Data != 0xFF))
    {
      OB->WRP3 = WRP3_Data;
     
      /* Wait for last operation to be completed [�ȴ����һ���������]*/
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
          
    if(status != FLASH_BUSY)
    {
      /* if the program operation is completed, disable the OPTPG Bit [�����̲�����ɣ���ֹOPTPGλ]*/
      FLASH->CR &= CR_OPTPG_Reset;
    }
  } 
  /* Return the write protection operation Status [����д��������״̬]*/
  return status;       
}

/*******************************************************************************
* ��������: FLASH_ReadOutProtection
* ��������: ʹ�ܣ�����ܣ���������.
*                  ����ڵ����������֮ǰʹ�����Ѿ������������ѡ���ֽڣ������
*                  �����������е�ѡ���ֽ��Ժ����������±����Щѡ���ֽ� .
* �������: NewState��������������״̬����������ɱ�����Ϊ��ENABLE��DISABLE.
* �������: ��
* ���ز���: FLASH״̬:�������ֵ������: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_ReadOutProtection(FunctionalState NewState)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  status = FLASH_WaitForLastOperation(EraseTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* Authorizes the small information block programming [��ȨС��Ϣ����]*/
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;

    FLASH->CR |= CR_OPTER_Set;
    FLASH->CR |= CR_STRT_Set;

    /* Wait for last operation to be completed [�ȴ����һ���������]*/
    status = FLASH_WaitForLastOperation(EraseTimeout);

    if(status == FLASH_COMPLETE)
    {
      /* if the erase operation is completed, disable the OPTER Bit [�������������ɣ���ֹOPTERλ]*/
      FLASH->CR &= CR_OPTER_Reset;

      /* Enable the Option Bytes Programming operation [ʹ���ֽڱ�̲���ѡ��]*/
      FLASH->CR |= CR_OPTPG_Set; 

      if(NewState != DISABLE)
      {
        OB->RDP = 0x00;
      }
      else
      {
        OB->RDP = RDP_Key;  
      }

      /* Wait for last operation to be completed [�ȴ����һ���������]*/
      status = FLASH_WaitForLastOperation(EraseTimeout); 
    
      if(status != FLASH_BUSY)
      {
        /* if the program operation is completed, disable the OPTPG Bit [�����̲�����ɣ���ֹOPTPGλ]*/
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else 
    {
      if(status != FLASH_BUSY)
      {
        /* Disable the OPTER Bit [��ֹOPTERλ]*/
        FLASH->CR &= CR_OPTER_Reset;
      }
    }
  }
  /* Return the protection operation Status [����д��������״̬]*/
  return status;      
}
    
/*******************************************************************************
* ��������: FLASH_UserOptionByteConfig
* ��������: ��дFLASH�û�ѡ���ֽڣ�IWDG_SW/RST_STOP/RST_STDBY.
* �������: (1)OB_IWDG��ѡ��IWDGģʽ��
*                     ������������������ֵ֮һ:
*                     - OB_IWDG_SW: ���IWDG��ѡ��
*                     - OB_IWDG_HW: Ӳ��IWDG��ѡ��
*           (2)OB_STOP��������STOPģʽʱ�����¼���
*                     ������������������ֵ֮һ:
*                     - OB_STOP_NoRST: ������STOPģʽʱû�в�����λ
*                     - OB_STOP_RST: ������STOPģʽʱ������λ
*           (3)OB_STDBY�����������ģʽʱ�����¼���
*                    ������������������ֵ֮һ:
*                     - OB_STDBY_NoRST: ������STOPģʽʱ������λ
*                     - OB_STDBY_RST: ������STANDBYģʽʱ������λ
* �������: ��
* ���ز���: FLASH״̬:�������ֵ������: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_UserOptionByteConfig(u16 OB_IWDG, u16 OB_STOP, u16 OB_STDBY)
{
  FLASH_Status status = FLASH_COMPLETE; 

  /* Check the parameters [������]*/
  assert_param(IS_OB_IWDG_SOURCE(OB_IWDG));
  assert_param(IS_OB_STOP_SOURCE(OB_STOP));
  assert_param(IS_OB_STDBY_SOURCE(OB_STDBY));

  /* Authorize the small information block programming [��ȨС��Ϣ����]*/
  FLASH->OPTKEYR = FLASH_KEY1;
  FLASH->OPTKEYR = FLASH_KEY2;
  
  /* Wait for last operation to be completed [�ȴ����һ���������]*/
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {  
    /* Enable the Option Bytes Programming operation [ʹ���ֽڱ�̲���ѡ��]*/
    FLASH->CR |= CR_OPTPG_Set; 
           
    OB->USER = ( OB_IWDG | OB_STOP |OB_STDBY) | (u16)0xF8; 
  
    /* Wait for last operation to be completed [�ȴ����һ���������]*/
    status = FLASH_WaitForLastOperation(ProgramTimeout);

    if(status != FLASH_BUSY)
    {
      /* if the program operation is completed, disable the OPTPG Bit [�����̲�����ɣ���ֹOPTPGλ]*/
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }    
  /* Return the Option Byte program Status [����ѡ���ֽڱ��״̬]*/
  return status;
}

/*******************************************************************************
* ��������: FLASH_GetUserOptionByte
* ��������: ����FLASH�û�ѡ���ֽ�ֵ.
* �������: ��
* �������: ��
* ���ز���: FLASH�û�ѡ���ֽ�ֵ��:IWDG_SW(Bit0), RST_STOP(Bit1)��RST_STDBY(Bit2).
*******************************************************************************/
u32 FLASH_GetUserOptionByte(void)
{
  /* Return the User Option Byte */
  return (u32)(FLASH->OBR >> 2);
}

/*******************************************************************************
* ��������: FLASH_GetWriteProtectionOptionByte
* ��������: ����FLASHд����ѡ���ֽڼĴ���ֵ.
* �������: ��
* �������: ��
* ���ز���: FLASHд����ѡ���ֽڼĴ���ֵ
*******************************************************************************/
u32 FLASH_GetWriteProtectionOptionByte(void)
{
  /* Return the Flash write protection Register value [����FLASHд�����Ĵ���ֵ]*/
  return (u32)(FLASH->WRPR);
}

/*******************************************************************************
* ��������: FLASH_GetReadOutProtectionStatus
* ��������: ���FLASH��ȡ����״̬�Ƿ���λ��.
* �������: ��
* �������: ��
* ���ز���: FLASH��������״̬��SET��RESET��
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
* ��������: FLASH_GetPrefetchBufferStatus
* ��������: ���FLASHԤȡ������״̬�Ƿ���λ.
* �������: ��
* �������: ��
* ���ز���: FLASHԤȡ������״̬��SET��RESET��.
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
  /* Return the new state of FLASH Prefetch Buffer Status (SET or RESET) [����flashԤȡ����״̬����״̬(SET �� RESET)]*/
  return bitstatus; 
}

/*******************************************************************************
* ��������: FLASH_ITConfig
* ��������: ʹ�ܣ�����ܣ��ض���FLASH�ж�.
* �������: FLASH_IT����ʹ�ܣ�����ܣ���FLASH�ж�Դ��
*                   �����������������ֵ���������:
*                       - FLASH_IT_ERROR: FLASH �����ж�
*                       - FLASH_IT_EOP: FLASH ��ɲ����ж�
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FLASH_ITConfig(u16 FLASH_IT, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FLASH_IT(FLASH_IT)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if(NewState != DISABLE)
  {
    /* Enable the interrupt sources [ʹ���ж�Դ]*/
    FLASH->CR |= FLASH_IT;
  }
  else
  {
    /* Disable the interrupt sources [��ֹ�ж�Դ]*/
    FLASH->CR &= ~(u32)FLASH_IT;
  }
}

/*******************************************************************************
* ��������: FLASH_GetFlagStatus
* ��������: ����ض���FLASH��־�Ƿ���λ.
* �������: FLASH_FLAG����Ҫ���ı�־��
*                     ������������������ֵ֮һ:
*                    - FLASH_FLAG_BSY: FLASHæ��־           
*                    - FLASH_FLAG_PGERR: FLASH��������־      
*                    - FLASH_FLAG_WRPRTERR: FLASHд���������־     
*                    - FLASH_FLAG_EOP: FLASH����������־          
*                    - FLASH_FLAG_OPTERR:  FLASHѡ���ֽڴ����־    
* �������: ��
* ���ز���: FLASH��־�Ĵ�����״̬����λ��λ����
*******************************************************************************/
FlagStatus FLASH_GetFlagStatus(u16 FLASH_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [������]*/
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
  /* Return the new state of FLASH_FLAG (SET or RESET)[FLASH��־�Ĵ�����״̬(��λ��λ)] */
  return bitstatus;
}

/*******************************************************************************
* ��������: FLASH_ClearFlag
* ��������: ���FLASH�����־.
* �������: FLASH_FLAG����Ҫ����ı�־��
*                    �����������������ֵ���������:
*                    - FLASH_FLAG_BSY: FLASHæ�ı�־           
*                    - FLASH_FLAG_PGERR: FLASH�������ı�־      
*                    - FLASH_FLAG_WRPRTERR: FLASHҳд��������ı�־     
*                    - FLASH_FLAG_EOP: FLASH���������ı�־          
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FLASH_ClearFlag(u16 FLASH_FLAG)
{
  /* Check the parameters [������]*/
  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;
  
  /* Clear the flags [�����־]*/
  FLASH->SR = FLASH_FLAG;
}

/*******************************************************************************
* ��������: FLASH_GetStatus
* ��������: ����FLASH״̬.
* �������: ��
* �������: ��
* ���ز���: FLASH״̬:�������ֵ������: FLASH_BUSY,
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
  /* Return the Flash Status [����FLASH״̬]*/
  return flashstatus;
}

/*******************************************************************************
* ��������: FLASH_WaitForLastOperation
* ��������: �ȴ�һ��Flash������������һ��TIMEOUT�¼�����.
* �������: - Timeout: FLASH��̳�ʱʱ��
* �������: ��
* ���ز���: FLASH״̬:�������ֵ������: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_WaitForLastOperation(u32 Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* Check for the Flash Status [���FLASH״̬]*/
  status = FLASH_GetStatus();

  /* Wait for a Flash operation to complete or a TIMEOUT to occur [�ȴ�FLASH������ɻ��߷�����ʱ]*/
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

  /* Return the operation status [���ز���״̬]*/
  return status;
}

/*******************************************************************************
* ��������: delay
* ��������: ����һ����ʱ.
* �������: ��
* �������: ��
* ���ز���: ��
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
