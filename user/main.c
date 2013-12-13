#include"fsmc.h"
#include"sccb.h"
#include"delay.h"
#include"GUI.h"
#include"ov7670.h"
#include"track.h"
#include"OLED.h"
#include"LCD_TFT_8080.h"
#include"EasyTracer.h"
#include"i2c_soft.h"
#include"wt588d.h"
#include"time.h"
#include"math.h"
#include"errno.h"
#include"stdlib.h"
#include"string.h"
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"

TARGET_CONDI Condition, Condition_red;
RESULT Resu, Resu1, Resu2, Resu3;

u8 VSYNC_Flag = 0;
u8 Mode = 0;
u16 X_base,Y_base;
s16 X_real,Y_real;
s16 X_laser,Y_laser;
u8 Triangle_Judge;
extern unsigned int CCR1_Val;
extern unsigned int CCR2_Val;
extern unsigned int PWM1;
extern unsigned int PWM2;
extern unsigned int xyflag;
extern u16 track_record_pointer;
unsigned int Dis_mm;
double Pixel_cm = 0.55;
double Dis_m = 3, Dis_cm = 300;
double Xcm,Ycm;

//TARGET_CONDI Condition = {140,175,65,240,55,200,3,3,30,30};


void camera_refresh(void);
void flag_led(void);
void botton_init(void);
int multi_point(void);
int single_point(void);
void get_distance(void);
double distance(double x);
double dis_per_pixel(double dis);

