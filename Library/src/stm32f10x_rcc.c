/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_rcc.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: This file provides all the RCC firmware functions.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_rcc.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
/* ------------ RCC registers bit address in the alias region ----------- */
#define RCC_OFFSET                (RCC_BASE - PERIPH_BASE)

/* --- CR �Ĵ��� ---*/
/* Alias word address of HSION bit */
#define CR_OFFSET                 (RCC_OFFSET + 0x00)
#define HSION_BitNumber           0x00
#define CR_HSION_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (HSION_BitNumber * 4))

/* Alias word address of PLLON bit */
#define PLLON_BitNumber           0x18
#define CR_PLLON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLON_BitNumber * 4))

/* Alias word address of CSSON bit */
#define CSSON_BitNumber           0x13
#define CR_CSSON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (CSSON_BitNumber * 4))

/* --- CFGR �Ĵ��� ---*/
/* Alias word address of USBPRE bit */
#define CFGR_OFFSET               (RCC_OFFSET + 0x04)
#define USBPRE_BitNumber          0x16
#define CFGR_USBPRE_BB            (PERIPH_BB_BASE + (CFGR_OFFSET * 32) + (USBPRE_BitNumber * 4))

/* --- BDCR �Ĵ��� ---*/
/* Alias word address of RTCEN bit */
#define BDCR_OFFSET               (RCC_OFFSET + 0x20)
#define RTCEN_BitNumber           0x0F
#define BDCR_RTCEN_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (RTCEN_BitNumber * 4))

/* Alias word address of BDRST bit */
#define BDRST_BitNumber           0x10
#define BDCR_BDRST_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (BDRST_BitNumber * 4))

/* --- CSR �Ĵ��� ---*/
/* Alias word address of LSION bit */
#define CSR_OFFSET                (RCC_OFFSET + 0x24)
#define LSION_BitNumber           0x00
#define CSR_LSION_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (LSION_BitNumber * 4))

/* ---------------------- RCC �Ĵ���s bit mask ------------------------ */
/* CR register bit mask */
#define CR_HSEBYP_Reset           ((u32)0xFFFBFFFF)
#define CR_HSEBYP_Set             ((u32)0x00040000)
#define CR_HSEON_Reset            ((u32)0xFFFEFFFF)
#define CR_HSEON_Set              ((u32)0x00010000)
#define CR_HSITRIM_Mask           ((u32)0xFFFFFF07)

/* CFGR register bit mask */
#define CFGR_PLL_Mask             ((u32)0xFFC0FFFF)
#define CFGR_PLLMull_Mask         ((u32)0x003C0000)
#define CFGR_PLLSRC_Mask          ((u32)0x00010000)
#define CFGR_PLLXTPRE_Mask        ((u32)0x00020000)
#define CFGR_SWS_Mask             ((u32)0x0000000C)
#define CFGR_SW_Mask              ((u32)0xFFFFFFFC)
#define CFGR_HPRE_Reset_Mask      ((u32)0xFFFFFF0F)
#define CFGR_HPRE_Set_Mask        ((u32)0x000000F0)
#define CFGR_PPRE1_Reset_Mask     ((u32)0xFFFFF8FF)
#define CFGR_PPRE1_Set_Mask       ((u32)0x00000700)
#define CFGR_PPRE2_Reset_Mask     ((u32)0xFFFFC7FF)
#define CFGR_PPRE2_Set_Mask       ((u32)0x00003800)
#define CFGR_ADCPRE_Reset_Mask    ((u32)0xFFFF3FFF)
#define CFGR_ADCPRE_Set_Mask      ((u32)0x0000C000)

/* CSR register bit mask */
#define CSR_RMVF_Set              ((u32)0x01000000)

/* RCC Flag Mask */
#define FLAG_Mask                 ((u8)0x1F)

/* Typical Value of the HSI in Hz */
#define HSI_Value                 ((u32)8000000)

/* CIR register byte 2 (Bits[15:8]) base address */
#define CIR_BYTE2_ADDRESS         ((u32)0x40021009)
/* CIR register byte 3 (Bits[23:16]) base address */
#define CIR_BYTE3_ADDRESS         ((u32)0x4002100A)

/* CFGR register byte 4 (Bits[31:24]) base address */
#define CFGR_BYTE4_ADDRESS        ((u32)0x40021007)

/* BDCR register base address */
#define BDCR_ADDRESS              (PERIPH_BASE + BDCR_OFFSET)

/* Time out for HSE start up */
#define HSEStartUp_TimeOut        ((u16)0x01FF)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
static uc8 APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};
static uc8 ADCPrescTable[4] = {2, 4, 6, 8};

static volatile FlagStatus HSEStatus;
static vu32 StartUpCounter = 0;

/* ���ú���ԭ�� -----------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: RCC_DeInit
* ��������: ��λRCC��Χ�豸�Ĵ�����Ĭ�ϸ�λֵ.
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_DeInit(void)
{
  /* Set HSION bit [��λHSION]*/
  RCC->CR |= (u32)0x00000001;

  /* Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], ADCPRE[1:0] and MCO[2:0] bits 
  *      [��λSW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], ADCPRE[1:0] �� MCO[2:0]λ]*/
  RCC->CFGR &= (u32)0xF8FF0000;
  
  /* Reset HSEON, CSSON and PLLON bits [��λHSEON, CSSON �� PLLONλ]*/
  RCC->CR &= (u32)0xFEF6FFFF;

  /* Reset HSEBYP bit [��λHSEBYP λ]*/
  RCC->CR &= (u32)0xFFFBFFFF;

  /* Reset PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE bits [��λPLLSRC, PLLXTPRE, PLLMUL[3:0]��USBPRE λ]*/
  RCC->CFGR &= (u32)0xFF80FFFF;

  /* Disable all interrupts [��ֹ�����ж�]*/
  RCC->CIR = 0x00000000;
}

