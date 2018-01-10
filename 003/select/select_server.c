#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>


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

	//保存最大文件描述符
	int maxfd = sockfd;
	//读文件文件描述符容器
	fd_set readfds;


	//4.接受客户端连接
	struct sockaddr_in caddr;//用来保存客户端地址
	socklen_t len = sizeof(caddr);


	//保存客户端通信的套接字
	int clientfds[100] = {0};

	while(1)
	{
		//把要监听的文件描述符添加到容器中
		maxfd = sockfd;
		FD_SET(sockfd, &readfds);
		int i=0;
		for(i=0; i<100; i++)
		{
			if(clientfds[i] != 0)
			{
				FD_SET(clientfds[i], &readfds);
				maxfd = maxfd>clientfds[i]?maxfd:clientfds[i];
			}
		}

		int select_ret = select(maxfd+1, &readfds, NULL, NULL, NULL);
		if(select_ret < 0)continue;
	
		if(FD_ISSET(sockfd, &readfds))//判断是否有客户端连接
		{
			//接受客户端连接创建新套接字--用来与客户端通信
			int newfd = accept(sockfd, (struct sockaddr*)&caddr, &len);//如果没有客户端连接，函数阻塞
			if(newfd < 0)
			{
				perror("accept fail");
			}
			//输出客户端地址
			printf("ip=%s\t port=%d\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));
			
			//把与客户端通信的套接字保存在clientfds
			int i=0; 
			for(i=0; i<100; i++)
			{
				if(clientfds[i] == 0) 
				{
					clientfds[i] = newfd;
					break;
				}
			}
		}

		for(i=0; i<100; i++)
		{
			if(FD_ISSET(clientfds[i], &readfds))
			{
				//读取客户端数据
				char recvbuf[1024]={0};
				int ret = read(clientfds[i], recvbuf, 1024);//阻塞
				if(ret < 0)
				{
					//客户端下线
					close(clientfds[i]);
					continue;
				}
				printf("recvbuf = %s\n", recvbuf);
			}
		}
		
	}

	close(sockfd);//关闭监听套接字
	return 0;
}
