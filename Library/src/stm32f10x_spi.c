/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* 文件名称: stm32f10x_spi.c
* 程序作者: MCD Application Team
* 程序版本: V2.0.2
* 编制日期: 07/11/2008
* 功能描述: 这个文件提供了所有SPI的固件函数.
********************************************************************************
* 目前的固件的目的是为客户提供关于他们的产品的编码信息以便于节省他们时间。
* 结果, 意法半导体将不会为此承担任何直接，间接或相应的损害赔偿方面的任何索赔要求，
* 例如固件或使用所作的客户编码中他涉嫌包含与他们的相关产品的信息。
*******************************************************************************/

/* 包涵的文件 ------------------------------------------------------------------*/
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"

/* 自用类型 -----------------------------------------------------------*/
/* 自用定义 ------------------------------------------------------------*/
/* SPI SPE mask */
#define CR1_SPE_Set          ((u16)0x0040)
#define CR1_SPE_Reset        ((u16)0xFFBF)

/* I2S I2SE mask */
#define I2SCFGR_I2SE_Set     ((u16)0x0400)
#define I2SCFGR_I2SE_Reset   ((u16)0xFBFF)

/* SPI CRCNext mask */
#define CR1_CRCNext_Set      ((u16)0x1000)

/* SPI CRCEN mask */
#define CR1_CRCEN_Set        ((u16)0x2000)
#define CR1_CRCEN_Reset      ((u16)0xDFFF)

/* SPI SSOE mask */
#define CR2_SSOE_Set         ((u16)0x0004)
#define CR2_SSOE_Reset       ((u16)0xFFFB)

/* SPI registers Masks */
#define CR1_CLEAR_Mask       ((u16)0x3040)
#define I2SCFGR_CLEAR_Mask   ((u16)0xF040)

/* SPI or I2S mode selection masks */
#define SPI_Mode_Select      ((u16)0xF7FF)
#define I2S_Mode_Select      ((u16)0x0800) 

/* 自用宏 -------------------------------------------------------------*/
/* 自用变量 ---------------------------------------------------------*/
/* 自用函数原型 -----------------------------------------------*/
/* 自用函数 ---------------------------------------------------------*/

/*******************************************************************************
* 函数名称: SPI_I2S_DeInit
* 功能描述: 将SPIx外设寄存器重置为他们的缺省值(Affects also the I2Ss).
* 输入参数: SPIx x为1，2或3用于选定SPI外设。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SPI_I2S_DeInit(SPI_TypeDef* SPIx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  switch (*(u32*)&SPIx)
  {
    case SPI1_BASE:
      /* Enable SPI1 reset state [使能SPI1复位状态]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);
      /* Release SPI1 from reset state [解除SPI1复位状态]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);
      break;

    case SPI2_BASE:
      /* Enable SPI2 reset state [使能SPI2复位状态]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);
      /* Release SPI2 from reset state [解除SPI2复位状态]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);
      break;

    case SPI3_BASE:
      /* Enable SPI3 reset state [使能SPI3复位状态]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);
      /* Release SPI3 from reset state [解除SPI3复位状态]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);
      break;

    default:
      break;
  }
}

/*******************************************************************************
* 函数名称: SPI_Init
* 功能描述: 根据SPI_InitStruct.中的特定参数初始化SPIx外设.
* 输入参数: (1)SPIx ：x为1，2或3用于选定SPI外设。
*           (2)SPI_InitStruct：指向一个包含特定SPI外设配置信息的SPI_InitTypeDef结构体的指针。
* 输出参数: 无
* 返回参数: 无
******************************************************************************/
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{
  u16 tmpreg = 0;
  
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));   
  
  /* Check the SPI parameters [检查SPI的参数]*/
  assert_param(IS_SPI_DIRECTION_MODE(SPI_InitStruct->SPI_Direction));
  assert_param(IS_SPI_MODE(SPI_InitStruct->SPI_Mode));
  assert_param(IS_SPI_DATASIZE(SPI_InitStruct->SPI_DataSize));
  assert_param(IS_SPI_CPOL(SPI_InitStruct->SPI_CPOL));
  assert_param(IS_SPI_CPHA(SPI_InitStruct->SPI_CPHA));
  assert_param(IS_SPI_NSS(SPI_InitStruct->SPI_NSS));
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_InitStruct->SPI_BaudRatePrescaler));
  assert_param(IS_SPI_FIRST_BIT(SPI_InitStruct->SPI_FirstBit));
  assert_param(IS_SPI_CRC_POLYNOMIAL(SPI_InitStruct->SPI_CRCPolynomial));

