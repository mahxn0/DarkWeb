#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<sys/time.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#include "./xml_op.h"

#define MAXLINE 300

int roll_time = 0;
char enable[8][10] = {0};
char jc_enable[8][10] = {0};
char rtsp_url[8][320] = {0};

static int now_no = 0;
static int pre_no = 0;

int no_all = 0;
int no_all_sz[8] = {0};
int modify_flag = -1;
int num_total;

pthread_t thread[3];

typedef struct
{
	char rtsp_url[256];
	int num;
}rtsp_info;



void signal_handler()
{
	int i;
	int    sockfd, n;
    char    recvline[300], sendline[256];
    struct sockaddr_in    servaddr;

	rtsp_info online_rtsp_info;
	//strcpy(online_rtsp_info.rtsp_url , "http://www.baidu.com111");
	//online_rtsp_info.num = 5;

	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	 printf("create socket error\n");
	 return;
	 }
 
	 memset(&servaddr, 0, sizeof(servaddr));
	 servaddr.sin_family = AF_INET;
	 servaddr.sin_port = htons(6669);
	 if( inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0){
     printf("inet_pton error\n");
     return;
     }	
	 	
	if(modify_flag == -1)
	{
		modify_flag = 0;
		if(no_all != 0)
		{
			now_no = 1;
			//跟算法对接 no_all_sz[0]
			printf("switch to no %d\n", no_all_sz[0]);
			memset(&sendline, 0, sizeof(sendline));
			sprintf(sendline, "rtspsrc location=\"%s\" latency=0 ! rtph264depay ! h264parse ! omxh264dec ! videoconvert ! appsink", rtsp_url[no_all_sz[0]-1]);
			strcpy(online_rtsp_info.rtsp_url , sendline);
			online_rtsp_info.num = no_all_sz[0];
			
			if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
		     printf("connect error\n");
		     return;
		     }
		     if( send(sockfd, (char *)&online_rtsp_info, sizeof(rtsp_info), 0) < 0)
		     {
		     printf("send msg error\n");
		     return;
		     }
		}
	}
	else if(modify_flag == 1)
	{
		modify_flag = 0;
		//停止之前的算法，从头开始轮询
		if(no_all != 0)
		{
			now_no = 1;
			//跟算法对接 no_all_sz[0]
			printf("switch to no %d\n", no_all_sz[0]);
			memset(&sendline, 0, sizeof(sendline));
			
			sprintf(sendline, "rtspsrc location=\"%s\" latency=0 ! rtph264depay ! h264parse ! omxh264dec ! videoconvert ! appsink", rtsp_url[no_all_sz[0]-1]);
			strcpy(online_rtsp_info.rtsp_url , sendline);
			online_rtsp_info.num = no_all_sz[0];
			if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
		     printf("connect error\n");
		     return;
		     }
		     if( send(sockfd, (char *)&online_rtsp_info, sizeof(rtsp_info), 0) < 0)
		     {
		     printf("send msg error\n");
		     return;
		     }
		}
		
	}
	else if(modify_flag == 0)
	{
		if(now_no < no_all)
		{
			if(no_all != 1)
			{
				printf("switch to no %d\n", no_all_sz[now_no]);

				printf("rtsp_url[no_all_sz[0]-1] =%d    %s\n",no_all_sz[now_no]-1,rtsp_url[no_all_sz[now_no]-1]);
				
				//跟算法对接,切换 no_all_sz[now_no]
				memset(&sendline, 0, sizeof(sendline));
				sprintf(sendline, "rtspsrc location=\"%s\" latency=0 ! rtph264depay ! h264parse ! omxh264dec ! videoconvert ! appsink", rtsp_url[no_all_sz[now_no]-1]);
				strcpy(online_rtsp_info.rtsp_url , sendline);
				online_rtsp_info.num = no_all_sz[now_no];
				if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
			     printf("connect error\n");
			     return;
			     }
			     if( send(sockfd, (char *)&online_rtsp_info, sizeof(rtsp_info), 0) < 0)
			     {
			     printf("send msg error\n");
			     return;
			     }
			}
			now_no++;
			if(now_no == no_all)
				now_no = 0;
		}
	}
    close(sockfd);
	alarm(roll_time);
}


