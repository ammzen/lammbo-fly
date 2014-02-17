/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* �ļ�����: stm32f10x_spi.c
* ��������: MCD Application Team
* ����汾: V2.0.2
* ��������: 07/11/2008
* ��������: ����ļ��ṩ������SPI�Ĺ̼�����.
********************************************************************************
* Ŀǰ�Ĺ̼���Ŀ����Ϊ�ͻ��ṩ�������ǵĲ�Ʒ�ı�����Ϣ�Ա��ڽ�ʡ����ʱ�䡣
* ���, �ⷨ�뵼�彫����Ϊ�˳е��κ�ֱ�ӣ���ӻ���Ӧ�����⳥������κ�����Ҫ��
* ����̼���ʹ�������Ŀͻ������������Ӱ��������ǵ���ز�Ʒ����Ϣ��
*******************************************************************************/

/* �������ļ� ------------------------------------------------------------------*/
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/
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

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/*******************************************************************************
* ��������: SPI_I2S_DeInit
* ��������: ��SPIx����Ĵ�������Ϊ���ǵ�ȱʡֵ(Affects also the I2Ss).
* �������: SPIx xΪ1��2��3����ѡ��SPI���衣
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SPI_I2S_DeInit(SPI_TypeDef* SPIx)
{
  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  switch (*(u32*)&SPIx)
  {
    case SPI1_BASE:
      /* Enable SPI1 reset state [ʹ��SPI1��λ״̬]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);
      /* Release SPI1 from reset state [���SPI1��λ״̬]*/
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);
      break;

    case SPI2_BASE:
      /* Enable SPI2 reset state [ʹ��SPI2��λ״̬]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);
      /* Release SPI2 from reset state [���SPI2��λ״̬]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);
      break;

    case SPI3_BASE:
      /* Enable SPI3 reset state [ʹ��SPI3��λ״̬]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);
      /* Release SPI3 from reset state [���SPI3��λ״̬]*/
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);
      break;

    default:
      break;
  }
}

/*******************************************************************************
* ��������: SPI_Init
* ��������: ����SPI_InitStruct.�е��ض�������ʼ��SPIx����.
* �������: (1)SPIx ��xΪ1��2��3����ѡ��SPI���衣
*           (2)SPI_InitStruct��ָ��һ�������ض�SPI����������Ϣ��SPI_InitTypeDef�ṹ���ָ�롣
* �������: ��
* ���ز���: ��
******************************************************************************/
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{
  u16 tmpreg = 0;
  
  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));   
  
  /* Check the SPI parameters [���SPI�Ĳ���]*/
  assert_param(IS_SPI_DIRECTION_MODE(SPI_InitStruct->SPI_Direction));
  assert_param(IS_SPI_MODE(SPI_InitStruct->SPI_Mode));
  assert_param(IS_SPI_DATASIZE(SPI_InitStruct->SPI_DataSize));
  assert_param(IS_SPI_CPOL(SPI_InitStruct->SPI_CPOL));
  assert_param(IS_SPI_CPHA(SPI_InitStruct->SPI_CPHA));
  assert_param(IS_SPI_NSS(SPI_InitStruct->SPI_NSS));
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_InitStruct->SPI_BaudRatePrescaler));
  assert_param(IS_SPI_FIRST_BIT(SPI_InitStruct->SPI_FirstBit));
  assert_param(IS_SPI_CRC_POLYNOMIAL(SPI_InitStruct->SPI_CRCPolynomial));

