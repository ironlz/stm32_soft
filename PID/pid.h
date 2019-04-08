#ifndef _pid_
#define _pid_
#include "stm32f10x_conf.h"

typedef struct
{
 float Sv;//�û��趨ֵ
 float Pv;
  float Sv1;//�û��趨ֵ
 float Pv1;
	
 float Kp;
 float T;  //PID��������--��������
 float Ti;
 float Td; 
	
	 float Kp1;
 float T1;  //PID��������--��������
 float Ti1;
 float Td1; 
	
	
	
 float Ek;  //����ƫ��
 float Ek_1;//�ϴ�ƫ��
 float SEk; //��ʷƫ��֮��
	
	 float Ek1;  //����ƫ��
 float Ek_11;//�ϴ�ƫ��
 float SEk1; //��ʷƫ��֮��
 
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
	
 u16 pwmcycle;//pwm����
  u16 pwmcycle1;//pwm����

 
 u32 times;
 u32 times1;
	
}PID;

extern PID pid; //���PID�㷨����Ҫ������
extern PID pid1; //���PID�㷨����Ҫ������

void PID_Calc(void); //pid����
void PID_Calc1(void);

void PID_Calc_overshoot(void);  //pid����-���崦��
void PID_Calc_overshoot1(void);
#endif
