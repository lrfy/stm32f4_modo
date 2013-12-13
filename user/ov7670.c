/*
 * ov7670.c
 *
 *  Created on: 2013-11-23
 *      Author: lrfy
 */
#include"ov7670.h"
#include"sccb.h"
#include"stdio.h"
#include"delay.h"
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"

void set_Cmos7670reg(void)
{

	WrCmos7670(0x3a, 0x04);
	WrCmos7670(0x40, 0xd0);
	WrCmos7670(0x12, 0x14);
	WrCmos7670(0x32, 0x80);
	WrCmos7670(0x17, 0x16);
	WrCmos7670(0x18, 0x04);
	WrCmos7670(0x19, 0x02);
	WrCmos7670(0x1a, 0x7b);
	WrCmos7670(0x03, 0x06);
	WrCmos7670(0x0c, 0x00);
	WrCmos7670(0x3e, 0x00);
	WrCmos7670(0x70, 0x3a);
	WrCmos7670(0x71, 0x35);
	WrCmos7670(0x72, 0x11);
	WrCmos7670(0x73, 0x00);
	WrCmos7670(0xa2, 0x02);
	WrCmos7670(0x11, 0x00);

	WrCmos7670(0x7a, 0x20);
	WrCmos7670(0x7b, 0x1c);
	WrCmos7670(0x7c, 0x28);
	WrCmos7670(0x7d, 0x3c);
	WrCmos7670(0x7e, 0x55);
	WrCmos7670(0x7f, 0x68);
	WrCmos7670(0x80, 0x76);
	WrCmos7670(0x81, 0x80);
	WrCmos7670(0x82, 0x88);
	WrCmos7670(0x83, 0x8f);
	WrCmos7670(0x84, 0x96);
	WrCmos7670(0x85, 0xa3);
	WrCmos7670(0x86, 0xaf);
	WrCmos7670(0x87, 0xc4);
	WrCmos7670(0x88, 0xd7);
	WrCmos7670(0x89, 0xe8);

	WrCmos7670(0x13, 0xe0);
	WrCmos7670(0x00, 0x00);

	WrCmos7670(0x10, 0x00);
	WrCmos7670(0x0d, 0x00);
	WrCmos7670(0x14, 0x28);
	WrCmos7670(0xa5, 0x05);
	WrCmos7670(0xab, 0x07);
	WrCmos7670(0x24, 0x75);
	WrCmos7670(0x25, 0x63);
	WrCmos7670(0x26, 0xA5);
	WrCmos7670(0x9f, 0x78);
	WrCmos7670(0xa0, 0x68);
	WrCmos7670(0xa1, 0x03);
	WrCmos7670(0xa6, 0xdf);
	WrCmos7670(0xa7, 0xdf);
	WrCmos7670(0xa8, 0xf0);
	WrCmos7670(0xa9, 0x90);
	WrCmos7670(0xaa, 0x94);
	WrCmos7670(0x13, 0xe5);

	WrCmos7670(0x0e, 0x61);
	WrCmos7670(0x0f, 0x4b);
	WrCmos7670(0x16, 0x02);
	WrCmos7670(0x1e, 0x37);
	WrCmos7670(0x21, 0x02);
	WrCmos7670(0x22, 0x91);
	WrCmos7670(0x29, 0x07);
	WrCmos7670(0x33, 0x0b);
	WrCmos7670(0x35, 0x0b);
	WrCmos7670(0x37, 0x1d);
	WrCmos7670(0x38, 0x71);
	WrCmos7670(0x39, 0x2a);
	WrCmos7670(0x3c, 0x78);
	WrCmos7670(0x4d, 0x40);
	WrCmos7670(0x4e, 0x20);
	WrCmos7670(0x69, 0x00);
	WrCmos7670(0x6b, 0x60);
	WrCmos7670(0x74, 0x19);
	WrCmos7670(0x8d, 0x4f);
	WrCmos7670(0x8e, 0x00);
	WrCmos7670(0x8f, 0x00);
	WrCmos7670(0x90, 0x00);
	WrCmos7670(0x91, 0x00);
	WrCmos7670(0x92, 0x00);
	WrCmos7670(0x96, 0x00);
	WrCmos7670(0x9a, 0x80);
	WrCmos7670(0xb0, 0x84);
	WrCmos7670(0xb1, 0x0c);
	WrCmos7670(0xb2, 0x0e);
	WrCmos7670(0xb3, 0x82);
	WrCmos7670(0xb8, 0x0a);



	WrCmos7670(0x43, 0x14);
	WrCmos7670(0x44, 0xf0);
	WrCmos7670(0x45, 0x34);
	WrCmos7670(0x46, 0x58);
	WrCmos7670(0x47, 0x28);
	WrCmos7670(0x48, 0x3a);
	WrCmos7670(0x59, 0x88);
	WrCmos7670(0x5a, 0x88);
	WrCmos7670(0x5b, 0x44);
	WrCmos7670(0x5c, 0x67);
	WrCmos7670(0x5d, 0x49);
	WrCmos7670(0x5e, 0x0e);
	WrCmos7670(0x64, 0x04);
	WrCmos7670(0x65, 0x20);
	WrCmos7670(0x66, 0x05);
	WrCmos7670(0x94, 0x04);
	WrCmos7670(0x95, 0x08);
	WrCmos7670(0x6c, 0x0a);
	WrCmos7670(0x6d, 0x55);
	WrCmos7670(0x6e, 0x11);
	WrCmos7670(0x6f, 0x9f);
	WrCmos7670(0x6a, 0x40);
	WrCmos7670(0x01, 0x40);
	WrCmos7670(0x02, 0x40);
	WrCmos7670(0x13, 0xe7);
	WrCmos7670(0x15, 0x00);


	WrCmos7670(0x4f, 0x80);
	WrCmos7670(0x50, 0x80);
	WrCmos7670(0x51, 0x00);
	WrCmos7670(0x52, 0x22);
	WrCmos7670(0x53, 0x5e);
	WrCmos7670(0x54, 0x80);
	WrCmos7670(0x58, 0x9e);

	WrCmos7670(0x41, 0x08);
	WrCmos7670(0x3f, 0x00);
	WrCmos7670(0x75, 0x05);
	WrCmos7670(0x76, 0xe1);
	WrCmos7670(0x4c, 0x00);
	WrCmos7670(0x77, 0x01);
	WrCmos7670(0x3d, 0xc2);
	WrCmos7670(0x4b, 0x09);
	WrCmos7670(0xc9, 0x60);
	WrCmos7670(0x41, 0x38);
	WrCmos7670(0x56, 0x40);

	WrCmos7670(0x34, 0x11);
	WrCmos7670(0x3b, 0x02);

	WrCmos7670(0xa4, 0x89);
	WrCmos7670(0x96, 0x00);
	WrCmos7670(0x97, 0x30);
	WrCmos7670(0x98, 0x20);
	WrCmos7670(0x99, 0x30);
	WrCmos7670(0x9a, 0x84);
	WrCmos7670(0x9b, 0x29);
	WrCmos7670(0x9c, 0x03);
	WrCmos7670(0x9d, 0x4c);
	WrCmos7670(0x9e, 0x3f);
	WrCmos7670(0x78, 0x04);

	WrCmos7670(0x79, 0x01);
	WrCmos7670(0xc8, 0xf0);
	WrCmos7670(0x79, 0x0f);
	WrCmos7670(0xc8, 0x00);
	WrCmos7670(0x79, 0x10);
	WrCmos7670(0xc8, 0x7e);
	WrCmos7670(0x79, 0x0a);
	WrCmos7670(0xc8, 0x80);
	WrCmos7670(0x79, 0x0b);
	WrCmos7670(0xc8, 0x01);
	WrCmos7670(0x79, 0x0c);
	WrCmos7670(0xc8, 0x0f);
	WrCmos7670(0x79, 0x0d);
	WrCmos7670(0xc8, 0x20);
	WrCmos7670(0x79, 0x09);
	WrCmos7670(0xc8, 0x80);
	WrCmos7670(0x79, 0x02);
	WrCmos7670(0xc8, 0xc0);
	WrCmos7670(0x79, 0x03);
	WrCmos7670(0xc8, 0x40);
	WrCmos7670(0x79, 0x05);
	WrCmos7670(0xc8, 0x30);
	WrCmos7670(0x79, 0x26);
	WrCmos7670(0x09, 0x00);


}
//初始化寄存器序列及其对应的值
/*const u8 ov7670_init_reg_tbl[][2]=
{
     以下为OV7670 QVGA RGB565参数
  	{0x3a, 0x04},//
	{0x40, 0x10},
	{0x12, 0x14},//QVGA,RGB输出

	//输出窗口设置
	{0x32, 0x80},
	{0x17, 0x16},
	{0x18, 0x04},//5
	{0x19, 0x02},
	{0x1a, 0x7a},//0x7a,
 	{0x03, 0x0a},//0x0a,


	{0x0c, 0x0c},
	{0x15, 0x00},
	{0x3e, 0x00},//10
	{0x70, 0x00},
	{0x71, 0x01},
	{0x72, 0x11},
	{0x73, 0x09},//

	{0xa2, 0x02},//15
	{0x11, 0x00},//时钟分频设置,0,不分频.
	{0x7a, 0x20},
	{0x7b, 0x1c},
	{0x7c, 0x28},

	{0x7d, 0x3c},//20
	{0x7e, 0x55},
	{0x7f, 0x68},
	{0x80, 0x76},
	{0x81, 0x80},

	{0x82, 0x88},
	{0x83, 0x8f},
	{0x84, 0x96},
	{0x85, 0xa3},
	{0x86, 0xaf},

	{0x87, 0xc4},//30
	{0x88, 0xd7},
	{0x89, 0xe8},
	{0x13, 0xe0},
	{0x00, 0x00},//AGC

	{0x10, 0x00},
	{0x0d, 0x00},
	{0x14, 0x20},//0x38, limit the max gain
	{0xa5, 0x05},
	{0xab, 0x07},

	{0x24, 0x75},//40
	{0x25, 0x63},
	{0x26, 0xA5},
	{0x9f, 0x78},
	{0xa0, 0x68},

	{0xa1, 0x03},//0x0b,
	{0xa6, 0xdf},//0xd8,
	{0xa7, 0xdf},//0xd8,
	{0xa8, 0xf0},
	{0xa9, 0x90},

	{0xaa, 0x94},//50
	{0x13, 0xe5},
	{0x0e, 0x61},
	{0x0f, 0x4b},
	{0x16, 0x02},

	{0x1e, 0x27},//图像输出镜像控制.0x07,
	{0x21, 0x02},
	{0x22, 0x91},
	{0x29, 0x07},
	{0x33, 0x0b},

	{0x35, 0x0b},//60
	{0x37, 0x1d},
	{0x38, 0x71},
	{0x39, 0x2a},
	{0x3c, 0x78},

	{0x4d, 0x40},
	{0x4e, 0x20},
	{0x69, 0x5d},
	{0x6b, 0x40},//PLL*4=48Mhz
	{0x74, 0x19},
	{0x8d, 0x4f},

	{0x8e, 0x00},//70
	{0x8f, 0x00},
	{0x90, 0x00},
	{0x91, 0x00},
	{0x92, 0x00},//0x19,//0x66

	{0x96, 0x00},
	{0x9a, 0x80},
	{0xb0, 0x84},
	{0xb1, 0x0c},
	{0xb2, 0x0e},

	{0xb3, 0x82},//80
	{0xb8, 0x0a},
	{0x43, 0x14},
	{0x44, 0xf0},
	{0x45, 0x34},

	{0x46, 0x58},
	{0x47, 0x28},
	{0x48, 0x3a},
	{0x59, 0x88},
	{0x5a, 0x88},

	{0x5b, 0x44},//90
	{0x5c, 0x67},
	{0x5d, 0x49},
	{0x5e, 0x0e},
	{0x64, 0x04},
	{0x65, 0x20},

	{0x66, 0x05},
	{0x94, 0x04},
	{0x95, 0x08},
	{0x6c, 0x0a},
	{0x6d, 0x55},


	{0x4f, 0x80},
	{0x50, 0x80},
	{0x51, 0x00},
	{0x52, 0x22},
	{0x53, 0x5e},
	{0x54, 0x80},

	//{0x54, 0x40},//110


	{0x09, 0x03},//驱动能力最大

	{0x6e, 0x11},//100
	{0x6f, 0x9f},//0x9e for advance AWB
    {0x55, 0x00},//亮度
    {0x56, 0x40},//对比度
    {0x57, 0x80},//0x40,  change according to Jim's request
}; */

