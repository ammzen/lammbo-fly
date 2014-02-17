

/********************LAMBO Fly soft********************


star:
13-01-07    edit by edmondchao
13-03-26    从C6更改为ZE
******************************************************/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_it.h"

#include <stdio.h>

#include <string.h>
#include <stdint.h>
#include <math.h>
#include <STDLIB.H>	  //abs





#include "com.h"
#include "RTC_Time.h"




#include "IOI2C.h"
#include "delay.h"
#include "moto.h"

#include "ppm_Parallel.h"
#include "led.h"
#include "key.h"
#include "MPU6050.h"
#include "fc.h"
 
#include "hmc5883.h"
#include "eeprom.h"
#include "my_math.h"
#include "ADC.h"

#include "wifi_703n.h"





/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SYS_Init(void);
//void InterruptConfig(void);
void RCC_Config(void);
void NVIC_Config(void);
//void GPIO_Config(void);
void SysTick_Config(void);
void Delay(u32 nCount);
//u32 DelayJoyStick(u32 nTime);
void Decrement_TimingDelay(void);
void Set_SELStatus(void);
//void CheckBitmapFilesStatus(void);
void IntExtOnOffConfig(FunctionalState NewState);
//void ADC_Config(void);


#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
