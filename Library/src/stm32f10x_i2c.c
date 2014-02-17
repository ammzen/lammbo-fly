/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_i2c.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������I2C�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_i2c.h"
#include "stm32f10x_rcc.h"
     
/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
/* I2C SPE mask */
#define CR1_PE_Set              ((u16)0x0001)
#define CR1_PE_Reset            ((u16)0xFFFE)

/* I2C START mask */
#define CR1_START_Set           ((u16)0x0100)
#define CR1_START_Reset         ((u16)0xFEFF)

/* I2C STOP mask */
#define CR1_STOP_Set            ((u16)0x0200)
#define CR1_STOP_Reset          ((u16)0xFDFF)

/* I2C ACK mask */
#define CR1_ACK_Set             ((u16)0x0400)
#define CR1_ACK_Reset           ((u16)0xFBFF)

/* I2C ENGC mask */
#define CR1_ENGC_Set            ((u16)0x0040)
#define CR1_ENGC_Reset          ((u16)0xFFBF)

/* I2C SWRST mask */
#define CR1_SWRST_Set           ((u16)0x8000)
#define CR1_SWRST_Reset         ((u16)0x7FFF)

/* I2C PEC mask */
#define CR1_PEC_Set             ((u16)0x1000)
#define CR1_PEC_Reset           ((u16)0xEFFF)

/* I2C ENPEC mask */
#define CR1_ENPEC_Set           ((u16)0x0020)
#define CR1_ENPEC_Reset         ((u16)0xFFDF)

/* I2C ENARP mask */
#define CR1_ENARP_Set           ((u16)0x0010)
#define CR1_ENARP_Reset         ((u16)0xFFEF)

/* I2C NOSTRETCH mask */
#define CR1_NOSTRETCH_Set       ((u16)0x0080)
#define CR1_NOSTRETCH_Reset     ((u16)0xFF7F)

/* I2C registers Masks */
#define CR1_CLEAR_Mask          ((u16)0xFBF5)

/* I2C DMAEN mask */
#define CR2_DMAEN_Set           ((u16)0x0800)
#define CR2_DMAEN_Reset         ((u16)0xF7FF)

/* I2C LAST mask */
#define CR2_LAST_Set            ((u16)0x1000)
#define CR2_LAST_Reset          ((u16)0xEFFF)

/* I2C FREQ mask */
#define CR2_FREQ_Reset          ((u16)0xFFC0)

/* I2C ADD0 mask */
#define OAR1_ADD0_Set           ((u16)0x0001)
#define OAR1_ADD0_Reset         ((u16)0xFFFE)

/* I2C ENDUAL mask */
#define OAR2_ENDUAL_Set         ((u16)0x0001)
#define OAR2_ENDUAL_Reset       ((u16)0xFFFE)

/* I2C ADD2 mask */
#define OAR2_ADD2_Reset         ((u16)0xFF01)

/* I2C F/S mask */
#define CCR_FS_Set              ((u16)0x8000)

/* I2C CCR mask */
#define CCR_CCR_Set             ((u16)0x0FFF)

/* I2C FLAG mask */
#define FLAG_Mask               ((u32)0x00FFFFFF)

/* I2C Interrupt Enable mask */
#define ITEN_Mask               ((u32)0x07000000)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: I2C_DeInit
* ��������: ��I2Cx����Ĵ�����λ�����ǵ�Ĭ��ֵ.
* �������: I2Cx��x��Ϊ1��2��ѡ��I2C����.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_DeInit(I2C_TypeDef* I2Cx)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  switch (*(u32*)&I2Cx)
  {
    case I2C1_BASE:
      /* Enable I2C1 reset state [����I2C1��λ״̬]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
      /* Release I2C1 from reset state [���I2C1��λ״̬]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
      break;

    case I2C2_BASE:
      /* Enable I2C2 reset state [����I2C2��λ״̬]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
      /* Release I2C2 from reset state [���I2C2��λ״̬]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
      break;

    default:
      break;
  }
}

/*******************************************************************************
* ��������: I2C_Init
* ��������: ����I2C_InitStruct���ض�������ʼ��I2C����.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)I2C_InitStruct��ָ��I2C_InitTypeDef�ṹ���ýṹ�������ض�I2C������������Ϣ��
* �������: ��
* ���ز���: ��
******************************************************************************/
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct)
{
  u16 tmpreg = 0, freqrange = 0;
  u16 result = 0x04;
  u32 pclk1 = 8000000;
  RCC_ClocksTypeDef  rcc_clocks;

  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_MODE(I2C_InitStruct->I2C_Mode));
  assert_param(IS_I2C_DUTY_CYCLE(I2C_InitStruct->I2C_DutyCycle));
  assert_param(IS_I2C_OWN_ADDRESS1(I2C_InitStruct->I2C_OwnAddress1));
  assert_param(IS_I2C_ACK_STATE(I2C_InitStruct->I2C_Ack));
  assert_param(IS_I2C_ACKNOWLEDGE_ADDRESS(I2C_InitStruct->I2C_AcknowledgedAddress));
  assert_param(IS_I2C_CLOCK_SPEED(I2C_InitStruct->I2C_ClockSpeed));