/*---------------------------- SPIx CR1 Configuration [SPIx CR1配置]*/
  /* Get the SPIx CR1 value [取得SPIx CR1的值]*/
  tmpreg = SPIx->CR1;
  /* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL and CPHA bits 
  清BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL 和CPHA 位*/
  tmpreg &= CR1_CLEAR_Mask;
  /* Configure SPIx[配置SPIx]: direction[方向], NSS management[NSS管理], first transmitted bit[第一个发送位], BaudRate prescaler[波特率预分频]
     master/salve mode[主/从模式], CPOL and CPHA */
  /* Set BIDImode, BIDIOE and RxONLY bits according to SPI_Direction value [依照SPI_Direction的值置位BIDImode, BIDIOE和RxONLY]*/
  /* Set SSM, SSI and MSTR bits according to SPI_Mode and SPI_NSS values [依照SPI_Mode 和SPI_NSS的值置位SSM, SSI 和 MSTR]*/
  /* Set LSBFirst bit according to SPI_FirstBit value [依照SPI_FirstBit的值置位LSBFirst]*/
  /* Set BR bits according to SPI_BaudRatePrescaler value [依照SPI_BaudRatePrescaler的值置位BR]*/
  /* Set CPOL bit according to SPI_CPOL value [依照SPI_CPOL的值置位CPOL]*/
  /* Set CPHA bit according to SPI_CPHA value [依照SPI_CPHA的值置位CPHA]*/
  tmpreg |= (u16)((u32)SPI_InitStruct->SPI_Direction | SPI_InitStruct->SPI_Mode |
                  SPI_InitStruct->SPI_DataSize | SPI_InitStruct->SPI_CPOL |  
                  SPI_InitStruct->SPI_CPHA | SPI_InitStruct->SPI_NSS |  
                  SPI_InitStruct->SPI_BaudRatePrescaler | SPI_InitStruct->SPI_FirstBit);
  /* Write to SPIx CR1 [写SPIx CR1]*/
  SPIx->CR1 = tmpreg;
  
  /* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
  SPIx->I2SCFGR &= SPI_Mode_Select;     

/*---------------------------- SPIx CRCPOLY Configuration --------------------*/
  /* Write to SPIx CRCPOLY [写SPIx CRCPOLY]*/
  SPIx->CRCPR = SPI_InitStruct->SPI_CRCPolynomial;
}

