/*
 * i2c_soft.h
 *
 *  Created on: 2013-12-6
 *      Author: lrfy
 */

#ifndef I2C_SOFT_H_
#define I2C_SOFT_H_
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"

#define SDA_IN()  {GPIOB->MODER&=0XFFFFFFFC;GPIOB->MODER|=0X00000000;GPIOB->PUPDR&=0xFFFFFFFC;GPIOB->PUPDR|=0x00000001;}
#define SDA_OUT() {GPIOB->MODER&=0XFFFFFFFC;GPIOB->MODER|=0X00000001;GPIOB->OTYPER&=0xFFFE;GPIOB->OTYPER|=0x0000;}

#define IIC_SCL_EN GPIOB->ODR|=0x0002
#define IIC_SCL_CR GPIOB->ODR&=0xFFFD
#define IIC_SDA_EN GPIOB->ODR|=0x0001
#define IIC_SDA_CR GPIOB->ODR&=0xFFFE

#define READ_SDA GPIOB->IDR&0x0001

void IIC_Init(void);
unsigned char KS103_ReadOneByte(u8 address, u8 reg);
void KS103_WriteOneByte(u8 address,u8 reg,u8 command);

#endif /* I2C_SOFT_H_ */
