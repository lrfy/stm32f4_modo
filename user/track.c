/* Include -----------------------------------------------*/
#include	"delay.h"
#include	"LCD_TFT_8080.h"
#include    "GUI.h"
#include 	"math.h"
#include    "errno.h"
#include    "stdlib.h"
#include 	"string.h"
#include	"track.h" 
#include	"stm32f4xx.h"
#include	"stm32f4xx_conf.h"

#define		X_SCALE_3M		0.1					//距离为三米时,每个像素的实际距离
#define 	Y_SCALE_3M 		0.1		
#define 	MAX_NUM 		500				//确定数组的大小
#define 	REC_GATE	    3					//用于判断是否记录数据
#define 	ERROR_GATE		50					//判断三角形时的误差阀值

/* Variate -----------------------------------------------*/
u16 Track_Record[MAX_NUM][2];				//记录轨迹坐标串,第一列为X坐标,第二列为Y坐标
u16 Origin[2];											//记录实际原点坐标，Origin[0]为X坐标,Origin[1]为Y坐标
u16 Coordinate[2];									//记录转换后的物体坐标
u16 track_record_pointer = 0;				//指示Track_Record中有多少数据



//函数功能:原点校正
//参数说明:x,y分别为物体的实际坐标
//配置一个外部中断用于确定原点,即在中断函数中调用此函数
void Origin_Correct(u16 x, u16 y)
{
	Origin[0] = x;
	Origin[1] = y;
}

//函数功能:坐标校正
//参数说明:x,y分别为物体当前实际坐标
//在每一次采集到物体的实际坐标之后,调用此函数转换成坐标纸坐标
void Translate_Co(u16 x, u16 y)
{
	Coordinate[0] = (x - Origin[0]) * X_SCALE_3M;
	Coordinate[1] = (y - Origin[1]) * Y_SCALE_3M;
}

//函数功能:判断是否为重复数据,重复数据不予记录
const u8 Match_Condition(u16 x, u16 y)
{
	if((abs(x - Track_Record[track_record_pointer-1][0]) > REC_GATE) | (abs(y - Track_Record[track_record_pointer-1][1]) > REC_GATE))
		return 1;
	else
		return 0;
}

//函数功能:清空轨迹记录数组
void Track_Clear(void)
{
	memset(Track_Record, 0, 1000*sizeof(u16));
}

//函数功能:复位轨迹记录数组
void Track_Reset(void)
{
	track_record_pointer = 0;
}

//函数功能:记录轨迹坐标并绘制轨迹
//配置一个外部中断用于启动和关闭轨迹绘制功能(定义一个标志位),在主循环中判断该标志位进入该函数
void Track_Show(u16 x, u16 y)
{
	u16 mul = 0;
	u16 rem = 0;
	u16 i = 0;
	
	rem = track_record_pointer % MAX_NUM;
	mul = track_record_pointer / MAX_NUM;

	if(Match_Condition(x,y))//满足条件才给予记录
	{
			Track_Record[rem][0] = x;
			Track_Record[rem][1] = y;
			track_record_pointer ++;
	}

	if(mul < 1)//若记录的数据量没有溢出，就从0连到当前的点
	{
		if((Track_Record[0][0] != 0) && (Track_Record[0][1] != 0) )
		{
			for(i=0; i<rem-1; i++)
			{
				LCD_Line(Track_Record[i][0], Track_Record[i][1], Track_Record[i+1][0], Track_Record[i+1][1], RED);
			}
		}

	}
	else
	{
		Track_Clear();
	}


}

//函数功能:判断三角形
//参数说明:三个顶点的实际坐标
u8 Triangle_Jud(u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3)
{
	float edge1 = 0, edge2 = 0, edge3 = 0;
	float longest = 0, short1 = 0, short2 = 0;
	float diff = 0, theta;
	
	edge1 = sqrt(pow((x1 - x2),2) + pow((y1 - y2),2));
	edge2 = sqrt(pow((x1 - x3),2) + pow((y1 - y3),2));
	edge3 = sqrt(pow((x3 - x2),2) + pow((y3 - y2),2));
	
	if(edge1 >= edge2)
	{
		longest = edge1;
		short1 = edge2;
	}
	else 
	{
		longest = edge2;
		short1 = edge1;
	}
	if(longest >= edge3)
	{
		short2 = edge3;
	}
	else
	{
		short2 = longest;
		longest = edge3;
	}
	
	diff = (pow(short1,2) + pow(short2,2) - pow(longest,2))/(2*short1*short2);
	theta= acos(diff);

	if((theta > (M_PI/2-M_PI/36))&&(theta < (M_PI/2+M_PI/36))) return 1;				//直角三角形返回1
	if(theta < (M_PI/2-M_PI/36)) return 2;	//锐角三角形返回2
	if(theta > (M_PI/2+M_PI/36)) return 3;	//钝角三角形返回3
	else return 0; //判断失败
}
