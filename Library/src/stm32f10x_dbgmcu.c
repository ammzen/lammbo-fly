/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_dbgmcu.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������DBGMCU�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_dbgmcu.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
#define IDCODE_DEVID_Mask    ((u32)0x00000FFF)

/* ���ú� -----------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: DBGMCU_GetREVID
* ��������: Returns the device revision identifier[�����豸��У����ʾ��].
* �������: ��
* �������: ��
* ���ز���: Device revision identifier[�豸��У����ʾ��]
*******************************************************************************/
u32 DBGMCU_GetREVID(void)
{
   return(DBGMCU->IDCODE >> 16);
}

/*******************************************************************************
* ��������: DBGMCU_GetDEVID
* ��������: Returns the device identifier[�����豸�ı�ʾ��].
* �������: ��
* �������: ��
* ���ز���: Device identifier[�豸�ı�ʾ��]
*******************************************************************************/
u32 DBGMCU_GetDEVID(void)
{
   return(DBGMCU->IDCODE & IDCODE_DEVID_Mask);
}

/*******************************************************************************
* ��������: DBGMCU_Config
* ��������: Configures the specified peripheral and low power mode behavior
*                  when the MCU under Debug mode[����MCU�ڵ���ģʽ��ָ������͵͹���ģʽ״̬].
* �������: (1) DBGMCU_Periph: specifies the peripheral and low power mode[ָ��������͵͹���ģʽ].
*                    This parameter can be any combination of the following values:[����ʹ����������ֵ�����]
*                       - DBGMCU_SLEEP: Keep debugger connection during SLEEP mode [��˯��ģʽ���ֵ�����������]             
*                       - DBGMCU_STOP: Keep debugger connection during STOP mode[����ͣģʽ���ֵ�����������]               
*                       - DBGMCU_STANDBY: Keep debugger connection during STANDBY mode[��ֹͣģʽ���ֵ�����������]             
*                       - DBGMCU_IWDG_STOP: Debug IWDG stopped when Core is halted[���ں�ֹͣʱֹͣ����IWDG]          
*                       - DBGMCU_WWDG_STOP: Debug WWDG stopped when Core is halted[���ں�ֹͣʱֹͣ����WWDG]            
*                       - DBGMCU_TIM1_STOP: TIM1 counter stopped when Core is halted[���ں�ֹͣʱֹͣTIM1������]           
*                       - DBGMCU_TIM2_STOP: TIM2 counter stopped when Core is halted[���ں�ֹͣʱֹͣTIM2������]          
*                       - DBGMCU_TIM3_STOP: TIM3 counter stopped when Core is halted[���ں�ֹͣʱֹͣTIM3������]          
*                       - DBGMCU_TIM4_STOP: TIM4 counter stopped when Core is halted[���ں�ֹͣʱֹͣTIM4������]          
*                       - DBGMCU_CAN_STOP: Debug CAN stopped when Core is halted[���ں�ֹͣʱֹͣ����CAN]           
*                       - DBGMCU_I2C1_SMBUS_TIMEOUT: I2C1 SMBUS timeout mode stopped
*                                                    when Core is halted[���ں�ֹͣʱֹͣI2C1���߳�ʱģʽ] 
*                       - DBGMCU_I2C2_SMBUS_TIMEOUT: I2C2 SMBUS timeout mode stopped
*                                                    when Core is halted[���ں�ֹͣʱֹͣI2C2���߳�ʱģʽ]
*                       - DBGMCU_TIM5_STOP: TIM5 counter stopped when Core is halted[���ں�ֹͣʱֹͣTIM5������]          
*                       - DBGMCU_TIM6_STOP: TIM6 counter stopped when Core is halted[���ں�ֹͣʱֹͣTIM6������]          
*                       - DBGMCU_TIM7_STOP: TIM7 counter stopped when Core is halted[���ں�ֹͣʱֹͣTIM7������]          
*                       - DBGMCU_TIM8_STOP: TIM8 counter stopped when Core is halted[���ں�ֹͣʱֹͣTIM8������]          
*           (2) NewState: new state of the specified peripheral in Debug mode.[ָ���������ڵ͹���ģʽ����״̬]
*                    This parameter can be: ENABLE or DISABLE.[������������ǣ�ENABLE or DISABLE]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void DBGMCU_Config(u32 DBGMCU_Periph, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_DBGMCU_PERIPH(DBGMCU_Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    DBGMCU->CR |= DBGMCU_Periph;
  }
  else
  {
    DBGMCU->CR &= ~DBGMCU_Periph;
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
