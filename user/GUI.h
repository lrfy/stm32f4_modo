/*
 * GUI.h
 *
 *  Created on: 2013-11-22
 *      Author: lrfy
 */

#ifndef GUI_H_
#define GUI_H_

#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"
#include"LCD_TFT_8080.h"

#define WHITE         	 0xFFFF	 //白色
#define BLACK         	 0x0000	 //黑色
#define BLUE         	 0x001F  //蓝色
#define BRED             0XF81F	 //
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色

void LCD_Clear(u16 Color);
void LCD_init();

#endif /* GUI_H_ */
