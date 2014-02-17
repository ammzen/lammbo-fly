

#include "main.h"
/********

star:
13-07-31    edit by edmondchao

����˼·�� TIMѭ�����������ݽ����жϼ�����������������ó�����������������2MSС��1MSΪ��		   
		   �ⲿ����100MSһ�β�����ÿ��ͨ�����ݣ�����һ������ʧ�ء�

***/

u8 tim_ppm_num=0;//PPM����TIM������0-10ѭ������
u32 PPM_TIM_coun[Chanl_num_max]; //���ڴ濪ʼ��TIMֵ
u8 PPM_check[Chanl_num_max];//����ȷ��ֵ����ʱ����Ӧͨ��+1�� 50MSһ�β�����Ϊ0��ppm_data_ok-1




u8 ppm_in[Chanl_num_max];


u8 ppm_data_ok=0;	   //����OK


u8 PPM1_pin(void){return PPM1_data_pin;}
u8 PPM2_pin(void){return PPM2_data_pin;}
u8 PPM3_pin(void){return PPM3_data_pin;}
u8 PPM4_pin(void){return PPM4_data_pin;}
u8 (*PPMx_pin[])(void)=	
{
	PPM1_pin,
	PPM2_pin,
	PPM3_pin,
	PPM4_pin,	
	//���ӿ��ƶ˿ں��� 
};


//ʧ�ؼ��100msһ��	���뵽������С�
u16  no_ppmin=0;
void No_PPM_check(void)
{
	  u8 i=0,j=0;;
	  for(i=0;i<Chanl_num_max;i++)
	  {
	  	  //UART1_Put_int(PPM_check[i]);
		  //UART1_Put_int(ppm_in[i]);	
		     			
	  	  if(PPM_check[i])
		  {	
		  	j++;
		  	PPM_check[i]--;
		  }
	  }
	  //UART1_Put_int(j);
	  //USART_newline;	//���� 

	  if(j==Chanl_num_max)
	  {
	  	 if(ppm_data_ok<ppm_ok_num)ppm_data_ok++;	  
	  }
	  else
	  {
	  	 if(ppm_data_ok)ppm_data_ok--;
	  }
	  //UART1_Put_int(ppm_data_ok);
	  //USART_newline;	//����
   
}

//ppm����  �����ж�
void ppm_cal(u8 chanl)
{
     u16 a;

	 //LED1_Turn;
	 
	 a=	TIM_GetCounter(TIM2);

	 if(PPMx_pin[chanl]()==1)
	 {
	 	 //LED4_ON;
		 PPM_TIM_coun[chanl]=tim_ppm_num*30000+a;

	 }
	 else
	 {
	 	 //LED4_OFF;
		 u32 m=0;

		 m=tim_ppm_num*30000+a;
		 if(m<PPM_TIM_coun[chanl]) m=(tim_ppm_num+10)*30000+a; //tim_ppm_num���δ�10-->0

		 a=m-PPM_TIM_coun[chanl];		 
		 if((a<800)|(a>2200))//����
		 {	
			PPM_check[chanl]=0;
			
			//LED4_Turn;
		 }
		 else
		 {
	   		//�޷�
	   		if(a>2000)a=2000;
	   		if(a<1000)a=1000;
	   
	   		a-=1000; 

			ppm_in[chanl]=(a>>2); 

			if(PPM_check[chanl]<ppm_ok_num)PPM_check[chanl]++;
	 	 }
	 }
}



/* ---------------------------------------------------------------
    TIM1 Configuration: Output Compare Timing Mode:
    TIM2CLK = 72 MHz, Prescaler = 72, TIM2 counter clock =1MHz
    ppm   1MS	:	1000

	PPM�����ڣ�1-2MS֮�䣬1000-2000��  
	����	Period��Ϊ3000��3MS��������Ϊ�µĿ�ʼ������롣
  --------------------------------------------------------------- */
  
    
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Init(void)
*��������:		��ʼ��I2C��Ӧ�Ľӿ����š�
*******************************************************************************/
void PPM_Init(void)
{			
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	EXTI_InitTypeDef EXTI_InitStructure;


   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    //NVIC
   
  /* Enable the TIM2 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

   TIM_TimeBaseStructure.TIM_Period = 30000;
   TIM_TimeBaseStructure.TIM_Prescaler = 71;
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

   /* TIM IT enable */
   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

   /* TIM2 enable counter */   
   TIM_Cmd(TIM2, ENABLE);
   //TIM_Cmd(TIM2, DISABLE);


	//INT
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	                              
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  	
  	GPIO_Init(GPIOA, &GPIO_InitStructure);


	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
	
	
	/**Key-EXIT******/
	EXTI_InitStructure.EXTI_Line = EXTI_Line4|EXTI_Line5|EXTI_Line6|EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	

	

	/**Key-NVIC******/
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
   
   NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQChannel;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
 
   NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQChannel;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);	    /**/
}											 
void PPM_close(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//EXTI_InitTypeDef EXTI_InitStructure;

	TIM_Cmd(TIM2, DISABLE);

	 
	

   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
   NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQChannel;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
   NVIC_Init(&NVIC_InitStructure);

   NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQChannel;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
   NVIC_Init(&NVIC_InitStructure);
}




//------------------End of File----------------------------
