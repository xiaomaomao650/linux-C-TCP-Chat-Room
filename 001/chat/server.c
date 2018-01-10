#include <stdio.h>  
#include <stdlib.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <unistd.h>  
#include <signal.h>  
#include <string.h>  
#include <pthread.h>  
#include <arpa/inet.h>  


const char * ip="127.0.0.1";
const int port=5678;
int sockfd;

typedef struct client
{
	char name[20];
	int sock;
}__Client;

__Client client[100];
int size = 0;

int server_init(void)
{
	printf("服务器初始化中\r\n");
	sleep(3);
	//socket
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
		perror("socket failed:");
		return -1;
	}
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port=htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	//绑定
	int ret= bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
	if(ret == -1)
	{
		perror("bind fail:");
		return -1;
	}
	//监听队列，做缓冲
	int res = listen(sockfd,100);
	if(res<0)
		perror("listen fail:");
	
	printf("server init success\n");
	
}



void *task(void *p)
{
	int socktemp = *((int *)p);
	char name[20]={0};
	char buf[100]={};
	int i,temp;
	
	if(read(client[size].sock,name,sizeof(name))>0)
		strcpy(client[size].name,name);
	temp = size;
	size++;
	char remind[100]={};
	sprintf(remind,"welcome %s join class....",client[size-1].name);
	sendmsgtoALL(remind);
	while(1)
	{
		if (read(socktemp,buf,sizeof(buf))<=0)
		{  
			char remind1[100]={};  
			sprintf(remind1,"%s退出聊天室",client[temp].name);  
			sendmsgtoALL(remind1);  
			for(i=0;i<size;i++)  
				if(client[i].sock == socktemp)
				{  
				   client[i].sock = 0;  
				} 
					break;
		}
		char msg[100]={0};
		sprintf(msg,"%s:%s\n",client[temp].name,buf);  
		sendmsgtoALL(msg);  
		memset(buf,0,strlen(buf));  
	}
}

int sendmsgtoALL(char *p)
{
	int i;
	i=size;
	while(i>=0)
	{
		write(client[i].sock, p, strlen(p)+1);
		i--;
	}
}


void server_start(void)
{
	printf("wait client connection.....\n");
	while(1)
	{
		//创建连接上来的客户端通信
		struct sockaddr_in caddr;
		socklen_t len = sizeof(caddr);
		//accept  connect
		int newfd = accept(sockfd, (struct sockaddr*)&caddr, &len);
		if(newfd < 0)
		{
			perror("accept fail:");
			continue;  //接收失败
		}
		client[size].sock=newfd;//接收成功，开启线程单独处理一个客户端
		pthread_t pid=0;
		pthread_create(&pid,NULL,task,(void*)&(client[size].sock));
	}
}

void close_server(void)
{
	printf("close server ing....");
	close(sockfd);
	exit(0);
	
}
int main(void)
{
	server_init();
	server_start();
	close_server();
}

