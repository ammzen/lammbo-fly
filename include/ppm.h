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




//#define RF_72_310   
#ifdef	RF_72_310

#define Chanl_star  0	 //从第几通道开始使用
#define Chanl_num  	4	 //从第几通道开始使用
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

#else

#define Chanl_star  1	 //从第几通道开始使用
#define Chanl_num  	4	 //从第几通道开始使用

//通道
#define Nick_num    2
#define Roll_num    4
#define Gier_num    3
#define Gas_num     1

//反向
#define Nick_Rev    1
#define Roll_Rev    1
#define Gier_Rev    0
#define Gas_Rev     1 

#endif



extern u8 ppm_in[];
extern u8 ppm_data_ok;
extern u8 channl;
extern u8 ppm_data_ovf;	   //溢出
#define ppm_ok_num 10



void ppm_cal(void);
void PPM_Init(void);
void No_PPM_check(void);
void PPM_close(void);

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/



