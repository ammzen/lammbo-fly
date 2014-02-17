/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_pwr.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: This file provides all the PWR firmware functions.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
/* --------- PWR registers bit address in the alias region ---------- */
#define PWR_OFFSET               (PWR_BASE - PERIPH_BASE)

/* --- CR Register ---*/
/* Alias word address of DBP bit */
#define CR_OFFSET                (PWR_OFFSET + 0x00)
#define DBP_BitNumber            0x08
#define CR_DBP_BB                (PERIPH_BB_BASE + (CR_OFFSET * 32) + (DBP_BitNumber * 4))

/* Alias word address of PVDE bit */
#define PVDE_BitNumber           0x04
#define CR_PVDE_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PVDE_BitNumber * 4))

/* --- CSR Register ---*/
/* Alias word address of EWUP bit */
#define CSR_OFFSET               (PWR_OFFSET + 0x04)
#define EWUP_BitNumber           0x08
#define CSR_EWUP_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (EWUP_BitNumber * 4))

/* ------------------ PWR registers bit mask ------------------------ */
/* CR register bit mask */
#define CR_PDDS_Set              ((u32)0x00000002)
#define CR_DS_Mask               ((u32)0xFFFFFFFC)
#define CR_CWUF_Set              ((u32)0x00000004)
#define CR_PLS_Mask              ((u32)0xFFFFFF1F)

/* --------- Cortex System Control register bit mask ---------------- */
/* Cortex System Control register address */
#define SCB_SysCtrl              ((u32)0xE000ED10)
/* SLEEPDEEP bit mask */
#define SysCtrl_SLEEPDEEP_Set    ((u32)0x00000004)

/* ���ú� --------------------------------------------------------------------*/
/* ���ñ��� ------------------------------------------------------------------*/
/* ���ú���ԭ�� --------------------------------------------------------------*/
/* ���ú��� ------------------------------------------------------------------*/

/*******************************************************************************
* ��������: PWR_DeInit
* ��������: ��λPWR��Χ�Ĵ���ΪĬ�ϸ�λֵ.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void PWR_DeInit(void)
{
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, DISABLE);
}

/*******************************************************************************
* ��������: PWR_BackupAccessCmd
* ��������: ʹ�ܻ�رն�RTC�ͱ��ݼĴ����ķ��ʡ�
* �������: NewState:����RTC�ͱ��ݼĴ�������״̬��������������ǣ�ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void PWR_BackupAccessCmd(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CR_DBP_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: PWR_PVDCmd
* ��������: ʹ�ܻ�رյ�Դ��ѹ̽������PVD����
* �������: NewState:PVD����״̬��������������ǣ�ENABLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void PWR_PVDCmd(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CR_PVDE_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: PWR_PVDLevelConfig
* ��������: �����ɵ�Դ��ѹ̽����̽��ĵ�ѹ����ֵ��PVD��.
* �������: PWR_PVDLevel:PVD̽���ƽ��
*                    �����������ȡ�����ֵ֮һ:
*                       - PWR_PVDLevel_2V2: PVD̽���ƽ����Ϊ 2.2V
*                       - PWR_PVDLevel_2V3: PVD̽���ƽ����Ϊ 2.3V
*                       - PWR_PVDLevel_2V4: PVD̽���ƽ����Ϊ 2.4V
*                       - PWR_PVDLevel_2V5: PVD̽���ƽ����Ϊ 2.5V
*                       - PWR_PVDLevel_2V6: PVD̽���ƽ����Ϊ 2.6V
*                       - PWR_PVDLevel_2V7: PVD̽���ƽ����Ϊ 2.7V
*                       - PWR_PVDLevel_2V8: PVD̽���ƽ����Ϊ 2.8V
*                       - PWR_PVDLevel_2V9: PVD̽���ƽ����Ϊ 2.9V
* �������: ��
* ���ز���: ��
*******************************************************************************/
void PWR_PVDLevelConfig(u32 PWR_PVDLevel)
{
  u32 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_PWR_PVD_LEVEL(PWR_PVDLevel));

  tmpreg = PWR->CR;

  /* Clear PLS[7:5] bits [����PLS[7:5]λ]*/
  tmpreg &= CR_PLS_Mask;

  /* Set PLS[7:5] bits according to PWR_PVDLevel value [����PWR_PVDLevel��ֵ��λPLS[7:5]]*/
  tmpreg |= PWR_PVDLevel;

  /* Store the new value [�����µ�ֵ]*/
  PWR->CR = tmpreg;
}

