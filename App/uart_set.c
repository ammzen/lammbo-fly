
/********
�ֳ�ʽ�豸�趨����
star:
13-04-01    edit by edmondchao

ͨ��Э�飺

��ʾ�豸Ϊ�����豸������Ϊ���豸��ֻ��Ӧ���豸�������������



ͷ + CMD + ���ݳ��� + ����

ͷ:		0xFD 0xDF

CMD:	01  ���� 					
		02  ���PID����������
		03	����PID����������,���洢
		04  ��ǰ��ѹֵ


		101 ����ִ�гɹ�
		102 ����ִ��ʧ��
		103 PID������ 	+���ݳ��� + ����                        
		104	


***/

#include "main.h" 


struct UART_DATA UT3_RX;//RX������




void USART3_isr(void)//�ж�
{
    u8 data;

	//LED2_Turn;
	
	data=USART_ReceiveData(USART3);	
	

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
			
			if((UT3_RX.data[1]==0xDF)) //����ͷ2��ȷ
			{
				UT3_RX.status=1;
			}  
			else UT3_RX.status=0;

			LED2_Turn;
		}
		//
	}
	else if(UT3_RX.status==1)//1��CMD
	{
		 UT3_RX.status=2;
		 if(data==1)//1	����
		 {
		 	 UT3_RX.ask=data;//
		 
		 }
		 else if(data==2)//2���PID����������
		 {
		 	 UT3_RX.ask=data;//�ݴ�����������ݺ�ԭ
		 
		 }
		 else if(data==3)//3	����PID����������,���洢
		 {
		 	 UT3_RX.ask=103;//�ݴ�����������ݺ�ԭ
		 
		 }
		 else if(data==4)//4 bat
		 {
		 	UT3_RX.ask=4;
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
	   if(data<(sizeof(UT3_RX.data)+1))	//�����ݷ�Χ��
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
		else //����Э���ڣ�ֱ���˳�
		{
		 	UT3_RX.num=0;
			UT3_RX.status=0;
		 	UT3_RX.ask=0;
		}
	
	}
	else if(UT3_RX.status==3)//3��data
	{
		 UT3_RX.data[UT3_RX.num]=data;
		 UT3_RX.num++;
		 if(UT3_RX.num>=UT3_RX.D_num)// ���ݽ������
		 {
		 	 UT3_RX.status=0;
			 if(UT3_RX.ask==103)
			 {
			    //u16 a;

				//a=((UT3_RX.data[19]<<8)+UT3_RX.data[18]); //id 		

			   UT3_RX.ask=3;//��ԭ����
			  
			 //UART1_Put_int((UT3_RX.data[19]<<8)+UT3_RX.data[18]); 
			 }

			UT3_RX.num=0;
			UT3_RX.status=0;
		 }
	}

	//UART1_Put_Chars("Y:");
    //UART1_Put_int(data); 

}



void Send_com3_byte(u8 a)  //����byte
{
	     USART3->DR = a;
         while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}
void Send_com3_word(u8 m,u16 a[])	 //��������
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
  //����NVIC���ȼ�����ΪGroup2��0-3��ռʽ���ȼ���0-3����Ӧʽ���ȼ� 
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    //���ڽ����жϴ�   
    // �������ȼ� Configure one bit for preemption priority  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQChannel; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure); 
	}	/**/	 
  
}

