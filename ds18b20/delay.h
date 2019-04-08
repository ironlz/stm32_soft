#ifndef __DELAY_H
#define __DELAY_H 			   
#include <stm32f10x.h>

//使用SysTick的普通计数模式对延迟进行管理
//包括delay_us,delay_ms
extern void delay_init(u8 SYSCLK);
extern void delay_ms(u16 nms);
extern void delay_us(u32 nus);

#endif





























