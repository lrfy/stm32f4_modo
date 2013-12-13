#include"EasyTracer.h"
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"

#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v1)))
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))

typedef struct{
    unsigned char  red;             // [0,255]
    unsigned char  green;           // [0,255]
    unsigned char  blue;            // [0,255]
}COLOR_RGB;//RGB格式颜色

typedef struct{
    unsigned char hue;              // [0,240]
    unsigned char saturation;       // [0,240]
    unsigned char luminance;        // [0,240]
}COLOR_HSL;//HSL格式颜色

typedef struct{
    unsigned int X_Start;              
    unsigned int X_End;
	unsigned int Y_Start;              
    unsigned int Y_End;
}SEARCH_AREA;//区域

//读取RBG格式颜色，唯一需要移植的函数
extern u16 LCD_GetPoint(u16 x,u16 y);
static void ReadColor(unsigned int x,unsigned int y,COLOR_RGB *Rgb)
{
	unsigned short C16;

	C16 = LCD_GetPoint(x,y);

	Rgb->red   =	 (unsigned char)((C16&0xf800)>>8);
	Rgb->green =	 (unsigned char)((C16&0x07e0)>>3);
	Rgb->blue  =     (unsigned char)((C16&0x001f)<<3);
}



//RGB转HSL
static void RGBtoHSL(const COLOR_RGB *Rgb, COLOR_HSL *Hsl)
{
    int h,s,l,maxVal,minVal,difVal;
	int r  = Rgb->red;
	int g  = Rgb->green;
    int b  = Rgb->blue;
	
	maxVal = max3v(r, g, b);
	minVal = min3v(r, g, b);
	
	difVal = maxVal-minVal;
	
	//计算亮度
    l = (maxVal+minVal)*240/255/2;
	
	if(maxVal == minVal)//若r=g=b
	{
		h = 0; 
		s = 0;
	}
	else
	{
		//计算色调
		if(maxVal==r)
		{
			if(g>=b)
				h = 40*(g-b)/(difVal);
			else
				h = 40*(g-b)/(difVal) + 240;
		}
		else if(maxVal==g)
			h = 40*(b-r)/(difVal) + 80;
		else if(maxVal==b)
			h = 40*(r-g)/(difVal) + 160;
		//计算饱和度
		if(l == 0)
			s = 0;
		else if(l<=120)
			s = (difVal)*240/(maxVal+minVal);
		else
			s = (difVal)*240/(480 - (maxVal+minVal));
	}
    Hsl->hue =        (unsigned char)(((h>240)? 240 : ((h<0)?0:h)));
    Hsl->saturation = (unsigned char)(((s>240)? 240 : ((s<0)?0:s)));
    Hsl->luminance =  (unsigned char)(((l>240)? 240 : ((l<0)?0:l)));
}

//匹配颜色
static int ColorMatch(const COLOR_HSL *Hsl,const TARGET_CONDI *Condition)
{
	if( 
		Hsl->hue		>	Condition->H_MIN &&
		Hsl->hue		<	Condition->H_MAX &&
		Hsl->saturation	>	Condition->S_MIN &&
		Hsl->saturation	<   Condition->S_MAX &&
		Hsl->luminance	>	Condition->L_MIN &&
		Hsl->luminance	<   Condition->L_MAX 
    )
		return 1;
	else
		return 0;
}

//搜索腐蚀中心
static int SearchCentre(unsigned int *x,unsigned int *y,const TARGET_CONDI *Condition,const SEARCH_AREA *Area)
{
	unsigned int SpaceX,SpaceY,i,j,k,FailCount=0;
	COLOR_RGB Rgb;
	COLOR_HSL Hsl;
	
	SpaceX = Condition->WIDTH_MIN/3;
	SpaceY = Condition->HIGHT_MIN/3;

	for(i=Area->Y_Start;i<Area->Y_End;i+=SpaceY)
	{
		for(j=Area->X_Start;j<Area->X_End;j+=SpaceX)
		{
			FailCount=0;
			for(k=0;k<SpaceX+SpaceY;k++)
			{
				if(k<SpaceX)
					ReadColor(j+k,i+SpaceY/2,&Rgb);
				else
					ReadColor(j+SpaceX/2,i+(k-SpaceX),&Rgb);
				RGBtoHSL(&Rgb,&Hsl);
				
				if(!ColorMatch(&Hsl,Condition))
					FailCount++;
				if(FailCount>((SpaceX+SpaceY)>>ALLOW_FAIL_PER))
					break;
			}
			if(k==SpaceX+SpaceY)
			{
				*x = j+SpaceX/2;
				*y = i+SpaceY/2;
				return 1;
			}
		}
	}
	return 0;
}