int main()
{
	int npcnt=0;
	u8 lightMode = 0;
	u8 saturation = 2;
	u8 brightness = 2;
	u8 contrast = 2;
	u8 effect = 0;
	/////////////////////////////////////condition init
	Condition.H_MIN = 120;//目标最小色调
	Condition.H_MAX = 190;//目标最大色调

	Condition.S_MIN = 60;//目标最小饱和度
	Condition.S_MAX = 130;//目标最大饱和度

	Condition.L_MIN = 60;//目标最小亮度
	Condition.L_MAX = 130;//目标最大亮度

	Condition.WIDTH_MIN = 3;//目标最小宽度
	Condition.HIGHT_MIN = 3;//目标最小高度

	Condition.WIDTH_MAX = 30;//目标最大宽度
	Condition.HIGHT_MAX = 30;//目标最大高度

	Condition_red.H_MIN = 0;//目标最小色调
	Condition_red.H_MAX = 240;//目标最大色调

	Condition_red.S_MIN = 0;//目标最小饱和度
	Condition_red.S_MAX = 240;//目标最大饱和度

	Condition_red.L_MIN = 180;//目标最小亮度
	Condition_red.L_MAX = 240;//目标最大亮度

	Condition_red.WIDTH_MIN = 3;//目标最小宽度
	Condition_red.HIGHT_MIN = 3;//目标最小高度

	Condition_red.WIDTH_MAX = 30;//目标最大宽度
	Condition_red.HIGHT_MAX = 30;//目标最大高度

	delay_init(168);                                           //delay function init

	flag_led();
	SCCB_Init();
	fsmc_init();
	LCD_init();
	OLED_Init();
	botton_init();
	IIC_Init();
	shot_init();
	EXTILine13_15_Config();


	while(OV7670_Init());                                      //摄像头初始化
	LCD_P8x16Str(19, 0, "UESTC ICKX");
	OV7670_Light_Mode(lightMode);
	OV7670_Color_Saturation(saturation);
	OV7670_Brightness(brightness);
	OV7670_Contrast(contrast);
	OV7670_Special_Effects(effect);
	OV7670_Window_Set(10,174,240,320);						   //初始化结束

	EXTI->IMR |= 1<<8;										   //打开中断
	EXTI->EMR |= 1<<8;										   //允许外部中断，准备取数据

	OV7670_OE_CR;

	while(!Mode)											   //设置基准点
	{
		camera_refresh();
		single_point();
		EXTI->IMR |= 1<<8;
		EXTI->EMR |= 1<<8;//允许外部中断，准备取数据
	}

	X_base = Resu.x;
	Y_base = Resu.y;
	Mode=0;

 	while(1)												   //切换模式。主循环
 	{
 		camera_refresh();
 		LCD_P6x8Str(0, 6, "                   ");
 		if(Mode == 0)
 			LCD_P6x8Str(0, 6, "Single Point Mode");
 		else if(Mode == 1)
 			LCD_P6x8Str(0, 6, "Triangle Mode");
 		else if(Mode == 2)
 			LCD_P6x8Str(0, 6, "Trace Mode");
 		else if(Mode == 3)
 			LCD_P6x8Str(0, 6, "Distance Mode");
 		else if(Mode == 4)
 		 	LCD_P6x8Str(0, 6, "Auto_dist Mode");
 		else if(Mode == 5)
 		 	LCD_P6x8Str(0, 6, "Laser Mode");
 		if(Mode == 5)
 		{
 			LCD_P6x8Str(0, 2, "                    ");
			LCD_P6x8Str(0, 3, "                    ");
			LCD_P6x8Str(0, 4, "                    ");
			LCD_P6x8Str(0, 5, "                    ");
 			if(single_point())
			{
				X_real = -(Resu.x - X_base);
				Y_real = Resu.y - Y_base;
				go_shot(X_real, Y_real);
			}
 		}
 		else if(Mode == 4)
 		{
 			get_distance();
 			Dis_m = Dis_mm/1000.0;
			if(single_point() == 1)
			{
	 			LCD_P6x8Str(0, 2, "X:                  ");
	 			LCD_P6x8Str(0, 3, "Y:                  ");
	 			LCD_P6x8Str(0, 4, "Distance(mm):       ");
	 			LCD_P6x8Str(0, 5, "                    ");
				npcnt = 0;
				X_real = Resu.x - X_base;
				Y_real = Resu.y - Y_base;
				Pixel_cm = dis_per_pixel(Dis_m);
				if(X_real < 0)
				{
					Xcm=(-X_real)*Pixel_cm;
					LCD_P8x6numstr(18, 2, Xcm);
					LCD_P8x6numstr(60, 2, -X_real);
				} else {
					LCD_P6x8Str(12, 2, "-");
					Xcm=(X_real)*Pixel_cm;
					LCD_P8x6numstr(18, 2, Xcm);
					LCD_P8x6numstr(60, 2, X_real);
				}
				if(Y_real < 0)
				{
					LCD_P6x8Str(12, 3, "-");
					Ycm=(-Y_real)*Pixel_cm;
					LCD_P8x6numstr(18, 3, Ycm);
					LCD_P8x6numstr(60, 3, -Y_real);
				} else {
					Ycm=(Y_real)*Pixel_cm;
					LCD_P8x6numstr(18, 3, Ycm);
					LCD_P8x6numstr(60, 3, Y_real);
				}
			} else {
				npcnt++;
				if(npcnt > 2)
				{
					LCD_P8x16Str(0, 2, "                   ");
					LCD_P8x16Str(0, 2, "No Point");
				}
			}
			LCD_P8x6numstr(0, 5, Dis_mm);
 		}
 		else if(Mode == 3)
 		{
 			Track_Clear();
 			track_record_pointer = 0;
 			if(single_point())
 			{
 				X_real = Resu.x - X_base;
 				LCD_P6x8Str(0, 2, "Distance(cm):       ");
 				LCD_P6x8Str(0, 3, "                    ");
 				X_real = abs(X_real);
 				Dis_cm  = distance(X_real);
 				LCD_P8x6numstr(0, 3, Dis_cm);
 				LCD_P8x6numstr(60, 3, X_real);
 			}

 		}
 		else if(Mode == 2)
 		{
 			LCD_P8x16Str(0, 2, "                ");
 			LCD_P8x16Str(0, 4, "                ");
 			if(single_point())
 			Track_Show(Resu.x, Resu.y);
 		}
 		else if(Mode == 1)
 		{
 			if(multi_point())
 			{
 				Triangle_Judge = Triangle_Jud(Resu.x, Resu.y, Resu2.x, Resu2.y, Resu3.x, Resu3.y);
 			}
 			else ;
 			LCD_P8x16Str(0, 2, "                ");
 			LCD_P8x16Str(0, 4, "                ");
 			if(Triangle_Judge == 1)
 			{
 				LCD_P8x16Str(0, 2, "Rec Triangle");
 			}
 			else if(Triangle_Judge == 2)
 			{
 				LCD_P8x16Str(0, 2, "Acu Triangle");
 			}
 			else if(Triangle_Judge == 3)
 			{
 				LCD_P8x16Str(0, 2, "Obt Triangle");
 			}
 			else
 			{
 				LCD_P8x16Str(0, 2, "No  Triangle");
 			}
 		}
 		else if(Mode == 0)
 		{

			if(single_point() == 1)
			{
				LCD_P6x8Str(0, 2, "X:                  ");
				LCD_P6x8Str(0, 3, "Y:                  ");
				npcnt = 0;
				X_real = Resu.x - X_base;
				Y_real = Resu.y - Y_base;
				Dis_m  = Dis_cm/100.0;
				Pixel_cm = dis_per_pixel(Dis_m);
				if(X_real < 0)
				{
					Xcm=(-X_real)*Pixel_cm;
					LCD_P8x6numstr(18, 2, Xcm);
					LCD_P8x6numstr(60, 2, -X_real);
				} else {
					LCD_P6x8Str(12, 2, "-");
					Xcm=(X_real)*Pixel_cm;
					LCD_P8x6numstr(18, 2, Xcm);
					LCD_P8x6numstr(60, 2, X_real);
				}
				if(Y_real < 0)
				{
					LCD_P6x8Str(12, 3, "-");
					Ycm=(-Y_real)*Pixel_cm;
					LCD_P8x6numstr(18, 3, Ycm);
					LCD_P8x6numstr(60, 3, -Y_real);
				} else {
					Ycm=(Y_real)*Pixel_cm;
					LCD_P8x6numstr(18, 3, Ycm);
					LCD_P8x6numstr(60, 3, Y_real);
				}
			} else {
				npcnt++;
				if(npcnt > 2)
				{
					LCD_P8x16Str(0, 2, "                   ");
					LCD_P8x16Str(0, 2, "No Point");
				}
			}
		}
 		else ;
 		EXTI->IMR |= 1<<8;
 		EXTI->EMR |= 1<<8;//允许外部中断，准备取数据
 	}
 	return 0;
}

