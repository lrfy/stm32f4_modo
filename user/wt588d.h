/*
 * wt588d.h
 *
 *  Created on: 2013-12-6
 *      Author: lrfy
 */

#ifndef WT588D_H_
#define WT588D_H_
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"

#define  volume_1 		0xe0
#define  volume_2 		0xe1
#define  volume_3 		0xe2
#define  volume_4 		0xe3
#define  volume_5 		0xe4
#define  volume_6	 	0xe5
#define  volume_7 		0xe6
#define  volume_8 		0xe7
#define  circle_play	0xf2
#define  stop_play		0xfe
/*语音地址与数字一一对应		*/
/*比如：播放第0段语音，发送0x00	*/
#define WT588D_DATA_EN GPIOC->ODR|=0x0010
#define WT588D_DATA_CR GPIOC->ODR&=0xFFEF
#define WT588D_RST_EN  GPIOC->ODR|=0x0020
#define WT588D_RST_CR  GPIOC->ODR&=0xFFDF

void WT588D_Init(void);
void send_command(u8 addr);



#endif /* WT588D_H_ */
