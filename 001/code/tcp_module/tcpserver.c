#include <arpa/inet.h>
//#include <linux/in.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>


int main(void)
{
	//1.创建套接字
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) 
	{
		perror("socket fail");
		return -1;
	}


	//2.绑定
	struct sockaddr_in myaddr;//本地-地址
	memset(&myaddr, 0 ,sizeof(myaddr));//清零
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(5678);
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定IP
	int ret = bind(sockfd, (struct sockaddr*)&myaddr, sizeof(myaddr));
	if(ret < 0)
	{
		perror("bind fail:");
		close(sockfd);
		return -1;
	}


	//3.监听--端口
	listen(sockfd, 10);

	//4.接受客户端连接
	struct sockaddr_in caddr;//用来保存客户端地址
	socklen_t len = sizeof(caddr);
	//接受客户端连接创建新套接字--用来与客户端通信
	int newfd = accept(sockfd, (struct sockaddr*)&caddr, &len);//如果没有客户端连接，函数阻塞
	if(newfd < 0)
	{
		perror("accept fail");
	}

	//输出客户端地址
	printf("ip=%s\t port=%d\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));

	//读取客户端数据
	char recvbuf[1024]={0};
	ret = read(newfd, recvbuf, 1024);//阻塞

	printf("recvbuf = %s\n", recvbuf);
	
	
	char *p = "hello world\r\n";

	write(newfd, p, strlen(p)+1);


	close(sockfd);//关闭监听套接字
	return 0;
}
