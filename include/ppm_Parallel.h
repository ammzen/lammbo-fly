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
#ifndef __PPM_H
#define __PPM_H	


#define Chanl_check  		4	 //设置需要检查通道数
#define Chanl_num_max  		4	 //设置需要检查通道数
#define ppm_ok_num 10

#define PPM_IN_port GPIOA

#define PPM1_in_pin 4
#define PPM2_in_pin 5
#define PPM3_in_pin 6
#define PPM4_in_pin 7

#define PPM1_data_pin  (GPIO_ReadInputDataBit(PPM_IN_port,(1<<PPM1_in_pin)))
#define PPM2_data_pin  (GPIO_ReadInputDataBit(PPM_IN_port,(1<<PPM2_in_pin)))
#define PPM3_data_pin  (GPIO_ReadInputDataBit(PPM_IN_port,(1<<PPM3_in_pin)))
#define PPM4_data_pin  (GPIO_ReadInputDataBit(PPM_IN_port,(1<<PPM4_in_pin)))


#define Chanl_star  0	 //从第几通道开始使用
#define Chanl_num  	4	 //使用几通道
//通道
#define Nick_num    1
#define Roll_num    3
#define Gier_num    0
#define Gas_num     2

//反向
#define Nick_Rev    0
#define Roll_Rev    1
#define Gier_Rev    1
#define Gas_Rev     0 


extern u8 tim_ppm_num;//PPM——TIM计数，0-100循环计数

extern u8 ppm_in[];
extern u8 ppm_data_ok;
//extern u8 channl;
extern u8 ppm_data_ovf;	   //溢出




void ppm_cal(u8 chanl);
void PPM_Init(void);
void No_PPM_check(void);
void PPM_close(void);

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/



