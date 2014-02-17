/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_sdio.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有SDIO的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_sdio.h"
#include "stm32f10x_rcc.h"

/* 自用类型 -----------------------------------------------------------*/
/* ------------ SDIO registers bit address in the alias region ----------- */
#define SDIO_OFFSET                (SDIO_BASE - PERIPH_BASE)

/* --- CLKCR Register ---*/
/* Alias word address of CLKEN bit */
#define CLKCR_OFFSET              (SDIO_OFFSET + 0x04)
#define CLKEN_BitNumber           0x08
#define CLKCR_CLKEN_BB            (PERIPH_BB_BASE + (CLKCR_OFFSET * 32) + (CLKEN_BitNumber * 4))

/* --- CMD Register ---*/
/* Alias word address of SDIOSUSPEND bit */
#define CMD_OFFSET                (SDIO_OFFSET + 0x0C)
#define SDIOSUSPEND_BitNumber     0x0B
#define CMD_SDIOSUSPEND_BB        (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (SDIOSUSPEND_BitNumber * 4))

/* Alias word address of ENCMDCOMPL bit */
#define ENCMDCOMPL_BitNumber      0x0C
#define CMD_ENCMDCOMPL_BB         (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (ENCMDCOMPL_BitNumber * 4))

/* Alias word address of NIEN bit */
#define NIEN_BitNumber            0x0D
#define CMD_NIEN_BB               (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (NIEN_BitNumber * 4))

/* Alias word address of ATACMD bit */
#define ATACMD_BitNumber          0x0E
#define CMD_ATACMD_BB             (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (ATACMD_BitNumber * 4))

/* --- DCTRL Register ---*/
/* Alias word address of DMAEN bit */
#define DCTRL_OFFSET              (SDIO_OFFSET + 0x2C)
#define DMAEN_BitNumber           0x03
#define DCTRL_DMAEN_BB            (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (DMAEN_BitNumber * 4))

/* Alias word address of RWSTART bit */
#define RWSTART_BitNumber         0x08
#define DCTRL_RWSTART_BB          (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (RWSTART_BitNumber * 4))

/* Alias word address of RWSTOP bit */
#define RWSTOP_BitNumber          0x09
#define DCTRL_RWSTOP_BB           (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (RWSTOP_BitNumber * 4))

/* Alias word address of RWMOD bit */
#define RWMOD_BitNumber           0x0A
#define DCTRL_RWMOD_BB            (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (RWMOD_BitNumber * 4))

/* Alias word address of SDIOEN bit */
#define SDIOEN_BitNumber          0x0B
#define DCTRL_SDIOEN_BB           (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (SDIOEN_BitNumber * 4))


/* ---------------------- SDIO registers bit mask ------------------------ */
/* --- CLKCR Register ---*/
/* CLKCR register clear mask */
#define CLKCR_CLEAR_MASK         ((u32)0xFFFF8100) 

/* --- PWRCTRL Register ---*/
/* SDIO PWRCTRL Mask */
#define PWR_PWRCTRL_MASK         ((u32)0xFFFFFFFC)

/* --- DCTRL Register ---*/
/* SDIO DCTRL Clear Mask */
#define DCTRL_CLEAR_MASK         ((u32)0xFFFFFF08)

/* --- CMD Register ---*/
/* CMD Register clear mask */
#define CMD_CLEAR_MASK           ((u32)0xFFFFF800)

/* SDIO RESP Registers Address */
#define SDIO_RESP_ADDR           ((u32)(SDIO_BASE + 0x14))

/* 自用定义 ---------------------------------------------------------*/
/* 自用宏 -----------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: SDIO_DeInit
* 功能描述: Deinitializes the SDIO peripheral registers to their default
*                  reset values[取消SDIO设备寄存器的初始化还原到他们的默认值].
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_DeInit(void)
{
  SDIO->POWER = 0x00000000;
  SDIO->CLKCR = 0x00000000;
  SDIO->ARG = 0x00000000;
  SDIO->CMD = 0x00000000;
  SDIO->DTIMER = 0x00000000;
  SDIO->DLEN = 0x00000000;
  SDIO->DCTRL = 0x00000000;
  SDIO->ICR = 0x00C007FF;
  SDIO->MASK = 0x00000000;
}

/*******************************************************************************
* 函数名称: SDIO_Init
* 功能描述: Initializes the SDIO peripheral according to the specified
*                  parameters in the SDIO_InitStruct[依照SDIO_InitStruct结构体指定的参数初始化SDIO设备].
* 输入参数: SDIO_InitStruct : pointer to a SDIO_InitTypeDef structure
*                  that contains the configuration information for the SDIO 
*                  peripheral.[指向包含着SDIO设备配置信息的结构体SDIO_InitTypeDef的指针]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_Init(SDIO_InitTypeDef* SDIO_InitStruct)
{
  u32 tmpreg = 0;
    
  /* Check the parameters [检查参数]*/
  assert_param(IS_SDIO_CLOCK_EDGE(SDIO_InitStruct->SDIO_ClockEdge));
  assert_param(IS_SDIO_CLOCK_BYPASS(SDIO_InitStruct->SDIO_ClockBypass));
  assert_param(IS_SDIO_CLOCK_POWER_SAVE(SDIO_InitStruct->SDIO_ClockPowerSave));
  assert_param(IS_SDIO_BUS_WIDE(SDIO_InitStruct->SDIO_BusWide));
  assert_param(IS_SDIO_HARDWARE_FLOW_CONTROL(SDIO_InitStruct->SDIO_HardwareFlowControl)); 
   
