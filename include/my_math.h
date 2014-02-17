



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MY_MATH_H
#define __MY_MATH_H

#define  M_PI   3.1415926
#define  DegToRad   M_PI/180


float angle_sin(u16 angle);//角度的SIN
float angle_cos(u16 angle);//角度的COS
//测试指南针
void test_hmc5883(u16 data);

/************
   消除360-0之间角度问题 
************/
s16 angle_360_math(s16 Target_angles);
//二角度相减
s16 angle_dec_math(u16 Target_angle,u16 HMC5883_data);






#endif 


