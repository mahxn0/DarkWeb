/****************************************************************
@filename:data_update.c
@time:08-25
@author:pengkunfan
****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "data_update.h"
#include "./webparam/xml_op.h"

void h_print()
{
	printf("Content-type:text/html%c%c",10,10);
}

void h_ok(int ret,int id)
{
	 if(ret==sucess_ok)
	{
       // printf("<h1 align='center'>your set is success!</h1>\n");
		switch(id)
		{
		case 0:
		break;
		case 4:
		printf("<meta http-equiv='refresh' content='1; url=../date.html'>");
		case 5:
		printf("<meta http-equiv='refresh' content='1; url=../ipaddr.html'>");
		break;
		case 6:
		printf("<meta http-equiv='refresh' content='1; url=../index.html'>");
		break;
		case 7:
		printf("<meta http-equiv='refresh' content='1; url=../video.html'>");
		break;
		case 8:
		printf("<meta http-equiv='refresh' content='1; url=../client.html'>");
		break;
		case 9:
		printf("<meta http-equiv='refresh' content='1; url=../audio.html'>");
		break;
		case 10:
		printf("<meta http-equiv='refresh' content='1; url=../video.html'>");
		break;
		case 50:
		printf("<meta http-equiv='refresh' content='1; url=../osdsetting.html'>");
		break;
		case 51:
		printf("<meta http-equiv='refresh' content='1; url=../arithmetic.html'>");
		break;
		case 52:
		printf("<meta http-equiv='refresh' content='1; url=../display.html'>");
		break;
		case 55:
		printf("<meta http-equiv='refresh' content='1; url=../facedetection.html'>");
		break;
		default:
		break;
		}
	}
	
	else
	{
		 printf("<h1 align='center'>your set is failed!</h1>\n");
	}
}

int main(int argc,char *argv[])
{
	
	int ret=0,id=0;
	int index=0;
	char name[128] = {0};
	char pwd[128] = {0};
	char cvip[1]={0};
	int start_sig=0;
/* get data */
	memset(in_buf,0,sizeof(entry_buf));
	int i,cont_length;
	struct tm today;
	time_t now;

	time(&now);
	today=*localtime(&now);
	mktime(&today);
	
	if (strcmp(getenv("REQUEST_METHOD"),"POST")==0)
	{
		
		char *str_len=NULL;
	
		cont_length=atoi((str_len=getenv("CONTENT_LENGTH")));//CONTENT_LENGTH
		//printf("Content-type:text/html%c%c",10,10);
		for (i=0;i<cont_length&&(!feof(stdin));i++)
		{
			cont_value=i;
			in_buf[i].value=fmake_data(stdin,'&',&cont_length);
			plustospace(in_buf[i].value);
			unescape_url(in_buf[i].value);//NULL
			in_buf[i].name=make_data(in_buf[i].value,'=');
			//printf("name =%s, value = %s\n<br>",in_buf[i].name,in_buf[i].value);
			
		}
		
		
	}
	else
	{
		
		printf("Content-type:text/html%c%cr2",10,10);
		printf("the form was sbumited with an unknown method!\n");
	}
	
		ret=get_index(in_buf);	//add
		if(ret==sucess_ok)
			{
				h_print();
				printf("set error !!!\n");
			}
		//h_print();
		//printf("set bit_rate:%s !!!\n",in_buf[1].value);
		index=in_buf[0].index;
		switch(index)
		{
			case index_repose_date:
			h_print();
			ret=date_process(in_buf,cont_value);
			id=4;
			ret = sucess_ok;
			h_ok(ret,id);
			//date_process(in_buf,cont_value);
			break;
			case index_repose_ipaddr:
			h_print();
			ret=ipaddr_process(in_buf,cont_value);
			id=5;
			h_ok(ret,id);

			break;
			case index_repose_serverip:
			h_print();
			ret=serverip_process(in_buf,cont_value);
			id=55;
			h_ok(ret,id);

			break;
			case index_repose_encode:
				h_print();
			ret=enc_param_process(in_buf,cont_value);
			ret = sucess_ok;
			id = 7;
			
			
			printf("<h1 align='center'>your set is success!</h1>\n");
			if(1 == atoi(in_buf[4].value))
			{
				printf("<h1 align='center'>The system will reboot !!!</h1>\n");
			}
				/*qjq test start*/
			/*qjq test end*/
			h_ok(ret,id);
			
			if(1 == atoi(in_buf[4].value))
			{	
				sleep(3);
				system("/sbin/reboot -f");
			}

		
			break;
			case index_repose_client_type:
			h_print();
			ret=client_param_process(in_buf,cont_value);
			ret = sucess_ok;
			id = 8;
			
			
			printf("<h1 align='center'>your set is success!</h1>\n");
			if(1 == atoi(in_buf[1].value))
			{
				printf("<h1 align='center'>The system will reboot !!!</h1>\n");
			}
			
			h_ok(ret,id);
			
			if(1 == atoi(in_buf[1].value))
			{	
				sleep(3);
				system("/sbin/reboot -f");
			}
			break;
			case index_repose_audio_type:
			h_print();
			ret=audio_param_process(in_buf,cont_value);
			ret = sucess_ok;
			id = 9;
			
			
			printf("<h1 align='center'>your set is success!</h1>\n");
			if(1 == atoi(in_buf[2].value))
			{
				printf("<h1 align='center'>The system will reboot !!!</h1>\n");
			}
			
			h_ok(ret,id);
			
			if(1 == atoi(in_buf[2].value))
			{	
				sleep(3);
				system("/sbin/reboot -f");
			}
			break;
			case 10://login process
			h_print();
			get_from_xml("user.name",name);
			get_from_xml("user.pwd",pwd);
			if((strcmp(in_buf[0].value,name)==0)&& (strcmp(in_buf[1].value,pwd)==0))
			{
				
							
				h_ok(0,52);
			}
			else
			{
				printf("<h1 align='center'>Error user name or password!!!</h1>\n");
				printf("<meta http-equiv='refresh' content='1; url=../index.html'>");
			}
			break;
			case 11://login process
			pwd_process(in_buf,cont_value);
			h_print();
	
			printf("<h1 align='center'>Successfully change password!!!</h1>\n");
			printf("<meta http-equiv='refresh' content='1; url=../change_pwd.html'>");
	
			
			break;
			case index_repose_osd:
				h_print();
			ret=osd_param_process(in_buf,cont_value);
			ret = sucess_ok;
			id = 50;
			
			
			printf("<h1 align='center'>your set is success!</h1>\n");
			
			h_ok(ret,id);
			
				
			break;

			case index_repose_arithmetic:
				h_print();
			ret=arithmetic_param_process(in_buf,cont_value);
			ret = sucess_ok;
			id = 51;
			
			printf("<h1 align='center'>your set is success!</h1>\n");
			
			h_ok(ret,id);
			
				
			break;

			case index_repose_display:
				h_print();
			ret=display_param_process(in_buf,cont_value);
			ret = sucess_ok;
			id = 52;
			printf("<h1 align='center'>your set is success!</h1>\n");
			h_ok(ret,id);
			
				
			break;
			
			default:
			break;
		}
	

/************/

        return 0;
}      