/*******************************************************************************
* ��������: RCC_HSEConfig
* ��������: �����ⲿ��������(HSE)
*                  HSE can not be stopped if it is used directly or through the 
*                  PLL as system clock[HSE���ֱ�ӻ���ͨ��PLL����ϵͳʱ�ӵ�ʱ���ǲ��ܱ�ͣ������].
* �������: RCC_HSE:HSE����״̬.
*                    ������������������ֵ֮һ:
*                       - RCC_HSE_OFF: HSE oscillator OFF[HSE�����ر�]
*                       - RCC_HSE_ON: HSE oscillator ON[HSE��������]
*                       - RCC_HSE_Bypass: HSE oscillator bypassed with external
*                         clock[�����ⲿʱ�ӵ�HSE����]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_HSEConfig(u32 RCC_HSE)
{
  /* Check the parameters [������]*/
  assert_param(IS_RCC_HSE(RCC_HSE));

  /* Reset HSEON and HSEBYP bits before configuring the HSE [������HSE֮ǰ��λ HSEON��HSEBYPλ]*/
  /* Reset HSEON bit [��λHSEON λ]*/
  RCC->CR &= CR_HSEON_Reset;

  /* Reset HSEBYP bit [��λHSEBYP λ]*/
  RCC->CR &= CR_HSEBYP_Reset;

  /* Configure HSE (RCC_HSE_OFF is already covered by the code section above) [����HSE��RCC_HSE_OFF�Ѿ������ִ��������أ�]*/
  switch(RCC_HSE)
  {
    case RCC_HSE_ON:
      /* Set HSEON bit [��λHSEON]*/
      RCC->CR |= CR_HSEON_Set;
      break;
      
    case RCC_HSE_Bypass:
      /* Set HSEBYP and HSEON bits [��λHSEBYP��HSEON]*/
      RCC->CR |= CR_HSEBYP_Set | CR_HSEON_Set;
      break;            
      
    default:
      break;      
  }
}

/*******************************************************************************
* ��������: RCC_WaitForHSEStartUp
* ��������: �ȴ�HSE����.
* �������: ��
* �������: ��
* ���ز���: һ��ErrorStatusö��ֵ:SUCCESS:HSE�����ȶ����ҿ���ʹ��.
*                                 ERROR:HSE����û��׼����.
*******************************************************************************/
ErrorStatus RCC_WaitForHSEStartUp(void)
{
  ErrorStatus status = ERROR;

  /* Wait till HSE is ready and if Time out is reached exit [�ȴ�HSE׼����,��ʱʱ�䵽��]*/
  do
  {
    HSEStatus = RCC_GetFlagStatus(RCC_FLAG_HSERDY);
    StartUpCounter++;  
  } while((HSEStatus == RESET) && (StartUpCounter != HSEStartUp_TimeOut));


  if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET)
  {
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }  

  return (status);
}

/*******************************************************************************
* ��������: RCC_AdjustHSICalibrationValue
* ��������: У���ڲ���������(HSI)���ֵ.
* �������: HSICalibrationValue:У�����ֵ���������һ������0��0x1F֮���ֵ.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_AdjustHSICalibrationValue(u8 HSICalibrationValue)
{
  u32 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_RCC_CALIBRATION_VALUE(HSICalibrationValue));

  tmpreg = RCC->CR;

  /* Clear HSITRIM[4:0] bits [��HSITRIM[4:0]λ]*/
  tmpreg &= CR_HSITRIM_Mask;

  /* Set the HSITRIM[4:0] bits according to HSICalibrationValue value [����HSICalibrationValue��ֵ����HSITRIM[4:0]λ]*/
  tmpreg |= (u32)HSICalibrationValue << 3;

  /* Store the new value [�����µ�ֵ]*/
  RCC->CR = tmpreg;
}

/*******************************************************************************
* ��������: RCC_HSICmd
* ��������: ʹ���ڲ���������(HIS).
*                  [HSI���ֱ�ӻ���ͨ��PLL����ϵͳʱ�ӵ�ʱ���ǲ��ܱ�ͣ������]
* �������: NewStateHSI����״̬.�������������:ENABLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_HSICmd(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CR_HSION_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: RCC_PLLConfig
* ��������: ����PLLʱ��Դ�ͳ˷�����.
*                  This function must be used only when the PLL is disabled.
* �������: (1)RCC_PLLSource:PLL����ʱ��Դ.
*                    ������������������ֵ֮һ:
*                       - RCC_PLLSource_HSI_Div2: PLLʱ���������HISʱ�Ӷ���Ƶ
*                       - RCC_PLLSource_HSE_Div1: PLLʱ���������HISʱ��
*                       - RCC_PLLSource_HSE_Div2: PLLʱ���������HISʱ�Ӷ���Ƶ
*           (2)RCC_PLLMul:PLL�˷�����.
*                    ������������� RCC_PLLMul_x x����ȡ:[2,16]
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_PLLConfig(u32 RCC_PLLSource, u32 RCC_PLLMul)
{
  u32 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_RCC_PLL_SOURCE(RCC_PLLSource));
  assert_param(IS_RCC_PLL_MUL(RCC_PLLMul));

  tmpreg = RCC->CFGR;

  /* Clear PLLSRC, PLLXTPRE and PLLMUL[3:0] bits [��PLLSRC, PLLXTPRE��PLLMUL[3:0]λ]*/
  tmpreg &= CFGR_PLL_Mask;

  /* Set the PLL configuration bits [����PLL ����λ]*/
  tmpreg |= RCC_PLLSource | RCC_PLLMul;

  /* Store the new value [�����µ�ֵ]*/
  RCC->CFGR = tmpreg;
}

