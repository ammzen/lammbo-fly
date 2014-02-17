/* HMC5883L.c file
编写者：lisn3188
网址：www.chiplab7.com
作者E-mail：lisn3188@163.com
编译环境：MDK-Lite  Version: 4.23
初版时间: 2012-04-25
测试： 本程序已在第七实验室的mini IMU上完成测试
功能：
提供HMC5883L 初始化 读取磁力计当前ADC转换结果
------------------------------------
 */

#include "main.h"



float x_max,y_max,z_max;
float HMC5883_lastx,HMC5883_lasty,HMC5883_lastz;
volatile signed short int x=0,y=0,z=0;

s16  HMC5883_maxx=0,HMC5883_maxy=0,HMC5883_maxz=0,
		 HMC5883_minx=0,HMC5883_miny=0,HMC5883_minz=0;
unsigned char HMC5883_Run=0; //初始化完成标志

u8 HMC5883_newdata=0;//新数据OK =1；
u8 HMC5883_new_datain=0; //1:有新遥控数据进来HMC5883

u16 HMC5883_data=0;

u8 HMC5883_check_num=0;	//HMC5883每50MS检查一次，放在TIM4
u8 HMC5883_err_mark=0;	//HMC5883错误标志

u8 vbuff[6];

double    y_scale=1;



																		  
//计算最大最小值
void HMC58X3_getmaxmin(void)
{

   if(x>HMC5883_maxx) {HMC5883_maxx=x;}//	e2rom_ask=1;}
   if(x<HMC5883_minx) {HMC5883_minx=x;}//	e2rom_ask=1;}

   if(y>HMC5883_maxy) {HMC5883_maxy=y;}//	e2rom_ask=1;}
   if(y<HMC5883_miny) {HMC5883_miny=y;}//	e2rom_ask=1;}

   if(z>HMC5883_maxz) {HMC5883_maxz=z;}//	e2rom_ask=1;}
   if(z<HMC5883_minz) {HMC5883_minz=z;}//	e2rom_ask=1;}
   

   #define Print_hmc5883_ad
   #ifdef Print_hmc5883_ad
   

   UART1_Put_Chars("max X:");
   UART1_Put_int(HMC5883_maxx); // << 8) | vbuff[1]);  	USART_newline;
   UART1_Put_int(HMC5883_minx); 

   UART1_Put_Chars("max Y:");
   UART1_Put_int(HMC5883_maxy); // << 8) | vbuff[5]);  	USART_newline;
   UART1_Put_int(HMC5883_miny);  

   UART1_Put_Chars("max Z:");
   UART1_Put_int(HMC5883_maxz); // << 8) | vbuff[3]);  	USART_newline;
   UART1_Put_int(HMC5883_minz);  

   y_scale=((double)HMC5883_maxx-(double)HMC5883_minx)/((double)HMC5883_maxy-(double)HMC5883_miny);//Y轴比率

   UART1_Put_Chars("y_scale:");
   UART1_Put_int(y_scale*10000); 

   USART_newline;
  #endif
} 

void HMC5883_setwork(void)//重启工作状态
{
   //UART1_Put_Chars("Rest 5883;");
   HMC58X3_setMode(2); 
   HMC58X3_setMode(0); 
}

//在读函数中不停将HMC5883_check_num清0；
//此函数每200MS调用一次，来检杳HMC5883_check_num,大于20（20*200=4S）无数据则重启HMC5883  
void  HMC5883_check_work(void)
{
	if(HMC5883_open)  //200MS检查一次
	 {		 	
		if(HMC5883_check_num<(40-1))HMC5883_check_num++; //20*2.5=50MS()
	 	else 
	 	{
		    HMC5883_check_num=0;HMC5883_setwork();
			HMC5883_err_mark=1;
	 	}	 
	 }
}




