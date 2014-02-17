/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_can.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有CAN的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_can.h"
#include "stm32f10x_rcc.h"

/* 自用类型 -----------------------------------------------------------*/

/* 自用定义 ------------------------------------------------------------*/
/* CAN Master Control Register bits [CAN主控寄存器位]*/
#define CAN_MCR_INRQ     ((u32)0x00000001) /* Initialization request [初始化请求]*/
#define CAN_MCR_SLEEP    ((u32)0x00000002) /* Sleep mode request [睡眠模式请求]*/
#define CAN_MCR_TXFP     ((u32)0x00000004) /* Transmit FIFO priority [发送FIFO优先级]*/
#define CAN_MCR_RFLM     ((u32)0x00000008) /* Receive FIFO locked mode [接收FIFO锁定模式]*/
#define CAN_MCR_NART     ((u32)0x00000010) /* No automatic retransmission [非自动重发]*/
#define CAN_MCR_AWUM     ((u32)0x00000020) /* Automatic wake up mode [自动唤醒模式]*/
#define CAN_MCR_ABOM     ((u32)0x00000040) /* Automatic bus-off management [自动总线关断管理]*/
#define CAN_MCR_TTCM     ((u32)0x00000080) /* time triggered communication [定时器触发通讯]*/

/* CAN Master Status Register bits [CAN主控模式状态寄存器位]*/
#define CAN_MSR_INAK     ((u32)0x00000001)    /* Initialization acknowledge [初始化确认]*/
#define CAN_MSR_WKUI     ((u32)0x00000008)    /* Wake-up interrupt [唤醒中断]*/
#define CAN_MSR_SLAKI    ((u32)0x00000010)    /* Sleep acknowledge interrupt [睡眠确认中断]*/

/* CAN Transmit Status Register bits [CAN发送状态寄存器位]*/
#define CAN_TSR_RQCP0    ((u32)0x00000001)    /* Request completed mailbox0 [信箱0完成请求]*/
#define CAN_TSR_TXOK0    ((u32)0x00000002)    /* Transmission OK of mailbox0 [信箱0发送OK]*/
#define CAN_TSR_ABRQ0    ((u32)0x00000080)    /* Abort request for mailbox0 [信箱0请求失败]*/
#define CAN_TSR_RQCP1    ((u32)0x00000100)    /* Request completed mailbox1 [信箱1完成请求]*/
#define CAN_TSR_TXOK1    ((u32)0x00000200)    /* Transmission OK of mailbox1 [信箱1发送OK]*/
#define CAN_TSR_ABRQ1    ((u32)0x00008000)    /* Abort request for mailbox1 [信箱1请求失败]*/
#define CAN_TSR_RQCP2    ((u32)0x00010000)    /* Request completed mailbox2 [信箱2完成请求]*/
#define CAN_TSR_TXOK2    ((u32)0x00020000)    /* Transmission OK of mailbox2 [信箱1发送OK]*/
#define CAN_TSR_ABRQ2    ((u32)0x00800000)    /* Abort request for mailbox2 [信箱2请求失败]*/
#define CAN_TSR_TME0     ((u32)0x04000000)    /* Transmit mailbox 0 empty [发送信箱0空]*/
#define CAN_TSR_TME1     ((u32)0x08000000)    /* Transmit mailbox 1 empty [发送信箱1空]*/
#define CAN_TSR_TME2     ((u32)0x10000000)    /* Transmit mailbox 2 empty [发送信箱2空]*/

/* CAN Receive FIFO 0 Register bits [CAN接收FIFO 0寄存器位]*/
#define CAN_RF0R_FULL0   ((u32)0x00000008)    /* FIFO 0 full [FIFO 0满]*/
#define CAN_RF0R_FOVR0   ((u32)0x00000010)    /* FIFO 0 overrun [FIFO 0溢出]*/
#define CAN_RF0R_RFOM0   ((u32)0x00000020)    /* Release FIFO 0 output mailbox [释放FIFO 0输出信箱]*/

/* CAN Receive FIFO 1 Register bits [CAN接收FIFO 1寄存器位]*/
#define CAN_RF1R_FULL1   ((u32)0x00000008)    /* FIFO 1 full [FIFO 1满]*/
#define CAN_RF1R_FOVR1   ((u32)0x00000010)    /* FIFO 1 overrun [FIFO 1溢出]*/
#define CAN_RF1R_RFOM1   ((u32)0x00000020)    /* Release FIFO 1 output mailbox [释放FIFO 1输出信箱]*/

/* CAN Error Status Register bits [CAN错误状态寄存器位]*/
#define CAN_ESR_EWGF     ((u32)0x00000001)    /* Error warning flag [错误警告标志]*/
#define CAN_ESR_EPVF     ((u32)0x00000002)    /* Error passive flag [错误被动标志]*/
#define CAN_ESR_BOFF     ((u32)0x00000004)    /* Bus-off flag [总线关断标志]*/