u8 OV7670_Init(void)
{
	GPIO_InitTypeDef  GPIO_conf;
	NVIC_InitTypeDef  NVIC_conf;
	EXTI_InitTypeDef  EXTI_conf;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
															   //使能相关端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);      //使能SYSCFG时钟
	GPIO_conf.GPIO_Mode=GPIO_Mode_IN;
	GPIO_conf.GPIO_OType=GPIO_OType_PP;
	GPIO_conf.GPIO_Pin=GPIO_Pin_8;
	GPIO_conf.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_conf.GPIO_Speed=GPIO_Speed_100MHz;
 	GPIO_Init(GPIOA, &GPIO_conf);

 	GPIO_conf.GPIO_Mode=GPIO_Mode_OUT;
 	GPIO_conf.GPIO_OType=GPIO_OType_PP ;
 	GPIO_conf.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
 	GPIO_conf.GPIO_PuPd = GPIO_PuPd_UP;
 	GPIO_conf.GPIO_Speed=GPIO_Speed_100MHz;
 	GPIO_Init(GPIOA, &GPIO_conf);

 	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource8);


 	EXTI_conf.EXTI_Line = EXTI_Line8;				    	   //设置外部中断为2线
 	EXTI_conf.EXTI_Mode = EXTI_Mode_Interrupt;		    	   //设置模式为中断模式 而非事件模式Event
 	EXTI_conf.EXTI_Trigger = EXTI_Trigger_Falling;	    	   //设置中断触发方式 Falling 还有其他方式Rising和Falling_Rising
 	EXTI_conf.EXTI_LineCmd = ENABLE;						   //使能
 	EXTI_Init(&EXTI_conf);

 	GPIO_conf.GPIO_Mode=GPIO_Mode_IN;
 	GPIO_conf.GPIO_OType=GPIO_OType_PP ;
 	GPIO_conf.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
 	GPIO_conf.GPIO_PuPd = GPIO_PuPd_NOPULL;
 	GPIO_conf.GPIO_Speed=GPIO_Speed_100MHz;
 	GPIO_Init(GPIOB, &GPIO_conf);

 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	            //设置分组
 	NVIC_conf.NVIC_IRQChannel = EXTI9_5_IRQn;             //设置2线中断
 	NVIC_conf.NVIC_IRQChannelPreemptionPriority = 0;	//设置抢占优先级为0
 	NVIC_conf.NVIC_IRQChannelSubPriority = 0;           //设置响应优先级为0
 	NVIC_conf.NVIC_IRQChannelCmd = ENABLE;	            //使能
 	NVIC_Init(&NVIC_conf);

 	EXTI->IMR&=~(1<<8);
 	EXTI->EMR&=~(1<<8);

	OV7670_RRST_EN;
	OV7670_OE_EN;
	OV7670_WR_EN;
	OV7670_RCK_EN;
	OV7670_WRST_EN;

	//初始化IIC											   //SCCB与IIC兼容
	if(SCCB_WR_Reg(0x08,0x12))return 1;						   //复位SCCB
	delay(50, mS);


	//初始化序列
	set_Cmos7670reg();

	return 0;
}

