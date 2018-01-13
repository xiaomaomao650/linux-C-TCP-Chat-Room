#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#define CLIENTS 100

typedef struct ClientInfo
{
	int sockfd;
	int id;
}ClientInfo;

struct ClientInfo clients[CLIENTS]; //客户端信息数组
void *client_run(void *arg);

int main(int argc, char **argv)//./app port
{
	if(argc < 2)
	{
		printf("using:app port\n");
		return -1;
	}
	
	// daemon(0,0);

	memset(clients, 0, sizeof(clients));

	//1.创建套接字
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd  <  0)
	{
		perror("socket fail");
		return -1;
	}

	//2.绑定
	struct sockaddr_in seraddr;
	bzero(&seraddr, sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(atoi(argv[1]));
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int ret = bind(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
	if(ret < 0)
	{
		perror("bind fail:");
		close(sockfd);
		return -1;
	}

	//3.监听
	ret = listen(sockfd, 10);
	if(ret < 0) perror("listen fail");


	//保存客户端地址
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);
	int newfd = 0;
	while(1)
	{
		//4.接受连接
		newfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
		if(newfd < 0)
		{
			perror("accept fail");
			break;
		}
		//成功---启动线程单独处理一个客户端
	    printf("ip=%s\t port=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		
		
		pthread_t id = 0;
		int ret = pthread_create(&id, NULL, client_run, (void *)&newfd);
		pthread_detach(id);
		
		//把客户端信息记录在数组中 newfd
		
		int i = 0;
		for(i=0; i<CLIENTS; i++)
		{
			if(clients[i].sockfd == 0)
			{
				clients[i].sockfd = newfd;
				break;
			}
		}
	}
}


//[id:xxxx, to:xxxx, data:xxxx]---协议
void *client_run(void *arg)
{
	int newfd = *((int *)arg);
	char recvbuf[1024]={0};
	int selfid = 0;
	int toid = 0;
	char data[1024]={0};
	bool checkid = false;

	while(1)
	{
		int ret = read(newfd, recvbuf, sizeof(recvbuf));
		if(ret <= 0)
		{
			//客户端掉线
			int i=0;
			for(i=0; i<CLIENTS; i++)
			{
				if(clients[i].sockfd == newfd)
				{
					clients[i].sockfd = 0;
					clients[i].id = 0;
					break;
				}
			}
			break;
		}else
		{
			printf("%s\n",recvbuf);
			//解析数据
			//[id:xxxx,to:xxxx,data:xxxx]---协议
			sscanf(recvbuf, "id:%d,to:%d,data:%s", &selfid, &toid, data);
			
			//检测ID与套机字是否绑定
			if(!checkid)
			{
				int i=0; 
				for(i=0; i<CLIENTS; i++)
				{
					if(clients[i].sockfd == newfd)
					{
						clients[i].id = selfid;
						checkid = true;
						break;
					}
				}
			}

			//把数据发送给toid对对应的客户端（根据toid找到sockfd）
			int i=0;
			for(i=0; i<CLIENTS; i++)
			{
				if(clients[i].id == toid)
				{
					write(clients[i].sockfd, data, strlen(data)+1);
					break;
				}
			}
			if(i == CLIENTS)
			{
				char *msg = "你要找的客户端不存在";
				write(newfd,msg ,strlen(msg)+1);
			}
		}
	}
}

