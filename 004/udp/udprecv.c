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

	//绑定
	struct sockaddr_in destaddr;
	memset(&destaddr, 0 , sizeof(destaddr));
	destaddr.sin_family = AF_INET;
	destaddr.sin_port = htons(6789);
	destaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret =  bind(sockfd, (struct sockaddr*)&destaddr, sizeof(destaddr));


	char recvbuf[128]={0};
	struct sockaddr saddr;
	socklen_t len = sizeof(saddr);

	size_t size = recvfrom(sockfd, recvbuf, sizeof(recvbuf), 0, &saddr, &len);
	printf("%s\n", recvbuf);

	//发送反馈信息
	
	size = sendto(sockfd, "recv msg", 9, 0, &saddr, len);
	
	close(sockfd);

	return 0;
}