/*---------------------------- SPIx CR1 Configuration [SPIx CR1����]*/
  /* Get the SPIx CR1 value [ȡ��SPIx CR1��ֵ]*/
  tmpreg = SPIx->CR1;
  /* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL and CPHA bits 
  ��BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL ��CPHA λ*/
  tmpreg &= CR1_CLEAR_Mask;
  /* Configure SPIx[����SPIx]: direction[����], NSS management[NSS����], first transmitted bit[��һ������λ], BaudRate prescaler[������Ԥ��Ƶ]
     master/salve mode[��/��ģʽ], CPOL and CPHA */
  /* Set BIDImode, BIDIOE and RxONLY bits according to SPI_Direction value [����SPI_Direction��ֵ��λBIDImode, BIDIOE��RxONLY]*/
  /* Set SSM, SSI and MSTR bits according to SPI_Mode and SPI_NSS values [����SPI_Mode ��SPI_NSS��ֵ��λSSM, SSI �� MSTR]*/
  /* Set LSBFirst bit according to SPI_FirstBit value [����SPI_FirstBit��ֵ��λLSBFirst]*/
  /* Set BR bits according to SPI_BaudRatePrescaler value [����SPI_BaudRatePrescaler��ֵ��λBR]*/
  /* Set CPOL bit according to SPI_CPOL value [����SPI_CPOL��ֵ��λCPOL]*/
  /* Set CPHA bit according to SPI_CPHA value [����SPI_CPHA��ֵ��λCPHA]*/
  tmpreg |= (u16)((u32)SPI_InitStruct->SPI_Direction | SPI_InitStruct->SPI_Mode |
                  SPI_InitStruct->SPI_DataSize | SPI_InitStruct->SPI_CPOL |  
                  SPI_InitStruct->SPI_CPHA | SPI_InitStruct->SPI_NSS |  
                  SPI_InitStruct->SPI_BaudRatePrescaler | SPI_InitStruct->SPI_FirstBit);
  /* Write to SPIx CR1 [дSPIx CR1]*/
  SPIx->CR1 = tmpreg;
  
  /* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
  SPIx->I2SCFGR &= SPI_Mode_Select;     

/*---------------------------- SPIx CRCPOLY Configuration --------------------*/
  /* Write to SPIx CRCPOLY [дSPIx CRCPOLY]*/
  SPIx->CRCPR = SPI_InitStruct->SPI_CRCPolynomial;
}

/*******************************************************************************
* ��������: I2S_Init
* ��������: ����SPI_InitStruct.�е��ض�������ʼ��SPIx����.
* �������: (1)SPIx ��xΪ1��2��3����ѡ��SPI����(����I2S ģʽ)��
*           (2)I2S_InitStruct��ָ��һ����ģʽI2S�����ض�SPI����������Ϣ��SPI_InitTypeDef�ṹ���ָ�롣
* �������: ��
* ���ز���: ��
******************************************************************************/
void I2S_Init(SPI_TypeDef* SPIx, I2S_InitTypeDef* I2S_InitStruct)
{
  u16 tmpreg = 0, i2sdiv = 2, i2sodd = 0, packetlength = 1;
  u32 tmp = 0;
  RCC_ClocksTypeDef RCC_Clocks;
   
  /* Check the I2S parameters [���I2S����]*/
  assert_param(IS_SPI_23_PERIPH(SPIx));
  assert_param(IS_I2S_MODE(I2S_InitStruct->I2S_Mode));
  assert_param(IS_I2S_STANDARD(I2S_InitStruct->I2S_Standard));
  assert_param(IS_I2S_DATA_FORMAT(I2S_InitStruct->I2S_DataFormat));
  assert_param(IS_I2S_MCLK_OUTPUT(I2S_InitStruct->I2S_MCLKOutput));
  assert_param(IS_I2S_AUDIO_FREQ(I2S_InitStruct->I2S_AudioFreq));
  assert_param(IS_I2S_CPOL(I2S_InitStruct->I2S_CPOL));  

/*----------------------- SPIx I2SCFGR & I2SPR Configuration -----------------*/

  /* Clear I2SMOD, I2SE, I2SCFG, PCMSYNC, I2SSTD, CKPOL, DATLEN and CHLEN bits
  [��I2SMOD, I2SE, I2SCFG, PCMSYNC, I2SSTD, CKPOL, DATLEN ��CHLEN��־] */
  SPIx->I2SCFGR &= I2SCFGR_CLEAR_Mask; 
  SPIx->I2SPR = 0x0002;
  
  /* Get the I2SCFGR register value [ȡ��I2SCFGRֵ]*/
  tmpreg = SPIx->I2SCFGR;
  
  /* If the default value has to be written, reinitialize i2sdiv and i2sodd[���Ĭ��ֵ�Ѿ�д�ˣ����³�ʼ��i2sdiv��i2sodd]*/
  if(I2S_InitStruct->I2S_AudioFreq == I2S_AudioFreq_Default)
  {
    i2sodd = (u16)0;
    i2sdiv = (u16)2;   
  }
  /* If the requested audio frequency is not the default, compute the prescaler [�������ĵ���Ƶ����Ĭ��ֵ������Ԥ��Ƶ]*/
  else
  {
    /* Check the frame length (For the Prescaler computing) [�������(����Ԥ��Ƶ��)]*/
    if(I2S_InitStruct->I2S_DataFormat == I2S_DataFormat_16b)
    {
      /* Packet length is 16 bits [���ĳ���16λ]*/
      packetlength = 1;
    }
    else
    {
      /* Packet length is 32 bits [���ĳ���32λ]*/
      packetlength = 2;
    }
    /* Get System Clock frequency [ȡ��ϵͳʱ��Ƶ��]*/
    RCC_GetClocksFreq(&RCC_Clocks);
    
    /* Compute the Real divider depending on the MCLK output state with a flaoting point 
    �ø���ָ�����MCLK���״̬������ʵ�ָ���*/
    if(I2S_InitStruct->I2S_MCLKOutput == I2S_MCLKOutput_Enable)
    {
      /* MCLK output is enabled [MCLK���ʹ��]*/
      tmp = (u16)(((10 * RCC_Clocks.SYSCLK_Frequency) / (256 * I2S_InitStruct->I2S_AudioFreq)) + 5);
    }
    else
    {
      /* MCLK output is disabled [MCLK�����ֹ]*/
      tmp = (u16)(((10 * RCC_Clocks.SYSCLK_Frequency) / (32 * packetlength * I2S_InitStruct->I2S_AudioFreq)) + 5);
    }
    
    /* Remove the flaoting point [ɾ������ָ��]*/
    tmp = tmp/10;  
      
    /* Check the parity of the divider [����������ż]*/
    i2sodd = (u16)(tmp & (u16)0x0001);
   
    /* Compute the i2sdiv prescaler [����i2sdivԤ��Ƶ��]*/
    i2sdiv = (u16)((tmp - i2sodd) / 2);
   
    /* Get the Mask for the Odd bit (SPI_I2SPR[8]) register [ȡ��(SPI_I2SPR[8])�Ĵ����ɵı�־λ]*/
    i2sodd = (u16) (i2sodd << 8);
  }
  
  /* Test if the divider is 1 or 0 [���Գ�����1����0]*/
  if ((i2sdiv < 2) || (i2sdiv > 0xFF))
  {
    /* Set the default values [����Ĭ��ֵ]*/
    i2sdiv = 2;
    i2sodd = 0;
  }

  /* Write to SPIx I2SPR register the computed value [��SPIx I2SPR�Ĵ���д�����ֵ]*/
  SPIx->I2SPR = (u16)(i2sdiv | i2sodd | I2S_InitStruct->I2S_MCLKOutput);  
 
  /* Configure the I2S with the SPI_InitStruct values [����SPI_InitStructֵ����I2S]*/
  tmpreg |= (u16)(I2S_Mode_Select | I2S_InitStruct->I2S_Mode | \
                  I2S_InitStruct->I2S_Standard | I2S_InitStruct->I2S_DataFormat | \
                  I2S_InitStruct->I2S_CPOL);
 
  /* Write to SPIx I2SCFGR [дSPIx I2SCFGR]*/  
  SPIx->I2SCFGR = tmpreg;                                    
}

