#include <stdio.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <string.h>
#include "bmp.h"



char *load_bmp(char *filename, struct image_info *imginfo);
void bmp2lcd(char *FB, int x, int y, char *rgb, struct image_info *imginfo);
char *init_lcd(void);
int show_bmp(char *bmp);

int main(void)
{
	//1.创建套接字
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("socket fail");
		return -1;
	}
	//2.连接服务器
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5678);
	servaddr.sin_addr.s_addr = inet_addr("192.168.6.54");

	int ret = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if(ret < 0)
	{
		perror("connect fail:");
		close(sockfd);
		return -1;
	}


	char buf[50]="0";

	// ret = write(sockfd, buf, strlen(buf)+1);
	while(1)
	{
		ret = read(sockfd, buf, sizeof(buf));
		printf("%s\n", buf);
		if(strcmp(buf,"closelight")==0)
		{
			show_bmp("/photo/CloseLight.bmp");
		}

		if(strcmp(buf,"openlight")==0)
		{
			show_bmp("/photo/OpenLight.bmp");
		}
	
	}
	close(sockfd);
	return 0;
	
}

char *load_bmp(char *filename, struct image_info *imginfo)
{
	FILE *fp = fopen(filename, "r");

	// 将图像的格式头读出来
	struct bitmap_header header;
	struct bitmap_info   info;

	fread(&header, sizeof(header), 1, fp);
	fread(&info,   sizeof(info),   1, fp);

	imginfo->width =  info.width;
	imginfo->height=  info.height;
	imginfo->pixel_size =  info.bit_count/8;

	int rgb_size = header.size-sizeof(header)-sizeof(info);
	char *rgb = calloc(1, rgb_size);
	if(rgb != NULL)
	{
		fread(rgb, rgb_size, 1, fp);
	}
	fclose(fp);
	return rgb;
}

void bmp2lcd(char *FB, int x, int y, char *rgb, struct image_info *imginfo)
{
	// 一行图像的字节数
	int pad = (4 - (imginfo->width * imginfo->pixel_size) % 4) % 4;
	int line = imginfo->width * imginfo->pixel_size + pad;

	// 让rgb指向图像的最后一行
	rgb = rgb + (imginfo->height-1) * line;

	// 让显存指针FB指向你规定的地方(x, y)
	FB += (800*y + x) * 4;

	// 将图像的RGB数据，恰当地拷贝到显存中
	int i;
	for(i=0; i<imginfo->height && i<480-y; i++)
	{
		// 将rgb指向的当前一行，拷贝到显存
		int j;
		for(j=0; j<imginfo->width && j<800-x; j++)
		{
			memcpy(FB+800*4*i+4*j, rgb+3*j, 3);
		}

		// 让rgb指向前一行 
		rgb -= line;
	}
}
//初始化lcd屏 并且返回映射指针
char *init_lcd(void)
{
	int lcd = open("/dev/fb0", O_RDWR);
	if(lcd == -1)
	{
		perror("open() failed");
		exit(0);
	}
	char *FB = mmap(NULL, 800*480*4, PROT_READ|PROT_WRITE, MAP_SHARED, lcd, 0);
	if(FB == MAP_FAILED)
	{
		perror("mmap() failed");
		exit(0);
	}
	return FB;
}

int show_bmp(char *bmp)
{
	// 准备好LCD，映射一块恰当大小的显存
	char *FB = init_lcd();

	// 加载一张指定的BMP图片：获取图片的尺寸，并且返回其RGB数据指针
	struct image_info imginfo;
	char *rgb = load_bmp(bmp, &imginfo);

	// 将RGB数据显示到显存FB的指定位置(x, y)
	bmp2lcd(FB, 0, 0, rgb, &imginfo);
	free(rgb);

	// 取消映射内存
	munmap(FB, 800*480*4);

	return 0;
	
}