/* CAN Mailbox Transmit Request [CAN信箱发送请求]*/
#define CAN_TMIDxR_TXRQ    ((u32)0x00000001) /* Transmit mailbox request [发送信箱请求]*/

/* CAN Filter Master Register bits [CAN滤波器主寄存器位]*/
#define CAN_FMR_FINIT ((u32)0x00000001) /* Filter init mode [滤波器初始化模式]*/


/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
static ITStatus CheckITStatus(u32 CAN_Reg, u32 It_Bit);

/* 自用函数 ---------------------------------------------------------*/
/*******************************************************************************
* 函数名称: CAN_DeInit
* 功能描述: D将CAN外设寄存器复位为它们的默认值
* 输入参数: 无.
* 输出参数: 无.
* 返回参数: 无.
*******************************************************************************/
void CAN_DeInit(void)
{
  /* Enable CAN reset state */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN, ENABLE);
  /* Release CAN from reset state */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN, DISABLE);
}

/*******************************************************************************
* 函数名称: CAN_Init
* 功能描述: 根据CAN_InitStruct中指定的参数初始化CAN外围模块。
* 输入参数: CAN_InitStruct: 指向结构体CAN_InitTypeDef的指针，包含了CAN外围模块的配置信息。
* 输出参数: 无.
* 返回参数: 一个说明CAN初始化是否成功的常量。CANINITFAILED ＝ 初始化失败
*                                            CANINITOK ＝ 初始化成功
*******************************************************************************/
u8 CAN_Init(CAN_InitTypeDef* CAN_InitStruct)
{
  u8 InitStatus = 0;
  u16 WaitAck = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(CAN_InitStruct->CAN_TTCM));
  assert_param(IS_FUNCTIONAL_STATE(CAN_InitStruct->CAN_ABOM));
  assert_param(IS_FUNCTIONAL_STATE(CAN_InitStruct->CAN_AWUM));
  assert_param(IS_FUNCTIONAL_STATE(CAN_InitStruct->CAN_NART));
  assert_param(IS_FUNCTIONAL_STATE(CAN_InitStruct->CAN_RFLM));
  assert_param(IS_FUNCTIONAL_STATE(CAN_InitStruct->CAN_TXFP));
  assert_param(IS_CAN_MODE(CAN_InitStruct->CAN_Mode));
  assert_param(IS_CAN_SJW(CAN_InitStruct->CAN_SJW));
  assert_param(IS_CAN_BS1(CAN_InitStruct->CAN_BS1));
  assert_param(IS_CAN_BS2(CAN_InitStruct->CAN_BS2));
  assert_param(IS_CAN_PRESCALER(CAN_InitStruct->CAN_Prescaler));

  /* Request initialisation [请求初始化]*/
  CAN->MCR = CAN_MCR_INRQ;

  /* ...and check acknowledged [检测应答]*/
  if ((CAN->MSR & CAN_MSR_INAK) == 0)
  {
    InitStatus = CANINITFAILED;
  }
  else
  {
    /* Set the time triggered communication mode [设置定时器触发通讯模式]*/
    if (CAN_InitStruct->CAN_TTCM == ENABLE)
    {
      CAN->MCR |= CAN_MCR_TTCM;
    }
    else
    {
      CAN->MCR &= ~CAN_MCR_TTCM;
    }

    /* Set the automatic bus-off management [设置自动总线关闭管理]*/
    if (CAN_InitStruct->CAN_ABOM == ENABLE)
    {
      CAN->MCR |= CAN_MCR_ABOM;
    }
    else
    {
      CAN->MCR &= ~CAN_MCR_ABOM;
    }

    /* Set the automatic wake-up mode [设置自动唤醒模式]*/
    if (CAN_InitStruct->CAN_AWUM == ENABLE)
    {
      CAN->MCR |= CAN_MCR_AWUM;
    }
    else
    {
      CAN->MCR &= ~CAN_MCR_AWUM;
    }

    /* Set the no automatic retransmission [设置非自动重发]*/
    if (CAN_InitStruct->CAN_NART == ENABLE)
    {
      CAN->MCR |= CAN_MCR_NART;
    }
    else
    {
      CAN->MCR &= ~CAN_MCR_NART;
    }

    /* Set the receive FIFO locked mode [设置接收FIFO锁定模式]*/
    if (CAN_InitStruct->CAN_RFLM == ENABLE)
    {
      CAN->MCR |= CAN_MCR_RFLM;
    }
    else
    {
      CAN->MCR &= ~CAN_MCR_RFLM;
    }

    /* Set the transmit FIFO priority [设置发送FIFO优先级]*/
    if (CAN_InitStruct->CAN_TXFP == ENABLE)
    {
      CAN->MCR |= CAN_MCR_TXFP;
    }
    else
    {
      CAN->MCR &= ~CAN_MCR_TXFP;
    }

    /* Set the bit timing register [设置定时器的位]*/
    CAN->BTR = (u32)((u32)CAN_InitStruct->CAN_Mode << 30) | ((u32)CAN_InitStruct->CAN_SJW << 24) |
               ((u32)CAN_InitStruct->CAN_BS1 << 16) | ((u32)CAN_InitStruct->CAN_BS2 << 20) |
               ((u32)CAN_InitStruct->CAN_Prescaler - 1);

    InitStatus = CANINITOK;

    /* Request leave initialisation [请求退出初始化]*/
    CAN->MCR &= ~CAN_MCR_INRQ;

    /* Wait the acknowledge [等待肯定的应答]*/
    for(WaitAck = 0x400; WaitAck > 0x0; WaitAck--)
    {
    }
    
    /* ...and check acknowledged [检测应答]*/
    if ((CAN->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)
    {
      InitStatus = CANINITFAILED;
    }
  }

  /* At this step, return the status of initialization [在这步，返回初始化状态]*/
  return InitStatus;
}

