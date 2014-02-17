
#ifndef __WIFI_703N_H
#define __WIFI_703N_H
#include "main.h"

#define USART2_newline  {UART2_Put_Char(13);UART2_Put_Char(10);}	  //����


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




extern u8 set_rx_en;	//1:��ֹ�������յ���λ����дE2ROM�������



void USART2_Configuration(void);

void wifi_703n_init(void);
void USART2_Put_Char(char a);
void UART2_Put_Chars(const  char *s);
void UART2_Put_Char(char a);
void uart_Send_byte(u8 a);  //����byte
void handset_Reply(void);//�ظ���λ��
void USART2_isr(void);//�ж�
void uart_Send_num(char *s,u8 num);	//�ӵ�ַ����һ������






#endif