/*---------------------------- I2Cx CR2 Configuration ------------------------*/
  /* Get the I2Cx CR2 value */
  tmpreg = I2Cx->CR2;
  /* Clear frequency FREQ[5:0] bits */
  tmpreg &= CR2_FREQ_Reset;
  /* Get pclk1 frequency value */
  RCC_GetClocksFreq(&rcc_clocks);
  pclk1 = rcc_clocks.PCLK1_Frequency;
  /* Set frequency bits depending on pclk1 value */
  freqrange = (u16)(pclk1 / 1000000);
  tmpreg |= freqrange;
  /* Write to I2Cx CR2 */
  I2Cx->CR2 = tmpreg;

/*---------------------------- I2Cx CCR Configuration ------------------------*/
  /* Disable the selected I2C peripheral to configure TRISE */
  I2Cx->CR1 &= CR1_PE_Reset;

  /* Reset tmpreg value */
  /* Clear F/S, DUTY and CCR[11:0] bits */
  tmpreg = 0;

  /* Configure speed in standard mode */
  if (I2C_InitStruct->I2C_ClockSpeed <= 100000)
  {
    /* Standard mode speed calculate */
    result = (u16)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed << 1));
    /* Test if CCR value is under 0x4*/
    if (result < 0x04)
    {
      /* Set minimum allowed value */
      result = 0x04;  
    }
    /* Set speed value for standard mode */
    tmpreg |= result;     
    /* Set Maximum Rise Time for standard mode */
    I2Cx->TRISE = freqrange + 1; 
  }
  /* Configure speed in fast mode */
  else /*(I2C_InitStruct->I2C_ClockSpeed <= 400000)*/
  {
    if (I2C_InitStruct->I2C_DutyCycle == I2C_DutyCycle_2)
    {
      /* Fast mode speed calculate: Tlow/Thigh = 2 */
      result = (u16)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed * 3));
    }
    else /*I2C_InitStruct->I2C_DutyCycle == I2C_DutyCycle_16_9*/
    {
      /* Fast mode speed calculate: Tlow/Thigh = 16/9 */
      result = (u16)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed * 25));
      /* Set DUTY bit */
      result |= I2C_DutyCycle_16_9;
    }
    /* Test if CCR value is under 0x1*/
    if ((result & CCR_CCR_Set) == 0)
    {
      /* Set minimum allowed value */
      result |= (u16)0x0001;  
    }
    /* Set speed value and set F/S bit for fast mode */
    tmpreg |= result | CCR_FS_Set;
    /* Set Maximum Rise Time for fast mode */
    I2Cx->TRISE = (u16)(((freqrange * 300) / 1000) + 1);  
  }
  /* Write to I2Cx CCR */
  I2Cx->CCR = tmpreg;

  /* Enable the selected I2C peripheral */
  I2Cx->CR1 |= CR1_PE_Set;

/*---------------------------- I2Cx CR1 Configuration ------------------------*/
  /* Get the I2Cx CR1 value */
  tmpreg = I2Cx->CR1;
  /* Clear ACK, SMBTYPE and  SMBUS bits */
  tmpreg &= CR1_CLEAR_Mask;
  /* Configure I2Cx: mode and acknowledgement */
  /* Set SMBTYPE and SMBUS bits according to I2C_Mode value */
  /* Set ACK bit according to I2C_Ack value */
  tmpreg |= (u16)((u32)I2C_InitStruct->I2C_Mode | I2C_InitStruct->I2C_Ack);
  /* Write to I2Cx CR1 */
  I2Cx->CR1 = tmpreg;

/*---------------------------- I2Cx OAR1 Configuration -----------------------*/
  /* Set I2Cx Own Address1 and acknowledged address */
  I2Cx->OAR1 = (I2C_InitStruct->I2C_AcknowledgedAddress | I2C_InitStruct->I2C_OwnAddress1);
}

/*******************************************************************************
* ��������: I2C_StructInit
* ��������: ΪI2C_InitStruct�ĳ�Ա��Ĭ��ֵ.
* �������: I2C_InitStruct��ָ������ʼ����I2C_InitTypeDef�ṹ.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
{
/*---------------- Reset I2C init structure parameters values ----------------*/
  /* Initialize the I2C_Mode member */
  I2C_InitStruct->I2C_Mode = I2C_Mode_I2C;

  /* Initialize the I2C_DutyCycle member */
  I2C_InitStruct->I2C_DutyCycle = I2C_DutyCycle_2;

  /* Initialize the I2C_OwnAddress1 member */
  I2C_InitStruct->I2C_OwnAddress1 = 0;

  /* Initialize the I2C_Ack member */
  I2C_InitStruct->I2C_Ack = I2C_Ack_Disable;

  /* Initialize the I2C_AcknowledgedAddress member */
  I2C_InitStruct->I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

  /* initialize the I2C_ClockSpeed member */
  I2C_InitStruct->I2C_ClockSpeed = 5000;
}

/*******************************************************************************
* ��������: I2C_Cmd
* ��������: ʹ�ܣ�����ܣ��ض���I2C����.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)NewState��I2C��������״̬.�ò�����Ϊ��ENABLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C peripheral [ʹ��ѡ���I2C�豸]*/
    I2Cx->CR1 |= CR1_PE_Set;
  }
  else
  {
    /* Disable the selected I2C peripheral [��ֹѡ���I2C�豸]*/
    I2Cx->CR1 &= CR1_PE_Reset;
  }
}

