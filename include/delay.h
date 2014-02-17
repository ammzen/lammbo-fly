#ifndef __DELAY_H
#define __DELAY_H 			   
#include "main.h"

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u16 nus);
void delay_100ms(u16 num);

#endif

//------------------End of File----------------------------
