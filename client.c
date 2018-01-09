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




int client_init(void)
{

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
	
	int ret= connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
	if(ret == -1)
	{
		perror("connect fail:");
		return -1;
	}
	
	printf("connect server success\n");
	
}


int *task(void *p)
{
	while(1)
	{
		char buf[100]={};
		if(read(sockfd,buf,sizeof(buf))<=0)
			return -1;
		printf("%s\r\n",buf);
		memset(buf,0,sizeof(buf));
	}
}


//线程用来读数据，主函数用来写数据
void client_start(void)
{
	pthread_t pid=0;
	pthread_create(&pid,NULL,task,(void *)&sockfd);
	char msg[100]={};
	while(1)
	{
		scanf("%s",msg);
		write(sockfd,msg,strlen(msg));
		memset(msg,0,strlen(msg));
	}
}

void close_client(void)
{
	printf("close server ing....");
	close(sockfd);
	exit(0);
	
}
int main(void)
{
	char name[100];
	printf("please input client name:");
	scanf("%s",name);
	client_init();
	write(sockfd,name,strlen(name));
	client_start();
}