/*******************************************************************************
* ��������: SPI_StructInit
* ��������: ʹ��ȱʡֵ���SPI_InitStructÿһ����Ա.
* �������: SPI_InitStruct:ָ��һ��������ʼ����SPI_InitTypeDef�ṹ��ָ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
{
/*--------------- Reset SPI init structure parameters values [��λSPI��ʼ���ṹ����ֵ]*/
  /* Initialize the SPI_Direction member [��ʼ��SPI_Direction��Ա]*/
  SPI_InitStruct->SPI_Direction = SPI_Direction_2Lines_FullDuplex;

  /* initialize the SPI_Mode member [��ʼ��SPI_ModeԱ]*/
  SPI_InitStruct->SPI_Mode = SPI_Mode_Slave;

  /* initialize the SPI_DataSize member [��ʼ��SPI_DataSize��Ա]*/
  SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b;

  /* Initialize the SPI_CPOL member [��ʼ��SPI_CPOL��Ա]*/
  SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;

  /* Initialize the SPI_CPHA member [��ʼ��SPI_CPHA��Ա]*/
  SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;

  /* Initialize the SPI_NSS member [��ʼ��SPI_NSS��Ա]*/
  SPI_InitStruct->SPI_NSS = SPI_NSS_Hard;

  /* Initialize the SPI_BaudRatePrescaler member [��ʼ��SPI_BaudRatePrescaler��Ա]*/
  SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;

  /* Initialize the SPI_FirstBit member [��ʼ��SPI_FirstBit��Ա]*/
  SPI_InitStruct->SPI_FirstBit = SPI_FirstBit_MSB;

  /* Initialize the SPI_CRCPolynomial member [��ʼ��SPI_CRCPolynomial��Ա]*/
  SPI_InitStruct->SPI_CRCPolynomial = 7;
}

