/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_can.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������CAN�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_can.h"
#include "stm32f10x_rcc.h"

/* �������� -----------------------------------------------------------*/

/* ���ö��� ------------------------------------------------------------*/
/* CAN Master Control Register bits [CAN���ؼĴ���λ]*/
#define CAN_MCR_INRQ     ((u32)0x00000001) /* Initialization request [��ʼ������]*/
#define CAN_MCR_SLEEP    ((u32)0x00000002) /* Sleep mode request [˯��ģʽ����]*/
#define CAN_MCR_TXFP     ((u32)0x00000004) /* Transmit FIFO priority [����FIFO���ȼ�]*/
#define CAN_MCR_RFLM     ((u32)0x00000008) /* Receive FIFO locked mode [����FIFO����ģʽ]*/
#define CAN_MCR_NART     ((u32)0x00000010) /* No automatic retransmission [���Զ��ط�]*/
#define CAN_MCR_AWUM     ((u32)0x00000020) /* Automatic wake up mode [�Զ�����ģʽ]*/
#define CAN_MCR_ABOM     ((u32)0x00000040) /* Automatic bus-off management [�Զ����߹ضϹ���]*/
#define CAN_MCR_TTCM     ((u32)0x00000080) /* time triggered communication [��ʱ������ͨѶ]*/

/* CAN Master Status Register bits [CAN����ģʽ״̬�Ĵ���λ]*/
#define CAN_MSR_INAK     ((u32)0x00000001)    /* Initialization acknowledge [��ʼ��ȷ��]*/
#define CAN_MSR_WKUI     ((u32)0x00000008)    /* Wake-up interrupt [�����ж�]*/
#define CAN_MSR_SLAKI    ((u32)0x00000010)    /* Sleep acknowledge interrupt [˯��ȷ���ж�]*/

/* CAN Transmit Status Register bits [CAN����״̬�Ĵ���λ]*/
#define CAN_TSR_RQCP0    ((u32)0x00000001)    /* Request completed mailbox0 [����0�������]*/
#define CAN_TSR_TXOK0    ((u32)0x00000002)    /* Transmission OK of mailbox0 [����0����OK]*/
#define CAN_TSR_ABRQ0    ((u32)0x00000080)    /* Abort request for mailbox0 [����0����ʧ��]*/
#define CAN_TSR_RQCP1    ((u32)0x00000100)    /* Request completed mailbox1 [����1�������]*/
#define CAN_TSR_TXOK1    ((u32)0x00000200)    /* Transmission OK of mailbox1 [����1����OK]*/
#define CAN_TSR_ABRQ1    ((u32)0x00008000)    /* Abort request for mailbox1 [����1����ʧ��]*/
#define CAN_TSR_RQCP2    ((u32)0x00010000)    /* Request completed mailbox2 [����2�������]*/
#define CAN_TSR_TXOK2    ((u32)0x00020000)    /* Transmission OK of mailbox2 [����1����OK]*/
#define CAN_TSR_ABRQ2    ((u32)0x00800000)    /* Abort request for mailbox2 [����2����ʧ��]*/
#define CAN_TSR_TME0     ((u32)0x04000000)    /* Transmit mailbox 0 empty [��������0��]*/
#define CAN_TSR_TME1     ((u32)0x08000000)    /* Transmit mailbox 1 empty [��������1��]*/
#define CAN_TSR_TME2     ((u32)0x10000000)    /* Transmit mailbox 2 empty [��������2��]*/

/* CAN Receive FIFO 0 Register bits [CAN����FIFO 0�Ĵ���λ]*/
#define CAN_RF0R_FULL0   ((u32)0x00000008)    /* FIFO 0 full [FIFO 0��]*/
#define CAN_RF0R_FOVR0   ((u32)0x00000010)    /* FIFO 0 overrun [FIFO 0���]*/
#define CAN_RF0R_RFOM0   ((u32)0x00000020)    /* Release FIFO 0 output mailbox [�ͷ�FIFO 0�������]*/

