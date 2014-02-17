/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_sdio.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������SDIO�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_sdio.h"
#include "stm32f10x_rcc.h"

/* �������� -----------------------------------------------------------*/
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

/* ���ö��� ---------------------------------------------------------*/
/* ���ú� -----------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: SDIO_DeInit
* ��������: Deinitializes the SDIO peripheral registers to their default
*                  reset values[ȡ��SDIO�豸�Ĵ����ĳ�ʼ����ԭ�����ǵ�Ĭ��ֵ].
* �������: ��
* �������: ��
* ���ز���: ��
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
* ��������: SDIO_Init
* ��������: Initializes the SDIO peripheral according to the specified
*                  parameters in the SDIO_InitStruct[����SDIO_InitStruct�ṹ��ָ���Ĳ�����ʼ��SDIO�豸].
* �������: SDIO_InitStruct : pointer to a SDIO_InitTypeDef structure
*                  that contains the configuration information for the SDIO 
*                  peripheral.[ָ�������SDIO�豸������Ϣ�Ľṹ��SDIO_InitTypeDef��ָ��]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_Init(SDIO_InitTypeDef* SDIO_InitStruct)
{
  u32 tmpreg = 0;
    
  /* Check the parameters [������]*/
  assert_param(IS_SDIO_CLOCK_EDGE(SDIO_InitStruct->SDIO_ClockEdge));
  assert_param(IS_SDIO_CLOCK_BYPASS(SDIO_InitStruct->SDIO_ClockBypass));
  assert_param(IS_SDIO_CLOCK_POWER_SAVE(SDIO_InitStruct->SDIO_ClockPowerSave));
  assert_param(IS_SDIO_BUS_WIDE(SDIO_InitStruct->SDIO_BusWide));
  assert_param(IS_SDIO_HARDWARE_FLOW_CONTROL(SDIO_InitStruct->SDIO_HardwareFlowControl)); 
   
/*---------------------------- SDIO CLKCR Configuration ------------------------*/  
  /* Get the SDIO CLKCR value [ȡ��SDIO CLKCR��ֵ]*/
  tmpreg = SDIO->CLKCR;
  
  /* Clear CLKDIV, PWRSAV, BYPASS, WIDBUS, NEGEDGE, HWFC_EN bits [��CLKDIV, PWRSAV, BYPASS, WIDBUS, NEGEDGE, HWFC_ENλ]*/
  tmpreg &= CLKCR_CLEAR_MASK;
  
  /* Set CLKDIV bits according to SDIO_ClockDiv value [����SDIO_ClockDiv��ֵ����CLKDIVλ]*/
  /* Set PWRSAV bit according to SDIO_ClockPowerSave value [����SDIO_ClockPowerSave��ֵ����PWRSAVλ]*/
  /* Set BYPASS bit according to SDIO_ClockBypass value [����SDIO_ClockBypass��ֵ����BYPASSλ]*/
  /* Set WIDBUS bits according to SDIO_BusWide value [����SDIO_BusWide��ֵ����WIDBUSλ]*/
  /* Set NEGEDGE bits according to SDIO_ClockEdge value [����SDIO_ClockEdge��ֵ����NEGEDGEλ]*/
  /* Set HWFC_EN bits according to SDIO_HardwareFlowControl value [����SDIO_HardwareFlowControl��ֵ����HWFC_ENλ]*/
  tmpreg |= (SDIO_InitStruct->SDIO_ClockDiv  | SDIO_InitStruct->SDIO_ClockPowerSave |
             SDIO_InitStruct->SDIO_ClockBypass | SDIO_InitStruct->SDIO_BusWide |
             SDIO_InitStruct->SDIO_ClockEdge | SDIO_InitStruct->SDIO_HardwareFlowControl); 
  
  /* Write to SDIO CLKCR [дSDIO CLKCR]*/
  SDIO->CLKCR = tmpreg;             
}

/*******************************************************************************
* ��������: SDIO_StructInit
* ��������: Fills each SDIO_InitStruct member with its default value.[��Ĭ��ֵ���SDIO_InitStruct��ÿһ����Ա]
* �������: SDIO_InitStruct: pointer to an SDIO_InitTypeDef structure which
*                  will be initialized.[ָ��Ҫ����ʼ���Ľṹ��SDIO_InitTypeDef��ָ��]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_StructInit(SDIO_InitTypeDef* SDIO_InitStruct)
{
  /* SDIO_InitStruct members default value [SDIO_InitStruct��Ա��Ĭ��ֵ]*/
  SDIO_InitStruct->SDIO_ClockDiv = 0x00;
  SDIO_InitStruct->SDIO_ClockEdge = SDIO_ClockEdge_Rising;
  SDIO_InitStruct->SDIO_ClockBypass = SDIO_ClockBypass_Disable;
  SDIO_InitStruct->SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
  SDIO_InitStruct->SDIO_BusWide = SDIO_BusWide_1b;
  SDIO_InitStruct->SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
}

