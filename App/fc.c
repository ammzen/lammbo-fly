

/********************LAMBO Fly soft********************


star:
13-01-07    edit by edmondchao	
*******************************************************************************/
#include "main.h"


#define  GAS_P_ovf  70	 //油门超过GAS_P_ovf后，GAS_P为1


volatile u16 Motor_front=moto_star,Motor_back=moto_star,Motor_left=moto_star,Motor_right=moto_star;//转速


struct RUN_info RUN;//RNN-参数。
struct Int_info Int;//主积分加补偿参数。ACC积分
//struct Int_info Int2;//主积分无补偿参数。
struct Int_info LInt;//长时间加补偿积分参数。//ACC积分后，再积分，=角度
//struct Int_info LInt2;//长时间无补偿积分参数。
struct Int_info angInt;//长时间角度积分参数。
struct Int_info Lcom;//长时间主积分补偿值。
//struct Int_info LInt_old;//长时间主积分上一次的值。		运动限制
struct Int_info err_numP;//计录积分超上限次数，大于2则进行中点重置
struct Int_info err_numN;//计录积分超下限次数，大于2则进行中点重置	

struct Int_info angInt_old;// 与angInt比较，在动态中，动作小于限定值时进行中点重置	

struct Int_info Gier_Int;//目的，非飞行状态下，进行Gier的校正	


struct Int_info PInt;//PPM积分参数。
struct Int_info CInt;//自稳定积分参数。
struct Int_info Cdif;//自稳定微分参数。
struct mpu6050_info C_6050;//自稳定时需要上一次mpu6050的输出值。

u8 Nick_mid=0,Roll_mid=0,Gier_mid=0;  //不启动时的PPM输入值(此点可能不是中立点127,因遥控器中点可能有微调输入)。

u8 PPM[10]={

 0,0,0,0,0,0,0,0,0,0,

};
struct PPM_info W_PPM;//WIFI_ppm参数。


u8 Main_ERR=0;	  //主错误标识	//1,RF故障 2,电量
u8 sys_star_ok=0;//1:系统启动完成

u16 Target_angle=0;//目标航向

float  GAS_P=1;  //油门PI倍率	 用于消除启飞时PI过高，趋地效应造成乱抖，
float  batt_P=1;  //电池PI倍率	 用于消除电机在电量高低时的不同响应度
float  bat_tem=0;  //电池滤波
u16  bat=0;	//电池
u16 bat_chk=0;//用于检查电池电压是否变化
u8  bat_gas=0;//用于低压保护  Gas-=bat_gas


u8 gas_delay=0; //丢信号时,油门缓慢下降延时。

struct PID_info PID;//PID参数。
struct PID_out PID_out;	 //PID输出到串口数据

//u8  moto_ON=1;	 //电机控制	1,hand-set上来就0
u8  hand_set_in=0;// 1,hand_set插入

struct dub_show_info dub_show;	 //DUG_SHOW的输出值
u8 dub_show_en=0;	 //允许DUG_SHOW的输出值	  100ms放在主计时里

u8 ms_2_en=0;	 //500HZ 信号  在主延时检测sys_tim_isr(void)中	//500hz 2ms
struct mpu6050_info S_6050;//mpu6050的输出值。

u8 wifi_RFok_num=0;//正确接收WIFI信号次数，	在主延时里，每200MS减1，为0时则失控
u8 wifi_ctrl=1;	 //是否WIFI控制	  //在接到上位机0x0a命令后，设定为WIFI控制
u8 RF_ctrl_num=0;//放在主延时里，作用为30MS运行一次PPM_MAIN_WORK
u8 PPM_open=1; //	1,打开


void dub_show_main(void)//发送上位机数据
{
	 /*	dub_show.id=0xfd;

	 	dub_show.AngNick=S_6050.AngNick;	//角度
		dub_show.AccNick=S_6050.AccNick;
		dub_show.IntNick=Int.Nick;

  		dub_show.AngRoll=S_6050.AngRoll;
  		dub_show.AccRoll=S_6050.AccRoll;	//角加速度
		dub_show.IntRoll=Int.Roll;
  		
  		dub_show.AccGier=S_6050.AccGier;
		dub_show.IntGier=Int.Gier;	



		dub_show.bat=bat_tem;	 

		//UART1_Put_Char(0xfd); //头
		UART1_Put_num(&dub_show.id1,sizeof(dub_show));	//从地址发送一批数据 */
}

void dub_show_fly(void)//发送上位机数据
{
	
	

	u8 check=0;//校验码:  ^所有数据 
	u8 lenth=0;//数据长度	

	uart_Send_byte(0xFD);  //头:		0xFD 0xDF
	uart_Send_byte(0xDF);  //

	check^=0xDF^0xFD;


		lenth=sizeof(dub_show);;//数据长度

		uart_Send_byte(0x0a); //返回原命令
		uart_Send_byte(lenth); //数据长度

		check^=0x0a^lenth;
	 	
		dub_show.bat=bat_tem;  
		dub_show.Height=0;
		
		
		//uart_Send_num(&dub_show,sizeof(dub_show));	//从地址发送一批数据	 


		//校验码
		{u8 a;	u8 b[sizeof(dub_show)];
		
			memcpy(&b[0],&dub_show,sizeof(dub_show));
			for(a=0;a<sizeof(dub_show);a++)
			{
				check^=b[a];
				uart_Send_byte(b[a]);

				//UART1_Put_int(b[a]);
			}
		/*UART1_Put_Chars("dub_show_fly:"); 
		UART1_Put_int(check);
		USART_newline;	//换行*/
		}
		uart_Send_byte(check);//
}

/******PPM*******/
void  PPM_Ave(void)//平均值
{
	u8 a=0;

	for(a=Chanl_star;a<(Chanl_star+Chanl_num);a++)
	{
	  	 #define  PPM_Ave_num  	2

		 PPM[a]=(PPM[a]*PPM_Ave_num+ppm_in[a])/(PPM_Ave_num+1);
	}
}