void OV7670_Light_Mode(u8 mode)
{
	u8 reg13val=0XE7;//默认就是设置为自动白平衡
	u8 reg01val=0;
	u8 reg02val=0;
	switch(mode)
	{
		case 1://sunny
			reg13val=0XE5;
			reg01val=0X5A;
			reg02val=0X5C;
			break;
		case 2://cloudy
			reg13val=0XE5;
			reg01val=0X58;
			reg02val=0X60;
			break;
		case 3://office
			reg13val=0XE5;
			reg01val=0X84;
			reg02val=0X4c;
			break;
		case 4://home
			reg13val=0XE5;
			reg01val=0X96;
			reg02val=0X40;
			break;
	}
	SCCB_WR_Reg(reg13val,0x13);//COM8设置
	SCCB_WR_Reg(reg01val,0x01);//AWB蓝色通道增益
	SCCB_WR_Reg(reg02val,0x02);//AWB红色通道增益
}
//色度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Color_Saturation(u8 sat)
{
	u8 reg4f5054val=0X80;//默认就是sat=2,即不调节色度的设置
 	u8 reg52val=0X22;
	u8 reg53val=0X5E;
 	switch(sat)
	{
		case 0://-2
			reg4f5054val=0X40;
			reg52val=0X11;
			reg53val=0X2F;
			break;
		case 1://-1
			reg4f5054val=0X66;
			reg52val=0X1B;
			reg53val=0X4B;
			break;
		case 3://1
			reg4f5054val=0X99;
			reg52val=0X28;
			reg53val=0X71;
			break;
		case 4://2
			reg4f5054val=0XC0;
			reg52val=0X33;
			reg53val=0X8D;
			break;
	}
	SCCB_WR_Reg(reg4f5054val,0x4f);	//色彩矩阵系数1
	SCCB_WR_Reg(reg4f5054val,0X50);	//色彩矩阵系数2
	SCCB_WR_Reg(0X00,0X51);			//色彩矩阵系数3
	SCCB_WR_Reg(reg52val,0X52);		//色彩矩阵系数4
	SCCB_WR_Reg(reg53val,0X53);		//色彩矩阵系数5
	SCCB_WR_Reg(reg4f5054val,0X54);	//色彩矩阵系数6
	SCCB_WR_Reg(0X9E,0X58);			//MTXS
}
//亮度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Brightness(u8 bright)
{
	u8 reg55val=0X00;//默认就是bright=2
  	switch(bright)
	{
		case 0://-2
			reg55val=0XB0;
			break;
		case 1://-1
			reg55val=0X98;
			break;
		case 3://1
			reg55val=0X18;
			break;
		case 4://2
			reg55val=0X30;
			break;
	}
	SCCB_WR_Reg(reg55val,0X55);	//亮度调节
}
//对比度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Contrast(u8 contrast)
{
	u8 reg56val=0X40;//默认就是contrast=2
  	switch(contrast)
	{
		case 0://-2
			reg56val=0X30;
			break;
		case 1://-1
			reg56val=0X38;
			break;
		case 3://1
			reg56val=0X50;
			break;
		case 4://2
			reg56val=0X60;
			break;
	}
	SCCB_WR_Reg(reg56val,0X56);	//对比度调节
}
//特效设置
//0:普通模式
//1,负片
//2,黑白
//3,偏红色
//4,偏绿色
//5,偏蓝色
//6,复古
void OV7670_Special_Effects(u8 eft)
{
	u8 reg3aval=0X04;//默认为普通模式
	u8 reg67val=0XC0;
	u8 reg68val=0X80;
	switch(eft)
	{
		case 1://负片
			reg3aval=0X24;
			reg67val=0X80;
			reg68val=0X80;
			break;
		case 2://黑白
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0X80;
			break;
		case 3://偏红色
			reg3aval=0X14;
			reg67val=0Xc0;
			reg68val=0X80;
			break;
		case 4://偏绿色
			reg3aval=0X14;
			reg67val=0X40;
			reg68val=0X40;
			break;
		case 5://偏蓝色
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0XC0;
			break;
		case 6://复古
			reg3aval=0X14;
			reg67val=0XA0;
			reg68val=0X40;
			break;
	}
	SCCB_WR_Reg(reg3aval,0X3A);//TSLB设置
	SCCB_WR_Reg(reg67val,0X68);//MANU,手动U值
	SCCB_WR_Reg(reg68val,0X67);//MANV,手动V值
}
//设置图像输出窗口
//对QVGA设置。
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height)
{
	u16 endx;
	u16 endy;
	u8 temp;
	endx=sx+width*2;							//V*2
 	endy=sy+height*2;
	if(endy>784)endy-=784;
	temp=SCCB_RD_Reg(0X03);				//读取Vref之前的值
	temp&=0XF0;
	temp|=((endx&0X03)<<2)|(sx&0X03);
	SCCB_WR_Reg(temp,0X03);				//设置Vref的start和end的最低2位
	SCCB_WR_Reg(sx>>2,0X19);			//设置Vref的start高8位
	SCCB_WR_Reg(endx>>2,0X1A);			//设置Vref的end的高8位

	temp=SCCB_RD_Reg(0X32);				//读取Href之前的值
	temp&=0XC0;
	temp|=((endy&0X07)<<3)|(sy&0X07);
	SCCB_WR_Reg(sy>>3,0X17);			//设置Href的start高8位
	SCCB_WR_Reg(endy>>3,0X18);			//设置Href的end的高8位
}


