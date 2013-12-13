/*
 * LCD_TFT_8080.c
 *
 *  Created on: 2013-11-22
 *      Author: lrfy
 */

#include"fsmc.h"
#include"delay.h"
#include"LCD_TFT_8080.h"
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"

unsigned int DeviceCode;

u16 LCD_ReadRAM(void)										   //read ram(must read two times)
{
	u16 dummy;
	LCD_WR_REG(0X22);
	dummy = *(vu16*)Bank1_LCD_D;
	return  *(vu16*)Bank1_LCD_D;
}

void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(R34);
}

void LCD_SetPoint(u16 x, u16 y, u16 color)
{
	LCD_SetCursor(x,y);
	LCD_WR_REG(0x22);
	LCD_WR_DATA(color);
}

//LCD画圆
void LCD_Circle(u16 Xpos, u16 Ypos, u16 Radius, u16 color)
{
  	s32  Dec;
  	u32  CurX;
  	u32  CurY;

  	CurX=0;
  	CurY=Radius;
  	Dec=3-(Radius<<1);
  	while(CurX<=CurY)
  	{
		LCD_SetPoint(Xpos+CurX,Ypos+CurY,color);
		LCD_SetPoint(Xpos+CurX,Ypos-CurY,color);
		LCD_SetPoint(Xpos-CurX,Ypos+CurY,color);
		LCD_SetPoint(Xpos-CurX,Ypos-CurY,color);
		LCD_SetPoint(Xpos+CurY,Ypos+CurX,color);
		LCD_SetPoint(Xpos+CurY,Ypos-CurX,color);
		LCD_SetPoint(Xpos-CurY,Ypos+CurX,color);
		LCD_SetPoint(Xpos-CurY,Ypos-CurX,color);
		if(Dec<0)Dec+=(CurX<<2)+6;
	    else
	    {
	      	Dec+=((CurX-CurY)<<2)+10;
	      	CurY--;
	    }
	    CurX++;
  	}
}

//LCD画任意斜线
void LCD_Line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;

	delta_x=x2-x1; //计算坐标增量
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向
	else if(delta_x==0)incx=0;//垂直线
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;//水平线
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )//画线输出
	{
		LCD_SetPoint(uRow,uCol,color);//画点
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

//LCD画长方形
void LCD_Rect(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	LCD_Line(x1,y1,x2,y1,color);
	LCD_Line(x1,y1,x1,y2,color);
	LCD_Line(x1,y2,x2,y2,color);
	LCD_Line(x2,y1,x2,y2,color);
}

//LCD读点
u16 LCD_GetPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);
	return BGR565toRGB565(LCD_ReadRAM());
}

//LCD开启显示
void LCD_DisplayOn(void)
{
	LCD_WriteReg(R7, 0x0173); //26万色显示开启
}
//LCD关闭显示
void LCD_DisplayOff(void)
{
	LCD_WriteReg(R7, 0x0);//关闭显示
}
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
		LCD_WriteReg(R32, Ypos);
		LCD_WriteReg(R33, 319-Xpos);

}
/////////////////////////////////////////////////////////初始化//////////////////////////////////////////////


/////////////////////////////////////////////////////////初始化/////////////////////////////////////////////
void LCD_Init_color(void)
{
		delay(50, mS); // delay 50 ms
		LCD_WriteReg(0x0000,0x0001);
		delay(50, mS); // delay 50 ms
		//DeviceCode = LCD_ReadReg(0x0000);
  		LCD_WriteReg(0x00e7,0x0010);
        LCD_WriteReg(0x0000,0x0001);//开启内部时钟
        LCD_WriteReg(0x0001,0x0100);
        LCD_WriteReg(0x0002,0x0700);//电源开启
		//LCD_WriteReg(0x0003,(1<<3)|(1<<4) ); 	//65K  RGB
		//DRIVE TABLE(寄存器 03H)
		//BIT3=AM BIT4:5=ID0:1
		//AM ID0 ID1   FUNCATION
		// 0  0   0	   R->L D->U
		// 1  0   0	   D->U	R->L
		// 0  1   0	   L->R D->U
		// 1  1   0    D->U	L->R
		// 0  0   1	   R->L U->D
		// 1  0   1    U->D	R->L
		// 0  1   1    L->R U->D 正常就用这个.
		// 1  1   1	   U->D	L->R
        LCD_WriteReg(0x0003,(1<<12)|(3<<4)|(0<<3) );//65K
        LCD_WriteReg(0x0004,0x0000);
        LCD_WriteReg(0x0008,0x0207);
        LCD_WriteReg(0x0009,0x0000);
        LCD_WriteReg(0x000a,0x0000);//display setting
        LCD_WriteReg(0x000c,0x0001);//display setting
        LCD_WriteReg(0x000d,0x0000);//0f3c
        LCD_WriteReg(0x000f,0x0000);
		//电源配置
        LCD_WriteReg(0x0010,0x0000);
        LCD_WriteReg(0x0011,0x0007);
        LCD_WriteReg(0x0012,0x0000);
        LCD_WriteReg(0x0013,0x0000);
        delay(50, mS);
        LCD_WriteReg(0x0010,0x1590);
        LCD_WriteReg(0x0011,0x0227);
        delay(50, mS);
        LCD_WriteReg(0x0012,0x009c);
        delay(50, mS);
        LCD_WriteReg(0x0013,0x1900);
        LCD_WriteReg(0x0029,0x0023);
        LCD_WriteReg(0x002b,0x000e);
        delay(50, mS);
        LCD_WriteReg(0x0020,0x0000);
        LCD_WriteReg(0x0021,0x013f);
        delay(50, mS);
		//伽马校正
        LCD_WriteReg(0x0030,0x0007);
        LCD_WriteReg(0x0031,0x0707);
        LCD_WriteReg(0x0032,0x0006);
        LCD_WriteReg(0x0035,0x0704);
        LCD_WriteReg(0x0036,0x1f04);
        LCD_WriteReg(0x0037,0x0004);
        LCD_WriteReg(0x0038,0x0000);
        LCD_WriteReg(0x0039,0x0706);
        LCD_WriteReg(0x003c,0x0701);
        LCD_WriteReg(0x003d,0x000f);
        delay(50, mS);
        LCD_WriteReg(0x0050,0x0000); //水平GRAM起始位置
        LCD_WriteReg(0x0051,0x00ef); //水平GRAM终止位置
        LCD_WriteReg(0x0052,0x0000); //垂直GRAM起始位置
        LCD_WriteReg(0x0053,0x013f); //垂直GRAM终止位置

        LCD_WriteReg(0x0060,0xa700);
        LCD_WriteReg(0x0061,0x0001);
        LCD_WriteReg(0x006a,0x0000);
        LCD_WriteReg(0x0080,0x0000);
        LCD_WriteReg(0x0081,0x0000);
        LCD_WriteReg(0x0082,0x0000);
        LCD_WriteReg(0x0083,0x0000);
        LCD_WriteReg(0x0084,0x0000);
        LCD_WriteReg(0x0085,0x0000);

        LCD_WriteReg(0x0090,0x0010);
        LCD_WriteReg(0x0092,0x0000);
        LCD_WriteReg(0x0093,0x0003);
        LCD_WriteReg(0x0095,0x0110);
        LCD_WriteReg(0x0097,0x0000);
        LCD_WriteReg(0x0098,0x0000);
        LCD_WriteReg(0x03,0x1018);
        //开启显示设置
        LCD_WriteReg(0x0007,0x0133);
        LCD_WriteReg(0x0020,0x0000);
        LCD_WriteReg(0x0021,0x013f);

}

