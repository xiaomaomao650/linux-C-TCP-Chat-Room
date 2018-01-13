#include <stdio.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <string.h>

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



	char buf[20]={0};
	char userbuf[100]={0};
	while(1)
	{
		memset(userbuf, 0, sizeof(userbuf));
		printf("input id:xxxx,to:xxxx,data:open/close\n");
		scanf("%s",userbuf);
	
		ret = write(sockfd, userbuf, strlen(userbuf)+1);
			
		
		read(sockfd, buf, sizeof(buf));
		printf("%s\n", buf);
	}
	close(sockfd);
	return 0;
}