

/********************LAMBO Fly soft********************


star:
13-01-07    edit by edmondchao	
*******************************************************************************/
#include "main.h"


#define  GAS_P_ovf  70	 //���ų���GAS_P_ovf��GAS_PΪ1


volatile u16 Motor_front=moto_star,Motor_back=moto_star,Motor_left=moto_star,Motor_right=moto_star;//ת��


struct RUN_info RUN;//RNN-������
struct Int_info Int;//�����ּӲ���������ACC����
//struct Int_info Int2;//�������޲���������
struct Int_info LInt;//��ʱ��Ӳ������ֲ�����//ACC���ֺ��ٻ��֣�=�Ƕ�
//struct Int_info LInt2;//��ʱ���޲������ֲ�����
struct Int_info angInt;//��ʱ��ǶȻ��ֲ�����
struct Int_info Lcom;//��ʱ�������ֲ���ֵ��
//struct Int_info LInt_old;//��ʱ����������һ�ε�ֵ��		�˶�����
struct Int_info err_numP;//��¼���ֳ����޴���������2������е�����
struct Int_info err_numN;//��¼���ֳ����޴���������2������е�����	

struct Int_info angInt_old;// ��angInt�Ƚϣ��ڶ�̬�У�����С���޶�ֵʱ�����е�����	

struct Int_info Gier_Int;//Ŀ�ģ��Ƿ���״̬�£�����Gier��У��	


struct Int_info PInt;//PPM���ֲ�����
struct Int_info CInt;//���ȶ����ֲ�����
struct Int_info Cdif;//���ȶ�΢�ֲ�����
struct mpu6050_info C_6050;//���ȶ�ʱ��Ҫ��һ��mpu6050�����ֵ��

u8 Nick_mid=0,Roll_mid=0,Gier_mid=0;  //������ʱ��PPM����ֵ(�˵���ܲ���������127,��ң�����е������΢������)��

u8 PPM[10]={

 0,0,0,0,0,0,0,0,0,0,

};
struct PPM_info W_PPM;//WIFI_ppm������


u8 Main_ERR=0;	  //�������ʶ	//1,RF���� 2,����
u8 sys_star_ok=0;//1:ϵͳ�������

u16 Target_angle=0;//Ŀ�꺽��

float  GAS_P=1;  //����PI����	 ������������ʱPI���ߣ�����ЧӦ����Ҷ���
float  batt_P=1;  //���PI����	 ������������ڵ����ߵ�ʱ�Ĳ�ͬ��Ӧ��
float  bat_tem=0;  //����˲�
u16  bat=0;	//���
u16 bat_chk=0;//���ڼ���ص�ѹ�Ƿ�仯
u8  bat_gas=0;//���ڵ�ѹ����  Gas-=bat_gas


u8 gas_delay=0; //���ź�ʱ,���Ż����½���ʱ��

struct PID_info PID;//PID������
struct PID_out PID_out;	 //PID�������������

//u8  moto_ON=1;	 //�������	1,hand-set������0
u8  hand_set_in=0;// 1,hand_set����

struct dub_show_info dub_show;	 //DUG_SHOW�����ֵ
u8 dub_show_en=0;	 //����DUG_SHOW�����ֵ	  100ms��������ʱ��

u8 ms_2_en=0;	 //500HZ �ź�  ������ʱ���sys_tim_isr(void)��	//500hz 2ms
struct mpu6050_info S_6050;//mpu6050�����ֵ��

u8 wifi_RFok_num=0;//��ȷ����WIFI�źŴ�����	������ʱ�ÿ200MS��1��Ϊ0ʱ��ʧ��
u8 wifi_ctrl=1;	 //�Ƿ�WIFI����	  //�ڽӵ���λ��0x0a������趨ΪWIFI����
u8 RF_ctrl_num=0;//��������ʱ�����Ϊ30MS����һ��PPM_MAIN_WORK
u8 PPM_open=1; //	1,��


void dub_show_main(void)//������λ������
{
	 /*	dub_show.id=0xfd;

	 	dub_show.AngNick=S_6050.AngNick;	//�Ƕ�
		dub_show.AccNick=S_6050.AccNick;
		dub_show.IntNick=Int.Nick;

  		dub_show.AngRoll=S_6050.AngRoll;
  		dub_show.AccRoll=S_6050.AccRoll;	//�Ǽ��ٶ�
		dub_show.IntRoll=Int.Roll;
  		
  		dub_show.AccGier=S_6050.AccGier;
		dub_show.IntGier=Int.Gier;	



		dub_show.bat=bat_tem;	 

		//UART1_Put_Char(0xfd); //ͷ
		UART1_Put_num(&dub_show.id1,sizeof(dub_show));	//�ӵ�ַ����һ������ */
}

