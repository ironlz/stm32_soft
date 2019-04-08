#ifndef __DS18B20_H
#define __DS18B20_H
#include "sys.h"


#define DS18B20_IO_IN()  {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=0X00800000;}
#define DS18B20_IO_OUT() {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=0X00300000;}

#define DS18B20_IO_IN1() {GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=0X00080000;}
#define DS18B20_IO_OUT1(){GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=0X00030000;}

////IO操作函数

#define	DS18B20_DQ_OUT PCout(5) //数据端口	PA8  
#define	DS18B20_DQ_IN  PCin(5)  //数据端口	PA8

#define	DS18B20_DQ_OUT1 PAout(4) //数据端口	PA4 
#define	DS18B20_DQ_IN1  PAin(4)  //数据端口	PA4

u8 DS18B20_Init(void);//初始化DS18B20
short DS18B20_Get_Temp(void);//获取温度
void DS18B20_Start(void);//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);//读出一个字节
u8 DS18B20_Read_Bit(void);//读出一个位
u8 DS18B20_Check(void);//检测是否存在DS18B20
void DS18B20_Rst(void);//复位DS18B20
void display_template_event(void);


u8 DS18B20_Init1(void);//初始化DS18B20
short DS18B20_Get_Temp1(void);//获取温度
void DS18B20_Start1(void);//开始温度转换
void DS18B20_Write_Byte1(u8 dat1);//写入一个字节
u8 DS18B20_Read_Byte1(void);//读出一个字节
u8 DS18B20_Read_Bit1(void);//读出一个位
u8 DS18B20_Check1(void);//检测是否存在DS18B20
void DS18B20_Rs1t(void);//复位DS18B20
void display_template_event1(void);
#endif