/*******************************************************************************
* 函数名称: CAN_FilterInit
* 功能描述: 根据CAN_FilterInitStruct中指定的参数初始化CAN外围模块。
* 输入参数: CAN_FilterInitStruct: 指向结构体CAN_FilterInitTypeDef的指针，它包含了配置信息。
* 输出参数: 无.
* 返回参数: 无.
*******************************************************************************/
void CAN_FilterInit(CAN_FilterInitTypeDef* CAN_FilterInitStruct)
{
  u16 FilterNumber_BitPos = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_CAN_FILTER_NUMBER(CAN_FilterInitStruct->CAN_FilterNumber));
  assert_param(IS_CAN_FILTER_MODE(CAN_FilterInitStruct->CAN_FilterMode));
  assert_param(IS_CAN_FILTER_SCALE(CAN_FilterInitStruct->CAN_FilterScale));
  assert_param(IS_CAN_FILTER_FIFO(CAN_FilterInitStruct->CAN_FilterFIFOAssignment));
  assert_param(IS_FUNCTIONAL_STATE(CAN_FilterInitStruct->CAN_FilterActivation));

  FilterNumber_BitPos = 
  (u16)((u16)0x0001 << ((u16)CAN_FilterInitStruct->CAN_FilterNumber));

  /* Initialisation mode for the filter [初始化滤波器模式]*/
  CAN->FMR |= CAN_FMR_FINIT;

  /* Filter Deactivation [禁止滤波器]*/
  CAN->FA1R &= ~(u32)FilterNumber_BitPos;

  /* Filter Scale [滤波器尺度]*/
  if (CAN_FilterInitStruct->CAN_FilterScale == CAN_FilterScale_16bit)
  {
    /* 16-bit scale for the filter [16位的滤波器尺]*/
    CAN->FS1R &= ~(u32)FilterNumber_BitPos;

    /* First 16-bit identifier and First 16-bit mask [第一个16位标示符和第一个16位的屏蔽]*/
    /* Or First 16-bit identifier and Second 16-bit identifier [或第一个16位标示符和第二个16位的标示符]*/
    CAN->sFilterRegister[CAN_FilterInitStruct->CAN_FilterNumber].FR1 = 
    ((u32)((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterMaskIdLow) << 16) |
        ((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterIdLow);

    /* Second 16-bit identifier and Second 16-bit mask [第二个16位标示符和第一个16位的屏蔽]*/
    /* Or Third 16-bit identifier and Fourth 16-bit identifier [或第三个16位标示符和第四个16位的标示符]*/
    CAN->sFilterRegister[CAN_FilterInitStruct->CAN_FilterNumber].FR2 = 
    ((u32)((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterMaskIdHigh) << 16) |
        ((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterIdHigh);
  }
  if (CAN_FilterInitStruct->CAN_FilterScale == CAN_FilterScale_32bit)
  {
    /* 32-bit scale for the filter [32位的滤波器尺]*/
    CAN->FS1R |= FilterNumber_BitPos;

    /* 32-bit identifier or First 32-bit identifier [32位标示符和第一个32位的标示符]*/
    CAN->sFilterRegister[CAN_FilterInitStruct->CAN_FilterNumber].FR1 = 
    ((u32)((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterIdHigh) << 16) |
        ((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterIdLow);

    /* 32-bit mask or Second 32-bit identifier [32位的屏蔽或第二个32位标示符]*/
    CAN->sFilterRegister[CAN_FilterInitStruct->CAN_FilterNumber].FR2 = 
    ((u32)((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterMaskIdHigh) << 16) |
        ((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterMaskIdLow);

  }

  /* Filter Mode [滤波器模式]*/
  if (CAN_FilterInitStruct->CAN_FilterMode == CAN_FilterMode_IdMask)
  {
    /*Id/Mask mode for the filter[滤波器Id/Mask模式]*/
    CAN->FM1R &= ~(u32)FilterNumber_BitPos;
  }
  else /* CAN_FilterInitStruct->CAN_FilterMode == CAN_FilterMode_IdList */
  {
    /*Identifier list mode for the filter滤波器标示符列表模式*/
    CAN->FM1R |= (u32)FilterNumber_BitPos;
  }

  /* Filter FIFO assignment [滤波器FIFO分配]*/
  if (CAN_FilterInitStruct->CAN_FilterFIFOAssignment == CAN_FilterFIFO0)
  {
    /* FIFO 0 assignation for the filter [FIFO 0分配给滤波器]*/
    CAN->FFA1R &= ~(u32)FilterNumber_BitPos;
  }
  if (CAN_FilterInitStruct->CAN_FilterFIFOAssignment == CAN_FilterFIFO1)
  {
    /* FIFO 1 assignation for the filter [FIFO 1分配给滤波器]*/
    CAN->FFA1R |= (u32)FilterNumber_BitPos;
  }
  
  /* Filter activation [激活滤波器]*/
  if (CAN_FilterInitStruct->CAN_FilterActivation == ENABLE)
  {
    CAN->FA1R |= FilterNumber_BitPos;
  }

  /* Leave the initialisation mode for the filter [退出滤波器初始化模式]*/
  CAN->FMR &= ~CAN_FMR_FINIT;
}

/*******************************************************************************
* 函数名称: CAN_StructInit
* 功能描述: 将CAN_InitStruct每个成员变量置为默认值
* 输入参数: CAN_InitStruct: 指向将要被初始化的结构体CAN_InitTypeDef。
* 输出参数: 无.
* 返回参数: 无.
*******************************************************************************/
void CAN_StructInit(CAN_InitTypeDef* CAN_InitStruct)
{
  /* Reset CAN init structure parameters values [复位CAN初始化结构参考值]*/

  /* Initialize the time triggered communication mode [初始化定时器触发通讯模式]*/
  CAN_InitStruct->CAN_TTCM = DISABLE;

  /* Initialize the automatic bus-off management [初始化自动总线断开管理]*/
  CAN_InitStruct->CAN_ABOM = DISABLE;

  /* Initialize the automatic wake-up mode [初始化自动唤醒模式]*/
  CAN_InitStruct->CAN_AWUM = DISABLE;

  /* Initialize the no automatic retransmission [初始化非自动发送模式]*/
  CAN_InitStruct->CAN_NART = DISABLE;

  /* Initialize the receive FIFO locked mode [初始化接收FIFO锁定模式]*/
  CAN_InitStruct->CAN_RFLM = DISABLE;

  /* Initialize the transmit FIFO priority [初始化发送FIFO优先级]*/
  CAN_InitStruct->CAN_TXFP = DISABLE;

  /* Initialize the CAN_Mode member [初始化CAN_Mode成员]*/
  CAN_InitStruct->CAN_Mode = CAN_Mode_Normal;

  /* Initialize the CAN_SJW member [初始化CAN_SJW成员]*/
  CAN_InitStruct->CAN_SJW = CAN_SJW_1tq;

  /* Initialize the CAN_BS1 member [初始化CAN_BS1成员]*/
  CAN_InitStruct->CAN_BS1 = CAN_BS1_4tq;

  /* Initialize the CAN_BS2 member [初始化CAN_BS2成员]*/
  CAN_InitStruct->CAN_BS2 = CAN_BS2_3tq;

  /* Initialize the CAN_Prescaler member [初始化CAN_Prescaler成员]*/
  CAN_InitStruct->CAN_Prescaler = 1;
}

/*******************************************************************************
* 函数名称: CAN_ITConfig
* 功能描述: 使能/禁止指定的CAN中断.
* 输入参数: CAN_IT: 配置使能/禁止的CAN中断源。
*                    这个参数可以是: CAN_IT_TME, CAN_IT_FMP0, CAN_IT_FF0,
*                                           CAN_IT_FOV0, CAN_IT_FMP1, CAN_IT_FF1,
*                                           CAN_IT_FOV1, CAN_IT_EWG, CAN_IT_EPV,
*                                           CAN_IT_LEC, CAN_IT_ERR, CAN_IT_WKU or
*                                           CAN_IT_SLK.
*           NewState: CAN中断的新状态。该参数可以是：ENABLE或DISABLE
* 输出参数: 无.
* 返回参数: 无.
*******************************************************************************/
void CAN_ITConfig(u32 CAN_IT, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_CAN_ITConfig(CAN_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected CAN interrupt [使能选择的CAN中断]*/
    CAN->IER |= CAN_IT;
  }
  else
  {
    /* Disable the selected CAN interrupt [禁止选择的CAN中断]*/
    CAN->IER &= ~CAN_IT;
  }
}

/*******************************************************************************
* 函数名称: CAN_Transmit
* 功能描述: 初始化报文的发送.
* 输入参数: TxMessage: 指向一个结构体的指针，它包括CAN ID, CAN DLC和CAN数据.
* 输出参数: 无.
* 返回参数: 返回值是用于发送的邮箱数或者是没有空邮箱时的CAN_NO_MB值.
*******************************************************************************/
u8 CAN_Transmit(CanTxMsg* TxMessage)
{
  u8 TransmitMailbox = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_CAN_STDID(TxMessage->StdId));
  assert_param(IS_CAN_EXTID(TxMessage->StdId));
  assert_param(IS_CAN_IDTYPE(TxMessage->IDE));
  assert_param(IS_CAN_RTR(TxMessage->RTR));
  assert_param(IS_CAN_DLC(TxMessage->DLC));

  /* Select one empty transmit mailbox [选择一个空的发送邮箱]*/
  if ((CAN->TSR&CAN_TSR_TME0) == CAN_TSR_TME0)
  {
    TransmitMailbox = 0;
  }
  else if ((CAN->TSR&CAN_TSR_TME1) == CAN_TSR_TME1)
  {
    TransmitMailbox = 1;
  }
  else if ((CAN->TSR&CAN_TSR_TME2) == CAN_TSR_TME2)
  {
    TransmitMailbox = 2;
  }
  else
  {
    TransmitMailbox = CAN_NO_MB;
  }

  if (TransmitMailbox != CAN_NO_MB)
  {
    /* Set up the Id [设置Id]*/
    CAN->sTxMailBox[TransmitMailbox].TIR &= CAN_TMIDxR_TXRQ;
    if (TxMessage->IDE == CAN_ID_STD)
    {
      TxMessage->StdId &= (u32)0x000007FF;
      TxMessage->StdId = TxMessage->StdId << 21;
      
      CAN->sTxMailBox[TransmitMailbox].TIR |= (TxMessage->StdId | TxMessage->IDE |
                                               TxMessage->RTR);
    }
    else
    {
      TxMessage->ExtId &= (u32)0x1FFFFFFF;
      TxMessage->ExtId <<= 3;

      CAN->sTxMailBox[TransmitMailbox].TIR |= (TxMessage->ExtId | TxMessage->IDE | 
                                               TxMessage->RTR);
    }
    
    /* Set up the DLC [设置DLC]*/
    TxMessage->DLC &= (u8)0x0000000F;
    CAN->sTxMailBox[TransmitMailbox].TDTR &= (u32)0xFFFFFFF0;
    CAN->sTxMailBox[TransmitMailbox].TDTR |= TxMessage->DLC;

    /* Set up the data field [设置数据失败]*/
    CAN->sTxMailBox[TransmitMailbox].TDLR = (((u32)TxMessage->Data[3] << 24) | 
                                             ((u32)TxMessage->Data[2] << 16) |
                                             ((u32)TxMessage->Data[1] << 8) | 
                                             ((u32)TxMessage->Data[0]));
    CAN->sTxMailBox[TransmitMailbox].TDHR = (((u32)TxMessage->Data[7] << 24) | 
                                             ((u32)TxMessage->Data[6] << 16) |
                                             ((u32)TxMessage->Data[5] << 8) |
                                             ((u32)TxMessage->Data[4]));

    /* Request transmission [请求发送]*/
    CAN->sTxMailBox[TransmitMailbox].TIR |= CAN_TMIDxR_TXRQ;
  }

  return TransmitMailbox;
}

/*******************************************************************************
* 函数名称: CAN_TransmitStatus
* 功能描述: 检查报文发送的状态.
* 输入参数: TransmitMailbox: 用于发送的邮箱号码.
* 输出参数: 无.
* 返回参数: CANTXOK:CAN驱动正在发送报文;CANTXPENDING:报文被挂起;CANTXFAILED:其他.
*******************************************************************************/
u8 CAN_TransmitStatus(u8 TransmitMailbox)
{
  /* RQCP, TXOK and TME bits */
  u8 State = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_CAN_TRANSMITMAILBOX(TransmitMailbox));

  switch (TransmitMailbox)
  {
    case (0): State |= (u8)((CAN->TSR & CAN_TSR_RQCP0) << 2);
      State |= (u8)((CAN->TSR & CAN_TSR_TXOK0) >> 0);
      State |= (u8)((CAN->TSR & CAN_TSR_TME0) >> 26);
      break;
    case (1): State |= (u8)((CAN->TSR & CAN_TSR_RQCP1) >> 6);
      State |= (u8)((CAN->TSR & CAN_TSR_TXOK1) >> 8);
      State |= (u8)((CAN->TSR & CAN_TSR_TME1) >> 27);
      break;
    case (2): State |= (u8)((CAN->TSR & CAN_TSR_RQCP2) >> 14);
      State |= (u8)((CAN->TSR & CAN_TSR_TXOK2) >> 16);
      State |= (u8)((CAN->TSR & CAN_TSR_TME2) >> 28);
      break;
    default:
      State = CANTXFAILED;
      break;
  }

  switch (State)
  {
      /* transmit pending   [发送挂起]*/
    case (0x0): State = CANTXPENDING;
      break;
      /* transmit failed  [发送失败]*/
    case (0x5): State = CANTXFAILED;
      break;
      /* transmit succedeed  [发送成功]*/
    case (0x7): State = CANTXOK;
      break;
    default:
      State = CANTXFAILED;
      break;
  }

  return State;
}

/*******************************************************************************
* 函数名称: CAN_CancelTransmit
* 功能描述: 取消一个发送请求.
* 输入参数: Mailbox number:邮箱号.
* 输出参数: 无.
* 返回参数: 无.
*******************************************************************************/
void CAN_CancelTransmit(u8 Mailbox)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_CAN_TRANSMITMAILBOX(Mailbox));

  /* abort transmission [放弃发送]*/
  switch (Mailbox)
  {
    case (0): CAN->TSR |= CAN_TSR_ABRQ0;
      break;
    case (1): CAN->TSR |= CAN_TSR_ABRQ1;
      break;
    case (2): CAN->TSR |= CAN_TSR_ABRQ2;
      break;
    default:
      break;
  }
}

/*******************************************************************************
* 函数名称: CAN_FIFORelease
* 功能描述: 释放一个FIFO.
* 输入参数: FIFO号码: 需要释放的FIFO，CANFIFO0或CANFIFO.
* 输出参数: 无.
* 返回参数: 无.
*******************************************************************************/
void CAN_FIFORelease(u8 FIFONumber)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_CAN_FIFO(FIFONumber));

  /* Release FIFO0 [释放FIFO0]*/
  if (FIFONumber == CAN_FIFO0)
  {
    CAN->RF0R = CAN_RF0R_RFOM0;
  }
  /* Release FIFO1 [释放FIFO1]*/
  else /* FIFONumber == CAN_FIFO1 */
  {
    CAN->RF1R = CAN_RF1R_RFOM1;
  }
}

/*******************************************************************************
* 函数名称: CAN_MessagePending
* 功能描述: 返回挂起报文的数量.
* 输入参数: FIFO号码：接收FIFO，CANFIFO0或CANFIFO1.
* 输出参数: 无.
* 返回参数: NbMessage，挂起报文的数量。
*******************************************************************************/
u8 CAN_MessagePending(u8 FIFONumber)
{
  u8 MessagePending=0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_CAN_FIFO(FIFONumber));

  if (FIFONumber == CAN_FIFO0)
  {
    MessagePending = (u8)(CAN->RF0R&(u32)0x03);
  }
  else if (FIFONumber == CAN_FIFO1)
  {
    MessagePending = (u8)(CAN->RF1R&(u32)0x03);
  }
  else
  {
    MessagePending = 0;
  }
  return MessagePending;
}

