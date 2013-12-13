/*
 * ov7670.h
 *
 *  Created on: 2013-11-23
 *      Author: lrfy
 */

#ifndef OV7670_H_
#define OV7670_H_

#include"stdio.h"
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"

#define		OV7670_VSYNC	    (GPIOA->IDR&0x0100)>>8 	//帧同步信号
#define		OV7670_WR_EN		GPIOA->ODR|=0x0020 	//写入FIFO使能 PA5
#define		OV7670_WR_CR		GPIOA->ODR&=0xFFDF	//
#define 	OV7670_OE_EN		GPIOA->ODR|=0x0008	//FIFO输出使能 PA3
#define 	OV7670_OE_CR		GPIOA->ODR&=0xFFF7	//
#define 	OV7670_WRST_EN		GPIOA->ODR|=0x0040	//写指针复位 PA6
#define 	OV7670_WRST_CR		GPIOA->ODR&=0xFFBF	//
#define 	OV7670_RRST_EN		GPIOA->ODR|=0x0010	//读指针复位 PA4
#define 	OV7670_RRST_CR		GPIOA->ODR&=0xFFEF	//读指针复位
#define 	OV7670_RCK_EN		GPIOA->ODR|=0x0004	//FIFO时钟控制 PA2
#define 	OV7670_RCK_CR		GPIOA->ODR&=0xFFFB	//FIFO时钟控制
//#define 	OV7670_RESET_EN	    PCout(14)	//复位端口（正常使用拉高）
//#define 	OV7670_RESET_CR	    PCout(14)	//复位端口（正常使用拉高）
//#define 	OV7670_PWDN_EN		PCout(15)	//功耗选择模式（正常使用拉低）
//#define 	OV7670_PWDN_CR		PCout(15)	//功耗选择模式（正常使用拉低）
#define 	OV7670_DATA 		(GPIOB->IDR&0x0FF0)>>4	//FIFO数据读取

u8 OV7670_Init(void);
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx, u16 sy, u16 width, u16 height);
void VSYNC_Exit_Init(void);

#endif /* OV7670_H_ */
