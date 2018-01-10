#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SIZE 1024*20

//1.加入光标定位

void *task(void *p);
//服务器
int main(int argc, char **argv)
{
	int row = 0;
	
	if(argc < 2)
	{
		printf("using:recvfile port\n");
		return -1;
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("socket fail");
		return -1;
	}

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

	ret = listen(sockfd, 10);
	while(1)
	{
		struct sockaddr_in clientaddr;
		socklen_t len = sizeof(clientaddr);
		int newfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
		if(newfd < 0)
		{
			perror("accept fail");
			continue;
		}

		pthread_t pid = 0;
		pthread_create(&pid,NULL,task,(void*)&newfd);
		
	}	
	

	close(sockfd);
	return 0;
}

void *task(void *newfd)
{
	row++;
	int socktemp = *((int *)newfd);
	
	//接收文件头信息 文件名， 文件大小
	//【filesize:filename 】
	char recvbuf[SIZE]={0};
	int ret = read(socktemp, recvbuf, sizeof(recvbuf));
	if(ret < 0) 
	{
		perror("read info fail");
		return 0;
	}
	
	int filesize = 0;
	char filename[strlen(recvbuf)];
	memset(filename,0,sizeof(filename));

	//解析文件大小，文件名
	sscanf(recvbuf, "%d:%s", &filesize, filename);
	printf("%s---%d\n", filename, filesize);

	//打开文件
	FILE *file = fopen(filename, "w+");
	if(file == NULL){perror("fopen fail");}

	int recvsize=0;
	while(recvsize < filesize)
	{
		//读文件内容
		ret = read(socktemp, recvbuf, sizeof(recvbuf));
		if(ret < 0) 
		{
			perror("read info fail");
			return 0;
		}
		fwrite(recvbuf, 1, ret, file);
		recvsize += ret;
		
		printf("已接收%d%%\n", (int)(recvsize*100.0/filesize));
		printf("\033[nA");
	}	
	fclose(file);
	n--;
}
