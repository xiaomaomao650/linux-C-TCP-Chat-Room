#include <stdio.h>
#include "cJson.h"
#include <string.h>
#include <sys/socket.h>
#include <linux/in.h>


int main(void)
{

/*
	FILE * file = fopen("./json.txt", "r");
	fseek(file, 0, SEEK_END);
	int len = ftell(file);
	rewind(file);

	char msg[len+1];
	memset(msg, 0, sizeof(msg));
	//读取文件中的数据
	fread(msg, 1, len, file);
	fclose(file);
*/
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
	servaddr.sin_port = htons(80);
	servaddr.sin_addr.s_addr = inet_addr("220.170.186.167");

	int ret = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if(ret < 0)
	{
		perror("connect fail:");
		close(sockfd);
		return -1;
	}

	//发送http请求
	char *httpstr = "GET  /open/api/weather/json.shtml?city=广州 HTTP/1.1\r\nHost:www.sojson.com\r\nConnection:close\r\n\r\n";

	ret = write(sockfd , httpstr, strlen(httpstr)+1);
	if(ret < 0)
	{
		perror("write fail");
	}

	//读取服务器返回的数据
	char recvbuf[1024*10]={0};

	ret = read(sockfd, recvbuf, sizeof(recvbuf));
	
	//去掉http头
	//char *msg = strstr(recvbuf, "\r\n\r\n");
	char *msg = strstr(recvbuf, "{");
	printf("%s\n", msg);

	
	


	//开始解析
	cJSON *root = cJSON_Parse(msg);
	//根据key获取values
	cJSON *date = cJSON_GetObjectItem(root, "date");
	printf("%s\n", date->valuestring);

	

	//解析今天天气
	cJSON *data = cJSON_GetObjectItem(root, "data");
	cJSON *forecastArray = cJSON_GetObjectItem(data, "forecast");
	
	int arraySize = cJSON_GetArraySize(forecastArray);
	printf("arraySize=%d\n", arraySize);

	//从数组中获取第一天天气
	int i=0;
	for(i=0; i<arraySize; i++)
	{
		cJSON *Date = cJSON_GetArrayItem(forecastArray, i);
		cJSON *fdate = cJSON_GetObjectItem(Date, "date");
		printf("%s\n", fdate->valuestring);

		cJSON *high = cJSON_GetObjectItem(Date, "high");
		printf("%s\n", high->valuestring);

		cJSON *low = cJSON_GetObjectItem(Date, "low");
		printf("%s\n", low->valuestring);

		printf("***************************************************\n");
	}

	return 0;

}
