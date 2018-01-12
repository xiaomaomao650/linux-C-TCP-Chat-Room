#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "cJSON.h"


int cJSON_job(char *msg);



int main(void)
{
	//1创建套接字
	int sockfd = socket(AF_INET, SOCK_DGRAM , 0);
	if(sockfd < 0)
	{
		perror("sockfd error");
		return -1;
	}

	//绑定
	struct sockaddr_in destaddr;
	memset(&destaddr, 0 , sizeof(destaddr));
	destaddr.sin_family = AF_INET;
	destaddr.sin_port = htons(20183);
	destaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret =  bind(sockfd, (struct sockaddr*)&destaddr, sizeof(destaddr));

	char recvbuf[200]={0};
	
	struct sockaddr saddr;
	socklen_t len = sizeof(saddr);

	while(1)
	{
		size_t size = recvfrom(sockfd, recvbuf, sizeof(recvbuf), 0, &saddr, &len);
		printf("%s\n", recvbuf);
		
		cJSON_job(recvbuf);
		//发送反馈信息
		
		size = sendto(sockfd, "recv ok", 9, 0, &saddr, len);
	}
	close(sockfd);

	return 0;
}


int cJSON_job(char *msg)
{
	cJSON *root = cJSON_Parse(msg);
	
	cJSON *name = cJSON_GetObjectItem(root,"name");
	printf("name:%s\n", name->valuestring);
	
	cJSON *sex = cJSON_GetObjectItem(root,"sex");
	printf("sex:%s\n", sex->valuestring);
	
	cJSON *number = cJSON_GetObjectItem(root,"number");
	printf("number:%s\n", number->valuestring);
	
	cJSON *qq = cJSON_GetObjectItem(root,"qq");
	printf("qq:%s\n", qq->valuestring);
	
	cJSON *phone = cJSON_GetObjectItem(root,"phone");
	printf("phone:%s\n", phone->valuestring);
}