/* CAN Receive FIFO 1 Register bits [CAN����FIFO 1�Ĵ���λ]*/
#define CAN_RF1R_FULL1   ((u32)0x00000008)    /* FIFO 1 full [FIFO 1��]*/
#define CAN_RF1R_FOVR1   ((u32)0x00000010)    /* FIFO 1 overrun [FIFO 1���]*/
#define CAN_RF1R_RFOM1   ((u32)0x00000020)    /* Release FIFO 1 output mailbox [�ͷ�FIFO 1�������]*/

/* CAN Error Status Register bits [CAN����״̬�Ĵ���λ]*/
#define CAN_ESR_EWGF     ((u32)0x00000001)    /* Error warning flag [���󾯸��־]*/
#define CAN_ESR_EPVF     ((u32)0x00000002)    /* Error passive flag [���󱻶���־]*/
#define CAN_ESR_BOFF     ((u32)0x00000004)    /* Bus-off flag [���߹ضϱ�־]*/

/* CAN Mailbox Transmit Request [CAN���䷢������]*/
#define CAN_TMIDxR_TXRQ    ((u32)0x00000001) /* Transmit mailbox request [������������]*/

/* CAN Filter Master Register bits [CAN�˲������Ĵ���λ]*/
#define CAN_FMR_FINIT ((u32)0x00000001) /* Filter init mode [�˲�����ʼ��ģʽ]*/


/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
static ITStatus CheckITStatus(u32 CAN_Reg, u32 It_Bit);

/* ���ú��� ---------------------------------------------------------*/
/*******************************************************************************
* ��������: CAN_DeInit
* ��������: D��CAN����Ĵ�����λΪ���ǵ�Ĭ��ֵ
* �������: ��.
* �������: ��.
* ���ز���: ��.
*******************************************************************************/
void CAN_DeInit(void)
{
  /* Enable CAN reset state */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN, ENABLE);
  /* Release CAN from reset state */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN, DISABLE);
}

