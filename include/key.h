#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f10x_map.h"


#define key_port GPIOA
#define key_pin 12


extern u8 key;

#define key_status  1//按键功能重定义标致位 0:单按键模式。1:连续按键模式


//void GPIO_TurnBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);
//void BB(u8 m);
//void BB_LED_Init(void);

void Key_Init(void);
void Key_int_isr(u8 key_num);
void Key_change_e2rom(void);

#endif

