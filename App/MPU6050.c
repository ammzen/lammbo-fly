/* MPU6050.c file */
/********************LAMBO Fly soft********************


star:
13-01-07    edit by edmondchao

��ע��   MPU6050����γ�ʼ�����������ݲ��ȶ�
*******************************************************************************/

#include "main.h"


uint8_t buffer[14];

volatile signed short int  AdWertNick = 0, AdWertRoll = 0, AdWertGier = 0;//?�������Ǵ���������ֵ
volatile signed short int  AdWertAngRoll = 0,AdWertAngNick = 0,AdWertAngHoch = 0;///?�ӽǶȴ���������ֵ
//volatile int NeutralAccX=0, NeutralAccY=0,NeutralAccZ = 0;//?���ݼ���ֵ //����ֵ

//int16_t Gx_offset=0,Gy_offset=0,Gz_offset=0;
//float Acc1G_Values;

u8 MPU6050_en=0;   //MPU6050_en=1;	 //1MS�¼�����
int16_t sensor_6050in_old[9];



struct mpu6050_info S0_6050;//mpu6050��ԭʼ���ֵ��
struct mpu6050_info corr_6050;//mpu6050�ľ���ֵ�� correct


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setClockSource(uint8_t source)
*��������:	    ����  MPU6050 ��ʱ��Դ
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

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setFullScaleAccelRange(uint8_t range)
*��������:	    ����  MPU6050 ���ٶȼƵ��������
*******************************************************************************/
void MPU6050_setFullScaleAccelRange(uint8_t range) {
    IICwriteBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setSleepEnabled(uint8_t enabled)
*��������:	    ����  MPU6050 �Ƿ����˯��ģʽ
				enabled =1   ˯��
			    enabled =0   ����
*******************************************************************************/
void MPU6050_setSleepEnabled(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t MPU6050_getDeviceID(void)
*��������:	    ��ȡ  MPU6050 WHO_AM_I ��ʶ	 ������ 0x68
*******************************************************************************/
uint8_t MPU6050_getDeviceID(void) {

    IICreadBytes(devAddr, MPU6050_RA_WHO_AM_I, 1, buffer);
    return buffer[0];
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t MPU6050_testConnection(void)
*��������:	    ���MPU6050 �Ƿ��Ѿ�����
*******************************************************************************/
uint8_t MPU6050_testConnection(void) {
   if(MPU6050_getDeviceID() == 0x68)  //0b01101000;
   {

	//LED1_Turn;
   return 1;

   }
   	else return 0;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
*��������:	    ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
*******************************************************************************/
void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
*��������:	    ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
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

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_initialize(void)
*��������:	    ��ʼ�� 	MPU6050 �Խ������״̬��
*******************************************************************************/
void MPU6050_initialize(void) 
{
//	int16_t temp[6];
//	unsigned char i;

	MPU6050_intio_Init();


    MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO); //����ʱ��
    MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);//������������� +-2000��ÿ��
    MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_16);	//���ٶȶ�������� +-2G
    MPU6050_setSleepEnabled(0); //���빤��״̬
	MPU6050_setI2CMasterModeEnabled(0);	 //����MPU6050 ����AUXI2C
	MPU6050_setI2CBypassEnabled(1);	 //����������I2C��	MPU6050��AUXI2C	ֱͨ������������ֱ�ӷ���HMC5883L
	//����MPU6050 ���ж�ģʽ ���жϵ�ƽģʽ
	//����λ����0��INT���ŵ��߼���ƽΪ�ߵ�ƽ������λ����1��INT���ŵ��߼���ƽΪ�͵�ƽ��Ч��////(1)
	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_LEVEL_BIT, 1);
	//����λ����0��INT���ű�����Ϊ���졣����λ����1��INT��������Ϊ©����·��
	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_OPEN_BIT, 0);
	//����λ����0��INT���ŷ�����50uS�ĳ����塣����λ����1��INT���ű���ǰ��(1)�����ƽ��ֱ���ж������
	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_LATCH_INT_EN_BIT, 1);
	//����λ����0���ж�״̬λ�����㣬ֻ��ͨ���Ķ�INT_STATUS��ע��58������λ����1���κζ��ж�״̬λ�����������
	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_RD_CLEAR_BIT, 1);
	//������ת������ж�
    IICwriteBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_DATA_RDY_BIT, 1);

	//MPU6050_en=1;

}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char MPU6050_is_DRY(void)
*��������:	    ��� MPU6050���ж����ţ������Ƿ����ת��
���� 1  ת�����
0 ���ݼĴ�����û�и���
*******************************************************************************/
unsigned char MPU6050_is_DRY(void)
{
    if(GPIO_ReadInputDataBit(MPU6050_port,MPU6050_pin)==0)return 1;
	else return 0;
}