void dub_show_fly(void)//������λ������
{
	
	

	u8 check=0;//У����:  ^�������� 
	u8 lenth=0;//���ݳ���	

	uart_Send_byte(0xFD);  //ͷ:		0xFD 0xDF
	uart_Send_byte(0xDF);  //

	check^=0xDF^0xFD;


		lenth=sizeof(dub_show);;//���ݳ���

		uart_Send_byte(0x0a); //����ԭ����
		uart_Send_byte(lenth); //���ݳ���

		check^=0x0a^lenth;
	 	
		dub_show.bat=bat_tem;  
		dub_show.Height=0;
		
		
		//uart_Send_num(&dub_show,sizeof(dub_show));	//�ӵ�ַ����һ������	 


		//У����
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
		USART_newline;	//����*/
		}
		uart_Send_byte(check);//
}

/******PPM*******/
void  PPM_Ave(void)//ƽ��ֵ
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
	 			   if((PPM[Gas_num]>star_Gas)&&(RUN.status==200))	 //�����������ţ��Ҹղ���ͣ��
				   {
				   	   RUN.status=1;  //����				   
				   }
				   else if((PPM[Gas_num]>star_Gas)&&(RUN.status==201)) //�����������ţ��Ҹղ��Ƿ���ʱ��ʧ�ź�
				   {
				   	   RUN.status=1;  //����				   
				   }
				   else if((PPM[Gas_num]>star_Gas)&&(RUN.status==0))	
				   {
				   	   Main_ERR=1;	 //�������ʶ  1,RF����
				   }
				   else if((PPM[Gas_num]<star_Gas))	
				   {
				   	   RUN.status=200;  //����
					   if(Main_ERR==1)Main_ERR=0;	 //�������ʶ 1,RF����
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

u8  ppm_ok_old=200;//���ڼ��ppm_data_ok�Ƿ�仯
void PPM_main_work(void)
{				
				//UART1_Put_Chars("PPM connect");
	  		   //UART1_Put_int(ppm_data_ok);
				//LED1_Turn; 
				if((ppm_data_ok>(ppm_ok_num-4)) )
				{
				   
				   //ppm_ok_old=ppm_data_ok;  //������
				   //ppm_data_ok--;		   //������

				   //LED1_Turn;  //
				   if(RUN.status==1) PPM_Ave();//ƽ��ֵ   				   
				   else  memcpy(&PPM[Chanl_star],&ppm_in[Chanl_star],Chanl_num);				   

				   RF_ctrl_mode_check();
				   			
				}
				else if(ppm_data_ok==0)	//���� 
				{
					if(RUN.status==1)  //�����ǰ�Ƿ���״̬ʱ�������Ϊ��ʧ�ź�״̬
					{
						RUN.status=201;	
						gas_delay=0; //���ź�ʱ,���Ż����½���ʱ��
					}
					else if(RUN.status!=201)RUN.status=0;  //�������Ϊ���ź�״̬

					Main_ERR=1;	 //�������ʶ	//1,����ʱ����δ��0

					
					if(ppm_ok_old!=ppm_data_ok)
					{
						ppm_ok_old=ppm_data_ok;
						UART1_Put_Chars("PPM connect err!");
					}					 
				}

}

void WIFI_main_work(void)
{
	

	if(PPM_open==1) //	1,��
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
					if(RUN.status==1)  //�����ǰ�Ƿ���״̬ʱ�������Ϊ��ʧ�ź�״̬
					{
						RUN.status=201;	
						gas_delay=0; //���ź�ʱ,���Ż����½���ʱ��
					}
					else if(RUN.status!=201)RUN.status=0;  //�������Ϊ���ź�״̬
					UART1_Put_Chars("wifi connect err!");

					Main_ERR=1;	 //�������ʶ	//1,����ʱ����δ��0	 
					LED2_OFF;


				wifi_ctrl=0;	 //�Ƿ�WIFI����		
				//����
				PPM_open=1; //
				PPM_Init();		
		}


}

#define scal_AngAcc   172//1000  //angle to IntAcc���� NICK��Roll   11000/64=171.5
#define scal_360Acc   100   //�ش�360 to IntAcc���� Gier 	16000/180=88.8
/*
#define pGAS_P     	6  		//ң�����ű���
#define pNick_P     5//10		//ң����������
#define pRoll_P     5//10		//ң�غ������
#define pGier_P     3//5		//ң����������

//PID
#define ACC_P     7//50//40 //50//35  //90

#define pNick_I    0.0001//0.002// 0.012		//ң���������� 0.02	0.002
#define pRoll_I    0.0001//0.002// 0.012		//ң�غ������
#define pGier_I    0.0005//0.01//0.02// 0.15		//ң����������
*/
/*
#define pGAS_P     	4  		//ң�����ű���
#define pNick_P     2//10		//ң����������
#define pRoll_P     2//10		//ң�غ������
#define pGier_P     0.5//5		//ң����������

//PID
#define ACC_P     7//50//40 //50//35  //90

#define pNick_I    0.0001//0.002// 0.012		//ң���������� 0.02	0.002
#define pRoll_I    0.0001//0.002// 0.012		//ң�غ������
#define pGier_I    0.0005//0.01//0.02// 0.15		//ң����������	*/