void botton_init(void)										    //初始化外部中断
{
	GPIO_InitTypeDef  GPIO_conf;
	NVIC_InitTypeDef  NVIC_conf;
	EXTI_InitTypeDef  EXTI_conf;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
															   //使能相关端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);      //使能SYSCFG时钟

 	GPIO_conf.GPIO_Mode=GPIO_Mode_IN;
 	GPIO_conf.GPIO_Pin=GPIO_Pin_0;
 	GPIO_conf.GPIO_PuPd = GPIO_PuPd_NOPULL;
 	GPIO_Init(GPIOA, &GPIO_conf);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
 	EXTI_conf.EXTI_Line = EXTI_Line0;				    	   //设置外部中断为2线
 	EXTI_conf.EXTI_Mode = EXTI_Mode_Interrupt;		    	   //设置模式为中断模式 而非事件模式Event
 	EXTI_conf.EXTI_Trigger = EXTI_Trigger_Rising;	    	   //设置中断触发方式 Falling 还有其他方式Rising和Falling_Rising
 	EXTI_conf.EXTI_LineCmd = ENABLE;						   //使能
 	EXTI_Init(&EXTI_conf);

 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	            //设置分组
 	NVIC_conf.NVIC_IRQChannel = EXTI0_IRQn;             //设置2线中断
 	NVIC_conf.NVIC_IRQChannelPreemptionPriority = 0;	//设置抢占优先级为0
 	NVIC_conf.NVIC_IRQChannelSubPriority = 0;           //设置响应优先级为0
 	NVIC_conf.NVIC_IRQChannelCmd = ENABLE;	            //使能
 	NVIC_Init(&NVIC_conf);
}

void flag_led(void)
{
	GPIO_InitTypeDef GPIO_conf;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_conf.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_conf.GPIO_OType=GPIO_OType_PP ;
	GPIO_conf.GPIO_Pin=GPIO_Pin_12;
	GPIO_conf.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_conf.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_conf);
}

void camera_refresh(void)
{
	u32 j;	  		   //循环变量
	u8 color_h, color_l;//单像素颜色
	LCD_SetCursor(0, 0);
	LCD_WriteRAM_Prepare();
	if(VSYNC_Flag == 2)
	{
		OV7670_RRST_CR;				//开始复位读指针
		OV7670_RCK_CR;
		OV7670_RCK_EN;
		OV7670_RCK_CR;
		OV7670_RRST_EN;				//复位读指针结束
		OV7670_RCK_EN;
 		for(j=0; j<76800; j++)
		{
			OV7670_RCK_CR;
			color_h=OV7670_DATA;	//读数据
			OV7670_RCK_EN;
			OV7670_RCK_CR;
			color_l=OV7670_DATA;	//读数据
			OV7670_RCK_EN;
			LCD_WR_DATA(((color_h<<8)|color_l));
		}
		VSYNC_Flag = 0;
	}
}

