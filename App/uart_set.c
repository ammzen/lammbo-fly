
/********
手持式设备设定参数
star:
13-04-01    edit by edmondchao

通信协议：

显示设备为主动设备，机子为从设备，只响应主设备命令，不主动连接



头 + CMD + 数据长度 + 数据

头:		0xFD 0xDF

CMD:	01  连接 					
		02  输出PID可设置数据
		03	输入PID可设置数据,并存储
		04  当前电压值


		101 命令执行成功
		102 命令执行失败
		103 PID数据组 	+数据长度 + 数据                        
		104	


***/

#include "main.h" 


struct UART_DATA UT3_RX;//RX参数。




void USART3_isr(void)//中断
{
    u8 data;

	//LED2_Turn;
	
	data=USART_ReceiveData(USART3);	
	

	if(UT3_RX.status==0)//0:开始接收，
	{
	    
		UT3_RX.data[UT3_RX.num]=data;
		UT3_RX.num++; //接收数目加1

		if(UT3_RX.num==1)//接收到数据头1
		{
		   if(UT3_RX.data[0]==0xFD); //数据头1正确
		   else   //错误刚重新开始
		   {
		   	   	UT3_RX.num=0;
				UT3_RX.status=0;
		   }
		   
		}
		else if(UT3_RX.num>1)//接收到数据头2
		{
		  	UT3_RX.num=0;
			
			if((UT3_RX.data[1]==0xDF)) //数据头2正确
			{
				UT3_RX.status=1;
			}  
			else UT3_RX.status=0;

			LED2_Turn;
		}
		//
	}
	else if(UT3_RX.status==1)//1，CMD
	{
		 UT3_RX.status=2;
		 if(data==1)//1	连接
		 {
		 	 UT3_RX.ask=data;//
		 
		 }
		 else if(data==2)//2输出PID可设置数据
		 {
		 	 UT3_RX.ask=data;//暂存命令，收完数据后复原
		 
		 }
		 else if(data==3)//3	输入PID可设置数据,并存储
		 {
		 	 UT3_RX.ask=103;//暂存命令，收完数据后复原
		 
		 }
		 else if(data==4)//4 bat
		 {
		 	UT3_RX.ask=4;
		 }
		 else //不在协义内，直接退出
		 {
		 	UT3_RX.num=0;
			UT3_RX.status=0;
		 	UT3_RX.ask=0;
		 }
		 
		 
	}
	else if(UT3_RX.status==2)//2，data长度
	{
	   if(data<(sizeof(UT3_RX.data)+1))	//在数据范围内
	   {
		 if(data==0)
		 {
		 	UT3_RX.status=0;
		 }
		 else
		 {
		 	UT3_RX.num=0;
			UT3_RX.D_num=data;
			UT3_RX.status=3;
		 }
		}
		else //不在协义内，直接退出
		{
		 	UT3_RX.num=0;
			UT3_RX.status=0;
		 	UT3_RX.ask=0;
		}
	
	}
	else if(UT3_RX.status==3)//3，data
	{
		 UT3_RX.data[UT3_RX.num]=data;
		 UT3_RX.num++;
		 if(UT3_RX.num>=UT3_RX.D_num)// 数据接收完成
		 {
		 	 UT3_RX.status=0;
			 if(UT3_RX.ask==103)
			 {
			    //u16 a;

				//a=((UT3_RX.data[19]<<8)+UT3_RX.data[18]); //id 		

			   UT3_RX.ask=3;//还原命令
			  
			 //UART1_Put_int((UT3_RX.data[19]<<8)+UT3_RX.data[18]); 
			 }

			UT3_RX.num=0;
			UT3_RX.status=0;
		 }
	}

	//UART1_Put_Chars("Y:");
    //UART1_Put_int(data); 

}



void Send_com3_byte(u8 a)  //发送byte
{
	     USART3->DR = a;
         while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}
void Send_com3_word(u8 m,u16 a[])	 //发送数组
{
    unsigned char n;

	for(n=0;n<m;n++)
	{
	     USART3->DR = *a>>8;
         while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		 USART1->DR = *a;
         while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		 a++;
	}
}





void USART3_Configuration(void)
{

  
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  //USART1_TX 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
    
    //USART1_RX 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    GPIO_Init(GPIOB, &GPIO_InitStructure); 

/* USART1 configuration ------------------------------------------------------*/
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
  USART_InitStructure.USART_BaudRate = 256000;	//115200;	//9600;//	921600;	//	   
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  

    USART_Init(USART3, &USART_InitStructure);
    
  /* Enable USART1 */
    USART_Cmd(USART3, ENABLE);

	
	{
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  
  //设置NVIC优先级分组为Group2：0-3抢占式优先级，0-3的响应式优先级 
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    //串口接收中断打开   
    // 配置优先级 Configure one bit for preemption priority  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQChannel; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure); 
	}	/**/	 
  
}