/*******************************************************************************
* 函数名称: CAN_Receive
* 功能描述: 接收一个报文.
* 输入参数: FIFO number: 接收FIFO的编号，CANFIFO0或CANFIFO1.
* 输出参数: RxMessage: 指向一个包括CAN ID,CAN DLC和CAN数据的结构体。
* 返回参数: 无.
*******************************************************************************/
void CAN_Receive(u8 FIFONumber, CanRxMsg* RxMessage)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_CAN_FIFO(FIFONumber));

  /* Get the Id [取得Id]*/
  RxMessage->IDE = (u8)0x04 & CAN->sFIFOMailBox[FIFONumber].RIR;
  if (RxMessage->IDE == CAN_ID_STD)
  {
    RxMessage->StdId = (u32)0x000007FF & (CAN->sFIFOMailBox[FIFONumber].RIR >> 21);
  }
  else
  {
    RxMessage->ExtId = (u32)0x1FFFFFFF & (CAN->sFIFOMailBox[FIFONumber].RIR >> 3);
  }
  
  RxMessage->RTR = (u8)0x02 & CAN->sFIFOMailBox[FIFONumber].RIR;

  /* Get the DLC [取得DLC]*/
  RxMessage->DLC = (u8)0x0F & CAN->sFIFOMailBox[FIFONumber].RDTR;

  /* Get the FMI [取得FMI]*/
  RxMessage->FMI = (u8)0xFF & (CAN->sFIFOMailBox[FIFONumber].RDTR >> 8);

  /* Get the data field [取得数据字段]*/
  RxMessage->Data[0] = (u8)0xFF & CAN->sFIFOMailBox[FIFONumber].RDLR;
  RxMessage->Data[1] = (u8)0xFF & (CAN->sFIFOMailBox[FIFONumber].RDLR >> 8);
  RxMessage->Data[2] = (u8)0xFF & (CAN->sFIFOMailBox[FIFONumber].RDLR >> 16);
  RxMessage->Data[3] = (u8)0xFF & (CAN->sFIFOMailBox[FIFONumber].RDLR >> 24);

  RxMessage->Data[4] = (u8)0xFF & CAN->sFIFOMailBox[FIFONumber].RDHR;
  RxMessage->Data[5] = (u8)0xFF & (CAN->sFIFOMailBox[FIFONumber].RDHR >> 8);
  RxMessage->Data[6] = (u8)0xFF & (CAN->sFIFOMailBox[FIFONumber].RDHR >> 16);
  RxMessage->Data[7] = (u8)0xFF & (CAN->sFIFOMailBox[FIFONumber].RDHR >> 24);

  /* Release the FIFO [释放FIFO]*/
  CAN_FIFORelease(FIFONumber);
}

