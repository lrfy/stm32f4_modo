#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"
#include"delay.h"
#include "OLED.h"
#include "codetab.h"
void OLED_IO_init(void)
{
	GPIO_InitTypeDef GPIO_Structure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC ,ENABLE);    //使能PORTE时钟
   	GPIO_Structure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Structure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Structure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC, &GPIO_Structure);
}

/*********************LCD写数据************************************/
void LCD_WrDat(unsigned char dat)	 
{
unsigned char i;
LCD_DC1;  
for(i=0;i<8;i++) //发送一个八位数据
{
if((dat << i) & 0x80)
{LCD_SDA1;}
else  LCD_SDA0;
LCD_SCL0;
LCD_SCL1;
}
}
/*********************LCD写命令************************************/
void LCD_WrCmd(unsigned char cmd)
{
unsigned char i;
LCD_DC0;
for(i=0;i<8;i++) //发送一个八位数据
{
if((cmd << i) & 0x80)
{LCD_SDA1;}
else  LCD_SDA0;
LCD_SCL0;
LCD_SCL1;
}
}
/*********************LCD 设置坐标************************************/
void LCD_Set_Pos(unsigned char x, unsigned char y) 
{ 
LCD_WrCmd(0xb0+y);
LCD_WrCmd(((x&0xf0)>>4)|0x10);
LCD_WrCmd((x&0x0f)|0x01); 
} 
/*********************LCD全屏************************************/
void LCD_Fill(unsigned char bmp_dat) 
{
unsigned char y,x;
for(y=0;y<8;y++)
{
LCD_WrCmd(0xb0+y);
LCD_WrCmd(0x01);
LCD_WrCmd(0x10);
for(x=0;x<X_WIDTH;x++)
LCD_WrDat(bmp_dat);
}
}
/*********************LCD复位************************************/
void LCD_CLS(void)
{
unsigned char y,x;	
for(y=0;y<8;y++)
{
LCD_WrCmd(0xb0+y);
LCD_WrCmd(0x01);
LCD_WrCmd(0x10); 
for(x=0;x<X_WIDTH;x++)
LCD_WrDat(0);
}
}
/*********************LCD初始化************************************/
void OLED_Init(void)
{
	OLED_IO_init();
	LCD_SCL1;
	LCD_RST0;
	delay(50, mS);
	LCD_RST1;       //从上电到下面开始初始化要有足够的时间，即等待RC复位完毕
	LCD_WrCmd(0xae);//--turn off oled panel
	LCD_WrCmd(0x00);//---set low column address
	LCD_WrCmd(0x10);//---set high column address
	LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	LCD_WrCmd(0x81);//--set contrast control register
	LCD_WrCmd(0xcf); // Set SEG Output Current Brightness
	LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	LCD_WrCmd(0xa6);//--set normal display
	LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	LCD_WrCmd(0x3f);//--1/64 duty
	LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	LCD_WrCmd(0x00);//-not offset
	LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	LCD_WrCmd(0xd9);//--set pre-charge period
	LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	LCD_WrCmd(0xda);//--set com pins hardware configuration
	LCD_WrCmd(0x12);
	LCD_WrCmd(0xdb);//--set vcomh
	LCD_WrCmd(0x40);//Set VCOM Deselect Level
	LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	LCD_WrCmd(0x02);//
	LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
	LCD_WrCmd(0x14);//--set(0x10) disable
	LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
	LCD_WrCmd(0xaf);//--turn on oled panel
	LCD_Fill(0x00);  //初始清屏
	LCD_Set_Pos(0,0);
} 
/***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
void LCD_P6x8Str(unsigned char x, unsigned char y,char* ch)
{
unsigned char c=0,i=0,j=0;      
while (ch[j]!='\0')
{    
c =ch[j]-32;
if(x>126){x=0;y++;}
LCD_Set_Pos(x,y);    
for(i=0;i<6;i++)     
LCD_WrDat(F6x8[c][i]);  
x+=6;
j++;
}
}
/*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7****************/
void LCD_P8x16Str(unsigned char x, unsigned char y,char* ch)
{
unsigned char c=0,i=0,j=0;
while (ch[j]!='\0')
{    
c =ch[j]-32;
if(x>120){x=0;y++;}
LCD_Set_Pos(x,y);    
for(i=0;i<8;i++)     
LCD_WrDat(F8X16[c][i]);
LCD_Set_Pos(x,y+1);    
for(i=0;i<8;i++)     
LCD_WrDat(F8X16[c][i+8]);  
x+=8;
j++;
}
} 
void LCD_P8x6numstr(unsigned char x, unsigned char y,unsigned int num1)
{
  unsigned char wei,i;
  unsigned char shuzi[5];
  shuzi[0]=num1/10000;
  shuzi[1]=num1%10000/1000;
  shuzi[2]=num1%1000/100;
  shuzi[3]=num1%100/10;
  shuzi[4]=num1%10;
  LCD_Set_Pos(x,y);
 for(wei=0;wei<5;wei++)
 {
   for(i=0;i<6;i++)     
    LCD_WrDat(num2[shuzi[wei]][i]);  
 }
}
void LCD_P8x6voltage2_5_2_0(unsigned char x, unsigned char y,float volnum) //1.86
{
  unsigned int zheng,volxiao;
  unsigned char wei,i,xiaoshu[3];
  float vol;
  //volnum&=0xff0;
  //volnum=(float)volnum;
  vol=volnum/4095.0*2.5;
  zheng=(unsigned int)vol;
  vol=vol-zheng;
  vol=vol*1000;
  volxiao=(unsigned int)vol;
  xiaoshu[2]=volxiao%10;
  xiaoshu[1]=volxiao%100/10;
  xiaoshu[0]=volxiao/100;
  LCD_Set_Pos(x,y);
  for(i=0;i<6;i++)     
    LCD_WrDat(num2[zheng][i]);
  for(i=0;i<6;i++)     
    LCD_WrDat(F6x8[14][i]); 
  for(wei=0;wei<3;wei++)
    for(i=0;i<6;i++)
      LCD_WrDat(num2[xiaoshu[wei]][i]);
  for(i=0;i<6;i++)     
    LCD_WrDat(F6x8['V'-32][i]);     
  
}
void LCD_P8x6voltage1_5_2_0(unsigned char x, unsigned char y,float volnum) //1.86
{
  unsigned int zheng,volxiao;
  unsigned char wei,i,xiaoshu[3];
  float vol;
  //volnum&=0xff0;
  //volnum=(float)volnum;
  vol=volnum/4095.0*1.5;
  zheng=(unsigned int)vol;
  vol=vol-zheng;
  vol=vol*1000;
  volxiao=(unsigned int)vol;
  xiaoshu[2]=volxiao%10;
  xiaoshu[1]=volxiao%100/10;
  xiaoshu[0]=volxiao/100;
  LCD_Set_Pos(x,y);
  for(i=0;i<6;i++)     
    LCD_WrDat(num2[zheng][i]);
  for(i=0;i<6;i++)     
    LCD_WrDat(F6x8[14][i]); 
  for(wei=0;wei<3;wei++)
    for(i=0;i<6;i++)
      LCD_WrDat(num2[xiaoshu[wei]][i]);
  for(i=0;i<6;i++)     
    LCD_WrDat(F6x8['V'-32][i]);     
  
}
void LCD_P8x6voltage3_3_2_0(unsigned char x, unsigned char y,float volnum) //1.86
{
  unsigned int zheng,volxiao;
  unsigned char wei,i,xiaoshu[3];
  float vol;
  //volnum&=0xff0;
  //volnum=(float)volnum;
  vol=volnum/4095.0*3.3;
  zheng=(unsigned int)vol;
  vol=vol-zheng;
  vol=vol*1000;
  volxiao=(unsigned int)vol;
  xiaoshu[2]=volxiao%10;
  xiaoshu[1]=volxiao%100/10;
  xiaoshu[0]=volxiao/100;
  LCD_Set_Pos(x,y);
  for(i=0;i<6;i++)     
    LCD_WrDat(num2[zheng][i]);
  for(i=0;i<6;i++)     
    LCD_WrDat(F6x8[14][i]); 
  for(wei=0;wei<3;wei++)
    for(i=0;i<6;i++)
      LCD_WrDat(num2[xiaoshu[wei]][i]);
  for(i=0;i<6;i++)     
    LCD_WrDat(F6x8['V'-32][i]);     
  
}

void LCD_P6x8cha(unsigned char x, unsigned char y,unsigned char c)
{
unsigned int i;
c =c-32;
if(x>126){x=0;y++;}
LCD_Set_Pos(x,y);    
for(i=0;i<6;i++)     
LCD_WrDat(F6x8[c][i]);  
x+=6;


} 
void LCD_P6x8float(unsigned char x, unsigned char y,float c)
{
  unsigned int zheng,xiao,i;
  LCD_Set_Pos(x,y);
  zheng=(unsigned int)c;
  c=c-zheng;
  c=(c+0.05)*10;
  xiao=(unsigned int)c;
  for(i=0;i<6;i++)     
    LCD_WrDat(num2[zheng][i]);
  for(i=0;i<6;i++)     
    LCD_WrDat(F6x8[14][i]);  
  for(i=0;i<6;i++)     
    LCD_WrDat(num2[xiao][i]); 
}