void read_xml(void)
{
	char value[320] = {0};
	char name[32] = {0};
	int i , j = 0;

	memset(value, 0 ,sizeof(value));
	get_from_xml("rtsp.setting_total",value);
	num_total = atoi(value);

	memset(value, 0 ,sizeof(value));
	no_all = 0;
	memset(no_all_sz, 0 ,sizeof(no_all_sz));
	get_from_xml("rtsp.roll_time",value);
	roll_time = atoi(value);
	//printf("roll_time = %d\n" , roll_time);
	for(i = 0 ; i < num_total ; i++)
	{
		memset(name, 0 ,sizeof(name));
		sprintf(name , "rtsp.enable%d" ,i+1);
		memset(value, 0 ,sizeof(value));
		get_from_xml(name,value);
		strcpy(enable[i] , value);	


		memset(name, 0 ,sizeof(name));
		sprintf(name , "rtsp.jc_enable%d" ,i+1);
		memset(value, 0 ,sizeof(value));
		get_from_xml(name,value);
		strcpy(jc_enable[i] , value);	

		if((strcmp(enable[i] , "on") == 0)&&(strcmp(jc_enable[i] , "on") == 0))	
		{
			no_all++;
			no_all_sz[j++] = i+1;
		}
	}


	for(i = 0 ; i < num_total ; i++)
	{
		memset(name, 0 ,sizeof(name));
		sprintf(name , "rtsp_url_%d" ,i+1);
	
		memset(value, 0 ,sizeof(value));
		get_from_xml(name,value);
		strcpy(rtsp_url[i] , value);	
		
		//printf("%s = %s\n",name , rtsp_url[i]);
	}
}


void *thread1(void)
{
	printf("enter thread1\n");
	signal(SIGALRM , signal_handler);
	alarm(1);
	while(1)
	{
		pause();
	}
}

void *thread3(void)
{
	printf("enter thread3\n");
	int    listenfd, connfd;
    struct sockaddr_in     servaddr;
    char    buff[300];
    int     n;

    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
	    printf("create socket error\n");
	    exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
	 if( inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0)
	 {
	     printf("inet_pton error\n");
	     exit(0);
	 }
    servaddr.sin_port = htons(6667);

    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
	    printf("bind socket error\n");
	    exit(0);
    }

    if( listen(listenfd, 10) == -1)
	{
	    printf("listen socket error\n");
	    exit(0);
    }

	
    while(1)
	{
	    if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1)
		{
	        printf("accept socket error");
	        continue;
	    }
	    n = recv(connfd, buff, MAXLINE, 0);
	    buff[n] = '\0';
	    //printf("recv msg from client: %s\n", buff);
	    close(connfd);

		if(strcmp(buff , "modify xml") == 0)
		{
			read_xml();
			modify_flag = 1;
			alarm(1);
			sleep(3);
			alarm(roll_time);
		}
    }

    close(listenfd);

}


int thread_create(void)
{
	int temp;
	memset(&thread, 0 , sizeof(thread));

	if((temp = pthread_create(&thread[0] , NULL , (void *)thread1, NULL)) != 0)
	{
		printf("thread1 create error\n");
		return -1;
	}

	if((temp = pthread_create(&thread[2] , NULL , (void *)thread3, NULL)) != 0)
	{
		printf("thread3 create error\n");
		return -1;
	}
}

void thread_wait(void)
{
	if(thread[0] != 0)
	{
		pthread_join(thread[0] , NULL);
	}

	if(thread[1] != 0)
	{
		pthread_join(thread[1] , NULL);
	}

	if(thread[2] != 0)
	{
		pthread_join(thread[2] , NULL);
	}
}


int main()
{
	read_xml();

	if(thread_create() == -1)
	{
		return -1;
	}


	thread_wait();
	return 0;
}



