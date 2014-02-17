

#include "main.h"


u8 ppm_in[10]={

 0,0,0,0,0,0,0,0,0,0,

};


u8 ppm_data_ok=0;	   //数据OK
u8 channl=0;		   //通道
u8 ppm_data_ovf=0;	   //溢出

//失控检测100ms一次	放入到主检测中。
u16  no_ppmin=0;
void No_PPM_check(void)
{    
	 if( ppm_data_ovf)
	 {
	 	no_ppmin++;

		if(no_ppmin>100) //失控
		{
		   no_ppmin=200;
		   if(ppm_data_ok)ppm_data_ok--; 
		} 	 
	 }
	 else  no_ppmin=0;
}

//ppm计算  放入中断
void ppm_cal(void)
{
     u16 a;

	 //LED1_Turn;
	 TIM_Cmd(TIM2, DISABLE);
	 a=	TIM_GetCounter(TIM2);

	  //a=0x100;

	if(ppm_data_ovf==1)
	{	
	  ppm_data_ovf=0;
	  channl=0;
	} 
	else if((a>2200)|(a<800))//错码
	{	
		channl=0;
		if(ppm_data_ok)ppm_data_ok--; 	  
	  	//ppm_data_ok=0;
		//LED1_OFF;
		 
	}
	else
	{
	   //限幅
	   if(a>2000)a=2000;
	   if(a<1000)a=1000;
	   
	   a-=1000;

	   //处理反向
	   if((channl==Gas_num)&&(Gas_Rev)) ppm_in[channl]=0xff-(a>>2);
	   else  if((channl==Nick_num)&&(Nick_Rev)) ppm_in[channl]=0xff-(a>>2);
	   else  if((channl==Roll_num)&&(Roll_Rev)) ppm_in[channl]=0xff-(a>>2);
	   else  if((channl==Gier_num)&&(Gier_Rev)) ppm_in[channl]=0xff-(a>>2);
	   else  ppm_in[channl]=a>>2;

	   channl++;
	   if(channl>7)
	   {
	    channl=0;

	   	if(ppm_data_ok<(ppm_ok_num))ppm_data_ok++;


	   }

	   //LED1_Turn;
	
	}


	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);

	
}

/* ---------------------------------------------------------------
    TIM1 Configuration: Output Compare Timing Mode:
    TIM2CLK = 72 MHz, Prescaler = 72, TIM2 counter clock =1MHz
    ppm   1MS	:	1000

	PPM脉宽在：1-2MS之间，1000-2000，  
	所以	Period设为3000，3MS，大于则为新的开始或错误码。
  --------------------------------------------------------------- */
  
    
/**************************实现函数********************************************
*函数原型:		void IIC_Init(void)
*功　　能:		初始化I2C对应的接口引脚。
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

   TIM_TimeBaseStructure.TIM_Period = 3000;
   TIM_TimeBaseStructure.TIM_Prescaler = 71;
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

   /* TIM IT enable */
   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

   /* TIM2 enable counter */   
  // TIM_Cmd(TIM1, ENABLE);
   TIM_Cmd(TIM2, DISABLE);


	//INT
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
	                              
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  	
  	GPIO_Init(GPIOA, &GPIO_InitStructure);


	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	
	
	/**Key-EXIT******/
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	

	

	/**Key-NVIC******/

	/* Configure the Priority Group to 2 bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

   
   
   NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQChannel;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

   
}
void PPM_close(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	EXTI_InitTypeDef EXTI_InitStructure;

	TIM_Cmd(TIM2, DISABLE);

	 /**Key-EXIT******/
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);

	/**Key-NVIC******/

	/* Configure the Priority Group to 2 bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
   NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQChannel;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
   NVIC_Init(&NVIC_InitStructure);
}




//------------------End of File----------------------------