void HMC5883_Circling(void)
{
     UART1_Put_Chars("HMC5883_Circling!/");
	 
	 //HMC5883_setwork();	 

	 while(key==0)
	 {
		if((HMC5883_newdata==1))	//等SENSOR有新数据
        {
			LED1_Turn;	

		  HMC5883_newdata=0; 

		  HMC5883_check_num=0;

   		 	IICreadBytes(HMC58X3_ADDR,HMC58X3_R_XM,6,vbuff);
   
   		 	x = ((int16_t)vbuff[0] << 8) | vbuff[1];
   		 	y = ((int16_t)vbuff[4] << 8) | vbuff[5];
   		 	z = ((int16_t)vbuff[2] << 8) | vbuff[3];

		  HMC58X3_getmaxmin();	 //计算最大最小值
		  //UART1_Put_int(key);
		}
		if(HMC5883_err_mark)
		{
		   HMC5883_err_mark=0;
		   HMC5883_setwork();		
		}
		 
	 }

}
double angle_tem;				
void HMC58X3_getRaw(void) 
{
   double angle;
  

  if((HMC5883_newdata==1))	//等SENSOR有新数据
  {	 // UART1_Put_Chars("test:");
   HMC5883_newdata=0;	  
   //LED1_Turn;
   
   HMC5883_check_num=0; 

   IICreadBytes(HMC58X3_ADDR,HMC58X3_R_XM,6,vbuff);
   
   x = ((int16_t)vbuff[0] << 8) | vbuff[1];
   y = ((int16_t)vbuff[4] << 8) | vbuff[5];
   z = ((int16_t)vbuff[2] << 8) | vbuff[3];

   	
   //HMC58X3_getmaxmin();	 //计算最大最小值
    x=(float)(x-((HMC5883_maxx+HMC5883_minx)/2));
    y=(float)(y-((HMC5883_maxy+HMC5883_miny)/2));
    z=(float)(z-((HMC5883_maxz+HMC5883_minz)/2));

	x= x;
	//y= 0.9681*y;
	y= y_scale*y;
	//y= 0.80*y;

	/*{float Hx, Hy, Hz, Pitch, Roll;
	Hx=x;
	Hy=y;
	Hz=z;
	Roll=S0_6050.AngRoll;
	Pitch=S0_6050.AngNick;
	 Hx = Hx * cos(Pitch) + Hy * sin(Pitch) * sin(Roll) - Hz * sin(Pitch) * cos(Roll);        //修正Hx
    Hy = Hy * cos(Roll) + Hz * sin(Roll);                                //修正Hy

	angle= atan2((double)Hy,(double)Hx) * (180 / 3.14159265) + 180;
	//angle= atan2((double)y,(double)x) * (180 / 3.14159265) + 180;// ;//

	UART1_Put_Chars("Angle:");
   	UART1_Put_int(angle);  
	} */

	angle= atan2((double)y,(double)x) * (180 / 3.14159265) + 180;

   if(HMC5883_Run)
   {
	 double angle_temA=angle_tem; 

	#define acchmc_num 2 
	
	if((abs(angle_temA-angle))>180)
	{
	    if((angle<180))  angle_temA=angle_temA-360; 
		else   angle_temA=angle_temA+360; 	
	}
	angle_tem=(angle_temA*acchmc_num+angle)/(acchmc_num+1);
	angle_tem=angle_360_math(angle_tem);
   }
   else
   {
   	 angle_tem= angle;
   }

   if(angle_tem<0) angle_tem=angle_tem+360;
   if(angle_tem>360) angle_tem=angle_tem-360;
   
	HMC5883_data=angle_tem;
    HMC5883_new_datain=1;
   
  #define debug_hmc5883_ad
  #ifdef debug_hmc5883_ad

  UART1_Put_Chars("Angle:");
   	UART1_Put_int(HMC5883_data); 
	UART1_Put_Chars("AccGier:");	  
	UART1_Put_int(S0_6050.AccGier);	
 /* 
   Lcd_WriteString(0,0,osd,Red,"Angle:");
   show_data_process(0,1,osd,Red,HMC5883_data);
   UART1_Put_Chars("X:");
   UART1_Put_int(x);

   UART1_Put_Chars("Y:");
   UART1_Put_int(y); 

   UART1_Put_Chars("Z:");
   UART1_Put_int(z); 
					 */
   USART_newline;  
  	#endif
   /**/

  }
  /*  */
}