u8 MPU6050_read_ok=0; 
volatile signed short int  AdWert_Nick = 0, AdWert_Roll = 0, AdWert_Gier = 0;//?�������Ǵ���������ֵ
volatile signed short int  AdWert_AngRoll = 0,AdWert_AngNick = 0,AdWert_AngHoch = 0;///?�ӽǶȴ���������ֵ
s16 AngX_mid=0,AngY_mid=0,AngZ_mid= 0;//?���ݼ���ֵ //����ֵ
s16 AccX_mid=0,AccY_mid=0,AccZ_mid= 0;//?���ݼ���ֵ //����ֵ

u8 mpu6050_star=0;//=0: ��һ������
void MPU6050_getMotion(void)
{
   // LED2_Turn;
    if(MPU6050_is_DRY()) 
	{
	 
	  //LED4_Turn;

	 IICreadBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 14, buffer);

	AdWertAngNick=(((((int16_t)buffer[0]) << 8) | buffer[1])); //�Ƕ�
    AdWertAngRoll=(((((int16_t)buffer[2]) << 8) | buffer[3]));	 //�Ƕ�
    //AdWertAngHoch=(((((int16_t)buffer[4]) << 8) | buffer[5]));  //�Ƕ�
	//�����¶�ADC
    AdWertRoll=(((((int16_t)buffer[8]) << 8) | buffer[9]));		 //���ٶ�
    AdWertNick=(((((int16_t)buffer[10]) << 8) | buffer[11]));	 //���ٶ�
    AdWertGier=(((((int16_t)buffer[12]) << 8) | buffer[13])); 	 //���ٶ�

#define r_move  5
	#define acc_num 3
	if(mpu6050_star)
	{
	AdWert_AngRoll=  (AdWert_AngRoll*acc_num + AdWertAngRoll)/(acc_num+1);
    AdWert_AngNick=  (AdWert_AngNick*acc_num + AdWertAngNick)/(acc_num+1);
    //AdWert_AngHoch=  (AdWert_AccHoch*acc_num + AdWertAccHoch)/(acc_num+1);
	/*	//�����¶�ADC
    AdWert_Roll=  (AdWert_Roll*1 + AdWertRoll)/2;
    AdWert_Nick=  (AdWert_Nick*1 + AdWertNick)/2;
    AdWert_Gier=  (AdWert_Gier*1 + AdWertGier)/2;  */

	//�˵�NICK��ROLL��һ�£���PCB��Ӧ
	S0_6050.AngRoll=	(((AdWert_AngRoll>>r_move))-AngX_mid-corr_6050.AngRoll); //��Ϊ��
	S0_6050.AngNick= -(((AdWert_AngNick>>r_move))-AngY_mid-corr_6050.AngNick);	//��Ϊ��
	//S0_6050.AngHoch=((AdWert_AngHoch>>r_move))-AngZ_mid;
	S0_6050.AccRoll=((AdWertRoll>>6)-AccX_mid-corr_6050.AccRoll); //��Ϊ��						
	S0_6050.AccNick=((AdWertNick>>6)-AccY_mid-corr_6050.AccNick);	 //��Ϊ��
	S0_6050.AccGier=-((AdWertGier>>6)-AccZ_mid-corr_6050.AccGier); //����Ϊ��
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



void S_6050_ready(void)//mpu6050����ʱУ��
{
	 	  
	 u8  S_6050_same_data=0;
	 //����E2ROM����
	 s16 AngX_midz,AngY_midz;//,AngZ_midz;//�Ƕ�����ֵ
	 s16 AccX_midz,AccY_midz,AccZ_midz;//���ٶ�����ֵ
	 u8 test_num=0;//���Դ���������100��浱ǰ����ֵ

	 e2rom_mpu6050_read();

	 AngX_midz=AngX_mid;
	 AngY_midz=AngY_mid;
	 //AngZ_midz=AngZ_mid;//�Ƕ�����ֵ
	 AccX_midz=AccX_mid;
	 AccY_midz=AccY_mid;
	 AccZ_midz=AccZ_mid;//���ٶ�����ֵ//
	 

		  MPU6050_initialize();   //MPU6050����γ�ʼ�����������ݲ��ȶ�
		  UART1_Put_Chars("S_6050_ready!");


 			while(S_6050_same_data<5)	 
		   { 
			 

		     
			 if((abs(AngX_mid-AngX_midz)>1)|(abs(AngY_mid-AngY_midz)>1))   //����ֵ��E2ROM�Ƚϣ�С��1����E2ROMֵ
		   	 { 															   //������Դ�������100��浱ǰ����ֵ
			  	//S_6050_same_data=0;

				if(test_num<10)test_num++;
				else
				{
				  S_6050_same_data=10;
				
					  { //�������������仯�Ƚϴ�,�򿪵�10s��������簴�����£����ش浱ǰֵ�������Ծ�����Ϊ׼
				 		u16 c=0;

				  		UART1_Put_Chars("MPU6050 DATA err than max!");
				  		UART1_Put_Chars("if you want change data,press key!");
				  		LED4_Turn;  LED3_Turn; LED2_Turn; 	  
				  
				  			while(c<10)	 //����10s
				  			{
				  				c++;delay_100ms(1);
								UART1_Put_Chars("key:");
								UART1_Put_int(key);
								if(key)  {LED1_OFF;	 c=301;  }

								if(c%5==0){LED4_Turn;  LED3_Turn; LED2_Turn;} 	

							//	BB(1);	 //BB����
				  			} /**/
				  		
						LED4_OFF;	 LED3_OFF;LED2_OFF;
				  		
				  
				  		if(key)
				  		{
				  			key=0;
				  			UART1_Put_Chars("e2rom write MPU6050_Mid data!");
				  			e2rom_data_write();  //�浱ǰ����ֵ 
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

			 
			 

			  while(!(MPU6050_is_DRY()));	//��SENSOR�������� 			  
			  MPU6050_getMotion();

			   delay_ms(1);  
			   LED1_Turn;
		   }

		   LED1_OFF;
		   delay_100ms(10);
		   mpu6050_star=1;

		   memset(&corr_6050,0,sizeof(corr_6050));//��0
}

void S_6050_set(void)//mpu6050����ʱǿ��У��
{
	 	  
	 u8  S_6050_same_data=0;
	 //����E2ROM����
	 s16 AngX_midz,AngY_midz;//,AngZ_midz;//�Ƕ�����ֵ
	 s16 AccX_midz,AccY_midz,AccZ_midz;//���ٶ�����ֵ
	 u8 test_num=0;//���Դ���������100��浱ǰ����ֵ

	 e2rom_mpu6050_read();

	 AngX_midz=AngX_mid;
	 AngY_midz=AngY_mid;
	 //AngZ_midz=AngZ_mid;//�Ƕ�����ֵ
	 AccX_midz=AccX_mid;
	 AccY_midz=AccY_mid;
	 AccZ_midz=AccZ_mid;//���ٶ�����ֵ//
	 

		  MPU6050_initialize();   //MPU6050����γ�ʼ�����������ݲ��ȶ�
		  UART1_Put_Chars("S_6050_ready!Star set s_6050_data!");


 			while(S_6050_same_data<5)	 
		   { 
			 

		     
			 if((abs(AngX_mid-AngX_midz)>1)|(abs(AngY_mid-AngY_midz)>1))   //����ֵ��E2ROM�Ƚϣ�С��1����E2ROMֵ
		   	 { 															   //������Դ�������100��浱ǰ����ֵ
			  	//S_6050_same_data=0;

				if(test_num<10)test_num++;
				else
				{
				  S_6050_same_data=10;
				
					  { //�������������仯�Ƚϴ�,���ش浱ǰֵ�������Ծ�����Ϊ׼

				  		UART1_Put_Chars("MPU6050 DATA err than max!");
				  		UART1_Put_Chars("e2rom write MPU6050_Mid data!");
				  		e2rom_data_write();  //�浱ǰ����ֵ 

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

			 
			 

			  while(!(MPU6050_is_DRY()));	//��SENSOR�������� 			  
			  MPU6050_getMotion();

			   delay_ms(1);  
			   LED1_Turn;
		   }

		   LED1_OFF;
		   delay_100ms(10);
		   mpu6050_star=1;
}
//------------------End of File----------------------------
