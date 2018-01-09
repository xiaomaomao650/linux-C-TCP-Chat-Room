#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define SIZE 1024*20


//客户端  sendfile ip port  filepath
int main(int argc, char **argv)
{
	if(argc < 4)
	{
		printf("using:sendfile ip port filepath\n");
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
	seraddr.sin_port = htons(atoi(argv[2]));
	seraddr.sin_addr.s_addr = inet_addr(argv[1]);

	int ret = connect(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
	if(ret < 0)
	{
		perror("connect fail");
		close(sockfd);
		return -1;
	}

	//发送文件信息（文件大小， 文件名）[filesize:filename]  /home/gec/my.txt, my.txt
	char *filename = argv[3]+strlen(argv[3]);
	while(*filename != '/'  &&  filename!=argv[3] )filename--;
	if(*filename == '/')filename++;

	//文件大小
	FILE *file = fopen(argv[3], "r");
	if(file == NULL)
	{
		perror("fopen fail:");
		close(sockfd);
		return -1;
	}

	fseek(file, 0 , SEEK_END);//偏移指针
	int filesize = ftell(file);//获取偏移量
	rewind(file);//把文件指针移动文件头


	char sendbuf[SIZE] ={0};
	sprintf(sendbuf, "%d:%s", filesize, filename);

	write(sockfd, sendbuf, strlen(sendbuf)+1);
	
	int sendsize = 0;
	while(1)
	{
		ret = fread(sendbuf, 1, SIZE, file);
		if(ret == 0)
		{
			break;
		}
		write(sockfd, sendbuf, ret);
		sendsize += ret;

		printf("已发送%d%%\n", (int)(sendsize*100.0/filesize));
		printf("\033[1A");
	}	
	fclose(file);

	sleep(1);
	close(sockfd);
	return 0;
}