//搜索腐蚀中心
static int Search(unsigned int *x,unsigned int *y,const TARGET_CONDI *Condition,const SEARCH_AREA *Area, const SEARCH_AREA *Area_dis1, const SEARCH_AREA *Area_dis2)
{
	unsigned int SpaceX,SpaceY,i,j,k,FailCount=0;
	COLOR_RGB Rgb;
	COLOR_HSL Hsl;

	SpaceX = Condition->WIDTH_MIN/3;
	SpaceY = Condition->HIGHT_MIN/3;

	for(i=Area->Y_Start;i<Area->Y_End;i+=SpaceY)
	{
		for(j=Area->X_Start;j<Area->X_End;j+=SpaceX)
		{
			if(i>Area_dis1->Y_Start&&i<Area_dis1->Y_End&&j>Area_dis1->X_Start&&j<Area_dis1->X_End) continue;
			if(i>Area_dis2->Y_Start&&i<Area_dis2->Y_End&&j>Area_dis2->X_Start&&j<Area_dis2->X_End) continue;

			FailCount=0;
			for(k=0;k<SpaceX+SpaceY;k++)
			{
				if(k<SpaceX)
					ReadColor(j+k,i+SpaceY/2,&Rgb);
				else
					ReadColor(j+SpaceX/2,i+(k-SpaceX),&Rgb);
				RGBtoHSL(&Rgb,&Hsl);

				if(!ColorMatch(&Hsl,Condition))
					FailCount++;
				if(FailCount>((SpaceX+SpaceY)>>ALLOW_FAIL_PER))
					break;
			}
			if(k==SpaceX+SpaceY)
			{
				*x = j+SpaceX/2;
				*y = i+SpaceY/2;
				return 1;
			}

		}
	}
	return 0;
}

//从腐蚀中心向外腐蚀，得到新的腐蚀中心
static int Corrode(unsigned int oldx,unsigned int oldy,const TARGET_CONDI *Condition,RESULT *Resu)
{
	unsigned int Xmin,Xmax,Ymin,Ymax,i,FailCount=0;
	COLOR_RGB Rgb;
	COLOR_HSL Hsl;
	
	for(i=oldx;i>IMG_X;i--)
	{
		ReadColor(i,oldy,&Rgb);
		RGBtoHSL(&Rgb,&Hsl);
		if(!ColorMatch(&Hsl,Condition))
			FailCount++;
		if(FailCount>(((Condition->WIDTH_MIN+Condition->WIDTH_MAX)>>2)>>ALLOW_FAIL_PER))
			break;	
	}
	Xmin=i;
	FailCount=0;
	
	for(i=oldx;i<IMG_X+IMG_W;i++)
	{
		ReadColor(i,oldy,&Rgb);
		RGBtoHSL(&Rgb,&Hsl);
		if(!ColorMatch(&Hsl,Condition))
			FailCount++;
		if(FailCount>(((Condition->WIDTH_MIN+Condition->WIDTH_MAX)>>2)>>ALLOW_FAIL_PER))
			break;	
	}
	Xmax=i;
	FailCount=0;
	
	for(i=oldy;i>IMG_Y;i--)
	{
		ReadColor(oldx,i,&Rgb);
		RGBtoHSL(&Rgb,&Hsl);
		if(!ColorMatch(&Hsl,Condition))
			FailCount++;
		if(FailCount>(((Condition->HIGHT_MIN+Condition->HIGHT_MAX)>>2)>>ALLOW_FAIL_PER))
			break;	
	}
	Ymin=i;
	FailCount=0;
	
	for(i=oldy;i<IMG_Y+IMG_H;i++)
	{
		ReadColor(oldx,i,&Rgb);
		RGBtoHSL(&Rgb,&Hsl);
		if(!ColorMatch(&Hsl,Condition))
			FailCount++;
		if(FailCount>(((Condition->HIGHT_MIN+Condition->HIGHT_MAX)>>2)>>ALLOW_FAIL_PER))
			break;	
	}
	Ymax=i;
	FailCount=0;
	
	Resu->x	= (Xmin+Xmax)/2;
	Resu->y	= (Ymin+Ymax)/2;
	Resu->w	= Xmax-Xmin;
	Resu->h	= Ymax-Ymin;

	if(((Xmax-Xmin)>(Condition->WIDTH_MIN)) && ((Ymax-Ymin)>(Condition->HIGHT_MIN)) &&\
	   ((Xmax-Xmin)<(Condition->WIDTH_MAX)) && ((Ymax-Ymin)<(Condition->HIGHT_MAX)))
		return 1;	
	else
		return 0;	
}

