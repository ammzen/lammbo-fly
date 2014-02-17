/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_fsmc.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有FSMC(Flexible static memory controller[可擦写的静态存储器控制器])的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_fsmc.h"
#include "stm32f10x_rcc.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
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

/* 自用宏 -----------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: FSMC_NORSRAMDeInit
* 功能描述: Deinitializes the FSMC NOR/SRAM Banks registers to their default
*                  reset values[取消FSMC NOR/SRAM 体寄存器的初始化还原到他们的默认值].
* 输入参数: - FSMC_Bank: specifies the FSMC Bank to be used[指定使用的FSMC体]
*                    这个参数可以是下面的值之一:
*                       - FSMC_Bank1_NORSRAM1: FSMC 体1 NOR/SRAM1  
*                       - FSMC_Bank1_NORSRAM2: FSMC 体1 NOR/SRAM2 
*                       - FSMC_Bank1_NORSRAM3: FSMC 体1 NOR/SRAM3 
*                       - FSMC_Bank1_NORSRAM4: FSMC 体1 NOR/SRAM4                       
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_NORSRAMDeInit(u32 FSMC_Bank)
{
  /* Check the parameter [检查参数]*/
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
* 函数名称: FSMC_NANDDeInit
* 功能描述: Deinitializes the FSMC NAND Banks registers to their default
*                  reset values[取消FSMC NAND体寄存器的初始化还原到他们的默认值].
* 输入参数: - FSMC_Bank: specifies the FSMC Bank to be used[指定使用的FSMC体]
*                    这个参数可以是下面的值之一:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND                       
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_NANDDeInit(u32 FSMC_Bank)
{
  /* Check the parameter [检查参数]*/
  assert_param(IS_FSMC_NAND_BANK(FSMC_Bank));
  
  if(FSMC_Bank == FSMC_Bank2_NAND)
  {
    /* Set the FSMC_Bank2 registers to their reset values [设置FSMC_Bank2寄存器到他们的复位值]*/
    FSMC_Bank2->PCR2 = 0x00000018;
    FSMC_Bank2->SR2 = 0x00000040;
    FSMC_Bank2->PMEM2 = 0xFCFCFCFC;
    FSMC_Bank2->PATT2 = 0xFCFCFCFC;  
  }
  /* FSMC_Bank3_NAND */  
  else
  {
    /* Set the FSMC_Bank3 registers to their reset values [设置FSMC_Bank3寄存器到他们的复位值]*/
    FSMC_Bank3->PCR3 = 0x00000018;
    FSMC_Bank3->SR3 = 0x00000040;
    FSMC_Bank3->PMEM3 = 0xFCFCFCFC;
    FSMC_Bank3->PATT3 = 0xFCFCFCFC; 
  }  
}

/*******************************************************************************
* 函数名称: FSMC_PCCARDDeInit
* 功能描述: Deinitializes the FSMC PCCARD Bank registers to their default
*                  reset values[取消FSMC PCCARD体寄存器的初始化还原到他们的默认值].
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_PCCARDDeInit(void)
{
  /* Set the FSMC_Bank4 registers to their reset values [设置FSMC_Bank4寄存器到他们的复位值]*/
  FSMC_Bank4->PCR4 = 0x00000018; 
  FSMC_Bank4->SR4 = 0x00000000; 
  FSMC_Bank4->PMEM4 = 0xFCFCFCFC;
  FSMC_Bank4->PATT4 = 0xFCFCFCFC;
  FSMC_Bank4->PIO4 = 0xFCFCFCFC;
}