/*******************************************************************************
* ��������: SDIO_ClockCmd
* ��������: Enables or disables the SDIO Clock.[ʹ�ܻ��ֹSDIOʱ��]
* �������: NewState: new state of the SDIO Clock.[SDIOʱ�ӵ���״̬]
*                  This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_ClockCmd(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) CLKCR_CLKEN_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: SDIO_SetPowerState
* ��������: Sets the power status of the controller[���ÿ������ĵ�Դ״̬].
* �������: SDIO_PowerState: new state of the Power state.[�µĵ�Դ״̬]
*                  ������������������ֵ֮һ:
*                   - SDIO_PowerState_OFF
*                   - SDIO_PowerState_ON
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_SetPowerState(u32 SDIO_PowerState)
{
  /* Check the parameters [������]*/
  assert_param(IS_SDIO_POWER_STATE(SDIO_PowerState));
  
  SDIO->POWER &= PWR_PWRCTRL_MASK;
  SDIO->POWER |= SDIO_PowerState;
}

/*******************************************************************************
* ��������: SDIO_GetPowerState
* ��������: Gets the power status of the controller[ȡ�ÿ������ĵ�Դ״̬].
* �������: ��
* �������: ��
* ���ز���: Power status of the controller[�������ĵ�Դ״̬]. The returned value can
*                  be one of the following:[����ֵ�����������ֵ֮һ]
*                       - 0x00: Power OFF
*                       - 0x02: Power UP
*                       - 0x03: Power ON 
*******************************************************************************/
u32 SDIO_GetPowerState(void)
{
  return (SDIO->POWER & (~PWR_PWRCTRL_MASK));
}

/*******************************************************************************
* ��������: SDIO_ITConfig
* ��������: Enables or disables the SDIO interrupts.[ʹ�ܻ��ֹSDIO�ж�]
* �������: (1) SDIO_IT: specifies the SDIO interrupt sources to be
*                    enabled or disabled.[ʹ�ܻ��ָֹ����SDIO�ж�Դ]
*                    This parameter can be one or a combination of the following
*                    values:[�����������ʹ������ֵ��һ�����߼���ֵ�����]
*                      - SDIO_IT_CCRCFAIL: Command response received (CRC check
*                                          failed) interrupt[������Ӧ����(CRCУ��ʧ��)�ж�]    
*                      - SDIO_IT_DCRCFAIL: Data block sent/received (CRC check 
*                                          failed) interrupt[���ݿ鷢��/����(CRCУ��ʧ��)�ж�]    
*                      - SDIO_IT_CTIMEOUT: Command response timeout interrupt[������Ӧ��ʱ�ж�]     
*                      - SDIO_IT_DTIMEOUT: Data timeout interrupt[���ݳ�ʱ�ж�]     
*                      - SDIO_IT_TXUNDERR: Transmit FIFO underrun error interrupt[����FIFO��ջ�������ж�]    
*                      - SDIO_IT_RXOVERR:  Received FIFO overrun error interrupt[����FIFO���ٳ����ж�]      
*                      - SDIO_IT_CMDREND:  Command response received (CRC check 
*                                          passed) interrupt[������Ӧ����(CRCУ��ͨ��)�ж�]     
*                      - SDIO_IT_CMDSENT:  Command sent (no response required) 
*                                          interrupt[�����(ȱ�ٱ�Ҫ����Ӧ)�ж�]     
*                      - SDIO_IT_DATAEND:  Data end (data counter, SDIDCOUNT, is 
*                                          zero) interrupt [���ݽ���(���ݼ�������SDIO������������)�ж�]    
*                      - SDIO_IT_STBITERR: Start bit not detected on all data [�ڿ�����ģʽ�д����е������ź����Ҳ�����ʼλ�ж�]
*                                          signals in wide bus mode interrupt    
*                      - SDIO_IT_DBCKEND:  Data block sent/received (CRC check 
*                                          passed) interrupt[���ݿ鷢��/����(CRCУ��ͨ��)�ж�]    
*                      - SDIO_IT_CMDACT:   Command transfer in progress interrupt[����͹��� �ж�]     
*                      - SDIO_IT_TXACT:    Data transmit in progress interrupt[���ݷ��͹��� �ж�]       
*                      - SDIO_IT_RXACT:    Data receive in progress interrupt[���ݽ��չ��� �ж�]      
*                      - SDIO_IT_TXFIFOHE: Transmit FIFO Half Empty interrupt[����FIFO��� �ж�]    
*                      - SDIO_IT_RXFIFOHF: Receive FIFO Half Full interrupt[����FIFO���� �ж�]   
*                      - SDIO_IT_TXFIFOF:  Transmit FIFO full interrupt[����FIFO�� �ж�]     
*                      - SDIO_IT_RXFIFOF:  Receive FIFO full interrupt[����FIFO�� �ж�]     
*                      - SDIO_IT_TXFIFOE:  Transmit FIFO empty interrupt[����FIFO�� �ж�]      
*                      - SDIO_IT_RXFIFOE:  Receive FIFO empty interrupt[����FIFO�� �ж�]     
*                      - SDIO_IT_TXDAVL:   Data available in transmit FIFO interrupt[����FIFO�����ݿ��� �ж�]      
*                      - SDIO_IT_RXDAVL:   Data available in receive FIFO interrupt[����FIFO�����ݿ��� �ж�]      
*                      - SDIO_IT_SDIOIT:   SD I/O interrupt received interrupt[SD I/O�ж� �����ж�]      
*                      - SDIO_IT_CEATAEND: CE-ATA command completion signal[CMD61��CE-ATA��������źŽ����ж�] 
*                                          received for CMD61 interrupt
*           (2) NewState: new state of the specified SDIO interrupts.[ָ����SDIO�жϵ���״̬]
*                  This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]  
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_ITConfig(u32 SDIO_IT, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_SDIO_IT(SDIO_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the SDIO interrupts [ʹ��SDIO�ж�]*/
    SDIO->MASK |= SDIO_IT;
  }
  else
  {
    /* Disable the SDIO interrupts [��ֹSDIO�ж�]*/
    SDIO->MASK &= ~SDIO_IT;
  } 
}