/*******************************************************************************
* 函数名称: I2S_Init
* 功能描述: 根据SPI_InitStruct.中的特定参数初始化SPIx外设.
* 输入参数: (1)SPIx ：x为1，2或3用于选定SPI外设(配置I2S 模式)。
*           (2)I2S_InitStruct：指向一个在模式I2S包含特定SPI外设配置信息的SPI_InitTypeDef结构体的指针。
* 输出参数: 无
* 返回参数: 无
******************************************************************************/
void I2S_Init(SPI_TypeDef* SPIx, I2S_InitTypeDef* I2S_InitStruct)
{
  u16 tmpreg = 0, i2sdiv = 2, i2sodd = 0, packetlength = 1;
  u32 tmp = 0;
  RCC_ClocksTypeDef RCC_Clocks;
   
  /* Check the I2S parameters [检查I2S参数]*/
  assert_param(IS_SPI_23_PERIPH(SPIx));
  assert_param(IS_I2S_MODE(I2S_InitStruct->I2S_Mode));
  assert_param(IS_I2S_STANDARD(I2S_InitStruct->I2S_Standard));
  assert_param(IS_I2S_DATA_FORMAT(I2S_InitStruct->I2S_DataFormat));
  assert_param(IS_I2S_MCLK_OUTPUT(I2S_InitStruct->I2S_MCLKOutput));
  assert_param(IS_I2S_AUDIO_FREQ(I2S_InitStruct->I2S_AudioFreq));
  assert_param(IS_I2S_CPOL(I2S_InitStruct->I2S_CPOL));  

/*----------------------- SPIx I2SCFGR & I2SPR Configuration -----------------*/

  /* Clear I2SMOD, I2SE, I2SCFG, PCMSYNC, I2SSTD, CKPOL, DATLEN and CHLEN bits
  [清I2SMOD, I2SE, I2SCFG, PCMSYNC, I2SSTD, CKPOL, DATLEN 和CHLEN标志] */
  SPIx->I2SCFGR &= I2SCFGR_CLEAR_Mask; 
  SPIx->I2SPR = 0x0002;
  
  /* Get the I2SCFGR register value [取得I2SCFGR值]*/
  tmpreg = SPIx->I2SCFGR;
  
  /* If the default value has to be written, reinitialize i2sdiv and i2sodd[如果默认值已经写了，重新初始化i2sdiv和i2sodd]*/
  if(I2S_InitStruct->I2S_AudioFreq == I2S_AudioFreq_Default)
  {
    i2sodd = (u16)0;
    i2sdiv = (u16)2;   
  }
  /* If the requested audio frequency is not the default, compute the prescaler [如果请求的的音频不是默认值，计算预分频]*/
  else
  {
    /* Check the frame length (For the Prescaler computing) [检查祯长(计算预分频器)]*/
    if(I2S_InitStruct->I2S_DataFormat == I2S_DataFormat_16b)
    {
      /* Packet length is 16 bits [包的长度16位]*/
      packetlength = 1;
    }
    else
    {
      /* Packet length is 32 bits [包的长度32位]*/
      packetlength = 2;
    }
    /* Get System Clock frequency [取得系统时钟频率]*/
    RCC_GetClocksFreq(&RCC_Clocks);
    
    /* Compute the Real divider depending on the MCLK output state with a flaoting point 
    用浮点指针根据MCLK输出状态计算真实分割器*/
    if(I2S_InitStruct->I2S_MCLKOutput == I2S_MCLKOutput_Enable)
    {
      /* MCLK output is enabled [MCLK输出使能]*/
      tmp = (u16)(((10 * RCC_Clocks.SYSCLK_Frequency) / (256 * I2S_InitStruct->I2S_AudioFreq)) + 5);
    }
    else
    {
      /* MCLK output is disabled [MCLK输出禁止]*/
      tmp = (u16)(((10 * RCC_Clocks.SYSCLK_Frequency) / (32 * packetlength * I2S_InitStruct->I2S_AudioFreq)) + 5);
    }
    
    /* Remove the flaoting point [删除浮点指针]*/
    tmp = tmp/10;  
      
    /* Check the parity of the divider [检查除数的奇偶]*/
    i2sodd = (u16)(tmp & (u16)0x0001);
   
    /* Compute the i2sdiv prescaler [计算i2sdiv预分频器]*/
    i2sdiv = (u16)((tmp - i2sodd) / 2);
   
    /* Get the Mask for the Odd bit (SPI_I2SPR[8]) register [取得(SPI_I2SPR[8])寄存器旧的标志位]*/
    i2sodd = (u16) (i2sodd << 8);
  }
  
  /* Test if the divider is 1 or 0 [测试除数是1还是0]*/
  if ((i2sdiv < 2) || (i2sdiv > 0xFF))
  {
    /* Set the default values [设置默认值]*/
    i2sdiv = 2;
    i2sodd = 0;
  }

  /* Write to SPIx I2SPR register the computed value [向SPIx I2SPR寄存器写入计算值]*/
  SPIx->I2SPR = (u16)(i2sdiv | i2sodd | I2S_InitStruct->I2S_MCLKOutput);  
 
  /* Configure the I2S with the SPI_InitStruct values [根据SPI_InitStruct值配置I2S]*/
  tmpreg |= (u16)(I2S_Mode_Select | I2S_InitStruct->I2S_Mode | \
                  I2S_InitStruct->I2S_Standard | I2S_InitStruct->I2S_DataFormat | \
                  I2S_InitStruct->I2S_CPOL);
 
  /* Write to SPIx I2SCFGR [写SPIx I2SCFGR]*/  
  SPIx->I2SCFGR = tmpreg;                                    
}

/*******************************************************************************
* 函数名称: SPI_StructInit
* 功能描述: 使用缺省值填充SPI_InitStruct每一个成员.
* 输入参数: SPI_InitStruct:指向一个将被初始化的SPI_InitTypeDef结构的指针.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
{
/*--------------- Reset SPI init structure parameters values [复位SPI初始化结构参数值]*/
  /* Initialize the SPI_Direction member [初始化SPI_Direction成员]*/
  SPI_InitStruct->SPI_Direction = SPI_Direction_2Lines_FullDuplex;

  /* initialize the SPI_Mode member [初始化SPI_Mode员]*/
  SPI_InitStruct->SPI_Mode = SPI_Mode_Slave;

  /* initialize the SPI_DataSize member [初始化SPI_DataSize成员]*/
  SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b;

  /* Initialize the SPI_CPOL member [初始化SPI_CPOL成员]*/
  SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;

  /* Initialize the SPI_CPHA member [初始化SPI_CPHA成员]*/
  SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;

  /* Initialize the SPI_NSS member [初始化SPI_NSS成员]*/
  SPI_InitStruct->SPI_NSS = SPI_NSS_Hard;

  /* Initialize the SPI_BaudRatePrescaler member [初始化SPI_BaudRatePrescaler成员]*/
  SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;

  /* Initialize the SPI_FirstBit member [初始化SPI_FirstBit成员]*/
  SPI_InitStruct->SPI_FirstBit = SPI_FirstBit_MSB;

  /* Initialize the SPI_CRCPolynomial member [初始化SPI_CRCPolynomial成员]*/
  SPI_InitStruct->SPI_CRCPolynomial = 7;
}