void RF_ctrl_mode_check(void)
{
	 			   if((PPM[Gas_num]>star_Gas)&&(RUN.status==200))	 //大于启动油门，且刚才是停机
				   {
				   	   RUN.status=1;  //启动				   
				   }
				   else if((PPM[Gas_num]>star_Gas)&&(RUN.status==201)) //大于启动油门，且刚才是飞行时丢失信号
				   {
				   	   RUN.status=1;  //启动				   
				   }
				   else if((PPM[Gas_num]>star_Gas)&&(RUN.status==0))	
				   {
				   	   Main_ERR=1;	 //主错误标识  1,RF故障
				   }
				   else if((PPM[Gas_num]<star_Gas))	
				   {
				   	   RUN.status=200;  //待机
					   if(Main_ERR==1)Main_ERR=0;	 //主错误标识 1,RF故障
				   }

				   //#define print_ppm
				   #ifdef  print_ppm
					UART1_Put_Chars("PPM:"); 
		
					/*UART1_Put_int(ppm_in[Roll_num]);
					UART1_Put_int(ppm_in[1]);
					UART1_Put_int(ppm_in[2]);
					UART1_Put_int(ppm_in[3]);
					UART1_Put_int(ppm_in[4]);
					UART1_Put_int(ppm_in[5]);
					UART1_Put_int(ppm_in[6]);
					UART1_Put_int(ppm_in[7]); */ 
					UART1_Put_int(RUN.status);
					UART1_Put_Chars_noend("Gas:");  //front
					UART1_Put_int(PPM[Gas_num]);
					UART1_Put_Chars_noend("Nick:");  //back
					UART1_Put_int(PPM[Nick_num]);
					UART1_Put_Chars_noend("Roll:");  //left
					UART1_Put_int(PPM[Roll_num]);
					UART1_Put_Chars_noend("Gier:");  //left
					UART1_Put_int(PPM[Gier_num]); 
					
					#endif	
}

u8  ppm_ok_old=200;//用于检查ppm_data_ok是否变化
void PPM_main_work(void)
{				
				//UART1_Put_Chars("PPM connect");
	  		   //UART1_Put_int(ppm_data_ok);
				//LED1_Turn; 
				if((ppm_data_ok>(ppm_ok_num-4)) )
				{
				   
				   //ppm_ok_old=ppm_data_ok;  //串行用
				   //ppm_data_ok--;		   //串行用

				   //LED1_Turn;  //
				   if(RUN.status==1) PPM_Ave();//平均值   				   
				   else  memcpy(&PPM[Chanl_star],&ppm_in[Chanl_star],Chanl_num);				   

				   RF_ctrl_mode_check();
				   			
				}
				else if(ppm_data_ok==0)	//出错 
				{
					if(RUN.status==1)  //如果当前是飞行状态时，则更改为丢失信号状态
					{
						RUN.status=201;	
						gas_delay=0; //丢信号时,油门缓慢下降延时。
					}
					else if(RUN.status!=201)RUN.status=0;  //否则更改为无信号状态

					Main_ERR=1;	 //主错误标识	//1,开机时油门未归0

					
					if(ppm_ok_old!=ppm_data_ok)
					{
						ppm_ok_old=ppm_data_ok;
						UART1_Put_Chars("PPM connect err!");
					}					 
				}

}

void WIFI_main_work(void)
{
	

	if(PPM_open==1) //	1,打开
	{
		PPM_open=0; //
		PPM_close();
	}
		
		if(wifi_RFok_num)
		{
			RF_ctrl_mode_check();
			LED2_Turn;
		}
		else
		{
					if(RUN.status==1)  //如果当前是飞行状态时，则更改为丢失信号状态
					{
						RUN.status=201;	
						gas_delay=0; //丢信号时,油门缓慢下降延时。
					}
					else if(RUN.status!=201)RUN.status=0;  //否则更改为无信号状态
					UART1_Put_Chars("wifi connect err!");

					Main_ERR=1;	 //主错误标识	//1,开机时油门未归0	 
					LED2_OFF;


				wifi_ctrl=0;	 //是否WIFI控制		
				//启动
				PPM_open=1; //
				PPM_Init();		
		}


}

#define scal_AngAcc   172//1000  //angle to IntAcc比率 NICK及Roll   11000/64=171.5
#define scal_360Acc   100   //地磁360 to IntAcc比率 Gier 	16000/180=88.8
/*
#define pGAS_P     	6  		//遥控油门倍数
#define pNick_P     5//10		//遥控仰俯倍数
#define pRoll_P     5//10		//遥控横滚倍数
#define pGier_P     3//5		//遥控自旋倍数

//PID
#define ACC_P     7//50//40 //50//35  //90

#define pNick_I    0.0001//0.002// 0.012		//遥控仰俯倍数 0.02	0.002
#define pRoll_I    0.0001//0.002// 0.012		//遥控横滚倍数
#define pGier_I    0.0005//0.01//0.02// 0.15		//遥控自旋倍数
*/
/*
#define pGAS_P     	4  		//遥控油门倍数
#define pNick_P     2//10		//遥控仰俯倍数
#define pRoll_P     2//10		//遥控横滚倍数
#define pGier_P     0.5//5		//遥控自旋倍数

//PID
#define ACC_P     7//50//40 //50//35  //90

#define pNick_I    0.0001//0.002// 0.012		//遥控仰俯倍数 0.02	0.002
#define pRoll_I    0.0001//0.002// 0.012		//遥控横滚倍数
#define pGier_I    0.0005//0.01//0.02// 0.15		//遥控自旋倍数	*/

#define center_P    0//.8//1//0.002// 0.012		//中心稳定P
#define center_I    0//.0005// 0.012//		//中心稳定I
#define center_D    0//0.01//0.02// 0.15	//中心稳定D


#define in_max 50
#define in_min 10
u8 max_Gier=0; //    最大控制信号输入量
u8 max_Nick=0; //    最大控制信号输入量
u8 max_Roll=0; //    最大控制信号输入量

//检测是否在允许范围	限幅
//#define check(speed)	{ if(speed>moto_max) speed=moto_max; if(speed<moto_mix)speed=moto_mix; }
s16  check(s16 speed)
{
  	if(speed>moto_max) speed=moto_max; 
	if(speed<moto_mix) speed=moto_mix;

	return speed;
}


