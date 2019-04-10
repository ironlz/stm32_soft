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

__IO u8 HTTPPostData[255];      //�������ݼ���������
__IO u8 lcdString[255];         //Һ������ʾ����
char  latString[100];           //��վ��λ����С���Ĳ���γ��
char  lonString[100];           //��վ��λ����С���Ĳ��־���
long  latNum=0;                 //ת��Ϊ
long  lonNum=0;
char  latTemp[9];               //��ʱת������
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
#define pwmout_1     GPIO_SetBits(GPIOC, GPIO_Pin_0)    //ֹͣ����
#define pwmout_0     GPIO_ResetBits(GPIOC,GPIO_Pin_0);   //����
#define pwmout1_1    GPIO_SetBits(GPIOC, GPIO_Pin_1)    //ֹͣ����
#define pwmout1_0    GPIO_ResetBits(GPIOC, GPIO_Pin_1)  //����

	  
void Display(void)
{
	//Show_string1(0x90,"�趨ֵ:");
	printf("pid.Sv=%f\r\n",pid.Sv);	
	//Show_string1(0x88,"��ǰֵ:");
	//Show_number(0X8c,pid.Pv);
	printf("pid.Pv=%f\r\n",pid.Pv);	
	//Show_string1(0x98,"PIDOUT:");
	//Show_number(0X9c,pid.OUT);
	printf("pid.OUT=%f\r\n",pid.OUT);	
		//Show_string1(0x90,"�趨ֵ:");
	printf("pid.Sv1=%f\r\n",pid.Sv1);	
	//Show_string1(0x88,"��ǰֵ:");
	//Show_number(0X8c,pid.Pv);
	printf("pid.Pv1=%f\r\n",pid.Pv1);	
	//Show_string1(0x98,"PIDOUT:");
	//Show_number(0X9c,pid.OUT);
	printf("pid.OUT1=%f\r\n",pid.OUT1);	

}
void PID_Init()
{
  pid.Sv=SetTemp;//�û��趨�¶�
	pid.Kp=30;
	pid.T=200;  //PID��������
  pid.Ti=8000000;//����ʱ��
	pid.Td=150000;//΢��ʱ��
	pid.pwmcycle=100;//pwm����1000
	pid.OUT0=1;
	  
	pid.Sv1=SetTemp1;//�û��趨�¶�
	pid.Kp1=30;
	pid.T1=200;  //PID��������
  pid.Ti1=8000000;//����ʱ��
	pid.Td1=150000;//΢��ʱ��
	pid.pwmcycle1=100;//pwm����1000
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

    temp1=DS18B20_Get_Temp1();//�õ��¶�
		temp=DS18B20_Get_Temp();//�õ��¶�
		time_flag++;
		if(time_flag>20)
		{
				pid.Pv = temp;//��ֵ��PV��Ϊ��ǰֵ
		    pid.Pv1= temp1;
				pid.Sv=SetTemp;//�û��趨�¶�
				pid.Sv1=SetTemp1;//�û��趨�¶�
			
			  time_flag =0;//����=�����¶ȴ��ݣ���ֹ����̫�졣��ʱ�趨Ϊ20�Ρ���Ҫ�Լ��޸�
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

void PID_out()  //���PID������������---ÿ1ms������1��
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
	   pwmout_1;//����

	 }
	 else
	 {
	   pwmout_0;//ֹͣ����

	 }


}


void PID_out1()  //���PID������������---ÿ1ms������1��
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

		 pwmout1_0;//ֹͣ����
	 }


}



 void TIM3_IRQHandler() //1ms 1��
{					  
   u8 st;
	st=TIM_GetFlagStatus(TIM3, TIM_IT_Update);
	if(st!=0)
	{  pid.C10ms++;
		pid.C10ms1++;
		 TIM_ClearFlag(TIM3, TIM_IT_Update); 
     PID_out(); //���PID������������
		 PID_out1();
	}
}

