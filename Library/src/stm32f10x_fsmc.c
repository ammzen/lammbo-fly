/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_fsmc.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������FSMC(Flexible static memory controller[�ɲ�д�ľ�̬�洢��������])�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_fsmc.h"
#include "stm32f10x_rcc.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
/* --------------------- FSMC registers bit mask ---------------------------- */
/* FSMC BCRx Mask */
#define BCR_MBKEN_Set                       ((u32)0x00000001)
#define BCR_MBKEN_Reset                     ((u32)0x000FFFFE)
#define BCR_FACCEN_Set                      ((u32)0x00000040)

/* FSMC PCRx Mask */
#define PCR_PBKEN_Set                       ((u32)0x00000004)
#define PCR_PBKEN_Reset                     ((u32)0x000FFFFB)
#define PCR_ECCEN_Set                       ((u32)0x00000040)
#define PCR_ECCEN_Reset                     ((u32)0x000FFFBF)
#define PCR_MemoryType_NAND                 ((u32)0x00000008)

/* ���ú� -----------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: FSMC_NORSRAMDeInit
* ��������: Deinitializes the FSMC NOR/SRAM Banks registers to their default
*                  reset values[ȡ��FSMC NOR/SRAM ��Ĵ����ĳ�ʼ����ԭ�����ǵ�Ĭ��ֵ].
* �������: - FSMC_Bank: specifies the FSMC Bank to be used[ָ��ʹ�õ�FSMC��]
*                    ������������������ֵ֮һ:
*                       - FSMC_Bank1_NORSRAM1: FSMC ��1 NOR/SRAM1  
*                       - FSMC_Bank1_NORSRAM2: FSMC ��1 NOR/SRAM2 
*                       - FSMC_Bank1_NORSRAM3: FSMC ��1 NOR/SRAM3 
*                       - FSMC_Bank1_NORSRAM4: FSMC ��1 NOR/SRAM4                       
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_NORSRAMDeInit(u32 FSMC_Bank)
{
  /* Check the parameter [������]*/
  assert_param(IS_FSMC_NORSRAM_BANK(FSMC_Bank));
  
  /* FSMC_Bank1_NORSRAM1 */
  if(FSMC_Bank == FSMC_Bank1_NORSRAM1)
  {
    FSMC_Bank1->BTCR[FSMC_Bank] = 0x000030DB;    
  }
  /* FSMC_Bank1_NORSRAM2,  FSMC_Bank1_NORSRAM3 or FSMC_Bank1_NORSRAM4 */
  else
  {   
    FSMC_Bank1->BTCR[FSMC_Bank] = 0x000030D2; 
  }

  FSMC_Bank1->BTCR[FSMC_Bank + 1] = 0x0FFFFFFF;
  FSMC_Bank1E->BWTR[FSMC_Bank] = 0x0FFFFFFF;  
}

/*******************************************************************************
* ��������: FSMC_NANDDeInit
* ��������: Deinitializes the FSMC NAND Banks registers to their default
*                  reset values[ȡ��FSMC NAND��Ĵ����ĳ�ʼ����ԭ�����ǵ�Ĭ��ֵ].
* �������: - FSMC_Bank: specifies the FSMC Bank to be used[ָ��ʹ�õ�FSMC��]
*                    ������������������ֵ֮һ:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND                       
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_NANDDeInit(u32 FSMC_Bank)
{
  /* Check the parameter [������]*/
  assert_param(IS_FSMC_NAND_BANK(FSMC_Bank));
  
  if(FSMC_Bank == FSMC_Bank2_NAND)
  {
    /* Set the FSMC_Bank2 registers to their reset values [����FSMC_Bank2�Ĵ��������ǵĸ�λֵ]*/
    FSMC_Bank2->PCR2 = 0x00000018;
    FSMC_Bank2->SR2 = 0x00000040;
    FSMC_Bank2->PMEM2 = 0xFCFCFCFC;
    FSMC_Bank2->PATT2 = 0xFCFCFCFC;  
  }
  /* FSMC_Bank3_NAND */  
  else
  {
    /* Set the FSMC_Bank3 registers to their reset values [����FSMC_Bank3�Ĵ��������ǵĸ�λֵ]*/
    FSMC_Bank3->PCR3 = 0x00000018;
    FSMC_Bank3->SR3 = 0x00000040;
    FSMC_Bank3->PMEM3 = 0xFCFCFCFC;
    FSMC_Bank3->PATT3 = 0xFCFCFCFC; 
  }  
}

/*******************************************************************************
* ��������: FSMC_PCCARDDeInit
* ��������: Deinitializes the FSMC PCCARD Bank registers to their default
*                  reset values[ȡ��FSMC PCCARD��Ĵ����ĳ�ʼ����ԭ�����ǵ�Ĭ��ֵ].
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_PCCARDDeInit(void)
{
  /* Set the FSMC_Bank4 registers to their reset values [����FSMC_Bank4�Ĵ��������ǵĸ�λֵ]*/
  FSMC_Bank4->PCR4 = 0x00000018; 
  FSMC_Bank4->SR4 = 0x00000000; 
  FSMC_Bank4->PMEM4 = 0xFCFCFCFC;
  FSMC_Bank4->PATT4 = 0xFCFCFCFC;
  FSMC_Bank4->PIO4 = 0xFCFCFCFC;
}

