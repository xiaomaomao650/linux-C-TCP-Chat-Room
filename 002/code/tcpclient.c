#include <stdio.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <string.h>
#include <pthread.h>


void *recv_handler(void *arg);
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


	

	//启动线程用来专门读取数据
	pthread_t id =0;
	ret = pthread_create(&id, NULL, recv_handler, (void *)&sockfd);
	pthread_detach(id);
	
	int selfid = 0;
	printf("selfid:");scanf("%d", &selfid);

	while(1)
	{	
		int toid = 0;
		char data[1024]={0};
		printf("toid:");scanf("%d", &toid);
		printf("data:");scanf("%s", data);
		char sendbuf[1024]={0};
		sprintf(sendbuf, "id:%d,to:%d,data:%s", selfid, toid, data);
		write(sockfd, sendbuf, strlen(sendbuf)+1);
	}
	close(sockfd);
	return 0;
}
void *recv_handler(void *arg)
{
	int sockfd= *((int *)arg);
	//读取客户端数据
	char recvbuf[1024]={0};

	while(1)
	{
		int ret = read(sockfd, recvbuf, 1024);//阻塞
		if(ret <= 0)
		{
			//客户端掉线
			break;
		}
		printf("recvbuf = %s\n", recvbuf);
		memset(recvbuf, 0, sizeof(recvbuf));
	}
}
