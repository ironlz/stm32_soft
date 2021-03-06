#include "pid.h"
#include "usart2.h" 
#include "usart3.h"	
PID pid; //存放PID算法所需要的数据
PID pid1; //存放PID算法所需要的数据


void PID_Calc()  //pid计算
{
 float DelEk;
	float ti,ki;
//	float Iout;
//	float Pout;
//	float Dout;
	float td;
	float kd;
	float out;
 printf("pid.Ek=%f\r\n",pid.Ek);	
 if(pid.C10ms<(pid.T))  //计算周期未到
 {
    return ;
 }

 pid.Ek=pid.Sv-pid.Pv;   //得到当前的偏差值

 pid.Pout=pid.Kp*pid.Ek;      //比例输出
 
 pid.SEk+=pid.Ek;        //历史偏差总和
 
 DelEk=pid.Ek-pid.Ek_1;  //最近两次偏差之差
 
 ti=pid.T/pid.Ti;
 ki=ti*pid.Kp;
 
  pid.Iout=ki*pid.SEk*pid.Kp;  //积分输出

 td=pid.Td/pid.T;
 
 kd=pid.Kp*td;
 
 pid.Dout=kd*DelEk;    //微分输出
 
 out= pid.Pout+ pid.Iout+ pid.Dout;
 
 //////////////////////////////////////////////////////////
 
 if(out>pid.pwmcycle)
 {
  pid.OUT=pid.pwmcycle;
 }
 else if(out<0)
 {
  pid.OUT=pid.OUT0; 
 }
 else 
 {
  pid.OUT=out;
 }
 //pid.OUT+=; //本次的计算结果
 pid.Ek_1=pid.Ek;  //更新偏差
 
 pid.C10ms=0;
}


void PID_Calc_overshoot()  //pid计算-过冲处理�
{
 float DelEk;
	float ti,ki;
//	float Iout;
//	float Pout;
//	float Dout;
	float td;
	float kd;
	float out;
  float SvSave;
 if(pid.C10ms<(pid.T))  //计算周期未到
 {
    return ;
 }
 
 
//判断Sv
  SvSave=pid.Sv;
 
 if(pid.Pv<(SvSave*0.55))//温度达到计数开始 66度
 {
    pid.times=0;
 }
 
  if(pid.times<=60)//3分钟 72度  100s
 {
  pid.Sv=SvSave*0.6;
 }
 
 if((pid.times>60)&&(pid.times<=100))//3分钟 96度  100s
 {
  pid.Sv=SvSave*0.8;
 }
 
 if((pid.times>100)&&(pid.times<=200))//2分钟 102度 100s
 {
  pid.Sv=SvSave*0.85;
 }
 
 if((pid.times>200)&&(pid.times<=300))//2分钟 108度 100s
 {
  pid.Sv=SvSave*0.9;
 }
 
  if((pid.times>300)&&(pid.times<=400))//2分钟 114度 100s
 {
  pid.Sv=SvSave*0.95;
 }
 
 if(pid.times>400)
 {
  pid.Sv=SvSave;
 }
//////////////////////////////////


 pid.Ek=pid.Sv-pid.Pv;   //得到当前的偏差值
 pid.Pout=pid.Kp*pid.Ek;      //比例输出
 
 pid.SEk+=pid.Ek;        //历史偏差总和
 
 DelEk=pid.Ek-pid.Ek_1;  //最近两次偏差之差
 
 ti=pid.T/pid.Ti;
 ki=ti*pid.Kp;
 
  pid.Iout=ki*pid.SEk*pid.Kp;  //积分输出

 td=pid.Td/pid.T;
 
 kd=pid.Kp*td;
 
  pid.Dout=kd*DelEk;    //微分输出
 
 out= pid.Pout+ pid.Iout+ pid.Dout;
 
 //////////////////////////////////////////////////////////
 
 if(out>pid.pwmcycle)
 {
  pid.OUT=pid.pwmcycle;
 }
 else if(out<0)
 {
  pid.OUT=pid.OUT0; 
 }
 else 
 {
  pid.OUT=out;
 }
 //pid.OUT+=; //本次的计算结果
 pid.Ek_1=pid.Ek;  //更新偏差
 
 pid.C10ms=0;
 pid.Sv=SvSave;
}










/*********************/