/*******************************************************************************
* ��������: FSMC_NORSRAMInit
* ��������: Initializes the FSMC NOR/SRAM Banks according to the[����FSMC_NORSRAMInitStruct�ṹ��ָ���Ĳ�����ʼ��FSMC NOR/SRAM]
*                  specified parameters in the FSMC_NORSRAMInitStruct.
* �������: - FSMC_NORSRAMInitStruct : pointer to a FSMC_NORSRAMInitTypeDef
*                  structure that contains the configuration information for 
*                  the FSMC NOR/SRAM specified Banks.[ָ�������FSMC NOR/SRAMָ������Ϣ�Ľṹ��FSMC_NORSRAMInitTypeDef��ָ��]                       
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_NORSRAMInit(FSMC_NORSRAMInitTypeDef* FSMC_NORSRAMInitStruct)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_FSMC_NORSRAM_BANK(FSMC_NORSRAMInitStruct->FSMC_Bank));
  assert_param(IS_FSMC_MUX(FSMC_NORSRAMInitStruct->FSMC_DataAddressMux));
  assert_param(IS_FSMC_MEMORY(FSMC_NORSRAMInitStruct->FSMC_MemoryType));
  assert_param(IS_FSMC_MEMORY_WIDTH(FSMC_NORSRAMInitStruct->FSMC_MemoryDataWidth));
  assert_param(IS_FSMC_BURSTMODE(FSMC_NORSRAMInitStruct->FSMC_BurstAccessMode));
  assert_param(IS_FSMC_WAIT_POLARITY(FSMC_NORSRAMInitStruct->FSMC_WaitSignalPolarity));
  assert_param(IS_FSMC_WRAP_MODE(FSMC_NORSRAMInitStruct->FSMC_WrapMode));
  assert_param(IS_FSMC_WAIT_SIGNAL_ACTIVE(FSMC_NORSRAMInitStruct->FSMC_WaitSignalActive));
  assert_param(IS_FSMC_WRITE_OPERATION(FSMC_NORSRAMInitStruct->FSMC_WriteOperation));
  assert_param(IS_FSMC_WAITE_SIGNAL(FSMC_NORSRAMInitStruct->FSMC_WaitSignal));
  assert_param(IS_FSMC_EXTENDED_MODE(FSMC_NORSRAMInitStruct->FSMC_ExtendedMode));
  assert_param(IS_FSMC_ASYNC_WAIT(FSMC_NORSRAMInitStruct->FSMC_AsyncWait));
  assert_param(IS_FSMC_WRITE_BURST(FSMC_NORSRAMInitStruct->FSMC_WriteBurst));  
  assert_param(IS_FSMC_ADDRESS_SETUP_TIME(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressSetupTime));
  assert_param(IS_FSMC_ADDRESS_HOLD_TIME(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressHoldTime));
  assert_param(IS_FSMC_DATASETUP_TIME(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataSetupTime));
  assert_param(IS_FSMC_TURNAROUND_TIME(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_BusTurnAroundDuration));
  assert_param(IS_FSMC_CLK_DIV(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_CLKDivision));
  assert_param(IS_FSMC_DATA_LATENCY(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataLatency));
  assert_param(IS_FSMC_ACCESS_MODE(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AccessMode)); 
  
  /* Bank1 NOR/SRAM control register configuration [��1NOR/SRAM���ƼĴ���������]*/ 
  FSMC_Bank1->BTCR[FSMC_NORSRAMInitStruct->FSMC_Bank] = 
            (u32)FSMC_NORSRAMInitStruct->FSMC_DataAddressMux |
            FSMC_NORSRAMInitStruct->FSMC_MemoryType |
            FSMC_NORSRAMInitStruct->FSMC_MemoryDataWidth |
            FSMC_NORSRAMInitStruct->FSMC_BurstAccessMode |
            FSMC_NORSRAMInitStruct->FSMC_WaitSignalPolarity |
            FSMC_NORSRAMInitStruct->FSMC_WrapMode |
            FSMC_NORSRAMInitStruct->FSMC_WaitSignalActive |
            FSMC_NORSRAMInitStruct->FSMC_WriteOperation |
            FSMC_NORSRAMInitStruct->FSMC_WaitSignal |
            FSMC_NORSRAMInitStruct->FSMC_ExtendedMode |
            FSMC_NORSRAMInitStruct->FSMC_AsyncWait |
            FSMC_NORSRAMInitStruct->FSMC_WriteBurst;

  if(FSMC_NORSRAMInitStruct->FSMC_MemoryType == FSMC_MemoryType_NOR)
  {
    FSMC_Bank1->BTCR[FSMC_NORSRAMInitStruct->FSMC_Bank] |= (u32)BCR_FACCEN_Set;
  }

  /* Bank1 NOR/SRAM timing register configuration [��1NOR/SRAMʱ��Ĵ���������]*/
  FSMC_Bank1->BTCR[FSMC_NORSRAMInitStruct->FSMC_Bank+1] = 
            (u32)FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressSetupTime |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressHoldTime << 4) |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataSetupTime << 8) |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_BusTurnAroundDuration << 16) |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_CLKDivision << 20) |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataLatency << 24) |
             FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AccessMode;
            

    
  /* Bank1 NOR/SRAM timing register for write configuration, if extended mode is used [���ʹ����չģʽ����1NOR/SRAMʱ��Ĵ�����д����]*/
  if(FSMC_NORSRAMInitStruct->FSMC_ExtendedMode == FSMC_ExtendedMode_Enable)
  {
    assert_param(IS_FSMC_ADDRESS_SETUP_TIME(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressSetupTime));
    assert_param(IS_FSMC_ADDRESS_HOLD_TIME(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressHoldTime));
    assert_param(IS_FSMC_DATASETUP_TIME(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_DataSetupTime));
    assert_param(IS_FSMC_TURNAROUND_TIME(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_BusTurnAroundDuration));
    assert_param(IS_FSMC_CLK_DIV(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_CLKDivision));
    assert_param(IS_FSMC_DATA_LATENCY(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_DataLatency));
    assert_param(IS_FSMC_ACCESS_MODE(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AccessMode));

    FSMC_Bank1E->BWTR[FSMC_NORSRAMInitStruct->FSMC_Bank] = 
              (u32)FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressSetupTime |
              (FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressHoldTime << 4 )|
              (FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_DataSetupTime << 8) |
              (FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_BusTurnAroundDuration << 16) |
              (FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_CLKDivision << 20) |
              (FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_DataLatency << 24) |
               FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AccessMode;
  }
  else
  {
    FSMC_Bank1E->BWTR[FSMC_NORSRAMInitStruct->FSMC_Bank] = 0x0FFFFFFF;
  }
}