void HMC5883_ready(void)	   //启动时校正
{
	 u16 HMC5883_data_old=0;
	 u8  HMC5883_same_data=0;		  
	 UART1_Put_Chars("HMC5883__ready!");
	 
		   while(HMC5883_same_data<6)
		   {  
		     if((HMC5883_data_old!=HMC5883_data)) 
		   	 { 
			  HMC5883_same_data=0;
			  HMC5883_data_old=HMC5883_data; 			  
			 }
			 else  HMC5883_same_data+=1;

			  while(HMC5883_newdata==0);	//等SENSOR有新数据
			  if((HMC5883_newdata==1)&&HMC5883_open)	 //HMC5883 MPU6050在同一时间取值
			  {	 
			  	HMC58X3_getRaw();				
			  }

			 UART1_Put_Chars_noend("HMC5883_same_data:");
			 UART1_Put_int(HMC5883_same_data); 
			 UART1_Put_int(HMC5883_data); 


		    delay_ms(3);  	 
			LED1_Turn;
		   }

		   LED1_OFF;
		   delay_100ms(10);
		   UART1_Put_Chars("HMC5883 ready  ok!");
}													

/**************************实现函数********************************************
*函数原型:	   void HMC58X3_writeReg(unsigned char reg, unsigned char val)
*功　　能:	   写HMC5883L的寄存器
输入参数：    reg  寄存器地址
			  val   要写入的值	
输出参数：  无
*******************************************************************************/
void HMC58X3_writeReg(unsigned char reg, unsigned char val) {
  IICwriteByte(HMC58X3_ADDR,reg,val);
}																		

/**************************实现函数********************************************
*函数原型:	  void HMC58X3_mgetValues(float *arry)
*功　　能:	   读取 校正后的 磁力计ADC值
输入参数：    输出数组指针	
输出参数：  无
******************************************************************************
void HMC58X3_mgetValues(float *arry) 
{
//  s16 xr,yr,zr;
 // HMC58X3_getRaw(&xr, &yr, &zr);
  arry[0]= HMC5883_lastx=(float)(xr-((HMC5883_maxx+HMC5883_minx)/2));
  arry[1]= HMC5883_lasty=(float)(yr-((HMC5883_maxy+HMC5883_miny)/2));
  arry[2]= HMC5883_lastz=(float)(zr-((HMC5883_maxz+HMC5883_minz)/2));
}	*/
																		   
/**************************实现函数********************************************
*函数原型:	  void HMC58X3_setGain(unsigned char gain)
*功　　能:	   设置 5883L的增益
输入参数：     目标增益 0-7
输出参数：  无
*******************************************************************************/
void HMC58X3_setGain(unsigned char gain) { 
  // 0-7, 1 default
  if (gain > 7) return;
  HMC58X3_writeReg(HMC58X3_R_CONFB, gain << 5);
}																			 

/**************************实现函数********************************************
*函数原型:	  void HMC58X3_setMode(unsigned char mode)
*功　　能:	   设置 5883L的工作模式
输入参数：     模式
输出参数：  无
*******************************************************************************/
void HMC58X3_setMode(unsigned char mode) {
  if (mode > 2) {
    return;
  }
  HMC58X3_writeReg(HMC58X3_R_MODE, mode);
  delay_us(100);
}																		 

/**************************实现函数********************************************
*函数原型:	  void HMC58X3_init(u8 setmode)
*功　　能:	   设置 5883L的工作模式
输入参数：     模式
输出参数：  无
*******************************************************************************/
/**************************实现函数********************************************
*函数原型:	  void HMC58X3_init(u8 setmode)
*功　　能:	   设置 5883L的工作模式
输入参数：     模式
输出参数：  无
*******************************************************************************/
void HMC58X3_init(u8 setmode) 
{
  // note that we don't initialize Wire here. 
  delay_us(5); // you need to wait at least 5ms after power on to initialize
  if (setmode) 
  {
    HMC58X3_setMode(0);
  }
  /*x_scale=1.0; // get actual values
  y_scale=1.0;
  z_scale=1.0; */

  HMC58X3_writeReg(HMC58X3_R_CONFA, 0x70); // 8 samples averaged, 15Hz frequency, no artificial bias. 11 100 00
  HMC58X3_writeReg(HMC58X3_R_CONFB, 0xA0);//增益5	101 00000
  HMC58X3_writeReg(HMC58X3_R_MODE, 0x00); //工作状态 连续

}
																	  
