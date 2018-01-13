#ifndef __BMP_H_
#define __BMP_H_

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <pthread.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//
//	前面三个结构体struct bitmap_header, struct bitmap_info, struct rgb_quad
//
//	包含了图片头文件54个字节内容,里面重要内容具体注释如下:
//
/////////////////////////////////////////////////////////////////////////////////////////////

struct bitmap_header
{
	int16_t type;
	int32_t size; // 图像文件大小
	int16_t reserved1;
	int16_t reserved2;
	int32_t offbits; // bmp图像数据偏移量
}__attribute__((packed));

struct bitmap_info
{
	int32_t size; // 本结构大小	
	int32_t width;   //图像宽
	int32_t height;	 //图像高
	int16_t planes;

	int16_t bit_count; // 色深 24位
	int32_t compression;
	int32_t size_img; // bmp数据大小，必须是4的整数倍(这个是bmp图片重要属性)
	int32_t X_pel;
	int32_t Y_pel;
	int32_t clrused;
	int32_t clrImportant;
}__attribute__((packed));

struct rgb_quad    //是否压缩（由于bmp图片没有压缩图片 这个结构体没什么用）
{
	int8_t blue;
	int8_t green;
	int8_t red;
	int8_t reserved;
}__attribute__((packed));



//自己定义的结构体 来保存图片一般的数据
struct image_info
{
	int width;   
	int height;
	int pixel_size; // 单位是byte 像素大小bmp一般一个像素3个字节
};

/***********************************************************************************
//功能：加载一张bmp图片
//参数：char *filename 图片名字  struct image_info *imginfo 保存图像数据结构体
//返回值：返回图片RGB数据指针
************************************************************************************/
char *load_bmp(char *filename, struct image_info *imginfo);


/************************************************************************************
//功能：将指定的bmp图片，显示到LCD的指定位置
//参数：char *FB  映射在LCD显存上的指针  int x ,int y  图片显示位置
//		char *rgb  图片rgb数据指针   struct image_info *imginfo 保存图像数据结构体
//返回值：无
***********************************************************************************/
void bmp2lcd(char *FB, int x, int y, char *rgb, struct image_info *imginfo);

char *init_lcd(void);

#endif //bmp.h
