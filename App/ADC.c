				

#include "main.h"


							 


//#define ADC3_DR_Address    ((u32)0x40013C4C)
//u16 ADC_ConvertedValue=0;
u8 adc_new_ok=0;  //��ǰ���е�ADCͨ��
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
  
    /**Adc-GPIO******���*/
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
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //ADC1��ADC2�����ڶ���ģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//ENABLE;//	//ɨ��ģʽ//���Σ���ͨ����ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //ENABLE;//����ģʽ	 �ⲿ����ʱ��ֹ
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//ADC_ExternalTrigConv_None;//�����ⲿ����ADC_ExternalTrigConv_T3_TRGO;//
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC�����Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = 1;		 //ADCͨ������Ŀ //5
  ADC_Init(ADC2, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ // �ڲ������������
  /*ADC�Ĺ�����ͨ��1��ADCͨ��14,�������ǵ�ת��˳��1�Ͳ���ʱ��Ϊ55.5����*/ 
  ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);  //battery

  /* Enable ADC1 DMA */
  //ADC_DMACmd(ADC3, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC2, ENABLE);

  /* Enable ADC1 reset calibaration register */  
  //У׼ 
  ADC_ResetCalibration(ADC2);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC2));
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC2);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC2));
     
  /* Start ADC1 Software Conversion */ 
  //ADC_SoftwareStartConvCmd(ADC3, ENABLE);//�ڲ������������
  ADC_ExternalTrigConvCmd(ADC2, ENABLE);//�ⲿӲ����������

  ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);  //ת������
  //ADC_ExternalTrigConvCmd(ADC3, ENABLE);


  delay_ms(50);
  batt= ADC_GetConversionValue(ADC2)>>1;
}


const  u8  ADC_Channel_sel[1]=  //adc,��Ӧͨ��
{
8,		//ͨ��1	���
		//ͨ��2 moto1
		//ͨ��3	moto2
};
u16  ADC_Channel_data[ADC_Channel_max];	//ADCֵ

		


u8  work_channel=0;
void Adc_isr(void)
{	 
		//LED3_Turn;
		if(work_channel==0)	//��� ������ֵ�˲�
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

	  ADC_RegularChannelConfig(ADC2,(ADC_Channel_sel[work_channel]), 1,ADC_SampleTime_239Cycles5);  //�跨�µ�ͨ��  
	  
	  //UART1_Put_Chars("Adc:");
	  //UART1_Put_int(ADC_Channel_data[work_channel]);	  
}
/********************************************************************************/