void PID_Calc1()  //pid计算
{
 float DelEk1;
	float ti1,ki1;
//	float Iout;
//	float Pout;
//	float Dout;
	float td1;
	float kd1;
	float out1;
 printf("pid.Ek1=%f\r\n",pid.Ek1);	
 if(pid.C10ms1 <(pid.T1))  //计算周期未到
 {
    return ;
 }

 pid.Ek1=pid.Sv1-pid.Pv1;   //得到当前的偏差值

 pid.Pout1=pid.Kp1*pid.Ek1;      //比例输出
 
 pid.SEk1+=pid.Ek1;        //历史偏差总和
 
 DelEk1=pid.Ek1-pid.Ek_11;  //最近两次偏差之差
 
 ti1=pid.T1/pid.Ti1;
 ki1=ti1*pid.Kp1;
 
  pid.Iout1=ki1*pid.SEk1*pid.Kp1;  //积分输出

 td1=pid.Td1/pid.T1;
 
 kd1=pid.Kp1*td1;
 
 pid.Dout1=kd1*DelEk1;    //微分输出
 
 out1= pid.Pout1+ pid.Iout1+ pid.Dout1;
 
 //////////////////////////////////////////////////////////
 
 if(out1>pid.pwmcycle1)
 {
  pid.OUT1=pid.pwmcycle1;
 }
 else if(out1<0)
 {
  pid.OUT1=pid.OUT01; 
 }
 else 
 {
  pid.OUT1=out1;
 }
 //pid.OUT+=; //本次的计算结果
 pid.Ek_11=pid.Ek1;  //更新偏差
 
 pid.C10ms1=0;
}


void PID_Calc_overshoot1()  //pid计算-过冲处理�
{
 float DelEk1;
	float ti1,ki1;
//	float Iout;
//	float Pout;
//	float Dout;
	float td1;
	float kd1;
	float out1;
  float SvSave1;
 if(pid.C10ms1<(pid.T1))  //计算周期未到
 {
    return ;
 }
 
 
//判断Sv
  SvSave1=pid.Sv1;
 
 if(pid.Pv1<(SvSave1*0.55))//温度达到计数开始 66度
 {
    pid.times1=0;
 }
 
  if(pid.times1<=60)//3分钟 72度  100s
 {
  pid.Sv1=SvSave1*0.6;
 }
 
 if((pid.times1>60)&&(pid.times1<=100))//3分钟 96度  100s
 {
  pid.Sv1=SvSave1*0.8;
 }
 
 if((pid.times1>1100)&&(pid.times1<=200))//2分钟 102度 100s
 {
  pid.Sv1=SvSave1*0.85;
 }
 
 if((pid.times1>200)&&(pid.times1<=300))//2分钟 108度 100s
 {
  pid.Sv1=SvSave1*0.9;
 }
 
  if((pid.times1>300)&&(pid.times1<=400))//2分钟 114度 100s
 {
  pid.Sv1=SvSave1*0.95;
 }
 
 if(pid.times1>400)
 {
  pid.Sv1=SvSave1;
 }
//////////////////////////////////


 pid.Ek1=pid.Sv1-pid.Pv1;   //得到当前的偏差值
 pid.Pout1=pid.Kp1*pid.Ek1;      //比例输出
 
 pid.SEk1+=pid.Ek1;        //历史偏差总和
 
 DelEk1=pid.Ek1-pid.Ek_11;  //最近两次偏差之差
 
 ti1 =pid.T1/pid.Ti1;
 ki1 =ti1*pid.Kp1;
 
  pid.Iout1=ki1*pid.SEk1*pid.Kp1;  //积分输出

 td1=pid.Td1/pid.T1;
 
 kd1=pid.Kp1*td1;
 
  pid.Dout1=kd1*DelEk1;    //微分输出
 
 out1= pid.Pout1+ pid.Iout1+ pid.Dout1;
 
 //////////////////////////////////////////////////////////
 
 if(out1>pid.pwmcycle1)
 {
  pid.OUT1=pid.pwmcycle1;
 }
 else if(out1<0)
 {
  pid.OUT1=pid.OUT01; 
 }
 else 
 {
  pid.OUT1=out1;
 }
 //pid.OUT+=; //本次的计算结果
 pid.Ek_11=pid.Ek1;  //更新偏差
 
 pid.C10ms1=0;
 pid.Sv1=SvSave1;
}