/*******************************************************************************
* ��������: RCC_PLLCmd
* ��������: ʹ�ܻ�ر�PLL��
*                  ���PLL������ϵͳʱ�ӣ����ǲ����Խ�ֹ�ġ�
* �������: NewState:PLL����״̬.�������������:ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_PLLCmd(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CR_PLLON_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: RCC_SYSCLKConfig
* ��������: ����ϵͳʱ��(SYSCLK).
* �������: RCC_SYSCLKSource:��Ϊϵͳʱ�ӵ�ʱ��Դ.
*                 ������������������ֵ֮һ:
*                       - RCC_SYSCLKSource_HSI: ѡ��HSI��Ϊϵͳʱ��
*                       - RCC_SYSCLKSource_HSE: ѡ��HSE��Ϊϵͳʱ��.
*                       - RCC_SYSCLKSource_PLLCLK: ѡ��PLL��Ϊϵͳʱ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_SYSCLKConfig(u32 RCC_SYSCLKSource)
{
  u32 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_RCC_SYSCLK_SOURCE(RCC_SYSCLKSource));

  tmpreg = RCC->CFGR;

  /* Clear SW[1:0] bits [��SW[1:0]λ]*/
  tmpreg &= CFGR_SW_Mask;

  /* Set SW[1:0] bits according to RCC_SYSCLKSource value [����RCC_SYSCLKSource��ֵ����SW[1:0]λ]*/
  tmpreg |= RCC_SYSCLKSource;

  /* Store the new value [�����µ�ֵ]*/
  RCC->CFGR = tmpreg;
}

/*******************************************************************************
* ��������: RCC_GetSYSCLKSource
* ��������: ��������ϵͳʱ�ӵ�ʱ��Դ.
* �������: ��
* �������: ��
* ���ز���: ʱ��Դ��Ϊϵͳʱ��ʹ��. 
*              ����ֵ������: -0x00:ʹ��HSI��Ϊϵͳʱ��
*                            -0x04:ʹ��HSE��Ϊϵͳʱ��
*                            -0x08:ʹ��PLL��Ϊϵͳʱ��.
*******************************************************************************/
u8 RCC_GetSYSCLKSource(void)
{
  return ((u8)(RCC->CFGR & CFGR_SWS_Mask));
}

/*******************************************************************************
* ��������: RCC_HCLKConfig
* ��������: ����AHBʱ��(HCLK).
* �������: RCC_HCLK:������AHBʱ��.���ʱ���Ǵ�ϵͳʱ��(SYSCLK)�õ���.
*                    ������������������ֵ֮һ:
*                       - RCC_SYSCLK_Div1: AHBʱ�ӵ���SYSCLK
*                       - RCC_SYSCLK_Div2: AHBʱ�ӵ���SYSCLK/2
*                       - RCC_SYSCLK_Div4: AHBʱ�ӵ���SYSCLK/4
*                       - RCC_SYSCLK_Div8: AHBʱ�ӵ���SYSCLK/8
*                       - RCC_SYSCLK_Div16: AHBʱ�ӵ���SYSCLK/16
*                       - RCC_SYSCLK_Div64: AHBʱ�ӵ���SYSCLK/64
*                       - RCC_SYSCLK_Div128: AHBʱ�ӵ���SYSCLK128
*                       - RCC_SYSCLK_Div256: AHBʱ�ӵ���SYSCLK/256
*                       - RCC_SYSCLK_Div512: AHBʱ�ӵ���SYSCLK/512
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_HCLKConfig(u32 RCC_SYSCLK)
{
  u32 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_RCC_HCLK(RCC_SYSCLK));

  tmpreg = RCC->CFGR;

  /* Clear HPRE[3:0] bits [��HPRE[3:0]λ]*/
  tmpreg &= CFGR_HPRE_Reset_Mask;

  /* Set HPRE[3:0] bits according to RCC_SYSCLK value [����RCC_SYSCLK��ֵ����HPRE[3:0]λ]*/
  tmpreg |= RCC_SYSCLK;

  /* Store the new value [�����µ�ֵ]*/
  RCC->CFGR = tmpreg;
}

