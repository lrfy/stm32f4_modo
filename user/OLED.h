#ifndef __OLED_H
#define __OLED_H
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"
#define LCD_DC1 GPIO_SetBits(GPIOC, GPIO_Pin_3)
#define LCD_DC0 GPIO_ResetBits(GPIOC, GPIO_Pin_3)
#define LCD_RST1 GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define LCD_RST0 GPIO_ResetBits(GPIOC, GPIO_Pin_2)
#define LCD_SDA1 GPIO_SetBits(GPIOC, GPIO_Pin_1)
#define LCD_SDA0 GPIO_ResetBits(GPIOC, GPIO_Pin_1)
#define LCD_SCL1 GPIO_SetBits(GPIOC, GPIO_Pin_0)
#define LCD_SCL0 GPIO_ResetBits(GPIOC, GPIO_Pin_0)
#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel	    ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF 
#define X_WIDTH 128
#define Y_WIDTH 64
void OLED_IO_init(void);
void LCD_DLY_ms(unsigned int ms);
void LCD_WrDat(unsigned char dat);										
void LCD_WrCmd(unsigned char cmd);
void LCD_Set_Pos(unsigned char x, unsigned char y);
void LCD_Fill(unsigned char bmp_dat);
void LCD_CLS(void);
void OLED_Init(void);
void LCD_P6x8Str(unsigned char x, unsigned char y,char* ch);
void LCD_P8x16Str(unsigned char x, unsigned char y,char* ch);
void LCD_P8x6numstr(unsigned char x, unsigned char y,unsigned int num1);
void LCD_P8x6voltage2_5_2_0(unsigned char x, unsigned char y,float volnum);
void LCD_P8x6voltage1_5_2_0(unsigned char x, unsigned char y,float volnum);
void LCD_P8x6voltage3_3_2_0(unsigned char x, unsigned char y,float volnum);
void LCD_P6x8cha(unsigned char x, unsigned char y,unsigned char c);
void LCD_P6x8float(unsigned char x, unsigned char y,float c);
#endif