/*******************************************************************************
* 函数名称: CAN_Sleep
* 功能描述: 将CAN置于低功耗模式.
* 输入参数: 无.
* 输出参数: 无.
* 返回参数: CANSLEEPOK（如果进入睡眠）CANSLEEPFAILED（其他情况）.
*******************************************************************************/
u8 CAN_Sleep(void)
{
  u8 SleepStatus = 0;

  /* Sleep mode entering request [进入睡眠模式请求]*/
  CAN->MCR |= CAN_MCR_SLEEP;
  SleepStatus = CANSLEEPOK;

  /* Sleep mode status [睡眠模式状态]*/
  if ((CAN->MCR&CAN_MCR_SLEEP) == 0)
  {
    /* Sleep mode not entered [进不去睡眠模式]*/
    SleepStatus = CANSLEEPFAILED;
  }

  /* At this step, sleep mode status [在这步，睡眠模式状态]*/
  return SleepStatus;
}

/*******************************************************************************
* 函数名称: CAN_WakeUp
* 功能描述: 唤醒CAN.
* 输入参数: 无.
* 输出参数: 无.
* 返回参数: CANWAKEUPOK（如果退出了睡眠模式）CANWAKEUPFAILED（其他情况）.
*******************************************************************************/
u8 CAN_WakeUp(void)
{
  u8 WakeUpStatus = 0;

  /* Wake up request [唤醒请求]*/
  CAN->MCR &= ~CAN_MCR_SLEEP;
  WakeUpStatus = CANWAKEUPFAILED;

  /* Sleep mode status [睡眠模式状态]*/
  if ((CAN->MCR&CAN_MCR_SLEEP) == 0)
  {
    /* Sleep mode exited [退出睡眠模式]*/
    WakeUpStatus = CANWAKEUPOK;
  }

  /* At this step, sleep mode status [在这步，睡眠模式状态]*/
  return WakeUpStatus;
}