/*******************************************************************************
* ��������: SDIO_DMACmd
* ��������: Enables or disables the SDIO DMA request.[ʹ�ܻ��ֹSDIO DMA����]
* �������: NewState: new state of the selected SDIO DMA request.[ָ��SDIO DMA �������״̬]
*                  This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_DMACmd(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) DCTRL_DMAEN_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: SDIO_SendCommand
* ��������: Initializes the SDIO Command according to the specified[����SDIO_CmdInitStruct�ͷ���������ָ���Ĳ�����ʼ��SDIO����]
*                  parameters in the SDIO_CmdInitStruct and send the command.
* �������: SDIO_CmdInitStruct : pointer to a SDIO_CmdInitTypeDef
*                  structure that contains the configuration information 
*                  for the SDIO command.[ָ�������SDIO����������Ϣ�Ľṹ��SDIO_CmdInitTypeDef��ָ��]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_SendCommand(SDIO_CmdInitTypeDef *SDIO_CmdInitStruct)
{
  u32 tmpreg = 0;
  
  /* Check the parameters [������]*/
  assert_param(IS_SDIO_CMD_INDEX(SDIO_CmdInitStruct->SDIO_CmdIndex));
  assert_param(IS_SDIO_RESPONSE(SDIO_CmdInitStruct->SDIO_Response));
  assert_param(IS_SDIO_WAIT(SDIO_CmdInitStruct->SDIO_Wait));
  assert_param(IS_SDIO_CPSM(SDIO_CmdInitStruct->SDIO_CPSM));
  
/*---------------------------- SDIO ARG Configuration ------------------------*/
  /* Set the SDIO Argument value [����SDIO����ֵ]*/
  SDIO->ARG = SDIO_CmdInitStruct->SDIO_Argument;
  
/*---------------------------- SDIO CMD Configuration ------------------------*/  
  /* Get the SDIO CMD value [ȡ��SDIO CMDֵ]*/
  tmpreg = SDIO->CMD;

  /* Clear CMDINDEX, WAITRESP, WAITINT, WAITPEND, CPSMEN bits [��CMDINDEX, WAITRESP, WAITINT, WAITPEND, CPSMENλ]*/
  tmpreg &= CMD_CLEAR_MASK;
  /* Set CMDINDEX bits according to SDIO_CmdIndex value [����SDIO_CmdIndex��ֵ����CMDINDEXλ]*/
  /* Set WAITRESP bits according to SDIO_Response value [����SDIO_Response��ֵ����WAITRESPλ]*/
  /* Set WAITINT and WAITPEND bits according to SDIO_Wait value [����SDIO_Wait��ֵ����WAITINT��WAITPENDλ]*/
  /* Set CPSMEN bits according to SDIO_CPSM value [����SDIO_CPSM��ֵ����CPSMENλ]*/
  tmpreg |= (u32)SDIO_CmdInitStruct->SDIO_CmdIndex | SDIO_CmdInitStruct->SDIO_Response
           | SDIO_CmdInitStruct->SDIO_Wait | SDIO_CmdInitStruct->SDIO_CPSM;
  
  /* Write to SDIO CMD [дSDIO CMD]*/
  SDIO->CMD = tmpreg;
}

/*******************************************************************************
* ��������: SDIO_CmdStructInit
* ��������: Fills each SDIO_CmdInitStruct member with its default value.[��Ĭ��ֵ���SDIO_CmdInitStruct��ÿһ����Ա]
* �������: SDIO_CmdInitStruct: pointer to an SDIO_CmdInitTypeDef
*                  structure which will be initialized.[ָ��Ҫ����ʼ���Ľṹ��SDIO_CmdInitTypeDef��ָ��]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_CmdStructInit(SDIO_CmdInitTypeDef* SDIO_CmdInitStruct)
{
  /* SDIO_CmdInitStruct members default value [SDIO_CmdInitStruct��Ա��Ĭ��ֵ]*/
  SDIO_CmdInitStruct->SDIO_Argument = 0x00;
  SDIO_CmdInitStruct->SDIO_CmdIndex = 0x00;
  SDIO_CmdInitStruct->SDIO_Response = SDIO_Response_No;
  SDIO_CmdInitStruct->SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct->SDIO_CPSM = SDIO_CPSM_Disable;
}

