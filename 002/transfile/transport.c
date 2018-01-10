#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>


int main(int argc,char **argv)
{
	if(argc<2)
	{
		printf("");
		return -1;
	}
	
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0) 
	{
		perror("socket fail");
		return -1;
	}
	
	struct sockaddr_in seraddr;//本地-地址
	memset(&seraddr, 0 ,sizeof(seraddr));//清零
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(atoi(argv[1]));
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定IP
	int ret = bind(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
	if(ret < 0)
	{
		perror("bind fail:");
		close(sockfd);
		return -1;
	}
	
	ret =listen(sockfd,10);
	
	struct sockaddr_in clientaddr;//用来保存客户端地址
	socklen_t len = sizeof(clientaddr);
	//接受客户端连接创建新套接字--用来与客户端通信
	int newfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);//如果没有客户端连接，函数阻塞
	if(newfd < 0)
	{
		perror("accept fail");
	}
	
	int filesize = 0;
	char recvbuf[1024]={0};
	ret = read(newfd,recvbuf,sizeof(recvbuf));
	if(ret<0)
	{
		perror("read fail:");
		return 0;
	}

	char filename[strlen(recvbuf)];
	memset(filename, 0 ,sizeof(filename));//清零
	//解析文件大小，文件名
	sscanf(recvbuf,"%d:%s",&filesize,filename);
	printf("%s-----%d\n",filename,filesize);
	
	FILE *file =fopen(filename,"w+");
	if(file == NULL)
	{
		perror("fopen faile:");
	}
	
	int recvsize=0;
	while(recvsize<filesize)
	{
		ret = read(newfd,recvbuf,sizeof(recvbuf));
		if(ret<0)
		{
			perror("read info fai:");
			return 0;
		}
		fwrite(recvbuf,1,ret,file);
		recvsize+=ret;
		
	}
	
	fclose(file);
	
	close(sockfd);
	close(newfd);
}