#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "cJSON.h"

char *json_pack();

int main(void)
{
	//1创建套接字
	int sockfd = socket(AF_INET, SOCK_DGRAM , 0);
	if(sockfd < 0)
	{
		perror("sockfd error");
		return -1;
	}


	//发送数据
	struct sockaddr_in destaddr;
	memset(&destaddr, 0 , sizeof(destaddr));
	destaddr.sin_family = AF_INET;
	destaddr.sin_port = htons(20183);
	destaddr.sin_addr.s_addr = inet_addr("192.168.6.255");

	// char sndbuf[150]="{\"name\":\"王俊\",\"sex\":\"男\",\"number\":\"20171225008\",\"qq\":\"597349205\",\"phone\":\"18587515530\"}"; 
	
	char * sndbuf=json_pack();
	//开启广播
	int no =1;
	int ret = setsockopt(sockfd, SOL_SOCKET,SO_BROADCAST, &no, sizeof(no));
	if(ret < 0)perror("set fail:");


	size_t size = sendto(sockfd, sndbuf, strlen(sndbuf)+1, 0, (struct sockaddr*)&destaddr, sizeof(destaddr));
	if(size <= 0)perror("sendto fail:");
	
	//等待接收数据----接收方反馈的数据
	memset(sndbuf, 0, sizeof(sndbuf));
	size = recvfrom(sockfd, sndbuf, 128, 0, NULL, NULL);
	printf("%s\n", sndbuf);

	return 0;
}

char *json_pack()
{
	cJSON * root = cJSON_CreateObject();
	if(!root)
	{
	   perror("cjson_createobject fail:");
	   return -1;
	}
	cJSON_AddNumberToObject(root,"name",cJSON_AddStringToObject("王俊")); 
	cJSON_AddNumberToObject(root,"sex",cJSON_AddStringToObject("男")); 
	cJSON_AddNumberToObject(root,"number",cJSON_AddStringToObject("20171225008")); 
	cJSON_AddNumberToObject(root,"qq",cJSON_AddStringToObject("597349205")); 
	cJSON_AddNumberToObject(root,"phone",cJSON_AddStringToObject("18587515530")); 
	
	char *msg;
	msg = cJSON_Print(root);
	cJSON_Delete(root);   
	
	return msg;
}