/*******************************************************************************
* ��������: FSMC_NANDInit
* ��������: Initializes the FSMC NAND Banks according to the specified
*                  parameters in the FSMC_NANDInitStruct[����FSMC_NANDInitStruct�ṹ��ָ���Ĳ�����ʼ��FSMC NAND].
* �������: - FSMC_NANDInitStruct : pointer to a FSMC_NANDInitTypeDef
*                    structure that contains the configuration information for 
*                    the FSMC NAND specified Banks[ָ�������FSMC NANDָ������Ϣ�Ľṹ��FSMC_NANDInitTypeDef��ָ��].                       
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_NANDInit(FSMC_NANDInitTypeDef* FSMC_NANDInitStruct)
{
  u32 tmppcr = 0x00000000, tmppmem = 0x00000000, tmppatt = 0x00000000; 
    
  /* Check the parameters [������]*/
  assert_param( IS_FSMC_NAND_BANK(FSMC_NANDInitStruct->FSMC_Bank));
  assert_param( IS_FSMC_WAIT_FEATURE(FSMC_NANDInitStruct->FSMC_Waitfeature));
  assert_param( IS_FSMC_DATA_WIDTH(FSMC_NANDInitStruct->FSMC_MemoryDataWidth));
  assert_param( IS_FSMC_ECC_STATE(FSMC_NANDInitStruct->FSMC_ECC));
  assert_param( IS_FSMC_ECCPAGE_SIZE(FSMC_NANDInitStruct->FSMC_ECCPageSize));
  assert_param( IS_FSMC_ADDRESS_LOW_MAPPING(FSMC_NANDInitStruct->FSMC_AddressLowMapping));
  assert_param( IS_FSMC_TCLR_TIME(FSMC_NANDInitStruct->FSMC_TCLRSetupTime));
  assert_param( IS_FSMC_TAR_TIME(FSMC_NANDInitStruct->FSMC_TARSetupTime));

  assert_param(IS_FSMC_SETUP_TIME(FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_SetupTime));
  assert_param(IS_FSMC_WAIT_TIME(FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_WaitSetupTime));
  assert_param(IS_FSMC_HOLD_TIME(FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HoldSetupTime));
  assert_param(IS_FSMC_HIZ_TIME(FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HiZSetupTime));

  assert_param(IS_FSMC_SETUP_TIME(FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_SetupTime));
  assert_param(IS_FSMC_WAIT_TIME(FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_WaitSetupTime));
  assert_param(IS_FSMC_HOLD_TIME(FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HoldSetupTime));
  assert_param(IS_FSMC_HIZ_TIME(FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HiZSetupTime));
  
  /* Set the tmppcr value according to FSMC_NANDInitStruct parameters [����FSMC_NANDInitStruct�Ĳ�������tmppcr��ֵ]*/
  tmppcr = (u32)FSMC_NANDInitStruct->FSMC_Waitfeature |
            PCR_MemoryType_NAND |
            FSMC_NANDInitStruct->FSMC_MemoryDataWidth |
            FSMC_NANDInitStruct->FSMC_ECC |
            FSMC_NANDInitStruct->FSMC_ECCPageSize |
            FSMC_NANDInitStruct->FSMC_AddressLowMapping |
            (FSMC_NANDInitStruct->FSMC_TCLRSetupTime << 9 )|
            (FSMC_NANDInitStruct->FSMC_TARSetupTime << 13);
            
  /* Set tmppmem value according to FSMC_CommonSpaceTimingStructure parameters [����FSMC_CommonSpaceTimingStructure�Ĳ�������tmppmem��ֵ]*/
  tmppmem = (u32)FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_SetupTime |
            (FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
            (FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
            (FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HiZSetupTime << 24); 
            
  /* Set tmppatt value according to FSMC_AttributeSpaceTimingStructure parameters [����FSMC_AttributeSpaceTimingStructure�Ĳ�������tmppatt��ֵ]*/
  tmppatt = (u32)FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_SetupTime |
            (FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
            (FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
            (FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HiZSetupTime << 24);
  
  if(FSMC_NANDInitStruct->FSMC_Bank == FSMC_Bank2_NAND)
  {
    /* FSMC_Bank2_NAND registers configuration [FSMC_Bank2_NAND�Ĵ�������]*/
    FSMC_Bank2->PCR2 = tmppcr;
    FSMC_Bank2->PMEM2 = tmppmem;
    FSMC_Bank2->PATT2 = tmppatt;
  }
  else
  {
    /* FSMC_Bank3_NAND registers configuration [FSMC_Bank3_NAND�Ĵ�������]*/
    FSMC_Bank3->PCR3 = tmppcr;
    FSMC_Bank3->PMEM3 = tmppmem;
    FSMC_Bank3->PATT3 = tmppatt;
  }
}

/*******************************************************************************
* ��������: FSMC_PCCARDInit
* ��������: Initializes the FSMC PCCARD Bank according to the specified
*                  parameters in the FSMC_PCCARDInitStruct[����FSMC_PCCARDInitStruct�ṹ��ָ���Ĳ�����ʼ��FSMC PCCARD].
* �������: - FSMC_PCCARDInitStruct : pointer to a FSMC_PCCARDInitTypeDef
*                    structure that contains the configuration information for 
*                    the FSMC PCCARD Bank.[ָ�������FSMC PCCARDָ������Ϣ�Ľṹ��FSMC_PCCARDInitTypeDef��ָ��]                       
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_PCCARDInit(FSMC_PCCARDInitTypeDef* FSMC_PCCARDInitStruct)
{
  /* Check the parameters [������]*/
  assert_param(IS_FSMC_WAIT_FEATURE(FSMC_PCCARDInitStruct->FSMC_Waitfeature));
  assert_param(IS_FSMC_ADDRESS_LOW_MAPPING(FSMC_PCCARDInitStruct->FSMC_AddressLowMapping));
  assert_param(IS_FSMC_TCLR_TIME(FSMC_PCCARDInitStruct->FSMC_TCLRSetupTime));
  assert_param(IS_FSMC_TAR_TIME(FSMC_PCCARDInitStruct->FSMC_TARSetupTime));

 
  assert_param(IS_FSMC_SETUP_TIME(FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_SetupTime));
  assert_param(IS_FSMC_WAIT_TIME(FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_WaitSetupTime));
  assert_param(IS_FSMC_HOLD_TIME(FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HoldSetupTime));
  assert_param(IS_FSMC_HIZ_TIME(FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HiZSetupTime));
  
  assert_param(IS_FSMC_SETUP_TIME(FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_SetupTime));
  assert_param(IS_FSMC_WAIT_TIME(FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_WaitSetupTime));
  assert_param(IS_FSMC_HOLD_TIME(FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HoldSetupTime));
  assert_param(IS_FSMC_HIZ_TIME(FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HiZSetupTime));

  assert_param(IS_FSMC_SETUP_TIME(FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_SetupTime));
  assert_param(IS_FSMC_WAIT_TIME(FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_WaitSetupTime));
  assert_param(IS_FSMC_HOLD_TIME(FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_HoldSetupTime));
  assert_param(IS_FSMC_HIZ_TIME(FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_HiZSetupTime));
  
  /* Set the PCR4 register value according to FSMC_PCCARDInitStruct parameters [����FSMC_PCCARDInitStruct�Ĳ�������PCR4�Ĵ�����ֵ]*/
  FSMC_Bank4->PCR4 = (u32)FSMC_PCCARDInitStruct->FSMC_Waitfeature |
                     FSMC_PCCARDInitStruct->FSMC_AddressLowMapping |
                     (FSMC_PCCARDInitStruct->FSMC_TCLRSetupTime << 9) |
                     (FSMC_PCCARDInitStruct->FSMC_TARSetupTime << 13);
            
  /* Set PMEM4 register value according to FSMC_CommonSpaceTimingStructure parameters [����FSMC_CommonSpaceTimingStructure�Ĳ�������PMEM4�Ĵ�����ֵ]*/
  FSMC_Bank4->PMEM4 = (u32)FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_SetupTime |
                      (FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
                      (FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
                      (FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HiZSetupTime << 24); 
            
  /* Set PATT4 register value according to FSMC_AttributeSpaceTimingStructure parameters [����FSMC_AttributeSpaceTimingStructure�Ĳ�������PATT4�Ĵ�����ֵ]*/
  FSMC_Bank4->PATT4 = (u32)FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_SetupTime |
                      (FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
                      (FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
                      (FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HiZSetupTime << 24);    
            
  /* Set PIO4 register value according to FSMC_IOSpaceTimingStructure parameters [����FSMC_IOSpaceTimingStructure�Ĳ�������PIO4�Ĵ�����ֵ]*/
  FSMC_Bank4->PIO4 = (u32)FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_SetupTime |
                     (FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
                     (FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
                     (FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_HiZSetupTime << 24);             
}

/*******************************************************************************
* ��������: FSMC_NORSRAMStructInit
* ��������: Fills each FSMC_NORSRAMInitStruct member with its default value[��Ĭ��ֵ���FSMC_NORSRAMInitStruct�ĳ�Ա].
* �������: - FSMC_NORSRAMInitStruct: pointer to a FSMC_NORSRAMInitTypeDef
*                    structure which will be initialized.[ָ��Ҫ����ʼ���Ľṹ��FSMC_NORSRAMInitTypeDef��ָ��]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_NORSRAMStructInit(FSMC_NORSRAMInitTypeDef* FSMC_NORSRAMInitStruct)
{  
  /* Reset NOR/SRAM Init structure parameters values [��λNOR/SRAM��ʼ���ṹ����ֵ]*/
  FSMC_NORSRAMInitStruct->FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStruct->FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
  FSMC_NORSRAMInitStruct->FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStruct->FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
  FSMC_NORSRAMInitStruct->FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStruct->FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStruct->FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStruct->FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStruct->FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStruct->FSMC_WaitSignal = FSMC_WaitSignal_Enable;
  FSMC_NORSRAMInitStruct->FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStruct->FSMC_AsyncWait = FSMC_AsyncWait_Disable;
  FSMC_NORSRAMInitStruct->FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressSetupTime = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressHoldTime = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataSetupTime = 0xFF;
  FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_BusTurnAroundDuration = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_CLKDivision = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataLatency = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AccessMode = FSMC_AccessMode_A; 
  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressSetupTime = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressHoldTime = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_DataSetupTime = 0xFF;
  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_BusTurnAroundDuration = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_CLKDivision = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_DataLatency = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AccessMode = FSMC_AccessMode_A;
}

/*******************************************************************************
* ��������: FSMC_NANDStructInit
* ��������: Fills each FSMC_NANDInitStruct member with its default value[��Ĭ��ֵ���FSMC_NANDInitStruct��ÿһ����Ա].
* �������: - FSMC_NORSRAMInitStruct: pointer to a FSMC_NANDInitTypeDef
*                    structure which will be initialized.[ָ��Ҫ����ʼ���Ľṹ��FSMC_NANDInitTypeDef��ָ��]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_NANDStructInit(FSMC_NANDInitTypeDef* FSMC_NANDInitStruct)
{ 
  /* Reset NAND Init structure parameters values [��λNAND��ʼ���ṹ����ֵ]*/
  FSMC_NANDInitStruct->FSMC_Bank = FSMC_Bank2_NAND;
  FSMC_NANDInitStruct->FSMC_Waitfeature = FSMC_Waitfeature_Disable;
  FSMC_NANDInitStruct->FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
  FSMC_NANDInitStruct->FSMC_ECC = FSMC_ECC_Disable;
  FSMC_NANDInitStruct->FSMC_ECCPageSize = FSMC_ECCPageSize_256Bytes;
  FSMC_NANDInitStruct->FSMC_AddressLowMapping = FSMC_AddressLowMapping_Direct;
  FSMC_NANDInitStruct->FSMC_TCLRSetupTime = 0x0;
  FSMC_NANDInitStruct->FSMC_TARSetupTime = 0x0;
  FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_SetupTime = 0xFC;
  FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_WaitSetupTime = 0xFC;
  FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HoldSetupTime = 0xFC;
  FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HiZSetupTime = 0xFC;
  FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_SetupTime = 0xFC;
  FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_WaitSetupTime = 0xFC;
  FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HoldSetupTime = 0xFC;
  FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HiZSetupTime = 0xFC;     
}

/*******************************************************************************
* ��������: FSMC_PCCARDStructInit
* ��������: Fills each FSMC_PCCARDInitStruct member with its default value.[��Ĭ��ֵ���FSMC_PCCARDInitStruct��ÿһ����Ա]
* �������: - FSMC_PCCARDInitStruct: pointer to a FSMC_PCCARDInitTypeDef
*                    structure which will be initialized.[ָ��Ҫ����ʼ���Ľṹ��FSMC_PCCARDInitTypeDef��ָ��]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_PCCARDStructInit(FSMC_PCCARDInitTypeDef* FSMC_PCCARDInitStruct)
{
  /* Reset PCCARD Init structure parameters values [��λPCCARD��ʼ���ṹ����ֵ]*/
  FSMC_PCCARDInitStruct->FSMC_Waitfeature = FSMC_Waitfeature_Disable;
  FSMC_PCCARDInitStruct->FSMC_AddressLowMapping = FSMC_AddressLowMapping_Direct;
  FSMC_PCCARDInitStruct->FSMC_TCLRSetupTime = 0x0;
  FSMC_PCCARDInitStruct->FSMC_TARSetupTime = 0x0;
  FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_SetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_WaitSetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HoldSetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HiZSetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_SetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_WaitSetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HoldSetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HiZSetupTime = 0xFC; 
  FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_SetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_WaitSetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_HoldSetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_HiZSetupTime = 0xFC;
}

/*******************************************************************************
* ��������: FSMC_NORSRAMCmd
* ��������: Enables or disables the specified NOR/SRAM Memory Bank[ʹ�ܻ��ָֹ����NOR/SRAM�ڴ���].
* �������: (1) FSMC_Bank: specifies the FSMC Bank to be used[ָ��ʹ�õ�FSMC��]
*                    ������������������ֵ֮һ:
*                       - FSMC_Bank1_NORSRAM1: FSMC Bank1 NOR/SRAM1  
*                       - FSMC_Bank1_NORSRAM2: FSMC Bank1 NOR/SRAM2 
*                       - FSMC_Bank1_NORSRAM3: FSMC Bank1 NOR/SRAM3 
*                       - FSMC_Bank1_NORSRAM4: FSMC Bank1 NOR/SRAM4 
*           (2) NewState: new state of the FSMC_Bank.[FSMC_Bank����״̬]
*                    This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_NORSRAMCmd(u32 FSMC_Bank, FunctionalState NewState)
{
  assert_param(IS_FSMC_NORSRAM_BANK(FSMC_Bank));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected NOR/SRAM Bank by setting the PBKEN bit in the BCRx register [ͨ����λBCRx�Ĵ����е�PBKENλʹ��ѡ���NOR/SRAM��]*/
    FSMC_Bank1->BTCR[FSMC_Bank] |= BCR_MBKEN_Set;
  }
  else
  {
    /* Disable the selected NOR/SRAM Bank by clearing the PBKEN bit in the BCRx register [ͨ�����BCRx�Ĵ����е�PBKENλ��ֹѡ���NOR/SRAM��]*/
    FSMC_Bank1->BTCR[FSMC_Bank] &= BCR_MBKEN_Reset;
  }
}

/*******************************************************************************
* ��������: FSMC_NANDCmd
* ��������: Enables or disables the specified NAND Memory Bank[ʹ�ܻ��ָֹ����NAND�ڴ���].
* �������: (1) FSMC_Bank: specifies the FSMC Bank to be used[ָ��ʹ�õ�FSMC��]
*                    ������������������ֵ֮һ:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
*           (2) NewState: new state of the FSMC_Bank.[FSMC_Bank����״̬]
*                    This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_NANDCmd(u32 FSMC_Bank, FunctionalState NewState)
{
  assert_param(IS_FSMC_NAND_BANK(FSMC_Bank));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected NAND Bank by setting the PBKEN bit in the PCRx register [ͨ����λPCRx�Ĵ����е�PBKENλʹ��ѡ���NAND��]*/
    if(FSMC_Bank == FSMC_Bank2_NAND)
    {
      FSMC_Bank2->PCR2 |= PCR_PBKEN_Set;
    }
    else
    {
      FSMC_Bank3->PCR3 |= PCR_PBKEN_Set;
    }
  }
  else
  {
    /* Disable the selected NAND Bank by clearing the PBKEN bit in the PCRx register [ͨ�����PCRx�Ĵ����е�PBKENλ��ֹѡ���NAND��]*/
    if(FSMC_Bank == FSMC_Bank2_NAND)
    {
      FSMC_Bank2->PCR2 &= PCR_PBKEN_Reset;
    }
    else
    {
      FSMC_Bank3->PCR3 &= PCR_PBKEN_Reset;
    }
  }
}

/*******************************************************************************
* ��������: FSMC_PCCARDCmd
* ��������: Enables or disables the PCCARD Memory Bank[ʹ�ܻ��ָֹ����PCCARD�ڴ���].
* �������:  NewState: new state of the PCCARD Memory Bank.[PCCARD �ڴ������״̬]
*                    This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_PCCARDCmd(FunctionalState NewState)
{
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the PCCARD Bank by setting the PBKEN bit in the PCR4 register [ͨ����λPCR4�Ĵ����е�PBKENλʹ��ѡ���PCCARD��]*/
    FSMC_Bank4->PCR4 |= PCR_PBKEN_Set;
  }
  else
  {
    /* Disable the PCCARD Bank by clearing the PBKEN bit in the PCR4 register [ͨ�����PCR4�Ĵ����е�PBKENλ��ֹѡ���PCCARD��]*/
    FSMC_Bank4->PCR4 &= PCR_PBKEN_Reset;
  }
}