/*******************************************************************************
* ��������: RCC_PCLK1Config
* ��������: ���õ���APBʱ��(PCLK1).
* �������: RCC_PCLK1:����APB1ʱ��.���ʱ���Ǵ�AHBʱ�ӵõ���(HCLK).
*                    ������������������ֵ֮һ:
*                       - RCC_HCLK_Div1: APB1ʱ�ӵ���HCLK
*                       - RCC_HCLK_Div2: APB1ʱ�ӵ���HCLK/2
*                       - RCC_HCLK_Div4: APB1ʱ�ӵ���HCLK/4
*                       - RCC_HCLK_Div8: APB1ʱ�ӵ���HCLK/8
*                       - RCC_HCLK_Div16: APB1ʱ�ӵ���HCLK/16
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_PCLK1Config(u32 RCC_HCLK)
{
  u32 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_RCC_PCLK(RCC_HCLK));

  tmpreg = RCC->CFGR;

  /* Clear PPRE1[2:0] bits [��PPRE1[2:0]λ]*/
  tmpreg &= CFGR_PPRE1_Reset_Mask;

  /* Set PPRE1[2:0] bits according to RCC_HCLK value [����RCC_HCLK��ֵ����PPRE1[2:0]λ]*/
  tmpreg |= RCC_HCLK;

  /* Store the new value [�����µ�ֵ]*/
  RCC->CFGR = tmpreg;
}

/*******************************************************************************
* ��������: RCC_PCLK2Config
* ��������: ����APBʱ��(PCLK2).
* �������: RCC_PCLK2:����APB2ʱ��.���ʱ���Ǵ�AHBʱ�ӵõ���(HCLK).
*                    ������������������ֵ֮һ:
*                       - RCC_HCLK_Div1: APB2ʱ�ӵ���HCLK
*                       - RCC_HCLK_Div2: APB2ʱ�ӵ���HCLK/2
*                       - RCC_HCLK_Div4: APB2ʱ�ӵ���HCLK/4
*                       - RCC_HCLK_Div8: APB2ʱ�ӵ���HCLK/8
*                       - RCC_HCLK_Div16: APB2ʱ�ӵ���HCLK/16
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_PCLK2Config(u32 RCC_HCLK)
{
  u32 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_RCC_PCLK(RCC_HCLK));

  tmpreg = RCC->CFGR;

  /* Clear PPRE2[2:0] bits [��PPRE2[2:0]λ]*/
  tmpreg &= CFGR_PPRE2_Reset_Mask;

  /* Set PPRE2[2:0] bits according to RCC_HCLK value [����RCC_HCLK��ֵ����PPRE2[2:0]λ]*/
  tmpreg |= RCC_HCLK << 3;

  /* Store the new value [�����µ�ֵ]*/
  RCC->CFGR = tmpreg;
}

/*******************************************************************************
* ��������: RCC_ITConfig
* ��������: ʹ�ܻ�ر��ض���RCC�ж�.
* �������: (1)RCC_IT:ָ��Ҫʹ�ܻ�Ҫ�رյ�RCC�ж�Դ.
*                    ���������������������ֵ����ϣ�
*                       - RCC_IT_LSIRDY: LSI�����ж�
*                       - RCC_IT_LSERDY: LSE�����ж�
*                       - RCC_IT_HSIRDY: HSI�����ж�
*                       - RCC_IT_HSERDY: HSE�����ж�
*                       - RCC_IT_PLLRDY: PLL�����ж�
*           (2)NewState:ָ����RCC�жϵ���״̬.�������������:ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_ITConfig(u8 RCC_IT, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_RCC_IT(RCC_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Perform Byte access to RCC_CIR[12:8] bits to enable the selected interrupts [ִ��RCC_CIR[12:8]λ���ֽڴ�ȡ��ʹ��ѡ����ж�]*/
    *(vu8 *) CIR_BYTE2_ADDRESS |= RCC_IT;
  }
  else
  {
    /* Perform Byte access to RCC_CIR[12:8] bits to disable the selected interrupts [ִ��RCC_CIR[12:8]λ���ֽڴ�ȡ����ֹѡ����ж�]*/
    *(vu8 *) CIR_BYTE2_ADDRESS &= (u8)~RCC_IT;
  }
}

/*******************************************************************************
* ��������: RCC_USBCLKConfig
* ��������: ����USBʱ��(USBCLK).
* �������: RCC_USBCLKSource:ָ��USBʱ��Դ,���ʱ����Դ��PLL���.
*                    ������������������ֵ֮һ:
*                       - RCC_USBCLKSource_PLLCLK_1Div5: USBʱ��Դ����ѡ����PLLʱ�ӵ�1.5��Ƶ
*                       - RCC_USBCLKSource_PLLCLK_Div1: USBʱ��Դ����ѡ����PLLʱ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_USBCLKConfig(u32 RCC_USBCLKSource)
{
  /* Check the parameters [������]*/
  assert_param(IS_RCC_USBCLK_SOURCE(RCC_USBCLKSource));

  *(vu32 *) CFGR_USBPRE_BB = RCC_USBCLKSource;
}

/*******************************************************************************
* ��������: RCC_ADCCLKConfig
* ��������: ����ADCʱ��(ADCCKL).
* �������: RCC_ADCCLK:������ADCʱ��.���ʱ������APB2ʱ��(PCLK2).
*                    ������������������ֵ֮һ:
*                       - RCC_PCLK2_Div2: ADCʱ�ӵ���PCLK2/2
*                       - RCC_PCLK2_Div4: ADCʱ�ӵ���PCLK2/4
*                       - RCC_PCLK2_Div6: ADCʱ�ӵ���PCLK2/6
*                       - RCC_PCLK2_Div8: ADCʱ�ӵ���PCLK2/8
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_ADCCLKConfig(u32 RCC_PCLK2)
{
  u32 tmpreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_RCC_ADCCLK(RCC_PCLK2));

  tmpreg = RCC->CFGR;

  /* Clear ADCPRE[1:0] bits [��ADCPRE[1:0]λ]*/
  tmpreg &= CFGR_ADCPRE_Reset_Mask;

  /* Set ADCPRE[1:0] bits according to RCC_PCLK2 value [����RCC_PCLK2��ֵ����ADCPRE[1:0]λ]*/
  tmpreg |= RCC_PCLK2;

  /* Store the new value [�����µ�ֵ]*/
  RCC->CFGR = tmpreg;
}