/*---------------------------- SDIO CLKCR Configuration ------------------------*/  
  /* Get the SDIO CLKCR value [取得SDIO CLKCR的值]*/
  tmpreg = SDIO->CLKCR;
  
  /* Clear CLKDIV, PWRSAV, BYPASS, WIDBUS, NEGEDGE, HWFC_EN bits [清CLKDIV, PWRSAV, BYPASS, WIDBUS, NEGEDGE, HWFC_EN位]*/
  tmpreg &= CLKCR_CLEAR_MASK;
  
  /* Set CLKDIV bits according to SDIO_ClockDiv value [依照SDIO_ClockDiv的值设置CLKDIV位]*/
  /* Set PWRSAV bit according to SDIO_ClockPowerSave value [依照SDIO_ClockPowerSave的值设置PWRSAV位]*/
  /* Set BYPASS bit according to SDIO_ClockBypass value [依照SDIO_ClockBypass的值设置BYPASS位]*/
  /* Set WIDBUS bits according to SDIO_BusWide value [依照SDIO_BusWide的值设置WIDBUS位]*/
  /* Set NEGEDGE bits according to SDIO_ClockEdge value [依照SDIO_ClockEdge的值设置NEGEDGE位]*/
  /* Set HWFC_EN bits according to SDIO_HardwareFlowControl value [依照SDIO_HardwareFlowControl的值设置HWFC_EN位]*/
  tmpreg |= (SDIO_InitStruct->SDIO_ClockDiv  | SDIO_InitStruct->SDIO_ClockPowerSave |
             SDIO_InitStruct->SDIO_ClockBypass | SDIO_InitStruct->SDIO_BusWide |
             SDIO_InitStruct->SDIO_ClockEdge | SDIO_InitStruct->SDIO_HardwareFlowControl); 
  
  /* Write to SDIO CLKCR [写SDIO CLKCR]*/
  SDIO->CLKCR = tmpreg;             
}

/*******************************************************************************
* 函数名称: SDIO_StructInit
* 功能描述: Fills each SDIO_InitStruct member with its default value.[用默认值填充SDIO_InitStruct的每一个成员]
* 输入参数: SDIO_InitStruct: pointer to an SDIO_InitTypeDef structure which
*                  will be initialized.[指向将要被初始化的结构体SDIO_InitTypeDef的指针]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_StructInit(SDIO_InitTypeDef* SDIO_InitStruct)
{
  /* SDIO_InitStruct members default value [SDIO_InitStruct成员的默认值]*/
  SDIO_InitStruct->SDIO_ClockDiv = 0x00;
  SDIO_InitStruct->SDIO_ClockEdge = SDIO_ClockEdge_Rising;
  SDIO_InitStruct->SDIO_ClockBypass = SDIO_ClockBypass_Disable;
  SDIO_InitStruct->SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
  SDIO_InitStruct->SDIO_BusWide = SDIO_BusWide_1b;
  SDIO_InitStruct->SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
}

/*******************************************************************************
* 函数名称: SDIO_ClockCmd
* 功能描述: Enables or disables the SDIO Clock.[使能或禁止SDIO时钟]
* 输入参数: NewState: new state of the SDIO Clock.[SDIO时钟的新状态]
*                  This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_ClockCmd(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) CLKCR_CLKEN_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: SDIO_SetPowerState
* 功能描述: Sets the power status of the controller[设置控制器的电源状态].
* 输入参数: SDIO_PowerState: new state of the Power state.[新的电源状态]
*                  这个参数可以是下面的值之一:
*                   - SDIO_PowerState_OFF
*                   - SDIO_PowerState_ON
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_SetPowerState(u32 SDIO_PowerState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SDIO_POWER_STATE(SDIO_PowerState));
  
  SDIO->POWER &= PWR_PWRCTRL_MASK;
  SDIO->POWER |= SDIO_PowerState;
}

/*******************************************************************************
* 函数名称: SDIO_GetPowerState
* 功能描述: Gets the power status of the controller[取得控制器的电源状态].
* 输入参数: 无
* 输出参数: 无
* 返回参数: Power status of the controller[控制器的电源状态]. The returned value can
*                  be one of the following:[返回值可以是下面的值之一]
*                       - 0x00: Power OFF
*                       - 0x02: Power UP
*                       - 0x03: Power ON 
*******************************************************************************/
u32 SDIO_GetPowerState(void)
{
  return (SDIO->POWER & (~PWR_PWRCTRL_MASK));
}