/*******************************************************************************
* ��������: I2C_DMACmd
* ��������: ʹ�ܣ�����ܣ��ض���I2C��DMA����.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)NewState��I2C��DMA�������״̬.�ò�����Ϊ��ENABLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_DMACmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C DMA requests [ʹ��ѡ���I2C DMA����]*/
    I2Cx->CR2 |= CR2_DMAEN_Set;
  }
  else
  {
    /* Disable the selected I2C DMA requests [����ѡ���I2C DMA����]*/
    I2Cx->CR2 &= CR2_DMAEN_Reset;
  }
}

/*******************************************************************************
* ��������: I2C_DMALastTransferCmd
* ��������: ָ����һ��DMA���������һ��.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)NewState��I2C�����DMA�������״̬.�ò���Ϊ��ENABLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_DMALastTransferCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Next DMA transfer is the last transfer [��һ�����������һ������]*/
    I2Cx->CR2 |= CR2_LAST_Set;
  }
  else
  {
    /* Next DMA transfer is not the last transfer [��һ�����䲻�����һ������]*/
    I2Cx->CR2 &= CR2_LAST_Reset;
  }
}

/*******************************************************************************
* ��������: I2C_GenerateSTART
* ��������: ����I2Cͨ�ŵ���ʼ����.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)NewState��I2C��ʼ�������ɵ���״̬.�ò���Ϊ��ENABLE��DISABLE.
* �������: ��
* ���ز���: ��.
*******************************************************************************/
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Generate a START condition [����һ����ʼ����]*/
    I2Cx->CR1 |= CR1_START_Set;
  }
  else
  {
    /* Disable the START condition generation [��ֹһ����ʼ��������]*/
    I2Cx->CR1 &= CR1_START_Reset;
  }
}

/*******************************************************************************
* ��������: I2C_GenerateSTOP
* ��������: ����I2Cͨ�ŵ�ֹͣ����.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)NewState��I2Cֹͣ�������ɵ���״̬.�ò���Ϊ��ENABLE��DISABLE.
* �������: ��
* ���ز���: ��.
*******************************************************************************/
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Generate a STOP condition [����һ��ֹͣ����]*/
    I2Cx->CR1 |= CR1_STOP_Set;
  }
  else
  {
    /* Disable the STOP condition generation [��ֹһ��ֹͣ��������]*/
    I2Cx->CR1 &= CR1_STOP_Reset;
  }
}

/*******************************************************************************
* ��������: I2C_AcknowledgeConfig
* ��������: ʹ�ܣ�����ܣ��ض���I2C��ȷ������.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)NewState��I2Cȷ�����Ե���״̬���ò���Ϊ��ENABLE��DISABLE
* �������: ��
* ���ز���: ��.
*******************************************************************************/
void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the acknowledgement [����ȷ��]*/
    I2Cx->CR1 |= CR1_ACK_Set;
  }
  else
  {
    /* Disable the acknowledgement [��ֹȷ��]*/
    I2Cx->CR1 &= CR1_ACK_Reset;
  }
}

/*******************************************************************************
* ��������: I2C_OwnAddress2Config
* ��������: �����ض���I2C�Ե�ַ2.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)��ַ��ȷ��7λI2C�Ե�ַ2.
* �������: ��
* ���ز���: ��.
*******************************************************************************/
void I2C_OwnAddress2Config(I2C_TypeDef* I2Cx, u8 Address)
{
  u16 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Get the old register value [ȡ�þɵļĴ���ֵ]*/
  tmpreg = I2Cx->OAR2;
  /* Reset I2Cx Own address2 bit [7:1] [��λI2Cx�Լ���ַλ[7:1]]*/
  tmpreg &= OAR2_ADD2_Reset;
  /* Set I2Cx Own address2 [����I2Cx�Լ���ַ]*/
  tmpreg |= (u16)(Address & (u16)0x00FE);
  /* Store the new register value [�����µļĴ���ֵ]*/
  I2Cx->OAR2 = tmpreg;
}

/*******************************************************************************
* ��������: I2C_DualAddressCmd
* ��������: ʹ�ܣ�����ܣ��ض���I2C˫�ص�ַģʽ.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)NewState��I2C˫�ص�ַģʽ����״̬���ò���Ϊ��ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_DualAddressCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable dual addressing mode [����˫��ַģʽ]*/
    I2Cx->OAR2 |= OAR2_ENDUAL_Set;
  }
  else
  {
    /* Disable dual addressing mode [��ֹ˫��ַģʽ]*/
    I2Cx->OAR2 &= OAR2_ENDUAL_Reset;
  }
}

/*******************************************************************************
* ��������: I2C_GeneralCallCmd
* ��������: ʹ�ܣ�����ܣ��ض�I2C��ͨ�õ�������.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)NewState��I2Cͨ�õ��õ���״̬���ò���Ϊ��ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_GeneralCallCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable generall call [����ͨ�õ���]*/
    I2Cx->CR1 |= CR1_ENGC_Set;
  }
  else
  {
    /* Disable generall call [��ֹͨ�õ���]*/
    I2Cx->CR1 &= CR1_ENGC_Reset;
  }
}

