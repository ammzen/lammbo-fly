

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HMC5883_H
#define __HMC5883_H

#define HMC58X3_ADDR 0x3C // 7 bit address of the HMC58X3 used with the Wire library
#define HMC_POS_BIAS 1
#define HMC_NEG_BIAS 2
// HMC58X3 register map. For details see HMC58X3 datasheet
#define HMC58X3_R_CONFA 0
#define HMC58X3_R_CONFB 1
#define HMC58X3_R_MODE 2
#define HMC58X3_R_XM 3
#define HMC58X3_R_XL 4

#define HMC58X3_R_YM (7)  //!< Register address for YM.
#define HMC58X3_R_YL (8)  //!< Register address for YL.
#define HMC58X3_R_ZM (5)  //!< Register address for ZM.
#define HMC58X3_R_ZL (6)  //!< Register address for ZL.

#define HMC58X3_X_SELF_TEST_GAUSS (+1.16)                       //!< X axis level when bias current is applied.
#define HMC58X3_Y_SELF_TEST_GAUSS (HMC58X3_X_SELF_TEST_GAUSS)   //!< Y axis level when bias current is applied.
#define HMC58X3_Z_SELF_TEST_GAUSS (+1.08)                       //!< Y axis level when bias current is applied.

#define SELF_TEST_LOW_LIMIT  (243.0/390.0)   //!< Low limit when gain is 5.
#define SELF_TEST_HIGH_LIMIT (575.0/390.0)   //!< High limit when gain is 5.

#define HMC58X3_R_STATUS 9
#define HMC58X3_R_IDA 10
#define HMC58X3_R_IDB 11
#define HMC58X3_R_IDC 12

/***************/
//��ӡ����ֵ


/***************/
#define hmc5883_intport GPIOA

#define hmc5883_intpin 12
#define HMC5883_IS_newdata  (GPIO_ReadInputDataBit(hmc5883_intport,(1<<hmc5883_intpin)) )



extern u8 HMC5883_newdata;//������OK =1��
extern u8 HMC5883_new_datain; //1:����ң�����ݽ���HMC5883
extern s16  HMC5883_maxx,HMC5883_maxy,HMC5883_maxz,HMC5883_minx,HMC5883_miny,HMC5883_minz;
extern u8 HMC5883_check_num;	//HMC5883ÿ50MS���һ�Σ�����TIM4
extern u8 HMC5883_err_mark;	//HMC5883�����־
extern u16 HMC5883_data;
extern double    y_scale;//Y�����

void HMC58X3_writeReg(unsigned char reg, unsigned char val);
void HMC58X3_setMode(unsigned char mode);
void HMC58X3_init(u8 setmode);
void HMC58X3_getID(void);
void HMC5883L_SetUp(void);
void HMC58X3_getRaw(void);
void HMC58X3_io_init(void);
void HMC5883_Circling(void);//�ⲿתȦ����������Сֵ
void HMC5883_setwork(void);//��������״̬
void HMC5883_ready(void);	   //����ʱУ��

//�ڶ������в�ͣ��HMC5883_check_num��0��
//�˺���ÿ200MS����һ�Σ�������HMC5883_check_num,����20��20*200=4S��������������HMC5883  
void  HMC5883_check_work(void);
//��������������E2ROM����  
void Key_change_e2rom(void);

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
