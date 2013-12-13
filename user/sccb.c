/*
 * sccb.c
 *
 *  Created on: 2013-11-23
 *      Author: lrfy
 */

#include"sccb.h"
#include"delay.h"
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"

//初始化SCCB接口
//CHECK OK
void SCCB_Init(void)
{

 	GPIO_InitTypeDef  GPIO_conf;

 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	   //使能PA端口时钟

 	GPIO_conf.GPIO_Mode=GPIO_Mode_OUT;
 	GPIO_conf.GPIO_OType=GPIO_OType_PP ;
 	GPIO_conf.GPIO_Pin=GPIO_Pin_1;
 	GPIO_conf.GPIO_PuPd = GPIO_PuPd_UP;
 	GPIO_conf.GPIO_Speed=GPIO_Speed_100MHz;
 	GPIO_Init(GPIOA, &GPIO_conf);
 	GPIO_SetBits(GPIOA,GPIO_Pin_1);						       // SCL输出高

 	GPIO_conf.GPIO_Mode=GPIO_Mode_OUT;
 	GPIO_conf.GPIO_OType=GPIO_OType_PP ;
 	GPIO_conf.GPIO_Pin=GPIO_Pin_7;
 	GPIO_conf.GPIO_PuPd = GPIO_PuPd_UP;
 	GPIO_conf.GPIO_Speed=GPIO_Speed_100MHz;
 	GPIO_Init(GPIOA, &GPIO_conf);
 	GPIO_SetBits(GPIOA,GPIO_Pin_7);						 	   // SDA输出高

 	SCCB_SDA_OUT();
}

//SCCB起始信号
//当时钟为高的时候,数据线的高到低,为SCCB起始信号
//在激活状态下,SDA和SCL均为低电平
void SCCB_Start(void)
{
    SCCB_SDA_EN;     //数据线高电平
    SCCB_SCL_EN;	    //在时钟线高的时候数据线由高至低
    delay(50, uS);
    SCCB_SDA_CR;
    delay(50, uS);
    SCCB_SCL_CR;	    //数据线恢复低电平，单操作函数必要
}

//SCCB停止信号
//当时钟为高的时候,数据线的低到高,为SCCB停止信号
//空闲状况下,SDA,SCL均为高电平
void SCCB_Stop(void)
{
    SCCB_SDA_CR;
    delay(50, uS);
    SCCB_SCL_EN;
    delay(50, uS);
    SCCB_SDA_EN;
    delay(50, uS);
}
//产生NA信号
void SCCB_No_Ack(void)
{
	delay(50, uS);
	SCCB_SDA_EN;
	SCCB_SCL_EN;
	delay(50, uS);
	SCCB_SCL_CR;
	delay(50, uS);
	SCCB_SDA_CR;
	delay(50, uS);
}
//SCCB,写入一个字节
//返回值:0,成功;1,失败.
u8 SCCB_WR_Byte(u8 dat)
{
	u8 j,res;
	for(j=0;j<8;j++) //循环8次发送数据
	{
		if(dat&0x80)SCCB_SDA_EN;
		else SCCB_SDA_CR;
		dat<<=1;
		delay(50, uS);
		SCCB_SCL_EN;
		delay(50, uS);
		SCCB_SCL_CR;
	}
	SCCB_SDA_IN();		//设置SDA为输入
	delay(50, uS);
	SCCB_SCL_EN;			//接收第九位,以判断是否发送成功
	delay(50, uS);
	if(SCCB_READ_SDA)res=1;  //SDA=1发送失败，返回1
	else res=0;         //SDA=0发送成功，返回0
	SCCB_SCL_CR;
	SCCB_SDA_OUT();		//设置SDA为输出
	return res;
}
//SCCB 读取一个字节
//在SCL的上升沿,数据锁存
//返回值:读到的数据
u8 SCCB_RD_Byte(void)
{
	u8 temp=0,j;
	SCCB_SDA_IN();		//设置SDA为输入
	for(j=8;j>0;j--) 	//循环8次接收数据
	{
		delay(50, uS);
		SCCB_SCL_EN;
		temp=temp<<1;
		if(SCCB_READ_SDA)temp++;
		delay(50, uS);
		SCCB_SCL_CR;
	}
	SCCB_SDA_OUT();		//设置SDA为输出
	return temp;
}
//写寄存器
//返回值:0,成功;1,失败.
u8 SCCB_WR_Reg(u8 data,u8 reg)
{
	u8 res=0;
	SCCB_Start(); 					//启动SCCB传输
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//写器件ID
	delay(100, uS);
  	if(SCCB_WR_Byte(reg))res=1;		//写寄存器地址
  	delay(100, uS);
  	if(SCCB_WR_Byte(data))res=1; 	//写数据
  	SCCB_Stop();
  	return	res;
}

void WrCmos7670(u8 reg, u8 data)
{
	 SCCB_WR_Reg(data,reg);
}

//读寄存器
//返回值:读到的寄存器值
u8 SCCB_RD_Reg(u8 reg)
{
	u8 val=0;
	SCCB_Start(); 				//启动SCCB传输
	SCCB_WR_Byte(SCCB_ID);		//写器件ID
	delay(100, uS);
  	SCCB_WR_Byte(reg);			//写寄存器地址
  	delay(100, uS);
	SCCB_Stop();
	delay(100, uS);
	//设置寄存器地址后，才是读
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//发送读命令
	delay(100, uS);
  	val=SCCB_RD_Byte();		 	//读取数据
  	SCCB_No_Ack();
  	SCCB_Stop();
  	return val;
}
