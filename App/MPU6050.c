/* MPU6050.c file */
/********************LAMBO Fly soft********************


star:
13-01-07    edit by edmondchao

备注：   MPU6050需二次初始化。否则数据不稳定
*******************************************************************************/

#include "main.h"


uint8_t buffer[14];

volatile signed short int  AdWertNick = 0, AdWertRoll = 0, AdWertGier = 0;//?从陀螺仪传感器读出值
volatile signed short int  AdWertAngRoll = 0,AdWertAngNick = 0,AdWertAngHoch = 0;///?从角度传感器读出值
//volatile int NeutralAccX=0, NeutralAccY=0,NeutralAccZ = 0;//?陀螺计数值 //中立值

//int16_t Gx_offset=0,Gy_offset=0,Gz_offset=0;
//float Acc1G_Values;

u8 MPU6050_en=0;   //MPU6050_en=1;	 //1MS事件处理
int16_t sensor_6050in_old[9];



struct mpu6050_info S0_6050;//mpu6050的原始输出值。
struct mpu6050_info corr_6050;//mpu6050的纠正值。 correct


/**************************实现函数********************************************
*函数原型:		void MPU6050_setClockSource(uint8_t source)
*功　　能:	    设置  MPU6050 的时钟源
 * CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
*******************************************************************************/
void MPU6050_setClockSource(uint8_t source){
    IICwriteBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

/** Set full-scale gyroscope range.
 * @param range New full-scale gyroscope range value
 * @see getFullScaleRange()
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_RA_GYRO_CONFIG
 * @see MPU6050_GCONFIG_FS_SEL_BIT
 * @see MPU6050_GCONFIG_FS_SEL_LENGTH
 */
void MPU6050_setFullScaleGyroRange(uint8_t range) {
    IICwriteBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setFullScaleAccelRange(uint8_t range)
*功　　能:	    设置  MPU6050 加速度计的最大量程
*******************************************************************************/
void MPU6050_setFullScaleAccelRange(uint8_t range) {
    IICwriteBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setSleepEnabled(uint8_t enabled)
*功　　能:	    设置  MPU6050 是否进入睡眠模式
				enabled =1   睡觉
			    enabled =0   工作
*******************************************************************************/
void MPU6050_setSleepEnabled(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

/**************************实现函数********************************************
*函数原型:		uint8_t MPU6050_getDeviceID(void)
*功　　能:	    读取  MPU6050 WHO_AM_I 标识	 将返回 0x68
*******************************************************************************/
uint8_t MPU6050_getDeviceID(void) {

    IICreadBytes(devAddr, MPU6050_RA_WHO_AM_I, 1, buffer);
    return buffer[0];
}

/**************************实现函数********************************************
*函数原型:		uint8_t MPU6050_testConnection(void)
*功　　能:	    检测MPU6050 是否已经连接
*******************************************************************************/
uint8_t MPU6050_testConnection(void) {
   if(MPU6050_getDeviceID() == 0x68)  //0b01101000;
   {

	//LED1_Turn;
   return 1;

   }
   	else return 0;
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
*******************************************************************************/
void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
*******************************************************************************/
void MPU6050_setI2CBypassEnabled(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

void MPU6050_intio_Init(void)
{
   	GPIO_InitTypeDef GPIO_InitStructure;

	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin =  MPU6050_pin;
	                              
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  	
  	GPIO_Init(MPU6050_port, &GPIO_InitStructure);


}

/**************************实现函数********************************************
*函数原型:		void MPU6050_initialize(void)
*功　　能:	    初始化 	MPU6050 以进入可用状态。
*******************************************************************************/
void MPU6050_initialize(void) 
{
//	int16_t temp[6];
//	unsigned char i;

	MPU6050_intio_Init();


    MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO); //设置时钟
    MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);//陀螺仪最大量程 +-2000度每秒
    MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_16);	//加速度度最大量程 +-2G
    MPU6050_setSleepEnabled(0); //进入工作状态
	MPU6050_setI2CMasterModeEnabled(0);	 //不让MPU6050 控制AUXI2C
	MPU6050_setI2CBypassEnabled(1);	 //主控制器的I2C与	MPU6050的AUXI2C	直通。控制器可以直接访问HMC5883L
	//配置MPU6050 的中断模式 和中断电平模式
	//当此位等于0，INT引脚的逻辑电平为高电平。当此位等于1，INT引脚的逻辑电平为低电平有效。////(1)
	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_LEVEL_BIT, 1);
	//当此位等于0，INT引脚被配置为推挽。当此位等于1，INT引脚配置为漏极开路。
	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_OPEN_BIT, 0);
	//当此位等于0，INT引脚发出需50uS的长脉冲。当此位等于1，INT引脚保持前面(1)定义电平，直到中断清除。
	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_LATCH_INT_EN_BIT, 1);
	//当此位等于0，中断状态位被清零，只能通过阅读INT_STATUS（注册58）当此位等于1，任何读中断状态位被清零操作。
	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_RD_CLEAR_BIT, 1);
	//开数据转换完成中断
    IICwriteBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_DATA_RDY_BIT, 1);

	//MPU6050_en=1;

}