/*******************************************************************************
* ��������: I2S_StructInit
* ��������: ��Ĭ��ֵ���ÿ��I2S_InitStruct�ṹ��Ա.
* �������: I2S_InitStruct : ָ��Ҫ����ʼ����I2S_InitTypeDef �ṹָ��.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2S_StructInit(I2S_InitTypeDef* I2S_InitStruct)
{
/*--------------- Reset I2S init structure parameters values [��λI2S��ʼ���ṹ����ֵ]*/
  /* Initialize the I2S_Mode member [��ʼ��I2S_Mode��Ա]*/
  I2S_InitStruct->I2S_Mode = I2S_Mode_SlaveTx;
  
  /* Initialize the I2S_Standard member [��ʼ��I2S_Standard��Ա]*/
  I2S_InitStruct->I2S_Standard = I2S_Standard_Phillips;
  
  /* Initialize the I2S_DataFormat member [��ʼ��I2S_DataFormat��Ա]*/
  I2S_InitStruct->I2S_DataFormat = I2S_DataFormat_16b;
  
  /* Initialize the I2S_MCLKOutput member [��ʼ��I2S_MCLKOutput��Ա]*/
  I2S_InitStruct->I2S_MCLKOutput = I2S_MCLKOutput_Disable;
  
  /* Initialize the I2S_AudioFreq member [��ʼ��I2S_AudioFreq��Ա]*/
  I2S_InitStruct->I2S_AudioFreq = I2S_AudioFreq_Default;
  
  /* Initialize the I2S_CPOL member [��ʼ��I2S_CPOL��Ա]*/
  I2S_InitStruct->I2S_CPOL = I2S_CPOL_Low;
}

/*******************************************************************************
* ��������: SPI_Cmd
* ��������: ʹ�ܻ�ر��ض�������.
* �������: (1)SPIx ��xΪ1��2��3����ѡ��SPI���衣
*           (2)NewState:���SPI�������״̬���������������ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected SPI peripheral [ʹ��ѡ���SPI����]*/
    SPIx->CR1 |= CR1_SPE_Set;
  }
  else
  {
    /* Disable the selected SPI peripheral [��ֹѡ���SPI����]*/
    SPIx->CR1 &= CR1_SPE_Reset;
  }
}

/*******************************************************************************
* ��������: I2S_Cmd
* ��������: ʹ�ܻ��ָֹ����SPI����(�� I2S ģʽ).
* �������: (1)SPIx ��xΪ1��2��3����ѡ��SPI���衣
*           (2)NewState: �µ�SPIx����״̬. 
*                    �������������: ENABLE or DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void I2S_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_SPI_23_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected SPI peripheral (in I2S mode) [ʹ��ѡ�������SPI(��I2S ģʽ)]*/
    SPIx->I2SCFGR |= I2SCFGR_I2SE_Set;
  }
  else
  {
    /* Disable the selected SPI peripheral (in I2S mode) [��ֹѡ�������SPI(��I2S ģʽ)]*/
    SPIx->I2SCFGR &= I2SCFGR_I2SE_Reset;
  }
}

/*******************************************************************************
* ��������: SPI_I2S_ITConfig
* ��������: ʹ�ܻ��ָֹ����SPI/I2S�жϡ�
* �������: (1)SPIx: x������ :
*                         - 1, 2 �� 3 �� SPI ģʽ 
*                         - 2 �� 3 �� I2S ģʽ
*           (2)SPI_I2S_IT: ָ�����ж�Դ��ʹ�ܻ��ֹ. 
*                    ������������������ֵ֮һ:
*                       - SPI_I2S_IT_TXE: Tx buffer empty interrupt mask
*                       - SPI_I2S_IT_RXNE: Rx buffer not empty interrupt mask
*                       - SPI_I2S_IT_ERR: Error interrupt mask
*           (3)NewState: ָ���� SPI/I2S �ж���״̬.
*                    �������������: ENABLE or DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SPI_I2S_ITConfig(SPI_TypeDef* SPIx, u8 SPI_I2S_IT, FunctionalState NewState)
{
  u16 itpos = 0, itmask = 0 ;

  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_SPI_I2S_CONFIG_IT(SPI_I2S_IT));

  /* Get the SPI/I2S IT index */
  itpos = SPI_I2S_IT >> 4;
  /* Set the IT mask */
  itmask = (u16)((u16)1 << itpos);

  if (NewState != DISABLE)
  {
    /* Enable the selected SPI/I2S interrupt [ʹ��ѡ���SPI/I2S �ж�]*/
    SPIx->CR2 |= itmask;
  }
  else
  {
    /* Disable the selected SPI/I2S interrupt [��ֹѡ���SPI/I2S �ж�]*/
    SPIx->CR2 &= (u16)~itmask;
  }
}