#define center_P    0//.8//1//0.002// 0.012		//�����ȶ�P
#define center_I    0//.0005// 0.012//		//�����ȶ�I
#define center_D    0//0.01//0.02// 0.15	//�����ȶ�D


#define in_max 50
#define in_min 10
u8 max_Gier=0; //    �������ź�������
u8 max_Nick=0; //    �������ź�������
u8 max_Roll=0; //    �������ź�������

//����Ƿ�������Χ	�޷�
//#define check(speed)	{ if(speed>moto_max) speed=moto_max; if(speed<moto_mix)speed=moto_mix; }
s16  check(s16 speed)
{
  	if(speed>moto_max) speed=moto_max; 
	if(speed<moto_mix) speed=moto_mix;

	return speed;
}


//��������е��Ƿ�����������
s16  P_check(s16 PPM)
{
    #define PPM_star 	5

	if(abs(PPM)>PPM_star)//�������ֵ����PPM_star
	{
		if(PPM>0)  					return PPM-PPM_star;   //
		else   						return PPM+PPM_star; 	
	}
	else 	return 0;	
}   



void moto_math(void)	//�������
{
	 s16 M_f_tem=0,M_b_tem=0,M_l_tem=0,M_r_tem=0;

	 //LED4_Turn;
						 //��ش���
						bat_tem=bat;  //����˲�
						bat_tem=(bat_tem*10+bat)/11;  //����˲�
						//UART1_Put_Chars_noend("bat_tem:"); 
						//UART1_Put_int(bat_tem);
	 
	 //RUN.status=1;//���ڲ���
	 //UART1_Put_int(RUN.status);
	 if((RUN.status==200)|(RUN.status==0))//δ���������ź�
	 {
			
	 		
	 		M_f_tem=M_b_tem=M_l_tem=M_r_tem=0;

			memset(&Int.Nick,0,sizeof(Int));  //�������ּӲ���
			//memset(&Int2.Nick,0,sizeof(Int2));  //���������޲���������



			//��PPM�е㣬���֣�У�����ݽǶ��е�
			//memset(&PInt,0,sizeof(PInt));  //�����

			//�����ݺ�
			memset(&LInt,0,sizeof(LInt));  //�峤ʱ��Ӳ������ֲ�����
			//memset(&LInt2,0,sizeof(LInt2));  //�峤ʱ���޲������ֲ�����
			memset(&Lcom,0,sizeof(Lcom));  //�����

			memset(&angInt,0,sizeof(angInt));  //�����

			//memset(&LInt_old,0,sizeof(LInt_old));  //�����
			memset(&err_numP,0,sizeof(err_numP));  //�����
			memset(&err_numN,0,sizeof(err_numN));  //�����
			
			memset(&angInt_old,0,sizeof(angInt_old));  //�����			

			//�Զ�ƽ��
			memset(&CInt,0,sizeof(CInt));  //�����
			memset(&Cdif,0,sizeof(Cdif));  //�����


			//��PPM����
			memset(&PInt,0,sizeof(PInt));  //�����

			Target_angle=HMC5883_data;//Ŀ�꺽��


						//����Ϣ���͵���λ��	
					   	dub_show.AngNick=S_6050.AngNick*10;
						dub_show.AngRoll=S_6050.AngRoll*10;


			//������ʱ��PPM����ֵ(�˵���ܲ���������127,��ң�����е������΢������)��
			Nick_mid=PPM[Nick_num];
			Roll_mid=PPM[Roll_num];
			Gier_mid=PPM[Gier_num];   /**/

			//Ŀ�ģ��Ƿ���״̬�£�����Gier��У�� /
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

	 
	 
	 if((RUN.status==1)|(RUN.status==201))//����	| ����ʱ��ʧ�ź�
	 {
			s16 gas_tem,Nick_tem,Roll_tem,Gier_tem;
			float PI_tem1,PI_tem2,PI_tem3;

			//memset(&Gier_Int,0,sizeof(Gier_Int));//	 //Ŀ�ģ��Ƿ���״̬�£�����Gier��У��

			//���PI���� + ��ѹ����
					   	batt_P=(((430 - bat_tem)/50)*0.2)+0.6;
						batt_P=1;  //���PI����	//����ʱʹ��
						/*UART1_Put_int(bat);
						UART1_Put_int(batt_P*100);
						USART_newline;*/

						#define  bat_gas_ovf  	370	 //��ѹ����bat_gas_ovf��bat_gasΪ0
						#define  bat_gas_limit  350	 //��ѹ����bat_gas_ovf��bat_gasΪ0
						//��ѹ����
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
				 
			    if(RUN.status==1) //������
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
				else  if((RUN.status==201))// ����ʱ��ʧ�ź�
				{
					
					if(PPM[Gas_num]>star_Gas) 
					{
						 if(gas_delay<10)gas_delay++; //���ź�ʱ,���Ż����½���ʱ��
						 else 
						 {
						  gas_delay=0;
						  PPM[Gas_num]--;  //���ŵݼ�
						 } 						 
					}
					else 
					{
					   	PPM[Gas_num]=0;
						RUN.status=0;//�����ŵݼ�Ϊ0ʱ,��״̬���Ļ����ź�
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

				//PPM���
	 		M_f_tem=gas_tem*PID.pGAS_P - Nick_tem*PID.pNick_P - Gier_tem*PID.pGier_P; //ǰ
	 		M_b_tem=gas_tem*PID.pGAS_P + Nick_tem*PID.pNick_P - Gier_tem*PID.pGier_P; //��

	 		M_l_tem=gas_tem*PID.pGAS_P + Roll_tem*PID.pRoll_P + Gier_tem*PID.pGier_P; //��
	 		M_r_tem=gas_tem*PID.pGAS_P - Roll_tem*PID.pRoll_P + Gier_tem*PID.pGier_P; //��		    
			
			 
				//����PI����
				if(gas_tem<GAS_P_ovf) GAS_P=gas_tem/GAS_P_ovf;
				else  GAS_P=1;  //����PI����
				GAS_P=1;  //����PI����  ������
			}

						
			//PI���ʼ���
			PI_tem1= GAS_P * batt_P * PID.ACC_P;
			PI_tem2= GAS_P * batt_P * PID.Gier_P;	
			
					 

			//Acc���
			M_f_tem=M_f_tem + PI_tem1*S_6050.AccNick + PI_tem2*S_6050.AccGier; 		//ǰ
	 		M_b_tem=M_b_tem - PI_tem1*S_6050.AccNick + PI_tem2*S_6050.AccGier;		//��
	 		M_l_tem=M_l_tem - PI_tem1*S_6050.AccRoll - PI_tem2*S_6050.AccGier;		//��
	 		M_r_tem=M_r_tem + PI_tem1*S_6050.AccRoll - PI_tem2*S_6050.AccGier;		//��


			{  
				//Int ����
				Int.Nick +=	 S_6050.AccNick - Lcom.Nick;  
				Int.Roll +=	 S_6050.AccRoll - Lcom.Roll;
				Int.Gier +=	 S_6050.AccGier; 

				//UART1_Put_Chars("Int.Gier:");//
				//UART1_Put_int(Int.Gier*10); // //

				//Intacc��angle���
				{
					//PI���ʼ���
					PI_tem1= GAS_P * batt_P * PID.Nick_I;
					PI_tem3= GAS_P * batt_P * PID.Gier_I;
					
					//����������	 
				   	//Int���
					M_f_tem=M_f_tem + PI_tem1*Int.Nick + PI_tem3*Int.Gier;		  //ǰ
	 				M_b_tem=M_b_tem - PI_tem1*Int.Nick + PI_tem3*Int.Gier;		  //��
	 				M_l_tem=M_l_tem - PI_tem1*Int.Roll - PI_tem3*Int.Gier;		  //��
	 				M_r_tem=M_r_tem + PI_tem1*Int.Roll - PI_tem3*Int.Gier;		  //�� 
				

					{ //��ʱ�ݺ�:��ACC���ֵõ��ĽǶ�ֵ��Ƕ�ֵ���㡣  �ŵ���һ��������
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
				   	#define 	Ltime	100	 //��ʱ���ֵĴ���  100ms				

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
				  		
						//����Ϣ���͵���λ��	
					   	dub_show.AngNick=angInt.Nick/Ltime*10;
						dub_show.AngRoll=angInt.Roll/Ltime*10;

						//��ʱ����	 ���ǶȻ����루ACC�����ٻ���=�ǶȻ��֣���ȽϺ�ó�����ֵ	����󣬶������־���0���¼�
						Lcom.Nick=(((angInt.Nick*scal_AngAcc)/Ltime - LInt.Nick/Ltime)/(Ltime));
						Lcom.Roll=(((angInt.Roll*scal_AngAcc)/Ltime - LInt.Roll/Ltime)/(Ltime));
						
						#define com_max   32
						if(Lcom.Nick>com_max)Lcom.Nick=com_max;
						if(Lcom.Nick<-com_max)Lcom.Nick=-com_max;
						if(Lcom.Roll>com_max)Lcom.Roll=com_max;
						if(Lcom.Roll<-com_max)Lcom.Roll=-com_max;
						
						
						//�е�У�������в���ֵ�������޲���ֵ�������ֽǶȣ����м��㡣���������������к�ACCƯ�ơ�
						{
						   //���²��־��Ƕ�LageKorrekturNick�������Ͷ������ǳ�ֵ��������
						   #define move_limit  10*Ltime   //������Χ�����10��
						   #define ang_limit  40*Ltime   //��������Ƕ�,���20��
						   #define corr_limit 4//��������
						  
						   //���ڴ���ʱ ������������Ƕ�ʱ
						   if((labs(angInt_old.Nick-angInt.Nick)<move_limit) && (labs(angInt.Nick)<ang_limit) )
						   {
						   		cnt=angInt.Nick/(Ltime*6);
								if(cnt>corr_limit) cnt=corr_limit;//�޷�
								if(cnt<-corr_limit) cnt=-corr_limit;//�޷�
								
								corr_6050.AccNick=-cnt;
								
								//UART1_Put_Chars("angInt.Nick:");
								//UART1_Put_int(angInt.Nick/Ltime*10);
								//UART1_Put_int(corr_6050.AccNick*1000);	

								/*UART1_Put_int(S_6050.AccNick*10);
								UART1_Put_int(LInt.Nick);
								USART_newline;	//����	 */
														   
						   }

						   if((labs(angInt_old.Roll-angInt.Roll)<move_limit) && (labs(angInt.Roll)<ang_limit) )
						   {
						   		cnt=angInt.Roll/(Ltime*6);
								if(cnt>corr_limit) cnt=corr_limit;//�޷�
								if(cnt<-corr_limit) cnt=-corr_limit;//�޷�
									
								corr_6050.AccRoll=-cnt;	
								
								/*UART1_Put_Chars("angInt.Roll:");
								UART1_Put_int(angInt.Roll/Ltime*10);
								UART1_Put_int(corr_6050.AccRoll*1000);	*/			   
						   }

							memcpy(&angInt_old,&angInt,sizeof(angInt_old));//�洢��һ�ε�ֵ
						} 

						//������ɣ�������
						memset(&LInt,0,sizeof(LInt));  //�峤ʱ��Ӳ������ֲ�����
						memset(&angInt,0,sizeof(angInt));  //�����	 
						
						Int.num=0;
					}  

				
				}
				else
				{
				  memset(&Lcom,0,sizeof(Lcom));
				  memset(&angInt,0,sizeof(angInt));  //�����
				  memset(&angInt_old,0,sizeof(angInt_old));  //�����

				  AccY_mid =0;	
				}
				
				
				
								
			}
	 }

	
	moto_math_end:


	//#define	print_moto	 //�Ƿ��ӡ�����Ϣ
	
#ifndef moto_PPM_OUT	// δ����,PWM���	����,PPM�������
	 

	 //�޷�
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

	USART_newline;	//����
	#endif
	



#else

	 //�޷�
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

	USART_newline;	//����
	#endif		 
#endif	 
	

} 
 


//�ɿ�������
void fly_main(void)
{
	   

	   Ready_work();

	   LED4_OFF; LED3_OFF; LED2_OFF;

	   				

	 	while(1)
		{
		    	if(key)
				{
					//handset_Reply();//�ظ���λ��
					//USART2_newline;

					key=0;
				}
				
			 	 //LED2_Turn;  LED2_ON;	 LED2_OFF;
				
				//MPU6050
				if(MPU6050_en==1)	 //1000HZ ����
				{
					  MPU6050_en=0;						   
					  //LED4_Turn;

					  MPU6050_getMotion();

						//ƽ��ֵ
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
					HMC58X3_getRaw();	//��SENSOR��������						
					//UART1_Put_int(HMC5883_data);
					}
				}

				if(ms_2_en)		//500HZ ����
				{
				   ms_2_en=0;
				   moto_math();	//������� 	
				}
				

				if(dub_show_en&&wifi_ctrl)	 //����DUG_SHOW�����ֵ	  500ms��������ʱ��) ����λ������ʱ
				{
				   dub_show_en=0;	 //����DUG_SHOW�����ֵ	  200ms��������ʱ��

				   //dub_show_main();//������λ������
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
					
						if(bat<370)//1080)// //��ѹ
						{
						   if(Main_ERR==0)Main_ERR=2;	 //�������ʶ	//1,����ʱ����δ��0
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
				if((UT3_RX.ask>0)&&(UT3_RX.ask<4)&&((RUN.status==200)|(RUN.status==0)))//Handset,cmd123���������ͣ���������Ӧ
				{
				
				   if(UT3_RX.ask<5)//ֻ��Ӧ��Ч����
				   {	//LED3_Turn;
				   		//__disable_irq();	   //�ر����ж�
						NVIC_SETFAULTMASK();   //�ر����ж�
						

				   		set_rx_en=1;//1:��ֹ�������յ���λ����дE2ROM�������
				   		//UART1_Put_Chars("UT3_RX:");
				   		//UART1_Put_int(UT3_RX.ask);
						/*//�ظ���λ��	*/
						handset_Reply();//�ظ���λ��

						memset(&UT3_RX,0,sizeof(UT3_RX));
						set_rx_en=0;	//1:��ֹ�������յ���λ����дE2ROM�������

						//__enable_irq();//�������ж�  
						NVIC_RESETFAULTMASK();//�������ж�
				   	  	//LED3_Turn; 
				   }
				   //hand_set_in=1;// 1,hand_set����   ִ�ж�д�����󣬱���ر����ý��棬�ſ������������
				   hand_set_num=0;//1s��ʱ
				   
				} 
				else if((UT3_RX.ask>3)&&(UT3_RX.ask<10))//Handset,cmd>4������ڷ���״̬���������Ӧ
				{
					if(UT3_RX.ask>3)//ֻ��Ӧ��Ч����
				   {	//LED3_Turn;
				   		set_rx_en=1;//1:��ֹ�������յ���λ����дE2ROM�������

						/*UART1_Put_Chars("UT3_RX:");
				   		UART1_Put_int(UT3_RX.ask);	*/
						
						handset_Reply();//�ظ���λ�� 
						memset(&UT3_RX,0,sizeof(UT3_RX));

						set_rx_en=0;//1:��ֹ�������յ���λ����дE2ROM�������
				   	  	//LED3_Turn; 
				   }
				   //hand_set_in=1;// 1,hand_set����
				   hand_set_num=0;//1s��ʱ
				
				}
				else if((UT3_RX.ask==10))//
				{
					if(W_PPM.num==1)//ֻ��Ӧ��Ч����
				   {	//LED3_Turn;
				   		//set_rx_en=1;//1:��ֹ�������յ���λ����дE2ROM������� 						
						
						/*UART1_Put_Chars("U");
				   		//UART1_Put_int(UT3_RX.ask);	
						UART1_Put_int(W_PPM.Gas);
						//UART1_Put_int(W_PPM.Gier);
						UART1_Put_int(W_PPM.Nick);
						//UART1_Put_int(W_PPM.Roll);
						USART_newline;	//����	  */   
						PPM[Gas_num]=W_PPM.Gas;
						PPM[Nick_num]=W_PPM.Nick+127;
						PPM[Roll_num]=W_PPM.Roll+127;
						PPM[Gier_num]=W_PPM.Gier+127;  

						W_PPM.num=0;

						if(wifi_RFok_num<5)wifi_RFok_num++;
						wifi_ctrl=1;	 //�Ƿ�WIFI����	  //�ڽӵ���λ��0x0a������趨ΪWIFI����

						//set_rx_en=0;//1:��ֹ�������յ���λ����дE2ROM�������
				   	  	//LED3_Turn; 
				   }
				   //hand_set_in=1;// 1,hand_set����
				   hand_set_num=0;//1s��ʱ
				
				}
				else if(hand_set_in==1)// 1,hand_set����
				{
					if(hand_set_1s)
					{
						hand_set_in=0;// 1,hand_set����
						hand_set_1s=0;  //hand_set�β���
						
					}				
				}/**/			
		} 
}

void Ready_work(void)
{
	 memset(&Int.Nick,0,sizeof(Int));  //�����

	S_6050_ready();//mpu6050����ʱУ��
	 
	 //mpu6050_star=1;

	 if(HMC5883_open)HMC5883_ready();	   //����ʱУ��
	 Target_angle=HMC5883_data;//Ŀ�꺽��

	 
	 PID_ready();


	 memcpy(&S_6050,&S0_6050,sizeof(S_6050));
	 MPU6050_initialize();   //MPU6050����γ�ʼ�����������ݲ��ȶ�
	 sys_star_ok=1;//1:ϵͳ�������	

	 UART1_Put_Chars("sys_star_run!");
}

const u16 PID_rate[]= //PID������ݱ���
{

1,	// pGAS_P; //2			//ң�����ű���
1,	// pNick_P;//2//10		//ң����������
1,	// pRoll_P;//2//10		//ң�غ������
10,	// pGier_P;//0.5//5	//ң����������

1,// ACC_P ;//7//50//40 //50//35  //90
1,// Gier_P;//7//50//40 //50//35  //90

10000,// Nick_I;//0.0001//0.002// 0.012		//ң���������� 0.02	0.002
//10000,// Roll_I;//0.0001//0.002// 0.012		//ң�غ������
10000,// Gier_I;//0.0005//0.01//0.02// 0.15		//ң����������

//����
  1000,		//Center_P=0;//
  10000,	//PID_out.Center_I=0;//
  1000,		//PID_out.Center_D=0;//	

  //id
  1,
};

void PID_out_setdault(void)
{
    PID_out.pGAS_P=6; //4			//ң�����ű���
  PID_out.pNick_P=15;//2//10		//ң����������
  PID_out.pRoll_P=15;//2//10		//ң�غ������
  PID_out.pGier_P=20;//5//0.5	//ң����������

  PID_out.ACC_P=50;//7*0.7//50//40 //50//35  //90
  PID_out.Gier_P=80;//10;//ACC_P*1.2

  PID_out.Nick_I=300;//1;//7*0.6//0.0001//0.002// 0.012		//ң���������� 0.02	0.002
  //PID_out.Roll_I=0;//1;//7*0.6//0.0001//0.002// 0.012		//ң�غ������
  PID_out.Gier_I=800;//5;//0.0005//0.01//0.02// 0.15		//ң����������

  //����
  PID_out.Center_P=0;//
  PID_out.Center_I=0;//
  PID_out.Center_D=0;//

  PID_out.ID=pid_id;	  //�ų���һ�ο���
}
void PID_data_out(void)	  //����趨ֵ��PID
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

#define pid_dug   1		//����PID���ߵ��ԣ�

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

	 PID_data_out();	  //����趨ֵ��PID

}

 /*
void hand_Reply(void)//�ظ���λ��
{
	Send_com3_byte(0xFD);  //ͷ:		0xFD 0xDF
	Send_com3_byte(0xDF);  //



	if(UT3_RX.ask==1) //1	����
	{
		Send_com3_byte(UT3_RX.ask); //����ԭ����
		Send_com3_byte(1); //���ݳ���
			
		Send_com3_byte(101); //����	101 ����ִ�гɹ�
	}
	else if(UT3_RX.ask==2) //���PID����������
	{
		Send_com3_byte(UT3_RX.ask);	  //����ԭ����
		Send_com3_byte(sizeof(PID_out)); //���ݳ���

		{ u8 a;	u8 b[sizeof(PID_out)];
		  
		  memcpy(&b[0],&PID_out,sizeof(PID_out));
		  for(a=0;a<(sizeof(PID_out));a++)
		  {
		  	   Send_com3_byte(b[a]);
		  }
		}
	}
	else if(UT3_RX.ask==3) //����PID����������,���洢
	{
	  
		Send_com3_byte(UT3_RX.ask); //����ԭ����
		Send_com3_byte(1); //���ݳ���

		{struct PID_out tem;
		 memcpy(&tem, &UT3_RX.data,sizeof(tem));

		 if(tem.ID==pid_id)//ID��ȷ��
		 {	   		 	
		 		struct PID_out tem1;					   				
				memcpy(&tem1, &PID_out,sizeof(PID_out));	//����

		 		memcpy(&PID_out,&tem, sizeof(PID_out));
		 	    e2rom_StrPID_write(); 
				e2rom_StrPID_read();

				if(PID_out.ID==pid_id)
				{
					PID_data_out();	  //����趨ֵ��PID						
					Send_com3_byte(101); //����	101 ����ִ�гɹ� 					
				}
				else
				{
					memcpy(&PID_out,&tem1, sizeof(PID_out));	//���ݻ�ԭ
				   	Send_com3_byte(102); //����	101 ����ִ�в��ɹ� ���·���					
				}	 			
		 }
		 else  Send_com3_byte(102); //����	101 ����ִ�в��ɹ� ���·���		
		} 
		
	}
	else if(UT3_RX.ask==4) //4 bat
	{
	   	UT3_RX.bat=bat;
	   	Send_com3_byte(UT3_RX.ask); //����ԭ����
		Send_com3_byte(2); //���ݳ���
			
		Send_com3_byte(bat); //����
		Send_com3_byte(bat>>8); //����
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


//�е�У�������в���ֵ�������޲���ֵ�������ֽǶȣ����м��㡣���������������к�ACCƯ�ơ�
					/*	{
						   struct Int_info err_Int;//��ʱ��Ӳ������ֲ�����;
						   int cnt=1;//
						   int t;

						   //ƽ��ֵ
						   LInt.Nick=LInt.Nick/Ltime;
						   LInt2.Nick=LInt2.Nick/Ltime;

						   LInt.Roll=LInt.Roll/Ltime;
						   LInt2.Roll=LInt2.Roll/Ltime;

						   //����ֵ
						   err_Int.Nick=LInt2.Nick-LInt.Nick;
						   err_Int.Roll=LInt2.Roll-LInt.Roll;

						   //���»��֣������´μ���
						   Int2.Nick-=err_Int.Nick;
						   Int2.Roll-=err_Int.Roll;


						   //���²��־��Ƕ�LageKorrekturNick�������Ͷ������ǳ�ֵ��������
						   #define move_limit  10000   //���10��
						   #define err_limit	Ltime*4
						   #define err_limit2	Ltime*16
						   #define err_num_limit		2	//�����������󣬲ſ����޸�	

						   //nick
						   cnt=1;
						   if(labs(LInt_old.Nick-LInt.Nick)<move_limit)
						   {  UART1_Put_Chars("err_Int.Nick:");
						   		UART1_Put_int(err_Int.Nick);
						      
						   	  	if(err_Int.Nick>err_limit2)
								{
									  if(err_numP.Nick>(err_num_limit-1))
									  {		 									  		
									  		//�����������ϴ�ʱ����������Ư�ƽ��в���											
            								cnt += labs(err_Int.Nick) / err_limit2; //���Խ������Խ��
            								t = err_Int.Nick / 8;
            								if(t > 5000) t = 5000;
            								Lcom.Nick += t / Ltime;  //����֤
											
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
									  		//�����������ϴ�ʱ����������Ư�ƽ��в���											
            								cnt += labs(err_Int.Nick) / err_limit2; //���Խ������Խ��
            								t = err_Int.Nick / 8;
            								if(t < -5000) t = -5000;
            								Lcom.Nick += t / Ltime;  //����֤
											
									  }
									  else
									  {
									  		if(err_numN.Nick<err_num_limit)err_numN.Nick++;
									  }
								
								}

						   
						   }
						   else cnt=0;

						   

						   if(cnt>4) cnt=4;//�޷�

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
									  		//�����������ϴ�ʱ����������Ư�ƽ��в���											
            								cnt += labs(err_Int.Roll) / err_limit2; //���Խ������Խ��
            								t = err_Int.Roll / 8;
            								if(t > 5000) t = 5000;
            								Lcom.Roll += t / Ltime;  //����֤
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
									  		//�����������ϴ�ʱ����������Ư�ƽ��в���											
            								cnt += labs(err_Int.Roll) / err_limit2; //���Խ������Խ��
            								t = err_Int.Roll / 8;
            								if(t < -5000) t = -5000;
            								Lcom.Roll += t / Ltime;  //����֤
									  }
									  else
									  {
									  		if(err_numN.Roll<err_num_limit)err_numN.Roll++;
									  }
								
								}
						   }
						   else cnt=0;

						   

						   if(cnt>4) cnt=4;//�޷�

						   //if(err_Int.Roll >  err_limit)   AccX_mid += cnt;
        				   //if(err_Int.Roll < -err_limit)   AccX_mid -= cnt;
																						
						   	

							memcpy(&LInt_old,&LInt,sizeof(LInt_old));//�洢��һ�ε�ƽ��ֵ
						}  	 */



/*
				//center_P	 �����ȶ�P
				{

				   	
					CInt.Nick+= S_6050.AngNick;
					CInt.Roll+= S_6050.AngRoll;

					Cdif.Nick= S_6050.AngNick - C_6050.AngNick;//Cdif.Nick;
					Cdif.Roll= S_6050.AngRoll - C_6050.AngRoll;

					memcpy(&C_6050,&S_6050,sizeof(S_6050));	  //�汾��ֵ

					//UART1_Put_Chars("CInt:");
					UART1_Put_int(CInt.Nick);
					UART1_Put_int(S_6050.AngNick);
					UART1_Put_int(Cdif.Nick);
					USART_newline;	//����	// 
					

				  	M_f_tem=M_f_tem + S_6050.AngNick*PID.Center_P + CInt.Nick*PID.Center_I + Cdif.Nick*PID.Center_D;
					M_b_tem=M_b_tem - S_6050.AngNick*PID.Center_P - CInt.Nick*PID.Center_I - Cdif.Nick*PID.Center_D;
					M_l_tem=M_l_tem - S_6050.AngRoll*PID.Center_P - CInt.Roll*PID.Center_I - Cdif.Roll*PID.Center_D;
					M_r_tem=M_r_tem + S_6050.AngRoll*PID.Center_P + CInt.Roll*PID.Center_I + Cdif.Roll*PID.Center_D; 
				} 	 */
			    	 

//------------------End of File----------------------------
