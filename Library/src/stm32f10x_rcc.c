/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_rcc.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: This file provides all the RCC firmware functions.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_rcc.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
/* ------------ RCC registers bit address in the alias region ----------- */
#define RCC_OFFSET                (RCC_BASE - PERIPH_BASE)

/* --- CR 寄存器 ---*/
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

/* --- CFGR 寄存器 ---*/
/* Alias word address of USBPRE bit */
#define CFGR_OFFSET               (RCC_OFFSET + 0x04)
#define USBPRE_BitNumber          0x16
#define CFGR_USBPRE_BB            (PERIPH_BB_BASE + (CFGR_OFFSET * 32) + (USBPRE_BitNumber * 4))

/* --- BDCR 寄存器 ---*/
/* Alias word address of RTCEN bit */
#define BDCR_OFFSET               (RCC_OFFSET + 0x20)
#define RTCEN_BitNumber           0x0F
#define BDCR_RTCEN_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (RTCEN_BitNumber * 4))

/* Alias word address of BDRST bit */
#define BDRST_BitNumber           0x10
#define BDCR_BDRST_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (BDRST_BitNumber * 4))

/* --- CSR 寄存器 ---*/
/* Alias word address of LSION bit */
#define CSR_OFFSET                (RCC_OFFSET + 0x24)
#define LSION_BitNumber           0x00
#define CSR_LSION_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (LSION_BitNumber * 4))

/* ---------------------- RCC 寄存器s bit mask ------------------------ */
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

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
static uc8 APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};
static uc8 ADCPrescTable[4] = {2, 4, 6, 8};

static volatile FlagStatus HSEStatus;
static vu32 StartUpCounter = 0;

/* 自用函数原型 -----------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: RCC_DeInit
* 功能描述: 复位RCC外围设备寄存器到默认复位值.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_DeInit(void)
{
  /* Set HSION bit [置位HSION]*/
  RCC->CR |= (u32)0x00000001;

  /* Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], ADCPRE[1:0] and MCO[2:0] bits 
  *      [复位SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], ADCPRE[1:0] 和 MCO[2:0]位]*/
  RCC->CFGR &= (u32)0xF8FF0000;
  
  /* Reset HSEON, CSSON and PLLON bits [复位HSEON, CSSON 和 PLLON位]*/
  RCC->CR &= (u32)0xFEF6FFFF;

  /* Reset HSEBYP bit [复位HSEBYP 位]*/
  RCC->CR &= (u32)0xFFFBFFFF;

  /* Reset PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE bits [复位PLLSRC, PLLXTPRE, PLLMUL[3:0]和USBPRE 位]*/
  RCC->CFGR &= (u32)0xFF80FFFF;

  /* Disable all interrupts [禁止所有中断]*/
  RCC->CIR = 0x00000000;
}

/*******************************************************************************
* 函数名称: RCC_HSEConfig
* 功能描述: 配置外部高速振荡器(HSE)
*                  HSE can not be stopped if it is used directly or through the 
*                  PLL as system clock[HSE如果直接或者通过PLL当作系统时钟的时候是不能被停下来的].
* 输入参数: RCC_HSE:HSE的新状态.
*                    这个参数可以是下面的值之一:
*                       - RCC_HSE_OFF: HSE oscillator OFF[HSE振荡器关闭]
*                       - RCC_HSE_ON: HSE oscillator ON[HSE振荡器开启]
*                       - RCC_HSE_Bypass: HSE oscillator bypassed with external
*                         clock[伴随外部时钟的HSE振荡器]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_HSEConfig(u32 RCC_HSE)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_HSE(RCC_HSE));

  /* Reset HSEON and HSEBYP bits before configuring the HSE [在配置HSE之前复位 HSEON和HSEBYP位]*/
  /* Reset HSEON bit [复位HSEON 位]*/
  RCC->CR &= CR_HSEON_Reset;

  /* Reset HSEBYP bit [复位HSEBYP 位]*/
  RCC->CR &= CR_HSEBYP_Reset;

  /* Configure HSE (RCC_HSE_OFF is already covered by the code section above) [配置HSE（RCC_HSE_OFF已经被部分代码上隐藏）]*/
  switch(RCC_HSE)
  {
    case RCC_HSE_ON:
      /* Set HSEON bit [置位HSEON]*/
      RCC->CR |= CR_HSEON_Set;
      break;
      
    case RCC_HSE_Bypass:
      /* Set HSEBYP and HSEON bits [置位HSEBYP和HSEON]*/
      RCC->CR |= CR_HSEBYP_Set | CR_HSEON_Set;
      break;            
      
    default:
      break;      
  }
}

