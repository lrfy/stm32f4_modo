/*
 * GUI.c
 *
 *  Created on: 2013-11-22
 *      Author: lrfy
 */
#include"GUI.h"
#include"LCD_TFT_8080.h"
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"

void LCD_init()
{
	LCD_Init_color();
}

void LCD_Clear(u16 Color)
{
	u32 index=0;
	LCD_SetCursor(0x00,0x0000);//设置光标位置
	LCD_WR_REG(R34);
	for(index=0;index<76800;index++)
	{
		LCD_WR_DATA(Color);
	}
}