/*******************************************************************************
* 函数名称: I2S_StructInit
* 功能描述: 用默认值填充每个I2S_InitStruct结构成员.
* 输入参数: I2S_InitStruct : 指向将要被初始化的I2S_InitTypeDef 结构指针.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2S_StructInit(I2S_InitTypeDef* I2S_InitStruct)
{
/*--------------- Reset I2S init structure parameters values [复位I2S初始化结构参数值]*/
  /* Initialize the I2S_Mode member [初始化I2S_Mode成员]*/
  I2S_InitStruct->I2S_Mode = I2S_Mode_SlaveTx;
  
  /* Initialize the I2S_Standard member [初始化I2S_Standard成员]*/
  I2S_InitStruct->I2S_Standard = I2S_Standard_Phillips;
  
  /* Initialize the I2S_DataFormat member [初始化I2S_DataFormat成员]*/
  I2S_InitStruct->I2S_DataFormat = I2S_DataFormat_16b;
  
  /* Initialize the I2S_MCLKOutput member [初始化I2S_MCLKOutput成员]*/
  I2S_InitStruct->I2S_MCLKOutput = I2S_MCLKOutput_Disable;
  
  /* Initialize the I2S_AudioFreq member [初始化I2S_AudioFreq成员]*/
  I2S_InitStruct->I2S_AudioFreq = I2S_AudioFreq_Default;
  
  /* Initialize the I2S_CPOL member [初始化I2S_CPOL成员]*/
  I2S_InitStruct->I2S_CPOL = I2S_CPOL_Low;
}

/*******************************************************************************
* 函数名称: SPI_Cmd
* 功能描述: 使能或关闭特定的外设.
* 输入参数: (1)SPIx ：x为1，2或3用于选定SPI外设。
*           (2)NewState:这个SPI外设的新状态，这个参数可能是ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected SPI peripheral [使能选择的SPI外设]*/
    SPIx->CR1 |= CR1_SPE_Set;
  }
  else
  {
    /* Disable the selected SPI peripheral [禁止选择的SPI外设]*/
    SPIx->CR1 &= CR1_SPE_Reset;
  }
}

/*******************************************************************************
* 函数名称: I2S_Cmd
* 功能描述: 使能或禁止指定的SPI外设(在 I2S 模式).
* 输入参数: (1)SPIx ：x为1，2或3用于选定SPI外设。
*           (2)NewState: 新的SPIx外设状态. 
*                    这个参数可以是: ENABLE or DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void I2S_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_23_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected SPI peripheral (in I2S mode) [使能选择的外设SPI(在I2S 模式)]*/
    SPIx->I2SCFGR |= I2SCFGR_I2SE_Set;
  }
  else
  {
    /* Disable the selected SPI peripheral (in I2S mode) [禁止选择的外设SPI(在I2S 模式)]*/
    SPIx->I2SCFGR &= I2SCFGR_I2SE_Reset;
  }
}