/*******************************************************************************
* 函数名称: RCC_WaitForHSEStartUp
* 功能描述: 等待HSE启动.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 一个ErrorStatus枚举值:SUCCESS:HSE振荡器稳定并且可以使用.
*                                 ERROR:HSE振荡器没有准备好.
*******************************************************************************/
ErrorStatus RCC_WaitForHSEStartUp(void)
{
  ErrorStatus status = ERROR;

  /* Wait till HSE is ready and if Time out is reached exit [等待HSE准备好,或超时时间到来]*/
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
* 函数名称: RCC_AdjustHSICalibrationValue
* 功能描述: 校正内部高速振荡器(HSI)标度值.
* 输入参数: HSICalibrationValue:校正标度值，这个参数一定是在0到0x1F之间的值.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_AdjustHSICalibrationValue(u8 HSICalibrationValue)
{
  u32 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_CALIBRATION_VALUE(HSICalibrationValue));

  tmpreg = RCC->CR;

  /* Clear HSITRIM[4:0] bits [清HSITRIM[4:0]位]*/
  tmpreg &= CR_HSITRIM_Mask;

  /* Set the HSITRIM[4:0] bits according to HSICalibrationValue value [依照HSICalibrationValue的值设置HSITRIM[4:0]位]*/
  tmpreg |= (u32)HSICalibrationValue << 3;

  /* Store the new value [保存新的值]*/
  RCC->CR = tmpreg;
}

/*******************************************************************************
* 函数名称: RCC_HSICmd
* 功能描述: 使能内部高速振荡器(HIS).
*                  [HSI如果直接或者通过PLL当作系统时钟的时候是不能被停下来的]
* 输入参数: NewStateHSI的新状态.这个参数可以是:ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_HSICmd(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CR_HSION_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: RCC_PLLConfig
* 功能描述: 配置PLL时钟源和乘法因子.
*                  This function must be used only when the PLL is disabled.
* 输入参数: (1)RCC_PLLSource:PLL进入时钟源.
*                    这个参数可以是下面的值之一:
*                       - RCC_PLLSource_HSI_Div2: PLL时钟输入等于HIS时钟二分频
*                       - RCC_PLLSource_HSE_Div1: PLL时钟输入等于HIS时钟
*                       - RCC_PLLSource_HSE_Div2: PLL时钟输入等于HIS时钟二分频
*           (2)RCC_PLLMul:PLL乘法因子.
*                    这个参数可以是 RCC_PLLMul_x x可以取:[2,16]
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_PLLConfig(u32 RCC_PLLSource, u32 RCC_PLLMul)
{
  u32 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_PLL_SOURCE(RCC_PLLSource));
  assert_param(IS_RCC_PLL_MUL(RCC_PLLMul));

  tmpreg = RCC->CFGR;

  /* Clear PLLSRC, PLLXTPRE and PLLMUL[3:0] bits [清PLLSRC, PLLXTPRE和PLLMUL[3:0]位]*/
  tmpreg &= CFGR_PLL_Mask;

  /* Set the PLL configuration bits [设置PLL 配置位]*/
  tmpreg |= RCC_PLLSource | RCC_PLLMul;

  /* Store the new value [保存新的值]*/
  RCC->CFGR = tmpreg;
}

/*******************************************************************************
* 函数名称: RCC_PLLCmd
* 功能描述: 使能或关闭PLL。
*                  如果PLL被用作系统时钟，它是不可以禁止的。
* 输入参数: NewState:PLL的新状态.这个参数可以是:ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_PLLCmd(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CR_PLLON_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: RCC_SYSCLKConfig
* 功能描述: 配置系统时钟(SYSCLK).
* 输入参数: RCC_SYSCLKSource:作为系统时钟的时钟源.
*                 这个参数可以是下面的值之一:
*                       - RCC_SYSCLKSource_HSI: 选择HSI作为系统时钟
*                       - RCC_SYSCLKSource_HSE: 选择HSE作为系统时钟.
*                       - RCC_SYSCLKSource_PLLCLK: 选择PLL作为系统时钟.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_SYSCLKConfig(u32 RCC_SYSCLKSource)
{
  u32 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_SYSCLK_SOURCE(RCC_SYSCLKSource));

  tmpreg = RCC->CFGR;

  /* Clear SW[1:0] bits [清SW[1:0]位]*/
  tmpreg &= CFGR_SW_Mask;

  /* Set SW[1:0] bits according to RCC_SYSCLKSource value [依照RCC_SYSCLKSource的值设置SW[1:0]位]*/
  tmpreg |= RCC_SYSCLKSource;

  /* Store the new value [保存新的值]*/
  RCC->CFGR = tmpreg;
}

