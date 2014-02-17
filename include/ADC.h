#ifndef _ADC_H_
#define _ADC_H_



/*#define batt_adc
#define batt_max 1379
#define batt_mix 1224
#define batt_per 22	*/

//输出值数组中位置
#define batt_place 		0
#define moto1_I_place 	1
#define Moto2_I_place 	2

#define ADC_Channel_max	1			//通道

//对外参数: 2个
extern u16  ADC_Channel_data[ADC_Channel_max];//每个通道的值
extern u8 adc_new_ok;  //当前改变值的通道

//删
extern volatile u16  batt_new_data;

void Adc_Init(void);
void Adc_isr(void);

#endif