/*******************************************************************************
* ��������: CAN_Init
* ��������: ����CAN_InitStruct��ָ���Ĳ�����ʼ��CAN��Χģ�顣
* �������: CAN_InitStruct: ָ��ṹ��CAN_InitTypeDef��ָ�룬������CAN��Χģ���������Ϣ��
* �������: ��.
* ���ز���: һ��˵��CAN��ʼ���Ƿ�ɹ��ĳ�����CANINITFAILED �� ��ʼ��ʧ��
*                                            CANINITOK �� ��ʼ���ɹ�
*******************************************************************************/
u8 CAN_Init(CAN_InitTypeDef* CAN_InitStruct)
{
  u8 InitStatus = 0;
  u16 WaitAck = 0;

  /* Check the parameters [������]*/
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

  /* Request initialisation [�����ʼ��]*/
  CAN->MCR = CAN_MCR_INRQ;

  /* ...and check acknowledged [���Ӧ��]*/
  if ((CAN->MSR & CAN_MSR_INAK) == 0)
  {
    InitStatus = CANINITFAILED;
  }
  else
  {
    /* Set the time triggered communication mode [���ö�ʱ������ͨѶģʽ]*/
    if (CAN_InitStruct->CAN_TTCM == ENABLE)
    {
      CAN->MCR |= CAN_MCR_TTCM;
    }
    else
    {
      CAN->MCR &= ~CAN_MCR_TTCM;
    }

    /* Set the automatic bus-off management [�����Զ����߹رչ���]*/
    if (CAN_InitStruct->CAN_ABOM == ENABLE)
    {
      CAN->MCR |= CAN_MCR_ABOM;
    }
    else
    {
      CAN->MCR &= ~CAN_MCR_ABOM;
    }

    /* Set the automatic wake-up mode [�����Զ�����ģʽ]*/
    if (CAN_InitStruct->CAN_AWUM == ENABLE)
    {
      CAN->MCR |= CAN_MCR_AWUM;
    }
    else
    {
      CAN->MCR &= ~CAN_MCR_AWUM;
    }

    /* Set the no automatic retransmission [���÷��Զ��ط�]*/
    if (CAN_InitStruct->CAN_NART == ENABLE)
    {
      CAN->MCR |= CAN_MCR_NART;
    }
    else
    {
      CAN->MCR &= ~CAN_MCR_NART;
    }

    /* Set the receive FIFO locked mode [���ý���FIFO����ģʽ]*/
    if (CAN_InitStruct->CAN_RFLM == ENABLE)
    {
      CAN->MCR |= CAN_MCR_RFLM;
    }
    else
    {
      CAN->MCR &= ~CAN_MCR_RFLM;
    }

    /* Set the transmit FIFO priority [���÷���FIFO���ȼ�]*/
    if (CAN_InitStruct->CAN_TXFP == ENABLE)
    {
      CAN->MCR |= CAN_MCR_TXFP;
    }
    else
    {
      CAN->MCR &= ~CAN_MCR_TXFP;
    }

    /* Set the bit timing register [���ö�ʱ����λ]*/
    CAN->BTR = (u32)((u32)CAN_InitStruct->CAN_Mode << 30) | ((u32)CAN_InitStruct->CAN_SJW << 24) |
               ((u32)CAN_InitStruct->CAN_BS1 << 16) | ((u32)CAN_InitStruct->CAN_BS2 << 20) |
               ((u32)CAN_InitStruct->CAN_Prescaler - 1);

    InitStatus = CANINITOK;

    /* Request leave initialisation [�����˳���ʼ��]*/
    CAN->MCR &= ~CAN_MCR_INRQ;

    /* Wait the acknowledge [�ȴ��϶���Ӧ��]*/
    for(WaitAck = 0x400; WaitAck > 0x0; WaitAck--)
    {
    }
    
    /* ...and check acknowledged [���Ӧ��]*/
    if ((CAN->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)
    {
      InitStatus = CANINITFAILED;
    }
  }

  /* At this step, return the status of initialization [���ⲽ�����س�ʼ��״̬]*/
  return InitStatus;
}

/*******************************************************************************
* ��������: CAN_FilterInit
* ��������: ����CAN_FilterInitStruct��ָ���Ĳ�����ʼ��CAN��Χģ�顣
* �������: CAN_FilterInitStruct: ָ��ṹ��CAN_FilterInitTypeDef��ָ�룬��������������Ϣ��
* �������: ��.
* ���ز���: ��.
*******************************************************************************/
void CAN_FilterInit(CAN_FilterInitTypeDef* CAN_FilterInitStruct)
{
  u16 FilterNumber_BitPos = 0;

  /* Check the parameters [������]*/
  assert_param(IS_CAN_FILTER_NUMBER(CAN_FilterInitStruct->CAN_FilterNumber));
  assert_param(IS_CAN_FILTER_MODE(CAN_FilterInitStruct->CAN_FilterMode));
  assert_param(IS_CAN_FILTER_SCALE(CAN_FilterInitStruct->CAN_FilterScale));
  assert_param(IS_CAN_FILTER_FIFO(CAN_FilterInitStruct->CAN_FilterFIFOAssignment));
  assert_param(IS_FUNCTIONAL_STATE(CAN_FilterInitStruct->CAN_FilterActivation));

  FilterNumber_BitPos = 
  (u16)((u16)0x0001 << ((u16)CAN_FilterInitStruct->CAN_FilterNumber));

  /* Initialisation mode for the filter [��ʼ���˲���ģʽ]*/
  CAN->FMR |= CAN_FMR_FINIT;

  /* Filter Deactivation [��ֹ�˲���]*/
  CAN->FA1R &= ~(u32)FilterNumber_BitPos;

  /* Filter Scale [�˲����߶�]*/
  if (CAN_FilterInitStruct->CAN_FilterScale == CAN_FilterScale_16bit)
  {
    /* 16-bit scale for the filter [16λ���˲�����]*/
    CAN->FS1R &= ~(u32)FilterNumber_BitPos;

    /* First 16-bit identifier and First 16-bit mask [��һ��16λ��ʾ���͵�һ��16λ������]*/
    /* Or First 16-bit identifier and Second 16-bit identifier [���һ��16λ��ʾ���͵ڶ���16λ�ı�ʾ��]*/
    CAN->sFilterRegister[CAN_FilterInitStruct->CAN_FilterNumber].FR1 = 
    ((u32)((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterMaskIdLow) << 16) |
        ((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterIdLow);

    /* Second 16-bit identifier and Second 16-bit mask [�ڶ���16λ��ʾ���͵�һ��16λ������]*/
    /* Or Third 16-bit identifier and Fourth 16-bit identifier [�������16λ��ʾ���͵��ĸ�16λ�ı�ʾ��]*/
    CAN->sFilterRegister[CAN_FilterInitStruct->CAN_FilterNumber].FR2 = 
    ((u32)((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterMaskIdHigh) << 16) |
        ((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterIdHigh);
  }
  if (CAN_FilterInitStruct->CAN_FilterScale == CAN_FilterScale_32bit)
  {
    /* 32-bit scale for the filter [32λ���˲�����]*/
    CAN->FS1R |= FilterNumber_BitPos;

    /* 32-bit identifier or First 32-bit identifier [32λ��ʾ���͵�һ��32λ�ı�ʾ��]*/
    CAN->sFilterRegister[CAN_FilterInitStruct->CAN_FilterNumber].FR1 = 
    ((u32)((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterIdHigh) << 16) |
        ((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterIdLow);

    /* 32-bit mask or Second 32-bit identifier [32λ�����λ�ڶ���32λ��ʾ��]*/
    CAN->sFilterRegister[CAN_FilterInitStruct->CAN_FilterNumber].FR2 = 
    ((u32)((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterMaskIdHigh) << 16) |
        ((u32)0x0000FFFF & CAN_FilterInitStruct->CAN_FilterMaskIdLow);

  }

  /* Filter Mode [�˲���ģʽ]*/
  if (CAN_FilterInitStruct->CAN_FilterMode == CAN_FilterMode_IdMask)
  {
    /*Id/Mask mode for the filter[�˲���Id/Maskģʽ]*/
    CAN->FM1R &= ~(u32)FilterNumber_BitPos;
  }
  else /* CAN_FilterInitStruct->CAN_FilterMode == CAN_FilterMode_IdList */
  {
    /*Identifier list mode for the filter�˲�����ʾ���б�ģʽ*/
    CAN->FM1R |= (u32)FilterNumber_BitPos;
  }

  /* Filter FIFO assignment [�˲���FIFO����]*/
  if (CAN_FilterInitStruct->CAN_FilterFIFOAssignment == CAN_FilterFIFO0)
  {
    /* FIFO 0 assignation for the filter [FIFO 0������˲���]*/
    CAN->FFA1R &= ~(u32)FilterNumber_BitPos;
  }
  if (CAN_FilterInitStruct->CAN_FilterFIFOAssignment == CAN_FilterFIFO1)
  {
    /* FIFO 1 assignation for the filter [FIFO 1������˲���]*/
    CAN->FFA1R |= (u32)FilterNumber_BitPos;
  }
  
  /* Filter activation [�����˲���]*/
  if (CAN_FilterInitStruct->CAN_FilterActivation == ENABLE)
  {
    CAN->FA1R |= FilterNumber_BitPos;
  }

  /* Leave the initialisation mode for the filter [�˳��˲�����ʼ��ģʽ]*/
  CAN->FMR &= ~CAN_FMR_FINIT;
}

/*******************************************************************************
* ��������: CAN_StructInit
* ��������: ��CAN_InitStructÿ����Ա������ΪĬ��ֵ
* �������: CAN_InitStruct: ָ��Ҫ����ʼ���Ľṹ��CAN_InitTypeDef��
* �������: ��.
* ���ز���: ��.
*******************************************************************************/
void CAN_StructInit(CAN_InitTypeDef* CAN_InitStruct)
{
  /* Reset CAN init structure parameters values [��λCAN��ʼ���ṹ�ο�ֵ]*/

  /* Initialize the time triggered communication mode [��ʼ����ʱ������ͨѶģʽ]*/
  CAN_InitStruct->CAN_TTCM = DISABLE;

  /* Initialize the automatic bus-off management [��ʼ���Զ����߶Ͽ�����]*/
  CAN_InitStruct->CAN_ABOM = DISABLE;

  /* Initialize the automatic wake-up mode [��ʼ���Զ�����ģʽ]*/
  CAN_InitStruct->CAN_AWUM = DISABLE;

  /* Initialize the no automatic retransmission [��ʼ�����Զ�����ģʽ]*/
  CAN_InitStruct->CAN_NART = DISABLE;

  /* Initialize the receive FIFO locked mode [��ʼ������FIFO����ģʽ]*/
  CAN_InitStruct->CAN_RFLM = DISABLE;

  /* Initialize the transmit FIFO priority [��ʼ������FIFO���ȼ�]*/
  CAN_InitStruct->CAN_TXFP = DISABLE;

  /* Initialize the CAN_Mode member [��ʼ��CAN_Mode��Ա]*/
  CAN_InitStruct->CAN_Mode = CAN_Mode_Normal;

  /* Initialize the CAN_SJW member [��ʼ��CAN_SJW��Ա]*/
  CAN_InitStruct->CAN_SJW = CAN_SJW_1tq;

  /* Initialize the CAN_BS1 member [��ʼ��CAN_BS1��Ա]*/
  CAN_InitStruct->CAN_BS1 = CAN_BS1_4tq;

  /* Initialize the CAN_BS2 member [��ʼ��CAN_BS2��Ա]*/
  CAN_InitStruct->CAN_BS2 = CAN_BS2_3tq;

  /* Initialize the CAN_Prescaler member [��ʼ��CAN_Prescaler��Ա]*/
  CAN_InitStruct->CAN_Prescaler = 1;
}

/*******************************************************************************
* ��������: CAN_ITConfig
* ��������: ʹ��/��ָֹ����CAN�ж�.
* �������: CAN_IT: ����ʹ��/��ֹ��CAN�ж�Դ��
*                    �������������: CAN_IT_TME, CAN_IT_FMP0, CAN_IT_FF0,
*                                           CAN_IT_FOV0, CAN_IT_FMP1, CAN_IT_FF1,
*                                           CAN_IT_FOV1, CAN_IT_EWG, CAN_IT_EPV,
*                                           CAN_IT_LEC, CAN_IT_ERR, CAN_IT_WKU or
*                                           CAN_IT_SLK.
*           NewState: CAN�жϵ���״̬���ò��������ǣ�ENABLE��DISABLE
* �������: ��.
* ���ز���: ��.
*******************************************************************************/
void CAN_ITConfig(u32 CAN_IT, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_CAN_ITConfig(CAN_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected CAN interrupt [ʹ��ѡ���CAN�ж�]*/
    CAN->IER |= CAN_IT;
  }
  else
  {
    /* Disable the selected CAN interrupt [��ֹѡ���CAN�ж�]*/
    CAN->IER &= ~CAN_IT;
  }
}

/*******************************************************************************
* ��������: CAN_Transmit
* ��������: ��ʼ�����ĵķ���.
* �������: TxMessage: ָ��һ���ṹ���ָ�룬������CAN ID, CAN DLC��CAN����.
* �������: ��.
* ���ز���: ����ֵ�����ڷ��͵�������������û�п�����ʱ��CAN_NO_MBֵ.
*******************************************************************************/
u8 CAN_Transmit(CanTxMsg* TxMessage)
{
  u8 TransmitMailbox = 0;

  /* Check the parameters [������]*/
  assert_param(IS_CAN_STDID(TxMessage->StdId));
  assert_param(IS_CAN_EXTID(TxMessage->StdId));
  assert_param(IS_CAN_IDTYPE(TxMessage->IDE));
  assert_param(IS_CAN_RTR(TxMessage->RTR));
  assert_param(IS_CAN_DLC(TxMessage->DLC));

  /* Select one empty transmit mailbox [ѡ��һ���յķ�������]*/
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
    /* Set up the Id [����Id]*/
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
    
    /* Set up the DLC [����DLC]*/
    TxMessage->DLC &= (u8)0x0000000F;
    CAN->sTxMailBox[TransmitMailbox].TDTR &= (u32)0xFFFFFFF0;
    CAN->sTxMailBox[TransmitMailbox].TDTR |= TxMessage->DLC;

    /* Set up the data field [��������ʧ��]*/
    CAN->sTxMailBox[TransmitMailbox].TDLR = (((u32)TxMessage->Data[3] << 24) | 
                                             ((u32)TxMessage->Data[2] << 16) |
                                             ((u32)TxMessage->Data[1] << 8) | 
                                             ((u32)TxMessage->Data[0]));
    CAN->sTxMailBox[TransmitMailbox].TDHR = (((u32)TxMessage->Data[7] << 24) | 
                                             ((u32)TxMessage->Data[6] << 16) |
                                             ((u32)TxMessage->Data[5] << 8) |
                                             ((u32)TxMessage->Data[4]));

    /* Request transmission [������]*/
    CAN->sTxMailBox[TransmitMailbox].TIR |= CAN_TMIDxR_TXRQ;
  }

  return TransmitMailbox;
}

/*******************************************************************************
* ��������: CAN_TransmitStatus
* ��������: ��鱨�ķ��͵�״̬.
* �������: TransmitMailbox: ���ڷ��͵��������.
* �������: ��.
* ���ز���: CANTXOK:CAN�������ڷ��ͱ���;CANTXPENDING:���ı�����;CANTXFAILED:����.
*******************************************************************************/
u8 CAN_TransmitStatus(u8 TransmitMailbox)
{
  /* RQCP, TXOK and TME bits */
  u8 State = 0;

  /* Check the parameters [������]*/
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
      /* transmit pending   [���͹���]*/
    case (0x0): State = CANTXPENDING;
      break;
      /* transmit failed  [����ʧ��]*/
    case (0x5): State = CANTXFAILED;
      break;
      /* transmit succedeed  [���ͳɹ�]*/
    case (0x7): State = CANTXOK;
      break;
    default:
      State = CANTXFAILED;
      break;
  }

  return State;
}

/*******************************************************************************
* ��������: CAN_CancelTransmit
* ��������: ȡ��һ����������.
* �������: Mailbox number:�����.
* �������: ��.
* ���ز���: ��.
*******************************************************************************/
void CAN_CancelTransmit(u8 Mailbox)
{
  /* Check the parameters [������]*/
  assert_param(IS_CAN_TRANSMITMAILBOX(Mailbox));

  /* abort transmission [��������]*/
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
* ��������: CAN_FIFORelease
* ��������: �ͷ�һ��FIFO.
* �������: FIFO����: ��Ҫ�ͷŵ�FIFO��CANFIFO0��CANFIFO.
* �������: ��.
* ���ز���: ��.
*******************************************************************************/
void CAN_FIFORelease(u8 FIFONumber)
{
  /* Check the parameters [������]*/
  assert_param(IS_CAN_FIFO(FIFONumber));

  /* Release FIFO0 [�ͷ�FIFO0]*/
  if (FIFONumber == CAN_FIFO0)
  {
    CAN->RF0R = CAN_RF0R_RFOM0;
  }
  /* Release FIFO1 [�ͷ�FIFO1]*/
  else /* FIFONumber == CAN_FIFO1 */
  {
    CAN->RF1R = CAN_RF1R_RFOM1;
  }
}

/*******************************************************************************
* ��������: CAN_MessagePending
* ��������: ���ع����ĵ�����.
* �������: FIFO���룺����FIFO��CANFIFO0��CANFIFO1.
* �������: ��.
* ���ز���: NbMessage�������ĵ�������
*******************************************************************************/
u8 CAN_MessagePending(u8 FIFONumber)
{
  u8 MessagePending=0;

  /* Check the parameters [������]*/
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
* ��������: CAN_Receive
* ��������: ����һ������.
* �������: FIFO number: ����FIFO�ı�ţ�CANFIFO0��CANFIFO1.
* �������: RxMessage: ָ��һ������CAN ID,CAN DLC��CAN���ݵĽṹ�塣
* ���ز���: ��.
*******************************************************************************/
void CAN_Receive(u8 FIFONumber, CanRxMsg* RxMessage)
{
  /* Check the parameters [������]*/
  assert_param(IS_CAN_FIFO(FIFONumber));

  /* Get the Id [ȡ��Id]*/
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

  /* Get the DLC [ȡ��DLC]*/
  RxMessage->DLC = (u8)0x0F & CAN->sFIFOMailBox[FIFONumber].RDTR;

  /* Get the FMI [ȡ��FMI]*/
  RxMessage->FMI = (u8)0xFF & (CAN->sFIFOMailBox[FIFONumber].RDTR >> 8);

  /* Get the data field [ȡ�������ֶ�]*/
  RxMessage->Data[0] = (u8)0xFF & CAN->sFIFOMailBox[FIFONumber].RDLR;
  RxMessage->Data[1] = (u8)0xFF & (CAN->sFIFOMailBox[FIFONumber].RDLR >> 8);
  RxMessage->Data[2] = (u8)0xFF & (CAN->sFIFOMailBox[FIFONumber].RDLR >> 16);
  RxMessage->Data[3] = (u8)0xFF & (CAN->sFIFOMailBox[FIFONumber].RDLR >> 24);

  RxMessage->Data[4] = (u8)0xFF & CAN->sFIFOMailBox[FIFONumber].RDHR;
  RxMessage->Data[5] = (u8)0xFF & (CAN->sFIFOMailBox[FIFONumber].RDHR >> 8);
  RxMessage->Data[6] = (u8)0xFF & (CAN->sFIFOMailBox[FIFONumber].RDHR >> 16);
  RxMessage->Data[7] = (u8)0xFF & (CAN->sFIFOMailBox[FIFONumber].RDHR >> 24);

  /* Release the FIFO [�ͷ�FIFO]*/
  CAN_FIFORelease(FIFONumber);
}

/*******************************************************************************
* ��������: CAN_Sleep
* ��������: ��CAN���ڵ͹���ģʽ.
* �������: ��.
* �������: ��.
* ���ز���: CANSLEEPOK���������˯�ߣ�CANSLEEPFAILED�����������.
*******************************************************************************/
u8 CAN_Sleep(void)
{
  u8 SleepStatus = 0;

  /* Sleep mode entering request [����˯��ģʽ����]*/
  CAN->MCR |= CAN_MCR_SLEEP;
  SleepStatus = CANSLEEPOK;

  /* Sleep mode status [˯��ģʽ״̬]*/
  if ((CAN->MCR&CAN_MCR_SLEEP) == 0)
  {
    /* Sleep mode not entered [����ȥ˯��ģʽ]*/
    SleepStatus = CANSLEEPFAILED;
  }

  /* At this step, sleep mode status [���ⲽ��˯��ģʽ״̬]*/
  return SleepStatus;
}

/*******************************************************************************
* ��������: CAN_WakeUp
* ��������: ����CAN.
* �������: ��.
* �������: ��.
* ���ز���: CANWAKEUPOK������˳���˯��ģʽ��CANWAKEUPFAILED�����������.
*******************************************************************************/
u8 CAN_WakeUp(void)
{
  u8 WakeUpStatus = 0;

  /* Wake up request [��������]*/
  CAN->MCR &= ~CAN_MCR_SLEEP;
  WakeUpStatus = CANWAKEUPFAILED;

  /* Sleep mode status [˯��ģʽ״̬]*/
  if ((CAN->MCR&CAN_MCR_SLEEP) == 0)
  {
    /* Sleep mode exited [�˳�˯��ģʽ]*/
    WakeUpStatus = CANWAKEUPOK;
  }

  /* At this step, sleep mode status [���ⲽ��˯��ģʽ״̬]*/
  return WakeUpStatus;
}

/*******************************************************************************
* ��������: CAN_GetFlagStatus
* ��������: ���ָ����CAN��־�Ƿ���λ.
* �������: CAN_FLAG: ָ��Ҫ���ı�־.
*                  �������������: CAN_FLAG_EWG, CAN_FLAG_EPV��CAN_FLAG_BOF.
* �������: ��.
* ���ز���: CAN_FLAG����״̬��SET��RESET��.
*******************************************************************************/
FlagStatus CAN_GetFlagStatus(u32 CAN_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [������]*/
  assert_param(IS_CAN_FLAG(CAN_FLAG));

  /* Check the status of the specified CAN flag [���ָ����CAN��־״̬]*/
  if ((CAN->ESR & CAN_FLAG) != (u32)RESET)
  {
    /* CAN_FLAG is set [��λCAN_FLAG]*/
    bitstatus = SET;
  }
  else
  {
    /* CAN_FLAG is reset [��λCAN_FLAG]*/
    bitstatus = RESET;
  }
  /* Return the CAN_FLAG status [����CAN_FLAG״̬]*/
  return  bitstatus;
}

/*******************************************************************************
* ��������: CAN_ClearFlag
* ��������: ���CAN�����־.
* �������: CAN_FLAG: ָ����Ҫ����ı�־��
* �������: ��.
* ���ز���: ��.
*******************************************************************************/
void CAN_ClearFlag(u32 CAN_FLAG)
{
  /* Check the parameters [������]*/
  assert_param(IS_CAN_FLAG(CAN_FLAG));

  /* Clear the selected CAN flags [���ѡ���CAN��־]*/
  CAN->ESR &= ~CAN_FLAG;
}

/*******************************************************************************
* ��������: CAN_GetITStatus
* ��������: ���ָ����CAN�ж��Ƿ���.
* �������: CAN_IT: CAN��Ҫ�����ж�Դ��
*                  �������������: CAN_IT_RQCP0, CAN_IT_RQCP1, CAN_IT_RQCP2,
*                                         CAN_IT_FF0, CAN_IT_FOV0, CAN_IT_FF1,
*                                         CAN_IT_FOV1, CAN_IT_EWG, CAN_IT_EPV, 
*                                         CAN_IT_BOF, CAN_IT_WKU or CAN_IT_SLK.
* �������: ��.
* ���ز���: CAN_IT����״̬��SET��RESET��.
*******************************************************************************/
ITStatus CAN_GetITStatus(u32 CAN_IT)
{
  ITStatus pendingbitstatus = RESET;

  /* Check the parameters [������]*/
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

  /* Return the CAN_IT status [����CAN_IT״̬]*/
  return  pendingbitstatus;
}

/*******************************************************************************
* ��������: CAN_ClearITPendingBit
* ��������: ���CAN�жϹ���λ.
* �������: CAN_IT: ָ����Ҫ������жϹ���λ��
* �������: ��.
* ���ز���: ��.
*******************************************************************************/
void CAN_ClearITPendingBit(u32 CAN_IT)
{
  /* Check the parameters [������]*/
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
* ��������: CheckITStatus
* ��������: ���CAN�ж��Ƿ���.
* �������: (1)CAN_Reg: Ҫ����ָ����CAN�жϼĴ���.
*           (2)It_Bit: Ҫ����ָ�����ж�Դλ.
* �������: ��.
* ���ز���: CAN�жϵ���״̬ (SET or RESET).
*******************************************************************************/
static ITStatus CheckITStatus(u32 CAN_Reg, u32 It_Bit)
{
  ITStatus pendingbitstatus = RESET;

  if ((CAN_Reg & It_Bit) != (u32)RESET)
  {
    /* CAN_IT is set [��λCAN_IT]*/
    pendingbitstatus = SET;
  }
  else
  {
    /* CAN_IT is reset [��λCAN_IT]*/
    pendingbitstatus = RESET;
  }

  return pendingbitstatus;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
