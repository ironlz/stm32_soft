#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "usart.h"	
#include "mc20.h"
#include "usart2.h" 
#include "usart3.h"	
#include "LCD5110.h"
#include "timer.h"
#include "ds18b20.h"
#include "pid.h"
u16 SetTemp=250;
u16 SetTemp1=250;
ErrorStatus MC20_init(void);
u8 key_press = 0,key_hold = 0;

__IO u8 HTTPPostData[255];      //发送数据集到服务器
__IO u8 lcdString[255];         //液晶屏显示数据
char  latString[100];           //基站定位数据小数的部分纬度
char  lonString[100];           //基站定位数据小数的部分经度
long  latNum=0;                 //转换为
long  lonNum=0;
char  latTemp[9];               //临时转换数据
char  lonTemp[9];
char lizhentemp[40];
char TZwendu[30];
char lizhentemp1[40];
char TZwendu1[30];
u16 Kms10;
#define  BUFLEN 20
#define  BUFLEN1 20
char wendu[BUFLEN];
char wendu_gprs[BUFLEN];

char wendu1[BUFLEN1];
char wendu_gprs1[BUFLEN1]; 
char wendu_tz[20]; 
char wendu_tz1[20]; 

char wendu_tz1[20]; 
char wendu_tz12[20]; 
u8 dis=0;
#define SERVERIP "AT+QIOPEN=\"UDP\",\"125.77.81.215\",\"1001\""
short temp = 0,time_flag=0,temp1 = 0;
#define pwmout_1     GPIO_SetBits(GPIOC, GPIO_Pin_0)    //停止加热
#define pwmout_0     GPIO_ResetBits(GPIOC,GPIO_Pin_0);   //加热
#define pwmout1_1    GPIO_SetBits(GPIOC, GPIO_Pin_1)    //停止加热
#define pwmout1_0    GPIO_ResetBits(GPIOC, GPIO_Pin_1)  //加热

	  
void Display(void)
{
	//Show_string1(0x90,"设定值:");
	printf("pid.Sv=%f\r\n",pid.Sv);	
	//Show_string1(0x88,"当前值:");
	//Show_number(0X8c,pid.Pv);
	printf("pid.Pv=%f\r\n",pid.Pv);	
	//Show_string1(0x98,"PIDOUT:");
	//Show_number(0X9c,pid.OUT);
	printf("pid.OUT=%f\r\n",pid.OUT);	
		//Show_string1(0x90,"设定值:");
	printf("pid.Sv1=%f\r\n",pid.Sv1);	
	//Show_string1(0x88,"当前值:");
	//Show_number(0X8c,pid.Pv);
	printf("pid.Pv1=%f\r\n",pid.Pv1);	
	//Show_string1(0x98,"PIDOUT:");
	//Show_number(0X9c,pid.OUT);
	printf("pid.OUT1=%f\r\n",pid.OUT1);	

}
void PID_Init()
{
  pid.Sv=SetTemp;//用户设定温度
	pid.Kp=30;
	pid.T=200;  //PID计算周期
  pid.Ti=8000000;//积分时间
	pid.Td=150000;//微分时间
	pid.pwmcycle=100;//pwm周期1000
	pid.OUT0=1;
	  
	pid.Sv1=SetTemp1;//用户设定温度
	pid.Kp1=30;
	pid.T1=200;  //PID计算周期
  pid.Ti1=8000000;//积分时间
	pid.Td1=150000;//微分时间
	pid.pwmcycle1=100;//pwm周期1000
	pid.OUT01=1;
}