//检测输入中点是否大于误差区间
s16  P_check(s16 PPM)
{
    #define PPM_star 	5

	if(abs(PPM)>PPM_star)//如果输入值大于PPM_star
	{
		if(PPM>0)  					return PPM-PPM_star;   //
		else   						return PPM+PPM_star; 	
	}
	else 	return 0;	
}   



void moto_math(void)	//电机主控
{
	 s16 M_f_tem=0,M_b_tem=0,M_l_tem=0,M_r_tem=0;

	 //LED4_Turn;
						 //电池处理
						bat_tem=bat;  //电池滤波
						bat_tem=(bat_tem*10+bat)/11;  //电池滤波
						//UART1_Put_Chars_noend("bat_tem:"); 
						//UART1_Put_int(bat_tem);
	 
	 //RUN.status=1;//用于测试
	 //UART1_Put_int(RUN.status);
	 if((RUN.status==200)|(RUN.status==0))//未启动或无信号
	 {
			
	 		
	 		M_f_tem=M_b_tem=M_l_tem=M_r_tem=0;

			memset(&Int.Nick,0,sizeof(Int));  //清主积分加补偿
			//memset(&Int2.Nick,0,sizeof(Int2));  //清主积分无补偿参数。



			//读PPM中点，积分，校正陀螺角度中点
			//memset(&PInt,0,sizeof(PInt));  //清积分

			//长期容和
			memset(&LInt,0,sizeof(LInt));  //清长时间加补偿积分参数。
			//memset(&LInt2,0,sizeof(LInt2));  //清长时间无补偿积分参数。
			memset(&Lcom,0,sizeof(Lcom));  //清积分

			memset(&angInt,0,sizeof(angInt));  //清积分

			//memset(&LInt_old,0,sizeof(LInt_old));  //清积分
			memset(&err_numP,0,sizeof(err_numP));  //清积分
			memset(&err_numN,0,sizeof(err_numN));  //清积分
			
			memset(&angInt_old,0,sizeof(angInt_old));  //清积分			

			//自动平衡
			memset(&CInt,0,sizeof(CInt));  //清积分
			memset(&Cdif,0,sizeof(Cdif));  //清积分


			//读PPM积分
			memset(&PInt,0,sizeof(PInt));  //清积分

			Target_angle=HMC5883_data;//目标航向


						//将信息发送到上位机	
					   	dub_show.AngNick=S_6050.AngNick*10;
						dub_show.AngRoll=S_6050.AngRoll*10;


			//不启动时的PPM输入值(此点可能不是中立点127,因遥控器中点可能有微调输入)。
			Nick_mid=PPM[Nick_num];
			Roll_mid=PPM[Roll_num];
			Gier_mid=PPM[Gier_num];   /**/

			//目的，非飞行状态下，进行Gier的校正 /
			if(Gier_Int.num<101)
			{
			Gier_Int.num++;

			Gier_Int.Gier+=S_6050.AccGier;

				if(Gier_Int.num==100)
				{
			  	Gier_Int.num=150;

				UART1_Put_Chars_noend("corr_6050.AccGier:");  //front
				UART1_Put_int(Gier_Int.Gier);  			

				corr_6050.AccGier= - Gier_Int.Gier/100;
				UART1_Put_int(corr_6050.AccGier*1000);

				Gier_Int.Gier=0;
				}
			 }

			goto moto_math_end;
	 }

	 
	 
	 if((RUN.status==1)|(RUN.status==201))//启动	| 飞行时丢失信号
	 {
			s16 gas_tem,Nick_tem,Roll_tem,Gier_tem;
			float PI_tem1,PI_tem2,PI_tem3;

			//memset(&Gier_Int,0,sizeof(Gier_Int));//	 //目的，非飞行状态下，进行Gier的校正

			//电池PI处理 + 低压保护
					   	batt_P=(((430 - bat_tem)/50)*0.2)+0.6;
						batt_P=1;  //电池PI倍率	//测试时使用
						/*UART1_Put_int(bat);
						UART1_Put_int(batt_P*100);
						USART_newline;*/

						#define  bat_gas_ovf  	370	 //电压超过bat_gas_ovf后，bat_gas为0
						#define  bat_gas_limit  350	 //电压超过bat_gas_ovf后，bat_gas为0
						//低压保护
						if((bat_tem<bat_gas_ovf)&&(bat_tem>bat_gas_limit))
						{
						   bat_gas=(bat_gas_ovf-bat_tem)*5;  						
						}
						else
						{
						   bat_gas=0;
						}

			//PPM
			if((PPM_open) |(wifi_ctrl==1))
			{ 		  
				 
			    if(RUN.status==1) //启动中
				{
					gas_tem	=PPM[Gas_num] -star_Gas;
					Nick_tem=P_check(PPM[Nick_num] - Nick_mid)+Nick_mid-127;//PPM[Nick_num]-128;//
					Roll_tem=P_check(PPM[Roll_num] - Roll_mid)+Roll_mid-127;//PPM[Roll_num]-128;//
					Gier_tem=P_check(PPM[Gier_num] - Gier_mid)+Gier_mid-127;//PPM[Gier_num]-128;//


					if(abs(Gier_tem)>in_max)max_Gier=1;
					else if(abs(Gier_tem)<in_min)max_Gier=0;

					if(abs(Nick_tem)>in_max)max_Nick=1;
					else if(abs(Nick_tem)<in_min)max_Nick=0;

					if(abs(Roll_tem)>in_max)max_Roll=1;
					else if(abs(Roll_tem)<in_min)max_Roll=0;

					if(max_Gier) 
					{
						S_6050.AccGier=0;	
						Int.Gier=0; 
						//UART1_Put_int(Gier_tem); 
					}


				}
				else  if((RUN.status==201))// 飞行时丢失信号
				{
					
					if(PPM[Gas_num]>star_Gas) 
					{
						 if(gas_delay<10)gas_delay++; //丢信号时,油门缓慢下降延时。
						 else 
						 {
						  gas_delay=0;
						  PPM[Gas_num]--;  //油门递减
						 } 						 
					}
					else 
					{
					   	PPM[Gas_num]=0;
						RUN.status=0;//当油门递减为0时,将状态更改回无信号
					}


					gas_tem	=PPM[Gas_num] -star_Gas;
					Nick_tem=0;
					Roll_tem=0;
					Gier_tem=0;
				}

				gas_tem-=bat_gas; 	
				
					//Nick_tem=0;//
					//Roll_tem=0;//
					//Gier_tem=0;//			

				//PPM混合
	 		M_f_tem=gas_tem*PID.pGAS_P - Nick_tem*PID.pNick_P - Gier_tem*PID.pGier_P; //前
	 		M_b_tem=gas_tem*PID.pGAS_P + Nick_tem*PID.pNick_P - Gier_tem*PID.pGier_P; //后

	 		M_l_tem=gas_tem*PID.pGAS_P + Roll_tem*PID.pRoll_P + Gier_tem*PID.pGier_P; //左
	 		M_r_tem=gas_tem*PID.pGAS_P - Roll_tem*PID.pRoll_P + Gier_tem*PID.pGier_P; //右		    
			
			 
				//油门PI倍率
				if(gas_tem<GAS_P_ovf) GAS_P=gas_tem/GAS_P_ovf;
				else  GAS_P=1;  //油门PI倍率
				GAS_P=1;  //油门PI倍率  测试用
			}

						
			//PI倍率计算
			PI_tem1= GAS_P * batt_P * PID.ACC_P;
			PI_tem2= GAS_P * batt_P * PID.Gier_P;	
			
					 

			//Acc混合
			M_f_tem=M_f_tem + PI_tem1*S_6050.AccNick + PI_tem2*S_6050.AccGier; 		//前
	 		M_b_tem=M_b_tem - PI_tem1*S_6050.AccNick + PI_tem2*S_6050.AccGier;		//后
	 		M_l_tem=M_l_tem - PI_tem1*S_6050.AccRoll - PI_tem2*S_6050.AccGier;		//左
	 		M_r_tem=M_r_tem + PI_tem1*S_6050.AccRoll - PI_tem2*S_6050.AccGier;		//右


			{  
				//Int 积分
				Int.Nick +=	 S_6050.AccNick - Lcom.Nick;  
				Int.Roll +=	 S_6050.AccRoll - Lcom.Roll;
				Int.Gier +=	 S_6050.AccGier; 

				//UART1_Put_Chars("Int.Gier:");//
				//UART1_Put_int(Int.Gier*10); // //

				//Intacc与angle混合
				{
					//PI倍率计算
					PI_tem1= GAS_P * batt_P * PID.Nick_I;
					PI_tem3= GAS_P * batt_P * PID.Gier_I;
					
					//加入电池曲线	 
				   	//Int混合
					M_f_tem=M_f_tem + PI_tem1*Int.Nick + PI_tem3*Int.Gier;		  //前
	 				M_b_tem=M_b_tem - PI_tem1*Int.Nick + PI_tem3*Int.Gier;		  //后
	 				M_l_tem=M_l_tem - PI_tem1*Int.Roll - PI_tem3*Int.Gier;		  //左
	 				M_r_tem=M_r_tem + PI_tem1*Int.Roll - PI_tem3*Int.Gier;		  //右 
				

					{ //即时容和:将ACC积分得到的角度值与角度值计算。  放到下一周期运行
				  	float tem_N=0,tem_R=0;

				  	tem_N=S_6050.AngNick*scal_AngAcc - Int.Nick;
					tem_R=S_6050.AngRoll*scal_AngAcc - Int.Roll;


					tem_N/=16;//16;
					tem_R/=16;//16;

					//Gier
					//tem_N=(angle_dec_math(HMC5883_data,Target_angle) * scal_360Acc) - Int.Gier;
					//tem_N/=16;
					//Int.Gier+=tem_N;

					Int.Nick+=tem_N;
					Int.Roll+=tem_R;
					}
				}

				  
				if((max_Nick==0)&&(max_Roll==0))
				{
				   	#define 	Ltime	100	 //长时积分的次数  100ms				

					Int.num++;

					LInt.Nick +=	Int.Nick;
					LInt.Roll +=	Int.Roll; 

					angInt.Nick+=S_6050.AngNick;
					angInt.Roll+=S_6050.AngRoll;

					

					if(Int.num>Ltime)
					{  float cnt;

						//UART1_Put_Chars("angInt.Nick:");
						//UART1_Put_int(angInt.Nick/Ltime*10);
						//UART1_Put_int(angInt_old.Nick/Ltime*10);
						//UART1_Put_int(LInt.Nick/Ltime/scal_AngAcc);
				  		
						//将信息发送到上位机	
					   	dub_show.AngNick=angInt.Nick/Ltime*10;
						dub_show.AngRoll=angInt.Roll/Ltime*10;

						//长时积分	 将角度积分与（ACC积分再积分=角度积分）相比较后得出补偿值	计算后，二个积分均清0重新计
						Lcom.Nick=(((angInt.Nick*scal_AngAcc)/Ltime - LInt.Nick/Ltime)/(Ltime));
						Lcom.Roll=(((angInt.Roll*scal_AngAcc)/Ltime - LInt.Roll/Ltime)/(Ltime));
						
						#define com_max   32
						if(Lcom.Nick>com_max)Lcom.Nick=com_max;
						if(Lcom.Nick<-com_max)Lcom.Nick=-com_max;
						if(Lcom.Roll>com_max)Lcom.Roll=com_max;
						if(Lcom.Roll<-com_max)Lcom.Roll=-com_max;
						
						
						//中点校正：将有补偿值积分与无补偿值二个积分角度，进行计算。以来纠正大动作飞行后，ACC漂移。
						{
						   //以下部分就是对LageKorrekturNick的修正和对陀螺仪常值误差的修正
						   #define move_limit  10*Ltime   //动作范围，大概10度
						   #define ang_limit  40*Ltime   //允许调整角度,大概20度
						   #define corr_limit 4//调整限制
						  
						   //不在大动作时 且在允许调整角度时
						   if((labs(angInt_old.Nick-angInt.Nick)<move_limit) && (labs(angInt.Nick)<ang_limit) )
						   {
						   		cnt=angInt.Nick/(Ltime*6);
								if(cnt>corr_limit) cnt=corr_limit;//限幅
								if(cnt<-corr_limit) cnt=-corr_limit;//限幅
								
								corr_6050.AccNick=-cnt;
								
								//UART1_Put_Chars("angInt.Nick:");
								//UART1_Put_int(angInt.Nick/Ltime*10);
								//UART1_Put_int(corr_6050.AccNick*1000);	

								/*UART1_Put_int(S_6050.AccNick*10);
								UART1_Put_int(LInt.Nick);
								USART_newline;	//换行	 */
														   
						   }

						   if((labs(angInt_old.Roll-angInt.Roll)<move_limit) && (labs(angInt.Roll)<ang_limit) )
						   {
						   		cnt=angInt.Roll/(Ltime*6);
								if(cnt>corr_limit) cnt=corr_limit;//限幅
								if(cnt<-corr_limit) cnt=-corr_limit;//限幅
									
								corr_6050.AccRoll=-cnt;	
								
								/*UART1_Put_Chars("angInt.Roll:");
								UART1_Put_int(angInt.Roll/Ltime*10);
								UART1_Put_int(corr_6050.AccRoll*1000);	*/			   
						   }

							memcpy(&angInt_old,&angInt,sizeof(angInt_old));//存储上一次的值
						} 

						//计算完成，清数据
						memset(&LInt,0,sizeof(LInt));  //清长时间加补偿积分参数。
						memset(&angInt,0,sizeof(angInt));  //清积分	 
						
						Int.num=0;
					}  

				
				}
				else
				{
				  memset(&Lcom,0,sizeof(Lcom));
				  memset(&angInt,0,sizeof(angInt));  //清积分
				  memset(&angInt_old,0,sizeof(angInt_old));  //清积分

				  AccY_mid =0;	
				}
				
				
				
								
			}
	 }

	
	moto_math_end:


	//#define	print_moto	 //是否打印电机信息
	
#ifndef moto_PPM_OUT	// 未定义,PWM输出	定义,PPM脉宽输出
	 

	 //限幅
	 Motor_front=check(M_f_tem)*moto_Mul;
	 Motor_back =check(M_b_tem)*moto_Mul;
	 Motor_left =check(M_l_tem)*moto_Mul;
	 Motor_right=check(M_r_tem)*moto_Mul;

											     
	#ifdef 	print_moto	

	UART1_Put_Chars("moto:");   

	UART1_Put_Chars_noend("M1:");  //front
	UART1_Put_int(Motor_front/moto_Mul);
	UART1_Put_Chars_noend("M2:");  //back
	UART1_Put_int(Motor_back/moto_Mul);
	UART1_Put_Chars_noend("M3:");  //left
	UART1_Put_int(Motor_left/moto_Mul);
	UART1_Put_Chars_noend("M4:");  //right
	UART1_Put_int(Motor_right/moto_Mul);/**/

	USART_newline;	//换行
	#endif
	



#else

	 //限幅
	 M_f_tem=check(M_f_tem);
	 M_b_tem =check(M_b_tem);
	 M_l_tem =check(M_l_tem);
	 M_r_tem=check(M_r_tem);


	 Motor_front=M_f_tem*moto_Mul+24000;
	 Motor_back =M_b_tem*moto_Mul+24000;
	 Motor_left =M_l_tem*moto_Mul+24000;
	 Motor_right=M_r_tem*moto_Mul+24000; 
	
											     
	#ifdef 	print_moto	

	UART1_Put_Chars("moto:");   

	UART1_Put_Chars_noend("MF:");  //front
	UART1_Put_int(M_f_tem);
	//UART1_Put_int(Motor_front-24000);
	UART1_Put_Chars_noend("MB:");  //back
	UART1_Put_int(M_b_tem);
	//UART1_Put_int(Motor_back-24000);
	UART1_Put_Chars_noend("ML:");  //left
	UART1_Put_int(M_l_tem);
	//UART1_Put_int(Motor_left-24000);
	UART1_Put_Chars_noend("MR:");  //right
	UART1_Put_int(M_r_tem);
	//UART1_Put_int(Motor_right-24000);

	USART_newline;	//换行
	#endif		 
#endif	 
	

} 
 