/*******************************************************************************
* 函数名称: RCC_GetSYSCLKSource
* 功能描述: 返回用作系统时钟的时钟源.
* 输入参数: 无
* 输出参数: 无
* 返回参数: 时钟源作为系统时钟使用. 
*              返回值可以是: -0x00:使用HSI作为系统时钟
*                            -0x04:使用HSE作为系统时钟
*                            -0x08:使用PLL作为系统时钟.
*******************************************************************************/
u8 RCC_GetSYSCLKSource(void)
{
  return ((u8)(RCC->CFGR & CFGR_SWS_Mask));
}

/*******************************************************************************
* 函数名称: RCC_HCLKConfig
* 功能描述: 配置AHB时钟(HCLK).
* 输入参数: RCC_HCLK:定义了AHB时钟.这个时钟是从系统时钟(SYSCLK)得到的.
*                    这个参数可以是下面的值之一:
*                       - RCC_SYSCLK_Div1: AHB时钟等于SYSCLK
*                       - RCC_SYSCLK_Div2: AHB时钟等于SYSCLK/2
*                       - RCC_SYSCLK_Div4: AHB时钟等于SYSCLK/4
*                       - RCC_SYSCLK_Div8: AHB时钟等于SYSCLK/8
*                       - RCC_SYSCLK_Div16: AHB时钟等于SYSCLK/16
*                       - RCC_SYSCLK_Div64: AHB时钟等于SYSCLK/64
*                       - RCC_SYSCLK_Div128: AHB时钟等于SYSCLK128
*                       - RCC_SYSCLK_Div256: AHB时钟等于SYSCLK/256
*                       - RCC_SYSCLK_Div512: AHB时钟等于SYSCLK/512
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_HCLKConfig(u32 RCC_SYSCLK)
{
  u32 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_HCLK(RCC_SYSCLK));

  tmpreg = RCC->CFGR;

  /* Clear HPRE[3:0] bits [清HPRE[3:0]位]*/
  tmpreg &= CFGR_HPRE_Reset_Mask;

  /* Set HPRE[3:0] bits according to RCC_SYSCLK value [依照RCC_SYSCLK的值设置HPRE[3:0]位]*/
  tmpreg |= RCC_SYSCLK;

  /* Store the new value [保存新的值]*/
  RCC->CFGR = tmpreg;
}

/*******************************************************************************
* 函数名称: RCC_PCLK1Config
* 功能描述: 配置低速APB时钟(PCLK1).
* 输入参数: RCC_PCLK1:定义APB1时钟.这个时钟是从AHB时钟得到的(HCLK).
*                    这个参数可以是下面的值之一:
*                       - RCC_HCLK_Div1: APB1时钟等于HCLK
*                       - RCC_HCLK_Div2: APB1时钟等于HCLK/2
*                       - RCC_HCLK_Div4: APB1时钟等于HCLK/4
*                       - RCC_HCLK_Div8: APB1时钟等于HCLK/8
*                       - RCC_HCLK_Div16: APB1时钟等于HCLK/16
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_PCLK1Config(u32 RCC_HCLK)
{
  u32 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_PCLK(RCC_HCLK));

  tmpreg = RCC->CFGR;

  /* Clear PPRE1[2:0] bits [清PPRE1[2:0]位]*/
  tmpreg &= CFGR_PPRE1_Reset_Mask;

  /* Set PPRE1[2:0] bits according to RCC_HCLK value [依照RCC_HCLK的值设置PPRE1[2:0]位]*/
  tmpreg |= RCC_HCLK;

  /* Store the new value [保存新的值]*/
  RCC->CFGR = tmpreg;
}

