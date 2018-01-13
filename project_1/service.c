#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <unistd.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>


typedef  struct sockinfo
{
	int id;
	int sock;
	struct sockinfo *next;
	
}sockinfo;

sockinfo *head;

struct sockinfo *init()
{
	struct sockinfo* new =malloc(sizeof(sockinfo));
	new->id=0;
	new->sock=0;
	new->next=NULL;
	return new;
}
void insert(struct sockinfo*head,int newsock,int id)
{
	     struct sockinfo* p=init();
	     p->sock = newsock;
		 p->id = id;
		// printf("new insert port%d\n",p->id);
		 struct sockinfo *tmp = head;
		 while(tmp->next!=NULL)
			 tmp = tmp->next;
		      tmp->next = p; 
}

void *rec_trans(void*arg)
{
	int newsocket = (*(int*)(arg));
	char buf[128],buf1[128];
	int rcont,port;
	char *p=NULL;
	struct sockinfo* tmp=NULL;
	bool flag=true;
	char *bk="exit";
	char *k="sessim is over";
	int rret=1;
	
	
	while(1)
	{
		memset(buf,0,sizeof(buf));
		memset(buf1,0,sizeof(buf1));
		rcont=read(newsocket,buf,sizeof(buf));
		if(rcont <=0)
			{
					tmp=head;
					while(tmp->next!=NULL)
					{
						tmp=tmp->next;
						if(tmp->sock==newsocket)
							printf("free socket %d",newsocket);
						free(tmp);
					}
				
			}
		if(rcont<=0)
		{
			printf("%d client lost connect!\n",newsocket);
			break;
		}
		p=strchr(buf,' ');
		if(p==NULL)
			continue;
		
		memcpy(buf1,buf,p-buf);
		buf1[p-buf]='\0';
		port=atoi(buf1);
		tmp=head;
		while(tmp->next!=NULL)
		{
			tmp=tmp->next;
			if(tmp->id==port)
			{
				port=0;
				while(1)
				{
					if(flag)
					{
						flag=false;
					   write(tmp->sock,p+1,strlen(p+1));
					}
				   else
				   {
					   rcont=read(newsocket,buf,sizeof(buf));
					   if(rcont <= 0)
					   	{
					   		free(tmp);
					   		break;
					   	}
					   rret=memcmp(buf,bk,4);
					   if(rret==0)
					   {
						    
						   write(tmp->sock,k,strlen(k)+1);
						   flag=true;
						   break;
					   }
				       write(tmp->sock,buf,strlen(buf)+1);
					   memset(buf,0,sizeof(buf));
				   }
				}
				break;
			}
			if(tmp->next==NULL)
				write(newsocket,"not found the client!\n",23);
		}
		
	}
}


int main(int argc,char *argv[])
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	 if(sockfd<0)
		 perror("socket fail!\n");
	 
	 
	 struct sockaddr_in addr;
	 addr.sin_family=AF_INET;
	 addr.sin_port=htons(atoi(argv[1]));
	 addr.sin_addr.s_addr=htonl(INADDR_ANY);
	 
	 int ret = bind(sockfd,(struct sockaddr *)&addr,sizeof(addr));
	 if(ret!=0)
		 perror("bind fail!\n");
	 
	 ret =listen(sockfd,10);
	 if(ret!=0)
		 perror("listen fail!\n");
	 
	 struct sockaddr_in client;
	 socklen_t len = sizeof(client);
	
	 int newsocket;
	 head=init();
	 pthread_t pid=0;
	 
	 while(1)
	 {
		 
		newsocket = accept(sockfd,(struct sockaddr*)&client,&len);
		if(ret==-1)
		 perror("accept fial!\n");
	    printf("new port link =%d\n",ntohs(client.sin_port));
		insert(head,newsocket,ntohs(client.sin_port));
		pthread_create(&pid,NULL,rec_trans,(void*)&newsocket);
		pthread_detach(pid);
		 
	 }
	 
	 
	 return 0;
}
