 #include<stdio.h>
 #include<stdlib.h>
 #include<string.h>
 #include<errno.h>
 #include<sys/types.h>
 #include<sys/socket.h>
 #include<netinet/in.h>
 
 #define MAXLINE 4096

 typedef struct
 {
	 char rtsp_url[128];
	 int num;
 }rtsp_info;

 
 int main(int argc, char** argv)
 {
     int    sockfd, n;
     char    recvline[300], sendline[300];
     struct sockaddr_in    servaddr;

	rtsp_info online_rtsp_info;
	strcpy(online_rtsp_info.rtsp_url , "http://www.baidu.com111");
	online_rtsp_info.num = 5;
	 
 #if 0
     if( argc != 2){
     printf("usage: ./client <ipaddress>\n");
     exit(0);
     }
 #endif
 
     if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
     printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
     exit(0);
     }
 
     memset(&servaddr, 0, sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     servaddr.sin_port = htons(6668);
#if 0
     if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
     printf("inet_pton error for %s\n",argv[1]);
     exit(0);
     }
#endif

	if( inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0){
     printf("inet_pton error for %s\n",argv[1]);
     exit(0);
     }	

     if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
     printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
     exit(0);
     }
 
     printf("send msg to server: \n");
     //fgets(sendline, 300, stdin);
     //if( send(sockfd, sendline, strlen(sendline), 0) < 0)
     if( send(sockfd, (char *)&online_rtsp_info, sizeof(rtsp_info), 0) < 0)
     {
     printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
     exit(0);
     }
 
     close(sockfd);
     exit(0);
 }