/*******************************************************************************
* ��������: FSMC_NANDECCCmd
* ��������: Enables or disables the FSMC NAND ECC feature.[ʹ�ܻ��ֹFSMC NAND ECC����]
* �������: - FSMC_Bank: specifies the FSMC Bank to be used[ָ��ʹ�õ�FSMC��]
*                    ������������������ֵ֮һ:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
*                : - NewState: new state of the FSMC NAND ECC feature.[FSMC NAND ECC���ܵ���״̬]  
*                    This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_NANDECCCmd(u32 FSMC_Bank, FunctionalState NewState)
{
  assert_param(IS_FSMC_NAND_BANK(FSMC_Bank));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected NAND Bank ECC function by setting the ECCEN bit in the PCRx register [ͨ����λPCRx�Ĵ����е�ECCENλʹ��ѡ���NAND���ECC����]*/
    if(FSMC_Bank == FSMC_Bank2_NAND)
    {
      FSMC_Bank2->PCR2 |= PCR_ECCEN_Set;
    }
    else
    {
      FSMC_Bank3->PCR3 |= PCR_ECCEN_Set;
    }
  }
  else
  {
    /* Disable the selected NAND Bank ECC function by clearing the ECCEN bit in the PCRx register [ͨ�����PCRx�Ĵ����е�ECCENλ��ֹѡ���NAND���ECC����]*/
    if(FSMC_Bank == FSMC_Bank2_NAND)
    {
      FSMC_Bank2->PCR2 &= PCR_ECCEN_Reset;
    }
    else
    {
      FSMC_Bank3->PCR3 &= PCR_ECCEN_Reset;
    }
  }
}