/*******************************************************************************
* ��������: I2C_ITConfig
* ��������: ʹ�ܣ�����ܣ��ض���I2C�ж�.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)I2C_IT����Ҫʹ��(�����)��I2C�ж�Դ��. 
*                    ���������������������ֵ�����:
*                       - I2C_IT_BUF: �������ж�����
*                       - I2C_IT_EVT: �¼��ж�����
*                       - I2C_IT_ERR: �����ж�����
*           (3)NewState��ָ����I2C�жϵ���״̬���ò���Ϊ��ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_ITConfig(I2C_TypeDef* I2Cx, u16 I2C_IT, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_I2C_CONFIG_IT(I2C_IT));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected I2C interrupts [ʹ��ѡ���I2C�ж�]*/
    I2Cx->CR2 |= I2C_IT;
  }
  else
  {
    /* Disable the selected I2C interrupts [��ֹѡ���I2C�ж�]*/
    I2Cx->CR2 &= (u16)~I2C_IT;
  }
}

/*******************************************************************************
* ��������: I2C_SendData
* ��������: ͨ��I2C����һ�ֽ�����.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)Data����Ҫ���͵��ֽ�.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_SendData(I2C_TypeDef* I2Cx, u8 Data)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Write in the DR register the data to be sent [д���͵����ݵ�DR�Ĵ���]*/
  I2Cx->DR = Data;
}

/*******************************************************************************
* ��������: I2C_ReceiveData
* ��������: ����I2Cx����������յ�����.
* �������: I2Cx��x��Ϊ1��2��ѡ��I2C����.
* �������: ��
* ���ز���: ��������ֵ.
*******************************************************************************/
u8 I2C_ReceiveData(I2C_TypeDef* I2Cx)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Return the data in the DR register [����DR�Ĵ���������]*/
  return (u8)I2Cx->DR;
}

/*******************************************************************************
* ��������: I2C_Send7bitAddress
* ��������: �����ַ�ֽ���ѡ����豸.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)Address����Ҫ�����͵Ĵ��豸��ַ
*           (3)I2C_Direction:ȷ��I2C�豸Ϊ���豸����豸. 
*                    ������������������ֵ֮һ
*                       - I2C_Direction_Transmitter: ѡ����ģʽ
*                       - I2C_Direction_Receiver: ѡ�����ģʽ
* �������: ��
* ���ز���: ��.
*******************************************************************************/
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, u8 Address, u8 I2C_Direction)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_DIRECTION(I2C_Direction));

  /* Test on the direction to set/reset the read/write bit [���Զ�/дλ��״̬����λ���Ǹ�λ]*/
  if (I2C_Direction != I2C_Direction_Transmitter)
  {
    /* Set the address bit0 for read [��λ��ַλ��0λ����״̬]*/
    Address |= OAR1_ADD0_Set;
  }
  else
  {
    /* Reset the address bit0 for write [��λ��ַλ��0λ��д״̬]*/
    Address &= OAR1_ADD0_Reset;
  }
  /* Send the address [���͵�ַ]*/
  I2Cx->DR = Address;
}

/*******************************************************************************
* ��������: I2C_ReadRegister
* ��������: ��ȡ�ض���I2C�Ĵ�����������ֵ.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����
*           (2)I2C_Register����Ҫ��ȡ�ļĴ���.
*                    ������������������ֵ֮һ:
*                       - I2C_Register_CR1:  CR1 �Ĵ���.
*                       - I2C_Register_CR2:   CR2 �Ĵ���.
*                       - I2C_Register_OAR1:  OAR1 �Ĵ���.
*                       - I2C_Register_OAR2:  OAR2 �Ĵ���.
*                       - I2C_Register_DR:    DR �Ĵ���.
*                       - I2C_Register_SR1:   SR1 �Ĵ���.
*                       - I2C_Register_SR2:   SR2 �Ĵ���.
*                       - I2C_Register_CCR:   CCR �Ĵ���.
*                       - I2C_Register_TRISE: TRISE �Ĵ���.
* �������: ��
* ���ز���: ѡ��Ĵ�����ֵ.
*******************************************************************************/
u16 I2C_ReadRegister(I2C_TypeDef* I2Cx, u8 I2C_Register)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_REGISTER(I2C_Register));

  /* Return the selected register value [����ѡ��Ĵ�����ֵ]*/
  return (*(vu16 *)(*((vu32 *)&I2Cx) + I2C_Register));
}

/*******************************************************************************
* ��������: I2C_SoftwareResetCmd
* ��������: ʹ��(���ֹ)�ض�I2C�����λ.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)NewState��I2C�����λ����״̬.
*                    �ò���Ϊ��ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_SoftwareResetCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Peripheral under reset [�����¸�λ]*/
    I2Cx->CR1 |= CR1_SWRST_Set;
  }
  else
  {
    /* Peripheral not under reset [���費�¸�λ]*/
    I2Cx->CR1 &= CR1_SWRST_Reset;
  }
}

