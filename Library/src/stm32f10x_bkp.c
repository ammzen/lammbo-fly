/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_bkp.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������BKP�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_bkp.h"
#include "stm32f10x_rcc.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
/* ------------ BKP registers bit address in the alias region ----------- */
#define BKP_OFFSET        (BKP_BASE - PERIPH_BASE)

/* --- CR Register ---*/
/* Alias word address of TPAL bit */
#define CR_OFFSET         (BKP_OFFSET + 0x30)
#define TPAL_BitNumber    0x01
#define CR_TPAL_BB        (PERIPH_BB_BASE + (CR_OFFSET * 32) + (TPAL_BitNumber * 4))

/* Alias word address of TPE bit */
#define TPE_BitNumber     0x00
#define CR_TPE_BB         (PERIPH_BB_BASE + (CR_OFFSET * 32) + (TPE_BitNumber * 4))

/* --- CSR Register ---*/
/* Alias word address of TPIE bit */
#define CSR_OFFSET        (BKP_OFFSET + 0x34)
#define TPIE_BitNumber    0x02
#define CSR_TPIE_BB       (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TPIE_BitNumber * 4))

/* Alias word address of TIF bit */
#define TIF_BitNumber     0x09
#define CSR_TIF_BB        (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TIF_BitNumber * 4))

/* Alias word address of TEF bit */
#define TEF_BitNumber     0x08
#define CSR_TEF_BB        (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TEF_BitNumber * 4))


/* ---------------------- BKP registers bit mask ------------------------ */
/* RTCCR register bit mask */
#define RTCCR_CAL_Mask    ((u16)0xFF80)
#define RTCCR_Mask        ((u16)0xFC7F)

/* CSR register bit mask */
#define CSR_CTE_Set       ((u16)0x0001)
#define CSR_CTI_Set       ((u16)0x0002)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: BKP_DeInit
* ��������: ��BKP����Ĵ�����λ�����ǵ�Ĭ��ֵ
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void BKP_DeInit(void)
{
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);
}

/*******************************************************************************
* ��������: BKP_TamperPinLevelConfig
* ��������: ���ôܸ����ŵ���Ч��ƽ.
* �������: BKP_TamperPinLevel: �ܸ����ŵ���Ч��ƽ.
*                    �����������ȡ�����е�һ��:
*                       - BKP_TamperPinLevel_High: �ܸ����Ÿ���Ч
*                       - BKP_TamperPinLevel_Low: �ܸ����ŵ���Ч
* �������: ��
* ���ز���: ��
*******************************************************************************/
void BKP_TamperPinLevelConfig(u16 BKP_TamperPinLevel)
{
  /* Check the parameters [������]*/
  assert_param(IS_BKP_TAMPER_PIN_LEVEL(BKP_TamperPinLevel));

  *(vu32 *) CR_TPAL_BB = BKP_TamperPinLevel;
}

/*******************************************************************************
* ��������: BKP_TamperPinCmd
* ��������: ʹ��/��ֹ�ܸ����ż���.
* �������: NewState: �ܸ����ż������״̬�ò���ȡֵ��ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void BKP_TamperPinCmd(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CR_TPE_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: BKP_ITConfig
* ��������: ʹ��/��ֹ�ܸ������ж�.
* �������: NewState: �ܸ������жϵ���״̬�ò���ȡֵ��ENABLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void BKP_ITConfig(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CSR_TPIE_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: BKP_RTCOutputConfig
* ��������: ʹ��/��ֹУ׼ʱ�ӵ����
* �������: NewState: У׼ʱ���������״̬���ò���ȡֵ��ENABLE��DISABLE.
*                    �����������ȡ�����ֵ֮һ:
*                       - BKP_RTCOutputSource_None: �ܸĵ�������û��ʵʱʱ�����.
*                       - BKP_RTCOutputSource_CalibClock: �ܸĵ������������ʵʱʱ�ӵ���Ƶ�ʳ���64��
*                       - BKP_RTCOutputSource_Alarm: �ܸĵ����������ʵʱʱ�Ӹ澯����.
*                       - BKP_RTCOutputSource_Second: �ܸĵ����������ʵʱʱ��������.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void BKP_RTCOutputConfig(u16 BKP_RTCOutputSource)
{
  u16 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_BKP_RTC_OUTPUT_SOURCE(BKP_RTCOutputSource));

  tmpreg = BKP->RTCCR;

  /* Clear CCO, ASOE and ASOS bits [���CCO, ASOE �� ASOSλ]*/
  tmpreg &= RTCCR_Mask;
  
  /* Set CCO, ASOE and ASOS bits according to BKP_RTCOutputSource value [���ձ���ʵʱʱ��Դ��ֵ��λCCO, ASOE �� ASOSλ]*/
  tmpreg |= BKP_RTCOutputSource;

  /* Store the new value [�����µ�ֵ]*/
  BKP->RTCCR = tmpreg;
}