void display_template_event(void)
{
    
    char tem[BUFLEN];
	  char tem1[BUFLEN1];
    memset(wendu,0,BUFLEN);
    memset(tem,0,BUFLEN);
    memset(wendu_gprs,0,BUFLEN);
	
	  memset(wendu1,0,BUFLEN1);
    memset(tem1,0,BUFLEN1);
    memset(wendu_gprs1,0,BUFLEN1);
    if(DS18B20_Init())
        return ;
    if(DS18B20_Init1())
				return ;

    temp1=DS18B20_Get_Temp1();//得到温度
		temp=DS18B20_Get_Temp();//得到温度
		time_flag++;
		if(time_flag>20)
		{
				pid.Pv = temp;//赋值给PV作为当前值
		    pid.Pv1= temp1;
				pid.Sv=SetTemp;//用户设定温度
				pid.Sv1=SetTemp1;//用户设定温度
			
			  time_flag =0;//用于=更新温度传递，防止传递太快。暂时设定为20次。需要稍加修改
		}
    if(temp<0)						
    {
        temp=-temp;

        printf("-");
        memset(wendu,0,BUFLEN);
        strcat(wendu,"-");
    }
		 if(temp1<0)						
    {
        temp1=-temp1;

        printf("-");
        memset(wendu1,0,BUFLEN1);
        strcat(wendu1,"-");
    }
		
    memset(tem,0,BUFLEN);
    sprintf(tem,"%d",temp/10);		
    strcat(wendu,tem);
    strcat(wendu,".");				 
    memset(tem,0,BUFLEN);
    sprintf(tem,"%d",temp%10);
    strcat(wendu,tem);
    sprintf(wendu_gprs,"temperature:%s#",wendu);
    printf("\r\n\r\nwendu_gprs=%s\r\n",wendu_gprs);
		
		memset(tem1,0,BUFLEN1);
    sprintf(tem1,"%d",temp1/10);		
    strcat(wendu1,tem1);
    strcat(wendu1,".");				 
    memset(tem1,0,BUFLEN1);
    sprintf(tem1,"%d",temp1%10);
    strcat(wendu1,tem1);
    sprintf(wendu_gprs1,"temperature:%s#",wendu1);
    printf("\r\n\r\nwendu_gprs=%s\r\n",wendu_gprs1);
    
}

void PID_out()  //输出PID运算结果到负载---每1ms被调用1次
{
   static u16 pw;
	 pw++;
	 if(pw>=pid.pwmcycle)  //
	 {
	   pw=0;
	 }
	  //0  ~  pid.pwmcycle-1
	 
	 if(pw<pid.OUT)
	 {
	   pwmout_1;//加热

	 }
	 else
	 {
	   pwmout_0;//停止加热

	 }


}


void PID_out1()  //输出PID运算结果到负载---每1ms被调用1次
{
   static u16 pw1;
	 pw1++;
	 if(pw1>=pid.pwmcycle1)  //
	 {
	   pw1=0;
	 }
	  //0  ~  pid.pwmcycle-1
	 
	 if(pw1<pid.OUT1)
	 {

		 pwmout1_1;
	 }
	 else
	 {

		 pwmout1_0;//停止加热
	 }


}



 void TIM3_IRQHandler() //1ms 1次
{					  
   u8 st;
	st=TIM_GetFlagStatus(TIM3, TIM_IT_Update);
	if(st!=0)
	{  pid.C10ms++;
		pid.C10ms1++;
		 TIM_ClearFlag(TIM3, TIM_IT_Update); 
     PID_out(); //输出PID运算结果到负载
		 PID_out1();
	}
}

// void TIM3_IRQHandler1() //1ms 1次
//{					  
//   u8 st;
//	st=TIM_GetFlagStatus(TIM3, TIM_IT_Update);
//	if(st!=0)
//	{  pid.C10ms1++;
//		 TIM_ClearFlag(TIM3, TIM_IT_Update); 
//     PID_out1(); //输出PID运算结果到负载
//	}
//}