/*******************************************************************************
* ��������: RCC_LSEConfig
* ��������: �����ⲿ��������(LSE).
* �������: RCC_LSE:LSE����״̬��
*                    ������������������ֵ֮һ:
*                       - RCC_LSE_OFF: LSE�����ر�
*                       - RCC_LSE_ON: LSE��������
*                       - RCC_LSE_Bypass: �����ⲿʱ�ӵ�LSE����
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_LSEConfig(u8 RCC_LSE)
{
  /* Check the parameters [������]*/
  assert_param(IS_RCC_LSE(RCC_LSE));

  /* Reset LSEON and LSEBYP bits before configuring the LSE [������LSE֮ǰ��λLSEON��LSEBYP]*/
  /* Reset LSEON bit [��λLSEON]*/
  *(vu8 *) BDCR_ADDRESS = RCC_LSE_OFF;

  /* Reset LSEBYP bit [��λLSEBYP]*/
  *(vu8 *) BDCR_ADDRESS = RCC_LSE_OFF;

  /* Configure LSE (RCC_LSE_OFF is already covered by the code section above) [����LSE��RCC_LSE_OFF�Ѿ������ִ��������أ�]*/
  switch(RCC_LSE)
  {
    case RCC_LSE_ON:
      /* Set LSEON bit [��λLSEON]*/
      *(vu8 *) BDCR_ADDRESS = RCC_LSE_ON;
      break;
      
    case RCC_LSE_Bypass:
      /* Set LSEBYP and LSEON bits [��λLSEBYP��LSEON]*/
      *(vu8 *) BDCR_ADDRESS = RCC_LSE_Bypass | RCC_LSE_ON;
      break;            
      
    default:
      break;      
  }
}

/*******************************************************************************
* ��������: RCC_LSICmd
* ��������: ʹ�ܻ�ر��ڲ���������(LSI).
*                  ���IWDG����,��LSI���ܱ���ֹ��
* �������: NewState:LSI����״̬�����������ֵ������:ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_LSICmd(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CSR_LSION_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: RCC_RTCCLKConfig
* ��������: ����RTCʱ��(RTCCLK).
*                  ֻҪѡ����RTCʱ��,RTCʱ�ӾͲ��ܸı�ֱ��������λ.
* �������: RCC_RTCCLKSource:RTCʱ��Դ.
*                    ������������������ֵ֮һ:
*                       - RCC_RTCCLKSource_LSE: ѡ��LSE��ΪRTCʱ��
*                       - RCC_RTCCLKSource_LSI: ѡ��LSI��ΪRTCʱ��
*                       - RCC_RTCCLKSource_HSE_Div128: ѡ��HSEʱ�ӵ�128��Ƶ��ΪRTCʱ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_RTCCLKConfig(u32 RCC_RTCCLKSource)
{
  /* Check the parameters [������]*/
  assert_param(IS_RCC_RTCCLK_SOURCE(RCC_RTCCLKSource));

  /* Select the RTC clock source [ѡ��RTCʱ��Դ]*/
  RCC->BDCR |= RCC_RTCCLKSource;
}