/*******************************************************************************
* 函数名称: SPI_I2S_ITConfig
* 功能描述: 使能或禁止指定的SPI/I2S中断。
* 输入参数: (1)SPIx: x可以是 :
*                         - 1, 2 或 3 在 SPI 模式 
*                         - 2 或 3 在 I2S 模式
*           (2)SPI_I2S_IT: 指定的中断源被使能或禁止. 
*                    这个参数可以是下面的值之一:
*                       - SPI_I2S_IT_TXE: Tx buffer empty interrupt mask
*                       - SPI_I2S_IT_RXNE: Rx buffer not empty interrupt mask
*                       - SPI_I2S_IT_ERR: Error interrupt mask
*           (3)NewState: 指定的 SPI/I2S 中断新状态.
*                    这个参数可以是: ENABLE or DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SPI_I2S_ITConfig(SPI_TypeDef* SPIx, u8 SPI_I2S_IT, FunctionalState NewState)
{
  u16 itpos = 0, itmask = 0 ;

  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_SPI_I2S_CONFIG_IT(SPI_I2S_IT));

  /* Get the SPI/I2S IT index */
  itpos = SPI_I2S_IT >> 4;
  /* Set the IT mask */
  itmask = (u16)((u16)1 << itpos);

  if (NewState != DISABLE)
  {
    /* Enable the selected SPI/I2S interrupt [使能选择的SPI/I2S 中断]*/
    SPIx->CR2 |= itmask;
  }
  else
  {
    /* Disable the selected SPI/I2S interrupt [禁止选择的SPI/I2S 中断]*/
    SPIx->CR2 &= (u16)~itmask;
  }
}

/*******************************************************************************
* 函数名称: SPI_I2S_DMACmd
* 功能描述: 使能或禁止SPIx/I2Sx DMA接口.
* 输入参数: (1)SPIx: x可以是 :
*                         - 1, 2 或 3 在 SPI 模式 
*                         - 2 或 3 在 I2S 模式
*           (2)SPI_I2S_DMAReq: 指定的 SPI/I2S DMA 传输请求被使能或禁止。
*                    这个参数可以是下面任意值的组合:
*                       - SPI_I2S_DMAReq_Tx: 发送缓冲 DMA传输请求
*                       - SPI_I2S_DMAReq_Rx:  接收缓冲DMA 传输请求
*           (3)NewState:选择的 SPI/I2S DMA传输请求新状态 
*                    这个参数可以是: ENABLE 或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SPI_I2S_DMACmd(SPI_TypeDef* SPIx, u16 SPI_I2S_DMAReq, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_SPI_I2S_DMAREQ(SPI_I2S_DMAReq));

  if (NewState != DISABLE)
  {
    /* Enable the selected SPI/I2S DMA requests [使能选择的SPI/I2S DMA请求]*/
    SPIx->CR2 |= SPI_I2S_DMAReq;
  }
  else
  {
    /* Disable the selected SPI/I2S DMA requests [禁止选择的SPI/I2S DMA请求]*/
    SPIx->CR2 &= (u16)~SPI_I2S_DMAReq;
  }
}

/*******************************************************************************
* 函数名称: SPI_I2S_SendData
* 功能描述: 通过SPIx/I2Sx外设发送数据
* 输入参数: (1)SPIx: x可以是 :
*                         - 1, 2 或 3 在 SPI 模式 
*                         - 2 或 3 在 I2S 模式
*           (2)Data : 发送的数据
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SPI_I2S_SendData(SPI_TypeDef* SPIx, u16 Data)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* Write in the DR register the data to be sent [把准备发送的数据写到DR寄存器]*/
  SPIx->DR = Data;
}

/*******************************************************************************
* 函数名称: SPI_I2S_ReceiveData
* 功能描述: 返回最近从SPIx/I2Sx外部设备接收的数据。
* 输入参数: (1)- SPIx: x可以是 :
*                         - 1, 2 或 3 在 SPI 模式 
*                         - 2 或 3 在 I2S 模式
* 输出参数: 无
* 返回参数: 接收的数据值.
*******************************************************************************/
u16 SPI_I2S_ReceiveData(SPI_TypeDef* SPIx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* Return the data in the DR register [返回DR寄存器的数据]*/
  return SPIx->DR;
}

/*******************************************************************************
* 函数名称: SPI_NSSInternalSoftwareConfig
* 功能描述: 软件配置选择的SPI内部引脚.
* 输入参数: (1)SPIx ：x为1，2或3用于选定SPI外设。
*           (2)SPI_NSSInternalSoft: 指定的SPI NSS内部状态.
*                    这个参数可以是下面的值之一:
*                       - SPI_NSSInternalSoft_Set: 置位NSS内部引脚
*                       - SPI_NSSInternalSoft_Reset: 复位NSS内部引脚
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SPI_NSSInternalSoftwareConfig(SPI_TypeDef* SPIx, u16 SPI_NSSInternalSoft)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_NSS_INTERNAL(SPI_NSSInternalSoft));

  if (SPI_NSSInternalSoft != SPI_NSSInternalSoft_Reset)
  {
    /* Set NSS pin internally by software [软件置位NSS内部引脚]*/
    SPIx->CR1 |= SPI_NSSInternalSoft_Set;
  }
  else
  {
    /* Reset NSS pin internally by software [软件复位NSS内部引脚]*/
    SPIx->CR1 &= SPI_NSSInternalSoft_Reset;
  }
}