/*******************************************************************************
* 函数名称: RCC_PCLK2Config
* 功能描述: 配置APB时钟(PCLK2).
* 输入参数: RCC_PCLK2:定义APB2时钟.这个时钟是从AHB时钟得到的(HCLK).
*                    这个参数可以是下面的值之一:
*                       - RCC_HCLK_Div1: APB2时钟等于HCLK
*                       - RCC_HCLK_Div2: APB2时钟等于HCLK/2
*                       - RCC_HCLK_Div4: APB2时钟等于HCLK/4
*                       - RCC_HCLK_Div8: APB2时钟等于HCLK/8
*                       - RCC_HCLK_Div16: APB2时钟等于HCLK/16
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_PCLK2Config(u32 RCC_HCLK)
{
  u32 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_PCLK(RCC_HCLK));

  tmpreg = RCC->CFGR;

  /* Clear PPRE2[2:0] bits [清PPRE2[2:0]位]*/
  tmpreg &= CFGR_PPRE2_Reset_Mask;

  /* Set PPRE2[2:0] bits according to RCC_HCLK value [依照RCC_HCLK的值设置PPRE2[2:0]位]*/
  tmpreg |= RCC_HCLK << 3;

  /* Store the new value [保存新的值]*/
  RCC->CFGR = tmpreg;
}

/*******************************************************************************
* 函数名称: RCC_ITConfig
* 功能描述: 使能或关闭特定的RCC中断.
* 输入参数: (1)RCC_IT:指定要使能或要关闭的RCC中断源.
*                    这个参数可以是下面任意值的组合：
*                       - RCC_IT_LSIRDY: LSI就绪中断
*                       - RCC_IT_LSERDY: LSE就绪中断
*                       - RCC_IT_HSIRDY: HSI就绪中断
*                       - RCC_IT_HSERDY: HSE就绪中断
*                       - RCC_IT_PLLRDY: PLL就绪中断
*           (2)NewState:指定的RCC中断的新状态.这个参数可以是:ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_ITConfig(u8 RCC_IT, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_IT(RCC_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Perform Byte access to RCC_CIR[12:8] bits to enable the selected interrupts [执行RCC_CIR[12:8]位的字节存取来使能选择的中断]*/
    *(vu8 *) CIR_BYTE2_ADDRESS |= RCC_IT;
  }
  else
  {
    /* Perform Byte access to RCC_CIR[12:8] bits to disable the selected interrupts [执行RCC_CIR[12:8]位的字节存取来禁止选择的中断]*/
    *(vu8 *) CIR_BYTE2_ADDRESS &= (u8)~RCC_IT;
  }
}

/*******************************************************************************
* 函数名称: RCC_USBCLKConfig
* 功能描述: 配置USB时钟(USBCLK).
* 输入参数: RCC_USBCLKSource:指定USB时钟源,这个时钟来源于PLL输出.
*                    这个参数可以是下面的值之一:
*                       - RCC_USBCLKSource_PLLCLK_1Div5: USB时钟源等于选定的PLL时钟的1.5分频
*                       - RCC_USBCLKSource_PLLCLK_Div1: USB时钟源等于选定的PLL时钟
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_USBCLKConfig(u32 RCC_USBCLKSource)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_USBCLK_SOURCE(RCC_USBCLKSource));

  *(vu32 *) CFGR_USBPRE_BB = RCC_USBCLKSource;
}

/*******************************************************************************
* 函数名称: RCC_ADCCLKConfig
* 功能描述: 配置ADC时钟(ADCCKL).
* 输入参数: RCC_ADCCLK:定义了ADC时钟.这个时钟来自APB2时钟(PCLK2).
*                    这个参数可以是下面的值之一:
*                       - RCC_PCLK2_Div2: ADC时钟等于PCLK2/2
*                       - RCC_PCLK2_Div4: ADC时钟等于PCLK2/4
*                       - RCC_PCLK2_Div6: ADC时钟等于PCLK2/6
*                       - RCC_PCLK2_Div8: ADC时钟等于PCLK2/8
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_ADCCLKConfig(u32 RCC_PCLK2)
{
  u32 tmpreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_ADCCLK(RCC_PCLK2));

  tmpreg = RCC->CFGR;

  /* Clear ADCPRE[1:0] bits [清ADCPRE[1:0]位]*/
  tmpreg &= CFGR_ADCPRE_Reset_Mask;

  /* Set ADCPRE[1:0] bits according to RCC_PCLK2 value [依照RCC_PCLK2的值设置ADCPRE[1:0]位]*/
  tmpreg |= RCC_PCLK2;

  /* Store the new value [保存新的值]*/
  RCC->CFGR = tmpreg;
}