/*******************************************************************************
* ��������: RCC_RTCCLKCmd
* ��������: ʹ�ܻ�ر�RTCʱ��.
*                  ����ʹ��RCC_RTCCLKConfig����ѡ��RTCʱ�Ӻ�,��ʹ���������.
* �������: NewState:.RTCʱ�ӵ���״̬.�������������:ENABLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_RTCCLKCmd(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) BDCR_RTCEN_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: RCC_GetClocksFreq
* ��������: ����Ƭ��ʱ�ӵĲ�ͬƵ��.
* �������: RCC_Clocks:ָ�����ʱ��Ƶ�ʵ�RCC_ClocksTypeDef�ṹ��ָ�롣
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
  u32 tmp = 0, pllmull = 0, pllsource = 0, presc = 0;

  /* Get SYSCLK source [ȡ��ϵͳʱ��Դ]*/
  tmp = RCC->CFGR & CFGR_SWS_Mask;

  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock [HSI��Ϊϵͳʱ��]*/
      RCC_Clocks->SYSCLK_Frequency = HSI_Value;
      break;

    case 0x04:  /* HSE used as system clock [HSE��Ϊϵͳʱ��]*/
      RCC_Clocks->SYSCLK_Frequency = HSE_Value;
      break;

    case 0x08:  /* PLL used as system clock [PLL��Ϊϵͳʱ��]*/
      /* Get PLL clock source and multiplication factor [ȡ��PLL��ʱ��Դ�ͱ�������]-*/
      pllmull = RCC->CFGR & CFGR_PLLMull_Mask;
      pllmull = ( pllmull >> 18) + 2;

      pllsource = RCC->CFGR & CFGR_PLLSRC_Mask;

      if (pllsource == 0x00)
      {/* HSI oscillator clock divided by 2 selected as PLL clock entry [ѡ��HSI����ʱ�ӳ���2��ΪPLL��ʱ������]*/
        RCC_Clocks->SYSCLK_Frequency = (HSI_Value >> 1) * pllmull;
      }
      else
      {/* HSE selected as PLL clock entry [ѡ��HSE��ΪPLL��ʱ������]*/

        if ((RCC->CFGR & CFGR_PLLXTPRE_Mask) != (u32)RESET)
        {/* HSE oscillator clock divided by 2 [HSE����ʱ�ӳ���2]**/

          RCC_Clocks->SYSCLK_Frequency = (HSE_Value >> 1) * pllmull;
        }
        else
        {
          RCC_Clocks->SYSCLK_Frequency = HSE_Value * pllmull;
        }
      }
      break;

    default:
      RCC_Clocks->SYSCLK_Frequency = HSI_Value;
      break;
  }

  /* Compute HCLK, PCLK1, PCLK2 and ADCCLK clocks frequencies [����HCLK, PCLK1, PCLK2��ADCCLKʱ��Ƶ��]*/
  /* Get HCLK prescaler */
  tmp = RCC->CFGR & CFGR_HPRE_Set_Mask;
  tmp = tmp >> 4;
  presc = APBAHBPrescTable[tmp];

  /* HCLK clock frequency [HCLK��ʱ��Ƶ��]*/
  RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;

  /* Get PCLK1 prescaler [����HCLK1��Ԥ��Ƶ]*/
  tmp = RCC->CFGR & CFGR_PPRE1_Set_Mask;
  tmp = tmp >> 8;
  presc = APBAHBPrescTable[tmp];

  /* PCLK1 clock frequency [PCLK1��ʱ��Ƶ��]*/
  RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;

  /* Get PCLK2 prescaler [ȡ��PCLK2��Ԥ��Ƶ]*/
  tmp = RCC->CFGR & CFGR_PPRE2_Set_Mask;
  tmp = tmp >> 11;
  presc = APBAHBPrescTable[tmp];

  /* PCLK2 clock frequency [PCLK2��ʱ��Ƶ��]*/
  RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;

  /* Get ADCCLK prescaler [ȡ��ADCCLK��Ԥ��Ƶ]*/
  tmp = RCC->CFGR & CFGR_ADCPRE_Set_Mask;
  tmp = tmp >> 14;
  presc = ADCPrescTable[tmp];

  /* ADCCLK clock frequency [ADCCLK��ʱ��Ƶ��]*/
  RCC_Clocks->ADCCLK_Frequency = RCC_Clocks->PCLK2_Frequency / presc;
}

