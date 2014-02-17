
#include "main.h" 

/********
手持式设备设定参数
star:
13-05-25    edit by edmondchao

通信协议：

显示设备为主动设备，机子为从设备，只响应主设备命令，不主动连接



头 + CMD + 数据长度 + 数据 + 校验码

头:      0xFD 0xDF

CMD:    01  连接                  
        02  输出到控制端PID可设置数据;
        03  输入到执行端PID可设置数据,并存储
        04  当前电压值

		0a  WIFI控制


        101 命令执行成功
        102 命令执行失败
        103 PID数据组  +数据长度 + 数据                        
        104 
        
校验码:  ^所有数据       

注意：
1,必需有一个数据


***/
struct UART_DATA UT3_RX;//RX参数。

u8 set_rx_en=0;	//1:禁止，由于收到上位机后，写E2ROM，需禁掉电机及接收


void handset_Reply(void)//回复上位机
{
	u8 check=0;//校验码:  ^所有数据 
	u8 lenth=0;//数据长度	

	uart_Send_byte(0xFD);  //头:		0xFD 0xDF
	uart_Send_byte(0xDF);  //

	check^=0xDF^0xFD;

	//LED3_Turn; 

	//UT3_RX.ask=4;//测试用



	if(UT3_RX.ask==1) //1	连接
	{
		lenth=1;//数据长度

		uart_Send_byte(UT3_RX.ask); //返回原命令
		uart_Send_byte(lenth); //数据长度
			
		uart_Send_byte(101); //数据	101 命令执行成功 		

		check^=lenth^UT3_RX.ask^101;

		uart_Send_byte(check);

		//UART1_Put_Chars("check=");
		//UART1_Put_int(check);
	}
	else if(UT3_RX.ask==2) //输出PID可设置数据
	{
		lenth=sizeof(PID_out);//数据长度

		uart_Send_byte(UT3_RX.ask);	  //返回原命令
		uart_Send_byte(lenth); //数据长度

		check^=UT3_RX.ask^lenth;

		{ u8 a;	u8 b[sizeof(PID_out)];
		  
		  memcpy(&b[0],&PID_out,sizeof(PID_out));
		  for(a=0;a<(sizeof(PID_out));a++)
		  {
		  	   uart_Send_byte(b[a]);
			   //UART1_Put_int(b[a]);
			   check^=b[a];
		  }
		}

		uart_Send_byte(check);
		//UART1_Put_Chars("check=");
		//UART1_Put_int(check);
	}
	else if(UT3_RX.ask==3) //输入PID可设置数据,并存储
	{
	  	lenth=1;//数据长度

		uart_Send_byte(UT3_RX.ask); //返回原命令
		uart_Send_byte(lenth); //数据长度

		check^=UT3_RX.ask^lenth;

		{struct PID_out tem;
		 memcpy(&tem, &UT3_RX.data,sizeof(tem));

		 {
			struct PID_out tem1;
			memcpy(&tem1, &tem, sizeof(PID_out));
		 }

		 if(tem.ID==pid_id)//ID正确，
		 {	   		 	
		 		struct PID_out tem1;					   				
				memcpy(&tem1, &PID_out,sizeof(PID_out));	//备份

		 		memcpy(&PID_out,&tem, sizeof(PID_out));
		 	    e2rom_StrPID_write(); 
				e2rom_StrPID_read();



				if(PID_out.ID==pid_id)
				{
					PID_data_out();	  //输出设定值到PID						
					uart_Send_byte(101); //数据	101 命令执行成功 
					
					check^=101;					
				}
				else
				{
					memcpy(&PID_out,&tem1, sizeof(PID_out));	//备份还原
				   	uart_Send_byte(102); //数据	101 命令执行不成功 重新发送	
					
					check^=102;					
				}	 	/**/		
		 }
		 else  
		 {
		 	uart_Send_byte(102); //数据	101 命令执行不成功 重新发送	

			check^=102;
		 } 		 
		 	
		} 

		uart_Send_byte(check);
		//UART1_Put_Chars("check=");
		//UART1_Put_int(check);
		
	}
	else if(UT3_RX.ask==4) //4 bat
	{
		lenth=2;//数据长度
	   	//UT3_RX.bat=bat;
	   	uart_Send_byte(UT3_RX.ask); //返回原命令    
		uart_Send_byte(lenth); //数据长度
			
		uart_Send_byte(bat); //数据
		uart_Send_byte(bat>>8); //数据

		check^=UT3_RX.ask^lenth^(u8)bat^(bat>>8);

		uart_Send_byte(check);
		//UART1_Put_Chars("bat=");
		//UART1_Put_int(check);
	}

}