/*******************************************************************************
* ��������: FSMC_GetECC
* ��������: Returns the error correction code register value[���ؾ������Ĵ�����ֵ].
* �������: - FSMC_Bank: specifies the FSMC Bank to be used[ָ��ʹ�õ�FSMC��]
*                    ������������������ֵ֮һ:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
* �������: ��
* ���ز���: The Error Correction Code (ECC) value[�������ֵ].
*******************************************************************************/
u32 FSMC_GetECC(u32 FSMC_Bank)
{
  u32 eccval = 0x00000000;
  
  if(FSMC_Bank == FSMC_Bank2_NAND)
  {
    /* Get the ECCR2 register value [ȡ��ECCR2�Ĵ���ֵ]*/
    eccval = FSMC_Bank2->ECCR2;
  }
  else
  {
    /* Get the ECCR3 register value [ȡ��ECCR3�Ĵ���ֵ]*/
    eccval = FSMC_Bank3->ECCR3;
  }
  /* Return the error correction code value [���ؾ�������ֵ]*/
  return(eccval);
}

/*******************************************************************************
* ��������: FSMC_ITConfig
* ��������: Enables or disables the specified FSMC interrupts[ʹ�ܻ��ָֹ����FSMC�ж�].
* �������: (1) FSMC_Bank: specifies the FSMC Bank to be used[ָ��ʹ�õ�FSMC��]
*                    ������������������ֵ֮һ:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
*                       - FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
*           (2) FSMC_IT: specifies the FSMC interrupt sources to be
*                    enabled or disabled.[ʹ�ܻ��ָֹ����FSMC�ж�Դ]
*                    ������������������κ�ֵ����ϣ�
*                       - FSMC_IT_RisingEdge: Rising edge detection interrupt[�����ؼ���ж�]. 
*                       - FSMC_IT_Level: Level edge detection interrupt[�½��ؼ���ж�].                                  
*                       - FSMC_IT_FallingEdge: Falling edge detection interrupt[��ƽ�仯����ж�].
*           (3) NewState: new state of the specified FSMC interrupts.[ָ��FSMC�жϵ���״̬]
*                    This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_ITConfig(u32 FSMC_Bank, u32 FSMC_IT, FunctionalState NewState)
{
  assert_param(IS_FSMC_IT_BANK(FSMC_Bank));
  assert_param(IS_FSMC_IT(FSMC_IT));    
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected FSMC_Bank2 interrupts [ʹ��ѡ���FSMC_Bank2�ж�]*/
    if(FSMC_Bank == FSMC_Bank2_NAND)
    {
      FSMC_Bank2->SR2 |= FSMC_IT;
    }
    /* Enable the selected FSMC_Bank3 interrupts [ʹ��ѡ���FSMC_Bank3�ж�]*/
    else if (FSMC_Bank == FSMC_Bank3_NAND)
    {
      FSMC_Bank3->SR3 |= FSMC_IT;
    }
    /* Enable the selected FSMC_Bank4 interrupts [ʹ��ѡ���FSMC_Bank4�ж�]*/
    else
    {
      FSMC_Bank4->SR4 |= FSMC_IT;    
    }
  }
  else
  {
    /* Disable the selected FSMC_Bank2 interrupts [��ֹѡ���FSMC_Bank2�ж�]*/
    if(FSMC_Bank == FSMC_Bank2_NAND)
    {
      
      FSMC_Bank2->SR2 &= (u32)~FSMC_IT;
    }
    /* Disable the selected FSMC_Bank3 interrupts [��ֹѡ���FSMC_Bank3�ж�]*/
    else if (FSMC_Bank == FSMC_Bank3_NAND)
    {
      FSMC_Bank3->SR3 &= (u32)~FSMC_IT;
    }
    /* Disable the selected FSMC_Bank4 interrupts [��ֹѡ���FSMC_Bank4�ж�]*/
    else
    {
      FSMC_Bank4->SR4 &= (u32)~FSMC_IT;    
    }
  }
}
                  