/*******************************************************************************
* ��������: SDIO_GetCommandResponse
* ��������: Returns command index of last command for which response
*                  received.[�������½�����Ӧ������������]
* �������: ��
* �������: ��
* ���ز���: Returns the command index of the last command response received.[�������½�����Ӧ������������]
*******************************************************************************/
u8 SDIO_GetCommandResponse(void)
{
  return (u8)(SDIO->RESPCMD);
}

/*******************************************************************************
* ��������: SDIO_GetResponse
* ��������: Returns response received from the card for the last command.[������������Ľ�����Ӧ]
* �������: - SDIO_RESP: Specifies the SDIO response register.[ָ����SDIO��Ӧ�Ĵ���]
*                     ������������������ֵ֮һ:
*                       - SDIO_RESP1: Response Register 1[��Ӧ�Ĵ���1]
*                       - SDIO_RESP2: Response Register 2[��Ӧ�Ĵ���2]
*                       - SDIO_RESP3: Response Register 3[��Ӧ�Ĵ���3]
*                       - SDIO_RESP4: Response Register 4[��Ӧ�Ĵ���4]                       
* �������: ��
* ���ز���: The Corresponding response register value.[��Ӧ����Ӧ�Ĵ���ֵ]
*******************************************************************************/
u32 SDIO_GetResponse(u32 SDIO_RESP)
{
  /* Check the parameters [������]*/
  assert_param(IS_SDIO_RESP(SDIO_RESP));
  
  return (*(vu32 *)(SDIO_RESP_ADDR + SDIO_RESP)); 
}

/*******************************************************************************
* ��������: SDIO_DataConfig
* ��������: Initializes the SDIO data path according to the specified
*                  parameters in the SDIO_DataInitStruct.[����SDIO_DataInitStruct��ָ���Ĳ�����ʼ��SDIO����·��]
* �������: SDIO_DataInitStruct : pointer to a SDIO_DataInitTypeDef
*                  structure that contains the configuration information 
*                  for the SDIO command.[ָ�������SDIO����������Ϣ�Ľṹ��SDIO_DataInitTypeDef��ָ��]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_DataConfig(SDIO_DataInitTypeDef* SDIO_DataInitStruct)
{
  u32 tmpreg = 0;
  
  /* Check the parameters [������]*/
  assert_param(IS_SDIO_DATA_LENGTH(SDIO_DataInitStruct->SDIO_DataLength));
  assert_param(IS_SDIO_BLOCK_SIZE(SDIO_DataInitStruct->SDIO_DataBlockSize));
  assert_param(IS_SDIO_TRANSFER_DIR(SDIO_DataInitStruct->SDIO_TransferDir));
  assert_param(IS_SDIO_TRANSFER_MODE(SDIO_DataInitStruct->SDIO_TransferMode));
  assert_param(IS_SDIO_DPSM(SDIO_DataInitStruct->SDIO_DPSM));

/*---------------------------- SDIO DTIMER Configuration ---------------------*/
  /* Set the SDIO Data TimeOut value [����SDIO���ݳ�ʱֵ]*/
  SDIO->DTIMER = SDIO_DataInitStruct->SDIO_DataTimeOut;
    
/*---------------------------- SDIO DLEN Configuration -----------------------*/
  /* Set the SDIO DataLength value [����SDIO���ݳ���ֵ]*/
  SDIO->DLEN = SDIO_DataInitStruct->SDIO_DataLength;
  
/*---------------------------- SDIO DCTRL Configuration ----------------------*/  
  /* Get the SDIO DCTRL value [ȡ��SDIO DCTRLֵ]*/
  tmpreg = SDIO->DCTRL;

  /* Clear DEN, DTMODE, DTDIR and DBCKSIZE bits [��DEN, DTMODE, DTDIR��DBCKSIZEλ]*/
  tmpreg &= DCTRL_CLEAR_MASK;
  /* Set DEN bit according to SDIO_DPSM value [����SDIO_DPSM��ֵ����DENλ]*/
  /* Set DTMODE bit according to SDIO_TransferMode value [����SDIO_TransferMode��ֵ����DTMODEλ]*/
  /* Set DTDIR bit according to SDIO_TransferDir value [���� SDIO_TransferDir��ֵ����DTDIRλ]*/
  /* Set DBCKSIZE bits according to SDIO_DataBlockSize value [����SDIO_DataBlockSize��ֵ����DBCKSIZEλ]*/
  tmpreg |= (u32)SDIO_DataInitStruct->SDIO_DataBlockSize | SDIO_DataInitStruct->SDIO_TransferDir
           | SDIO_DataInitStruct->SDIO_TransferMode | SDIO_DataInitStruct->SDIO_DPSM;
  
  /* Write to SDIO DCTRL [дSDIO DCTRL]*/
  SDIO->DCTRL = tmpreg;
}

