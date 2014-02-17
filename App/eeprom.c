
#include "main.h"


#define   max_page	 64
#define   page_data	 1024
#define   common_data_page	(max_page-1)
#define   PAGE_ADDR   		(0x08000000 + common_data_page * page_data)	 //普通数据存在最后一页
//普通数据
#define  e2_rom_w_mark   51		//是否是第一次开机
#define  e2_num     (1+6+6)		//当前使用E2ROM HalfWord数量//写入总数

//struct_pid 总大小不能大于2048
#define   PID_PAGE_ADDR   	(0x08000000 + (common_data_page-1) * page_data) 


void test_e2rom_write_read_path(void)
{
	 /*u16 a,b;
	
	UART1_Put_Chars("path info");

	 USART_newline;	//换行
	  */
}

void e2rom_StrPID_read(void)
{
	u16 tem_buf[1024];

	//UART1_Put_Chars("StrPID_read");
	//UART1_Put_int(sizeof(PID_out));

	MemReadByte(tem_buf,sizeof(PID_out)/2,PID_PAGE_ADDR); //写入时为二字节，sizeof是单字节
	memcpy(&PID_out,(&tem_buf),sizeof(PID_out));

}
void e2rom_StrPID_write(void)
{
	u16 tem_buf[1024];
	
	
	//UART1_Put_Chars("StrPID_write");
	//UART1_Put_int(sizeof(PID_out));
	 
	memcpy(&tem_buf,(&PID_out),sizeof(PID_out));
	
	MemWriteByte(tem_buf,sizeof(PID_out)/2,PID_PAGE_ADDR);	//写入时为二字节，sizeof是单字节
	
}



void e2rom_data_write(void)
{
	u16 e2prom_tem[e2_num]={0x1033,};

    e2prom_tem[HMC_maxx]=HMC5883_maxx; 
	e2prom_tem[HMC_maxy]=HMC5883_maxy; 
	e2prom_tem[HMC_maxz]=HMC5883_maxz;
	e2prom_tem[HMC_minx]=HMC5883_minx; 
	e2prom_tem[HMC_miny]=HMC5883_miny; 
	e2prom_tem[HMC_minz]=HMC5883_minz; 

	e2prom_tem[AngX_midZ]=AngX_mid;
	e2prom_tem[AngY_midZ]=AngY_mid;
	e2prom_tem[AngZ_midZ]=AngZ_mid;//角度中立值
	e2prom_tem[AccX_midZ]=AccX_mid;
	e2prom_tem[AccY_midZ]=AccY_mid;
	e2prom_tem[AccZ_midZ]=AccZ_mid;//角速度中立值
	
	e2prom_tem[e2rom_use]=e2_rom_w_mark ;

	MemWriteByte(e2prom_tem,e2_num,PAGE_ADDR);
	//MemWriteByte(e2prom_tem,u16 num); 
}

