#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(void)
{
	//1创建套接字
	int sockfd = socket(AF_INET, SOCK_DGRAM , 0);
	if(sockfd < 0)
	{
		perror("sockfd error");
		return -1;
	}
	IPPROTO_IP

	//发送数据
	struct sockaddr_in destaddr;
	memset(&destaddr, 0 , sizeof(destaddr));
	destaddr.sin_family = AF_INET;
	destaddr.sin_port = htons(6789);
	destaddr.sin_addr.s_addr = inet_addr("224.0.0.100");


	//开启广播
	int no =1;
	int ret = setsockopt(sockfd, SOL_SOCKET,SO_BROADCAST, &no, sizeof(no));
	if(ret < 0)perror("set fail:");


	char sndbuf[128]="hello udp";
	size_t size = sendto(sockfd, sndbuf, strlen(sndbuf)+1, 0, (struct sockaddr*)&destaddr, sizeof(destaddr));
	if(size <= 0)perror("sendto fail:");
	
	//等待接收数据----接收方反馈的数据
	memset(sndbuf, 0, sizeof(sndbuf));
	size = recvfrom(sockfd, sndbuf, 128, 0, NULL, NULL);
	printf("%s\n", sndbuf);

	return 0;
}
