/*
 * delay.h
 *
 *  Created on: 2013-11-23
 *      Author: lrfy
 */

#ifndef DELAY_H_
#define DELAY_H_
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"

#define mS 1
#define uS 0

void delay_init(u8 HCLK);
void delay(u32 value, u16 unit);


#endif /* DELAY_H_ */