/*******************************************************************************
* ��������: SPI_I2S_DMACmd
* ��������: ʹ�ܻ��ֹSPIx/I2Sx DMA�ӿ�.
* �������: (1)SPIx: x������ :
*                         - 1, 2 �� 3 �� SPI ģʽ 
*                         - 2 �� 3 �� I2S ģʽ
*           (2)SPI_I2S_DMAReq: ָ���� SPI/I2S DMA ��������ʹ�ܻ��ֹ��
*                    ���������������������ֵ�����:
*                       - SPI_I2S_DMAReq_Tx: ���ͻ��� DMA��������
*                       - SPI_I2S_DMAReq_Rx:  ���ջ���DMA ��������
*           (3)NewState:ѡ��� SPI/I2S DMA����������״̬ 
*                    �������������: ENABLE ��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SPI_I2S_DMACmd(SPI_TypeDef* SPIx, u16 SPI_I2S_DMAReq, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_SPI_I2S_DMAREQ(SPI_I2S_DMAReq));

  if (NewState != DISABLE)
  {
    /* Enable the selected SPI/I2S DMA requests [ʹ��ѡ���SPI/I2S DMA����]*/
    SPIx->CR2 |= SPI_I2S_DMAReq;
  }
  else
  {
    /* Disable the selected SPI/I2S DMA requests [��ֹѡ���SPI/I2S DMA����]*/
    SPIx->CR2 &= (u16)~SPI_I2S_DMAReq;
  }
}

/*******************************************************************************
* ��������: SPI_I2S_SendData
* ��������: ͨ��SPIx/I2Sx���跢������
* �������: (1)SPIx: x������ :
*                         - 1, 2 �� 3 �� SPI ģʽ 
*                         - 2 �� 3 �� I2S ģʽ
*           (2)Data : ���͵�����
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SPI_I2S_SendData(SPI_TypeDef* SPIx, u16 Data)
{
  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* Write in the DR register the data to be sent [��׼�����͵�����д��DR�Ĵ���]*/
  SPIx->DR = Data;
}

/*******************************************************************************
* ��������: SPI_I2S_ReceiveData
* ��������: ���������SPIx/I2Sx�ⲿ�豸���յ����ݡ�
* �������: (1)- SPIx: x������ :
*                         - 1, 2 �� 3 �� SPI ģʽ 
*                         - 2 �� 3 �� I2S ģʽ
* �������: ��
* ���ز���: ���յ�����ֵ.
*******************************************************************************/
u16 SPI_I2S_ReceiveData(SPI_TypeDef* SPIx)
{
  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* Return the data in the DR register [����DR�Ĵ���������]*/
  return SPIx->DR;
}

/*******************************************************************************
* ��������: SPI_NSSInternalSoftwareConfig
* ��������: �������ѡ���SPI�ڲ�����.
* �������: (1)SPIx ��xΪ1��2��3����ѡ��SPI���衣
*           (2)SPI_NSSInternalSoft: ָ����SPI NSS�ڲ�״̬.
*                    ������������������ֵ֮һ:
*                       - SPI_NSSInternalSoft_Set: ��λNSS�ڲ�����
*                       - SPI_NSSInternalSoft_Reset: ��λNSS�ڲ�����
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SPI_NSSInternalSoftwareConfig(SPI_TypeDef* SPIx, u16 SPI_NSSInternalSoft)
{
  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_NSS_INTERNAL(SPI_NSSInternalSoft));

  if (SPI_NSSInternalSoft != SPI_NSSInternalSoft_Reset)
  {
    /* Set NSS pin internally by software [�����λNSS�ڲ�����]*/
    SPIx->CR1 |= SPI_NSSInternalSoft_Set;
  }
  else
  {
    /* Reset NSS pin internally by software [�����λNSS�ڲ�����]*/
    SPIx->CR1 &= SPI_NSSInternalSoft_Reset;
  }
}

