
#ifndef __COM_H
#define __COM_H
#include "main.h"

#define USART_newline  {UART1_Put_Char(13);UART1_Put_Char(10);}	  //换行

extern unsigned char Uart1_send[10];

void USART_Configuration(void);

void Send_com_signbyte(signed char a);
void Send_com(unsigned char m,unsigned char a[]);
void Send_com_word(unsigned char m,u16 a[]);
void Send_com_byte(unsigned char a);
void Send_com_signword(unsigned char m,s16 a[]);
void Send_com_signbytebuf(unsigned char m,s8 a[]);

void UART1_Put_Char(char a);
void UART1_ReportMotion(int16_t buf[]);

void UART1_Put_Chars(const char *s);
void UART1_Put_Chars_noend(const  char *s);	//发送英文字符段
void UART1_Put_uChars(u8 *s);	//发送英文字符段
void UART1_Put_sChar(char *s);	//发送英文字符
void UART1_Put_int(int m);
void UART1_Put_int_4(int m); 
void UART1_Put_num(char *s,u8 num);	//从地址发送一批数据



 #endif


