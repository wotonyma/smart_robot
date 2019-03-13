#ifndef __LCD_APP_H
#define __LCD_APP_H

// 显示 横线 竖线 框 字符 汉字 字体 图像等
#include <sys/mman.h>
#include <stdio.h>
#include <linux/fb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <wchar.h>
#include <string.h>
#include "ascii.h"

#include <ft2build.h>
#include FT_FREETYPE_H

//LCD初始化
extern void lcd_init(void);
//撤除LCD初始化
extern void lcd_uninit(void);

//将屏幕清成黑色
extern void lcd_clear_display(unsigned int c);
//LCD描点函数
extern void show_put_pixel(int x ,int y , unsigned int color);

//画线函数
extern void lcd_put_line(int x1, int y1 , int x2, int y2 , unsigned int color);

//显示当个字符
extern void lcd_put_ascii(int x, int y , unsigned char c);
//显示GBK汉字码
extern void lcd_put_gbk(int x, int y, unsigned char *str );

//目标像素点显示
extern void draw_bitmap(int color,int flag, FT_Bitmap*  bitmap, FT_Int  x, FT_Int y);


/*******************************************
函数说明：显示汉字字符
wchar_t *wtext:传入的汉字或者字符
int size	  ：字体大小     
int color	  ：字体颜色
int flag	  ：有无底色，0显示底色 1 无底色
int start_x   ：显示的起点坐标X轴
int start_y   ：显示的起点坐标y轴

********************************************/
extern void show_hanzi(wchar_t *wtext,int size,int color, int flag,int start_x,int start_y);

/****************************************
函数说明：任意位置显示一张小于800*480 的24位bmp图片
const char *pathname：图片路径
int start_x：图片起始坐标X轴
int start_y：图片超始坐标Y轴
***************************************/
void show_bmp(const char *pathname,int start_x,int start_y);


#endif