#include<stdio.h>
#include<stdlib.h>
#include<wait.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include"cgic.h"

#include "./xml_op.h"
#include "data_define.h"


#define BufferLen 1024

//FILE *fb;  //cwf



int cgiMain(void)
{
	//fb = fopen("/home/nvidia/cwf/12.txt","a");   //cwf
	
    cgiFilePtr file;
    int targetFile;
    mode_t mode;
    char name[128];
    char fileNameOnServer[64];
    char file_name[64];
	char cmd[300];
    char contentType[1024];
    char buffer[BufferLen];
    char *tmpStr=NULL;
    int size;
    int got,t;
	int i = 0;
	char value[32] = {0};  //cwf

	 int    sockfd, n;
	 char    recvline[300], sendline[300];
	 struct sockaddr_in    servaddr;

	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", 0);
	save_to_xml("offline.load_flag",value);
	
   // fprintf(stderr,"could not retrieve filename11\n");
    cgiHeaderContentType("text/html");
    //obtain the file path from html
    if (cgiFormFileName("uploadfile", name, sizeof(name)) !=cgiFormSuccess) 
	{
        fprintf(stderr,"could not retrieve filename22\n");
		printf("0");  //cwf
        return 0;
    }
    cgiFormFileSize("uploadfile", &size);

	//fprintf(fb,"ree111114 name = %s size = %d\n",name,size);  //cwf
    //obtain the type of the file 
    cgiFormFileContentType("uploadfile", contentType, sizeof(contentType));
    //the present file exists in the temporary content 
    if (cgiFormFileOpen("uploadfile", &file) != cgiFormSuccess) 
	{
		printf("0");  //cwf
        fprintf(stderr,"could not open the file\n");
        return 0;
    }
    t=-1;
    //find the filename 
    while(1)
	{
        //if "\\" is not path separator, try "/"
        tmpStr=strstr(name+t+1,"\\");
        if(NULL==tmpStr)
            tmpStr=strstr(name+t+1,"/");
        if(NULL!=tmpStr)
            t=(int)(tmpStr-name);
        else
            break;
    }
    strcpy(fileNameOnServer,name+t+1);
#if 0
    //sprintf(file_name,"/opt/dvr_rdk/ti814x/%s",fileNameOnServer);
#else
	//fprintf(fb,"ree111115  fileNameOnServer = %s\n",fileNameOnServer);  //cwf
	//sprintf(file_name,"/update_dir/%s",fileNameOnServer);  //cwf
	sprintf(file_name,"/home/nvidia/var/sptp_upload/%s",fileNameOnServer);
#endif
    mode=S_IRWXU|S_IRGRP|S_IROTH;
    //build new file 
	 targetFile=open(file_name,O_RDWR|O_CREAT|O_TRUNC|O_APPEND,mode);
    if(targetFile<0)
	{
        fprintf(stderr,"could not create the new file,%s\n",file_name);
		printf("0");  //cwf
        return 0;
    }
    //read the content
    //printf("systerm updating from ...\n");
    while (cgiFormFileRead(file, buffer, BufferLen, &got) ==cgiFormSuccess)
	{
        if(got>0)
    	{
        	write(targetFile,buffer,got);
			if(5 == i)
			{
				//printf(".\n");
				i = 0;
			}
			i++;
    	}
    }
    cgiFormFileClose(file);
    close(targetFile);
	memset(cmd , 0 ,sizeof(cmd));
	sprintf(cmd,"chmod 777 %s",file_name);
	system(cmd);

	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", 1);
	save_to_xml("offline.load_flag",value);


	
	if(strncmp(contentType , "image" ,5) == 0)  //cwf
	{
		//printf("the type is image\n");
		printf("1");  //cwf

		
	
		//decoder_param.offline_detsig = 0;	//检测未完成
	    memset(value, 0 ,sizeof(value));
		sprintf(value, "%d", 0);
		save_to_xml("offline.det_sig",value);
		save_to_xml("offline.pic_url","/sptp_upload/dog.jpg");
		save_to_xml("offline.type","image");

	#if 0
		memset(cmd , 0 ,sizeof(cmd));
		sprintf(cmd,"/home/nvidia/darknet_offline/runpic_det.sh %s",file_name);
		//sprintf(cmd,"/home/nvidia/cwf/cwf/web_2017_0816/web/cgi/sptp_upload/ccc.sh %s &",file_name);
		system(cmd);
		printf("<meta http-equiv='refresh' content='0; url=../local.html'>");//add
	#else
		#if 1
			#if 0
				memset(cmd , 0 ,sizeof(cmd));
				sprintf(cmd,"/home/nvidia/darknet_offline/runpic_det.sh %s",file_name);
				//sprintf(cmd,"/home/nvidia/cwf/cwf/web_2017_0816/web/cgi/sptp_upload/ccc.sh %s &",file_name);
				//system(cmd);
				FILE *p = popen(cmd , "r");
				pclose(p);
				printf("<meta http-equiv='refresh' content='0; url=../local.html'>");//add
			#else
				if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
				 printf("create socket error\n");
				 return 0;
				 }

				 memset(&servaddr, 0, sizeof(servaddr));
				 servaddr.sin_family = AF_INET;
				 servaddr.sin_port = htons(6666);

				if( inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0){
				 return 0;
				 }	

				 if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
				 printf("connect error\n");
				 return 0;
				 }

				
				 if( send(sockfd, file_name, strlen(file_name), 0) < 0)
				 {
				 printf("send msg error\n");
				 return 0;
				 }

				 close(sockfd);

				 printf("<meta http-equiv='refresh' content='0; url=../local.html'>");//add
			#endif
		#else
			pid_t pid;
			pid=fork();
			if(pid==0) 
			{ 
				memset(cmd , 0 ,sizeof(cmd));
				sprintf(cmd,"/home/nvidia/darknet_offline/runpic_det.sh %s",file_name);
				//sprintf(cmd,"/home/nvidia/cwf/cwf/web_2017_0816/web/cgi/sptp_upload/ccc.sh %s &",file_name);
				//system(cmd);
		
				execlp("/bin/sh", "sh" , "/home/nvidia/darknet_offline/runpic_det.sh" , file_name,NULL);
				
			} 
			else if(pid>0)
			{	
				printf("<meta http-equiv='refresh' content='0; url=../1.html'>");//add 
				//return 0;
				//waitpid(pid,NULL,0);
			}
		#endif
	#endif
	}
	else if(strncmp(contentType , "video" ,5) == 0)
	{
		//printf("the type is video\n");
		save_to_xml("offline.type","video");
		printf("1");  //cwf
	}
	else
	{
		//printf("please upload the correct type file\n");
		save_to_xml("offline.type","error");
		printf("0");  //cwf
	}

	
