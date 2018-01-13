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
	servaddr.sin_addr.s_addr = inet_addr("112.74.37.132");

	int ret = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if(ret < 0)
	{
		perror("connect fail:");
		close(sockfd);
		return -1;
	}


	char buf[1024]="hello network";

	ret = write(sockfd, buf, strlen(buf)+1);


	ret = read(sockfd, buf, sizeof(buf));
	printf("%s\n", buf);
	
	close(sockfd);
	return 0;
}