/*******************************************************************************
* 函数名称: SDIO_ITConfig
* 功能描述: Enables or disables the SDIO interrupts.[使能或禁止SDIO中断]
* 输入参数: (1) SDIO_IT: specifies the SDIO interrupt sources to be
*                    enabled or disabled.[使能或禁止指定的SDIO中断源]
*                    This parameter can be one or a combination of the following
*                    values:[这个参数可以使用下述值的一个或者几个值的组合]
*                      - SDIO_IT_CCRCFAIL: Command response received (CRC check
*                                          failed) interrupt[命令响应接收(CRC校验失败)中断]    
*                      - SDIO_IT_DCRCFAIL: Data block sent/received (CRC check 
*                                          failed) interrupt[数据块发送/接收(CRC校验失败)中断]    
*                      - SDIO_IT_CTIMEOUT: Command response timeout interrupt[命令响应超时中断]     
*                      - SDIO_IT_DTIMEOUT: Data timeout interrupt[数据超时中断]     
*                      - SDIO_IT_TXUNDERR: Transmit FIFO underrun error interrupt[发送FIFO空栈读出错中断]    
*                      - SDIO_IT_RXOVERR:  Received FIFO overrun error interrupt[接收FIFO超速出错中断]      
*                      - SDIO_IT_CMDREND:  Command response received (CRC check 
*                                          passed) interrupt[命令响应接收(CRC校验通过)中断]     
*                      - SDIO_IT_CMDSENT:  Command sent (no response required) 
*                                          interrupt[命令发送(缺少必要的响应)中断]     
*                      - SDIO_IT_DATAEND:  Data end (data counter, SDIDCOUNT, is 
*                                          zero) interrupt [数据结束(数据计数器，SDIO计数器等于零)中断]    
*                      - SDIO_IT_STBITERR: Start bit not detected on all data [在宽总线模式中从所有的数据信号中找不到开始位中断]
*                                          signals in wide bus mode interrupt    
*                      - SDIO_IT_DBCKEND:  Data block sent/received (CRC check 
*                                          passed) interrupt[数据块发送/接收(CRC校验通过)中断]    
*                      - SDIO_IT_CMDACT:   Command transfer in progress interrupt[命令发送过程 中断]     
*                      - SDIO_IT_TXACT:    Data transmit in progress interrupt[数据发送过程 中断]       
*                      - SDIO_IT_RXACT:    Data receive in progress interrupt[数据接收过程 中断]      
*                      - SDIO_IT_TXFIFOHE: Transmit FIFO Half Empty interrupt[发送FIFO半空 中断]    
*                      - SDIO_IT_RXFIFOHF: Receive FIFO Half Full interrupt[接收FIFO半满 中断]   
*                      - SDIO_IT_TXFIFOF:  Transmit FIFO full interrupt[发送FIFO满 中断]     
*                      - SDIO_IT_RXFIFOF:  Receive FIFO full interrupt[接收FIFO满 中断]     
*                      - SDIO_IT_TXFIFOE:  Transmit FIFO empty interrupt[发送FIFO空 中断]      
*                      - SDIO_IT_RXFIFOE:  Receive FIFO empty interrupt[接收FIFO空 中断]     
*                      - SDIO_IT_TXDAVL:   Data available in transmit FIFO interrupt[发送FIFO中数据可用 中断]      
*                      - SDIO_IT_RXDAVL:   Data available in receive FIFO interrupt[接收FIFO中数据可用 中断]      
*                      - SDIO_IT_SDIOIT:   SD I/O interrupt received interrupt[SD I/O中断 接收中断]      
*                      - SDIO_IT_CEATAEND: CE-ATA command completion signal[CMD61的CE-ATA命令完成信号接收中断] 
*                                          received for CMD61 interrupt
*           (2) NewState: new state of the specified SDIO interrupts.[指定的SDIO中断的新状态]
*                  This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]  
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_ITConfig(u32 SDIO_IT, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SDIO_IT(SDIO_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the SDIO interrupts [使能SDIO中断]*/
    SDIO->MASK |= SDIO_IT;
  }
  else
  {
    /* Disable the SDIO interrupts [禁止SDIO中断]*/
    SDIO->MASK &= ~SDIO_IT;
  } 
}

/*******************************************************************************
* 函数名称: SDIO_DMACmd
* 功能描述: Enables or disables the SDIO DMA request.[使能或禁止SDIO DMA请求]
* 输入参数: NewState: new state of the selected SDIO DMA request.[指定SDIO DMA 请求的新状态]
*                  This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_DMACmd(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) DCTRL_DMAEN_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: SDIO_SendCommand
* 功能描述: Initializes the SDIO Command according to the specified[依照SDIO_CmdInitStruct和发送命令中指定的参数初始化SDIO命令]
*                  parameters in the SDIO_CmdInitStruct and send the command.
* 输入参数: SDIO_CmdInitStruct : pointer to a SDIO_CmdInitTypeDef
*                  structure that contains the configuration information 
*                  for the SDIO command.[指向包含着SDIO命令配置信息的结构体SDIO_CmdInitTypeDef的指针]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_SendCommand(SDIO_CmdInitTypeDef *SDIO_CmdInitStruct)
{
  u32 tmpreg = 0;
  
  /* Check the parameters [检查参数]*/
  assert_param(IS_SDIO_CMD_INDEX(SDIO_CmdInitStruct->SDIO_CmdIndex));
  assert_param(IS_SDIO_RESPONSE(SDIO_CmdInitStruct->SDIO_Response));
  assert_param(IS_SDIO_WAIT(SDIO_CmdInitStruct->SDIO_Wait));
  assert_param(IS_SDIO_CPSM(SDIO_CmdInitStruct->SDIO_CPSM));
  
/*---------------------------- SDIO ARG Configuration ------------------------*/
  /* Set the SDIO Argument value [设置SDIO参数值]*/
  SDIO->ARG = SDIO_CmdInitStruct->SDIO_Argument;
  