/*******************************************************************************
* 函数名称: RCC_LSEConfig
* 功能描述: 配置外部低速振荡器(LSE).
* 输入参数: RCC_LSE:LSE的新状态。
*                    这个参数可以是下面的值之一:
*                       - RCC_LSE_OFF: LSE振荡器关闭
*                       - RCC_LSE_ON: LSE振荡器开启
*                       - RCC_LSE_Bypass: 带有外部时钟的LSE振荡器
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_LSEConfig(u8 RCC_LSE)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_LSE(RCC_LSE));

  /* Reset LSEON and LSEBYP bits before configuring the LSE [在配置LSE之前复位LSEON和LSEBYP]*/
  /* Reset LSEON bit [复位LSEON]*/
  *(vu8 *) BDCR_ADDRESS = RCC_LSE_OFF;

  /* Reset LSEBYP bit [复位LSEBYP]*/
  *(vu8 *) BDCR_ADDRESS = RCC_LSE_OFF;

  /* Configure LSE (RCC_LSE_OFF is already covered by the code section above) [配置LSE（RCC_LSE_OFF已经被部分代码上隐藏）]*/
  switch(RCC_LSE)
  {
    case RCC_LSE_ON:
      /* Set LSEON bit [置位LSEON]*/
      *(vu8 *) BDCR_ADDRESS = RCC_LSE_ON;
      break;
      
    case RCC_LSE_Bypass:
      /* Set LSEBYP and LSEON bits [置位LSEBYP和LSEON]*/
      *(vu8 *) BDCR_ADDRESS = RCC_LSE_Bypass | RCC_LSE_ON;
      break;            
      
    default:
      break;      
  }
}

/*******************************************************************************
* 函数名称: RCC_LSICmd
* 功能描述: 使能或关闭内部低速振荡器(LSI).
*                  如果IWDG运行,则LSI不能被禁止。
* 输入参数: NewState:LSI的新状态。这个参数的值可以是:ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_LSICmd(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CSR_LSION_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: RCC_RTCCLKConfig
* 功能描述: 配置RTC时钟(RTCCLK).
*                  只要选择了RTC时钟,RTC时钟就不能改变直到备份域复位.
* 输入参数: RCC_RTCCLKSource:RTC时钟源.
*                    这个参数可以是下面的值之一:
*                       - RCC_RTCCLKSource_LSE: 选择LSE作为RTC时钟
*                       - RCC_RTCCLKSource_LSI: 选择LSI作为RTC时钟
*                       - RCC_RTCCLKSource_HSE_Div128: 选择HSE时钟的128分频作为RTC时钟
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_RTCCLKConfig(u32 RCC_RTCCLKSource)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_RTCCLK_SOURCE(RCC_RTCCLKSource));

  /* Select the RTC clock source [选择RTC时钟源]*/
  RCC->BDCR |= RCC_RTCCLKSource;
}

