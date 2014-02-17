				

#include "main.h"


							 


//#define ADC3_DR_Address    ((u32)0x40013C4C)
//u16 ADC_ConvertedValue=0;
u8 adc_new_ok=0;  //当前运行的ADC通道
volatile u16  batt_new_data=0;
u16 batt=0;




void Adc_Init(void)
{
   	GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
   // DMA_InitTypeDef DMA_InitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2 | RCC_APB2Periph_GPIOB, ENABLE);
  
    /**Adc-GPIO******电池*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);




      /* Configure and enable ADC interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	 
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //ADC1和ADC2工作在独立模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//ENABLE;//	//扫描模式//单次（单通道）模式
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //ENABLE;//连续模式	 外部触发时禁止
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//ADC_ExternalTrigConv_None;//不是外部触发ADC_ExternalTrigConv_T3_TRGO;//
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1;		 //ADC通道的数目 //5
  ADC_Init(ADC2, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ // 内部软件触发开启
  /*ADC的规则组通道1，ADC通道14,设置它们的转化顺序1和采样时间为55.5周期*/ 
  ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);  //battery

  /* Enable ADC1 DMA */
  //ADC_DMACmd(ADC3, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC2, ENABLE);

  /* Enable ADC1 reset calibaration register */  
  //校准 
  ADC_ResetCalibration(ADC2);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC2));
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC2);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC2));
     
  /* Start ADC1 Software Conversion */ 
  //ADC_SoftwareStartConvCmd(ADC3, ENABLE);//内部软件触发开启
  ADC_ExternalTrigConvCmd(ADC2, ENABLE);//外部硬件触发开启

  ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);  //转换结束
  //ADC_ExternalTrigConvCmd(ADC3, ENABLE);


  delay_ms(50);
  batt= ADC_GetConversionValue(ADC2)>>1;
}


const  u8  ADC_Channel_sel[1]=  //adc,对应通道
{
8,		//通道1	电池
		//通道2 moto1
		//通道3	moto2
};
u16  ADC_Channel_data[ADC_Channel_max];	//ADC值

		


u8  work_channel=0;
void Adc_isr(void)
{	 
		//LED3_Turn;
		if(work_channel==0)	//电池 加入中值滤波
		{
		  ADC_Channel_data[work_channel]=(((ADC_Channel_data[work_channel]*10)+(ADC_GetConversionValue(ADC2)>>1)))/11;		
		}
		else //ADC_Channel_data[work_channel]=ADC_GetConversionValue(ADC3)>>1;
		{
		  ADC_Channel_data[work_channel]=(((ADC_Channel_data[work_channel]*5)+(ADC_GetConversionValue(ADC2)>>1)))/6;		
		}

		#if(ADC_Channel_max>1)
		{
		adc_new_ok=work_channel;
		}
		#else 
		{
		adc_new_ok=1;
		}
		#endif
	  
	  #if(ADC_Channel_max>1)
	  {
	  if(work_channel<(ADC_Channel_max-1)) work_channel++;
	  else work_channel=0;
	  }  
	  #endif

	  ADC_RegularChannelConfig(ADC2,(ADC_Channel_sel[work_channel]), 1,ADC_SampleTime_239Cycles5);  //设法新的通道  
	  
	  //UART1_Put_Chars("Adc:");
	  //UART1_Put_int(ADC_Channel_data[work_channel]);	  
}
/********************************************************************************/