//飞控主程序
void fly_main(void)
{
	   

	   Ready_work();

	   LED4_OFF; LED3_OFF; LED2_OFF;

	   				

	 	while(1)
		{
		    	if(key)
				{
					//handset_Reply();//回复上位机
					//USART2_newline;

					key=0;
				}
				
			 	 //LED2_Turn;  LED2_ON;	 LED2_OFF;
				
				//MPU6050
				if(MPU6050_en==1)	 //1000HZ 控制
				{
					  MPU6050_en=0;						   
					  //LED4_Turn;

					  MPU6050_getMotion();

						//平均值
					  	S_6050.AngRoll=(S0_6050.AngRoll+S_6050.AngRoll)/2; 
						S_6050.AngNick=(S0_6050.AngNick+S_6050.AngNick)/2;		
						S_6050.AccRoll=(S0_6050.AccRoll+S_6050.AccRoll)/2; 						
						S_6050.AccNick=(S0_6050.AccNick+S_6050.AccNick)/2;	 
						S_6050.AccGier=(S0_6050.AccGier+S_6050.AccGier)/2;
					  
				}

				//HMC5883
				if(HMC5883_open)
				{
				  	if(HMC5883_newdata==1)
					{
					HMC58X3_getRaw();	//等SENSOR有新数据						
					//UART1_Put_int(HMC5883_data);
					}
				}

				if(ms_2_en)		//500HZ 控制
				{
				   ms_2_en=0;
				   moto_math();	//电机主控 	
				}
				

				if(dub_show_en&&wifi_ctrl)	 //允许DUG_SHOW的输出值	  500ms放在主计时里) 且上位机在线时
				{
				   dub_show_en=0;	 //允许DUG_SHOW的输出值	  200ms放在主计时里

				   //dub_show_main();//发送上位机数据
				   dub_show_fly();
				}


				//RF
				if((RF_ctrl_num>13)) 
				{  
				  RF_ctrl_num=0;

				  if(wifi_ctrl==0)
				  {
					//PPM
					if((PPM_open)) PPM_main_work();					
				  }
				  else WIFI_main_work(); 
				}

				//ADC
				if(adc_new_ok)
				{
				   
				   adc_new_ok=0; 

			   		bat=((double)ADC_Channel_data[0]/2048)*330*10;	//3.15V
					
						if(bat<370)//1080)// //低压
						{
						   if(Main_ERR==0)Main_ERR=2;	 //主错误标识	//1,开机时油门未归0
						} 						
						else
						{
						   if(Main_ERR==2) Main_ERR=0;
						}
					if(bat_chk!=bat)
					{
					  	bat_chk=bat;
						//UART1_Put_Chars("BAT:");
				   		//UART1_Put_int(bat);	/**/

						

						
					}	
				}

				//hand_set
				if((UT3_RX.ask>0)&&(UT3_RX.ask<4)&&((RUN.status==200)|(RUN.status==0)))//Handset,cmd123命令，必须在停机情况下响应
				{
				
				   if(UT3_RX.ask<5)//只响应有效命令
				   {	//LED3_Turn;
				   		//__disable_irq();	   //关闭总中断
						NVIC_SETFAULTMASK();   //关闭总中断
						

				   		set_rx_en=1;//1:禁止，由于收到上位机后，写E2ROM，需禁掉
				   		//UART1_Put_Chars("UT3_RX:");
				   		//UART1_Put_int(UT3_RX.ask);
						/*//回复上位机	*/
						handset_Reply();//回复上位机

						memset(&UT3_RX,0,sizeof(UT3_RX));
						set_rx_en=0;	//1:禁止，由于收到上位机后，写E2ROM，需禁掉

						//__enable_irq();//开放总中断  
						NVIC_RESETFAULTMASK();//开放总中断
				   	  	//LED3_Turn; 
				   }
				   //hand_set_in=1;// 1,hand_set插入   执行读写操作后，必须关闭设置界面，才可以启动电机。
				   hand_set_num=0;//1s计时
				   
				} 
				else if((UT3_RX.ask>3)&&(UT3_RX.ask<10))//Handset,cmd>4命令可在飞行状态下情况下响应
				{
					if(UT3_RX.ask>3)//只响应有效命令
				   {	//LED3_Turn;
				   		set_rx_en=1;//1:禁止，由于收到上位机后，写E2ROM，需禁掉

						/*UART1_Put_Chars("UT3_RX:");
				   		UART1_Put_int(UT3_RX.ask);	*/
						
						handset_Reply();//回复上位机 
						memset(&UT3_RX,0,sizeof(UT3_RX));

						set_rx_en=0;//1:禁止，由于收到上位机后，写E2ROM，需禁掉
				   	  	//LED3_Turn; 
				   }
				   //hand_set_in=1;// 1,hand_set插入
				   hand_set_num=0;//1s计时
				
				}
				else if((UT3_RX.ask==10))//
				{
					if(W_PPM.num==1)//只响应有效命令
				   {	//LED3_Turn;
				   		//set_rx_en=1;//1:禁止，由于收到上位机后，写E2ROM，需禁掉 						
						
						/*UART1_Put_Chars("U");
				   		//UART1_Put_int(UT3_RX.ask);	
						UART1_Put_int(W_PPM.Gas);
						//UART1_Put_int(W_PPM.Gier);
						UART1_Put_int(W_PPM.Nick);
						//UART1_Put_int(W_PPM.Roll);
						USART_newline;	//换行	  */   
						PPM[Gas_num]=W_PPM.Gas;
						PPM[Nick_num]=W_PPM.Nick+127;
						PPM[Roll_num]=W_PPM.Roll+127;
						PPM[Gier_num]=W_PPM.Gier+127;  

						W_PPM.num=0;

						if(wifi_RFok_num<5)wifi_RFok_num++;
						wifi_ctrl=1;	 //是否WIFI控制	  //在接到上位机0x0a命令后，设定为WIFI控制

						//set_rx_en=0;//1:禁止，由于收到上位机后，写E2ROM，需禁掉
				   	  	//LED3_Turn; 
				   }
				   //hand_set_in=1;// 1,hand_set插入
				   hand_set_num=0;//1s计时
				
				}
				else if(hand_set_in==1)// 1,hand_set插入
				{
					if(hand_set_1s)
					{
						hand_set_in=0;// 1,hand_set插入
						hand_set_1s=0;  //hand_set拔插检测
						
					}				
				}/**/			
		} 
}