/*******************************************************************************
* ��������: FSMC_GetFlagStatus
* ��������: Checks whether the specified FSMC flag is set or not[���ָ����FSMC��־�Ƿ���λ].
* �������: - FSMC_Bank: specifies the FSMC Bank to be used[ָ��ʹ�õ�FSMC��]
*                    ������������������ֵ֮һ:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
*                       - FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
*                  - FSMC_FLAG: specifies the flag to check.[������ָ����־]
*                    ������������������ֵ֮һ:
*                       - FSMC_FLAG_RisingEdge: Rising egde detection Flag.[�����ؼ���־]
*                       - FSMC_FLAG_Level: Level detection Flag.[��ƽ����־]
*                       - FSMC_FLAG_FallingEdge: Falling egde detection Flag[�½��ؼ���־].
*                       - FSMC_FLAG_FEMPT: Fifo empty Flag[Fifo�ձ�־]. 
* �������: ��
* ���ز���: The new state of FSMC_FLAG (SET or RESET).[ָ��FSMC_FLAG����״̬(SET or RESET)]
*******************************************************************************/                   
FlagStatus FSMC_GetFlagStatus(u32 FSMC_Bank, u32 FSMC_FLAG)
{
  FlagStatus bitstatus = RESET;
  u32 tmpsr = 0x00000000;
  
  /* Check the parameters [������]*/
  assert_param(IS_FSMC_GETFLAG_BANK(FSMC_Bank));
  assert_param(IS_FSMC_GET_FLAG(FSMC_FLAG));
  
  if(FSMC_Bank == FSMC_Bank2_NAND)
  {
    tmpsr = FSMC_Bank2->SR2;
  }  
  else if(FSMC_Bank == FSMC_Bank3_NAND)
  {
    tmpsr = FSMC_Bank3->SR3;
  }
  /* FSMC_Bank4_PCCARD*/
  else
  {
    tmpsr = FSMC_Bank4->SR4;
  } 
  
  /* Get the flag status [ȡ�ñ�־״̬]*/
  if ((tmpsr & FSMC_FLAG) != (u16)RESET )
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* Return the flag status [���ر�־״̬]*/
  return bitstatus;
}