/*******************************************************************************
* 函数名称: CAN_GetFlagStatus
* 功能描述: 检查指定的CAN标志是否置位.
* 输入参数: CAN_FLAG: 指定要检查的标志.
*                  这个参数可以是: CAN_FLAG_EWG, CAN_FLAG_EPV或CAN_FLAG_BOF.
* 输出参数: 无.
* 返回参数: CAN_FLAG的新状态（SET或RESET）.
*******************************************************************************/
FlagStatus CAN_GetFlagStatus(u32 CAN_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [检查参数]*/
  assert_param(IS_CAN_FLAG(CAN_FLAG));

  /* Check the status of the specified CAN flag [检查指定的CAN标志状态]*/
  if ((CAN->ESR & CAN_FLAG) != (u32)RESET)
  {
    /* CAN_FLAG is set [置位CAN_FLAG]*/
    bitstatus = SET;
  }
  else
  {
    /* CAN_FLAG is reset [复位CAN_FLAG]*/
    bitstatus = RESET;
  }
  /* Return the CAN_FLAG status [返回CAN_FLAG状态]*/
  return  bitstatus;
}

/*******************************************************************************
* 函数名称: CAN_ClearFlag
* 功能描述: 清除CAN挂起标志.
* 输入参数: CAN_FLAG: 指定需要清除的标志。
* 输出参数: 无.
* 返回参数: 无.
*******************************************************************************/
void CAN_ClearFlag(u32 CAN_FLAG)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_CAN_FLAG(CAN_FLAG));

  /* Clear the selected CAN flags [清除选择的CAN标志]*/
  CAN->ESR &= ~CAN_FLAG;
}

