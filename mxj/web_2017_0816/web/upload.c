#include<stdio.h>
#include<stdlib.h>
#include<wait.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include"cgic.h"
#define BufferLen 1024




int cgiMain(void)
{
    cgiFilePtr file;
    int targetFile;
    mode_t mode;
    char name[128];
    char fileNameOnServer[64];
    char file_name[64];
	char cmd[100];
    char contentType[1024];
    char buffer[BufferLen];
    char *tmpStr=NULL;
    int size;
    int got,t;
	int i = 0;
    fprintf(stderr,"could not retrieve filename\n");
    cgiHeaderContentType("text/html");
    //obtain the file path from html
    if (cgiFormFileName("uploadfile", name, sizeof(name)) !=cgiFormSuccess) 
	{
        fprintf(stderr,"could not retrieve filename\n");
        return 0;
    }
    cgiFormFileSize("uploadfile", &size);
    //obtain the type of the file 
    cgiFormFileContentType("uploadfile", contentType, sizeof(contentType));
    //the present file exists in the temporary content 
    if (cgiFormFileOpen("uploadfile", &file) != cgiFormSuccess) 
	{
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
	sprintf(file_name,"/update_dir/%s",fileNameOnServer);
#endif
    mode=S_IRWXU|S_IRGRP|S_IROTH;
    //build new file 
	 targetFile=open(file_name,O_RDWR|O_CREAT|O_TRUNC|O_APPEND,mode);
    if(targetFile<0)
	{
        fprintf(stderr,"could not create the new file,%s\n",file_name);
        return 0;
    }
    //read the content

    printf("systerm updating from ...\n");
    while (cgiFormFileRead(file, buffer, BufferLen, &got) ==cgiFormSuccess)
	{
        if(got>0)
    	{
        	write(targetFile,buffer,got);
			if(5 == i)
			{
				printf(".\n");
				i = 0;
			}
			i++;
    	}
    }
    cgiFormFileClose(file);
    close(targetFile);
	
	sprintf(cmd,"chmod 777 %s",file_name);
	system(cmd);

	printf("Down load file completed !!\n");
	printf("updating ...\n");
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
}



