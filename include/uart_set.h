
#ifndef __UARTSET_H
#define __UARTSET_H
#include "main.h"



/********UART_DATA-����************/
struct UART_DATA
{ 
  u8  status;	 //����״̬RX��0:��ʼ���գ�  1��CMD	2��data���ȣ�3��data
  u8  ask;		 //��λ������0:���У�  1����   2 ���PID����������  3	����PID����������,���洢 4 bat
  
  u8  cmd;		//���յ�������
  u8  num;  	//���յ�����Ŀ
  u8  D_num;  	//���յ���������Ŀ
  u16 bat;		//��ѹֵ
  u8 data[30];	//���յ������� 20

};
extern struct UART_DATA UT3_RX;//RX������



void USART3_Configuration(void);
void USART3_isr(void);//�ж�
void Send_com3_byte(u8 a);  //����byte;



 #endif


