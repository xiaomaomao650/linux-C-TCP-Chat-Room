#include <stdio.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <string.h>
#include <sys/epoll.h>

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("using:app ip port\n");
		return -1;
	}
	
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
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);

	int ret = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if(ret < 0)
	{
		perror("connect fail:");
		close(sockfd);
		return -1;
	}

	//1.创建epoll事件
	int epfd = epoll_create(25);
	if(epfd < 0)
	{
		perror("epoll create fail");
		return -1;
	}
	//2.添加事件
	struct epoll_event evt;
	evt.events = EPOLLIN|EPOLLET;
	evt.data.fd = sockfd;

	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &evt);
	if(ret < 0)perror("add fail");
	
	evt.data.fd = 0;
        ret = epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &evt);
	if(ret < 0)perror("add fail");
	

	//存储事件结构体数组
	struct epoll_event ep[20];
	
	while(1)
	{
		ret = epoll_wait(epfd, ep, 20, -1);
		
		int i=0;
		for(i=0; i<ret; i++)
		{
			if(ep[i].data.fd == 0)
			{
				char sendbuf[1024]={0};
				scanf("%s", sendbuf);
				write(sockfd, sendbuf, strlen(sendbuf)+1);
			}else 
			if(ep[i].data.fd == sockfd)
			{
				char recvbuf[1024]={0};
				read(sockfd, recvbuf, 1024);
				printf("%s\n", recvbuf);
			}
		}
	}
	close(epfd);
	close(sockfd);
	return 0;
}