/*******************************************************************************
* ��������: SPI_SSOutputCmd
* ��������: ʹ�ܻ�ر�ѡ����SPI�ӿڵ�SS���.
* �������: (1)SPIx ��xΪ1��2��3����ѡ��SPI���衣
*           (2)NewState:ѡ����SPI SS�������״̬���������������ENABLE��DISABLE
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SPI_SSOutputCmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected SPI SS output [ʹ��ѡ���SPI SS���]*/
    SPIx->CR2 |= CR2_SSOE_Set;
  }
  else
  {
    /* Disable the selected SPI SS output [��ֹѡ���SPI SS���]*/
    SPIx->CR2 &= CR2_SSOE_Reset;
  }
}

/*******************************************************************************
* ��������: SPI_DataSizeConfig
* ��������: Ϊѡ����SPI�ӿ��������ݴ�С.
* �������: (1)SPIx ��xΪ1��2��3����ѡ��SPI���衣
*           (2)SPI_DataSize: SPI���ݴ�С��
*                    ������������������ֵ֮һ:
*                       - SPI_DataSize_16b: �������ݴ�СΪ16λ
*                       - SPI_DataSize_8b: �������ݴ�СΪ8λ
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SPI_DataSizeConfig(SPI_TypeDef* SPIx, u16 SPI_DataSize)
{
  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_DATASIZE(SPI_DataSize));

  /* Clear DFF bit [���DFFλ]*/
  SPIx->CR1 &= (u16)~SPI_DataSize_16b;
  /* Set new DFF bit value [����DFFλ����ֵ]*/
  SPIx->CR1 |= SPI_DataSize;
}

/*******************************************************************************
* ��������: SPI_TransmitCRC
* ��������: ����ĳ��SPI��CRCУ��ֵ
* �������: SPIx ��xΪ1��2��3����ѡ��SPI���衣
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SPI_TransmitCRC(SPI_TypeDef* SPIx)
{
  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* Enable the selected SPI CRC transmission [ʹ��ѡ�е�SPI CRC����]*/
  SPIx->CR1 |= CR1_CRCNext_Set;
}

/*******************************************************************************
* ��������: SPI_CalculateCRC
* ��������: ʹ�ܻ�ȡ�������ֽڵ�CRCУ��ֵ�ļ���.
* �������: (1)SPIx ��xΪ1��2��3����ѡ��SPI���衣
*           (2)NewState: ѡ��SPI�ӿڵ�CRC�������״̬���������������ENABLE��DISABLE.
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SPI_CalculateCRC(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected SPI CRC calculation [ʹ��ѡ�е�SPI CRC����]*/
    SPIx->CR1 |= CR1_CRCEN_Set;
  }
  else
  {
    /* Disable the selected SPI CRC calculation [��ֹѡ�е�SPI CRC����]*/
    SPIx->CR1 &= CR1_CRCEN_Reset;
  }
}

/*******************************************************************************
* ��������: SPI_GetCRC
* ��������: �����ض�SPI���贫�ͻ���յ�CRC�Ĵ�����ֵ.
* �������: (1)SPIx ��xΪ1��2��3����ѡ��SPI���衣
*           (2)SPI_CRC:������ȡ��CRC�Ĵ�����
*                    ������������������ֵ֮һ:
*                       - SPI_CRC_Tx: ѡ��Tx CRC�Ĵ���
*                       - SPI_CRC_Rx: ѡ��Rx CRC�Ĵ���
* �������: ��
* ���ز���: ��ѡ���CRC�Ĵ�����ֵ��
*******************************************************************************/
u16 SPI_GetCRC(SPI_TypeDef* SPIx, u8 SPI_CRC)
{
  u16 crcreg = 0;

  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_CRC(SPI_CRC));

  if (SPI_CRC != SPI_CRC_Rx)
  {
    /* Get the Tx CRC register [ȡ��Tx CRC�Ĵ���]*/
    crcreg = SPIx->TXCRCR;
  }
  else
  {
    /* Get the Rx CRC register [ȡ��Rx CRC�Ĵ���]*/
    crcreg = SPIx->RXCRCR;
  }

  /* Return the selected CRC register [����ѡ���CRC�Ĵ���]*/
  return crcreg;
}

/*******************************************************************************
* ��������: SPI_GetCRCPolynomial
* ��������: �����ض�SPI�ӿڵ�CRC����ʽ�Ĵ�����ֵ.
* �������: SPIx ��xΪ1��2��3����ѡ��SPI���衣
* �������: ��
* ���ز���: CRC����ʽ�Ĵ�����ֵ.
*******************************************************************************/
u16 SPI_GetCRCPolynomial(SPI_TypeDef* SPIx)
{
  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* Return the CRC polynomial register [����CRC����ʽ�Ĵ���]*/
  return SPIx->CRCPR;
}

