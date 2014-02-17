
#ifndef __FC_H
#define __FC_H
#include "main.h"


#define HMC5883_open   	1  //	1,打开

#define moto_ON			1  //	1,打开
//#define moto_PPM_OUT	1  //	未定义,PWM输出	定义,PPM脉宽输出




extern u8 PPM_open; //	1,打开
//extern u8 moto_ON;  //电机控制	1,hand-set上来就0
extern u8 hand_set_in;// 1,hand_set插入 
extern u16 bat;	//电池
extern u8 dub_show_en;	 //允许DUG_SHOW的输出值	  100ms放在主计时里
extern u8 ms_2_en;	 //500HZ 信号  在主延时检测sys_tim_isr(void)中	//500hz 2ms
extern struct mpu6050_info S_6050;//mpu6050的输出值。
extern struct mpu6050_info S1_6050;//mpu6050上一次值。
extern u8 RF_ctrl_num;//放在主延时里，作用为10MS运行一次PPM_MAIN_WORK
extern u8 wifi_RFok_num;//正确接收WIFI信号次数，	在主延时里，每200MS减1，为0时则失控



#define star_Gas  50  //启动油门



/****
     strcpy(Pnt_num[0].URL,"ii");
     memcpy(mystructA,mystructB,sizeof(mystructB));
	 memcmp(&a1, &a2, sizeof A);
	 memset(&a1, 0, sizeof A); 
*****/



/********RNN-参数************/
struct RUN_info
{ 
  u8  status;	 //运行状态，0无信号  1，启动	200.停机，
  //u8  bat;		//BAT状态，	 1大于3.7V   0低于3.7V

  //记忆此次的陀螺仪值
  s16  AccRoll;	//角加速度
  s16  AccNick;
  s16  AccGier;
};
extern struct RUN_info RUN;//RNN-参数。


/********Int_info************/
struct Int_info
{ 
  float  Nick;
  float  Gier;
  float  Roll;
  u16  num;
};
extern struct Int_info Int;//主积分参数。

/********PID_info************/
struct PID_info
{ 
  float pGAS_P; //2			//遥控油门倍数
  float pNick_P;//2//10		//遥控仰俯倍数
  float pRoll_P;//2//10		//遥控横滚倍数
  float pGier_P;//0.5//5	//遥控自旋倍数

  float ACC_P ;//7//50//40 //50//35  //90
  float Gier_P;//7//50//40 //50//35  //90

  float Nick_I;//0.0001//0.002// 0.012		//遥控仰俯倍数 0.02	0.002
  //float Roll_I;//0.0001//0.002// 0.012		//遥控横滚倍数
  float Gier_I;//0.0005//0.01//0.02// 0.15		//遥控自旋倍数

  //自稳
  float Center_P;//
  float Center_I;//
  float Center_D;//

};
extern struct PID_info PID;//PID参数。

#define pid_id   10326		//12306
/********PID输出到串口数据************/
struct PID_out
{ 
  u16 pGAS_P; //2			//遥控油门倍数
  u16 pNick_P;//2//10		//遥控仰俯倍数
  u16 pRoll_P;//2//10		//遥控横滚倍数
  u16 pGier_P;//0.5//5	//遥控自旋倍数

  u16 ACC_P ;//7//50//40 //50//35  //90
  u16 Gier_P;//7//50//40 //50//35  //90

  u16 Nick_I;//0.0001//0.002// 0.012		//遥控仰俯倍数 0.02	0.002
  //u16 Roll_I;//0.0001//0.002// 0.012		//遥控横滚倍数
  u16 Gier_I;//0.0005//0.01//0.02// 0.15		//遥控自旋倍数

  //自稳
  u16 Center_P;//
  u16 Center_I;//
  u16 Center_D;//

  u16 ID;//排除第一次开机
};
extern struct PID_out PID_out;	 //PID输出到串口数据  

/********DUG_SHOW的输出值************/
struct dub_show_info
{ 
  

  s16  AngNick;	//角度
  //s8  AccNick;
  //s16 IntNick;

  
  s16  AngRoll;
  //s8  AccRoll;	//角加速度
  //s32 IntRoll;

  /*
  s8  AccGier;
  s32 IntGier; */	

  u16 bat;	
  u16 Height;  

  //nmea_msg nmea_msg;
  //u16 check;
       
};
extern struct dub_show_info dub_show;	 //DUG_SHOW的输出值

/********PPM_info************/
struct PPM_info
{ 
  u8  Gas;
  s8  Gier;
  s8  Nick;
  s8  Roll; 
  
  u8  num; 
};
extern struct PPM_info W_PPM;//WIFI_ppm参数。




extern volatile u16 Motor_front,Motor_back,Motor_left,Motor_right;//转速
extern volatile u8 sys_star;
extern u8 Main_ERR;	  //主错误标识	//1,开机时油门未归0
extern u8 sys_star_ok;//1:系统启动完成
void PID_data_out(void);	  //输出设定值到PID
void PID_ready(void);
void hand_Reply(void);//回复上位机

void fly_main(void);
void Ready_work(void);


#endif


