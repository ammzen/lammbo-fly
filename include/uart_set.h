
#ifndef __UARTSET_H
#define __UARTSET_H
#include "main.h"



/********UART_DATA-参数************/
struct UART_DATA
{ 
  u8  status;	 //运行状态RX，0:开始接收，  1，CMD	2，data长度，3，data
  u8  ask;		 //上位机需求，0:空闲，  1连接   2 输出PID可设置数据  3	输入PID可设置数据,并存储 4 bat
  
  u8  cmd;		//接收到的命令
  u8  num;  	//接收到的数目
  u8  D_num;  	//接收到的数据数目
  u16 bat;		//电压值
  u8 data[30];	//接收到的数据 20

};
extern struct UART_DATA UT3_RX;//RX参数。



void USART3_Configuration(void);
void USART3_isr(void);//中断
void Send_com3_byte(u8 a);  //发送byte;



 #endif


