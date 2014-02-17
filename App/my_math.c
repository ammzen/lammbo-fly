





/* Includes ------------------------------------------------------------------*/
#include "main.h"

/***************
&:按位与
&&:与



定义函数：double pow(double x, double y); 
函数说明：pow()用来计算以x 为底的y 次方值, 即xy 值, 然后将结果返回.


********************/

/************
   消除360-0之间角度问题 
************/
s16 angle_360_math(s16 Target_angles)
{  
   if(Target_angles<0)Target_angles+=360;
   if(Target_angles>360)Target_angles-=360;

   return Target_angles;
}

//二角度相减
s16 angle_dec_math(u16 Target_angle,u16 HMC5883_data)
{
	 s16	angle_tem=0;


	 angle_tem=Target_angle-HMC5883_data;
	  	
			 //如果转动角度大于180，那么应该从另一边转会更快一点
	   		if(abs(angle_tem)>180)
	   		{
	   	  		if(Target_angle>HMC5883_data)                                  
		  		{
		  	  		angle_tem-=360;
		  		}
		  		else
		  		{
		  	  		angle_tem+=360;		  
		  		}	   	  
	   		}

	  return   angle_tem;
}


