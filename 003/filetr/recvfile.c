#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define SIZE 1024


void *recv_run(void *arg);

int row = 1;

//服务器
int main(int argc, char **argv)
{
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

	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);
	int newfd = 0;

	//printf("\033[2J");//清屏
	while(1)
	{
		newfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
		if(newfd < 0)
		{
			perror("accept fail");
			continue;
		}
		
		//创建线程
		pthread_t id = 0;
		ret = pthread_create(&id, NULL, recv_run,(void*)&newfd);
		if(ret < 0) continue;
		//分离
		pthread_detach(id);
		usleep(10);
		row++;
	}

	close(sockfd);
	return 0;
}

void *recv_run(void *arg)
{
	int newfd = *((int*)arg);
	char buf[10]={0};
	//sprintf(buf,"\033[%d;0H", row);

	//接收文件头信息 文件名， 文件大小
	//【filesize:filename 】
	char recvbuf[SIZE]={0};
	int ret = read(newfd, recvbuf, sizeof(recvbuf));
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
		ret = read(newfd, recvbuf, sizeof(recvbuf));
		if(ret <= 0) 
		{
			perror("read data fail");
			break;
		}
		fwrite(recvbuf, 1, ret, file);
		recvsize += ret;
		//printf("%s", buf);
	}	
	printf("接收文件%s---%d\n",filename, recvsize);
	fclose(file);
	close(newfd);
	return 0;
}