void get_distance(void)
{
	delay(20, mS);
	KS103_WriteOneByte(0XE8,0X02,0xb4);
	delay(100, mS);
	Dis_mm = KS103_ReadOneByte(0xe8, 0x02);
	delay(100, mS);
	Dis_mm <<= 8;
	Dis_mm += KS103_ReadOneByte(0xe8, 0x03);
}

double distance(double x)
{
	double dis;
	dis = 0.09531*x*x - 14.22*x + 734.5;
	return dis;
}

double dis_per_pixel(double dis)
{
	double pixel_dis;
	pixel_dis=-0.002822*dis*dis + 0.1797*dis + 0.02305;
	return pixel_dis;
}

int multi_point(void)
{
	if(Trace2(&Condition, &Resu, &Resu2, &Resu3))
	{
		 LCD_Rect(Resu.x-Resu.w/2, Resu.y-Resu.h/2, Resu.x+Resu.w, Resu.y+Resu.h/2, RED);
		 LCD_Rect(Resu2.x-Resu2.w/2, Resu2.y-Resu2.h/2, Resu2.x+Resu2.w, Resu2.y+Resu2.h/2, GREEN);
		 LCD_Rect(Resu3.x-Resu3.w/2, Resu3.y-Resu3.h/2, Resu3.x+Resu3.w, Resu3.y+Resu3.h/2, BLUE);

		 LCD_Line(Resu.x, Resu.y, Resu2.x, Resu2.y, RED);
		 LCD_Line(Resu.x, Resu.y, Resu3.x, Resu3.y, RED);
		 LCD_Line(Resu3.x, Resu3.y, Resu2.x, Resu2.y, RED);
		 return 1;
	}
	else return 0;
}

int single_point(void)
{
	if(Trace(&Condition, &Resu))
	{
		 LCD_Rect(Resu.x-Resu.w/2, Resu.y-Resu.h/2, Resu.x+Resu.w, Resu.y+Resu.h/2, RED);
		 return 1;
	}
	else return 0;
}


void EXTI0_IRQHandler(void)
{
 if(EXTI_GetFlagStatus(EXTI_Line0)==SET)
 {
   delay(20, mS);
   if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1)
   {
	   Mode++;
	   if(Mode==6) Mode=0;
   }
   EXTI_ClearITPendingBit(EXTI_Line0);
 }
}

void EXTI9_5_IRQHandler(void)
{
 if(EXTI_GetFlagStatus(EXTI_Line8)==SET)
 {
		VSYNC_Flag++;
		if(VSYNC_Flag == 1)
		{
			OV7670_WRST_EN;
			OV7670_WRST_CR;
			delay(80, uS);
			OV7670_WRST_EN;
			OV7670_WR_EN; //允许数据写入FIFO
		}
		else if(VSYNC_Flag == 2)
		{
			OV7670_WR_CR;//禁止写入FIFO
			OV7670_WRST_CR;//写指针复位
			OV7670_WRST_EN;

			EXTI->IMR&=~(1<<8);
			EXTI->EMR&=~(1<<8);//禁止外部中断，准备取数据
		}
   EXTI_ClearITPendingBit(EXTI_Line8);
 }
}

void EXTI15_10_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		delay(20, mS);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
		{
			CCR1_Val += 1;
			while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
			{
				delay(50, mS);
				CCR1_Val += 1;
				if(CCR1_Val == 9600)CCR1_Val = 9000;
				TIM_SetCompare1(TIM3, CCR1_Val);
			}


		}
		EXTI_ClearITPendingBit(EXTI_Line13);
	}

	if(EXTI_GetITStatus(EXTI_Line14) != RESET)
	{
		delay(20, mS);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)
			{
				delay(50, mS);
				CCR2_Val += 1;
				if(CCR2_Val == 9600)CCR2_Val = 9000;
				TIM_SetCompare2(TIM3, CCR2_Val);
			}

		}
		EXTI_ClearITPendingBit(EXTI_Line14);
	}

	if(EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		delay(5, mS);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0)
		{
			PWM1 = CCR1_Val;
			PWM2 = CCR2_Val;
			xyflag = 1;

		}
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}