/*******************************************************************************
* 函数名称: RCC_RTCCLKCmd
* 功能描述: 使能或关闭RTC时钟.
*                  仅当使用RCC_RTCCLKConfig函数选择RTC时钟后,才使用这个函数.
* 输入参数: NewState:.RTC时钟的新状态.这个参数可以是:ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_RTCCLKCmd(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) BDCR_RTCEN_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: RCC_GetClocksFreq
* 功能描述: 返回片上时钟的不同频率.
* 输入参数: RCC_Clocks:指向包括时钟频率的RCC_ClocksTypeDef结构的指针。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
  u32 tmp = 0, pllmull = 0, pllsource = 0, presc = 0;

  /* Get SYSCLK source [取得系统时钟源]*/
  tmp = RCC->CFGR & CFGR_SWS_Mask;

  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock [HSI作为系统时钟]*/
      RCC_Clocks->SYSCLK_Frequency = HSI_Value;
      break;

    case 0x04:  /* HSE used as system clock [HSE作为系统时钟]*/
      RCC_Clocks->SYSCLK_Frequency = HSE_Value;
      break;

    case 0x08:  /* PLL used as system clock [PLL作为系统时钟]*/
      /* Get PLL clock source and multiplication factor [取得PLL的时钟源和倍增因数]-*/
      pllmull = RCC->CFGR & CFGR_PLLMull_Mask;
      pllmull = ( pllmull >> 18) + 2;

      pllsource = RCC->CFGR & CFGR_PLLSRC_Mask;

      if (pllsource == 0x00)
      {/* HSI oscillator clock divided by 2 selected as PLL clock entry [选择HSI振荡器时钟除以2作为PLL的时钟输入]*/
        RCC_Clocks->SYSCLK_Frequency = (HSI_Value >> 1) * pllmull;
      }
      else
      {/* HSE selected as PLL clock entry [选择HSE作为PLL的时钟输入]*/

        if ((RCC->CFGR & CFGR_PLLXTPRE_Mask) != (u32)RESET)
        {/* HSE oscillator clock divided by 2 [HSE振荡器时钟除以2]**/

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

  /* Compute HCLK, PCLK1, PCLK2 and ADCCLK clocks frequencies [计算HCLK, PCLK1, PCLK2和ADCCLK时钟频率]*/
  /* Get HCLK prescaler */
  tmp = RCC->CFGR & CFGR_HPRE_Set_Mask;
  tmp = tmp >> 4;
  presc = APBAHBPrescTable[tmp];

  /* HCLK clock frequency [HCLK的时钟频率]*/
  RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;

  /* Get PCLK1 prescaler [计算HCLK1的预分频]*/
  tmp = RCC->CFGR & CFGR_PPRE1_Set_Mask;
  tmp = tmp >> 8;
  presc = APBAHBPrescTable[tmp];

  /* PCLK1 clock frequency [PCLK1的时钟频率]*/
  RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;

  /* Get PCLK2 prescaler [取得PCLK2的预分频]*/
  tmp = RCC->CFGR & CFGR_PPRE2_Set_Mask;
  tmp = tmp >> 11;
  presc = APBAHBPrescTable[tmp];

  /* PCLK2 clock frequency [PCLK2的时钟频率]*/
  RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;

  /* Get ADCCLK prescaler [取得ADCCLK的预分频]*/
  tmp = RCC->CFGR & CFGR_ADCPRE_Set_Mask;
  tmp = tmp >> 14;
  presc = ADCPrescTable[tmp];

  /* ADCCLK clock frequency [ADCCLK的时钟频率]*/
  RCC_Clocks->ADCCLK_Frequency = RCC_Clocks->PCLK2_Frequency / presc;
}

/*******************************************************************************
* 函数名称: RCC_AHBPeriphClockCmd
* 功能描述: 使能或关闭AHB外围设备时钟.
* 输入参数: (1)RCC_AHBPeriph:用于门控时钟的AHB外围设备。
*                    这个参数可以是下面任意值的组合：
*                       - RCC_AHBPeriph_DMA1
*                       - RCC_AHBPeriph_DMA2
*                       - RCC_AHBPeriph_SRAM
*                       - RCC_AHBPeriph_FLITF
*                       - RCC_AHBPeriph_CRC
*                       - RCC_AHBPeriph_FSMC
*                       - RCC_AHBPeriph_SDIO
*                    SRAM 和FLITF 时钟只有在睡眠的模式下可以无效.
*           (2)NewState:指定的外围设备时钟的新状态.这个参数可以是:ENALBLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_AHBPeriphClockCmd(u32 RCC_AHBPeriph, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
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
* 函数名称: RCC_APB2PeriphClockCmd
* 功能描述: 使能或关闭高速APB(APB2)外围设备时钟.
* 输入参数: (1)RCC_AHB2Periph:用于门控时钟的AHB2外围设备。
*                    这个参数可以是下面任意值的组合：
*                       - RCC_APB2Periph_AFIO, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB,
*                         RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOE,
*                         RCC_APB2Periph_GPIOF, RCC_APB2Periph_GPIOG, RCC_APB2Periph_ADC1,
*                         RCC_APB2Periph_ADC2, RCC_APB2Periph_TIM1, RCC_APB2Periph_SPI1,
*                         RCC_APB2Periph_TIM8, RCC_APB2Periph_USART1, RCC_APB2Periph_ADC3,
*                         RCC_APB2Periph_ALL
*           (2)NewState:专用外围设备时钟的新状态.这个参数可以是:ENALBLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_APB2PeriphClockCmd(u32 RCC_APB2Periph, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
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
* 函数名称: RCC_APB1PeriphClockCmd
* 功能描述: 使能或关闭低速APB(APB1)外围设备时钟
* 输入参数: (1)RCC_APB1Periph: 用于门控时钟的APB1外围设备。
*                    这个参数可以是下面任意值的组合：
*                       - RCC_APB1Periph_TIM2, RCC_APB1Periph_TIM3, RCC_APB1Periph_TIM4,
*                         RCC_APB1Periph_TIM5, RCC_APB1Periph_TIM6, RCC_APB1Periph_TIM7,
*                         RCC_APB1Periph_WWDG, RCC_APB1Periph_SPI2, RCC_APB1Periph_SPI3,
*                         RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_USART4, 
*                         RCC_APB1Periph_USART5, RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2,
*                         RCC_APB1Periph_USB, RCC_APB1Periph_CAN, RCC_APB1Periph_BKP,
*                         RCC_APB1Periph_PWR, RCC_APB1Periph_DAC, RCC_APB1Periph_ALL
*           (2)NewState:专用外围设备时钟的新状态.这个参数可以是:ENALBLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_APB1PeriphClockCmd(u32 RCC_APB1Periph, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
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
* 函数名称: RCC_APB2PeriphResetCmd
* 功能描述: 强迫或释放高速APB(APB2)外围设备复位.
* 输入参数: (1)RCC_APB2Periph:复位的APB2外围设备.
*                    这个参数可以是下面任意值的组合：
*                       - RCC_APB2Periph_AFIO, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB,
*                         RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOE,
*                         RCC_APB2Periph_GPIOF, RCC_APB2Periph_GPIOG, RCC_APB2Periph_ADC1,
*                         RCC_APB2Periph_ADC2, RCC_APB2Periph_TIM1, RCC_APB2Periph_SPI1,
*                         RCC_APB2Periph_TIM8, RCC_APB2Periph_USART1, RCC_APB2Periph_ADC3,
*                         RCC_APB2Periph_ALL
*           (2)NewState:指定外围设备复位的新状态.这个参数可以是:ENALBLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_APB2PeriphResetCmd(u32 RCC_APB2Periph, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
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
* 函数名称: RCC_APB1PeriphResetCmd
* 功能描述: 强制或释放低速APB(APB1)外围设备复位.
* 输入参数: (1)RCC_APB1Periph:指定要复位的APB1外围设备.
*                    这个参数可以是下面任意值的组合：
*                       - RCC_APB1Periph_TIM2, RCC_APB1Periph_TIM3, RCC_APB1Periph_TIM4,
*                         RCC_APB1Periph_TIM5, RCC_APB1Periph_TIM6, RCC_APB1Periph_TIM7,
*                         RCC_APB1Periph_WWDG, RCC_APB1Periph_SPI2, RCC_APB1Periph_SPI3,
*                         RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_USART4, 
*                         RCC_APB1Periph_USART5, RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2,
*                         RCC_APB1Periph_USB, RCC_APB1Periph_CAN, RCC_APB1Periph_BKP,
*                         RCC_APB1Periph_PWR, RCC_APB1Periph_DAC, RCC_APB1Periph_ALL
*           (2)NewState:指定外围设备复位的新状态.这个参数可以是:ENALBLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_APB1PeriphResetCmd(u32 RCC_APB1Periph, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
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
* 函数名称: RCC_BackupResetCmd
* 功能描述: 强制或释放备份域复位.
* 输入参数: NewState:备份域复位的新状态.这个参数可以是:ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_BackupResetCmd(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) BDCR_BDRST_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: RCC_ClockSecuritySystemCmd
* 功能描述: 使能或关闭时钟安全系统
* 输入参数: NewState:时钟安全系统的新状态.这个参数可以是:ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_ClockSecuritySystemCmd(FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(vu32 *) CR_CSSON_BB = (u32)NewState;
}

/*******************************************************************************
* 函数名称: RCC_MCOConfig
* 功能描述: 选择MCO引脚上的输出时钟源
* 输入参数: RCC_MCO:详细说明要输出的时钟源.
*                    这个参数可以是下面的值之一:
*                       - RCC_MCO_NoClock: 没有选择时钟
*                       - RCC_MCO_SYSCLK: 选择系统时钟
*                       - RCC_MCO_HSI: 选择HIS振荡时钟
*                       - RCC_MCO_HSE: 选择HSE振荡时钟
*                       - RCC_MCO_PLLCLK_Div2:选择PLL时钟两分频
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_MCOConfig(u8 RCC_MCO)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_MCO(RCC_MCO));

  /* Perform Byte access to MCO[2:0] bits to select the MCO source [执行MCO[2:0]位的字节存取来选择MCO源]*/
  *(vu8 *) CFGR_BYTE4_ADDRESS = RCC_MCO;
}

