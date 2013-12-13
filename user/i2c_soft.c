/*
 * i2c_soft.c
 *
 *  Created on: 2013-12-6
 *      Author: lrfy
 */
#include"i2c_soft.h"
#include"delay.h"
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"



void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_conf;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_conf.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_conf.GPIO_OType=GPIO_OType_PP ;
	GPIO_conf.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_conf.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_conf);
	GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
}
//产生 IIC 起始信号
static void IIC_Start(void)
{
	SDA_OUT();//SDA 线输出
	IIC_SDA_EN;
	IIC_SCL_EN;
	delay(10, uS);
	IIC_SDA_CR;//START:when CLK is high,DATA change form high to low
	delay(10, uS);
	IIC_SCL_CR;//钳住 I2C 总线,准备发送或接收数据
}

//产生 IIC 停止信号
static void IIC_Stop(void)
{
	SDA_OUT();//sda 线输出
	IIC_SCL_CR;
	IIC_SDA_CR;//STOP:when CLK is high DATA change form low to high
	delay(10, uS);
	IIC_SCL_EN;
	IIC_SDA_EN;//发送 I2C 总线结束信号

	delay(10, uS);
}

//等待应答信号到来
//返回值:1,接收应答失败
//0,接收应答成功
static u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();
	//SDA 设置为输入
	IIC_SDA_EN;delay(6, uS);
	IIC_SCL_EN;delay(6, uS);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_CR;//时钟输出 0
	return 0;
}

//产生 ACK 应答
static void IIC_Ack(void)
{
	IIC_SCL_CR;
	SDA_OUT();
	IIC_SDA_CR;
	delay(10, uS);
	IIC_SCL_EN;
	delay(10, uS);
	IIC_SCL_CR;
}

//不产生 ACK 应答
static void IIC_NAck(void)
{
	IIC_SCL_CR;
	SDA_OUT();
	IIC_SDA_EN;
	delay(10, uS);
	IIC_SCL_EN;
	delay(10, uS);
	IIC_SCL_CR;
}

//IIC 发送一个字节
//返回从机有无应答
//1,有应答
//0,无应答
static void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL_CR;//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{
		if(txd&0x80) IIC_SDA_EN;
		else IIC_SDA_CR;
		txd<<=1;
		delay(10, uS);
		IIC_SCL_EN;
		delay(10, uS);
		IIC_SCL_CR;
		delay(10, uS);
	}
}

//读 1 个字节,ack=1 时,发送 ACK,ack=0,发送 nACK
static u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA 设置为输入
	for(i=0;i<8;i++ )
	{
	IIC_SCL_CR;
	delay(10, uS);
	IIC_SCL_EN;
	receive<<=1;
	if(READ_SDA)receive++;
	delay(5, uS);
	}
	if (!ack)
	IIC_NAck();//发送 nACK
	else
	IIC_Ack(); //发送 ACK
	return receive;
}

u8 KS103_ReadOneByte(u8 address, u8 reg)
{
	u8 temp=0;
	IIC_Start();
	IIC_Send_Byte(address);
	//发送低地址
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	//发送低地址
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(address + 1);
	//进入接收模式
	IIC_Wait_Ack();
	delay(50, uS);
	//增加此代码通信成功!
	temp=IIC_Read_Byte(0);
	//读寄存器 3
	IIC_Stop();//产生一个停止条件
	return temp;
}

void KS103_WriteOneByte(u8 address,u8 reg,u8 command)
{
	IIC_Start();
	IIC_Send_Byte(address);
	//发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);//发送高地址
	IIC_Wait_Ack();
	IIC_Send_Byte(command);
	//发送低地址
	IIC_Wait_Ack();
	IIC_Stop();//产生一个停止条件
}

/*
int main(void)
{
	u16 range;
	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);
	//延时初始化
	uart_init(72,9600); //串口 1 初始化
	while(1)
	{
		KS103_WriteOneByte(0XE8,0X02,0XB0);
		delay_ms(80);
		range = KS103_ReadOneByte(0xe8, 0x02);
		range <<= 8;
		range += KS103_ReadOneByte(0xe8, 0x03);
	}
}*/