#if 0
	#if 1
		memset(cmd, 0,100);
		sprintf(cmd,"tar -zxvf %s -C /opt",file_name);
	#else
		memset(cmd, 0,100);
		sprintf(cmd,"tar -zxvf %s -C /test",file_name);
	
	#endif
	//sleep(10);
	int status = system(cmd);
	//printf("status:%d\n",status);
	//printf("WEXITSTATUS:%d\n",WEXITSTATUS(status));
	//printf("WIFEXITED:%d\n",WIFEXITED(status));

	memset(cmd, 0,100);
	sprintf(cmd,"rm %s",file_name);
	system(cmd);

	system("sync");


	
	if((-1 != status)&&(WIFEXITED(status))&&(0 == WEXITSTATUS(status)))//(0 == status)
    	goto END;
	else
	{
		printf ("error status %d\n",status);		
		return status;
	}
#endif

#if 0 //cwf
	//printf("Down load file completed !!\n");
	//printf("updating ...\n");
	pid_t pid;
	pid=fork();
	if(pid==0) 
	{ 
		
#if 1
					 memset(cmd, 0,100);
					 sprintf(cmd,"tar -zxvf %s -C /update_dir",file_name);
				
				
					 system(cmd);
				  
					 memset(cmd, 0,100);
					 sprintf(cmd,"rm %s",file_name);
					 system(cmd);
					 system("sync");
					 
					 system("chmod 777 /update_dir/* -R");
					 system("/update_dir/upgrade.sh");
				 	 system("rm /update_dir/*");
					 system("sync");
					 system("/sbin/reboot");
					 
 #endif
		//printf("<meta http-equiv='refresh' content='1; url=../firmware.html'>");//
		
	} 
	else if(pid>0)
	{	
 		printf("<meta http-equiv='refresh' content='1; url=../firmware.html'>");//add 
		return 0;
		//waitpid(pid,NULL,0);
	}  
#else
	//printf("<meta http-equiv='refresh' content='5; url=../firmware.html'>");//add 
	//printf("<meta http-equiv='refresh' content='0; url=../local.html'>");//add 
	//printf("<meta http-equiv='refresh' content='1; url=../1.html'>");//add 

#endif
}