/*******************************************************************************
* 函数名称: RCC_GetFlagStatus
* 功能描述: 检查指定的RCC标志设置与否.
* 输入参数: RCC_FLAG:要检查的标志.
*                    这个参数可以是下面的值之一:
*                       - RCC_FLAG_HSIRDY: HIS振荡时钟就绪
*                       - RCC_FLAG_HSERDY: HSE振荡时钟就绪
*                       - RCC_FLAG_PLLRDY: PLL时钟就绪
*                       - RCC_FLAG_LSERDY: LSE振荡时钟就绪
*                       - RCC_FLAG_LSIRDY: LSI振荡时钟就绪
*                       - RCC_FLAG_PINRST: 引脚复位
*                       - RCC_FLAG_PORRST: POR/PDR复位
*                       - RCC_FLAG_SFTRST: 软件复位
*                       - RCC_FLAG_IWDGRST: 独立看门狗复位
*                       - RCC_FLAG_WWDGRST: 窗口看门狗复位
*                       - RCC_FLAG_LPWRRST: 低电量复位
* 输出参数: 无
* 返回参数: RCC_FLAG的新状态(SET或RESET).
*******************************************************************************/
FlagStatus RCC_GetFlagStatus(u8 RCC_FLAG)
{
  u32 tmp = 0;
  u32 statusreg = 0;
  FlagStatus bitstatus = RESET;

  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_FLAG(RCC_FLAG));

  /* Get the RCC register index [取得RCC寄存器引索]*/
  tmp = RCC_FLAG >> 5;

  if (tmp == 1)               /* The flag to check is in CR register [CR寄存器中的检查标志]*/
  {
    statusreg = RCC->CR;
  }
  else if (tmp == 2)          /* The flag to check is in BDCR register [BDCR寄存器中的检查标志]*/
  {
    statusreg = RCC->BDCR;
  }
  else                       /* The flag to check is in CSR register [CSR寄存器中的检查标志]*/
  {
    statusreg = RCC->CSR;
  }

  /* Get the flag position [取得标志的位置]*/
  tmp = RCC_FLAG & FLAG_Mask;

  if ((statusreg & ((u32)1 << tmp)) != (u32)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* Return the flag status [返回标志的状态]*/
  return bitstatus;
}