/*******************************************************************************
* 函数名称: SPI_SSOutputCmd
* 功能描述: 使能或关闭选定的SPI接口的SS输出.
* 输入参数: (1)SPIx ：x为1，2或3用于选定SPI外设。
*           (2)NewState:选定的SPI SS输出的新状态，这个参数可能是ENABLE或DISABLE
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SPI_SSOutputCmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected SPI SS output [使能选择的SPI SS输出]*/
    SPIx->CR2 |= CR2_SSOE_Set;
  }
  else
  {
    /* Disable the selected SPI SS output [禁止选择的SPI SS输出]*/
    SPIx->CR2 &= CR2_SSOE_Reset;
  }
}

/*******************************************************************************
* 函数名称: SPI_DataSizeConfig
* 功能描述: 为选定的SPI接口配置数据大小.
* 输入参数: (1)SPIx ：x为1，2或3用于选定SPI外设。
*           (2)SPI_DataSize: SPI数据大小。
*                    这个参数可以是下面的值之一:
*                       - SPI_DataSize_16b: 设置数据大小为16位
*                       - SPI_DataSize_8b: 设置数据大小为8位
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SPI_DataSizeConfig(SPI_TypeDef* SPIx, u16 SPI_DataSize)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_DATASIZE(SPI_DataSize));

  /* Clear DFF bit [清除DFF位]*/
  SPIx->CR1 &= (u16)~SPI_DataSize_16b;
  /* Set new DFF bit value [设置DFF位的新值]*/
  SPIx->CR1 |= SPI_DataSize;
}

/*******************************************************************************
* 函数名称: SPI_TransmitCRC
* 功能描述: 传送某个SPI的CRC校验值
* 输入参数: SPIx ：x为1，2或3用于选定SPI外设。
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SPI_TransmitCRC(SPI_TypeDef* SPIx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* Enable the selected SPI CRC transmission [使能选中的SPI CRC发送]*/
  SPIx->CR1 |= CR1_CRCNext_Set;
}

/*******************************************************************************
* 函数名称: SPI_CalculateCRC
* 功能描述: 使能或取消传送字节的CRC校验值的计算.
* 输入参数: (1)SPIx ：x为1，2或3用于选定SPI外设。
*           (2)NewState: 选定SPI接口的CRC计算的新状态，这个参数可能是ENABLE或DISABLE.
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SPI_CalculateCRC(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected SPI CRC calculation [使能选中的SPI CRC计算]*/
    SPIx->CR1 |= CR1_CRCEN_Set;
  }
  else
  {
    /* Disable the selected SPI CRC calculation [禁止选中的SPI CRC计算]*/
    SPIx->CR1 &= CR1_CRCEN_Reset;
  }
}

/*******************************************************************************
* 函数名称: SPI_GetCRC
* 功能描述: 返回特定SPI外设传送或接收的CRC寄存器的值.
* 输入参数: (1)SPIx ：x为1，2或3用于选定SPI外设。
*           (2)SPI_CRC:将被读取的CRC寄存器。
*                    这个参数可以是下面的值之一:
*                       - SPI_CRC_Tx: 选择Tx CRC寄存器
*                       - SPI_CRC_Rx: 选择Rx CRC寄存器
* 输出参数: 无
* 返回参数: 所选择的CRC寄存器的值。
*******************************************************************************/
u16 SPI_GetCRC(SPI_TypeDef* SPIx, u8 SPI_CRC)
{
  u16 crcreg = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_CRC(SPI_CRC));

  if (SPI_CRC != SPI_CRC_Rx)
  {
    /* Get the Tx CRC register [取得Tx CRC寄存器]*/
    crcreg = SPIx->TXCRCR;
  }
  else
  {
    /* Get the Rx CRC register [取得Rx CRC寄存器]*/
    crcreg = SPIx->RXCRCR;
  }

  /* Return the selected CRC register [返回选择的CRC寄存器]*/
  return crcreg;
}

/*******************************************************************************
* 函数名称: SPI_GetCRCPolynomial
* 功能描述: 返回特定SPI接口的CRC多项式寄存器的值.
* 输入参数: SPIx ：x为1，2或3用于选定SPI外设。
* 输出参数: 无
* 返回参数: CRC多项式寄存器的值.
*******************************************************************************/
u16 SPI_GetCRCPolynomial(SPI_TypeDef* SPIx)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* Return the CRC polynomial register [返回CRC多项式寄存器]*/
  return SPIx->CRCPR;
}

