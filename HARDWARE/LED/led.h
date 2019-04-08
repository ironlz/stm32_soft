#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#define LED0 PCout(0)
#define LED1 PCout(1)
#define GPRS_Pwk PAout(4)
#define GPRS_Pwk_IO             GPIOB   //PB0
#define GPRS_Pwk_B      	GPIO_Pin_0
void LED_Init(void);//初始化
void GPRS_Pwk_Init(void);
void PWM_Init(void);	//PWM初始化	 				    
#endif
