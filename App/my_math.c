





/* Includes ------------------------------------------------------------------*/
#include "main.h"

/***************
&:��λ��
&&:��



���庯����double pow(double x, double y); 
����˵����pow()����������x Ϊ�׵�y �η�ֵ, ��xy ֵ, Ȼ�󽫽������.


********************/

/************
   ����360-0֮��Ƕ����� 
************/
s16 angle_360_math(s16 Target_angles)
{  
   if(Target_angles<0)Target_angles+=360;
   if(Target_angles>360)Target_angles-=360;

   return Target_angles;
}

//���Ƕ����
s16 angle_dec_math(u16 Target_angle,u16 HMC5883_data)
{
	 s16	angle_tem=0;


	 angle_tem=Target_angle-HMC5883_data;
	  	
			 //���ת���Ƕȴ���180����ôӦ�ô���һ��ת�����һ��
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