/*******************************************************************************
* 函数名称: SPI_BiDirectionalLineConfig
* 功能描述: 为特定的SPI接口在双向模式时选择数据传输方向.
* 输入参数: (1)SPIx ：x为1，2或3用于选定SPI外设。
*           (2)SPI_Direction: 在双向模式中的数据传输方向。 
*                    这个参数可以是下面的值之一:
*                       - SPI_Direction_Tx: 选择Tx传送方向
*                       - SPI_Direction_Rx: 选择Rx接收方向
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SPI_BiDirectionalLineConfig(SPI_TypeDef* SPIx, u16 SPI_Direction)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_DIRECTION(SPI_Direction));

  if (SPI_Direction == SPI_Direction_Tx)
  {
    /* Set the Tx only mode [设置只发送模式]*/
    SPIx->CR1 |= SPI_Direction_Tx;
  }
  else
  {
    /* Set the Rx only mode [设置只接收模式]*/
    SPIx->CR1 &= SPI_Direction_Rx;
  }
}

/*******************************************************************************
* 函数名称: SPI_I2S_GetFlagStatus
* 功能描述: 检查指定的SPI/I2S标记是否被置位.
* 输入参数: (1)- SPIx: x可以是 :
*                         - 1, 2 或 3 在 SPI 模式 
*                         - 2 或 3 在 I2S 模式
*           (2)检查指定的SPI/I2S标记是否被设置. 
*                    这个参数可以是下面的值之一:
*                       - SPI_I2S_FLAG_TXE: 传输缓冲为空标记.
*                       - SPI_I2S_FLAG_RXNE: 接收缓冲不空标记.
*                       - SPI_I2S_FLAG_BSY: 忙碌标记.
*                       - SPI_I2S_FLAG_OVR: 溢出标记.
*                       - SPI_FLAG_MODF: 模式错误标记.
*                       - SPI_FLAG_CRCERR: CRC校验错误标记.
*                       - I2S_FLAG_UDR: 空栈读出错标记. 
*                       - I2S_FLAG_CHSIDE: 通道边标志.
* 输出参数: 无
* 返回参数: SPI_I2S_FLAG标记的新状态  (SET or RESET).
*******************************************************************************/
FlagStatus SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, u16 SPI_I2S_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_GET_FLAG(SPI_I2S_FLAG));

  /* Check the status of the specified SPI/I2S flag [检查指定的SPI/I2S标志状态]*/
  if ((SPIx->SR & SPI_I2S_FLAG) != (u16)RESET)
  {
    /* SPI_I2S_FLAG is set [置位SPI_I2S_FLAG]*/
    bitstatus = SET;
  }
  else
  {
    /* SPI_I2S_FLAG is reset [复位SPI_I2S_FLAG]*/
    bitstatus = RESET;
  }
  /* Return the SPI_I2S_FLAG status [返回SPI_I2S_FLAG状态]*/
  return  bitstatus;
}

/*******************************************************************************
* 函数名称: SPI_I2S_ClearFlag
* 功能描述: 清除SPIx CRC错误(CRCERR) 标志.
* 输入参数: (1)SPIx: x可以是 :
*                         - 1, 2 或 3 在 SPI 模式 
*           (2)SPI_I2S_FLAG: 指定清除的SPI标志. 
*                    这个函数只能清除CRCERR标志.                                           
*                  注意:
*                       - OVR (过速错误)中断挂起位可以被软件按顺序清除: 一个读  
*                         SPI_DR寄存器操作 (SPI_I2S_ReceiveData()) 跟着一个读  
*                         SPI_SR寄存器操作 (SPI_I2S_GetITStatus()).
*                       - UDR (空栈读错误) 中断挂起位可以被一个读SPI_SR寄存器操
*                         作清除(SPI_I2S_GetITStatus())。
*                       - MODF (模式错误) 中断挂起位被软件按顺序清除:一个读/写 
*                         SPI_SR寄存器操作(SPI_I2S_GetITStatus())跟着一个写SPI_CR1 
*                         寄存器操作 (SPI_Cmd() 使能 SPI).    
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SPI_I2S_ClearFlag(SPI_TypeDef* SPIx, u16 SPI_I2S_FLAG)
{
  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_CLEAR_FLAG(SPI_I2S_FLAG));
    
    /* Clear the selected SPI CRC Error (CRCERR) flag [清除选择的SPI CRC错误(CRCERR)标志]*/
    SPIx->SR = (u16)~SPI_I2S_FLAG;
}

