
#include "main.h" 

/********
�ֳ�ʽ�豸�趨����
star:
13-05-25    edit by edmondchao

ͨ��Э�飺

��ʾ�豸Ϊ�����豸������Ϊ���豸��ֻ��Ӧ���豸�������������



ͷ + CMD + ���ݳ��� + ���� + У����

ͷ:      0xFD 0xDF

CMD:    01  ����                  
        02  ��������ƶ�PID����������;
        03  ���뵽ִ�ж�PID����������,���洢
        04  ��ǰ��ѹֵ

		0a  WIFI����


        101 ����ִ�гɹ�
        102 ����ִ��ʧ��
        103 PID������  +���ݳ��� + ����                        
        104 
        
У����:  ^��������       

ע�⣺
1,������һ������


***/
struct UART_DATA UT3_RX;//RX������

u8 set_rx_en=0;	//1:��ֹ�������յ���λ����дE2ROM����������������


void handset_Reply(void)//�ظ���λ��
{
	u8 check=0;//У����:  ^�������� 
	u8 lenth=0;//���ݳ���	

	uart_Send_byte(0xFD);  //ͷ:		0xFD 0xDF
	uart_Send_byte(0xDF);  //

	check^=0xDF^0xFD;

	//LED3_Turn; 

	//UT3_RX.ask=4;//������



	if(UT3_RX.ask==1) //1	����
	{
		lenth=1;//���ݳ���

		uart_Send_byte(UT3_RX.ask); //����ԭ����
		uart_Send_byte(lenth); //���ݳ���
			
		uart_Send_byte(101); //����	101 ����ִ�гɹ� 		

		check^=lenth^UT3_RX.ask^101;

		uart_Send_byte(check);

		//UART1_Put_Chars("check=");
		//UART1_Put_int(check);
	}
	else if(UT3_RX.ask==2) //���PID����������
	{
		lenth=sizeof(PID_out);//���ݳ���

		uart_Send_byte(UT3_RX.ask);	  //����ԭ����
		uart_Send_byte(lenth); //���ݳ���

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
	else if(UT3_RX.ask==3) //����PID����������,���洢
	{
	  	lenth=1;//���ݳ���

		uart_Send_byte(UT3_RX.ask); //����ԭ����
		uart_Send_byte(lenth); //���ݳ���

		check^=UT3_RX.ask^lenth;

		{struct PID_out tem;
		 memcpy(&tem, &UT3_RX.data,sizeof(tem));

		 {
			struct PID_out tem1;
			memcpy(&tem1, &tem, sizeof(PID_out));
		 }

		 if(tem.ID==pid_id)//ID��ȷ��
		 {	   		 	
		 		struct PID_out tem1;					   				
				memcpy(&tem1, &PID_out,sizeof(PID_out));	//����

		 		memcpy(&PID_out,&tem, sizeof(PID_out));
		 	    e2rom_StrPID_write(); 
				e2rom_StrPID_read();



				if(PID_out.ID==pid_id)
				{
					PID_data_out();	  //����趨ֵ��PID						
					uart_Send_byte(101); //����	101 ����ִ�гɹ� 
					
					check^=101;					
				}
				else
				{
					memcpy(&PID_out,&tem1, sizeof(PID_out));	//���ݻ�ԭ
				   	uart_Send_byte(102); //����	101 ����ִ�в��ɹ� ���·���	
					
					check^=102;					
				}	 	/**/		
		 }
		 else  
		 {
		 	uart_Send_byte(102); //����	101 ����ִ�в��ɹ� ���·���	

			check^=102;
		 } 		 
		 	
		} 

		uart_Send_byte(check);
		//UART1_Put_Chars("check=");
		//UART1_Put_int(check);
		
	}
	else if(UT3_RX.ask==4) //4 bat
	{
		lenth=2;//���ݳ���
	   	//UT3_RX.bat=bat;
	   	uart_Send_byte(UT3_RX.ask); //����ԭ����    
		uart_Send_byte(lenth); //���ݳ���
			
		uart_Send_byte(bat); //����
		uart_Send_byte(bat>>8); //����

		check^=UT3_RX.ask^lenth^(u8)bat^(bat>>8);

		uart_Send_byte(check);
		//UART1_Put_Chars("bat=");
		//UART1_Put_int(check);
	}

}


void USART2_isr(void)//�ж�
{
    u8 data;

	//LED2_Turn;
	
	data=USART_ReceiveData(USART2);	
	

	if(UT3_RX.status==0)//0:��ʼ���գ�
	{
	    
		UT3_RX.data[UT3_RX.num]=data;
		UT3_RX.num++; //������Ŀ��1

		if(UT3_RX.num==1)//���յ�����ͷ1
		{
		   if(UT3_RX.data[0]==0xFD); //����ͷ1��ȷ
		   else   //��������¿�ʼ
		   {
		   	   	UT3_RX.num=0;
				UT3_RX.status=0;
		   }
		   
		}
		else if(UT3_RX.num>1)//���յ�����ͷ2
		{
		  	UT3_RX.num=0;
			
			if((UT3_RX.data[1]==0xDF)&&(set_rx_en==0)) //����ͷ2��ȷ	���������
			{
				UT3_RX.status=1;
			}  
			else UT3_RX.status=0;

		//	LED2_Turn;
		}
		//
	}
	else if(UT3_RX.status==1)//1��CMD
	{
		 UT3_RX.status=2;
		 if(data==1)//1	����
		 {
		 	 UT3_RX.ask=101;//
		 	 UT3_RX.cmd=data;
		 }
		 else if(data==2)//2���PID����������
		 {
		 	 UT3_RX.ask=102;//�ݴ�����������ݺ�ԭ
		 	 UT3_RX.cmd=data;
		 }
		 else if(data==3)//3	����PID����������,���洢
		 {
		 	 UT3_RX.ask=103;//�ݴ�����������ݺ�ԭ
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
		 else //����Э���ڣ�ֱ���˳�
		 {
		 	UT3_RX.num=0;
			UT3_RX.status=0;
		 	UT3_RX.ask=0;
		 }
		 
		 
	}
	else if(UT3_RX.status==2)//2��data����
	{
	   	if(data<(sizeof(PID_out)+1))	//�����ݷ�Χ��
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
		else //����Э���ڣ�ֱ���˳�
		{
		 	UT3_RX.num=0;
			UT3_RX.status=0;
		 	UT3_RX.ask=0;
		}
	
	}
	else if(UT3_RX.status==3)//3��data
	{
		 //UART1_Put_int(data);

		 UT3_RX.data[UT3_RX.num]=data;
		 UT3_RX.num++;
		 //UART1_Put_int(UT3_RX.num);
		 if(UT3_RX.num>=(UT3_RX.D_num)+1)// ���ݽ������+  У����
		 {
		 		
					/*{u8 a;
					UART1_Put_Chars("data="); 		
					for(a=0;a<UT3_RX.num;a++) UART1_Put_int(UT3_RX.data[a]);				
					}*/

			 //У����
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
			   				//UT3_RX.ask=3;//��ԭ����
							UT3_RX.ask-=100;//��ԭ����
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

void UART2_Put_Chars(const  char *s)	//����Ӣ���ַ���
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
	  
	  UART2_Put_Char(a); //�����ֽ���

	  USART2_newline;
	 // new_line;	//����
}
void UART2_Put_Chars_noend(const  char *s)	//����Ӣ���ַ���
{

	  while(*s)
	  {
	     USART2->DR = *s++;
         while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
         {
         }
	  }	 
	 // USART_newline;
	 // new_line;	//����
}

void uart_Send_char(char a)  //����byte
{
	     USART2->DR = a;
         while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}
void uart_Send_byte(u8 a)  //����byte
{
	     USART2->DR = a;
         while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}


void uart_Send_num(char *s,u8 num)	//�ӵ�ַ����һ������
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
  //����NVIC���ȼ�����ΪGroup2��0-3��ռʽ���ȼ���0-3����Ӧʽ���ȼ� 
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    //���ڽ����жϴ�   
    // �������ȼ� Configure one bit for preemption priority  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure); 
	}	/**/	 
  
}


 /*END-FILE------------------------*/