int main(void)
{
	  // added by lizhen
    /*
		*the lcd will display like that
		*      ======================
	  *     |  Server:x      |
		*     |  lo:xxx.xx          |
		*     |  la:xxx.xx          |
		*     |  T1:xx.xx/xx        |
		*     |  T2:xx.xx/xx        |
		*      ======================
		*/
		// first line format: F is failed, P is connected.
	  char serverFormat[] = "Service:%c";
		char serverStr[15];
		char isServerOk = 'F';
		
		// second and third line format, for location: 
		// lo:xxx.xx
		// la:xxx.xx
		char loFormat[] = "lo:%5.2f";// 经度的格式化字符串
		char loStr[15];
		char laFormat[] = "la:%5.2f";// 纬度的格式化字符串
		char laStr[15];
    float longtitude = 0.0f;
    float latitude = 0.0f;
		
		// forth and fifth line format, for tempture:
		char tOneFormat[] = "T1:%s/%s";
		char tOneStr[15];
		char tTwoFormat[] = "T2:%s/%s";
		char tTwoStr[15];
		float tOneReal = 0.0f;
		float tOneSet = 0;
		float tTwoReal = 0.0f;
		float tTwoSet = 0;  
	  // added end
	
    char temtz[50];
	  char temtz1[50];
    NVIC_Configuration();               //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    delay_init();	    	 	//延时函数初始化
    uart_init(115200);	 		//串口1初始化为9600
    delay_ms(100);
    LED_Init();		  		//初始化与LED连接的硬件接口 PA8  PC6
	  KEY_Init();   //按键初始化
    GPRS_Pwk_Init();                    //GPRS启动引脚

    GPRS_Pwk_WriteBit(GPRS_Pwk_IO, GPRS_Pwk_B, Bit_SET);		 //开启GPRS
    delay_ms(10000);
    USART2_Init(9600);	//与GSM模块通信
    uart3_init(115200);
    GPRS_Pwk_WriteBit(GPRS_Pwk_IO, GPRS_Pwk_B, Bit_RESET);		//打开LED

    LCD5110_Init();     //液晶屏初始化
    delay_ms(10);
    LCD_Clear();
    delay_ms(10); //
    LCD_Write_EnStr(1,1,"STM32+MC20");
    LCD_Write_EnStr(0,3,"GPS GPRS LBS");
	
    DS18B20_Init();
    printf("\r\n ############ http://csic.taobao.com/ ############\r\n ############("__DATE__ " - " __TIME__ ")############");
		PID_Init();//PID初始化
    //u2_printf("串口二初始化成功\r\n");//串口2测试
    //Usart_SendString(USART3,"串口三初始化成功\r\n");//串口3测试
    
    PWM_Init();//PWM引脚初始话
    EXTIX_Init();//
							
    pwmout_0;//停止加热�
    pwmout1_0;//停止加热�
    
    while(1)
    {

        LCD_Clear();
        LCD_Write_EnStr(0,2,"register[..]");
        delay_ms(2000);

        if (MC20_init() == ERROR)continue;	//与服务器建立HTTP链接;

        LCD_Clear();
        LCD_Write_EnStr(0,2,"connect[ok]");

        printf("====已连接服务器==== \r\n");
        delay_ms(2000);

        LCD_Clear();
        LCD_Write_EnStr(0,2,"start GPS[..]");

        MC20_send_cmd("AT+QGNSSC=1\r\n","OK","+CME ERROR: Operation failed","NULL",200);
        delay_ms(2000);
			  Timer3_init();
        while(1)
        {				
                 LCD_Clear();					
					 
					      PID_Calc(); //pid计算 
					      PID_Calc1(); //pid计算 
      					display_template_event();
					      sprintf(lizhentemp, "w1:%s", wendu);
			          //LCD_Write_EnStr(0,4,(u8*)lizhentemp);		
					      sprintf(lizhentemp1, "w2:%s", wendu1);
			          //LCD_Write_EnStr(7,4,(u8*)lizhentemp1);						
					      memset(TZwendu,0,30);
								memset(temtz,0,50);
								sprintf(temtz,"%d",SetTemp/10);		
								strcat(TZwendu,temtz);
								strcat(TZwendu,".");				 
								memset(temtz,0,50);
								sprintf(temtz,"%d",SetTemp%10);
								strcat(TZwendu,temtz);
								sprintf(wendu_tz,"t1:%s",TZwendu);
				      // sprintf(lizhentemp, "wendu: %s", wendu);
		            //LCD_Write_EnStr(0,5,(u8*)wendu_tz);				
				      	memset(TZwendu1,0,30);
								memset(temtz1,0,50);
								sprintf(temtz1,"%d",SetTemp1/10);		
								strcat(TZwendu1,temtz1);
								strcat(TZwendu1,".");				 
								memset(temtz1,0,50);
								sprintf(temtz1,"%d",SetTemp1%10);
								strcat(TZwendu1,temtz1);
								sprintf(wendu_tz1,"t2:%s",TZwendu1);							
				      // sprintf(lizhentemp, "wendu: %s", wendu);
		            //LCD_Write_EnStr(7,5,(u8*)wendu_tz1);
				    	/*if(key_press==1)
				        LCD_Write_EnStr(0,0,"One ");
				    	else
						    LCD_Write_EnStr(0,0,"Two");
							*/
								// 处理LCD要显示的信息
								sprintf(serverStr, serverFormat, isServerOk);// first line
								LCD_Write_EnStr(0, 0, (unsigned char*)serverStr);
								// second line
								sprintf(loStr, loFormat, longtitude);
								LCD_Write_EnStr(0, 1, (unsigned char*)loStr);
								// third line
								sprintf(laStr, laFormat, latitude);
								LCD_Write_EnStr(0, 2, (unsigned char*)laStr);
								// forth line
								sprintf(tOneStr, tOneFormat, wendu, TZwendu);
								LCD_Write_EnStr(0, 3, (unsigned char*)tOneStr);
								// fifth line
								sprintf(tTwoStr, tTwoFormat, wendu1, TZwendu1);
								LCD_Write_EnStr(0, 4, (unsigned char*)tTwoStr);
								
								
								Display();
					      delay_ms(2000);
            /*if(USART3_RX_STA&0X8000)		//接收到数据串口3和GPS通信 串口2和GPRS通信
            {
                printf("#REV GPS DATA:");
                printf((const char*)USART3_RX_BUF,"#\r\n");
                if(USART3_RX_BUF[18]=='A')		//GPS已经定位好了
                {
									 LCD_Clear();
									  LCD_Write_EnStr(0,0," GPS ok");
                    ClearRAM((u8*)HTTPPostData,255);//清空HTTPPostData数据包缓存
                    strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),"$GPRMC",6);
                    strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&USART3_RX_BUF[6]),62);
                    strcpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),"\x1a");

              
									
									
                    LCD_Clear();
                    LCD_Write_EnStr(0,0,"send GPS data");//这里对不对
									
                    if(strlen((const char *)lcdString)>0)
                        LCD_Write_EnStr(0,2,(u8*)lcdString);

                    printf("----------send GPS data------------\r\n");

                    if (MC20_send_cmd("AT+QISEND\r\n","\r\n>","NULL","NULL",2000) != 1)
                    {
                        USART2_RX_STA=0;
                        USART3_RX_STA=0;
                        break;
                    }
                    delay_ms(2000);
                    if ((MC20_send_cmd((u8 *)HTTPPostData,"\r\nSEND OK","SEND OK","NULL",2000) != 1)) //REI
                    {
                        USART2_RX_STA=0;
                        USART3_RX_STA=0;
                        break;
                    }
                }
                else if(USART3_RX_BUF[18]=='V'&& rand()%5==2)	//没定位好
                {

                    LCD_Clear();
                    LCD_Write_EnStr(0,2,"SEND LBS DATA");//没问题吖？
									
									
									
									

                    MC20_send_cmd("AT+CGATT?\r\n","+CGATT: 1","OK","NULL",200);		//查询基站定位结果
                    delay_ms(200);
                    MC20_send_cmd("AT+QIFGCNT=0\r\n","OK","NULL","NULL",200);			//?Context0
                    delay_ms(200);
                    if(MC20_send_cmd("AT+QCELLLOC=1\r\n","+QCELLLOC:","NULL","NULL",200)==1)	//获取基站数据
                    {
                        printf("----------send LBS data------------\r\n");		//打印获取的基站信息
                        delay_ms(1000);
                        ClearRAM((u8*)HTTPPostData,255);			//清空HTTPPostData数据包缓存
                        ClearRAM((u8*)lcdString,255);                           //清空液晶屏显示数据包缓存
                        ClearRAM((u8*)latString,100);
                        ClearRAM((u8*)lonString,100);
                        ClearRAM((u8*)latTemp,9);
                        ClearRAM((u8*)lonTemp,9);
                        //下面开始经纬度换算
                        strncpy((char*)(latString+strlen((char*)latString)),(char*)(&USART2_RX_BUF[27]),6); //获取维度小数点 065016
                        strncpy((char*)(lonString+strlen((char*)lonString)),(char*)(&USART2_RX_BUF[17]),6); //获取经度小数点

                        latNum = atoi((const char *)latString)*6;   //转换 390096
                        lonNum = atoi((const char *)lonString)*6;   //度 长整形

                        sprintf(latTemp,"%ld",latNum);
                        sprintf(lonTemp,"%ld",lonNum);

                        ClearRAM((u8*)latString,100);   //清数组 注意长度
                        ClearRAM((u8*)lonString,100);

                        if(strlen(latTemp)<7)       //纬度中间的0390096
                        {
                            latString[0]='0';
                            strcat(latString,latTemp);
                        }
                        else
                        {strcpy(latString,latTemp);}

                        if(strlen(lonTemp)<7)       //分不到需要补零处理
                        {
                            lonString[0]='0';
                            strcat(lonString,lonTemp);
                        }
                        else
                        {strcpy(lonString,lonTemp);}


                        //$GPRMC,042631.000,A,2603.31476,N,11913.03068,E,0.76,11.12,281017,,,D*4C
                        strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),"$GPRMC",6);
                        strncpy((char*)(lcdString+strlen((char*)lcdString)),"LBS=",8);
                        strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),",043754.000,A,",14);
                        strncpy((char*)(lcdString+strlen((char*)lcdString)),"LAT:",10);
                        strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&USART2_RX_BUF[24]),2);   //原来的纬度copy26
                        strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&latString[0]),2);        //处理后copy03

                        strncpy((char*)(lcdString+strlen((char*)lcdString)),(char*)(&USART2_RX_BUF[24]),2);
                        strncpy((char*)(lcdString+strlen((char*)lcdString)),(char*)(&USART2_RX_BUF[27]),2);
                        strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),".",1);
                        strncpy((char*)(lcdString+strlen((char*)lcdString)),".",1);
                        strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&latString[2]),5);
                        strncpy((char*)(lcdString+strlen((char*)lcdString)),(char*)(&USART2_RX_BUF[29]),4);


                        strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),",N,",3);

                        strncpy((char*)(lcdString+strlen((char*)lcdString)),",LON:",10);
                        strncpy((char*)(lcdString+strlen((char*)lcdString)),(char*)(&USART2_RX_BUF[13]),3);
                        strncpy((char*)(lcdString+strlen((char*)lcdString)),(char*)(&USART2_RX_BUF[17]),2);
                        strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&USART2_RX_BUF[13]),3);
                        strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&lonString[0]),2);
                        strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),".",1);
                        strncpy((char*)(lcdString+strlen((char*)lcdString)),".",1);
                        strncpy((char*)(lcdString+strlen((char*)lcdString)),(char*)(&USART2_RX_BUF[19]),5);
                        strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&lonString[2]),5);
                        strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),",E,0.17,43.08,210817,,,A*4",26);
                        strcpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),"\x1a");
                        printf("LBS POST=%s\r\n",HTTPPostData);

                        LCD_Clear();
                        delay_ms(10); //
												
												
												
												
												
												
												
                        LCD_Write_EnStr(0,1,(u8*)lcdString);
                        if (MC20_send_cmd("AT+QISEND\r\n","\r\n>","NULL","NULL",2000) != 1)
                        {
                            USART2_RX_STA=0;
                            USART3_RX_STA=0;
                            break;
                        }
                        USART2_RX_STA=0;
                        delay_ms(500);
                        if ((MC20_send_cmd((u8 *)HTTPPostData,"\r\nSEND OK","SEND OK","NULL",2000) != 1))	//REI
                        {
                            USART2_RX_STA=0;
                            USART3_RX_STA=0;
                            break;
                        }
                        USART2_RX_STA=0;
                        USART3_RX_STA=0;
                    }
                }
                USART3_RX_STA=0;
            }          
            if(USART2_RX_STA&0X8000)		//接收到数据GPRS的数据
            {
                printf((const char*)USART2_RX_BUF,"\r\n");
                USART2_RX_STA=0;
            }
            delay_ms(2000);*/
        }
    }
}