/*******************************************************************************
* ��������: SDIO_DataStructInit
* ��������: Fills each SDIO_DataInitStruct member with its default value.[��Ĭ��ֵ���SDIO_DataInitStruct��ÿһ����Ա]
* �������: SDIO_DataInitStruct: pointer to an SDIO_DataInitTypeDef
*                  structure which will be initialized.[ָ�������SDIO����������Ϣ�Ľṹ��SDIO_DataInitTypeDef��ָ��]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_DataStructInit(SDIO_DataInitTypeDef* SDIO_DataInitStruct)
{
  /* SDIO_DataInitStruct members default value [SDIO_DataInitStruct��Ա��Ĭ��ֵ]*/
  SDIO_DataInitStruct->SDIO_DataTimeOut = 0xFFFFFFFF;
  SDIO_DataInitStruct->SDIO_DataLength = 0x00;
  SDIO_DataInitStruct->SDIO_DataBlockSize = SDIO_DataBlockSize_1b;
  SDIO_DataInitStruct->SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStruct->SDIO_TransferMode = SDIO_TransferMode_Block;  
  SDIO_DataInitStruct->SDIO_DPSM = SDIO_DPSM_Disable;
}

/*******************************************************************************
* ��������: SDIO_GetDataCounter
* ��������: Returns number of remaining data bytes to be transferred.[���ط���ʣ�������ֽ���]
* �������: ��
* �������: ��
* ���ز���: Number of remaining data bytes to be transferred[����ʣ�������ֽ���]
*******************************************************************************/
u32 SDIO_GetDataCounter(void)
{ 
  return SDIO->DCOUNT;
}

/*******************************************************************************
* ��������: SDIO_ReadData
* ��������: Read one data word from Rx FIFO.[��Rx FIFO��һ��������]
* �������: ��
* �������: ��
* ���ز���: Data received[���յ�����]
*******************************************************************************/
u32 SDIO_ReadData(void)
{ 
  return SDIO->FIFO;
}

/*******************************************************************************
* ��������: SDIO_WriteData
* ��������: Write one data word to Tx FIFO.[��Tx FIFO��дһ��������]
* �������: Data: 32-bit data word to write.[32λ��������]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_WriteData(u32 Data)
{ 
  SDIO->FIFO = Data;
}

/*******************************************************************************
* ��������: SDIO_GetFIFOCount
* ��������: Returns the number of words left to be written to or read
*                  from FIFO.[����һ��FIFO��дʣ�������]   
* �������: ��
* �������: ��
* ���ز���: Remaining number of words.[ʣ�������]
*******************************************************************************/
u32 SDIO_GetFIFOCount(void)
{ 
  return SDIO->FIFOCNT;
}

/*******************************************************************************
* ��������: SDIO_StartSDIOReadWait
* ��������: Starts the SD I/O Read Wait operation.[��ʼSD I/O���ȴ�����]
* �������: NewState: new state of the Start SDIO Read Wait operation.[�������ȴ���������״̬]
*                  This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_StartSDIOReadWait(FunctionalState NewState)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) DCTRL_RWSTART_BB = (u32) NewState;
}

/*******************************************************************************
* ��������: SDIO_StopSDIOReadWait
* ��������: Stops the SD I/O Read Wait operation.[ֹͣSD I/O���ȴ�����]
* �������: NewState: new state of the Stop SDIO Read Wait operation.[ֹͣ���ȴ���������״̬]
*                  This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_StopSDIOReadWait(FunctionalState NewState)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) DCTRL_RWSTOP_BB = (u32) NewState;
}

/*******************************************************************************
* ��������: SDIO_SetSDIOReadWaitMode
* ��������: Sets one of the two options of inserting read wait interval.[��������������ȴ�ʱ��ѡ���е�һ��]
* �������: SDIOReadWaitMode: SD I/O Read Wait operation mode.[SD I/O���ȴ�ģʽ]
*                  This parametre can be:[�������������]
*                    - SDIO_ReadWaitMode_CLK: Read Wait control by stopping SDIOCLK[ֹͣSDIOCLK���ƶ��ȴ�]
*                    - SDIO_ReadWaitMode_DATA2: Read Wait control using SDIO_DATA2[SDIO_DATA2���ƶ��ȴ�]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_SetSDIOReadWaitMode(u32 SDIO_ReadWaitMode)
{
  /* Check the parameters [������]*/
  assert_param(IS_SDIO_READWAIT_MODE(SDIO_ReadWaitMode));
  
  *(vu32 *) DCTRL_RWMOD_BB = SDIO_ReadWaitMode;
}

