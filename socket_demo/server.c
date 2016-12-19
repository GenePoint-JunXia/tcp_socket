#include "unp_xj.h"
#include <time.h>

#define m_PORT 6666
#define MAX_LINE 4096

int main(int argc,char **argv){
	int listenfd,confd;
	socklen_t len;
	struct sockaddr_in seraddr,cliaddr;
	char sendbuff[MAX_LINE];
	char recvbuff[MAX_LINE];
	time_t ticks;

	if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0){
		printf("socket error!\n");
		return 0;
	}

	bzero(&seraddr,sizeof(seraddr));
	seraddr.sin_family=AF_INET;
	seraddr.sin_addr.s_addr=htonl(INADDR_ANY);
	seraddr.sin_port=htons(m_PORT);
	
	if(bind(listenfd,(struct sockaddr*)&seraddr,sizeof(seraddr))<0){
		printf("bind error!\n");
		return 0;
	}
	
	printf("Listening...\n");	
	if(listen(listenfd,5)<0){
		printf("listen error!\n");
		return 0;
	}

	while(1){
		
		len=sizeof(cliaddr);
		if((confd=accept(listenfd,(struct sockaddr*)&cliaddr,&len))<0){
			printf("accept error!\n");
			return 0;
		}
		printf("connection from %s,port %d\n",
		      inet_ntop(AF_INET,&cliaddr.sin_addr,recvbuff,sizeof(recvbuff)),ntohs(cliaddr.sin_port));		
		while(1){
			int bufLen=0;
			if((bufLen=recv(confd,recvbuff,MAX_LINE,0))<0){
				printf("recv error!\n");
				return -1;
			}
			recvbuff[bufLen]='\0';
			printf("\nRECEIVE : %s",recvbuff);
			if(strcmp(recvbuff,"bye\n")==0){
				printf("\nCLOSE THIS CONNECTION.\n\nListening...\n");
				break;
			}			
	
			printf("INPUT : ");
			fgets(sendbuff,MAX_LINE,stdin);	
			send(confd,sendbuff,strlen(sendbuff),MSG_NOSIGNAL);
			if(strcmp(sendbuff,"bye\n")==0){
				printf("\nCLOSE THIS CONNECTION.\n\nListening...\n");
				break;
			}
		}
		close(confd);
	}
	close(listenfd);
	return 0;
}
