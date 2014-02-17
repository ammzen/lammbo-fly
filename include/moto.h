/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.h
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 07/22/2008
* Description        : Header for main.c module
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTO_H
#define __MOTO_H

#include "fc.h"//moto_PPM_OUT������ļ���	

#ifdef moto_PPM_OUT	// δ����,PWM���	����,PPM�������
//��������С������1.148MS max:1.832MS
#define 	moto_Mul		24
#define 	moto_max 		900
#define 	moto_mix 		0	 //��С�ɵ�5
#define 	moto_star		24000//PPM�����խ����

#define		Period		48000	//�й�TIM��ʼ��
#define		Prescaler	2

#else

#define 	moto_Mul		36
#define 	moto_max 		999
#define 	moto_mix 		1	 //��С�ɵ�5  //1����ΪADC��Ҫ�������Դ
#define 	moto_star		1//PWM���Ϊ0

#define		Period		0x8CA0	 //�й�TIM��ʼ��
#define		Prescaler	1

#endif	 //moto_PPM_OUT





void moto_Init(void);

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/



