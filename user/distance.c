/* Include -----------------------------------------------*/
#include	"stm32f4xx.h"
#include	"stm32f4xx_conf.h"
#include 	"math.h"
#include 	"string.h"
#include 	"track.h"

#define 	Original_Distance		300
#define		lambda					2
#define  	gamma					2

/*PS:以上两个常数需要实际调整,以下两个函数哪个准用哪个*/

/* Variate -----------------------------------------------*/
u16 real_distance = 0;
extern u16 Origin[2];
extern u16 Coordinate[2];	

/* Function ----------------------------------------------*/
//函数功能:测距(基于矩形)
//参数说明:当前物体在摄像头坐标中的位置
u16 Measure_Dis1(u16 ori_y)
{
	u16 delta_x = 0;
	delta_x = (Origin[1] - ori_y) * lambda;
	real_distance = Original_Distance - delta_x;
	return real_distance;
}

//函数功能:测距(基于圆形)
//参数说明:当前物体在摄像头坐标中的位置
u16 Measure_Dis2(u16 ori_x, u16 ori_y)
{
	u16 delta_r = 0;
	u16 delta_x = 0;
	Translate_Co(ori_x, ori_y);
	delta_r = sqrt(pow(Coordinate[0],2) + pow(Coordinate[1],2));
	delta_x = delta_r * gamma;
	real_distance = Original_Distance - delta_x;
	return real_distance;
}