/**************************实现函数********************************************
*函数原型:		unsigned char MPU6050_is_DRY(void)
*功　　能:	    检查 MPU6050的中断引脚，测试是否完成转换
返回 1  转换完成
0 数据寄存器还没有更新
*******************************************************************************/
unsigned char MPU6050_is_DRY(void)
{
    if(GPIO_ReadInputDataBit(MPU6050_port,MPU6050_pin)==0)return 1;
	else return 0;
}

u8 MPU6050_read_ok=0; 
volatile signed short int  AdWert_Nick = 0, AdWert_Roll = 0, AdWert_Gier = 0;//?从陀螺仪传感器读出值
volatile signed short int  AdWert_AngRoll = 0,AdWert_AngNick = 0,AdWert_AngHoch = 0;///?从角度传感器读出值
s16 AngX_mid=0,AngY_mid=0,AngZ_mid= 0;//?陀螺计数值 //中立值
s16 AccX_mid=0,AccY_mid=0,AccZ_mid= 0;//?陀螺计数值 //中立值

u8 mpu6050_star=0;//=0: 第一次启动
void MPU6050_getMotion(void)
{
   // LED2_Turn;
    if(MPU6050_is_DRY()) 
	{
	 
	  //LED4_Turn;

	 IICreadBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 14, buffer);

	AdWertAngNick=(((((int16_t)buffer[0]) << 8) | buffer[1])); //角度
    AdWertAngRoll=(((((int16_t)buffer[2]) << 8) | buffer[3]));	 //角度
    //AdWertAngHoch=(((((int16_t)buffer[4]) << 8) | buffer[5]));  //角度
	//跳过温度ADC
    AdWertRoll=(((((int16_t)buffer[8]) << 8) | buffer[9]));		 //加速度
    AdWertNick=(((((int16_t)buffer[10]) << 8) | buffer[11]));	 //加速度
    AdWertGier=(((((int16_t)buffer[12]) << 8) | buffer[13])); 	 //加速度

#define r_move  5
	#define acc_num 3
	if(mpu6050_star)
	{
	AdWert_AngRoll=  (AdWert_AngRoll*acc_num + AdWertAngRoll)/(acc_num+1);
    AdWert_AngNick=  (AdWert_AngNick*acc_num + AdWertAngNick)/(acc_num+1);
    //AdWert_AngHoch=  (AdWert_AccHoch*acc_num + AdWertAccHoch)/(acc_num+1);
	/*	//跳过温度ADC
    AdWert_Roll=  (AdWert_Roll*1 + AdWertRoll)/2;
    AdWert_Nick=  (AdWert_Nick*1 + AdWertNick)/2;
    AdWert_Gier=  (AdWert_Gier*1 + AdWertGier)/2;  */

	//此地NICK与ROLL反一下，与PCB对应
	S0_6050.AngRoll=	(((AdWert_AngRoll>>r_move))-AngX_mid-corr_6050.AngRoll); //下为减
	S0_6050.AngNick= -(((AdWert_AngNick>>r_move))-AngY_mid-corr_6050.AngNick);	//左为减
	//S0_6050.AngHoch=((AdWert_AngHoch>>r_move))-AngZ_mid;
	S0_6050.AccRoll=((AdWertRoll>>6)-AccX_mid-corr_6050.AccRoll); //下为减						
	S0_6050.AccNick=((AdWertNick>>6)-AccY_mid-corr_6050.AccNick);	 //左为减
	S0_6050.AccGier=-((AdWertGier>>6)-AccZ_mid-corr_6050.AccGier); //左旋为减
	}
	else
	{ 

	 AngX_mid=((AdWertAngRoll>>r_move));
	 AngY_mid=((AdWertAngNick>>r_move));
	 //AngZ_mid=((AdWertAngHoch>>r_move));
	 AccX_mid=  (AdWertRoll>>6);
     AccY_mid=  (AdWertNick>>6);
     AccZ_mid=  (AdWertGier>>6);

	 	S0_6050.AngNick=	AngX_mid; 
		S0_6050.AngRoll= AngY_mid;	
		//S0_6050.AngHoch=AngZ_mid;
		S0_6050.AccNick= AccX_mid; 						
		S0_6050.AccRoll= AccY_mid;	 
		S0_6050.AccGier= AccZ_mid; 
	}

	
	
//#define	test_ppm_ad											     
#ifdef 	test_ppm_ad	

	//UART1_Put_Chars("Temperature:");
	//UART1_Put_int((((((int16_t)buffer[6]) <<8) | buffer[7]))>>6);

	/*UART1_Put_Chars("AccRoll:");
	UART1_Put_int(S0_6050.AccRoll);
	UART1_Put_Chars("AccNick:");
	UART1_Put_int(S0_6050.AccNick);
	
	//UART1_Put_Chars("AngHoch:");
	//UART1_Put_int(S0_6050.AngHoch); 
	UART1_Put_Chars("AngRoll:");
	UART1_Put_int(S0_6050.AngRoll);	
	UART1_Put_Chars("AngN:");
	UART1_Put_int(S0_6050.AngNick);	
	UART1_Put_int(S0_6050.AccNick);	 */
	 UART1_Put_Chars("AccGier:");	  
	UART1_Put_int(S0_6050.AccGier);

	USART_newline;
#endif	

	 //ZaehlMessungen++;
	}
}