ErrorStatus MC20_init(void)
{
    u8 data=0,ret=0;
    u8 err=0;
    USART2_RX_STA=0;
    if(MC20_send_cmd("AT","OK","NULL","NULL",1000))err|=1<<0;//检测是否应答AT指令
    USART2_RX_STA=0;
    if(MC20_send_cmd("ATE0","OK","NULL","NULL",2000))err|=1<<1;//不回显
    USART2_RX_STA=0;
    if(MC20_send_cmd("AT+CPIN?","OK","NULL","NULL",2000))err|=1<<3;	//查询SIM卡是否在位
    USART2_RX_STA=0;
    data = 0;
    //查询GSM网络注册状态，确认找网成功
    while (MC20_send_cmd("AT+CREG?\r\n","\r\n+CREG: 0,1","NULL","NULL",2000)!= 1 && data < 10)
    {
        USART2_RX_STA=0;
        delay_ms(100);
        data++;
    }
    USART2_RX_STA=0;
    if (data == 10)
    {
        return ERROR;                                                                             //找网不成功模块重启
    }
    MC20_send_cmd("AT+CGATT?\r\n","+CGATT: 1","OK","NULL",2000);
    USART2_RX_STA=0;
    delay_ms(200);
    MC20_send_cmd("AT+QIFGCNT=0\r\n","OK","NULL","NULL",2000);
    USART2_RX_STA=0;
    delay_ms(200);
    USART2_RX_STA=0;

    data=MC20_send_cmd(SERVERIP,"OK","ALREADY CONNECT","CONNECT OK",200);	//设置被叫号码显示
    USART2_RX_STA=0;
    ret=MC20_send_cmd("NULL","CONNECT","NULL","NULL",20);	//设置来电显示
    USART2_RX_STA=0;

    LCD_Clear();
    LCD_Write_EnStr(0,0,SERVERIP);
    delay_ms(4000);

    if (data == 1 || data == 2 || data == 3 || ret==1)
    {
        printf("data=%d\r\n",data);
        return SUCCESS;
    }
    else
    {
        return ERROR;
    }
} 