/*******************************************************************************
* ��������: I2C_SMBusAlertConfig
* ��������: Ϊ�ض���I2C����SMBAlert����Ϊ�߻��.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)I2C_SMBusAlert��SMBusAlert���ŵ�ƽ�� 
*                    ������������������ֵ֮һ:
*                       - I2C_SMBusAlert_Low: SMBusAlert��������Ϊ��
*                       - I2C_SMBusAlert_High: SMBusAlert��������Ϊ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_SMBusAlertConfig(I2C_TypeDef* I2Cx, u16 I2C_SMBusAlert)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_SMBUS_ALERT(I2C_SMBusAlert));

  if (I2C_SMBusAlert == I2C_SMBusAlert_Low)
  {
    /* Drive the SMBusAlert pin Low [����SMBusAlert����Ϊ��]*/
    I2Cx->CR1 |= I2C_SMBusAlert_Low;
  }
  else
  {
    /* Drive the SMBusAlert pin High  [����SMBusAlert����Ϊ��]*/
    I2Cx->CR1 &= I2C_SMBusAlert_High;
  }
}

/*******************************************************************************
* ��������: I2C_TransmitPEC
* ��������: ʹ�ܣ����ֹ���ض�I2C��PEC����
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)NewState��I2C��PEC�������״̬.
*                    �ò���Ϊ��ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_TransmitPEC(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C PEC transmission [ʹ��ѡ��I2C��PEC����]*/
    I2Cx->CR1 |= CR1_PEC_Set;
  }
  else
  {
    /* Disable the selected I2C PEC transmission [��ֹѡ��I2C��PEC����]*/
    I2Cx->CR1 &= CR1_PEC_Reset;
  }
}

/*******************************************************************************
* ��������: I2C_PECPositionConfig
* ��������: ѡ���ض���I2C��PECλ��.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)I2C_PECPosition��PECλ��. 
*                    ������������������ֵ֮һ:
*                       - I2C_PECPosition_Next: ��һ�ֽھ���PEC
*                       - I2C_PECPosition_Current: ��ǰ�ֽھ���PEC
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_PECPositionConfig(I2C_TypeDef* I2Cx, u16 I2C_PECPosition)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_PEC_POSITION(I2C_PECPosition));

  if (I2C_PECPosition == I2C_PECPosition_Next)
  {
    /* Next byte in shift register is PEC [����λ�Ĵ����е���һ���ֽ���PEC]*/
    I2Cx->CR1 |= I2C_PECPosition_Next;
  }
  else
  {
    /* Current byte in shift register is PEC [����λ�Ĵ����еĵ�ǰ�ֽ���PEC]*/
    I2Cx->CR1 &= I2C_PECPosition_Current;
  }
}

/*******************************************************************************
* ��������: I2C_CalculatePEC
* ��������: ʹ��(���ֹ)�Ѵ����ֽڵ�PEC����ֵ.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)NewState��PEC����ֵ����״̬. �ò���Ϊ��ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_CalculatePEC(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C PEC calculation [ʹ��ѡ���I2C PEC����ֵ]*/
    I2Cx->CR1 |= CR1_ENPEC_Set;
  }
  else
  {
    /* Disable the selected I2C PEC calculation [��ֹѡ���I2C PEC����ֵ]*/
    I2Cx->CR1 &= CR1_ENPEC_Reset;
  }
}

/*******************************************************************************
* ��������: I2C_GetPEC
* ��������: Ϊ�ض�I2C�ӿڷ���PECֵ.
* �������: I2Cx��x��Ϊ1��2��ѡ��I2C����.
* �������: ��
* ���ز���: PECֵ.
*******************************************************************************/
u8 I2C_GetPEC(I2C_TypeDef* I2Cx)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Return the selected I2C PEC value [����ѡ���I2C PECֵ]*/
  return ((I2Cx->SR2) >> 8);
}

/*******************************************************************************
* ��������: I2C_ARPCmd
* ��������: ʹ�ܣ����ֹ���ض���I2C��ARP
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)NewState��I2Cx��ARP����״̬.�ò���Ϊ��ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_ARPCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C ARP [ʹ��ѡ���I2C ARP]*/
    I2Cx->CR1 |= CR1_ENARP_Set;
  }
  else
  {
    /* Disable the selected I2C ARP [��ֹѡ���I2C ARP]*/
    I2Cx->CR1 &= CR1_ENARP_Reset;
  }
}

/*******************************************************************************
* ��������: I2C_StretchClockCmd
* ��������: ʹ�ܣ����ֹ���ض���I2Cʱ����չ
* �������: I(1)2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)NewState��ʱ����չ����״̬.�ò���Ϊ��ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_StretchClockCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState == DISABLE)
  {
    /* Enable the selected I2C Clock stretching [ʹ��ѡ���I2Cʱ����չ]*/
    I2Cx->CR1 |= CR1_NOSTRETCH_Set;
  }
  else
  {
    /* Disable the selected I2C Clock stretching [��ֹѡ���I2Cʱ����չ]*/
    I2Cx->CR1 &= CR1_NOSTRETCH_Reset;
  }
}

/*******************************************************************************
* ��������: I2C_FastModeDutyCycleConfig
* ��������: ѡ���ض�I2C�Ŀ���ģʽռ������.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)I2C_DutyCycle������ģʽ��ռ�����ڡ�
*                    ������������������ֵ֮һ:
*                       - I2C_DutyCycle_2: I2C��ģʽTlow/Thigh=2
*                       - I2C_DutyCycle_16_9: I2C��ģʽTlow/Thigh=16/9
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_FastModeDutyCycleConfig(I2C_TypeDef* I2Cx, u16 I2C_DutyCycle)
{
  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_DUTY_CYCLE(I2C_DutyCycle));

  if (I2C_DutyCycle != I2C_DutyCycle_16_9)
  {
    /* I2C fast mode Tlow/Thigh=2 [I2C��ģʽTlow/Thigh=2]*/
    I2Cx->CCR &= I2C_DutyCycle_2;
  }
  else
  {
    /* I2C fast mode Tlow/Thigh=16/9 [I2C��ģʽTlow/Thigh=16/9]*/
    I2Cx->CCR |= I2C_DutyCycle_16_9;
  }
}

