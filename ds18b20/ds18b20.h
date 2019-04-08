#ifndef __DS18B20_H
#define __DS18B20_H
#include "sys.h"


#define DS18B20_IO_IN()  {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=0X00800000;}
#define DS18B20_IO_OUT() {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=0X00300000;}

#define DS18B20_IO_IN1() {GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=0X00080000;}
#define DS18B20_IO_OUT1(){GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=0X00030000;}

////IO��������

#define	DS18B20_DQ_OUT PCout(5) //���ݶ˿�	PA8  
#define	DS18B20_DQ_IN  PCin(5)  //���ݶ˿�	PA8

#define	DS18B20_DQ_OUT1 PAout(4) //���ݶ˿�	PA4 
#define	DS18B20_DQ_IN1  PAin(4)  //���ݶ˿�	PA4

u8 DS18B20_Init(void);//��ʼ��DS18B20
short DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20
void display_template_event(void);


u8 DS18B20_Init1(void);//��ʼ��DS18B20
short DS18B20_Get_Temp1(void);//��ȡ�¶�
void DS18B20_Start1(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte1(u8 dat1);//д��һ���ֽ�
u8 DS18B20_Read_Byte1(void);//����һ���ֽ�
u8 DS18B20_Read_Bit1(void);//����һ��λ
u8 DS18B20_Check1(void);//����Ƿ����DS18B20
void DS18B20_Rs1t(void);//��λDS18B20
void display_template_event1(void);
#endif















