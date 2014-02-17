				
#include "main.h"


u8 reset=0;
u8 key=0;

//exti: 外部中断通道    trig: 目标触发类型，0：Falling	1: Rising 3: EXTI_Trigger_Rising_Falling

//传入中断号
void Key_int_isr(u8 key_num)
{
	if(GPIO_ReadInputDataBit(key_port,(1<<(key_num))))	key=0;
	else {key=1;	 }	
}
void Key_Init(void)
{

   	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
    

	/**Key-GPIO******/ 

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
	                              
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  	
  	GPIO_Init(GPIOA, &GPIO_InitStructure);



	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);


	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);
	
	/**Key-EXIT******/

	

	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	

	

	/**Key-NVIC******/

	/* Configure the Priority Group to 2 bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

   /* Configure the SysTick handler priority */
    //NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 0, 0);
  
   
   
   NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);


}
/********************************************************************************/




