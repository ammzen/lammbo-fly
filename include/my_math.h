



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MY_MATH_H
#define __MY_MATH_H

#define  M_PI   3.1415926
#define  DegToRad   M_PI/180


float angle_sin(u16 angle);//�Ƕȵ�SIN
float angle_cos(u16 angle);//�Ƕȵ�COS
//����ָ����
void test_hmc5883(u16 data);

/************
   ����360-0֮��Ƕ����� 
************/
s16 angle_360_math(s16 Target_angles);
//���Ƕ����
s16 angle_dec_math(u16 Target_angle,u16 HMC5883_data);






#endif 


