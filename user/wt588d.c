/*
 * wt588d.c
 *
 *  Created on: 2013-12-6
 *      Author: lrfy
 */
#include"wt588d.h"
#include"delay.h"
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"

#define  RES	PEout(3)
#define  DATA	PEout(4)

//语音模块初始化：配置IO口
void WT588D_Init(void)
{
	GPIO_InitTypeDef GPIO_conf;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	GPIO_conf.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_conf.GPIO_OType=GPIO_OType_PP ;
	GPIO_conf.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_conf.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_conf);
}

static void send_high(void)
{
	 delay(600, uS);
	 WT588D_DATA_CR;
	 delay(300, uS);
}

static void send_low(void)
{
	 delay(300, uS);
	 WT588D_DATA_CR;
	 delay(600, uS);
}

//发送指令
void send_command(u8 addr)
{
	 u8 i = 0;
	 WT588D_RST_CR;
	 delay(3, mS);
	 WT588D_RST_EN;
	 delay(17, mS);
	 WT588D_DATA_CR;
	 delay(17, mS);
	 for(i = 0; i < 8; i++)
	 {
	 	WT588D_DATA_EN;
		if(addr & 0x01)send_high();
		else send_low();
		addr >>= 1;
	 }
	 WT588D_DATA_EN;
}