/*---------------------------- SDIO CMD Configuration ------------------------*/  
  /* Get the SDIO CMD value [取得SDIO CMD值]*/
  tmpreg = SDIO->CMD;

  /* Clear CMDINDEX, WAITRESP, WAITINT, WAITPEND, CPSMEN bits [清CMDINDEX, WAITRESP, WAITINT, WAITPEND, CPSMEN位]*/
  tmpreg &= CMD_CLEAR_MASK;
  /* Set CMDINDEX bits according to SDIO_CmdIndex value [依照SDIO_CmdIndex的值设置CMDINDEX位]*/
  /* Set WAITRESP bits according to SDIO_Response value [依照SDIO_Response的值设置WAITRESP位]*/
  /* Set WAITINT and WAITPEND bits according to SDIO_Wait value [依照SDIO_Wait的值设置WAITINT和WAITPEND位]*/
  /* Set CPSMEN bits according to SDIO_CPSM value [依照SDIO_CPSM的值设置CPSMEN位]*/
  tmpreg |= (u32)SDIO_CmdInitStruct->SDIO_CmdIndex | SDIO_CmdInitStruct->SDIO_Response
           | SDIO_CmdInitStruct->SDIO_Wait | SDIO_CmdInitStruct->SDIO_CPSM;
  
  /* Write to SDIO CMD [写SDIO CMD]*/
  SDIO->CMD = tmpreg;
}

/*******************************************************************************
* 函数名称: SDIO_CmdStructInit
* 功能描述: Fills each SDIO_CmdInitStruct member with its default value.[用默认值填充SDIO_CmdInitStruct的每一个成员]
* 输入参数: SDIO_CmdInitStruct: pointer to an SDIO_CmdInitTypeDef
*                  structure which will be initialized.[指向将要被初始化的结构体SDIO_CmdInitTypeDef的指针]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_CmdStructInit(SDIO_CmdInitTypeDef* SDIO_CmdInitStruct)
{
  /* SDIO_CmdInitStruct members default value [SDIO_CmdInitStruct成员的默认值]*/
  SDIO_CmdInitStruct->SDIO_Argument = 0x00;
  SDIO_CmdInitStruct->SDIO_CmdIndex = 0x00;
  SDIO_CmdInitStruct->SDIO_Response = SDIO_Response_No;
  SDIO_CmdInitStruct->SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct->SDIO_CPSM = SDIO_CPSM_Disable;
}

/*******************************************************************************
* 函数名称: SDIO_GetCommandResponse
* 功能描述: Returns command index of last command for which response
*                  received.[返回最新接收响应命令的命令序号]
* 输入参数: 无
* 输出参数: 无
* 返回参数: Returns the command index of the last command response received.[返回最新接收响应命令的命令序号]
*******************************************************************************/
u8 SDIO_GetCommandResponse(void)
{
  return (u8)(SDIO->RESPCMD);
}

/*******************************************************************************
* 函数名称: SDIO_GetResponse
* 功能描述: Returns response received from the card for the last command.[返回最新命令卡的接收响应]
* 输入参数: - SDIO_RESP: Specifies the SDIO response register.[指定的SDIO响应寄存器]
*                     这个参数可以是下面的值之一:
*                       - SDIO_RESP1: Response Register 1[响应寄存器1]
*                       - SDIO_RESP2: Response Register 2[响应寄存器2]
*                       - SDIO_RESP3: Response Register 3[响应寄存器3]
*                       - SDIO_RESP4: Response Register 4[响应寄存器4]                       
* 输出参数: 无
* 返回参数: The Corresponding response register value.[相应的响应寄存器值]
*******************************************************************************/
u32 SDIO_GetResponse(u32 SDIO_RESP)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SDIO_RESP(SDIO_RESP));
  
  return (*(vu32 *)(SDIO_RESP_ADDR + SDIO_RESP)); 
}

/*******************************************************************************
* 函数名称: SDIO_DataConfig
* 功能描述: Initializes the SDIO data path according to the specified
*                  parameters in the SDIO_DataInitStruct.[依照SDIO_DataInitStruct中指定的参数初始化SDIO数据路径]
* 输入参数: SDIO_DataInitStruct : pointer to a SDIO_DataInitTypeDef
*                  structure that contains the configuration information 
*                  for the SDIO command.[指向包含着SDIO命令配置信息的结构体SDIO_DataInitTypeDef的指针]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_DataConfig(SDIO_DataInitTypeDef* SDIO_DataInitStruct)
{
  u32 tmpreg = 0;
  
  /* Check the parameters [检查参数]*/
  assert_param(IS_SDIO_DATA_LENGTH(SDIO_DataInitStruct->SDIO_DataLength));
  assert_param(IS_SDIO_BLOCK_SIZE(SDIO_DataInitStruct->SDIO_DataBlockSize));
  assert_param(IS_SDIO_TRANSFER_DIR(SDIO_DataInitStruct->SDIO_TransferDir));
  assert_param(IS_SDIO_TRANSFER_MODE(SDIO_DataInitStruct->SDIO_TransferMode));
  assert_param(IS_SDIO_DPSM(SDIO_DataInitStruct->SDIO_DPSM));