/*******************************************************************************
* ��������: PWR_WakeUpPinCmd
* ��������: ʹ�ܻ������ŵĹ���
* �������: NewState:�������Ź��ܵ���״̬��������������ǣ�ENALBLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void PWR_WakeUpPinCmd(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CSR_EWUP_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: PWR_EnterSTOPMode
* ��������: ����STOPģʽ.
* �������: PWR_Regulator:STOPģʽ��У׼״̬��
*                    ������������������ֵ֮һ:
*                       - PWR_Regulator_ON: У׼��������STOPģʽ
*                       - PWR_Regulator_LowPower: �ڵ͹���ģʽ�´���У׼����STOPģʽ��
*                  - PWR_STOPEntry: �������������STOP����ģʽ.
*                    ������������������ֵ֮һ:
*                       - PWR_STOPEntry_WFI: ������WFIָ���STOPģʽ
*                       - PWR_STOPEntry_WFE: ������WFEָ���STOPģʽ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void PWR_EnterSTOPMode(u32 PWR_Regulator, u8 PWR_STOPEntry)
{
  u32 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_PWR_REGULATOR(PWR_Regulator));
  assert_param(IS_PWR_STOP_ENTRY(PWR_STOPEntry));
  
  /* Select the regulator state in STOP mode [��ֹͣģʽѡ��У׼״̬]*/
  tmpreg = PWR->CR;

  /* Clear PDDS and LPDS bits [��PDDS��LPDSλ]*/
  tmpreg &= CR_DS_Mask;

  /* Set LPDS bit according to PWR_Regulator value [����PWR_Regulatorֵ��λLPDS]*/
  tmpreg |= PWR_Regulator;

  /* Store the new value [�����µ�ֵ]*/
  PWR->CR = tmpreg;

  /* Set SLEEPDEEP bit of Cortex System Control Register [��λCortexϵͳ�Ŀ��ƼĴ���SLEEPDEEPλ]*/
  *(vu32 *) SCB_SysCtrl |= SysCtrl_SLEEPDEEP_Set;
  
  /* Select STOP mode entry [ѡ��ֹͣģʽ����]*/
  if(PWR_STOPEntry == PWR_STOPEntry_WFI)
  {   
    /* Request Wait For Interrupt [����ȴ��ж�]*/
    __WFI();
  }
  else
  {
    /* Request Wait For Event [����ȴ��¼�]*/
    __WFE();
  }
}

/*******************************************************************************
* ��������: PWR_EnterSTANDBYMode
* ��������: ����STANDBYģʽ.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void PWR_EnterSTANDBYMode(void)
{
  /* Clear Wake-up flag [���Wake-up��־]*/
  PWR->CR |= CR_CWUF_Set;

  /* Select STANDBY mode [ѡ��STANDBYģʽ]*/
  PWR->CR |= CR_PDDS_Set;

  /* Set SLEEPDEEP bit of Cortex System Control Register [��λCortexϵͳ�Ŀ��ƼĴ���SLEEPDEEPλ]*/
  *(vu32 *) SCB_SysCtrl |= SysCtrl_SLEEPDEEP_Set;

  /* Request Wait For Interrupt [����ȴ��ж�]*/
  __WFI();
}

/*******************************************************************************
* ��������: PWR_GetFlagStatus
* ��������: ���ָ����PWR��־λ�������.
* �������: PWR_FLAG:Ҫ���ı�־��
*                    ������������������ֵ֮һ:
*                       - PWR_FLAG_WU: Wake Up ��־
*                       - PWR_FLAG_SB: StandBy ��־
*                       - PWR_FLAG_PVDO: PVD ���
* �������: ��
* ���ز���: PWR_FLAG����״̬��SET��RESET��.
*******************************************************************************/
FlagStatus PWR_GetFlagStatus(u32 PWR_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [������]*/
  assert_param(IS_PWR_GET_FLAG(PWR_FLAG));
  
  if ((PWR->CSR & PWR_FLAG) != (u32)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* Return the flag status [����״̬��־]*/
  return bitstatus;
}

/*******************************************************************************
* ��������: PWR_ClearFlag
* ��������: ���PWR�����־λ��
* �������: PWR_FLAG:Ҫ��յı�־��
*                    ������������������ֵ֮һ:
*                       - PWR_FLAG_WU: Wake Up ��־
*                       - PWR_FLAG_SB: StandBy ��־
* �������: ��
* ���ز���: ��
*******************************************************************************/
void PWR_ClearFlag(u32 PWR_FLAG)
{
  /* Check the parameters [������]*/
  assert_param(IS_PWR_CLEAR_FLAG(PWR_FLAG));
         
  PWR->CR |=  PWR_FLAG << 2;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