/*******************************************************************************
* ��������: SDIO_SetSDIOOperation
* ��������: Enables or disables the SD I/O Mode Operation.[ʹ�ܻ��߽�ֹSD I/Oģʽ����]
* �������: NewState: new state of SDIO specific operation.[SDIOָ����������״̬]
*                  This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_SetSDIOOperation(FunctionalState NewState)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) DCTRL_SDIOEN_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: SDIO_SendSDIOSuspendCmd
* ��������: Enables or disables the SD I/O Mode suspend command sending.[ʹ�ܻ��߽�ֹSD I/Oģʽ��ֹ�����]
* �������: NewState: new state of the SD I/O Mode suspend command.[SD I/Oģʽ��ֹ�������״̬]
*                  This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_SendSDIOSuspendCmd(FunctionalState NewState)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) CMD_SDIOSUSPEND_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: SDIO_CommandCompletionCmd
* ��������: Enables or disables the command completion signal.[ʹ�ܻ��߽�ֹ��������ź�]
* �������: NewState: new state of command completion signal.[��������źŵ���״̬]
*                  This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_CommandCompletionCmd(FunctionalState NewState)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) CMD_ENCMDCOMPL_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: SDIO_CEATAITCmd
* ��������: Enables or disables the CE-ATA interrupt.[ʹ�ܻ��߽�ֹCE-ATA�ж�]
* �������: NewState: new state of CE-ATA interrupt.[CE-ATA�жϵ���״̬]
*                  This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_CEATAITCmd(FunctionalState NewState)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) CMD_NIEN_BB = (u32)((~((u32)NewState)) & ((u32)0x1));
}

/*******************************************************************************
* ��������: SDIO_SendCEATACmd
* ��������: Sends CE-ATA command (CMD61).[����CE-ATA����]
* �������: NewState: new state of CE-ATA command.[CE-ATA�������״̬]
*                  This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_SendCEATACmd(FunctionalState NewState)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(vu32 *) CMD_ATACMD_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: SDIO_GetFlagStatus
* ��������: Checks whether the specified SDIO flag is set or not.[���ָ����SDIO��־�Ƿ���λ]
* �������: SDIO_FLAG: specifies the flag to check.[��Ҫ����ָ����־]
*                  ������������������ֵ֮һ:
*                     - SDIO_FLAG_CCRCFAIL: Command response received (CRC check
*                                           failed) [������Ӧ����(CRCУ��ʧ��)]   
*                     - SDIO_FLAG_DCRCFAIL: Data block sent/received (CRC check 
*                                           failed)[���ݿ鷢��/����(CRCУ��ʧ��)]    
*                     - SDIO_FLAG_CTIMEOUT: Command response timeout[������Ӧ��ʱ]    
*                     - SDIO_FLAG_DTIMEOUT: Data timeou [���ݳ�ʱ]  
*                     - SDIO_FLAG_TXUNDERR: Transmit FIFO underrun error[����FIFO��ջ������]   
*                     - SDIO_FLAG_RXOVERR:  Received FIFO overrun error[����FIFO���ٳ���]    
*                     - SDIO_FLAG_CMDREND:  Command response received (CRC check 
*                                           passed)[������Ӧ����(CRCУ��ͨ��)]     
*                     - SDIO_FLAG_CMDSENT:  Command sent (no response required)[�����(ȱ�ٱ�Ҫ����Ӧ)]     
*                     - SDIO_FLAG_DATAEND:  Data end (data counter, SDIDCOUNT, is
*                                           zero)[���ݽ���(���ݼ�������SDIO������������)]    
*                     - SDIO_FLAG_STBITERR: Start bit not detected on all data 
*                                           signals in wide bus mode[�ڿ�����ģʽ�д����е������ź����Ҳ�����ʼλ]   
*                     - SDIO_FLAG_DBCKEND:  Data block sent/received (CRC check 
*                                           passed)[���ݿ鷢��/����(CRCУ��ͨ��)]    
*                     - SDIO_FLAG_CMDACT:   Command transfer in progress[����͹���]     
*                     - SDIO_FLAG_TXACT:    Data transmit in progress[���ݷ��͹���]      
*                     - SDIO_FLAG_RXACT:    Data receive in progress[���ݽ��չ���]      
*                     - SDIO_FLAG_TXFIFOHE: Transmit FIFO Half Empty[����FIFO���]   
*                     - SDIO_FLAG_RXFIFOHF: Receive FIFO Half Full[����FIFO����]   
*                     - SDIO_FLAG_TXFIFOF:  Transmit FIFO full[����FIFO��]    
*                     - SDIO_FLAG_RXFIFOF:  Receive FIFO full[����FIFO��]     
*                     - SDIO_FLAG_TXFIFOE:  Transmit FIFO empty[����FIFO��]     
*                     - SDIO_FLAG_RXFIFOE:  Receive FIFO empty[����FIFO��]     
*                     - SDIO_FLAG_TXDAVL:   Data available in transmit FIFO [����FIFO�����ݿ���]     
*                     - SDIO_FLAG_RXDAVL:   Data available in receive FIFO[����FIFO�����ݿ���]     
*                     - SDIO_FLAG_SDIOIT:   SD I/O interrupt received[SD I/O�ж� ����]     
*                     - SDIO_FLAG_CEATAEND: CE-ATA command completion signal 
*                                           received for CMD61 [CMD61��CE-ATA��������źŽ���]    
* �������: ��
* ���ز���: The new state of SDIO_FLAG (SET or RESET).
*******************************************************************************/
FlagStatus SDIO_GetFlagStatus(u32 SDIO_FLAG)
{ 
  FlagStatus bitstatus = RESET;
  
  /* Check the parameters [������]*/
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
* ��������: SDIO_ClearFlag
* ��������: Clears the SDIO's pending flags.[���SDIO�����־]
* �������: SDIO_FLAG: specifies the flag to clear.[Ҫ�����ָ����־]
*                  This parameter can be one or a combination of the following
*                  values:[����ʹ������ֵ��һ�����߼���ֵ�����]
*                     - SDIO_FLAG_CCRCFAIL: Command response received (CRC check
*                                           failed) [������Ӧ����(CRCУ��ʧ��)]   
*                     - SDIO_FLAG_DCRCFAIL: Data block sent/received (CRC check 
*                                           failed)[���ݿ鷢��/����(CRCУ��ʧ��)]    
*                     - SDIO_FLAG_CTIMEOUT: Command response timeout [������Ӧ��ʱ]   
*                     - SDIO_FLAG_DTIMEOUT: Data timeou [���ݳ�ʱ]  
*                     - SDIO_FLAG_TXUNDERR: Transmit FIFO underrun error [����FIFO��ջ������]  
*                     - SDIO_FLAG_RXOVERR:  Received FIFO overrun error[����FIFO���ٳ���]    
*                     - SDIO_FLAG_CMDREND:  Command response received (CRC check 
*                                           passed)[������Ӧ����(CRCУ��ͨ��)]    
*                     - SDIO_FLAG_CMDSENT:  Command sent (no response required)[�����(ȱ�ٱ�Ҫ����Ӧ)]    
*                     - SDIO_FLAG_DATAEND:  Data end (data counter, SDIDCOUNT, is
*                                           zero)[���ݽ���(���ݼ�������SDIO������������)]    
*                     - SDIO_FLAG_STBITERR: Start bit not detected on all data 
*                                           signals in wide bus mode [�ڿ�����ģʽ�д����е������ź����Ҳ�����ʼλ]  
*                     - SDIO_FLAG_DBCKEND:  Data block sent/received (CRC check 
*                                           passed) [���ݿ鷢��/����(CRCУ��ͨ��)]        
*                     - SDIO_FLAG_SDIOIT:   SD I/O interrupt received[SD I/O�ж� ����]     
*                     - SDIO_FLAG_CEATAEND: CE-ATA command completion signal 
*                                           received for CMD61 [CMD61��CE-ATA��������źŽ���]   
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_ClearFlag(u32 SDIO_FLAG)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_SDIO_CLEAR_FLAG(SDIO_FLAG));
   
  SDIO->ICR = SDIO_FLAG;
}