void Ready_work(void)
{
	 memset(&Int.Nick,0,sizeof(Int));  //清积分

	S_6050_ready();//mpu6050启动时校正
	 
	 //mpu6050_star=1;

	 if(HMC5883_open)HMC5883_ready();	   //启动时校正
	 Target_angle=HMC5883_data;//目标航向

	 
	 PID_ready();


	 memcpy(&S_6050,&S0_6050,sizeof(S_6050));
	 MPU6050_initialize();   //MPU6050需二次初始化。否则数据不稳定
	 sys_star_ok=1;//1:系统启动完成	

	 UART1_Put_Chars("sys_star_run!");
}

const u16 PID_rate[]= //PID输出数据比率
{

1,	// pGAS_P; //2			//遥控油门倍数
1,	// pNick_P;//2//10		//遥控仰俯倍数
1,	// pRoll_P;//2//10		//遥控横滚倍数
10,	// pGier_P;//0.5//5	//遥控自旋倍数

1,// ACC_P ;//7//50//40 //50//35  //90
1,// Gier_P;//7//50//40 //50//35  //90

10000,// Nick_I;//0.0001//0.002// 0.012		//遥控仰俯倍数 0.02	0.002
//10000,// Roll_I;//0.0001//0.002// 0.012		//遥控横滚倍数
10000,// Gier_I;//0.0005//0.01//0.02// 0.15		//遥控自旋倍数

//自稳
  1000,		//Center_P=0;//
  10000,	//PID_out.Center_I=0;//
  1000,		//PID_out.Center_D=0;//	

  //id
  1,
};

