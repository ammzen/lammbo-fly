
#include "main.h" 

unsigned char Uart1_send[10]={
0,2,3,4,5,6,7,8,9,10,
};

/**************************实现函数********************************************
*函数原型:		void UART1_ReportMotion(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz,
					int16_t hx,int16_t hy,int16_t hz)
*功　　能:		向上位机发送当前传感器的输出值
输入参数：
	int16_t ax  加速度 X轴ADC输出 范围 ：一个有符号整型
	int16_t ay  加速度 Y轴ADC输出 范围 ：一个有符号整型
	int16_t az  加速度 Z轴ADC输出 范围 ：一个有符号整型
	int16_t gx  陀螺仪 X轴ADC输出 范围 ：一个有符号整型
	int16_t gy  陀螺仪 Y轴ADC输出 范围 ：一个有符号整型
	int16_t gz  陀螺仪 Z轴ADC输出 范围 ：一个有符号整型
	int16_t hx  磁罗盘 X轴ADC输出 范围 ：一个有符号整型
	int16_t hy  磁罗盘 Y轴ADC输出 范围 ：一个有符号整型
	int16_t hz  磁罗盘 Z轴ADC输出 范围 ：一个有符号整型
	
输出参数：没有	
******************************************************************************
void UART1_ReportMotion(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz,
					int16_t hx,int16_t hy,int16_t hz)	 */
void UART1_ReportMotion(int16_t buf[])
{
 	unsigned int temp=0xaF+9;
	char ctemp;
	u8 a;
	UART1_Put_Char(0xa5);
	UART1_Put_Char(0x5a);
	UART1_Put_Char(14+8);
	UART1_Put_Char(0xA2);

	for(a=0;a<9;a++)
	{
	   // buf[a]=buf[a]>>6;

		buf[a]=(sensor_6050in_old[a]+ buf[a])/2;
		sensor_6050in_old[a]= buf[a];

	  	if(buf[a]<0)buf[a]=32768-buf[a];
		ctemp=buf[a]>>8;
		UART1_Put_Char(ctemp);
		temp+=ctemp;
		ctemp=buf[a];
		UART1_Put_Char(ctemp);
		temp+=ctemp;
	}


	UART1_Put_Char(temp%256);
	UART1_Put_Char(0xaa);
}

char *char_num="0123456789abcdef";
/**************/
/***发送到串口*/
/**************/

void UART1_Put_int(int m)	 //0xffff
{


     u16 a=0,b=0,c=0,d=0,e=0;

      if(m<0) 
	  {
	    UART1_Put_sChar("-");
		m=fabs(m);
	  }
      a=m/10000;
	  b=m%10000;
	  c=b%1000;
	  d=c%100;
	  e=d%10;
	  b=b/1000;
	  c=c/100;
	  d=d/10;
	  

	  if(a)UART1_Put_sChar(char_num+a);
	  if(b|a)UART1_Put_sChar(char_num+b);
	  if(c|a|b)UART1_Put_sChar(char_num+c);
	  if(d|c|a|b)UART1_Put_sChar(char_num+d);
	  UART1_Put_sChar(char_num+e);

	 
	  USART_newline;	//换行
}

void UART1_Put_uChars(u8 *s)	//发送英文字符段
{

	  while(*s)
	  {
	     USART1->DR = *s++;
         while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	  }	 
	  USART_newline;
	 // new_line;	//换行
}
void UART1_Put_Chars(const  char *s)	//发送英文字符段
{

	  while(*s)
	  {
	     USART1->DR = *s++;
         while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	  }	 
	  USART_newline;
	 // new_line;	//换行
}
void UART1_Put_Chars_noend(const  char *s)	//发送英文字符段
{

	  while(*s)
	  {
	     USART1->DR = *s++;
         while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
         {
         }
	  }	 
	 // USART_newline;
	 // new_line;	//换行
}

void UART1_Put_sChar(char *s)	//发送英文字符
{
	     USART1->DR = *s;
         while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
         {
         }
}
void UART1_Put_Char(char a)
{
	     USART1->DR = a;
         while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
}
void UART1_Put_num(char *s,u8 num)	//从地址发送一批数据
{
	     u8 a=0;
		 
		 for(a=0;a<num;a++)
		 {
		 	UART1_Put_Char(*s++); 			
		 }	
}



void USART_Configuration(void)
{

  //NVIC_InitTypeDef NVIC_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

  //USART1_TX 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
    
/*    //USART1_RX 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

 USART1 configuration ------------------------------------------------------*/
  /* USART1 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the middle 
        - USART LastBit: The clock pulse of the last data bit is not output to 
                         the SCLK pin
  */
  USART_InitStructure.USART_BaudRate = 256000;	//	921600;	//	115200;	   
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode =  USART_Mode_Tx; //USART_Mode_Rx |
  

    USART_Init(USART1, &USART_InitStructure);
   
   
    //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  
  /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);

  //设置NVIC优先级分组为Group2：0-3抢占式优先级，0-3的响应式优先级 
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    //串口接收中断打开   
    /* 配置优先级 Configure one bit for preemption priority 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure); 	 */	 
  
}