void S_6050_ready(void)//mpu6050启动时校正
{
	 	  
	 u8  S_6050_same_data=0;
	 //缓存E2ROM数据
	 s16 AngX_midz,AngY_midz;//,AngZ_midz;//角度中立值
	 s16 AccX_midz,AccY_midz,AccZ_midz;//角速度中立值
	 u8 test_num=0;//测试次数，大于100则存当前中立值

	 e2rom_mpu6050_read();

	 AngX_midz=AngX_mid;
	 AngY_midz=AngY_mid;
	 //AngZ_midz=AngZ_mid;//角度中立值
	 AccX_midz=AccX_mid;
	 AccY_midz=AccY_mid;
	 AccZ_midz=AccZ_mid;//角速度中立值//
	 

		  MPU6050_initialize();   //MPU6050需二次初始化。否则数据不稳定
		  UART1_Put_Chars("S_6050_ready!");


 			while(S_6050_same_data<5)	 
		   { 
			 

		     
			 if((abs(AngX_mid-AngX_midz)>1)|(abs(AngY_mid-AngY_midz)>1))   //中立值于E2ROM比较，小于1即用E2ROM值
		   	 { 															   //否则测试次数大于100则存当前读出值
			  	//S_6050_same_data=0;

				if(test_num<10)test_num++;
				else
				{
				  S_6050_same_data=10;
				
					  { //如果发现中立点变化比较大,则开灯10s，其间如如按键按下，则重存当前值，否则以旧数据为准
				 		u16 c=0;

				  		UART1_Put_Chars("MPU6050 DATA err than max!");
				  		UART1_Put_Chars("if you want change data,press key!");
				  		LED4_Turn;  LED3_Turn; LED2_Turn; 	  
				  
				  			while(c<10)	 //亮灯10s
				  			{
				  				c++;delay_100ms(1);
								UART1_Put_Chars("key:");
								UART1_Put_int(key);
								if(key)  {LED1_OFF;	 c=301;  }

								if(c%5==0){LED4_Turn;  LED3_Turn; LED2_Turn;} 	

							//	BB(1);	 //BB报警
				  			} /**/
				  		
						LED4_OFF;	 LED3_OFF;LED2_OFF;
				  		
				  
				  		if(key)
				  		{
				  			key=0;
				  			UART1_Put_Chars("e2rom write MPU6050_Mid data!");
				  			e2rom_data_write();  //存当前读出值 
				  			delay_100ms(10);
							
							UART1_Put_Chars_noend("AngX_mid use:");
							UART1_Put_int(AngX_mid);
							UART1_Put_Chars_noend("AngY_mid use:");
							UART1_Put_int(AngY_mid);				  
				  		}
				  		else 	
						{
						
						AngX_mid=AngX_midz;
						AngY_mid=AngY_midz;
						AccX_mid=AccX_midz;
						AccY_mid=AccY_midz;
						AccZ_mid=AccZ_midz;
						mpu6050_star=1;

						UART1_Put_Chars("MPU6050 use old data!");
						UART1_Put_Chars_noend("AngX_mid use:");
						UART1_Put_int(AngX_midz);
						UART1_Put_int(AngX_mid);
						UART1_Put_Chars_noend("AngY_mid use:");
						UART1_Put_int(AngY_midz);
						UART1_Put_int(AngY_mid);
						UART1_Put_Chars_noend("AccX_mid use:");
						UART1_Put_int(AccX_midz);
						UART1_Put_int(AccX_mid);
						UART1_Put_Chars_noend("AccY_mid use:");
						UART1_Put_int(AccY_midz);
						UART1_Put_int(AccY_mid);
						UART1_Put_Chars_noend("AccZ_mid use:");
						UART1_Put_int(AccZ_midz);
						UART1_Put_int(AccZ_mid);

						}
				 	  }	 /**/
				  

				}
			 }
			 else  
			 {
			 	S_6050_same_data+=1;
				
				if(S_6050_same_data==5)
				{
				UART1_Put_Chars("MPU6050 DATA ok!");	
				UART1_Put_Chars("MPU6050 use old data!");

				UART1_Put_Chars_noend("AngX_mid use:");
				UART1_Put_int(AngX_midz);
				UART1_Put_Chars_noend("AngY_mid use:");
				UART1_Put_int(AngY_midz);
				}
			 }

				if(S_6050_same_data<5)
				{
			 	UART1_Put_Chars_noend("same_data:");
				UART1_Put_int(S_6050_same_data);
				UART1_Put_Chars_noend("AngX_midZ:");
				UART1_Put_int(AngX_midz);
			 	UART1_Put_Chars_noend("AngX_mid:");
				UART1_Put_int(AngX_mid);
				UART1_Put_Chars_noend("AngY_midZ:");
				UART1_Put_int(AngY_midz);
			 	UART1_Put_Chars_noend("AngY_mid:");
				UART1_Put_int(AngY_mid);
				USART_newline;
				}
				
				
				
				
				
					/* */

			 
			 

			  while(!(MPU6050_is_DRY()));	//等SENSOR有新数据 			  
			  MPU6050_getMotion();

			   delay_ms(1);  
			   LED1_Turn;
		   }

		   LED1_OFF;
		   delay_100ms(10);
		   mpu6050_star=1;

		   memset(&corr_6050,0,sizeof(corr_6050));//清0
}