void USART2_isr(void)//中断
{
    u8 data;

	//LED2_Turn;
	
	data=USART_ReceiveData(USART2);	
	

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
			
			if((UT3_RX.data[1]==0xDF)&&(set_rx_en==0)) //数据头2正确	且允许接收
			{
				UT3_RX.status=1;
			}  
			else UT3_RX.status=0;

		//	LED2_Turn;
		}
		//
	}
	else if(UT3_RX.status==1)//1，CMD
	{
		 UT3_RX.status=2;
		 if(data==1)//1	连接
		 {
		 	 UT3_RX.ask=101;//
		 	 UT3_RX.cmd=data;
		 }
		 else if(data==2)//2输出PID可设置数据
		 {
		 	 UT3_RX.ask=102;//暂存命令，收完数据后复原
		 	 UT3_RX.cmd=data;
		 }
		 else if(data==3)//3	输入PID可设置数据,并存储
		 {
		 	 UT3_RX.ask=103;//暂存命令，收完数据后复原
		 	 UT3_RX.cmd=data;
		 }
		 else if(data==4)//4 bat
		 {
		 	UT3_RX.ask=104;
			UT3_RX.cmd=data;
		 }
		 else if(data==0x0a)//4 bat
		 {
		 	UT3_RX.ask=110;
			UT3_RX.cmd=data;
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
	   	if(data<(sizeof(PID_out)+1))	//在数据范围内
	   	{
		 	//if(data==0)
		 	{
		 	//UT3_RX.status=3;
		 	}
		 	//else
		 	{
		 	//UT3_RX.num++;
			UT3_RX.D_num=data;
			UT3_RX.status=3;

			//UART1_Put_Chars("lenth=");
			//UART1_Put_int(UT3_RX.D_num);
			//UART1_Put_int(UT3_RX.num);
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
		 //UART1_Put_int(data);

		 UT3_RX.data[UT3_RX.num]=data;
		 UT3_RX.num++;
		 //UART1_Put_int(UT3_RX.num);
		 if(UT3_RX.num>=(UT3_RX.D_num)+1)// 数据接收完成+  校验码
		 {
		 		
					/*{u8 a;
					UART1_Put_Chars("data="); 		
					for(a=0;a<UT3_RX.num;a++) UART1_Put_int(UT3_RX.data[a]);				
					}*/

			 //校验码
			 {
			 		u8 k=0;
					k^=	0xFD^0xDF^UT3_RX.cmd^UT3_RX.D_num;
			 
			 		if(UT3_RX.D_num)
					{
						u8 a;  for(a=0;a<UT3_RX.D_num;a++)k^=UT3_RX.data[a];
					}

					//UART1_Put_Chars("check="); 	 UART1_Put_int(k);	UART1_Put_int(UT3_RX.data[UT3_RX.num-1]);
			 		
					if(k == UT3_RX.data[UT3_RX.num-1])
					{
					   //UART1_Put_Chars("check ok!"); 
					   //LED2_Turn;

					   	//if(UT3_RX.ask==103)	 W_PPM
			 			{
			   				//UT3_RX.ask=3;//还原命令
							UT3_RX.ask-=100;//还原命令
			 			}
						if(UT3_RX.ask==0x0a)
						{
							memcpy(&W_PPM,&UT3_RX.data,UT3_RX.D_num);
							W_PPM.num=1;
						}
							
					}
			 }
		 	 //if(
			 
			UT3_RX.cmd=0; 
			UT3_RX.D_num=0;
			UT3_RX.num=0;
			UT3_RX.status=0;
		 }
	}

	//UART1_Put_Chars("Y:");
    //UART1_Put_int(data); 

}

void UART2_Put_Chars(const  char *s)	//发送英文字符段
{	
	u8 a=0;
	  
	  while(*s)
	  {
	     USART2->DR = *s++;
         while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
         {
         }
		 a++;
	  }	
	  
	  UART2_Put_Char(a); //发送字节数

	  USART2_newline;
	 // new_line;	//换行
}
void UART2_Put_Chars_noend(const  char *s)	//发送英文字符段
{

	  while(*s)
	  {
	     USART2->DR = *s++;
         while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
         {
         }
	  }	 
	 // USART_newline;
	 // new_line;	//换行
}

void uart_Send_char(char a)  //发送byte
{
	     USART2->DR = a;
         while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}
void uart_Send_byte(u8 a)  //发送byte
{
	     USART2->DR = a;
         while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}


void uart_Send_num(char *s,u8 num)	//从地址发送一批数据
{
	     u8 a=0;
		 
		 for(a=0;a<num;a++)
		 {
		 	uart_Send_char(*s++); 			
		 }	
}

void UART2_Put_Char(char a)
{
    

	     USART2->DR = a;
         while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
         {
         }
	
}


void wifi_703n_init(void)
{
  	USART2_Configuration();


}


void USART2_Configuration(void)
{ 
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  //USART1_TX 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
    //USART1_RX 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

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
  USART_InitStructure.USART_BaudRate = 115200;	//9600;//256000;	//	921600;	//	   
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  

    USART_Init(USART2, &USART_InitStructure);
    
  /* Enable USART1 */
    USART_Cmd(USART2, ENABLE);

	
	{
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  
	//USART_ITConfig(USART2, USART_IT_TC, ENABLE); 
  //设置NVIC优先级分组为Group2：0-3抢占式优先级，0-3的响应式优先级 
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    //串口接收中断打开   
    // 配置优先级 Configure one bit for preemption priority  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure); 
	}	/**/	 
  
}


 /*END-FILE------------------------*/
