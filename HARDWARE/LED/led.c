#include "led.h"


//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD  | RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PA,PD�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                       //LED2 PC6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;               //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC,GPIO_Pin_6);						 //

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                                   //LED3  PA8
    GPIO_Init(GPIOA, &GPIO_InitStructure);	  				//������� ��IO���ٶ�Ϊ50MHz
    GPIO_SetBits(GPIOA,GPIO_Pin_8); 						//

    GPIO_WriteBit(GPIOC,GPIO_Pin_6,Bit_RESET);
    GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);
}
//LED IO��ʼ��
void PWM_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_ResetBits(GPIOC,GPIO_Pin_0);						 //PB.5 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOC,GPIO_Pin_1); 						 //PE.5 ����� 
}
 
//GPRS �������ų�ʼ�� PB0
void GPRS_Pwk_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    GPRS_Pwk_WriteBit(GPRS_Pwk_IO,GPRS_Pwk_B , Bit_RESET);
}