/*******************************************************************************
* ��������: SDIO_GetITStatus
* ��������: Checks whether the specified SDIO interrupt has occurred or not.[���ָ����SDIO�ж��Ƿ����]
* �������: SDIO_IT: specifies the SDIO interrupt source to check.[��Ҫ����SDIOָ���ж�Դ]
*                  ������������������ֵ֮һ:
*                      - SDIO_IT_CCRCFAIL: Command response received (CRC check
*                                          failed) interrupt[������Ӧ����(CRCУ��ʧ��)�ж�]    
*                      - SDIO_IT_DCRCFAIL: Data block sent/received (CRC check 
*                                          failed) interrupt[���ݿ鷢��/����(CRCУ��ʧ��)�ж�]    
*                      - SDIO_IT_CTIMEOUT: Command response timeout interrupt[������Ӧ��ʱ�ж�]     
*                      - SDIO_IT_DTIMEOUT: Data timeout interrupt[���ݳ�ʱ�ж�]     
*                      - SDIO_IT_TXUNDERR: Transmit FIFO underrun error interrupt[����FIFO��ջ�������ж�]    
*                      - SDIO_IT_RXOVERR:  Received FIFO overrun error interrupt[����FIFO���ٳ����ж�]      
*                      - SDIO_IT_CMDREND:  Command response received (CRC check 
*                                          passed) interrupt[������Ӧ����(CRCУ��ͨ��)�ж�]     
*                      - SDIO_IT_CMDSENT:  Command sent (no response required) 
*                                          interrupt[�����(ȱ�ٱ�Ҫ����Ӧ)�ж�]     
*                      - SDIO_IT_DATAEND:  Data end (data counter, SDIDCOUNT, is 
*                                          zero) interrupt [���ݽ���(���ݼ�������SDIO������������)�ж�]    
*                      - SDIO_IT_STBITERR: Start bit not detected on all data [�ڿ�����ģʽ�д����е������ź����Ҳ�����ʼλ�ж�]
*                                          signals in wide bus mode interrupt    
*                      - SDIO_IT_DBCKEND:  Data block sent/received (CRC check 
*                                          passed) interrupt[���ݿ鷢��/����(CRCУ��ͨ��)�ж�]    
*                      - SDIO_IT_CMDACT:   Command transfer in progress interrupt[����͹��� �ж�]     
*                      - SDIO_IT_TXACT:    Data transmit in progress interrupt[���ݷ��͹��� �ж�]       
*                      - SDIO_IT_RXACT:    Data receive in progress interrupt[���ݽ��չ��� �ж�]      
*                      - SDIO_IT_TXFIFOHE: Transmit FIFO Half Empty interrupt[����FIFO��� �ж�]    
*                      - SDIO_IT_RXFIFOHF: Receive FIFO Half Full interrupt[����FIFO���� �ж�]   
*                      - SDIO_IT_TXFIFOF:  Transmit FIFO full interrupt[����FIFO�� �ж�]     
*                      - SDIO_IT_RXFIFOF:  Receive FIFO full interrupt[����FIFO�� �ж�]     
*                      - SDIO_IT_TXFIFOE:  Transmit FIFO empty interrupt[����FIFO�� �ж�]      
*                      - SDIO_IT_RXFIFOE:  Receive FIFO empty interrupt[����FIFO�� �ж�]     
*                      - SDIO_IT_TXDAVL:   Data available in transmit FIFO interrupt[����FIFO�����ݿ��� �ж�]      
*                      - SDIO_IT_RXDAVL:   Data available in receive FIFO interrupt[����FIFO�����ݿ��� �ж�]      
*                      - SDIO_IT_SDIOIT:   SD I/O interrupt received interrupt[SD I/O�ж� �����ж�]      
*                      - SDIO_IT_CEATAEND: CE-ATA command completion signal[CMD61��CE-ATA��������źŽ����ж�] 
*                                          received for CMD61 interrupt
* �������: ��
* ���ز���: The new state of SDIO_IT (SET or RESET).[SDIO_IT����״̬]
*******************************************************************************/
ITStatus SDIO_GetITStatus(u32 SDIO_IT)
{ 
  ITStatus bitstatus = RESET;
  
  /* Check the parameters [������]*/
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
* ��������: SDIO_ClearITPendingBit
* ��������: Clears the SDIO's interrupt pending bits.[���SDIO�жϹ���λ]
* �������: SDIO_IT: specifies the interrupt pending bit to clear.[��Ҫ�����ָ���жϹ���λ]
*                   This parameter can be one or a combination of the following
*                   values:[����ʹ������ֵ��һ�����߼���ֵ�����]
*                      - SDIO_IT_CCRCFAIL: Command response received (CRC check
*                                          failed) interrupt[������Ӧ����(CRCУ��ʧ��)�ж�]     
*                      - SDIO_IT_DCRCFAIL: Data block sent/received (CRC check 
*                                          failed) interrupt [���ݿ鷢��/����(CRCУ��ʧ��)�ж�]    
*                      - SDIO_IT_CTIMEOUT: Command response timeout interrupt [������Ӧ��ʱ�ж�]   
*                      - SDIO_IT_DTIMEOUT: Data timeout interrupt[���ݳ�ʱ�ж�]    
*                      - SDIO_IT_TXUNDERR: Transmit FIFO underrun error interrupt [����FIFO��ջ�������ж�]   
*                      - SDIO_IT_RXOVERR:  Received FIFO overrun error interrupt[����FIFO���ٳ����ж�]     
*                      - SDIO_IT_CMDREND:  Command response received (CRC check 
*                                          passed) interrupt[������Ӧ����(CRCУ��ͨ��)�ж�]      
*                      - SDIO_IT_CMDSENT:  Command sent (no response required) 
*                                          interrupt [�����(ȱ�ٱ�Ҫ����Ӧ)�ж�]    
*                      - SDIO_IT_DATAEND:  Data end (data counter, SDIDCOUNT, is 
*                                          zero) interrupt[���ݽ���(���ݼ�������SDIO������������)�ж�]      
*                      - SDIO_IT_STBITERR: Start bit not detected on all data [�ڿ�����ģʽ�д����е������ź����Ҳ�����ʼλ�ж�]
*                                          signals in wide bus mode interrupt          
*                      - SDIO_IT_SDIOIT:   SD I/O interrupt received interrupt[SD I/O�ж� �����ж�]      
*                      - SDIO_IT_CEATAEND: CE-ATA command completion signal 
*                                          received for CMD61[CMD61��CE-ATA��������źŽ����ж�] 
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SDIO_ClearITPendingBit(u32 SDIO_IT)
{ 
  /* Check the parameters [������]*/
  assert_param(IS_SDIO_CLEAR_IT(SDIO_IT));
   
  SDIO->ICR = SDIO_IT;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