/*******************************************************************************
* 函数名称: RCC_ClearFlag
* 功能描述: 清除RCC复位标志.
*           复位标志为: RCC_FLAG_PINRST, RCC_FLAG_PORRST,
*                  RCC_FLAG_SFTRST, RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST,
*                  RCC_FLAG_LPWRRST
* 输入参数: 无
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_ClearFlag(void)
{
  /* Set RMVF bit to clear the reset flags [通过置位RMVF位来清除复位标志]*/
  RCC->CSR |= CSR_RMVF_Set;
}

/*******************************************************************************
* 函数名称: RCC_GetITStatus
* 功能描述: 检查指定的RCC中断是否发生.
* 输入参数: RCC_IT:检查RCC中断源.
*                    这个参数可以是下面的值之一:
*                       - RCC_IT_LSIRDY: LSI就绪中断
*                       - RCC_IT_LSERDY: LSE就绪中断
*                       - RCC_IT_HSIRDY: HIS就绪中断
*                       - RCC_IT_HSERDY: HSE就绪中断
*                       - RCC_IT_PLLRDY: PLL就绪中断
*                       - RCC_IT_CSS: 时钟安全系统中断
* 输出参数: 无
* 返回参数: RCC_IT的新状态(SETorRESET).
*******************************************************************************/
ITStatus RCC_GetITStatus(u8 RCC_IT)
{
  ITStatus bitstatus = RESET;

  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_GET_IT(RCC_IT));

  /* Check the status of the specified RCC interrupt [检查指定的RCC中断状态]*/
  if ((RCC->CIR & RCC_IT) != (u32)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* Return the RCC_IT status [返回RCC_IT状态]*/
  return  bitstatus;
}

/*******************************************************************************
* 函数名称: RCC_ClearITPendingBit
* 功能描述: 清除RCC中断挂起位
* 输入参数: RCC_IT:指定要清除的中断挂起位..
*                    这个参数可以是下面任意值的组合：
*                       - RCC_IT_LSIRDY: LSI就绪中断
*                       - RCC_IT_LSERDY: LSE就绪中断
*                       - RCC_IT_HSIRDY: HIS就绪中断
*                       - RCC_IT_HSERDY: HSE就绪中断
*                       - RCC_IT_PLLRDY: PLL就绪中断
*                       - RCC_IT_CSS: 时钟安全系统中断
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void RCC_ClearITPendingBit(u8 RCC_IT)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_RCC_CLEAR_IT(RCC_IT));

  /* Perform Byte access to RCC_CIR[23:16] bits to clear the selected interrupt
     pending bits [执行RCC_CIR[23:16]位的字节存取来清除选择的中断挂起位]*/
  *(vu8 *) CIR_BYTE3_ADDRESS = RCC_IT;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
