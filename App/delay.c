/* delay.c file

13-07-01���ڶ�ʱ���٣��ｫSYSTIME����Ϊ1MS��ʱ��
��ʱ��������ͨ��ʱ����

edmondchao
------------------------------------ */

#include "delay.h"



void delay_us(u16 nus)
{		
	u16 a; 
	
	for(a=0;a<nus;a++)
	{
		u8 i;
		for(i=6;i>0;i--);
	}
}

void delay_ms(u16 num)
{	 		  	  
	u16  a;

	for(a=0;a<num;a++)
	{
	   delay_us(1000);			   
	}	  	    
} 

void delay_100ms(u16 num)
{	 		  	  
	u16  a;

	for(a=0;a<num;a++)
	{
	   delay_ms(100);			   
	}	  	    
} 

/*	 
static u8  fac_us=0;//us��ʱ������
static u16 fac_ms=0;//ms��ʱ������

//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
*************************ʵ�ֺ���********************************************
*����ԭ��:		void delay_init(u8 SYSCLK)
*��������:		��ʼ���ӳ�ϵͳ��ʹ��ʱ����������״̬
******************************************************************************
void delay_init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2���,ѡ���ⲿʱ��  HCLK/8
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
}		*/		
				    
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void delay_ms(u16 nms)
*��������:		���뼶��ʱ  ��ʱnms  nms<=1864 
******************************************************************************
void delay_ms(u16 nms)	  //<200ms
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL=0x01 ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
}  


void delay_100ms(u16 num)
{	 		  	  
	u16  a;

	for(a=0;a<num;a++)
	{
	   delay_ms(100);			   
	}	  	    
}   		  */

//��ʱnus
//nusΪҪ��ʱ��us��.
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void delay_us(u32 nus)
*��������:		΢�뼶��ʱ  ��ʱnus  nms<=1864 
******************************************************************************		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL=0x01 ;      //��ʼ���� 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}												*/

//------------------End of File----------------------------