/*******************************************************************************
* ��������: SPI_BiDirectionalLineConfig
* ��������: Ϊ�ض���SPI�ӿ���˫��ģʽʱѡ�����ݴ��䷽��.
* �������: (1)SPIx ��xΪ1��2��3����ѡ��SPI���衣
*           (2)SPI_Direction: ��˫��ģʽ�е����ݴ��䷽�� 
*                    ������������������ֵ֮һ:
*                       - SPI_Direction_Tx: ѡ��Tx���ͷ���
*                       - SPI_Direction_Rx: ѡ��Rx���շ���
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SPI_BiDirectionalLineConfig(SPI_TypeDef* SPIx, u16 SPI_Direction)
{
  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_DIRECTION(SPI_Direction));

  if (SPI_Direction == SPI_Direction_Tx)
  {
    /* Set the Tx only mode [����ֻ����ģʽ]*/
    SPIx->CR1 |= SPI_Direction_Tx;
  }
  else
  {
    /* Set the Rx only mode [����ֻ����ģʽ]*/
    SPIx->CR1 &= SPI_Direction_Rx;
  }
}

/*******************************************************************************
* ��������: SPI_I2S_GetFlagStatus
* ��������: ���ָ����SPI/I2S����Ƿ���λ.
* �������: (1)- SPIx: x������ :
*                         - 1, 2 �� 3 �� SPI ģʽ 
*                         - 2 �� 3 �� I2S ģʽ
*           (2)���ָ����SPI/I2S����Ƿ�����. 
*                    ������������������ֵ֮һ:
*                       - SPI_I2S_FLAG_TXE: ���仺��Ϊ�ձ��.
*                       - SPI_I2S_FLAG_RXNE: ���ջ��岻�ձ��.
*                       - SPI_I2S_FLAG_BSY: æµ���.
*                       - SPI_I2S_FLAG_OVR: ������.
*                       - SPI_FLAG_MODF: ģʽ������.
*                       - SPI_FLAG_CRCERR: CRCУ�������.
*                       - I2S_FLAG_UDR: ��ջ��������. 
*                       - I2S_FLAG_CHSIDE: ͨ���߱�־.
* �������: ��
* ���ز���: SPI_I2S_FLAG��ǵ���״̬  (SET or RESET).
*******************************************************************************/
FlagStatus SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, u16 SPI_I2S_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_GET_FLAG(SPI_I2S_FLAG));

  /* Check the status of the specified SPI/I2S flag [���ָ����SPI/I2S��־״̬]*/
  if ((SPIx->SR & SPI_I2S_FLAG) != (u16)RESET)
  {
    /* SPI_I2S_FLAG is set [��λSPI_I2S_FLAG]*/
    bitstatus = SET;
  }
  else
  {
    /* SPI_I2S_FLAG is reset [��λSPI_I2S_FLAG]*/
    bitstatus = RESET;
  }
  /* Return the SPI_I2S_FLAG status [����SPI_I2S_FLAG״̬]*/
  return  bitstatus;
}

/*******************************************************************************
* ��������: SPI_I2S_ClearFlag
* ��������: ���SPIx CRC����(CRCERR) ��־.
* �������: (1)SPIx: x������ :
*                         - 1, 2 �� 3 �� SPI ģʽ 
*           (2)SPI_I2S_FLAG: ָ�������SPI��־. 
*                    �������ֻ�����CRCERR��־.                                           
*                  ע��:
*                       - OVR (���ٴ���)�жϹ���λ���Ա������˳�����: һ����  
*                         SPI_DR�Ĵ������� (SPI_I2S_ReceiveData()) ����һ����  
*                         SPI_SR�Ĵ������� (SPI_I2S_GetITStatus()).
*                       - UDR (��ջ������) �жϹ���λ���Ա�һ����SPI_SR�Ĵ�����
*                         �����(SPI_I2S_GetITStatus())��
*                       - MODF (ģʽ����) �жϹ���λ�������˳�����:һ����/д 
*                         SPI_SR�Ĵ�������(SPI_I2S_GetITStatus())����һ��дSPI_CR1 
*                         �Ĵ������� (SPI_Cmd() ʹ�� SPI).    
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SPI_I2S_ClearFlag(SPI_TypeDef* SPIx, u16 SPI_I2S_FLAG)
{
  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_CLEAR_FLAG(SPI_I2S_FLAG));
    
    /* Clear the selected SPI CRC Error (CRCERR) flag [���ѡ���SPI CRC����(CRCERR)��־]*/
    SPIx->SR = (u16)~SPI_I2S_FLAG;
}