void PID_out_setdault(void)
{
    PID_out.pGAS_P=6; //4			//遥控油门倍数
  PID_out.pNick_P=15;//2//10		//遥控仰俯倍数
  PID_out.pRoll_P=15;//2//10		//遥控横滚倍数
  PID_out.pGier_P=20;//5//0.5	//遥控自旋倍数

  PID_out.ACC_P=50;//7*0.7//50//40 //50//35  //90
  PID_out.Gier_P=80;//10;//ACC_P*1.2

  PID_out.Nick_I=300;//1;//7*0.6//0.0001//0.002// 0.012		//遥控仰俯倍数 0.02	0.002
  //PID_out.Roll_I=0;//1;//7*0.6//0.0001//0.002// 0.012		//遥控横滚倍数
  PID_out.Gier_I=800;//5;//0.0005//0.01//0.02// 0.15		//遥控自旋倍数

  //自稳
  PID_out.Center_P=0;//
  PID_out.Center_I=0;//
  PID_out.Center_D=0;//

  PID_out.ID=pid_id;	  //排除第一次开机
}
void PID_data_out(void)	  //输出设定值到PID
{
  	u8 a;

	for(a=0;a<(sizeof(PID)/sizeof(PID.pGAS_P));a++)
	{
	  (*(&PID.pGAS_P + a))= (float)(*(&PID_out.pGAS_P + a)) / PID_rate[a] ; 
	}

	UART1_Put_Chars("PID_out:"); 

	for(a=0;a<((sizeof(PID_out)/(sizeof(PID_out.pGAS_P)))-1);a++)
	{
	UART1_Put_int((*(&PID.pGAS_P + a))*PID_rate[a]);
	}

	/*UART1_Put_int(PID.pGAS_P*PID_rate[0]);		  
	UART1_Put_int(PID.pNick_P*PID_rate[1]);
	UART1_Put_int(PID.pRoll_P*PID_rate[2]);
	UART1_Put_int(PID.pGier_P*PID_rate[3]);
	UART1_Put_int(PID.ACC_P*PID_rate[4]);
	UART1_Put_int(PID.Gier_P*PID_rate[5]);
	UART1_Put_int(PID.Nick_I*PID_rate[6]);
	//UART1_Put_int(PID.Roll_I*PID_rate[7]);
	UART1_Put_int(PID.Gier_I*PID_rate[7]);*/
		
}
void PID_ready(void)
{

#define pid_dug   1		//用于PID在线调试，

	 e2rom_StrPID_read();

	 if((PID_out.ID!=pid_id)|(pid_dug==1))
	 {
	 			
	 		PID_out_setdault();
			e2rom_StrPID_write(); 
			e2rom_StrPID_read();

			UART1_Put_Chars("PID_setdault!");
			{u8 a;
			  for(a=0;a<(sizeof(PID_out)/sizeof(PID_out.pGAS_P));a++)UART1_Put_int((*(&PID_out.pGAS_P + a)));
			}
	 }

	 PID_data_out();	  //输出设定值到PID

}

 /*
void hand_Reply(void)//回复上位机
{
	Send_com3_byte(0xFD);  //头:		0xFD 0xDF
	Send_com3_byte(0xDF);  //



	if(UT3_RX.ask==1) //1	连接
	{
		Send_com3_byte(UT3_RX.ask); //返回原命令
		Send_com3_byte(1); //数据长度
			
		Send_com3_byte(101); //数据	101 命令执行成功
	}
	else if(UT3_RX.ask==2) //输出PID可设置数据
	{
		Send_com3_byte(UT3_RX.ask);	  //返回原命令
		Send_com3_byte(sizeof(PID_out)); //数据长度

		{ u8 a;	u8 b[sizeof(PID_out)];
		  
		  memcpy(&b[0],&PID_out,sizeof(PID_out));
		  for(a=0;a<(sizeof(PID_out));a++)
		  {
		  	   Send_com3_byte(b[a]);
		  }
		}
	}
	else if(UT3_RX.ask==3) //输入PID可设置数据,并存储
	{
	  
		Send_com3_byte(UT3_RX.ask); //返回原命令
		Send_com3_byte(1); //数据长度

		{struct PID_out tem;
		 memcpy(&tem, &UT3_RX.data,sizeof(tem));

		 if(tem.ID==pid_id)//ID正确，
		 {	   		 	
		 		struct PID_out tem1;					   				
				memcpy(&tem1, &PID_out,sizeof(PID_out));	//备份

		 		memcpy(&PID_out,&tem, sizeof(PID_out));
		 	    e2rom_StrPID_write(); 
				e2rom_StrPID_read();

				if(PID_out.ID==pid_id)
				{
					PID_data_out();	  //输出设定值到PID						
					Send_com3_byte(101); //数据	101 命令执行成功 					
				}
				else
				{
					memcpy(&PID_out,&tem1, sizeof(PID_out));	//备份还原
				   	Send_com3_byte(102); //数据	101 命令执行不成功 重新发送					
				}	 			
		 }
		 else  Send_com3_byte(102); //数据	101 命令执行不成功 重新发送		
		} 
		
	}
	else if(UT3_RX.ask==4) //4 bat
	{
	   	UT3_RX.bat=bat;
	   	Send_com3_byte(UT3_RX.ask); //返回原命令
		Send_com3_byte(2); //数据长度
			
		Send_com3_byte(bat); //数据
		Send_com3_byte(bat>>8); //数据
	}

} */