/*******************************************************************************
* 函数名称: CAN_GetITStatus
* 功能描述: 检查指定的CAN中断是否发生.
* 输入参数: CAN_IT: CAN需要检查的中断源。
*                  这个参数可以是: CAN_IT_RQCP0, CAN_IT_RQCP1, CAN_IT_RQCP2,
*                                         CAN_IT_FF0, CAN_IT_FOV0, CAN_IT_FF1,
*                                         CAN_IT_FOV1, CAN_IT_EWG, CAN_IT_EPV, 
*                                         CAN_IT_BOF, CAN_IT_WKU or CAN_IT_SLK.
* 输出参数: 无.
* 返回参数: CAN_IT的新状态（SET或RESET）.
*******************************************************************************/
ITStatus CAN_GetITStatus(u32 CAN_IT)
{
  ITStatus pendingbitstatus = RESET;

  /* Check the parameters [检查参数]*/
  assert_param(IS_CAN_ITStatus(CAN_IT));

  switch (CAN_IT)
  {
    case CAN_IT_RQCP0:
      pendingbitstatus = CheckITStatus(CAN->TSR, CAN_TSR_RQCP0);
      break;
    case CAN_IT_RQCP1:
      pendingbitstatus = CheckITStatus(CAN->TSR, CAN_TSR_RQCP1);
      break;
    case CAN_IT_RQCP2:
      pendingbitstatus = CheckITStatus(CAN->TSR, CAN_TSR_RQCP2);
      break;
    case CAN_IT_FF0:
      pendingbitstatus = CheckITStatus(CAN->RF0R, CAN_RF0R_FULL0);
      break;
    case CAN_IT_FOV0:
      pendingbitstatus = CheckITStatus(CAN->RF0R, CAN_RF0R_FOVR0);
      break;
    case CAN_IT_FF1:
      pendingbitstatus = CheckITStatus(CAN->RF1R, CAN_RF1R_FULL1);
      break;
    case CAN_IT_FOV1:
      pendingbitstatus = CheckITStatus(CAN->RF1R, CAN_RF1R_FOVR1);
      break;
    case CAN_IT_EWG:
      pendingbitstatus = CheckITStatus(CAN->ESR, CAN_ESR_EWGF);
      break;
    case CAN_IT_EPV:
      pendingbitstatus = CheckITStatus(CAN->ESR, CAN_ESR_EPVF);
      break;
    case CAN_IT_BOF:
      pendingbitstatus = CheckITStatus(CAN->ESR, CAN_ESR_BOFF);
      break;
    case CAN_IT_SLK:
      pendingbitstatus = CheckITStatus(CAN->MSR, CAN_MSR_SLAKI);
      break;
    case CAN_IT_WKU:
      pendingbitstatus = CheckITStatus(CAN->MSR, CAN_MSR_WKUI);
      break;

    default :
      pendingbitstatus = RESET;
      break;
  }

  /* Return the CAN_IT status [返回CAN_IT状态]*/
  return  pendingbitstatus;
}

