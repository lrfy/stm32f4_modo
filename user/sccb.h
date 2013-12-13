/*
 * sccb.h
 *
 *  Created on: 2013-11-23
 *      Author: lrfy
 */

#ifndef SCCB_H_
#define SCCB_H_
#include"stdio.h"
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"

#define SCCB_SDA_IN()  {GPIOA->MODER&=0XFFFF3FFF;GPIOA->MODER|=0X00000000;GPIOA->PUPDR&=0xFFFF3FFF;GPIOA->PUPDR|=0x00004000;}
#define SCCB_SDA_OUT() {GPIOA->MODER&=0XFFFF3FFF;GPIOA->MODER|=0X00004000;GPIOA->OTYPER&=0xFF7F;GPIOA->OTYPER|=0x0000;}

//IO操作函数
#define SCCB_SCL_EN    		GPIOA->ODR|=0x0002 	 	//SCL
#define SCCB_SDA_EN    		GPIOA->ODR|=0x0080 		//SDA
#define SCCB_SCL_CR    		GPIOA->ODR&=0xFFFD 	 	//SCL
#define SCCB_SDA_CR    		GPIOA->ODR&=0xFF7F 		//SDA

#define SCCB_READ_SDA    	(GPIOA->IDR&0x0080)>>7  		//输入SDA
#define SCCB_ID   			0X42  			//OV7670的ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);
void WrCmos7670(u8 reg, u8 data);

#endif /* SCCB_H_ */