/*---------------------------- SDIO DTIMER Configuration ---------------------*/
  /* Set the SDIO Data TimeOut value [设置SDIO数据超时值]*/
  SDIO->DTIMER = SDIO_DataInitStruct->SDIO_DataTimeOut;
    
/*---------------------------- SDIO DLEN Configuration -----------------------*/
  /* Set the SDIO DataLength value [设置SDIO数据长度值]*/
  SDIO->DLEN = SDIO_DataInitStruct->SDIO_DataLength;
  
/*---------------------------- SDIO DCTRL Configuration ----------------------*/  
  /* Get the SDIO DCTRL value [取得SDIO DCTRL值]*/
  tmpreg = SDIO->DCTRL;

  /* Clear DEN, DTMODE, DTDIR and DBCKSIZE bits [清DEN, DTMODE, DTDIR和DBCKSIZE位]*/
  tmpreg &= DCTRL_CLEAR_MASK;
  /* Set DEN bit according to SDIO_DPSM value [依照SDIO_DPSM的值设置DEN位]*/
  /* Set DTMODE bit according to SDIO_TransferMode value [依照SDIO_TransferMode的值设置DTMODE位]*/
  /* Set DTDIR bit according to SDIO_TransferDir value [依照 SDIO_TransferDir的值设置DTDIR位]*/
  /* Set DBCKSIZE bits according to SDIO_DataBlockSize value [依照SDIO_DataBlockSize的值设置DBCKSIZE位]*/
  tmpreg |= (u32)SDIO_DataInitStruct->SDIO_DataBlockSize | SDIO_DataInitStruct->SDIO_TransferDir
           | SDIO_DataInitStruct->SDIO_TransferMode | SDIO_DataInitStruct->SDIO_DPSM;
  
  /* Write to SDIO DCTRL [写SDIO DCTRL]*/
  SDIO->DCTRL = tmpreg;
}

/*******************************************************************************
* 函数名称: SDIO_DataStructInit
* 功能描述: Fills each SDIO_DataInitStruct member with its default value.[用默认值填充SDIO_DataInitStruct的每一个成员]
* 输入参数: SDIO_DataInitStruct: pointer to an SDIO_DataInitTypeDef
*                  structure which will be initialized.[指向包含着SDIO命令配置信息的结构体SDIO_DataInitTypeDef的指针]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_DataStructInit(SDIO_DataInitTypeDef* SDIO_DataInitStruct)
{
  /* SDIO_DataInitStruct members default value [SDIO_DataInitStruct成员的默认值]*/
  SDIO_DataInitStruct->SDIO_DataTimeOut = 0xFFFFFFFF;
  SDIO_DataInitStruct->SDIO_DataLength = 0x00;
  SDIO_DataInitStruct->SDIO_DataBlockSize = SDIO_DataBlockSize_1b;
  SDIO_DataInitStruct->SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStruct->SDIO_TransferMode = SDIO_TransferMode_Block;  
  SDIO_DataInitStruct->SDIO_DPSM = SDIO_DPSM_Disable;
}

/*******************************************************************************
* 函数名称: SDIO_GetDataCounter
* 功能描述: Returns number of remaining data bytes to be transferred.[返回发送剩余数据字节数]
* 输入参数: 无
* 输出参数: 无
* 返回参数: Number of remaining data bytes to be transferred[发送剩余数据字节数]
*******************************************************************************/
u32 SDIO_GetDataCounter(void)
{ 
  return SDIO->DCOUNT;
}

/*******************************************************************************
* 函数名称: SDIO_ReadData
* 功能描述: Read one data word from Rx FIFO.[从Rx FIFO读一个数据字]
* 输入参数: 无
* 输出参数: 无
* 返回参数: Data received[接收的数据]
*******************************************************************************/
u32 SDIO_ReadData(void)
{ 
  return SDIO->FIFO;
}

/*******************************************************************************
* 函数名称: SDIO_WriteData
* 功能描述: Write one data word to Tx FIFO.[向Tx FIFO内写一个数据字]
* 输入参数: Data: 32-bit data word to write.[32位的数据字]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_WriteData(u32 Data)
{ 
  SDIO->FIFO = Data;
}

/*******************************************************************************
* 函数名称: SDIO_GetFIFOCount
* 功能描述: Returns the number of words left to be written to or read
*                  from FIFO.[返回一个FIFO读写剩余的字数]   
* 输入参数: 无
* 输出参数: 无
* 返回参数: Remaining number of words.[剩余的字数]
*******************************************************************************/
u32 SDIO_GetFIFOCount(void)
{ 
  return SDIO->FIFOCNT;
}