//唯一的API，用户将识别条件写入Condition指向的结构体中，该函数将返回目标的x，y坐标和长宽
//返回1识别成功，返回1识别失败
int Trace(const TARGET_CONDI *Condition,RESULT *Resu)
{
	unsigned int i;
	static unsigned int x0,y0,flag=0;
	static SEARCH_AREA Area={IMG_X,IMG_X+IMG_W,IMG_Y,IMG_Y+IMG_H};
	RESULT Result;	
	

	if(flag==0)
	{
		if(SearchCentre(&x0,&y0,Condition,&Area))
			flag=1;
		else
		{
			Area.X_Start= IMG_X;
			Area.X_End  = IMG_X+IMG_W;
			Area.Y_Start= IMG_Y;
			Area.Y_End  = IMG_Y+IMG_H;

			if(!(SearchCentre(&x0,&y0,Condition,&Area)))
			{
				flag=0;
				return 0;
			}	
		}
	}
	Result.x = x0;
	Result.y = y0;
	
	for(i=0;i<ITERATE_NUM;i++)
		Corrode(Result.x,Result.y,Condition,&Result);
		
	if(Corrode(Result.x,Result.y,Condition,&Result))
	{
		x0=Result.x;
		y0=Result.y;
		Resu->x=Result.x;
		Resu->y=Result.y;
		Resu->w=Result.w;
		Resu->h=Result.h;
		flag=1;

		Area.X_Start= Result.x - ((Result.w)>>1);
		Area.X_End  = Result.x + ((Result.w)>>1);
		Area.Y_Start= Result.y - ((Result.h)>>1);
		Area.Y_End  = Result.y + ((Result.h)>>1);


		return 1;
	}
	else
	{
		flag=0;
		return 0;
	}

}

//返回1识别成功，返回1识别失败
int Trace2(const TARGET_CONDI *Condition,RESULT *Resu, RESULT *Resu2, RESULT *Resu3)
{
	unsigned int i;
	static unsigned int x0,y0;
	SEARCH_AREA Area={IMG_X,IMG_X+IMG_W,IMG_Y,IMG_Y+IMG_H};
	SEARCH_AREA Area1={0,0,0,0};
	SEARCH_AREA Area2={0,0,0,0};
	const SEARCH_AREA Area0={0,0,0,0};


	RESULT Result;

	if (Search(&x0,&y0,Condition,&Area, &Area0, &Area0))
	{
		Result.x = x0;
		Result.y = y0;

		for(i=0;i<ITERATE_NUM;i++) Corrode(Result.x,Result.y,Condition,&Result);

		x0=Result.x;
		y0=Result.y;

		Resu->x=Result.x;
		Resu->y=Result.y;
		Resu->w=Result.w;
		Resu->h=Result.h;

		Area1.X_Start = Result.x - ((Result.w)<<1);
		Area1.X_End = Result.x + ((Result.w)<<1);
		Area1.Y_End = Result.y + ((Result.h)<<1);
		Area1.Y_Start = Result.y - ((Result.h)<<1);
	} else return 0;

	if (Search(&x0,&y0,Condition,&Area, &Area1, &Area0))
	{
		Result.x = x0;
		Result.y = y0;

		for(i=0;i<ITERATE_NUM;i++) Corrode(Result.x,Result.y,Condition,&Result);

		x0=Result.x;
		y0=Result.y;

		Resu2->x=Result.x;
		Resu2->y=Result.y;
		Resu2->w=Result.w;
		Resu2->h=Result.h;

		Area2.X_Start = Result.x - ((Result.w)<<1);
		Area2.X_End = Result.x + ((Result.w)<<1);
		Area2.Y_End = Result.y + ((Result.h)<<1);
		Area2.Y_Start = Result.y - ((Result.h)<<1);
	} else return 0;

	if (Search(&x0,&y0,Condition,&Area, &Area1, &Area2))
	{
		Result.x = x0;
		Result.y = y0;

		for(i=0;i<ITERATE_NUM;i++) Corrode(Result.x,Result.y,Condition,&Result);

		x0=Result.x;
		y0=Result.y;

		Resu3->x=Result.x;
		Resu3->y=Result.y;
		Resu3->w=Result.w;
		Resu3->h=Result.h;

	} else return 0;

	return 1;





}