/*******************************************************************************
* 函数名称: CAN_ClearITPendingBit
* 功能描述: 清除CAN中断挂起位.
* 输入参数: CAN_IT: 指定需要清除的中断挂起位。
* 输出参数: 无.
* 返回参数: 无.
*******************************************************************************/
void CAN_ClearITPendingBit(u32 CAN_IT)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_CAN_ITStatus(CAN_IT));

  switch (CAN_IT)
  {
    case CAN_IT_RQCP0:
      CAN->TSR = CAN_TSR_RQCP0; /* rc_w1*/
      break;
    case CAN_IT_RQCP1:
      CAN->TSR = CAN_TSR_RQCP1; /* rc_w1*/
      break;
    case CAN_IT_RQCP2:
      CAN->TSR = CAN_TSR_RQCP2; /* rc_w1*/
      break;
    case CAN_IT_FF0:
      CAN->RF0R = CAN_RF0R_FULL0; /* rc_w1*/
      break;
    case CAN_IT_FOV0:
      CAN->RF0R = CAN_RF0R_FOVR0; /* rc_w1*/
      break;
    case CAN_IT_FF1:
      CAN->RF1R = CAN_RF1R_FULL1; /* rc_w1*/
      break;
    case CAN_IT_FOV1:
      CAN->RF1R = CAN_RF1R_FOVR1; /* rc_w1*/
      break;
    case CAN_IT_EWG:
      CAN->ESR &= ~ CAN_ESR_EWGF; /* rw */
      break;
    case CAN_IT_EPV:
      CAN->ESR &= ~ CAN_ESR_EPVF; /* rw */
      break;
    case CAN_IT_BOF:
      CAN->ESR &= ~ CAN_ESR_BOFF; /* rw */
      break;
    case CAN_IT_WKU:
      CAN->MSR = CAN_MSR_WKUI;  /* rc_w1*/
      break;
    case CAN_IT_SLK:
      CAN->MSR = CAN_MSR_SLAKI;  /* rc_w1*/
      break;
    default :
      break;
  }
}

/*******************************************************************************
* 函数名称: CheckITStatus
* 功能描述: 检查CAN中断是否发生.
* 输入参数: (1)CAN_Reg: 要检查的指定的CAN中断寄存器.
*           (2)It_Bit: 要检查的指定的中断源位.
* 输出参数: 无.
* 返回参数: CAN中断的新状态 (SET or RESET).
*******************************************************************************/
static ITStatus CheckITStatus(u32 CAN_Reg, u32 It_Bit)
{
  ITStatus pendingbitstatus = RESET;

  if ((CAN_Reg & It_Bit) != (u32)RESET)
  {
    /* CAN_IT is set [置位CAN_IT]*/
    pendingbitstatus = SET;
  }
  else
  {
    /* CAN_IT is reset [复位CAN_IT]*/
    pendingbitstatus = RESET;
  }

  return pendingbitstatus;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