/*******************************************************************************
* ��������: RCC_AHBPeriphClockCmd
* ��������: ʹ�ܻ�ر�AHB��Χ�豸ʱ��.
* �������: (1)RCC_AHBPeriph:�����ſ�ʱ�ӵ�AHB��Χ�豸��
*                    ���������������������ֵ����ϣ�
*                       - RCC_AHBPeriph_DMA1
*                       - RCC_AHBPeriph_DMA2
*                       - RCC_AHBPeriph_SRAM
*                       - RCC_AHBPeriph_FLITF
*                       - RCC_AHBPeriph_CRC
*                       - RCC_AHBPeriph_FSMC
*                       - RCC_AHBPeriph_SDIO
*                    SRAM ��FLITF ʱ��ֻ����˯�ߵ�ģʽ�¿�����Ч.
*           (2)NewState:ָ������Χ�豸ʱ�ӵ���״̬.�������������:ENALBLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_AHBPeriphClockCmd(u32 RCC_AHBPeriph, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_RCC_AHB_PERIPH(RCC_AHBPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHBENR |= RCC_AHBPeriph;
  }
  else
  {
    RCC->AHBENR &= ~RCC_AHBPeriph;
  }
}

/*******************************************************************************
* ��������: RCC_APB2PeriphClockCmd
* ��������: ʹ�ܻ�رո���APB(APB2)��Χ�豸ʱ��.
* �������: (1)RCC_AHB2Periph:�����ſ�ʱ�ӵ�AHB2��Χ�豸��
*                    ���������������������ֵ����ϣ�
*                       - RCC_APB2Periph_AFIO, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB,
*                         RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOE,
*                         RCC_APB2Periph_GPIOF, RCC_APB2Periph_GPIOG, RCC_APB2Periph_ADC1,
*                         RCC_APB2Periph_ADC2, RCC_APB2Periph_TIM1, RCC_APB2Periph_SPI1,
*                         RCC_APB2Periph_TIM8, RCC_APB2Periph_USART1, RCC_APB2Periph_ADC3,
*                         RCC_APB2Periph_ALL
*           (2)NewState:ר����Χ�豸ʱ�ӵ���״̬.�������������:ENALBLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_APB2PeriphClockCmd(u32 RCC_APB2Periph, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->APB2ENR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2ENR &= ~RCC_APB2Periph;
  }
}

/*******************************************************************************
* ��������: RCC_APB1PeriphClockCmd
* ��������: ʹ�ܻ�رյ���APB(APB1)��Χ�豸ʱ��
* �������: (1)RCC_APB1Periph: �����ſ�ʱ�ӵ�APB1��Χ�豸��
*                    ���������������������ֵ����ϣ�
*                       - RCC_APB1Periph_TIM2, RCC_APB1Periph_TIM3, RCC_APB1Periph_TIM4,
*                         RCC_APB1Periph_TIM5, RCC_APB1Periph_TIM6, RCC_APB1Periph_TIM7,
*                         RCC_APB1Periph_WWDG, RCC_APB1Periph_SPI2, RCC_APB1Periph_SPI3,
*                         RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_USART4, 
*                         RCC_APB1Periph_USART5, RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2,
*                         RCC_APB1Periph_USB, RCC_APB1Periph_CAN, RCC_APB1Periph_BKP,
*                         RCC_APB1Periph_PWR, RCC_APB1Periph_DAC, RCC_APB1Periph_ALL
*           (2)NewState:ר����Χ�豸ʱ�ӵ���״̬.�������������:ENALBLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_APB1PeriphClockCmd(u32 RCC_APB1Periph, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->APB1ENR |= RCC_APB1Periph;
  }
  else
  {
    RCC->APB1ENR &= ~RCC_APB1Periph;
  }
}

/*******************************************************************************
* ��������: RCC_APB2PeriphResetCmd
* ��������: ǿ�Ȼ��ͷŸ���APB(APB2)��Χ�豸��λ.
* �������: (1)RCC_APB2Periph:��λ��APB2��Χ�豸.
*                    ���������������������ֵ����ϣ�
*                       - RCC_APB2Periph_AFIO, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB,
*                         RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOE,
*                         RCC_APB2Periph_GPIOF, RCC_APB2Periph_GPIOG, RCC_APB2Periph_ADC1,
*                         RCC_APB2Periph_ADC2, RCC_APB2Periph_TIM1, RCC_APB2Periph_SPI1,
*                         RCC_APB2Periph_TIM8, RCC_APB2Periph_USART1, RCC_APB2Periph_ADC3,
*                         RCC_APB2Periph_ALL
*           (2)NewState:ָ����Χ�豸��λ����״̬.�������������:ENALBLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_APB2PeriphResetCmd(u32 RCC_APB2Periph, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->APB2RSTR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2RSTR &= ~RCC_APB2Periph;
  }
}

/*******************************************************************************
* ��������: RCC_APB1PeriphResetCmd
* ��������: ǿ�ƻ��ͷŵ���APB(APB1)��Χ�豸��λ.
* �������: (1)RCC_APB1Periph:ָ��Ҫ��λ��APB1��Χ�豸.
*                    ���������������������ֵ����ϣ�
*                       - RCC_APB1Periph_TIM2, RCC_APB1Periph_TIM3, RCC_APB1Periph_TIM4,
*                         RCC_APB1Periph_TIM5, RCC_APB1Periph_TIM6, RCC_APB1Periph_TIM7,
*                         RCC_APB1Periph_WWDG, RCC_APB1Periph_SPI2, RCC_APB1Periph_SPI3,
*                         RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_USART4, 
*                         RCC_APB1Periph_USART5, RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2,
*                         RCC_APB1Periph_USB, RCC_APB1Periph_CAN, RCC_APB1Periph_BKP,
*                         RCC_APB1Periph_PWR, RCC_APB1Periph_DAC, RCC_APB1Periph_ALL
*           (2)NewState:ָ����Χ�豸��λ����״̬.�������������:ENALBLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_APB1PeriphResetCmd(u32 RCC_APB1Periph, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->APB1RSTR |= RCC_APB1Periph;
  }
  else
  {
    RCC->APB1RSTR &= ~RCC_APB1Periph;
  }
}

/*******************************************************************************
* ��������: RCC_BackupResetCmd
* ��������: ǿ�ƻ��ͷű�����λ.
* �������: NewState:������λ����״̬.�������������:ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_BackupResetCmd(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) BDCR_BDRST_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: RCC_ClockSecuritySystemCmd
* ��������: ʹ�ܻ�ر�ʱ�Ӱ�ȫϵͳ
* �������: NewState:ʱ�Ӱ�ȫϵͳ����״̬.�������������:ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_ClockSecuritySystemCmd(FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CR_CSSON_BB = (u32)NewState;
}

/*******************************************************************************
* ��������: RCC_MCOConfig
* ��������: ѡ��MCO�����ϵ����ʱ��Դ
* �������: RCC_MCO:��ϸ˵��Ҫ�����ʱ��Դ.
*                    ������������������ֵ֮һ:
*                       - RCC_MCO_NoClock: û��ѡ��ʱ��
*                       - RCC_MCO_SYSCLK: ѡ��ϵͳʱ��
*                       - RCC_MCO_HSI: ѡ��HIS��ʱ��
*                       - RCC_MCO_HSE: ѡ��HSE��ʱ��
*                       - RCC_MCO_PLLCLK_Div2:ѡ��PLLʱ������Ƶ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_MCOConfig(u8 RCC_MCO)
{
  /* Check the parameters [������]*/
  assert_param(IS_RCC_MCO(RCC_MCO));

  /* Perform Byte access to MCO[2:0] bits to select the MCO source [ִ��MCO[2:0]λ���ֽڴ�ȡ��ѡ��MCOԴ]*/
  *(vu8 *) CFGR_BYTE4_ADDRESS = RCC_MCO;
}