/*******************************************************************************
* ��������: I2C_GetLastEvent
* ��������: �������I2Cx�¼�.
* �������: I2Cx��x��Ϊ1��2��ѡ��I2C����.
* �������: ��
* ���ز���: �����I2Cx�¼�
*******************************************************************************/
u32 I2C_GetLastEvent(I2C_TypeDef* I2Cx)
{
  u32 lastevent = 0;
  u32 flag1 = 0, flag2 = 0;

  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Read the I2Cx status register [��I2Cx״̬�Ĵ���]*/
  flag1 = I2Cx->SR1;
  flag2 = I2Cx->SR2;
  flag2 = flag2 << 16;

  /* Get the last event value from I2C status register [��I2Cx״̬�Ĵ�����ȡ�������¼�ֵ]*/
  lastevent = (flag1 | flag2) & FLAG_Mask;

  /* Return status [����״̬����]*/
  return lastevent;
}

/*******************************************************************************
* ��������: I2C_CheckEvent
* ��������: ��������I2C�¼��Ƿ�����Ǹ��Ѵ���Ĳ���.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)I2C_EVENT��ָ���������¼���
*                    ������������������ֵ֮һ:
*                       - I2C_EVENT_SLAVE_ADDRESS_MATCHED   : EV1
*                       - I2C_EVENT_SLAVE_BYTE_RECEIVED     : EV2
*                       - I2C_EVENT_SLAVE_BYTE_TRANSMITTED  : EV3
*                       - I2C_EVENT_SLAVE_ACK_FAILURE       : EV3-2
*                       - I2C_EVENT_MASTER_MODE_SELECT      : EV5
*                       - I2C_EVENT_MASTER_MODE_SELECTED    : EV6
*                       - I2C_EVENT_MASTER_BYTE_RECEIVED    : EV7
*                       - I2C_EVENT_MASTER_BYTE_TRANSMITTED : EV8
*                       - I2C_EVENT_MASTER_MODE_ADDRESS10   : EV9
*                       - I2C_EVENT_SLAVE_STOP_DETECTED     : EV4
* �������: ��
* ���ز���: һ��ErrorStatusö��ֵ��SUCCESS��������¼���I2C_EVENT��ͬ��
*                                  ERROR��������¼���I2C_EVENT��ͬ
*
*******************************************************************************/
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, u32 I2C_EVENT)
{
  u32 lastevent = 0;
  u32 flag1 = 0, flag2 = 0;
  ErrorStatus status = ERROR;

  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_EVENT(I2C_EVENT));

  /* Read the I2Cx status register [��I2Cx״̬�Ĵ���]*/
  flag1 = I2Cx->SR1;
  flag2 = I2Cx->SR2;
  flag2 = flag2 << 16;

  /* Get the last event value from I2C status register [��I2Cx״̬�Ĵ�����ȡ�������¼�ֵ]*/
  lastevent = (flag1 | flag2) & FLAG_Mask;

  /* Check whether the last event is equal to I2C_EVENT [��������¼�ֵ�Ƿ����I2C_EVENT]*/
  if (lastevent == I2C_EVENT )
  {
    /* SUCCESS: last event is equal to I2C_EVENT [SUCCESS��������¼���I2C_EVENT��ͬ]*/
    status = SUCCESS;
  }
  else
  {
    /* ERROR: last event is different from I2C_EVENT [ERROR��������¼���I2C_EVENT��ͬ]*/
    status = ERROR;
  }

  /* Return status [����״̬����]*/
  return status;
}