void test_PID_WR(void)
{
	/*u8 a;

	PID_out_setdault();

	UART1_Put_Chars("test_PID_WR:");
	
	for(a=0;a<(sizeof(PID_out)/sizeof(PID_out.pGAS_P));a++)
	{
		UART1_Put_int((*(&PID_out.pGAS_P + a)));  
	}

	e2rom_StrPID_write();

	memset(&PID_out, 0, sizeof(PID_out));
	UART1_Put_Chars("test_PID_w=0"); 	
	for(a=0;a<(sizeof(PID_out)/sizeof(PID_out.pGAS_P));a++)
	{
		UART1_Put_int(((u16)*(&PID_out.pGAS_P + a)));  
	}

	e2rom_StrPID_read();

	UART1_Put_Chars("test_PID_R:"); 
		
	for(a=0;a<(sizeof(PID_out)/sizeof(PID_out.pGAS_P));a++)
	{
		UART1_Put_int(((u16)*(&PID_out.pGAS_P + a)));  
	}*/

}


//中点校正：将有补偿值积分与无补偿值二个积分角度，进行计算。以来纠正大动作飞行后，ACC漂移。
					/*	{
						   struct Int_info err_Int;//长时间加补偿积分参数。;
						   int cnt=1;//
						   int t;

						   //平均值
						   LInt.Nick=LInt.Nick/Ltime;
						   LInt2.Nick=LInt2.Nick/Ltime;

						   LInt.Roll=LInt.Roll/Ltime;
						   LInt2.Roll=LInt2.Roll/Ltime;

						   //错误值
						   err_Int.Nick=LInt2.Nick-LInt.Nick;
						   err_Int.Roll=LInt2.Roll-LInt.Roll;

						   //更新积分，用以下次计算
						   Int2.Nick-=err_Int.Nick;
						   Int2.Roll-=err_Int.Roll;


						   //以下部分就是对LageKorrekturNick的修正和对陀螺仪常值误差的修正
						   #define move_limit  10000   //大概10度
						   #define err_limit	Ltime*4
						   #define err_limit2	Ltime*16
						   #define err_num_limit		2	//超过此数量后，才可以修改	

						   //nick
						   cnt=1;
						   if(labs(LInt_old.Nick-LInt.Nick)<move_limit)
						   {  UART1_Put_Chars("err_Int.Nick:");
						   		UART1_Put_int(err_Int.Nick);
						      
						   	  	if(err_Int.Nick>err_limit2)
								{
									  if(err_numP.Nick>(err_num_limit-1))
									  {		 									  		
									  		//连续两次误差较大时，对陀螺仪漂移进行补偿											
            								cnt += labs(err_Int.Nick) / err_limit2; //误差越大增加越大
            								t = err_Int.Nick / 8;
            								if(t > 5000) t = 5000;
            								Lcom.Nick += t / Ltime;  //需验证
											
									  }
									  else
									  {
									  		if(err_numP.Nick<err_num_limit)err_numP.Nick++;
									  }
								}
								else if(err_Int.Nick< -err_limit2)
								{
									  if(err_numN.Nick>(err_num_limit-1))
									  {		 									  		
									  		//连续两次误差较大时，对陀螺仪漂移进行补偿											
            								cnt += labs(err_Int.Nick) / err_limit2; //误差越大增加越大
            								t = err_Int.Nick / 8;
            								if(t < -5000) t = -5000;
            								Lcom.Nick += t / Ltime;  //需验证
											
									  }
									  else
									  {
									  		if(err_numN.Nick<err_num_limit)err_numN.Nick++;
									  }
								
								}

						   
						   }
						   else cnt=0;

						   

						   if(cnt>4) cnt=4;//限幅

						   //if(err_Int.Nick >  err_limit)   AccY_mid += cnt;
        				   //if(err_Int.Nick < -err_limit)   AccY_mid -= cnt;

						   	UART1_Put_Chars("ERR_cnt:");
											UART1_Put_int(cnt);
											UART1_Put_int(AccY_mid);							
											USART_newline;

						   //roll
						   cnt=1;
						   if(labs(LInt_old.Roll-LInt.Roll)<move_limit)
						   {
						   	  	if(err_Int.Roll>err_limit2)
								{
									  if(err_numP.Roll>(err_num_limit-1))
									  {		 									  		
									  		//连续两次误差较大时，对陀螺仪漂移进行补偿											
            								cnt += labs(err_Int.Roll) / err_limit2; //误差越大增加越大
            								t = err_Int.Roll / 8;
            								if(t > 5000) t = 5000;
            								Lcom.Roll += t / Ltime;  //需验证
									  }
									  else
									  {
									  		if(err_numP.Roll<err_num_limit)err_numP.Roll++;
									  }
								}
								else if(err_Int.Roll< -err_limit2)
								{
									  if(err_numN.Roll>(err_num_limit-1))
									  {		 									  		
									  		//连续两次误差较大时，对陀螺仪漂移进行补偿											
            								cnt += labs(err_Int.Roll) / err_limit2; //误差越大增加越大
            								t = err_Int.Roll / 8;
            								if(t < -5000) t = -5000;
            								Lcom.Roll += t / Ltime;  //需验证
									  }
									  else
									  {
									  		if(err_numN.Roll<err_num_limit)err_numN.Roll++;
									  }
								
								}
						   }
						   else cnt=0;

						   

						   if(cnt>4) cnt=4;//限幅

						   //if(err_Int.Roll >  err_limit)   AccX_mid += cnt;
        				   //if(err_Int.Roll < -err_limit)   AccX_mid -= cnt;
																						
						   	

							memcpy(&LInt_old,&LInt,sizeof(LInt_old));//存储上一次的平均值
						}  	 */



