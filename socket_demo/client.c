#include "unp_xj.h"

#define MAX_LINE 4096
#define m_PORT 6666

int main(int argc,char **argv){
	int sockfd,n;
	char recvline[MAX_LINE];
	char sendbuff[MAX_LINE];
	struct sockaddr_in server_addr;
	
	if(argc!=2){
		printf("usage:a.out <IPAddress>");
		return 0;
	}
	
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
		printf("socket error!\n");
		return 0;
	}
	
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(m_PORT);

	if(inet_pton(AF_INET,argv[1],&server_addr.sin_addr)<0){
		printf("inet_pton error!\n");
		return 0;
	}

	if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
		printf("connect error!\n");
		return 0;
	}

	while(1){
	
		printf("INPUT : ");
		fgets(sendbuff,MAX_LINE,stdin);

		send(sockfd,sendbuff,strlen(sendbuff),MSG_NOSIGNAL);
		if(strcmp(sendbuff,"bye\n")==0){
			break;
		}	
	
		int bufLen=recv(sockfd,recvline,MAX_LINE,0);
		recvline[bufLen]='\0';

		printf("RECEIVED : %s\n",recvline);
		if(strcmp(recvline,"bye\n")==0){
			break;
		}	
		
	}
	return 0;
}
