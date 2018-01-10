#include <stdio.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <string.h>


int main(int argc, char **argv)
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
	servaddr.sin_addr.s_addr = inet_addr("220.170.186.167");

	int ret = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if(ret < 0)
	{
		perror("connect fail:");
		close(sockfd);
		return -1;
	}

	//发送http请求
	char *httpstr = "GET  /open/api/weather/json.shtml?city=广州 HTTP/1.1\r\nHost:www.sojson.com\r\nConnection:close\r\n\r\n";

	ret = write(sockfd , httpstr, strlen(httpstr)+1);
	if(ret < 0)
	{
		perror("write fail");
	}

	//读取服务器返回的数据
	char recvbuf[1024*10]={0};

	ret = read(sockfd, recvbuf, sizeof(recvbuf));


	printf("%s\n", recvbuf);

	close(sockfd);

	return 0;

}
	

