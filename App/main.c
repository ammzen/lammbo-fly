


/********************LAMBO Fly soft********************


star:
13-01-07    edit by edmondchao


重量：

PPM接收：9克
电池240mAH：8克

华科尔机架：20克+PCB=25+电池=	33

新机架：		   26				=40
    四电机：13克  PCB:11

*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

vu32 TimingDelay = 0;


int main(void)
{ 

  SysTick_Config();
  SYS_Init();	
  
  //delay_init(72); 
  
  led_Init();  	
  USART_Configuration();
  UART1_Put_Chars("sys star load!");

	sys_tim_init();	 
   	Key_Init();
  	IIC_Init();
  	moto_Init();  	
	Adc_Init();			
   
   
	if(PPM_open)PPM_Init();

  	delay_100ms(10); //元件上电

 	MPU6050_initialize();
  	if(HMC5883_open)HMC5883L_SetUp();	
  	
    Key_change_e2rom();	//开机常按，重置E2ROM数据
    e2rom_data_read();

	//USART3_Configuration();	//hand-set
	wifi_703n_init();	/* */
	 
	


	UART1_Put_Chars("sys load ok!  /");
  	USART_newline;
	
     //MS5611_Init(); 

    while(1)
    {	
		
		fly_main();




	}
		
	
  
}

/*******************************************************************************
* Function Name  : STM32_LCD_Demo
* Description    : Intializes the STM32 LCD Demo.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SYS_Init(void)
{

  volatile ErrorStatus HSEStartUpStatus = SUCCESS;
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 
 
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
 
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
 
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);
 
    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }
 
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 
    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);	  //必须打开，否则不能复用IO中断
}



/*******************************************************************************
* Function Name  : SysTick_Config
* Description    : Configure a SysTick Base time to 10 ms.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Config(void)
{
  /* Configure HCLK clock as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
 
  /* SysTick interrupt each 100 Hz with HCLK equal to 72MHz */
  SysTick_SetReload(72000);	//1MS


  SysTick_CounterCmd(SysTick_Counter_Enable);
  /* Enable the SysTick Interrupt */
  SysTick_ITConfig(ENABLE);


}



/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length (time base 10 ms).
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(u32 nCount)
{
  TimingDelay = nCount;

  /* Enable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Enable);
  
  while(TimingDelay != 0)
  {
  }

  /* Disable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Disable);

  /* Clear the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Clear);
}



/*******************************************************************************
* Function Name  : Decrement_TimingDelay
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void Decrement_TimingDelay(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}




/*******************************************************************************
* Function Name  : Set_SELStatus
* Description    : Sets the SELStatus variable.
* Input          : None
* Output         : SELStatus
* Return         : None
******************************************************************************
void Set_SELStatus(void)
{
  SELStatus = 1;
}
																			*/
#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description:    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: Lcd_WriteString(0,2,White,Blue, "Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