/*******************************************************************************
* 函数名称: SPI_I2S_GetITStatus
* 功能描述: SPI_IT:检查指定的的SPI/I2S中断是否发生了。
* 输入参数: (1) SPIx: x可以是 :
*                         - 1, 2 或 3 在 SPI 模式 
*                         - 2 或 3 在 I2S 模式
*           (2) SPI_I2S_IT:检查的指定的SPI/I2S中断源。 
*                    这个参数可以是下面的值之一:
*                       - SPI_I2S_IT_TXE: 传输缓冲为空中断
*                       - SPI_I2S_IT_RXNE: 接收缓冲不空中断
*                       - SPI_I2S_IT_OVR: 溢出中断.
*                       - SPI_IT_MODF: 模式错误中断
*                       - SPI_IT_CRCERR: CRC校验错误中断.
*                       - I2S_IT_UDR: 空栈读错误中断.
* 输出参数: 无
* 返回参数: SPI_I2S_IT的新状态 (SET or RESET).
*******************************************************************************/
ITStatus SPI_I2S_GetITStatus(SPI_TypeDef* SPIx, u8 SPI_I2S_IT)
{
  ITStatus bitstatus = RESET;
  u16 itpos = 0, itmask = 0, enablestatus = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_GET_IT(SPI_I2S_IT));

  /* Get the SPI/I2S IT index [取得SPI/I2S IT引索]*/
  itpos = (u16)((u16)0x01 << (SPI_I2S_IT & (u8)0x0F));

  /* Get the SPI/I2S IT mask [取得SPI/I2S IT标志]*/
  itmask = SPI_I2S_IT >> 4;
  /* Set the IT mask [置位IT标志]*/
  itmask = (u16)((u16)0x01 << itmask);
  /* Get the SPI_I2S_IT enable bit status [取得SPI_I2S_IT使能状态位]*/
  enablestatus = (SPIx->CR2 & itmask) ;

  /* Check the status of the specified SPI/I2S interrupt [检查指定的SPI/I2S中断状态]*/
  if (((SPIx->SR & itpos) != (u16)RESET) && enablestatus)
  {
    /* SPI_I2S_IT is set [置位SPI_I2S_IT]*/
    bitstatus = SET;
  }
  else
  {
    /* SPI_I2S_IT is reset [复位SPI_I2S_IT]*/
    bitstatus = RESET;
  }
  /* Return the SPI_I2S_IT status [返回SPI_I2S_IT状态]*/
  return bitstatus;
}

/*******************************************************************************
* 函数名称: SPI_I2S_ClearITPendingBit
* 功能描述: 清除某个SPI CRC错误(CRCERR)中断挂起位.
* 输入参数: (1)- SPIx: x可以是 :
*                         - 1, 2 或 3 在 SPI 模式 
*           (2) SPI_I2S_IT:清除指定的中断挂起位。
*                    这个函数只清除CRCERR中断挂起位。  
*                  Notes:
*                       - OVR (过速错误)中断挂起位可以被软件按顺序清除: 一个读  
*                         SPI_DR寄存器操作 (SPI_I2S_ReceiveData()) 跟着一个读  
*                         SPI_SR寄存器操作 (SPI_I2S_GetITStatus()).
*                       - UDR (空栈读错误) 中断挂起位可以被一个读SPI_SR寄存器操
*                         作清除(SPI_I2S_GetITStatus())。
*                       - MODF (模式错误) 中断挂起位被软件按顺序清除:一个读/写 
*                         SPI_SR寄存器操作(SPI_I2S_GetITStatus())跟着一个写SPI_CR1 
*                         寄存器操作 (SPI_Cmd() 使能 SPI).   
* 输出参数: 无
* 返回参数: 无
*******************************************************************************/
void SPI_I2S_ClearITPendingBit(SPI_TypeDef* SPIx, u8 SPI_I2S_IT)
{
  u16 itpos = 0;

  /* Check the parameters [检查参数]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_CLEAR_IT(SPI_I2S_IT));

  /* Get the SPI IT index [取得SPI IT向量]*/
  itpos = (u16)((u16)0x01 << (SPI_I2S_IT & (u8)0x0F));
  /* Clear the selected SPI CRC Error (CRCERR) interrupt pending bit [清除选择的SPI CRC错误中断挂起标志]*/
  SPIx->SR = (u16)~itpos;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
