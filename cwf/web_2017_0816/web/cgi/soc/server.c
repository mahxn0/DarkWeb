#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define MAXLINE 300


typedef struct
{
	char rtsp_url[128];
	int num;
}rtsp_info;


int main(int argc, char** argv)
{
    int    listenfd, connfd;
    struct sockaddr_in     servaddr;
    char    buff[300];
    int     n;

	rtsp_info online_rtsp_info;

	
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
    printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
     if( inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0){
     printf("inet_pton error for %s\n",argv[1]);
     exit(0);
     }
    servaddr.sin_port = htons(6668);

    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
    printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }

    if( listen(listenfd, 10) == -1){
    printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }

    printf("======waiting for client's request======\n");

	
    while(1){
    if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
        continue;
    }
    n = recv(connfd, buff, MAXLINE, 0);
    //buff[n] = '\0';
    memcpy(&online_rtsp_info , buff , sizeof(rtsp_info));
    printf("rtsp_url=%s  num=%d\n", online_rtsp_info.rtsp_url , online_rtsp_info.num);
	memset(&online_rtsp_info, 0 , sizeof(online_rtsp_info));
	memset(buff, 0 , sizeof(buff));
    close(connfd);
    }

    close(listenfd);
}