/*******************************************************************************
* ��������: RCC_GetFlagStatus
* ��������: ���ָ����RCC��־�������.
* �������: RCC_FLAG:Ҫ���ı�־.
*                    ������������������ֵ֮һ:
*                       - RCC_FLAG_HSIRDY: HIS��ʱ�Ӿ���
*                       - RCC_FLAG_HSERDY: HSE��ʱ�Ӿ���
*                       - RCC_FLAG_PLLRDY: PLLʱ�Ӿ���
*                       - RCC_FLAG_LSERDY: LSE��ʱ�Ӿ���
*                       - RCC_FLAG_LSIRDY: LSI��ʱ�Ӿ���
*                       - RCC_FLAG_PINRST: ���Ÿ�λ
*                       - RCC_FLAG_PORRST: POR/PDR��λ
*                       - RCC_FLAG_SFTRST: �����λ
*                       - RCC_FLAG_IWDGRST: �������Ź���λ
*                       - RCC_FLAG_WWDGRST: ���ڿ��Ź���λ
*                       - RCC_FLAG_LPWRRST: �͵�����λ
* �������: ��
* ���ز���: RCC_FLAG����״̬(SET��RESET).
*******************************************************************************/
FlagStatus RCC_GetFlagStatus(u8 RCC_FLAG)
{
  u32 tmp = 0;
  u32 statusreg = 0;
  FlagStatus bitstatus = RESET;

  /* Check the parameters [������]*/
  assert_param(IS_RCC_FLAG(RCC_FLAG));

  /* Get the RCC register index [ȡ��RCC�Ĵ�������]*/
  tmp = RCC_FLAG >> 5;

  if (tmp == 1)               /* The flag to check is in CR register [CR�Ĵ����еļ���־]*/
  {
    statusreg = RCC->CR;
  }
  else if (tmp == 2)          /* The flag to check is in BDCR register [BDCR�Ĵ����еļ���־]*/
  {
    statusreg = RCC->BDCR;
  }
  else                       /* The flag to check is in CSR register [CSR�Ĵ����еļ���־]*/
  {
    statusreg = RCC->CSR;
  }

  /* Get the flag position [ȡ�ñ�־��λ��]*/
  tmp = RCC_FLAG & FLAG_Mask;

  if ((statusreg & ((u32)1 << tmp)) != (u32)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* Return the flag status [���ر�־��״̬]*/
  return bitstatus;
}

/*******************************************************************************
* ��������: RCC_ClearFlag
* ��������: ���RCC��λ��־.
*           ��λ��־Ϊ: RCC_FLAG_PINRST, RCC_FLAG_PORRST,
*                  RCC_FLAG_SFTRST, RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST,
*                  RCC_FLAG_LPWRRST
* �������: ��
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_ClearFlag(void)
{
  /* Set RMVF bit to clear the reset flags [ͨ����λRMVFλ�������λ��־]*/
  RCC->CSR |= CSR_RMVF_Set;
}

/*******************************************************************************
* ��������: RCC_GetITStatus
* ��������: ���ָ����RCC�ж��Ƿ���.
* �������: RCC_IT:���RCC�ж�Դ.
*                    ������������������ֵ֮һ:
*                       - RCC_IT_LSIRDY: LSI�����ж�
*                       - RCC_IT_LSERDY: LSE�����ж�
*                       - RCC_IT_HSIRDY: HIS�����ж�
*                       - RCC_IT_HSERDY: HSE�����ж�
*                       - RCC_IT_PLLRDY: PLL�����ж�
*                       - RCC_IT_CSS: ʱ�Ӱ�ȫϵͳ�ж�
* �������: ��
* ���ز���: RCC_IT����״̬(SETorRESET).
*******************************************************************************/
ITStatus RCC_GetITStatus(u8 RCC_IT)
{
  ITStatus bitstatus = RESET;

  /* Check the parameters [������]*/
  assert_param(IS_RCC_GET_IT(RCC_IT));

  /* Check the status of the specified RCC interrupt [���ָ����RCC�ж�״̬]*/
  if ((RCC->CIR & RCC_IT) != (u32)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* Return the RCC_IT status [����RCC_IT״̬]*/
  return  bitstatus;
}

/*******************************************************************************
* ��������: RCC_ClearITPendingBit
* ��������: ���RCC�жϹ���λ
* �������: RCC_IT:ָ��Ҫ������жϹ���λ..
*                    ���������������������ֵ����ϣ�
*                       - RCC_IT_LSIRDY: LSI�����ж�
*                       - RCC_IT_LSERDY: LSE�����ж�
*                       - RCC_IT_HSIRDY: HIS�����ж�
*                       - RCC_IT_HSERDY: HSE�����ж�
*                       - RCC_IT_PLLRDY: PLL�����ж�
*                       - RCC_IT_CSS: ʱ�Ӱ�ȫϵͳ�ж�
* �������: ��
* ���ز���: ��
*******************************************************************************/
void RCC_ClearITPendingBit(u8 RCC_IT)
{
  /* Check the parameters [������]*/
  assert_param(IS_RCC_CLEAR_IT(RCC_IT));

  /* Perform Byte access to RCC_CIR[23:16] bits to clear the selected interrupt
     pending bits [ִ��RCC_CIR[23:16]λ���ֽڴ�ȡ�����ѡ����жϹ���λ]*/
  *(vu8 *) CIR_BYTE3_ADDRESS = RCC_IT;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
