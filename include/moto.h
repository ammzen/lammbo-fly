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

#include "fc.h"//moto_PPM_OUT在这个文件里	

#ifdef moto_PPM_OUT	// 未定义,PWM输出	定义,PPM脉宽输出
//四轴电调最小起动脉宽：1.148MS max:1.832MS
#define 	moto_Mul		24
#define 	moto_max 		900
#define 	moto_mix 		0	 //最小可到5
#define 	moto_star		24000//PPM输出最窄脉宽

#define		Period		48000	//有关TIM初始化
#define		Prescaler	2

#else

#define 	moto_Mul		36
#define 	moto_max 		999
#define 	moto_mix 		1	 //最小可到5  //1：因为ADC需要这个触发源
#define 	moto_star		1//PWM输出为0

#define		Period		0x8CA0	 //有关TIM初始化
#define		Prescaler	1

#endif	 //moto_PPM_OUT





void moto_Init(void);

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/