/*******************************************************************************
* ��������: I2C_GetFlagStatus
* ��������: ����ض���I2C��־�Ƿ���λ��.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)I2C_FLAG��ȷ����Ҫ�����ı�־�� 
*                    ������������������ֵ֮һ:
*                       - I2C_FLAG_DUALF: ˫�ر�־����ģʽ��
*                       - I2C_FLAG_SMBHOST: SMBus����ͷ����ģʽ��
*                       - I2C_FLAG_SMBDEFAULT: SMBusĬ��ͷ����ģʽ��
*                       - I2C_FLAG_GENCALL: ͨ�õ���ͷ��־����ģʽ��
*                       - I2C_FLAG_TRA: ������/��������־
*                       - I2C_FLAG_BUSY: ����æ�ı�־
*                       - I2C_FLAG_MSL: ����/�ӻ���־
*                       - I2C_FLAG_SMBALERT: SMBus�����־
*                       - I2C_FLAG_TIMEOUT: ��ʱ��Tlow�����־
*                       - I2C_FLAG_PECERR: ����PEC�����־
*                       - I2C_FLAG_OVR: ����/�����־����ģʽ��
*                       - I2C_FLAG_AF: ȷ��ʧ�ܱ�־
*                       - I2C_FLAG_ARLO: ʧȥ�ܲñ�־����ģʽ��
*                       - I2C_FLAG_BERR: ���ߴ����־
*                       - I2C_FLAG_TXE: ���ݼĴ����յı�־����������
*                       - I2C_FLAG_RXNE: ���ݼĴ����ǿյı�־����������
*                       - I2C_FLAG_STOPF: ֹͣ���Ĵ�������ģʽ��
*                       - I2C_FLAG_ADD10: 10λͷ�����ͱ�־����ģʽ��
*                       - I2C_FLAG_BTF: �ֽڷ��ͽ�����־
*                       - I2C_FLAG_ADDR: ��ַ���ͱ�־����ģʽ��"ADSL"����ַƥ���־����ģʽ��"ENDAD"                                      Address matched flag (Slave mode)�ENDAD?*                       - I2C_FLAG_SB: Start bit flag (Master mode)
* �������: ��
* ���ز���: I2C_FLAG����״̬��SET��RESET��.
*******************************************************************************/
FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, u32 I2C_FLAG)
{
  FlagStatus bitstatus = RESET;
  u32 i2creg = 0, i2cxbase = 0;

  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_GET_FLAG(I2C_FLAG));

  /* Get the I2Cx peripheral base address [ȡ��I2Cx�������ַ]*/
  i2cxbase = (*(u32*)&(I2Cx));
  
  /* Read flag register index [����־�Ĵ�������]*/
  i2creg = I2C_FLAG >> 28;
  
  /* Get bit[23:0] of the flag [ȡ�ñ�־��bit[23:0]λ]*/
  I2C_FLAG &= FLAG_Mask;
  
  if(i2creg != 0)
  {
    /* Get the I2Cx SR1 register address [ȡ��I2Cx SR1�Ĵ�����ַ]*/
    i2cxbase += 0x14;
  }
  else
  {
    /* Flag in I2Cx SR2 Register [���I2Cx SR2�ļĴ���]*/
    I2C_FLAG = (u32)(I2C_FLAG >> 16);
    /* Get the I2Cx SR2 register address [ȡ��I2Cx SR2�Ĵ�����ַ]*/
    i2cxbase += 0x18;
  }
  
  if(((*(vu32 *)i2cxbase) & I2C_FLAG) != (u32)RESET)
  {
    /* I2C_FLAG is set [��λI2C_FLAG]*/
    bitstatus = SET;
  }
  else
  {
    /* I2C_FLAG is reset [��λI2C_FLAG]*/
    bitstatus = RESET;
  }
  
  /* Return the I2C_FLAG status [����I2C_FLAG��״̬]*/
  return  bitstatus;
}

/*******************************************************************************
* ��������: I2C_ClearFlag
* ��������: ���I2Cx�Ĺ����־.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)I2C_FLAG��������ı�־��
*                    �����������������ֵ���������:
*                       - I2C_FLAG_SMBALERT: SMBus�����־
*                       - I2C_FLAG_TIMEOUT: ��ʱ��Tlow�����־
*                       - I2C_FLAG_PECERR: ���յ�PEC�����־
*                       - I2C_FLAG_OVR: ����/�����־����ģʽ��
*                       - I2C_FLAG_AF: ȷ��ʧ�ܱ�־
*                       - I2C_FLAG_ARLO: ʧȥ�ܲñ�־����ģʽ��
*                       - I2C_FLAG_BERR: ���ߴ����־
*                       
*                  ע��: 
*                        - STOPF (ֹͣ���)ֻ���������������������  
*                          ��I2C_SR1�Ĵ�������(I2C_GetFlagStatus())�������д
*                          I2C_CR1 �Ĵ������� (I2C_Cmd() to ����ʹ��I2C����). 
*                        - ADD10 (10-λͷ����)ֻ���������������������
*                          ��I2C_SR1�Ĵ�������(I2C_GetFlagStatus())�������д
*                          �ڶ����ֽڵ�ַ��DR�Ĵ�����
*                        - BTF (�ֽڷ������) ֻ��������������������� 
*                          ��I2C_SR1�Ĵ�������(I2C_GetFlagStatus())������Ŷ�/д
*                          I2C_DR�Ĵ��� (I2C_SendData()).
*                        - ADDR (��ַ����)ֻ���������������������
*                          ��I2C_SR1�Ĵ�������(I2C_GetFlagStatus())������Ŷ�
*                          I2C_SR2�Ĵ������� ((void)(I2Cx->SR2)).
*                        - SB (��ʼλ) ֻ�������������������:��I2C_SR1 
*                          �Ĵ�������(I2C_GetFlagStatus())�������дI2C_DR�Ĵ��� 
*                          ����(I2C_SendData()). 
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_ClearFlag(I2C_TypeDef* I2Cx, u32 I2C_FLAG)
{
  u32 flagpos = 0;

  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_CLEAR_FLAG(I2C_FLAG));

  /* Get the I2C flag position [ȡ��I2C��־λ��]*/
  flagpos = I2C_FLAG & FLAG_Mask;

  /* Clear the selected I2C flag [���ѡ���I2C��־]*/
  I2Cx->SR1 = (u16)~flagpos;
}

