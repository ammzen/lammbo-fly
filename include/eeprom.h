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
#ifndef __EEPROM_H
#define __EEPROM_H

#define e2rom_use	0

#define HMC_maxx	1
#define HMC_minx	2   
#define HMC_maxy	3
#define HMC_miny	4
#define HMC_maxz	5
#define HMC_minz	6

#define AngX_midZ	7
#define AngY_midZ	8
#define AngZ_midZ	9
#define AccX_midZ	10
#define AccY_midZ	11
#define AccZ_midZ	12

//#define smart_record_mark_e2num 7



extern u8  e2rom_ask;
//extern u8  e2_rom_w_mark;		//是否是第一次开机

extern u16 e2prom_tem[];

u8 MemReadByte(u16 *data,u16 num,u32 page_addr);
u8 MemWriteByte(u16 *data,u16 num,u32 temp_addr);
void e2rom_data_write(void);
void e2rom_data_read(void);

void test_e2rom_write_read(void);

void e2rom_mpu6050_read(void);

void e2rom_StrPID_write(void);
void e2rom_StrPID_read(void);




#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/