void EXTI2_IRQHandler(void) //旋转开关中断
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET) //按键加
	{
		delay_ms(5);   //消抖		
		 if((KEY0==0) && (key_press==1))
			 {
					SetTemp+=5;	
					printf("\r\n\r\this is PC2 is press\r\n");
			}
		 if((KEY0==0) && (key_press==0))
			 {
					SetTemp1=SetTemp1+5;	
					printf("\r\n\r\this is PC2 is press\r\n");
			}

	       						      
	}

	 EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE15线路挂起位
}
void EXTI3_IRQHandler(void) //旋转开关中断 //按键确认
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) 
	{
        delay_ms(5);   //消抖			
				if(KEY1==0)
					{			
						key_press++;
							printf("\r\n\r\this is PC3 is press=%c\r\n",key_press);
					}
					if(key_press>=2)
					{
						key_press=0;					
					}
	}
	 EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE15线路挂起位
}
void EXTI4_IRQHandler(void) //旋转开关中断
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)  //按键减
	{
		delay_ms(5);   //消抖		
		if((WK_UP==0)&&(key_press==1))
		{	  
			 SetTemp-=5; 
			 printf("\r\n\r\this is PC4 is press\r\n");
		}
				if((WK_UP==0)&&(key_press==0))
		{	  
			 SetTemp1=SetTemp1-5; 
			 printf("\r\n\r\this is PC4 is press\r\n");
		}
	}	
	 EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE15线路挂起位
}