/*******************************************************************************
* ��������: FSMC_ClearFlag
* ��������: Clears the FSMC��s pending flags.[���FSMC�����־]
* �������: - FSMC_Bank: specifies the FSMC Bank to be used[ָ��ʹ�õ�FSMC��]
*                    ������������������ֵ֮һ:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
*                       - FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
*                  - FSMC_FLAG: specifies the flag to clear.[Ҫ�����ָ����־]
*                    ������������������κ�ֵ����ϣ�
*                       - FSMC_FLAG_RisingEdge: Rising egde detection Flag.[�����ر�־]
*                       - FSMC_FLAG_Level: Level detection Flag.[��ƽ����־]
*                       - FSMC_FLAG_FallingEdge: Falling egde detection Flag[�½��ر�־].
* �������: ��
* ���ز���: ��
*******************************************************************************/                   
void FSMC_ClearFlag(u32 FSMC_Bank, u32 FSMC_FLAG)
{
 /* Check the parameters [������]*/
  assert_param(IS_FSMC_GETFLAG_BANK(FSMC_Bank));
  assert_param(IS_FSMC_CLEAR_FLAG(FSMC_FLAG)) ;
    
  if(FSMC_Bank == FSMC_Bank2_NAND)
  {
    FSMC_Bank2->SR2 &= ~FSMC_FLAG; 
  }  
  else if(FSMC_Bank == FSMC_Bank3_NAND)
  {
    FSMC_Bank3->SR3 &= ~FSMC_FLAG;
  }
  /* FSMC_Bank4_PCCARD*/
  else
  {
    FSMC_Bank4->SR4 &= ~FSMC_FLAG;
  }
}

