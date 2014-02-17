


#include "main.h"


//u8 BB_EN=0;
    
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Init(void)
*��������:		��ʼ��I2C��Ӧ�Ľӿ����š�
*******************************************************************************/
void led_Init(void)
{			
	GPIO_InitTypeDef GPIO_InitStructure;
	//led
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  
   GPIO_InitStructure.GPIO_Pin = led1_pin|led2_pin|led3_pin|led4_pin ;//| GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

   GPIO_Init(led_port, &GPIO_InitStructure);

  // LED1_OFF;
  // LED2_OFF;
} 
/*************************************************
	TIM2 Configuration: Output Compare Timing Mode:
    TIM2CLK = 72 MHz, Prescaler = 0x8CA0(36000), TIM2 counter clock =1KHz   

	 72/36000=2   TIM_Prescaler=1+1=2--> 1KHz
	 72M/0.4K	=180K  180K/36000=5	---> TIM_Prescaler=5-1=4
	 72M/500Hz	=144K  144K/5=28800	---> TIM_Prescaler=4  TIM_Period=28800
***************************************************/
//400Hz������
void  sys_tim_init(void)
{
	//�ѷ�����1000HZ��

   


}


////����MPU6050ȡֵ	 tim3 Ϊ400HZ  ȡ��Ϊ400HZ  
volatile u8  MPU6050_delay=0;
u16 sys_2_5ms_num=0;
u8 check_err=99;//˨���������ʶ 99���ڵ�һ�μ�� 
u16 dub_show_delay=0;

#define work_delay	49 //��������Ƶ��2.5*100=250MS
u16 led_delay=work_delay; //2.5*100=250MS	 //led��˸ Ƶ��

u8 ppm_delay=0;

u8 led_on_off=0;//0,��LED ��1
u8 led_on_num=0; //����ʱ��	 ����
u8 led_off_num=0;

u8 hand_set_1s=0;  //hand_set�β���
u16 hand_set_num=0;//1s��ʱ
u16 wifi_check_num=0;

const u16 led_ON_OFF_num[][3]={
/*
��,���ظ�������1������ʱ��
*/
0,10,50, //  work	  ������

0,0,400, //1,RF����	  һ��һ�� ��Ƶ��
2,5,50,	 //2,�͵����	  ������

0,10,50, //  work	  ������
};

void sys_tim_isr(void)	//500hz 2ms
{
	 //LED2_Turn;
	 if(check_err!=Main_ERR)
	 {
		led_delay=led_ON_OFF_num[Main_ERR][2];//1s
		sys_2_5ms_num=0;  
		led_off_num=0;
		led_on_num=0;
		//led_on_off=1;	//�ȷ�תһ��
		LED1_ON;	//�ȷ�תһ��

	   	check_err=Main_ERR;

		//UART1_Put_Chars("check_err");
		//UART1_Put_int(check_err); 
	 }
	 //LED				    
	 if(sys_star_ok)
	 {
	 	if(sys_2_5ms_num<led_delay)sys_2_5ms_num++;//2.5*100
	 	else
	 	{
	 		sys_2_5ms_num=0;

			if(led_on_off==0)
			{
			   LED1_OFF; // LED2_OFF;LED3_OFF;LED4_OFF;
			   if(led_off_num)led_off_num--;
			   else {led_on_off=1;	led_off_num=led_ON_OFF_num[Main_ERR][1];}
			}
			else 
			{
			   LED1_Turn;//LED2_Turn;LED3_Turn;LED4_Turn;
			   if(led_on_num)led_on_num--;
			   else {led_on_off=0;	led_on_num=led_ON_OFF_num[Main_ERR][0];}		
			}
			//LED1_Turn;	//����ʾ�����幤������
	 	} /**/
	 }
	

	 if(RF_ctrl_num<14)RF_ctrl_num++;//��������ʱ�����Ϊ30MS����һ��PPM_MAIN_WORK

	 if(wifi_check_num<199)wifi_check_num++;
	 else
	 {
	 	 
	 	 wifi_check_num=0;
	 	 if(wifi_RFok_num)wifi_RFok_num--;//��ȷ����WIFI�źŴ�����	������ʱ�ÿ200MS��1��Ϊ0ʱ��ʧ��
	 }
	 
	 
	 	 

	 ms_2_en=1;//500HZ �ź�  ������ʱ���sys_tim_isr(void)��	//500hz 2ms

	 //PPM
	 if(ppm_delay<49)ppm_delay++;//50*2=100MS
	 else
	 {
	    ppm_delay=0;
	 	No_PPM_check();
	 }

	 //DUG_SHOW�����ֵ	
	 if(dub_show_delay<249)dub_show_delay++;	//500ms
	 else
	 {
	 	dub_show_delay=0;
	 
	 	dub_show_en=1;	 //����DUG_SHOW�����ֵ	  200ms��������ʱ��
	 }

	 //HMC5883	 
	 HMC5883_check_work();

	 if(hand_set_in==1)// 1,hand_set����
	 {
	 	if(hand_set_num<499)hand_set_num++;
	 	else
	 	{
			hand_set_num=0;
	 		hand_set_1s=1;  //hand_set�β���
			//LED5_Turn;	 
	 	}
	 }  
	 
}





//------------------End of File----------------------------
