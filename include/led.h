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
#ifndef __LED_H
#define __LED_H


#define led_port GPIOB
/*
#define led1_pin (1<<1)
#define led2_pin (1<<13)
#define led3_pin (1<<12)
#define led4_pin (1<<2)
*/
#define led1_pin (1<<1)
#define led2_pin (1<<2)
#define led3_pin (1<<13)
#define led4_pin (1<<12)


#define LED1_ON  (led_port->ODR &= ~led1_pin)
#define LED2_ON  (led_port->ODR &= ~led2_pin)
#define LED1_OFF  (led_port->ODR |= led1_pin)
#define LED2_OFF  (led_port->ODR |= led2_pin)
#define LED1_Turn  (led_port->ODR ^= led1_pin)
#define LED2_Turn  (led_port->ODR ^= led2_pin)

#define LED3_ON  	(led_port->ODR &= ~led3_pin)
#define LED3_OFF  	(led_port->ODR |= led3_pin)
#define LED3_Turn  	(led_port->ODR ^= led3_pin)

#define LED4_ON   	(led_port->ODR &= ~led4_pin)
#define LED4_OFF  	(led_port->ODR |= led4_pin)
#define LED4_Turn  	(led_port->ODR ^= led4_pin)





extern u8 hand_set_1s;  //hand_set°Î²å¼ì²â
extern u16 hand_set_num;//1s¼ÆÊ±


void led_Init(void);

void sys_tim_init(void);
void sys_tim_isr(void);






#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/