void e2rom_mpu6050_read(void)
{
	u16 e2prom_tem[e2_num]={0x1033,};

   MemReadByte(e2prom_tem,e2_num,PAGE_ADDR);

   	AngX_mid=e2prom_tem[AngX_midZ];
	AngY_mid=e2prom_tem[AngY_midZ];
	AngZ_mid=e2prom_tem[AngZ_midZ];//角度中立值
	AccX_mid=e2prom_tem[AccX_midZ];
	AccY_mid=e2prom_tem[AccY_midZ];
	AccZ_mid=e2prom_tem[AccZ_midZ];//角速度中立值
}
void e2rom_data_read(void)
{
	u16 e2prom_tem[e2_num]={0x1033,};

   MemReadByte(e2prom_tem,e2_num,PAGE_ADDR);   	

  if(e2prom_tem[e2rom_use]==e2_rom_w_mark)
  {	
	HMC5883_maxx=e2prom_tem[HMC_maxx]; 
	HMC5883_maxy=e2prom_tem[HMC_maxy]; 
	HMC5883_maxz=e2prom_tem[HMC_maxz]; 
	HMC5883_minx=e2prom_tem[HMC_minx]; 
	HMC5883_miny=e2prom_tem[HMC_miny]; 
	HMC5883_minz=e2prom_tem[HMC_minz];



	y_scale=((double)HMC5883_maxx-(double)HMC5883_minx)/((double)HMC5883_maxy-(double)HMC5883_miny);//Y轴比率
  }
  else 	//第一次开机
  {
  	
	if(HMC5883_open)HMC5883_Circling();	 //外部转圈以求得最大最小值
	
	e2rom_data_write();
  
	
  }
	
}
/***************************************************************************************************
* 函数名称: MemReadByte()
* 函数功能: 从存储器中读出num字节数据
* 入口参数: *dat:读出数据的保存地址
*                        num        :读出的字节数
* 出口参数: 0：操作失败；1：操作成功
* 使用说明: 无
* 创建日期: 2010年12月15日
***************************************************************************************************/
u8 MemReadByte(u16 *data,u16 num,u32 page_addr)                                
{
        u16 *temp_addr = (u16 *)page_addr;
        
		
        while(num --)
        {
                *data ++ = *temp_addr ++;
        }
                                                                                                        
        return 1;                                                                                                        
}
/***************************************************************************************************
* 函数名称: MemWriteByte()
* 函数功能: 向存储器中写入num字节数据
* 入口参数: *dat:待写入数据
*                        num        :写入的字节数
* 出口参数: 0：操作失败；1：操作成功
* 使用说明: 无
* 创建日期: 2010年12月15日
***************************************************************************************************/
u8 MemWriteByte(u16 *data,u16 num,u32 temp_addr)                          
{
        FLASH_Status temp_stat;
       // u32 temp_addr = PAGE_ADDR;
        
		
	//	MemReadByte(e2prom_tem,num);
		 
		
		       
        FLASH_Unlock();  		     // Flash解锁，允许操作相关的寄存器

        temp_stat = FLASH_ErasePage(temp_addr); // 擦出制定的页
        

        if(temp_stat != FLASH_COMPLETE)
        {
                FLASH_Lock();
                return 0;
        } /**/
        
        while(num --)
        {
                temp_stat = FLASH_ProgramHalfWord(temp_addr,*data);
                if(temp_stat != FLASH_COMPLETE)
                {
                        FLASH_Lock();
                        return 0;
                }
        
                temp_addr += 2;
                data++;
        }
        
        FLASH_Lock();        
        return 1;
}

/*
//开机常按，重置E2ROM数据  
void Key_change_e2rom(void)	   //
{
  if(!(GPIO_ReadInputDataBit(key_port,(1<<(key_pin))))) //开机常按，重置E2ROM数据  
  {
	 LED4_Turn;  LED3_Turn; LED2_Turn; 

	 UART1_Put_Chars("开机常按，重置E2ROM数据  !/");
	 while(!(GPIO_ReadInputDataBit(key_port,(1<<(key_pin))))) ; 

     if(HMC5883_open)HMC5883_Circling();	 //外部转圈以求得最大最小值
	
	 e2rom_data_write();
	 
	 LED4_OFF; LED3_OFF;LED2_OFF;
  }
} */

//开机常按，重置E2ROM数据  
void Key_change_e2rom(void)	   //
{
  if(!(GPIO_ReadInputDataBit(key_port,(1<<(key_pin))))) //开机常按，重置E2ROM数据  
  {
	 LED4_Turn;  LED3_Turn; LED2_Turn; 

	 UART1_Put_Chars("开机常按，重置E2ROM-HMC5883数据  !/");
	 while(!(GPIO_ReadInputDataBit(key_port,(1<<(key_pin))))) ; 

     if(HMC5883_open)HMC5883_Circling();	 //外部转圈以求得最大最小值
	
	 e2rom_data_write();



	 UART1_Put_Chars("开机常按，重置E2ROM-mpu6050数据  !/");

	 S_6050_set();//mpu6050启动时强制校正

	 
	 LED4_OFF;	 LED3_OFF;LED2_OFF;
  }
}


