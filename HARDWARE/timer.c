#include "timer.h"
#include "led.h"
#include "key.h"
void Timer4_init(void)	//T4 10msʱ��
{	 //72000000/7200=100us
//
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4,ENABLE);

 

TIM_TimeBaseStructure.TIM_Period = 10000-1; //��������     //100us*10=1000us=10ms
TIM_TimeBaseStructure.TIM_Prescaler =72-1;//��Ƶֵ   	    
TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 	//�ָ�ʱ��			
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
 TIM_DeInit(TIM4);
TIM_TimeBaseInit(TIM4, & TIM_TimeBaseStructure); 
TIM_Cmd(TIM4, ENABLE); 	 //ʹ�ܶ�ʱ��2

 /*���¶�ʱ��4�жϳ�ʼ��*/
TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //���ϼ�����������ж�


}
  
void Timer3_init(void)	//T3 1msʱ��
{	 //72000000/7200=100us
//
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
NVIC_InitTypeDef NVIC_InitStructure; 
RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3,ENABLE);

 

TIM_TimeBaseStructure.TIM_Period = 1000-1; //��������     //10ms
TIM_TimeBaseStructure.TIM_Prescaler =72-1;//��Ƶֵ   	    
TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 	//�ָ�ʱ��			
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
 TIM_DeInit(TIM3);
TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure); 
TIM_Cmd(TIM3, ENABLE); 	 //ʹ�ܶ�ʱ��2

	
NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;// TIM3_IRQChannel; 
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
NVIC_Init (&NVIC_InitStructure);
	
 /*���¶�ʱ��4�жϳ�ʼ��*/
TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //���ϼ�����������ж�

}

void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
}

 
//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{
 //�������Ų�,����Ļ��ˢ�²�
 	  EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��

	//  KEY_Init();//��ʼ��������Ӧioģʽ

    //GPIOC.14 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource2);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    //GPIOB.13	  �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    //GPIOB.12	  �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);

   	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


   	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//|EXTI3_IRQn| EXTI4_IRQn    ;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
		NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//|EXTI3_IRQn| EXTI4_IRQn    ;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//|EXTI3_IRQn| EXTI4_IRQn    ;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
 
}













