#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/in.h>

#define HTTPGET  "GET %s HTTP/1.1\r\nHost:%s\r\nConnection:close\r\n\r\n"

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
	servaddr.sin_port = htons(80);
	//servaddr.sin_addr.s_addr = inet_addr("220.170.186.167");
	//servaddr.sin_addr.s_addr = inet_addr("180.149.145.241");
	servaddr.sin_addr.s_addr = inet_addr("222.186.172.93");

	int ret = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if(ret < 0)
	{
		perror("connect fail:");
		close(sockfd);
		return -1;
	}

	//发送http请求
	//char *path = "/share/qrcode?w=150&h=150&url=http://www.tiydy.cc";
	//char *host = "pan.baidu.com";
	char *path = "/20171027114026_v8VFwP_joanne_722_27_10_2017_11_40_17_370.jpeg";
	char *host = "7xi8d6.com1.z0.glb.clouddn.com";
	char httpstr[strlen(HTTPGET)+strlen(path)+strlen(host)+1];
	memset(httpstr, 0, sizeof(httpstr));
	sprintf(httpstr, HTTPGET, path, host);
	
	ret = write(sockfd , httpstr, strlen(httpstr)+1);
	if(ret < 0)
	{
		perror("write fail");
	}

	//读取服务器返回的数据
	char recvbuf[1024*10+190781]={0};
	ret = read(sockfd, recvbuf, sizeof(recvbuf));
	printf("%d--%s\n", ret, recvbuf);


	//去掉http头
	char *msg = strstr(recvbuf, "\r\n\r\n");//http头结束
	if(msg == NULL) return;

	msg+=4;//去数据开始位置
	printf("%s\n", msg);//如果msg为NULL---段错误

	
	int headsize = msg - recvbuf;
	

	FILE *file =  fopen("mic.jpg", "w+");
	fwrite(msg, ret-headsize, 1, file);

	
	while((ret = read(sockfd, recvbuf, sizeof(recvbuf)))>0)
	{
		fwrite(recvbuf, ret,1, file);
		printf("%d---\n", ret);
	}
	fclose(file);

	return 0;

}