/*******************************************************************************
* 函数名称: SDIO_StartSDIOReadWait
* 功能描述: Starts the SD I/O Read Wait operation.[开始SD I/O读等待操作]
* 输入参数: NewState: new state of the Start SDIO Read Wait operation.[启动读等待操作的新状态]
*                  This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_StartSDIOReadWait(FunctionalState NewState)
{ 
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) DCTRL_RWSTART_BB = (u32) NewState;
}

/*******************************************************************************
* 函数名称: SDIO_StopSDIOReadWait
* 功能描述: Stops the SD I/O Read Wait operation.[停止SD I/O读等待操作]
* 输入参数: NewState: new state of the Stop SDIO Read Wait operation.[停止读等待操作的新状态]
*                  This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_StopSDIOReadWait(FunctionalState NewState)
{ 
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) DCTRL_RWSTOP_BB = (u32) NewState;
}

/*******************************************************************************
* 函数名称: SDIO_SetSDIOReadWaitMode
* 功能描述: Sets one of the two options of inserting read wait interval.[设置两个插入读等待时间选项中的一个]
* 输入参数: SDIOReadWaitMode: SD I/O Read Wait operation mode.[SD I/O读等待模式]
*                  This parametre can be:[这个参数可以是]
*                    - SDIO_ReadWaitMode_CLK: Read Wait control by stopping SDIOCLK[停止SDIOCLK控制读等待]
*                    - SDIO_ReadWaitMode_DATA2: Read Wait control using SDIO_DATA2[SDIO_DATA2控制读等待]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_SetSDIOReadWaitMode(u32 SDIO_ReadWaitMode)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SDIO_READWAIT_MODE(SDIO_ReadWaitMode));
  
  *(vu32 *) DCTRL_RWMOD_BB = SDIO_ReadWaitMode;
}

/*******************************************************************************
* 函数名称: SDIO_SetSDIOOperation
* 功能描述: Enables or disables the SD I/O Mode Operation.[使能或者禁止SD I/O模式操作]
* 输入参数: NewState: new state of SDIO specific operation.[SDIO指定操作的新状态]
*                  This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_SetSDIOOperation(FunctionalState NewState)
{ 
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) DCTRL_SDIOEN_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: SDIO_SendSDIOSuspendCmd
* 功能描述: Enables or disables the SD I/O Mode suspend command sending.[使能或者禁止SD I/O模式中止命令发送]
* 输入参数: NewState: new state of the SD I/O Mode suspend command.[SD I/O模式中止命令发的新状态]
*                  This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_SendSDIOSuspendCmd(FunctionalState NewState)
{ 
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) CMD_SDIOSUSPEND_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: SDIO_CommandCompletionCmd
* 功能描述: Enables or disables the command completion signal.[使能或者禁止命令完成信号]
* 输入参数: NewState: new state of command completion signal.[命令完成信号的新状态]
*                  This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_CommandCompletionCmd(FunctionalState NewState)
{ 
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) CMD_ENCMDCOMPL_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: SDIO_CEATAITCmd
* 功能描述: Enables or disables the CE-ATA interrupt.[使能或者禁止CE-ATA中断]
* 输入参数: NewState: new state of CE-ATA interrupt.[CE-ATA中断的新状态]
*                  This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_CEATAITCmd(FunctionalState NewState)
{ 
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) CMD_NIEN_BB = (u32)((~((u32)NewState)) & ((u32)0x1));
}

/*******************************************************************************
* 函数名称: SDIO_SendCEATACmd
* 功能描述: Sends CE-ATA command (CMD61).[发送CE-ATA命令]
* 输入参数: NewState: new state of CE-ATA command.[CE-ATA命令的新状态]
*                  This parameter can be: ENABLE or DISABLE.[这个参数可以是：ENABLE or DISABLE]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_SendCEATACmd(FunctionalState NewState)
{ 
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) CMD_ATACMD_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: SDIO_GetFlagStatus
* 功能描述: Checks whether the specified SDIO flag is set or not.[检查指定的SDIO标志是否置位]
* 输入参数: SDIO_FLAG: specifies the flag to check.[需要检查的指定标志]
*                  这个参数可以是下面的值之一:
*                     - SDIO_FLAG_CCRCFAIL: Command response received (CRC check
*                                           failed) [命令响应接收(CRC校验失败)]   
*                     - SDIO_FLAG_DCRCFAIL: Data block sent/received (CRC check 
*                                           failed)[数据块发送/接收(CRC校验失败)]    
*                     - SDIO_FLAG_CTIMEOUT: Command response timeout[命令响应超时]    
*                     - SDIO_FLAG_DTIMEOUT: Data timeou [数据超时]  
*                     - SDIO_FLAG_TXUNDERR: Transmit FIFO underrun error[发送FIFO空栈读出错]   
*                     - SDIO_FLAG_RXOVERR:  Received FIFO overrun error[接收FIFO超速出错]    
*                     - SDIO_FLAG_CMDREND:  Command response received (CRC check 
*                                           passed)[命令响应接收(CRC校验通过)]     
*                     - SDIO_FLAG_CMDSENT:  Command sent (no response required)[命令发送(缺少必要的响应)]     
*                     - SDIO_FLAG_DATAEND:  Data end (data counter, SDIDCOUNT, is
*                                           zero)[数据结束(数据计数器，SDIO计数器等于零)]    
*                     - SDIO_FLAG_STBITERR: Start bit not detected on all data 
*                                           signals in wide bus mode[在宽总线模式中从所有的数据信号中找不到开始位]   
*                     - SDIO_FLAG_DBCKEND:  Data block sent/received (CRC check 
*                                           passed)[数据块发送/接收(CRC校验通过)]    
*                     - SDIO_FLAG_CMDACT:   Command transfer in progress[命令发送过程]     
*                     - SDIO_FLAG_TXACT:    Data transmit in progress[数据发送过程]      
*                     - SDIO_FLAG_RXACT:    Data receive in progress[数据接收过程]      
*                     - SDIO_FLAG_TXFIFOHE: Transmit FIFO Half Empty[发送FIFO半空]   
*                     - SDIO_FLAG_RXFIFOHF: Receive FIFO Half Full[接收FIFO半满]   
*                     - SDIO_FLAG_TXFIFOF:  Transmit FIFO full[发送FIFO满]    
*                     - SDIO_FLAG_RXFIFOF:  Receive FIFO full[接收FIFO满]     
*                     - SDIO_FLAG_TXFIFOE:  Transmit FIFO empty[发送FIFO空]     
*                     - SDIO_FLAG_RXFIFOE:  Receive FIFO empty[接收FIFO空]     
*                     - SDIO_FLAG_TXDAVL:   Data available in transmit FIFO [发送FIFO中数据可用]     
*                     - SDIO_FLAG_RXDAVL:   Data available in receive FIFO[接收FIFO中数据可用]     
*                     - SDIO_FLAG_SDIOIT:   SD I/O interrupt received[SD I/O中断 接收]     
*                     - SDIO_FLAG_CEATAEND: CE-ATA command completion signal 
*                                           received for CMD61 [CMD61的CE-ATA命令完成信号接收]    
* 输出参数: 无
* 返回参数: The new state of SDIO_FLAG (SET or RESET).
*******************************************************************************/
FlagStatus SDIO_GetFlagStatus(u32 SDIO_FLAG)
{ 
  FlagStatus bitstatus = RESET;
  
  /* Check the parameters [检查参数]*/
  assert_param(IS_SDIO_FLAG(SDIO_FLAG));
  
  if ((SDIO->STA & SDIO_FLAG) != (u32)RESET)
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
* 函数名称: SDIO_ClearFlag
* 功能描述: Clears the SDIO's pending flags.[清除SDIO挂起标志]
* 输入参数: SDIO_FLAG: specifies the flag to clear.[要清除的指定标志]
*                  This parameter can be one or a combination of the following
*                  values:[可以使用下述值的一个或者几个值的组合]
*                     - SDIO_FLAG_CCRCFAIL: Command response received (CRC check
*                                           failed) [命令响应接收(CRC校验失败)]   
*                     - SDIO_FLAG_DCRCFAIL: Data block sent/received (CRC check 
*                                           failed)[数据块发送/接收(CRC校验失败)]    
*                     - SDIO_FLAG_CTIMEOUT: Command response timeout [命令响应超时]   
*                     - SDIO_FLAG_DTIMEOUT: Data timeou [数据超时]  
*                     - SDIO_FLAG_TXUNDERR: Transmit FIFO underrun error [发送FIFO空栈读出错]  
*                     - SDIO_FLAG_RXOVERR:  Received FIFO overrun error[接收FIFO超速出错]    
*                     - SDIO_FLAG_CMDREND:  Command response received (CRC check 
*                                           passed)[命令响应接收(CRC校验通过)]    
*                     - SDIO_FLAG_CMDSENT:  Command sent (no response required)[命令发送(缺少必要的响应)]    
*                     - SDIO_FLAG_DATAEND:  Data end (data counter, SDIDCOUNT, is
*                                           zero)[数据结束(数据计数器，SDIO计数器等于零)]    
*                     - SDIO_FLAG_STBITERR: Start bit not detected on all data 
*                                           signals in wide bus mode [在宽总线模式中从所有的数据信号中找不到开始位]  
*                     - SDIO_FLAG_DBCKEND:  Data block sent/received (CRC check 
*                                           passed) [数据块发送/接收(CRC校验通过)]        
*                     - SDIO_FLAG_SDIOIT:   SD I/O interrupt received[SD I/O中断 接收]     
*                     - SDIO_FLAG_CEATAEND: CE-ATA command completion signal 
*                                           received for CMD61 [CMD61的CE-ATA命令完成信号接收]   
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_ClearFlag(u32 SDIO_FLAG)
{ 
  /* Check the parameters [检查参数]*/
  assert_param(IS_SDIO_CLEAR_FLAG(SDIO_FLAG));
   
  SDIO->ICR = SDIO_FLAG;
}

/*******************************************************************************
* 函数名称: SDIO_GetITStatus
* 功能描述: Checks whether the specified SDIO interrupt has occurred or not.[检查指定的SDIO中断是否产生]
* 输入参数: SDIO_IT: specifies the SDIO interrupt source to check.[需要检查的SDIO指定中断源]
*                  这个参数可以是下面的值之一:
*                      - SDIO_IT_CCRCFAIL: Command response received (CRC check
*                                          failed) interrupt[命令响应接收(CRC校验失败)中断]    
*                      - SDIO_IT_DCRCFAIL: Data block sent/received (CRC check 
*                                          failed) interrupt[数据块发送/接收(CRC校验失败)中断]    
*                      - SDIO_IT_CTIMEOUT: Command response timeout interrupt[命令响应超时中断]     
*                      - SDIO_IT_DTIMEOUT: Data timeout interrupt[数据超时中断]     
*                      - SDIO_IT_TXUNDERR: Transmit FIFO underrun error interrupt[发送FIFO空栈读出错中断]    
*                      - SDIO_IT_RXOVERR:  Received FIFO overrun error interrupt[接收FIFO超速出错中断]      
*                      - SDIO_IT_CMDREND:  Command response received (CRC check 
*                                          passed) interrupt[命令响应接收(CRC校验通过)中断]     
*                      - SDIO_IT_CMDSENT:  Command sent (no response required) 
*                                          interrupt[命令发送(缺少必要的响应)中断]     
*                      - SDIO_IT_DATAEND:  Data end (data counter, SDIDCOUNT, is 
*                                          zero) interrupt [数据结束(数据计数器，SDIO计数器等于零)中断]    
*                      - SDIO_IT_STBITERR: Start bit not detected on all data [在宽总线模式中从所有的数据信号中找不到开始位中断]
*                                          signals in wide bus mode interrupt    
*                      - SDIO_IT_DBCKEND:  Data block sent/received (CRC check 
*                                          passed) interrupt[数据块发送/接收(CRC校验通过)中断]    
*                      - SDIO_IT_CMDACT:   Command transfer in progress interrupt[命令发送过程 中断]     
*                      - SDIO_IT_TXACT:    Data transmit in progress interrupt[数据发送过程 中断]       
*                      - SDIO_IT_RXACT:    Data receive in progress interrupt[数据接收过程 中断]      
*                      - SDIO_IT_TXFIFOHE: Transmit FIFO Half Empty interrupt[发送FIFO半空 中断]    
*                      - SDIO_IT_RXFIFOHF: Receive FIFO Half Full interrupt[接收FIFO半满 中断]   
*                      - SDIO_IT_TXFIFOF:  Transmit FIFO full interrupt[发送FIFO满 中断]     
*                      - SDIO_IT_RXFIFOF:  Receive FIFO full interrupt[接收FIFO满 中断]     
*                      - SDIO_IT_TXFIFOE:  Transmit FIFO empty interrupt[发送FIFO空 中断]      
*                      - SDIO_IT_RXFIFOE:  Receive FIFO empty interrupt[接收FIFO空 中断]     
*                      - SDIO_IT_TXDAVL:   Data available in transmit FIFO interrupt[发送FIFO中数据可用 中断]      
*                      - SDIO_IT_RXDAVL:   Data available in receive FIFO interrupt[接收FIFO中数据可用 中断]      
*                      - SDIO_IT_SDIOIT:   SD I/O interrupt received interrupt[SD I/O中断 接收中断]      
*                      - SDIO_IT_CEATAEND: CE-ATA command completion signal[CMD61的CE-ATA命令完成信号接收中断] 
*                                          received for CMD61 interrupt
* 输出参数: 无
* 返回参数: The new state of SDIO_IT (SET or RESET).[SDIO_IT的新状态]
*******************************************************************************/
ITStatus SDIO_GetITStatus(u32 SDIO_IT)
{ 
  ITStatus bitstatus = RESET;
  
  /* Check the parameters [检查参数]*/
  assert_param(IS_SDIO_GET_IT(SDIO_IT));

  if ((SDIO->STA & SDIO_IT) != (u32)RESET)  
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
* 函数名称: SDIO_ClearITPendingBit
* 功能描述: Clears the SDIO's interrupt pending bits.[清除SDIO中断挂起位]
* 输入参数: SDIO_IT: specifies the interrupt pending bit to clear.[需要清除的指定中断挂起位]
*                   This parameter can be one or a combination of the following
*                   values:[可以使用下述值的一个或者几个值的组合]
*                      - SDIO_IT_CCRCFAIL: Command response received (CRC check
*                                          failed) interrupt[命令响应接收(CRC校验失败)中断]     
*                      - SDIO_IT_DCRCFAIL: Data block sent/received (CRC check 
*                                          failed) interrupt [数据块发送/接收(CRC校验失败)中断]    
*                      - SDIO_IT_CTIMEOUT: Command response timeout interrupt [命令响应超时中断]   
*                      - SDIO_IT_DTIMEOUT: Data timeout interrupt[数据超时中断]    
*                      - SDIO_IT_TXUNDERR: Transmit FIFO underrun error interrupt [发送FIFO空栈读出错中断]   
*                      - SDIO_IT_RXOVERR:  Received FIFO overrun error interrupt[接收FIFO超速出错中断]     
*                      - SDIO_IT_CMDREND:  Command response received (CRC check 
*                                          passed) interrupt[命令响应接收(CRC校验通过)中断]      
*                      - SDIO_IT_CMDSENT:  Command sent (no response required) 
*                                          interrupt [命令发送(缺少必要的响应)中断]    
*                      - SDIO_IT_DATAEND:  Data end (data counter, SDIDCOUNT, is 
*                                          zero) interrupt[数据结束(数据计数器，SDIO计数器等于零)中断]      
*                      - SDIO_IT_STBITERR: Start bit not detected on all data [在宽总线模式中从所有的数据信号中找不到开始位中断]
*                                          signals in wide bus mode interrupt          
*                      - SDIO_IT_SDIOIT:   SD I/O interrupt received interrupt[SD I/O中断 接收中断]      
*                      - SDIO_IT_CEATAEND: CE-ATA command completion signal 
*                                          received for CMD61[CMD61的CE-ATA命令完成信号接收中断] 
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SDIO_ClearITPendingBit(u32 SDIO_IT)
{ 
  /* Check the parameters [检查参数]*/
  assert_param(IS_SDIO_CLEAR_IT(SDIO_IT));
   
  SDIO->ICR = SDIO_IT;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