/*******************************************************************************
* ��������: FSMC_GetITStatus
* ��������: Checks whether the specified FSMC interrupt has occurred or not[���ָ����FSMC�ж��Ƿ��Ѿ�����].
* �������: - FSMC_Bank: specifies the FSMC Bank to be used[ָ��ʹ�õ�FSMC��]
*                    ������������������ֵ֮һ:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
*                       - FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
*                  - FSMC_IT: specifies the FSMC interrupt source to check[Ҫ����ָ��FSMC�ж�Դ].
*                    ������������������ֵ֮һ:
*                       - FSMC_IT_RisingEdge: Rising edge detection interrupt[�������ж�]. 
*                       - FSMC_IT_Level: Level edge detection interrupt.[��ƽ����ж�]                                  
*                       - FSMC_IT_FallingEdge: Falling edge detection interrupt[�½����ж�]. 
* �������: ��
* ���ز���: The new state of FSMC_IT (SET or RESET).
*******************************************************************************/ 
ITStatus FSMC_GetITStatus(u32 FSMC_Bank, u32 FSMC_IT)
{
  ITStatus bitstatus = RESET;
  u32 tmpsr = 0x0, itstatus = 0x0, itenable = 0x0; 
  
  /* Check the parameters [������]*/
  assert_param(IS_FSMC_IT_BANK(FSMC_Bank));
  assert_param(IS_FSMC_GET_IT(FSMC_IT));
  
  if(FSMC_Bank == FSMC_Bank2_NAND)
  {
    tmpsr = FSMC_Bank2->SR2;
  }  
  else if(FSMC_Bank == FSMC_Bank3_NAND)
  {
    tmpsr = FSMC_Bank3->SR3;
  }
  /* FSMC_Bank4_PCCARD*/
  else
  {
    tmpsr = FSMC_Bank4->SR4;
  } 
  
  itstatus = tmpsr & FSMC_IT;
  
  itenable = tmpsr & (FSMC_IT >> 3);

  if ((itstatus != (u32)RESET)  && (itenable != (u32)RESET))
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
* ��������: FSMC_ClearITPendingBit
* ��������: Clears the FSMC'S interrupt pending bits.[���FSMC�жϹ���λ]
* �������: - FSMC_Bank: specifies the FSMC Bank to be used[ָ��ʹ�õ�FSMC��]
*                    ������������������ֵ֮һ:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
*                       - FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
*                  - FSMC_IT: specifies the interrupt pending bit to clear[Ҫ�����ָ���жϹ���λ].
*                    ������������������κ�ֵ����ϣ�
*                       - FSMC_IT_RisingEdge: Rising edge detection interrupt[�������ж�]. 
*                       - FSMC_IT_Level: Level edge detection interrupt.[��ƽ����ж�]                                  
*                       - FSMC_IT_FallingEdge: Falling edge detection interrupt.[�½����ж�]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void FSMC_ClearITPendingBit(u32 FSMC_Bank, u32 FSMC_IT)
{
  /* Check the parameters [������]*/
  assert_param(IS_FSMC_IT_BANK(FSMC_Bank));
  assert_param(IS_FSMC_IT(FSMC_IT));
    
  if(FSMC_Bank == FSMC_Bank2_NAND)
  {
    FSMC_Bank2->SR2 &= ~(FSMC_IT >> 3); 
  }  
  else if(FSMC_Bank == FSMC_Bank3_NAND)
  {
    FSMC_Bank3->SR3 &= ~(FSMC_IT >> 3);
  }
  /* FSMC_Bank4_PCCARD*/
  else
  {
    FSMC_Bank4->SR4 &= ~(FSMC_IT >> 3);
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