/*******************************************************************************
* ��������: I2C_GetITStatus
* ��������: ����ض���I2C�ж��Ƿ����.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)I2C_IT��ȷ����Ҫ�����ж�Դ. 
*                    ������������������ֵ֮һ:
*                       - I2C_IT_SMBALERT: SMBus�����־
*                       - I2C_IT_TIMEOUT: ��ʱ��Tlow�����־
*                       - I2C_IT_PECERR: ���յ�PEC�����־
*                       - I2C_IT_OVR: ����/�����־����ģʽ��
*                       - I2C_IT_AF: ȷ��ʧ�ܱ�־
*                       - I2C_IT_ARLO: ʧȥ�ܲñ�־����ģʽ��
*                       - I2C_IT_BERR: ���ߴ����־
*                       - I2C_IT_TXE: ���ݼĴ����յı�־����������
*                       - I2C_IT_RXNE: ���ݼĴ����ǿյı�־����������
*                       - I2C_IT_STOPF: ֹͣ���Ĵ�������ģʽ��
*                       - I2C_IT_ADD10: 10λͷ���ͱ�־����ģʽ��
*                       - I2C_IT_BTF: �ֽڴ��ͽ�����־
*                       - I2C_IT_ADDR: ��ַ���ͱ�־����ģʽ��"ADSL"����ַ���ϱ�־����ģʽ��"ENDAD"                                      Address matched flag (Slave mode)�ENDAD?*                       - I2C_IT_SB: Start bit flag (Master mode)
* �������: ��
* ���ز���: I2C_IT����״̬��SET��RESET��.
*******************************************************************************/
ITStatus I2C_GetITStatus(I2C_TypeDef* I2Cx, u32 I2C_IT)
{
  ITStatus bitstatus = RESET;
  u32 enablestatus = 0;

  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_GET_IT(I2C_IT));

  /* Check if the interrupt source is enabled or not [����ж�Դ�Ƿ�ʹ��]*/
  enablestatus = (u32)(((I2C_IT & ITEN_Mask) >> 16) & (I2Cx->CR2)) ;  

  /* Get bit[23:0] of the flag [ȡ�ñ�־λ��[23:0]λ]*/
  I2C_IT &= FLAG_Mask;

  /* Check the status of the specified I2C flag [���ָ����I2C��־״̬]*/
  if (((I2Cx->SR1 & I2C_IT) != (u32)RESET) && enablestatus)
  {
    /* I2C_IT is set [��λI2C_IT]*/
    bitstatus = SET;
  }
  else
  {
    /* I2C_IT is reset [��λI2C_IT]*/
    bitstatus = RESET;
  }
  /* Return the I2C_IT status [����I2C_IT��״̬]*/
  return  bitstatus;
}

/*******************************************************************************
* ��������: I2C_ClearITPendingBit
* ��������: ���I2Cx���жϹ���λ.
* �������: (1)I2Cx��x��Ϊ1��2��ѡ��I2C����.
*           (2)I2C_IT��ȷ����Ҫ������жϹ���λ�� 
*                    �����������������ֵ���������:
*                       - I2C_IT_SMBALERT: SMBus�澯�ж�
*                       - I2C_IT_TIMEOUT: ��ʱ��Tlow�����ж�
*                       - I2C_IT_PECERR: ���յ�PEC�����ж�
*                       - I2C_IT_OVR: ����/�����жϣ���ģʽ��
*                       - I2C_IT_AF: ȷ��ʧ���ж�
*                       - I2C_IT_ARLO: ʧȥ�ܲ��жϣ���ģʽ��
*                       - I2C_IT_BERR: ���ߴ����ж�
*                       
*                  ע��:
*                        - STOPF (ֹͣ���)ֻ���������������������  
*                          ��I2C_SR1�Ĵ�������(I2C_GetFlagStatus())�������д
*                          I2C_CR1 �Ĵ������� (I2C_Cmd() to ����ʹ��I2C����). 
*                        - ADD10 (10-λͷ����)ֻ���������������������
*                          ��I2C_SR1�Ĵ�������(I2C_GetFlagStatus())�������д
*                          �ڶ����ֽڵ�ַ��DR�Ĵ�����
*                        - BTF (�ֽڷ������) ֻ��������������������� 
*                          ��I2C_SR1�Ĵ�������(I2C_GetFlagStatus())������Ŷ�/д
*                          I2C_DR�Ĵ��� (I2C_SendData()).
*                        - ADDR (��ַ����)ֻ���������������������
*                          ��I2C_SR1�Ĵ�������(I2C_GetFlagStatus())������Ŷ�
*                          I2C_SR2�Ĵ������� ((void)(I2Cx->SR2)).
*                        - SB (��ʼλ) ֻ�������������������:��I2C_SR1 
*                          �Ĵ�������(I2C_GetFlagStatus())�������дI2C_DR�Ĵ��� 
*                          ����(I2C_SendData()).
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2C_ClearITPendingBit(I2C_TypeDef* I2Cx, u32 I2C_IT)
{
  u32 flagpos = 0;

  /* Check the parameters [������]*/
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_CLEAR_IT(I2C_IT));

  /* Get the I2C flag position [ȡ��I2C��־��λ��]*/
  flagpos = I2C_IT & FLAG_Mask;

  /* Clear the selected I2C flag [���ѡ���I2C��־]*/
  I2Cx->SR1 = (u16)~flagpos;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