/*******************************************************************************
* 函数名称: FSMC_NORSRAMInit
* 功能描述: Initializes the FSMC NOR/SRAM Banks according to the[依照FSMC_NORSRAMInitStruct结构体指定的参数初始化FSMC NOR/SRAM]
*                  specified parameters in the FSMC_NORSRAMInitStruct.
* 输入参数: - FSMC_NORSRAMInitStruct : pointer to a FSMC_NORSRAMInitTypeDef
*                  structure that contains the configuration information for 
*                  the FSMC NOR/SRAM specified Banks.[指向包含着FSMC NOR/SRAM指定体信息的结构体FSMC_NORSRAMInitTypeDef的指针]                       
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_NORSRAMInit(FSMC_NORSRAMInitTypeDef* FSMC_NORSRAMInitStruct)
{ 
  /* Check the parameters [检查参数]*/
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
  
  /* Bank1 NOR/SRAM control register configuration [体1NOR/SRAM控制寄存器的配置]*/ 
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

  /* Bank1 NOR/SRAM timing register configuration [体1NOR/SRAM时间寄存器的配置]*/
  FSMC_Bank1->BTCR[FSMC_NORSRAMInitStruct->FSMC_Bank+1] = 
            (u32)FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressSetupTime |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressHoldTime << 4) |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataSetupTime << 8) |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_BusTurnAroundDuration << 16) |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_CLKDivision << 20) |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataLatency << 24) |
             FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AccessMode;
            

    
  /* Bank1 NOR/SRAM timing register for write configuration, if extended mode is used [如果使用扩展模式，体1NOR/SRAM时间寄存器的写配置]*/
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
* 函数名称: FSMC_NANDInit
* 功能描述: Initializes the FSMC NAND Banks according to the specified
*                  parameters in the FSMC_NANDInitStruct[依照FSMC_NANDInitStruct结构体指定的参数初始化FSMC NAND].
* 输入参数: - FSMC_NANDInitStruct : pointer to a FSMC_NANDInitTypeDef
*                    structure that contains the configuration information for 
*                    the FSMC NAND specified Banks[指向包含着FSMC NAND指定体信息的结构体FSMC_NANDInitTypeDef的指针].                       
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_NANDInit(FSMC_NANDInitTypeDef* FSMC_NANDInitStruct)
{
  u32 tmppcr = 0x00000000, tmppmem = 0x00000000, tmppatt = 0x00000000; 
    
  /* Check the parameters [检查参数]*/
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
  
  /* Set the tmppcr value according to FSMC_NANDInitStruct parameters [依照FSMC_NANDInitStruct的参数设置tmppcr的值]*/
  tmppcr = (u32)FSMC_NANDInitStruct->FSMC_Waitfeature |
            PCR_MemoryType_NAND |
            FSMC_NANDInitStruct->FSMC_MemoryDataWidth |
            FSMC_NANDInitStruct->FSMC_ECC |
            FSMC_NANDInitStruct->FSMC_ECCPageSize |
            FSMC_NANDInitStruct->FSMC_AddressLowMapping |
            (FSMC_NANDInitStruct->FSMC_TCLRSetupTime << 9 )|
            (FSMC_NANDInitStruct->FSMC_TARSetupTime << 13);
            
  /* Set tmppmem value according to FSMC_CommonSpaceTimingStructure parameters [依照FSMC_CommonSpaceTimingStructure的参数设置tmppmem的值]*/
  tmppmem = (u32)FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_SetupTime |
            (FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
            (FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
            (FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HiZSetupTime << 24); 
            
  /* Set tmppatt value according to FSMC_AttributeSpaceTimingStructure parameters [依照FSMC_AttributeSpaceTimingStructure的参数设置tmppatt的值]*/
  tmppatt = (u32)FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_SetupTime |
            (FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
            (FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
            (FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HiZSetupTime << 24);
  
  if(FSMC_NANDInitStruct->FSMC_Bank == FSMC_Bank2_NAND)
  {
    /* FSMC_Bank2_NAND registers configuration [FSMC_Bank2_NAND寄存器配置]*/
    FSMC_Bank2->PCR2 = tmppcr;
    FSMC_Bank2->PMEM2 = tmppmem;
    FSMC_Bank2->PATT2 = tmppatt;
  }
  else
  {
    /* FSMC_Bank3_NAND registers configuration [FSMC_Bank3_NAND寄存器配置]*/
    FSMC_Bank3->PCR3 = tmppcr;
    FSMC_Bank3->PMEM3 = tmppmem;
    FSMC_Bank3->PATT3 = tmppatt;
  }
}

/*******************************************************************************
* 函数名称: FSMC_PCCARDInit
* 功能描述: Initializes the FSMC PCCARD Bank according to the specified
*                  parameters in the FSMC_PCCARDInitStruct[依照FSMC_PCCARDInitStruct结构体指定的参数初始化FSMC PCCARD].
* 输入参数: - FSMC_PCCARDInitStruct : pointer to a FSMC_PCCARDInitTypeDef
*                    structure that contains the configuration information for 
*                    the FSMC PCCARD Bank.[指向包含着FSMC PCCARD指定体信息的结构体FSMC_PCCARDInitTypeDef的指针]                       
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_PCCARDInit(FSMC_PCCARDInitTypeDef* FSMC_PCCARDInitStruct)
{
  /* Check the parameters [检查参数]*/
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
  
  /* Set the PCR4 register value according to FSMC_PCCARDInitStruct parameters [依照FSMC_PCCARDInitStruct的参数设置PCR4寄存器的值]*/
  FSMC_Bank4->PCR4 = (u32)FSMC_PCCARDInitStruct->FSMC_Waitfeature |
                     FSMC_PCCARDInitStruct->FSMC_AddressLowMapping |
                     (FSMC_PCCARDInitStruct->FSMC_TCLRSetupTime << 9) |
                     (FSMC_PCCARDInitStruct->FSMC_TARSetupTime << 13);
            
  /* Set PMEM4 register value according to FSMC_CommonSpaceTimingStructure parameters [依照FSMC_CommonSpaceTimingStructure的参数设置PMEM4寄存器的值]*/
  FSMC_Bank4->PMEM4 = (u32)FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_SetupTime |
                      (FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
                      (FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
                      (FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HiZSetupTime << 24); 
            
  /* Set PATT4 register value according to FSMC_AttributeSpaceTimingStructure parameters [依照FSMC_AttributeSpaceTimingStructure的参数设置PATT4寄存器的值]*/
  FSMC_Bank4->PATT4 = (u32)FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_SetupTime |
                      (FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
                      (FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
                      (FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HiZSetupTime << 24);    
            
  /* Set PIO4 register value according to FSMC_IOSpaceTimingStructure parameters [依照FSMC_IOSpaceTimingStructure的参数设置PIO4寄存器的值]*/
  FSMC_Bank4->PIO4 = (u32)FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_SetupTime |
                     (FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
                     (FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
                     (FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_HiZSetupTime << 24);             
}

/*******************************************************************************
* 函数名称: FSMC_NORSRAMStructInit
* 功能描述: Fills each FSMC_NORSRAMInitStruct member with its default value[用默认值填充FSMC_NORSRAMInitStruct的成员].
* 输入参数: - FSMC_NORSRAMInitStruct: pointer to a FSMC_NORSRAMInitTypeDef
*                    structure which will be initialized.[指向将要被初始化的结构体FSMC_NORSRAMInitTypeDef的指针]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_NORSRAMStructInit(FSMC_NORSRAMInitTypeDef* FSMC_NORSRAMInitStruct)
{  
  /* Reset NOR/SRAM Init structure parameters values [复位NOR/SRAM初始化结构参数值]*/
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
* 函数名称: FSMC_NANDStructInit
* 功能描述: Fills each FSMC_NANDInitStruct member with its default value[用默认值填充FSMC_NANDInitStruct的每一个成员].
* 输入参数: - FSMC_NORSRAMInitStruct: pointer to a FSMC_NANDInitTypeDef
*                    structure which will be initialized.[指向将要被初始化的结构体FSMC_NANDInitTypeDef的指针]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_NANDStructInit(FSMC_NANDInitTypeDef* FSMC_NANDInitStruct)
{ 
  /* Reset NAND Init structure parameters values [复位NAND初始化结构参数值]*/
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
* 函数名称: FSMC_PCCARDStructInit
* 功能描述: Fills each FSMC_PCCARDInitStruct member with its default value.[用默认值填充FSMC_PCCARDInitStruct的每一个成员]
* 输入参数: - FSMC_PCCARDInitStruct: pointer to a FSMC_PCCARDInitTypeDef
*                    structure which will be initialized.[指向将要被初始化的结构体FSMC_PCCARDInitTypeDef的指针]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_PCCARDStructInit(FSMC_PCCARDInitTypeDef* FSMC_PCCARDInitStruct)
{
  /* Reset PCCARD Init structure parameters values [复位PCCARD初始化结构参数值]*/
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
* 函数名称: FSMC_NORSRAMCmd
* 功能描述: Enables or disables the specified NOR/SRAM Memory Bank[使能或禁止指定的NOR/SRAM内存体].
* 输入参数: (1) FSMC_Bank: specifies the FSMC Bank to be used[指定使用的FSMC体]
*                    这个参数可以是下面的值之一:
*                       - FSMC_Bank1_NORSRAM1: FSMC Bank1 NOR/SRAM1  
*                       - FSMC_Bank1_NORSRAM2: FSMC Bank1 NOR/SRAM2 
*                       - FSMC_Bank1_NORSRAM3: FSMC Bank1 NOR/SRAM3 
*                       - FSMC_Bank1_NORSRAM4: FSMC Bank1 NOR/SRAM4 
*           (2) NewState: new state of the FSMC_Bank.[FSMC_Bank的新状态]
*                    This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_NORSRAMCmd(u32 FSMC_Bank, FunctionalState NewState)
{
  assert_param(IS_FSMC_NORSRAM_BANK(FSMC_Bank));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected NOR/SRAM Bank by setting the PBKEN bit in the BCRx register [通过置位BCRx寄存器中的PBKEN位使能选择的NOR/SRAM体]*/
    FSMC_Bank1->BTCR[FSMC_Bank] |= BCR_MBKEN_Set;
  }
  else
  {
    /* Disable the selected NOR/SRAM Bank by clearing the PBKEN bit in the BCRx register [通过清除BCRx寄存器中的PBKEN位禁止选择的NOR/SRAM体]*/
    FSMC_Bank1->BTCR[FSMC_Bank] &= BCR_MBKEN_Reset;
  }
}

/*******************************************************************************
* 函数名称: FSMC_NANDCmd
* 功能描述: Enables or disables the specified NAND Memory Bank[使能或禁止指定的NAND内存体].
* 输入参数: (1) FSMC_Bank: specifies the FSMC Bank to be used[指定使用的FSMC体]
*                    这个参数可以是下面的值之一:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
*           (2) NewState: new state of the FSMC_Bank.[FSMC_Bank的新状态]
*                    This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_NANDCmd(u32 FSMC_Bank, FunctionalState NewState)
{
  assert_param(IS_FSMC_NAND_BANK(FSMC_Bank));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected NAND Bank by setting the PBKEN bit in the PCRx register [通过置位PCRx寄存器中的PBKEN位使能选择的NAND体]*/
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
    /* Disable the selected NAND Bank by clearing the PBKEN bit in the PCRx register [通过清除PCRx寄存器中的PBKEN位禁止选择的NAND体]*/
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
* 函数名称: FSMC_PCCARDCmd
* 功能描述: Enables or disables the PCCARD Memory Bank[使能或禁止指定的PCCARD内存体].
* 输入参数:  NewState: new state of the PCCARD Memory Bank.[PCCARD 内存体的新状态]
*                    This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_PCCARDCmd(FunctionalState NewState)
{
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the PCCARD Bank by setting the PBKEN bit in the PCR4 register [通过置位PCR4寄存器中的PBKEN位使能选择的PCCARD体]*/
    FSMC_Bank4->PCR4 |= PCR_PBKEN_Set;
  }
  else
  {
    /* Disable the PCCARD Bank by clearing the PBKEN bit in the PCR4 register [通过清除PCR4寄存器中的PBKEN位禁止选择的PCCARD体]*/
    FSMC_Bank4->PCR4 &= PCR_PBKEN_Reset;
  }
}

/*******************************************************************************
* 函数名称: FSMC_NANDECCCmd
* 功能描述: Enables or disables the FSMC NAND ECC feature.[使能或禁止FSMC NAND ECC功能]
* 输入参数: - FSMC_Bank: specifies the FSMC Bank to be used[指定使用的FSMC体]
*                    这个参数可以是下面的值之一:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
*                : - NewState: new state of the FSMC NAND ECC feature.[FSMC NAND ECC功能的新状态]  
*                    This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_NANDECCCmd(u32 FSMC_Bank, FunctionalState NewState)
{
  assert_param(IS_FSMC_NAND_BANK(FSMC_Bank));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected NAND Bank ECC function by setting the ECCEN bit in the PCRx register [通过置位PCRx寄存器中的ECCEN位使能选择的NAND体的ECC功能]*/
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
    /* Disable the selected NAND Bank ECC function by clearing the ECCEN bit in the PCRx register [通过清除PCRx寄存器中的ECCEN位禁止选择的NAND体的ECC功能]*/
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
* 函数名称: FSMC_GetECC
* 功能描述: Returns the error correction code register value[返回纠错代码寄存器的值].
* 输入参数: - FSMC_Bank: specifies the FSMC Bank to be used[指定使用的FSMC体]
*                    这个参数可以是下面的值之一:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
* 输出参数: 无
* 返回参数: The Error Correction Code (ECC) value[纠错代码值].
*******************************************************************************/
u32 FSMC_GetECC(u32 FSMC_Bank)
{
  u32 eccval = 0x00000000;
  
  if(FSMC_Bank == FSMC_Bank2_NAND)
  {
    /* Get the ECCR2 register value [取得ECCR2寄存器值]*/
    eccval = FSMC_Bank2->ECCR2;
  }
  else
  {
    /* Get the ECCR3 register value [取得ECCR3寄存器值]*/
    eccval = FSMC_Bank3->ECCR3;
  }
  /* Return the error correction code value [返回纠错代码的值]*/
  return(eccval);
}

/*******************************************************************************
* 函数名称: FSMC_ITConfig
* 功能描述: Enables or disables the specified FSMC interrupts[使能或禁止指定的FSMC中断].
* 输入参数: (1) FSMC_Bank: specifies the FSMC Bank to be used[指定使用的FSMC体]
*                    这个参数可以是下面的值之一:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
*                       - FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
*           (2) FSMC_IT: specifies the FSMC interrupt sources to be
*                    enabled or disabled.[使能或禁止指定的FSMC中断源]
*                    这个参数可以是下面任何值的组合：
*                       - FSMC_IT_RisingEdge: Rising edge detection interrupt[上升沿检测中断]. 
*                       - FSMC_IT_Level: Level edge detection interrupt[下降沿检测中断].                                  
*                       - FSMC_IT_FallingEdge: Falling edge detection interrupt[电平变化检测中断].
*           (3) NewState: new state of the specified FSMC interrupts.[指定FSMC中断的新状态]
*                    This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_ITConfig(u32 FSMC_Bank, u32 FSMC_IT, FunctionalState NewState)
{
  assert_param(IS_FSMC_IT_BANK(FSMC_Bank));
  assert_param(IS_FSMC_IT(FSMC_IT));    
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected FSMC_Bank2 interrupts [使能选择的FSMC_Bank2中断]*/
    if(FSMC_Bank == FSMC_Bank2_NAND)
    {
      FSMC_Bank2->SR2 |= FSMC_IT;
    }
    /* Enable the selected FSMC_Bank3 interrupts [使能选择的FSMC_Bank3中断]*/
    else if (FSMC_Bank == FSMC_Bank3_NAND)
    {
      FSMC_Bank3->SR3 |= FSMC_IT;
    }
    /* Enable the selected FSMC_Bank4 interrupts [使能选择的FSMC_Bank4中断]*/
    else
    {
      FSMC_Bank4->SR4 |= FSMC_IT;    
    }
  }
  else
  {
    /* Disable the selected FSMC_Bank2 interrupts [禁止选择的FSMC_Bank2中断]*/
    if(FSMC_Bank == FSMC_Bank2_NAND)
    {
      
      FSMC_Bank2->SR2 &= (u32)~FSMC_IT;
    }
    /* Disable the selected FSMC_Bank3 interrupts [禁止选择的FSMC_Bank3中断]*/
    else if (FSMC_Bank == FSMC_Bank3_NAND)
    {
      FSMC_Bank3->SR3 &= (u32)~FSMC_IT;
    }
    /* Disable the selected FSMC_Bank4 interrupts [禁止选择的FSMC_Bank4中断]*/
    else
    {
      FSMC_Bank4->SR4 &= (u32)~FSMC_IT;    
    }
  }
}
                  
/*******************************************************************************
* 函数名称: FSMC_GetFlagStatus
* 功能描述: Checks whether the specified FSMC flag is set or not[检查指定的FSMC标志是否置位].
* 输入参数: - FSMC_Bank: specifies the FSMC Bank to be used[指定使用的FSMC体]
*                    这个参数可以是下面的值之一:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
*                       - FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
*                  - FSMC_FLAG: specifies the flag to check.[待检查的指定标志]
*                    这个参数可以是下面的值之一:
*                       - FSMC_FLAG_RisingEdge: Rising egde detection Flag.[上升沿检测标志]
*                       - FSMC_FLAG_Level: Level detection Flag.[电平检测标志]
*                       - FSMC_FLAG_FallingEdge: Falling egde detection Flag[下降沿检测标志].
*                       - FSMC_FLAG_FEMPT: Fifo empty Flag[Fifo空标志]. 
* 输出参数: 无
* 返回参数: The new state of FSMC_FLAG (SET or RESET).[指定FSMC_FLAG的新状态(SET or RESET)]
*******************************************************************************/                   
FlagStatus FSMC_GetFlagStatus(u32 FSMC_Bank, u32 FSMC_FLAG)
{
  FlagStatus bitstatus = RESET;
  u32 tmpsr = 0x00000000;
  
  /* Check the parameters [检查参数]*/
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
  
  /* Get the flag status [取得标志状态]*/
  if ((tmpsr & FSMC_FLAG) != (u16)RESET )
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* Return the flag status [返回标志状态]*/
  return bitstatus;
}

/*******************************************************************************
* 函数名称: FSMC_ClearFlag
* 功能描述: Clears the FSMC’s pending flags.[清除FSMC挂起标志]
* 输入参数: - FSMC_Bank: specifies the FSMC Bank to be used[指定使用的FSMC体]
*                    这个参数可以是下面的值之一:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
*                       - FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
*                  - FSMC_FLAG: specifies the flag to clear.[要清除的指定标志]
*                    这个参数可以是下面任何值的组合：
*                       - FSMC_FLAG_RisingEdge: Rising egde detection Flag.[上升沿标志]
*                       - FSMC_FLAG_Level: Level detection Flag.[电平检测标志]
*                       - FSMC_FLAG_FallingEdge: Falling egde detection Flag[下降沿标志].
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/                   
void FSMC_ClearFlag(u32 FSMC_Bank, u32 FSMC_FLAG)
{
 /* Check the parameters [检查参数]*/
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
* 函数名称: FSMC_GetITStatus
* 功能描述: Checks whether the specified FSMC interrupt has occurred or not[检查指定的FSMC中断是否已经发生].
* 输入参数: - FSMC_Bank: specifies the FSMC Bank to be used[指定使用的FSMC体]
*                    这个参数可以是下面的值之一:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
*                       - FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
*                  - FSMC_IT: specifies the FSMC interrupt source to check[要检查的指定FSMC中断源].
*                    这个参数可以是下面的值之一:
*                       - FSMC_IT_RisingEdge: Rising edge detection interrupt[上升沿中断]. 
*                       - FSMC_IT_Level: Level edge detection interrupt.[电平检测中断]                                  
*                       - FSMC_IT_FallingEdge: Falling edge detection interrupt[下降沿中断]. 
* 输出参数: 无
* 返回参数: The new state of FSMC_IT (SET or RESET).
*******************************************************************************/ 
ITStatus FSMC_GetITStatus(u32 FSMC_Bank, u32 FSMC_IT)
{
  ITStatus bitstatus = RESET;
  u32 tmpsr = 0x0, itstatus = 0x0, itenable = 0x0; 
  
  /* Check the parameters [检查参数]*/
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
* 函数名称: FSMC_ClearITPendingBit
* 功能描述: Clears the FSMC'S interrupt pending bits.[清除FSMC中断挂起位]
* 输入参数: - FSMC_Bank: specifies the FSMC Bank to be used[指定使用的FSMC体]
*                    这个参数可以是下面的值之一:
*                       - FSMC_Bank2_NAND: FSMC Bank2 NAND 
*                       - FSMC_Bank3_NAND: FSMC Bank3 NAND
*                       - FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
*                  - FSMC_IT: specifies the interrupt pending bit to clear[要清除的指定中断挂起位].
*                    这个参数可以是下面任何值的组合：
*                       - FSMC_IT_RisingEdge: Rising edge detection interrupt[上升沿中断]. 
*                       - FSMC_IT_Level: Level edge detection interrupt.[电平检测中断]                                  
*                       - FSMC_IT_FallingEdge: Falling edge detection interrupt.[下降沿中断]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void FSMC_ClearITPendingBit(u32 FSMC_Bank, u32 FSMC_IT)
{
  /* Check the parameters [检查参数]*/
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
