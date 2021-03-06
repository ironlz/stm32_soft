#include "ds18b20.h"
#include "delay.h"
//2010/6/17

//复位DS18B20
void DS18B20_Rst(void)
{
    DS18B20_IO_OUT(); //SET PA0 OUTPUT
    DS18B20_DQ_OUT=0; //拉低DQ
    delay_us(750);    //拉低750us
    DS18B20_DQ_OUT=1; //DQ=1
    delay_us(15);     //15US
}
void DS18B20_Rst1(void)
{
    DS18B20_IO_OUT1(); //SET PA0 OUTPUT
    DS18B20_DQ_OUT1=0; //拉低DQ
    delay_us(750);    //拉低750us
    DS18B20_DQ_OUT1=1; //DQ=1
    delay_us(15);     //15US
}
//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
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
//从DS18B20读取一个位
//返回值：1/0
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
//从DS18B20读取一个字节
//返回值：读到的数据
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
//写一个字节到DS18B20
//dat：要写入的字节
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
//开始温度转换
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

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在
u8 DS18B20_Init(void)
{
    RCC->APB2ENR|=1<<4;    //使能PORTA口时钟
    GPIOC->CRL&=0XFF0FFFFF;
    GPIOC->CRL|=0x00300000;
    GPIOC->ODR|=1<<5;      //PC5输出1
    DS18B20_Rst();
    return DS18B20_Check();
}
u8 DS18B20_Init1(void)                    //我没改，只改了两处加1
{
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
   // RCC->APB2ENR|=1<<4;    //使能PORTA口时钟
    GPIOA->CRL&=0XFFF0FFFF;
    GPIOA->CRL|=0x00030000;
    GPIOA->ODR|=1<<4;      //PA4输出1
    DS18B20_Rst1();
    return DS18B20_Check1();
}
//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250）
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
        temp=0;//温度为负
    } else temp=1;//温度为正
    tem=TH; //获得高八位
    tem<<=8;
    tem+=TL;//获得底八位
    tem=(float)tem*0.625;//转换
    if(temp)return tem; //返回温度值
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
        temp1=0;//温度为负
    } else temp1=1;//温度为正
    tem1=TH1; //获得高八位
    tem1<<=8;
    tem1+=TL1;//获得底八位
    tem1=(float)tem1*0.625;//转换
    if(temp1)return tem1; //返回温度值
    else return -tem1;
}