//唯一的API，用户将识别条件写入Condition指向的结构体中，该函数将返回目标的x，y坐标和长宽
//返回1识别成功，返回1识别失败return 0;
int Trace_es(const TARGET_CONDI *Condition, RESULT *Resu1, RESULT *Resu2, RESULT *Resu3)
{
	unsigned int i;
	static unsigned int x[3],y[3];
	static SEARCH_AREA Area1={IMG_X,IMG_X+IMG_W,IMG_Y,IMG_Y+IMG_H},Area2={IMG_X,IMG_X+IMG_W,IMG_Y,IMG_Y+IMG_H};
	RESULT Result1,Result2,Result3;

	if(SearchCentre(&x[0],&y[0],Condition,&Area1))
	{
		Result1.x = x[0];
		Result1.y = y[0];

		Area1.X_Start= IMG_X;
		Area1.X_End  = x[0]-10;
		Area1.Y_Start= y[0]+10;
		Area1.Y_End  = IMG_H;

		Area2.X_Start= x[0]+10;
		Area2.X_End  = IMG_W;
		Area2.Y_Start= y[0]+10;
		Area2.Y_End  = IMG_H;

		if(SearchCentre(&x[1],&y[1],Condition,&Area1))
		{
			Result2.x = x[1];
			Result2.y = y[1];

			Area1.X_Start= IMG_X;
			Area1.X_End  = x[1]-10;
			Area1.Y_Start= y[1]+10;
			Area1.Y_End  = IMG_H;

			Area2.X_Start= x[1]+10;
			Area2.X_End  = x[0]-10;
			Area2.Y_Start= y[1]+10;
			Area2.Y_End  = IMG_H;

			if(SearchCentre(&x[2],&y[2],Condition,&Area1))
			{
				Result3.x = x[2];
				Result3.y = y[2];
			}
			else
			{
				if(SearchCentre(&x[2],&y[2],Condition,&Area2))
				{
					Result3.x = x[2];
					Result3.y = y[2];
				}
				else
				{
					Area2.X_Start= x[0]+10;
					Area2.X_End  = IMG_W;
					Area2.Y_Start= y[0]+10;
					Area2.Y_End  = IMG_H;

					if(SearchCentre(&x[2],&y[2],Condition,&Area2))
					{
						Result3.x = x[1];
						Result3.y = y[1];
					}
					else return 0;
				}
			}
		}
		else
		{
			if(SearchCentre(&x[1],&y[1],Condition,&Area2))
			{
				Result2.x = x[1];
				Result2.y = y[1];

				Area1.X_Start= x[0]+10;
				Area1.X_End  = x[1]-10;
				Area1.Y_Start= y[1]+10;
				Area1.Y_End  = IMG_H;

				Area2.X_Start= x[1]+10;
				Area2.X_End  = IMG_W;
				Area2.Y_Start= y[1]+10;
				Area2.Y_End  = IMG_H;

				if(SearchCentre(&x[2],&y[2],Condition,&Area1))
				{
					Result3.x = x[1];
					Result3.y = y[1];
				}
				else
				{
					if(SearchCentre(&x[2],&y[2],Condition,&Area2))
					{
						Result3.x = x[1];
						Result3.y = y[1];
					}
					else return 0;
				}
			}
			else return 0;
		}
	}
	else return 0;

	for(i=0;i<ITERATE_NUM;i++)
	{
		Corrode(Result1.x,Result1.y,Condition,&Result1);
		Corrode(Result2.x,Result2.y,Condition,&Result2);
		Corrode(Result3.x,Result3.y,Condition,&Result3);
	}

	if(Corrode(Result1.x,Result1.y,Condition,&Result1)&&
	   Corrode(Result2.x,Result2.y,Condition,&Result2)&&
	   Corrode(Result3.x,Result3.y,Condition,&Result3)
	   )
	{

		Resu1->x=Result1.x;
		Resu1->y=Result1.y;
		Resu1->w=Result1.w;
		Resu1->h=Result1.h;

		Resu2->x=Result2.x;
		Resu2->y=Result2.y;
		Resu2->w=Result2.w;
		Resu2->h=Result2.h;

		Resu3->x=Result3.x;
		Resu3->y=Result3.y;
		Resu3->w=Result3.w;
		Resu3->h=Result3.h;

		return 1;
	}
	else
	{
		return 0;
	}

}