// void TIM3_IRQHandler1() //1ms 1��
//{					  
//   u8 st;
//	st=TIM_GetFlagStatus(TIM3, TIM_IT_Update);
//	if(st!=0)
//	{  pid.C10ms1++;
//		 TIM_ClearFlag(TIM3, TIM_IT_Update); 
//     PID_out1(); //���PID������������
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
		char loFormat[] = "lo:%5.2f";// ���ȵĸ�ʽ���ַ���
		char loStr[15];
		char laFormat[] = "la:%5.2f";// γ�ȵĸ�ʽ���ַ���
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
    NVIC_Configuration();               //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    delay_init();	    	 	//��ʱ������ʼ��
    uart_init(115200);	 		//����1��ʼ��Ϊ9600
    delay_ms(100);
    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ� PA8  PC6
	  KEY_Init();   //������ʼ��
    GPRS_Pwk_Init();                    //GPRS��������

    GPRS_Pwk_WriteBit(GPRS_Pwk_IO, GPRS_Pwk_B, Bit_SET);		 //����GPRS
    delay_ms(10000);
    USART2_Init(9600);	//��GSMģ��ͨ��
    uart3_init(115200);
    GPRS_Pwk_WriteBit(GPRS_Pwk_IO, GPRS_Pwk_B, Bit_RESET);		//��LED

    LCD5110_Init();     //Һ������ʼ��
    delay_ms(10);
    LCD_Clear();
    delay_ms(10); //
    LCD_Write_EnStr(1,1,"STM32+MC20");
    LCD_Write_EnStr(0,3,"GPS GPRS LBS");
	
    DS18B20_Init();
    printf("\r\n ############ http://csic.taobao.com/ ############\r\n ############("__DATE__ " - " __TIME__ ")############");
		PID_Init();//PID��ʼ��
    //u2_printf("���ڶ���ʼ���ɹ�\r\n");//����2����
    //Usart_SendString(USART3,"��������ʼ���ɹ�\r\n");//����3����
    
    PWM_Init();//PWM���ų�ʼ��
    EXTIX_Init();//
							
    pwmout_0;//ֹͣ���ȡ
    pwmout1_0;//ֹͣ���ȡ
    
    while(1)
    {

        LCD_Clear();
        LCD_Write_EnStr(0,2,"register[..]");
        delay_ms(2000);

        if (MC20_init() == ERROR)continue;	//�����������HTTP����;

        LCD_Clear();
        LCD_Write_EnStr(0,2,"connect[ok]");

        printf("====�����ӷ�����==== \r\n");
        delay_ms(2000);

        LCD_Clear();
        LCD_Write_EnStr(0,2,"start GPS[..]");

        MC20_send_cmd("AT+QGNSSC=1\r\n","OK","+CME ERROR: Operation failed","NULL",200);
        delay_ms(2000);
			  Timer3_init();
        while(1)
        {				
                 LCD_Clear();					
					 
					      PID_Calc(); //pid���� 
					      PID_Calc1(); //pid���� 
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
								// ����LCDҪ��ʾ����Ϣ
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
            /*if(USART3_RX_STA&0X8000)		//���յ����ݴ���3��GPSͨ�� ����2��GPRSͨ��
            {
                printf("#REV GPS DATA:");
                printf((const char*)USART3_RX_BUF,"#\r\n");
                if(USART3_RX_BUF[18]=='A')		//GPS�Ѿ���λ����
                {
									 LCD_Clear();
									  LCD_Write_EnStr(0,0," GPS ok");
                    ClearRAM((u8*)HTTPPostData,255);//���HTTPPostData���ݰ�����
                    strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),"$GPRMC",6);
                    strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&USART3_RX_BUF[6]),62);
                    strcpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),"\x1a");

              
									
									
                    LCD_Clear();
                    LCD_Write_EnStr(0,0,"send GPS data");//����Բ���
									
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
                else if(USART3_RX_BUF[18]=='V'&& rand()%5==2)	//û��λ��
                {

                    LCD_Clear();
                    LCD_Write_EnStr(0,2,"SEND LBS DATA");//û����߹��
									
									
									
									

                    MC20_send_cmd("AT+CGATT?\r\n","+CGATT: 1","OK","NULL",200);		//��ѯ��վ��λ���
                    delay_ms(200);
                    MC20_send_cmd("AT+QIFGCNT=0\r\n","OK","NULL","NULL",200);			//?Context0
                    delay_ms(200);
                    if(MC20_send_cmd("AT+QCELLLOC=1\r\n","+QCELLLOC:","NULL","NULL",200)==1)	//��ȡ��վ����
                    {
                        printf("----------send LBS data------------\r\n");		//��ӡ��ȡ�Ļ�վ��Ϣ
                        delay_ms(1000);
                        ClearRAM((u8*)HTTPPostData,255);			//���HTTPPostData���ݰ�����
                        ClearRAM((u8*)lcdString,255);                           //���Һ������ʾ���ݰ�����
                        ClearRAM((u8*)latString,100);
                        ClearRAM((u8*)lonString,100);
                        ClearRAM((u8*)latTemp,9);
                        ClearRAM((u8*)lonTemp,9);
                        //���濪ʼ��γ�Ȼ���
                        strncpy((char*)(latString+strlen((char*)latString)),(char*)(&USART2_RX_BUF[27]),6); //��ȡά��С���� 065016
                        strncpy((char*)(lonString+strlen((char*)lonString)),(char*)(&USART2_RX_BUF[17]),6); //��ȡ����С����

                        latNum = atoi((const char *)latString)*6;   //ת�� 390096
                        lonNum = atoi((const char *)lonString)*6;   //�� ������

                        sprintf(latTemp,"%ld",latNum);
                        sprintf(lonTemp,"%ld",lonNum);

                        ClearRAM((u8*)latString,100);   //������ ע�ⳤ��
                        ClearRAM((u8*)lonString,100);

                        if(strlen(latTemp)<7)       //γ���м��0390096
                        {
                            latString[0]='0';
                            strcat(latString,latTemp);
                        }
                        else
                        {strcpy(latString,latTemp);}

                        if(strlen(lonTemp)<7)       //�ֲ�����Ҫ���㴦��
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
                        strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&USART2_RX_BUF[24]),2);   //ԭ����γ��copy26
                        strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&latString[0]),2);        //�����copy03

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
            if(USART2_RX_STA&0X8000)		//���յ�����GPRS������
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
    if(MC20_send_cmd("AT","OK","NULL","NULL",1000))err|=1<<0;//����Ƿ�Ӧ��ATָ��
    USART2_RX_STA=0;
    if(MC20_send_cmd("ATE0","OK","NULL","NULL",2000))err|=1<<1;//������
    USART2_RX_STA=0;
    if(MC20_send_cmd("AT+CPIN?","OK","NULL","NULL",2000))err|=1<<3;	//��ѯSIM���Ƿ���λ
    USART2_RX_STA=0;
    data = 0;
    //��ѯGSM����ע��״̬��ȷ�������ɹ�
    while (MC20_send_cmd("AT+CREG?\r\n","\r\n+CREG: 0,1","NULL","NULL",2000)!= 1 && data < 10)
    {
        USART2_RX_STA=0;
        delay_ms(100);
        data++;
    }
    USART2_RX_STA=0;
    if (data == 10)
    {
        return ERROR;                                                                             //�������ɹ�ģ������
    }
    MC20_send_cmd("AT+CGATT?\r\n","+CGATT: 1","OK","NULL",2000);
    USART2_RX_STA=0;
    delay_ms(200);
    MC20_send_cmd("AT+QIFGCNT=0\r\n","OK","NULL","NULL",2000);
    USART2_RX_STA=0;
    delay_ms(200);
    USART2_RX_STA=0;

    data=MC20_send_cmd(SERVERIP,"OK","ALREADY CONNECT","CONNECT OK",200);	//���ñ��к�����ʾ
    USART2_RX_STA=0;
    ret=MC20_send_cmd("NULL","CONNECT","NULL","NULL",20);	//����������ʾ
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

void EXTI2_IRQHandler(void) //��ת�����ж�
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET) //������
	{
		delay_ms(5);   //����		
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

	 EXTI_ClearITPendingBit(EXTI_Line2);  //���LINE15��·����λ
}
void EXTI3_IRQHandler(void) //��ת�����ж� //����ȷ��
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) 
	{
        delay_ms(5);   //����			
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
	 EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE15��·����λ
}
void EXTI4_IRQHandler(void) //��ת�����ж�
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)  //������
	{
		delay_ms(5);   //����		
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
	 EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE15��·����λ
}
