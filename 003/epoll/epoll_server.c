#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>


int main(void)
{
	//1.创建套接字
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) 
	{
		perror("socket fail");
		return -1;
	}


	//2.绑定
	struct sockaddr_in myaddr;//本地-地址
	memset(&myaddr, 0 ,sizeof(myaddr));//清零
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(6789);
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定IP
	int ret = bind(sockfd, (struct sockaddr*)&myaddr, sizeof(myaddr));
	if(ret < 0)
	{
		perror("bind fail:");
		close(sockfd);
		return -1;
	}


	//3.监听--端口
	listen(sockfd, 10);

	//epoll实现多客户端服务器
	//1.创建epoll事件
	int epfd = epoll_create(255);
	//2.添加文件描述符（事件）
	struct epoll_event evt;
	evt.events = EPOLLIN|EPOLLET;
	evt.data.fd = sockfd;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &evt);

	//4.接受客户端连接
	struct sockaddr_in caddr;//用来保存客户端地址
	socklen_t len = sizeof(caddr);

	//保存事件的结构体数组
	struct epoll_event  epvt[20];
	while(1)
	{
		int epoll_wait_ret = epoll_wait(epfd, epvt, 20, -1);
		if(epoll_wait_ret <= 0)continue;
		printf("ret = %d\n", epoll_wait_ret);
	
		int i=0;
		for(i=0; i<epoll_wait_ret; i++)
		{
			if(epvt[i].data.fd == sockfd)//判断是否有客户端连接
			{
				//接受客户端连接创建新套接字--用来与客户端通信
				int newfd = accept(sockfd, (struct sockaddr*)&caddr, &len);//如果没有客户端连接，函数阻塞
				if(newfd < 0)
				{
					perror("accept fail");
				}
				//输出客户端地址
				printf("ip=%s\t port=%d\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));
				
				//把newfd 添加到epoll事件中
				
				evt.data.fd = newfd;
				epoll_ctl(epfd, EPOLL_CTL_ADD, newfd, &evt);

			}else
			{
				int newfd = epvt[i].data.fd ;
				//读取客户端数据
				char recvbuf[1024]={0};
				ret = read(newfd, recvbuf, 1024);//阻塞
				if(ret < 0)
				{
					//客户端下线
					printf("******\n");
					epoll_ctl(epfd, EPOLL_CTL_DEL, newfd, &epvt[i]);
					close(newfd);
					continue;
				}
				printf("recvbuf = %s\n", recvbuf);
			}
		}
	}

	close(epfd);
	
	close(sockfd);//关闭监听套接字
	return 0;
}
