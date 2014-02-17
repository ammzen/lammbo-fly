
#ifndef __WIFI_703N_H
#define __WIFI_703N_H
#include "main.h"

#define USART2_newline  {UART2_Put_Char(13);UART2_Put_Char(10);}	  //换行


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




extern u8 set_rx_en;	//1:禁止，由于收到上位机后，写E2ROM，需禁掉



void USART2_Configuration(void);

void wifi_703n_init(void);
void USART2_Put_Char(char a);
void UART2_Put_Chars(const  char *s);
void UART2_Put_Char(char a);
void uart_Send_byte(u8 a);  //发送byte
void handset_Reply(void);//回复上位机
void USART2_isr(void);//中断
void uart_Send_num(char *s,u8 num);	//从地址发送一批数据






#endif