/*
				//center_P	 中心稳定P
				{

				   	
					CInt.Nick+= S_6050.AngNick;
					CInt.Roll+= S_6050.AngRoll;

					Cdif.Nick= S_6050.AngNick - C_6050.AngNick;//Cdif.Nick;
					Cdif.Roll= S_6050.AngRoll - C_6050.AngRoll;

					memcpy(&C_6050,&S_6050,sizeof(S_6050));	  //存本次值

					//UART1_Put_Chars("CInt:");
					UART1_Put_int(CInt.Nick);
					UART1_Put_int(S_6050.AngNick);
					UART1_Put_int(Cdif.Nick);
					USART_newline;	//换行	// 
					

				  	M_f_tem=M_f_tem + S_6050.AngNick*PID.Center_P + CInt.Nick*PID.Center_I + Cdif.Nick*PID.Center_D;
					M_b_tem=M_b_tem - S_6050.AngNick*PID.Center_P - CInt.Nick*PID.Center_I - Cdif.Nick*PID.Center_D;
					M_l_tem=M_l_tem - S_6050.AngRoll*PID.Center_P - CInt.Roll*PID.Center_I - Cdif.Roll*PID.Center_D;
					M_r_tem=M_r_tem + S_6050.AngRoll*PID.Center_P + CInt.Roll*PID.Center_I + Cdif.Roll*PID.Center_D; 
				} 	 */
			    	 

//------------------End of File----------------------------
