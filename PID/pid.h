#ifndef _pid_
#define _pid_
#include "stm32f10x_conf.h"

typedef struct
{
 float Sv;//用户设定值
 float Pv;
  float Sv1;//用户设定值
 float Pv1;
	
 float Kp;
 float T;  //PID计算周期--采样周期
 float Ti;
 float Td; 
	
	 float Kp1;
 float T1;  //PID计算周期--采样周期
 float Ti1;
 float Td1; 
	
	
	
 float Ek;  //本次偏差
 float Ek_1;//上次偏差
 float SEk; //历史偏差之和
	
	 float Ek1;  //本次偏差
 float Ek_11;//上次偏差
 float SEk1; //历史偏差之和
 
	float Iout;
	float Pout;
	float Dout;
	
		float Iout1;
	float Pout1;
	float Dout1;
	
 float OUT0;
  float OUT01;

 float OUT;
 float OUT1;

	
	
 u16 C10ms;
 u16 C10ms1;
	
 u16 pwmcycle;//pwm周期
  u16 pwmcycle1;//pwm周期

 
 u32 times;
 u32 times1;
	
}PID;

extern PID pid; //存放PID算法所需要的数据
extern PID pid1; //存放PID算法所需要的数据

void PID_Calc(void); //pid计算
void PID_Calc1(void);

void PID_Calc_overshoot(void);  //pid计算-过冲处理
void PID_Calc_overshoot1(void);
#endif
