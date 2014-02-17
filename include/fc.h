
#ifndef __FC_H
#define __FC_H
#include "main.h"


#define HMC5883_open   	1  //	1,��

#define moto_ON			1  //	1,��
//#define moto_PPM_OUT	1  //	δ����,PWM���	����,PPM�������




extern u8 PPM_open; //	1,��
//extern u8 moto_ON;  //�������	1,hand-set������0
extern u8 hand_set_in;// 1,hand_set���� 
extern u16 bat;	//���
extern u8 dub_show_en;	 //����DUG_SHOW�����ֵ	  100ms��������ʱ��
extern u8 ms_2_en;	 //500HZ �ź�  ������ʱ���sys_tim_isr(void)��	//500hz 2ms
extern struct mpu6050_info S_6050;//mpu6050�����ֵ��
extern struct mpu6050_info S1_6050;//mpu6050��һ��ֵ��
extern u8 RF_ctrl_num;//��������ʱ�����Ϊ10MS����һ��PPM_MAIN_WORK
extern u8 wifi_RFok_num;//��ȷ����WIFI�źŴ�����	������ʱ�ÿ200MS��1��Ϊ0ʱ��ʧ��



#define star_Gas  50  //��������



/****
     strcpy(Pnt_num[0].URL,"ii");
     memcpy(mystructA,mystructB,sizeof(mystructB));
	 memcmp(&a1, &a2, sizeof A);
	 memset(&a1, 0, sizeof A); 
*****/



/********RNN-����************/
struct RUN_info
{ 
  u8  status;	 //����״̬��0���ź�  1������	200.ͣ����
  //u8  bat;		//BAT״̬��	 1����3.7V   0����3.7V

  //����˴ε�������ֵ
  s16  AccRoll;	//�Ǽ��ٶ�
  s16  AccNick;
  s16  AccGier;
};
extern struct RUN_info RUN;//RNN-������


/********Int_info************/
struct Int_info
{ 
  float  Nick;
  float  Gier;
  float  Roll;
  u16  num;
};
extern struct Int_info Int;//�����ֲ�����

/********PID_info************/
struct PID_info
{ 
  float pGAS_P; //2			//ң�����ű���
  float pNick_P;//2//10		//ң����������
  float pRoll_P;//2//10		//ң�غ������
  float pGier_P;//0.5//5	//ң����������

  float ACC_P ;//7//50//40 //50//35  //90
  float Gier_P;//7//50//40 //50//35  //90

  float Nick_I;//0.0001//0.002// 0.012		//ң���������� 0.02	0.002
  //float Roll_I;//0.0001//0.002// 0.012		//ң�غ������
  float Gier_I;//0.0005//0.01//0.02// 0.15		//ң����������

  //����
  float Center_P;//
  float Center_I;//
  float Center_D;//

};
extern struct PID_info PID;//PID������

#define pid_id   10326		//12306
/********PID�������������************/
struct PID_out
{ 
  u16 pGAS_P; //2			//ң�����ű���
  u16 pNick_P;//2//10		//ң����������
  u16 pRoll_P;//2//10		//ң�غ������
  u16 pGier_P;//0.5//5	//ң����������

  u16 ACC_P ;//7//50//40 //50//35  //90
  u16 Gier_P;//7//50//40 //50//35  //90

  u16 Nick_I;//0.0001//0.002// 0.012		//ң���������� 0.02	0.002
  //u16 Roll_I;//0.0001//0.002// 0.012		//ң�غ������
  u16 Gier_I;//0.0005//0.01//0.02// 0.15		//ң����������

  //����
  u16 Center_P;//
  u16 Center_I;//
  u16 Center_D;//

  u16 ID;//�ų���һ�ο���
};
extern struct PID_out PID_out;	 //PID�������������  

/********DUG_SHOW�����ֵ************/
struct dub_show_info
{ 
  

  s16  AngNick;	//�Ƕ�
  //s8  AccNick;
  //s16 IntNick;

  
  s16  AngRoll;
  //s8  AccRoll;	//�Ǽ��ٶ�
  //s32 IntRoll;

  /*
  s8  AccGier;
  s32 IntGier; */	

  u16 bat;	
  u16 Height;  

  //nmea_msg nmea_msg;
  //u16 check;
       
};
extern struct dub_show_info dub_show;	 //DUG_SHOW�����ֵ

/********PPM_info************/
struct PPM_info
{ 
  u8  Gas;
  s8  Gier;
  s8  Nick;
  s8  Roll; 
  
  u8  num; 
};
extern struct PPM_info W_PPM;//WIFI_ppm������




extern volatile u16 Motor_front,Motor_back,Motor_left,Motor_right;//ת��
extern volatile u8 sys_star;
extern u8 Main_ERR;	  //�������ʶ	//1,����ʱ����δ��0
extern u8 sys_star_ok;//1:ϵͳ�������
void PID_data_out(void);	  //����趨ֵ��PID
void PID_ready(void);
void hand_Reply(void);//�ظ���λ��

void fly_main(void);
void Ready_work(void);


#endif