/*******************************************************************************
* ��������: SPI_I2S_GetITStatus
* ��������: SPI_IT:���ָ���ĵ�SPI/I2S�ж��Ƿ����ˡ�
* �������: (1) SPIx: x������ :
*                         - 1, 2 �� 3 �� SPI ģʽ 
*                         - 2 �� 3 �� I2S ģʽ
*           (2) SPI_I2S_IT:����ָ����SPI/I2S�ж�Դ�� 
*                    ������������������ֵ֮һ:
*                       - SPI_I2S_IT_TXE: ���仺��Ϊ���ж�
*                       - SPI_I2S_IT_RXNE: ���ջ��岻���ж�
*                       - SPI_I2S_IT_OVR: ����ж�.
*                       - SPI_IT_MODF: ģʽ�����ж�
*                       - SPI_IT_CRCERR: CRCУ������ж�.
*                       - I2S_IT_UDR: ��ջ�������ж�.
* �������: ��
* ���ز���: SPI_I2S_IT����״̬ (SET or RESET).
*******************************************************************************/
ITStatus SPI_I2S_GetITStatus(SPI_TypeDef* SPIx, u8 SPI_I2S_IT)
{
  ITStatus bitstatus = RESET;
  u16 itpos = 0, itmask = 0, enablestatus = 0;

  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_GET_IT(SPI_I2S_IT));

  /* Get the SPI/I2S IT index [ȡ��SPI/I2S IT����]*/
  itpos = (u16)((u16)0x01 << (SPI_I2S_IT & (u8)0x0F));

  /* Get the SPI/I2S IT mask [ȡ��SPI/I2S IT��־]*/
  itmask = SPI_I2S_IT >> 4;
  /* Set the IT mask [��λIT��־]*/
  itmask = (u16)((u16)0x01 << itmask);
  /* Get the SPI_I2S_IT enable bit status [ȡ��SPI_I2S_ITʹ��״̬λ]*/
  enablestatus = (SPIx->CR2 & itmask) ;

  /* Check the status of the specified SPI/I2S interrupt [���ָ����SPI/I2S�ж�״̬]*/
  if (((SPIx->SR & itpos) != (u16)RESET) && enablestatus)
  {
    /* SPI_I2S_IT is set [��λSPI_I2S_IT]*/
    bitstatus = SET;
  }
  else
  {
    /* SPI_I2S_IT is reset [��λSPI_I2S_IT]*/
    bitstatus = RESET;
  }
  /* Return the SPI_I2S_IT status [����SPI_I2S_IT״̬]*/
  return bitstatus;
}

/*******************************************************************************
* ��������: SPI_I2S_ClearITPendingBit
* ��������: ���ĳ��SPI CRC����(CRCERR)�жϹ���λ.
* �������: (1)- SPIx: x������ :
*                         - 1, 2 �� 3 �� SPI ģʽ 
*           (2) SPI_I2S_IT:���ָ�����жϹ���λ��
*                    �������ֻ���CRCERR�жϹ���λ��  
*                  Notes:
*                       - OVR (���ٴ���)�жϹ���λ���Ա������˳�����: һ����  
*                         SPI_DR�Ĵ������� (SPI_I2S_ReceiveData()) ����һ����  
*                         SPI_SR�Ĵ������� (SPI_I2S_GetITStatus()).
*                       - UDR (��ջ������) �жϹ���λ���Ա�һ����SPI_SR�Ĵ�����
*                         �����(SPI_I2S_GetITStatus())��
*                       - MODF (ģʽ����) �жϹ���λ�������˳�����:һ����/д 
*                         SPI_SR�Ĵ�������(SPI_I2S_GetITStatus())����һ��дSPI_CR1 
*                         �Ĵ������� (SPI_Cmd() ʹ�� SPI).   
* �������: ��
* ���ز���: ��
*******************************************************************************/
void SPI_I2S_ClearITPendingBit(SPI_TypeDef* SPIx, u8 SPI_I2S_IT)
{
  u16 itpos = 0;

  /* Check the parameters [������]*/
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_CLEAR_IT(SPI_I2S_IT));

  /* Get the SPI IT index [ȡ��SPI IT����]*/
  itpos = (u16)((u16)0x01 << (SPI_I2S_IT & (u8)0x0F));
  /* Clear the selected SPI CRC Error (CRCERR) interrupt pending bit [���ѡ���SPI CRC�����жϹ����־]*/
  SPIx->SR = (u16)~itpos;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
