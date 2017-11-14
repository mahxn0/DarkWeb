#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc,char *argv[])
{
    time_t current;
    struct tm *timeinfo;
	
	int cont_length =0;
	int i=0;
	char c;
	
    time(&current);
    timeinfo = localtime(&current);
    
    //这一句一定要加，否则异步访问会出现页面异常
    printf("Content type: text/html\n\n");
	//printf("argc = %d",argc);
    printf("%s", asctime(timeinfo));
	
	/*
	if (strcmp(getenv("REQUEST_METHOD"),"POST")==0)
	{
		
		char *str_len=NULL;
	
		cont_length=atoi((str_len=getenv("CONTENT_LENGTH")));//CONTENT_LENGTH
		//printf("<br>start:");
		
		for (i=0;i<cont_length&&(!feof(stdin));i++)
		{
			
			printf("%c",fgetc(stdin));
		}
			
	}*/
}