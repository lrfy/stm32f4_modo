/*
 * delay.c
 *
 *  Created on: 2013-11-23
 *      Author: lrfy
 */
#include"delay.h"
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"

static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数

void delay_init(u8 HCLK)
{
	SysTick->CTRL&=0xfffffffb;
	fac_us=HCLK/8;
	fac_ms=(u16)fac_us*1000;

}
void delay(u32 value, u16 unit)
{
	u32 temp;
	if(unit == mS)
	SysTick->LOAD=value*fac_ms;//时间加载(SysTick->LOAD为24bit)
	else
	SysTick->LOAD=value*fac_us;

	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL=0x01 ;          //开始倒数
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp& 0x01)&&!(temp&(1<<16)));//等待时间到达
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器
}