/*******************************************************************************
* ��������: BKP_SetRTCCalibrationValue
* ��������: ����RTCʱ��У׼ֵ.
* �������: CalibrationValue: RTCʱ��У׼ֵ���ò���ȡֵ��0��0x7F.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void BKP_SetRTCCalibrationValue(u8 CalibrationValue)
{
  u16 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_BKP_CALIBRATION_VALUE(CalibrationValue));

  tmpreg = BKP->RTCCR;

  /* Clear CAL[6:0] bits [���CAL[6:0]λ]*/
  tmpreg &= RTCCR_CAL_Mask;

  /* Set CAL[6:0] bits according to CalibrationValue value [����У׼ֵ��λCAL[6:0]]*/
  tmpreg |= CalibrationValue;

  /* Store the new value */
  BKP->RTCCR = tmpreg;
}

/*******************************************************************************
* ��������: BKP_WriteBackupRegister
* ��������: ���û�����д��ָ�������ݱ��ݼĴ���.
* �������: (1)BKP_DR: ���ݱ��ݼĴ���.�������������BKP_DRx��xȡֵ��1��42��
*           (2)Data: ��Ҫд�������
* �������: ��
* ���ز���: ��
*******************************************************************************/
void BKP_WriteBackupRegister(u16 BKP_DR, u16 Data)
{
  /* Check the parameters [������]*/
  assert_param(IS_BKP_DR(BKP_DR));

  *(vu16 *) (BKP_BASE + BKP_DR) = Data;
}

/*******************************************************************************
* ��������: BKP_ReadBackupRegister
* ��������: ��ָ�������ݱ��ݼĴ����ж�������.
* �������: BKP_DR: ���ݱ��ݼĴ���.�������������BKP_DRx��xȡֵ��1��42��
* �������: ��
* ���ز���: ָ�������ݱ��ݼĴ���������
*******************************************************************************/
u16 BKP_ReadBackupRegister(u16 BKP_DR)
{
  /* Check the parameters [������]*/
  assert_param(IS_BKP_DR(BKP_DR));

  return (*(vu16 *) (BKP_BASE + BKP_DR));
}

/*******************************************************************************
* ��������: BKP_GetFlagStatus
* ��������: ���ܸ������¼���־�Ƿ���λ.
* �������: ��
* �������: ��
* ���ز���: �ܸ������¼���־����״̬��SET��RESET��.
*******************************************************************************/
FlagStatus BKP_GetFlagStatus(void)
{
  return (FlagStatus)(*(vu32 *) CSR_TEF_BB);
}

/*******************************************************************************
* ��������: BKP_ClearFlag
* ��������: ����ܸ������¼������־.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void BKP_ClearFlag(void)
{
  /* Set CTE bit to clear Tamper Pin Event flag [��λCTEλ����ܸ������¼���־]*/
  BKP->CSR |= CSR_CTE_Set;
}

/*******************************************************************************
* ��������: BKP_GetITStatus
* ��������: ���ܸ������ж��Ƿ���.
* �������: ��
* �������: ��
* ���ز���: �ܸ������жϵ���״̬��SET��RESET��.
*******************************************************************************/
ITStatus BKP_GetITStatus(void)
{
  return (ITStatus)(*(vu32 *) CSR_TIF_BB);
}

/*******************************************************************************
* ��������: BKP_ClearITPendingBit
* ��������: ����ܸ������жϹ���λ.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void BKP_ClearITPendingBit(void)
{
  /* Set CTI bit to clear Tamper Pin Interrupt pending bit [����ܸ������жϹ���λ]*/
  BKP->CSR |= CSR_CTI_Set;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
