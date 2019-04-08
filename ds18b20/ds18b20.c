#include "ds18b20.h"
#include "delay.h"
//2010/6/17

//��λDS18B20
void DS18B20_Rst(void)
{
    DS18B20_IO_OUT(); //SET PA0 OUTPUT
    DS18B20_DQ_OUT=0; //����DQ
    delay_us(750);    //����750us
    DS18B20_DQ_OUT=1; //DQ=1
    delay_us(15);     //15US
}
void DS18B20_Rst1(void)
{
    DS18B20_IO_OUT1(); //SET PA0 OUTPUT
    DS18B20_DQ_OUT1=0; //����DQ
    delay_us(750);    //����750us
    DS18B20_DQ_OUT1=1; //DQ=1
    delay_us(15);     //15US
}
//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
u8 DS18B20_Check(void)
{
    u8 retry=0;
    DS18B20_IO_IN();//SET PA8 INPUT
    while (DS18B20_DQ_IN&&retry<200)
    {
        retry++;
        delay_us(1);
    };
    if(retry>=200)return 1;
    else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
    {
        retry++;
        delay_us(1);
    };
    if(retry>=240)return 1;
    return 0;
}
u8 DS18B20_Check1(void)
{
    u8 retry1=0;
    DS18B20_IO_IN1();//SET PA8 INPUT
    while (DS18B20_DQ_IN1&&retry1<200)
    {
        retry1++;
        delay_us(1);
    };
    if(retry1>=200)return 1;
    else retry1=0;
    while (!DS18B20_DQ_IN1&&retry1<240)
    {
        retry1++;
        delay_us(1);
    };
    if(retry1>=240)return 1;
    return 0;
}
//��DS18B20��ȡһ��λ
//����ֵ��1/0
u8 DS18B20_Read_Bit(void) 			 // read one bit
{
    u8 data;
    DS18B20_IO_OUT();//SET PA0 OUTPUT
    DS18B20_DQ_OUT=0;
    delay_us(2);
    DS18B20_DQ_OUT=1;
    DS18B20_IO_IN();//SET PA0 INPUT
    delay_us(12);
    if(DS18B20_DQ_IN)data=1;
    else data=0;
    delay_us(50);
    return data;
}
u8 DS18B20_Read_Bit1(void) 			 // read one bit
{
    u8 data1;
    DS18B20_IO_OUT1();//SET PA0 OUTPUT
    DS18B20_DQ_OUT1=0;
    delay_us(2);
    DS18B20_DQ_OUT1=1;
    DS18B20_IO_IN1();//SET PA0 INPUT
    delay_us(12);
    if(DS18B20_DQ_IN1)data1=1;
    else data1=0;
    delay_us(50);
    return data1;
}
//��DS18B20��ȡһ���ֽ�
//����ֵ������������
u8 DS18B20_Read_Byte(void)    // read one byte
{
    u8 i,j,dat;
    dat=0;
    for (i=1; i<=8; i++)
    {
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }
    return dat;
}
u8 DS18B20_Read_Byte1(void)    // read one byte
{
    u8 i1,j1,dat1;
    dat1=0;
    for (i1=1; i1<=8; i1++)
    {
        j1=DS18B20_Read_Bit1();
        dat1=(j1<<7)|(dat1>>1);
    }
    return dat1;
}
//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(u8 dat)
{
    u8 j;
    u8 testb;
    DS18B20_IO_OUT();//SET PA0 OUTPUT;
    for (j=1; j<=8; j++)
    {
        testb=dat&0x01;
        dat=dat>>1;
        if (testb)
        {
            DS18B20_DQ_OUT=0;// Write 1
            delay_us(2);
            DS18B20_DQ_OUT=1;
            delay_us(60);
        }
        else
        {
            DS18B20_DQ_OUT=0;// Write 0
            delay_us(60);
            DS18B20_DQ_OUT=1;
            delay_us(2);
        }
    }
}
void DS18B20_Write_Byte1(u8 dat1)
{
    u8 j1;
    u8 testb1;
    DS18B20_IO_OUT1();//SET PA0 OUTPUT;
    for (j1=1; j1<=8; j1++)
    {
        testb1=dat1&0x01;
        dat1=dat1>>1;
        if (testb1)
        {
            DS18B20_DQ_OUT1=0;// Write 1
            delay_us(2);
            DS18B20_DQ_OUT1=1;
            delay_us(60);
        }
        else
        {
            DS18B20_DQ_OUT1=0;// Write 0
            delay_us(60);
            DS18B20_DQ_OUT1=1;
            delay_us(2);
        }
    }
}
//��ʼ�¶�ת��
void DS18B20_Start(void)// ds1820 start convert
{
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}
void DS18B20_Start1(void)// ds1820 start convert
{
    DS18B20_Rst1();
    DS18B20_Check1();
    DS18B20_Write_Byte1(0xcc);// skip rom
    DS18B20_Write_Byte1(0x44);// convert
}

//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����
u8 DS18B20_Init(void)
{
    RCC->APB2ENR|=1<<4;    //ʹ��PORTA��ʱ��
    GPIOC->CRL&=0XFF0FFFFF;
    GPIOC->CRL|=0x00300000;
    GPIOC->ODR|=1<<5;      //PC5���1
    DS18B20_Rst();
    return DS18B20_Check();
}
u8 DS18B20_Init1(void)                    //��û�ģ�ֻ����������1
{
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
   // RCC->APB2ENR|=1<<4;    //ʹ��PORTA��ʱ��
    GPIOA->CRL&=0XFFF0FFFF;
    GPIOA->CRL|=0x00030000;
    GPIOA->ODR|=1<<4;      //PA4���1
    DS18B20_Rst1();
    return DS18B20_Check1();
}
//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250��
short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
    short tem;
    DS18B20_Start ();                    // ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert
    TL=DS18B20_Read_Byte(); // LSB
    TH=DS18B20_Read_Byte(); // MSB

    if(TH>7)
    {
        TH=~TH;
        TL=~TL;
        temp=0;//�¶�Ϊ��
    } else temp=1;//�¶�Ϊ��
    tem=TH; //��ø߰�λ
    tem<<=8;
    tem+=TL;//��õװ�λ
    tem=(float)tem*0.625;//ת��
    if(temp)return tem; //�����¶�ֵ
    else return -tem;
}
short DS18B20_Get_Temp1(void)
{
    u8 temp1;
    u8 TL1,TH1;
    short tem1;
    DS18B20_Start1 ();                    // ds1820 start convert
    DS18B20_Rst1();
    DS18B20_Check1();
    DS18B20_Write_Byte1(0xcc);// skip rom
    DS18B20_Write_Byte1(0xbe);// convert
    TL1=DS18B20_Read_Byte1(); // LSB
    TH1=DS18B20_Read_Byte1(); // MSB

    if(TH1>7)
    {
        TH1=~TH1;
        TL1=~TL1;
        temp1=0;//�¶�Ϊ��
    } else temp1=1;//�¶�Ϊ��
    tem1=TH1; //��ø߰�λ
    tem1<<=8;
    tem1+=TL1;//��õװ�λ
    tem1=(float)tem1*0.625;//ת��
    if(temp1)return tem1; //�����¶�ֵ
    else return -tem1;
}