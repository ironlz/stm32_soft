#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
 


#define KEY0 PCin(2)   	
#define KEY1 PCin(3)	 
#define WK_UP  PCin(4)	 
 

//#define KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)//��ȡ����0
//#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)//��ȡ����1
//#define WK_UP   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)//��ȡ����2 
 

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define WKUP_PRES	3		//WK_UP  

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8 mode);  	//����ɨ�躯��					    
#endif