/*
   校准其中有几个弱点。
    1。使用一读错的增益。
    2。而不是使用的最大平均最大正火时轴到另一个。
    3。不使用负偏差。 （测量可能的改进）。
    Calibrate which has a few weaknesses.
    1. Uses wrong gain for first reading.
    2. Uses max instead of max of average when normalizing the axis to one another.
    3. Doesn't use neg bias. (possible improvement in measurement).
*/
void HMC58X3_calibrate(unsigned char gain) {
  s16 i;
  
  HMC58X3_writeReg(HMC58X3_R_CONFA, 0x010 + HMC_POS_BIAS); // Reg A 15Hz + X、Y、Z 轴正偏压配置
  HMC58X3_setGain(gain);
  HMC58X3_setMode(0);

  
  for (i=0; i<5; i++) 
  { 
	while(HMC5883_newdata==0);	//等SENSOR有新数据
    HMC58X3_getRaw();
  }

  HMC58X3_writeReg(HMC58X3_R_CONFA, 0x010 + HMC_NEG_BIAS); // Reg A 15Hz + X、Y、Z 轴正偏压配置
  HMC58X3_setGain(gain);
  HMC58X3_setMode(0);

  for (i=0; i<5; i++) 
  { 
	while(HMC5883_newdata==0);	//等SENSOR有新数据

    HMC58X3_getRaw();
    
  }	 
   
   HMC58X3_writeReg(HMC58X3_R_CONFA, 0x010); // set RegA/DOR back to default //15Hz	 /**/  

  
}   // calibrate().

/**************************实现函数********************************************
*函数原型:	  void HMC58X3_setDOR(unsigned char DOR)
*功　　能:	   设置 5883L的 数据输出速率
输入参数：     速率值
0 -> 0.75Hz  |   1 -> 1.5Hz
2 -> 3Hz     |   3 -> 7.5Hz
4 -> 15Hz    |   5 -> 30Hz
6 -> 75Hz  
输出参数：  无
*******************************************************************************/
void HMC58X3_setDOR(unsigned char DOR) {
  if (DOR>6) return;
  HMC58X3_writeReg(HMC58X3_R_CONFA,DOR<<2);
}																		  

/**************************实现函数********************************************
*函数原型:	  void HMC58X3_getID(char id[3])
*功　　能:	   读取芯片的ID
输入参数：     	ID存放的数组
输出参数：  无
*******************************************************************************/
void HMC58X3_getID(void) 
{	 
   UART1_Put_Chars("AdWertAccRoll:");

      UART1_Put_int(I2C_ReadOneByte(HMC58X3_ADDR,HMC58X3_R_IDA));  	USART_newline;
      UART1_Put_int(I2C_ReadOneByte(HMC58X3_ADDR,HMC58X3_R_IDB));	USART_newline;
      UART1_Put_int(I2C_ReadOneByte(HMC58X3_ADDR,HMC58X3_R_IDC));  USART_newline;

	 
	
	USART_newline;
}   // getID().
void HMC58X3_io_init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/**Key-GPIO***** 	*/

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	                              
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  	
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);	  
	
	/**Key-EXIT***** */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);		
	/**Key-NVIC******/

	/* Configure the Priority Group to 2 bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
   
   NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQChannel;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

}																				 
/**************************实现函数********************************************
*函数原型:	  void HMC5883L_SetUp(void)
*功　　能:	   初始化 HMC5883L 使之进入可用状态
输入参数：     	
输出参数：  无
*******************************************************************************/
void HMC5883L_SetUp(void)
{ 
  //u16 HMC5883_data_i=0;
  
   HMC58X3_io_init();
 
  HMC5883_Run=0;
  HMC58X3_init(0); // Don't set mode yet, we'll do that later on.
  // Calibrate HMC using self test, not recommended to change the gain after calibration.
  HMC58X3_calibrate(1); // Use gain 1=default, valid 0-7, 7 not recommended.
  // Single mode conversion was used in calibration, now set continuous mode
  HMC58X3_setMode(0);
  delay_us(100);
  HMC58X3_setDOR(6);  //75hz
  delay_us(50);
 // for(i=0;i<10;i++)HMC58X3_FIFO_init();
  HMC5883_Run=1;

  
  
  UART1_Put_Chars("ENd_HMC5883L_SetUp/");

  
}  
																		 

//------------------End of File----------------------------