void S_6050_set(void)//mpu6050启动时强制校正
{
	 	  
	 u8  S_6050_same_data=0;
	 //缓存E2ROM数据
	 s16 AngX_midz,AngY_midz;//,AngZ_midz;//角度中立值
	 s16 AccX_midz,AccY_midz,AccZ_midz;//角速度中立值
	 u8 test_num=0;//测试次数，大于100则存当前中立值

	 e2rom_mpu6050_read();

	 AngX_midz=AngX_mid;
	 AngY_midz=AngY_mid;
	 //AngZ_midz=AngZ_mid;//角度中立值
	 AccX_midz=AccX_mid;
	 AccY_midz=AccY_mid;
	 AccZ_midz=AccZ_mid;//角速度中立值//
	 

		  MPU6050_initialize();   //MPU6050需二次初始化。否则数据不稳定
		  UART1_Put_Chars("S_6050_ready!Star set s_6050_data!");


 			while(S_6050_same_data<5)	 
		   { 
			 

		     
			 if((abs(AngX_mid-AngX_midz)>1)|(abs(AngY_mid-AngY_midz)>1))   //中立值于E2ROM比较，小于1即用E2ROM值
		   	 { 															   //否则测试次数大于100则存当前读出值
			  	//S_6050_same_data=0;

				if(test_num<10)test_num++;
				else
				{
				  S_6050_same_data=10;
				
					  { //如果发现中立点变化比较大,则重存当前值，否则以旧数据为准

				  		UART1_Put_Chars("MPU6050 DATA err than max!");
				  		UART1_Put_Chars("e2rom write MPU6050_Mid data!");
				  		e2rom_data_write();  //存当前读出值 

						UART1_Put_Chars_noend("AngX_mid use:");
						UART1_Put_int(AngX_mid);
						UART1_Put_Chars_noend("AngY_mid use:");
						UART1_Put_int(AngY_mid);
				  		
				 	  }	 /**/
				  

				}
			 }
			 else  
			 {
			 	S_6050_same_data+=1;
				if(S_6050_same_data==5)
				{												 		
						AngX_mid=AngX_midz;
						AngY_mid=AngY_midz;
						AccX_mid=AccX_midz;
						AccY_mid=AccY_midz;
						AccZ_mid=AccZ_midz;
						mpu6050_star=1;

				UART1_Put_Chars("MPU6050 DATA ok!");	
				UART1_Put_Chars("MPU6050 use old data!");
				UART1_Put_Chars_noend("AngX_mid use:");
				UART1_Put_int(AngX_midz);
				UART1_Put_Chars_noend("AngY_mid use:");
				UART1_Put_int(AngY_midz);
				}
			 }



			 	if(S_6050_same_data<5)
				{
			 	UART1_Put_Chars_noend("same_data:");
				UART1_Put_int(S_6050_same_data);
				UART1_Put_Chars_noend("AngX_midZ:");
				UART1_Put_int(AngX_midz);
			 	UART1_Put_Chars_noend("AngX_mid:");
				UART1_Put_int(AngX_mid);
				UART1_Put_Chars_noend("AngY_midZ:");
				UART1_Put_int(AngY_midz);
			 	UART1_Put_Chars_noend("AngY_mid:");
				UART1_Put_int(AngY_mid);
				USART_newline;
				}
				
					/* */

			 
			 

			  while(!(MPU6050_is_DRY()));	//等SENSOR有新数据 			  
			  MPU6050_getMotion();

			   delay_ms(1);  
			   LED1_Turn;
		   }

		   LED1_OFF;
		   delay_100ms(10);
		   mpu6050_star=1;
}
//------------------End of File----------------------------
