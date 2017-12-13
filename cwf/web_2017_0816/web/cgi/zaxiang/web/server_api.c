/****************************************************************
@filename:server_api.c
@time:08-25
@author:pengkunfan
****************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <asm/types.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <pthread.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "data_define.h"
#include "data_update.h"


#include "server_api.h"


#include <sys/reboot.h>
static char *netname = "eth0";


typedef struct _test_struct
{
	int x;
	int y;
	int width;
	int height;
	int flag;
	int pro;
	int big;
}test_struct;



/***************************************************************************/
/********************DATE  TIME    PAGE************************************/
/**************************************************************************/

/****************************************************************
@function:sys_set_date
@brief:set system date
@param:year month day
@author:pengkunfan
****************************************************************/
int sys_set_date(int year, int month, int day)
{
	struct tm *tm;
	time_t now;

	now = time(NULL);
	tm = localtime(&now);

	year = (year>1900) ? year-1900 : 0;
	tm->tm_year = year;
	month = (month>0) ? month-1 : 0;
	tm->tm_mon = month;
	tm->tm_mday = day;
	//printf("year = %d, month = %d , day = %d \n",year,month,day);
	if ((now = mktime(tm)) < 0)
		return -1;
	
	stime(&now);
	system("hwclock -uw");
	
	return 0;
}

int sys_set_datetime( datetime sysdatetime)
{
	return 0;
}

int sys_set_time(int hour, int min, int sec)
{
	struct tm *tm;
	time_t now;

	now = time(NULL);
	tm = localtime(&now);

	tm->tm_hour = hour;
	tm->tm_min = min;
	tm->tm_sec = sec + 3;

	if ((now = mktime(tm)) < 0)
		return -1;
	
	stime(&now);
	system("/sbin/hwclock -uw");
	
	return 0;
}

void set_system_date(char *req, COMMAND_ARGUMENT *argm)
{
	int year, month, day;
	char *temp_ptr=(char*)malloc(2);
	char yea[5];
	char mon[2];
	char dat[2];
	//default day month year
	
	memcpy(dat,argm->value,2);
	memcpy(temp_ptr,argm->value,2);
	*(temp_ptr+2)='\0';
	dat[2]='\0';//after this dat is null hacker!!!!!!!!!!
	
	memcpy(mon,argm->value+3,2);
	mon[2]='\0';

	memcpy(yea,argm->value+6,4);
	yea[4]='\0';
	
	year=atoi(yea);
	month=atoi(mon);
	day=atoi(temp_ptr);

	do {

		if (sys_set_date(year, month, day) < 0)
			break;
		return;
	} while (0);
	
	return;
}

void set_system_time(char *req, COMMAND_ARGUMENT *argm)
{
	//int hour, min, sec;

}

void set_dateformat(char *req, COMMAND_ARGUMENT *argm)
{
	//__u8 value = atoi(argm->value);
	do {
		
			break;
		return;
	} while (0);
}


void set_tstampformat(char *req, COMMAND_ARGUMENT *argm)
{
	//__u8 value = atoi(argm->value);
	do {
			break;
		return;
	} while (0);
}

/***************************************************************************************/
/*******************************		set ipaddr		*******************************/
/***************************************************************************************/

/****************************************************************
@function:get_ipaddr
@brief:set ipaddr
@param:NULL
@author:pengkunfan
****************************************************************/
void get_ipaddr()
{
	FILE* h_fd;
	h_fd=fopen("../ipaddr.html","rb");
	struct stat stat_buf;
	stat("../ipaddr.html",&stat_buf);
	
	char st_buffer[1024*1024] ={0};
	int temp_p;//
	int p_i=0;
	
	FILE* my_fd;
	
	char *out_buf=malloc(strlen(st_buffer)+1);
	FILE* o_fd;
	int ret;
	char data_char[10];
	char *ip_buf;
	char *netmask_buf;
	char *gateway_buf;
	
	memset(st_buffer,0,sizeof(st_buffer));
	
	while((temp_p=fgetc(h_fd))!=EOF)
	{
		st_buffer[p_i++]=temp_p;
	}
	
	st_buffer[p_i]='\0';
	fclose(h_fd);
	
	o_fd=fopen("sys.cfg","rb");
	ret=fread(&decoder_param,1,sizeof(system_param),o_fd);
	fclose(o_fd);
	

	strcpy(data_char,"123456789");
	ip_buf = inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.ip_add);
	sprintf(data_char,"%s",ip_buf);

	reset_value((unsigned char *)st_buffer,(unsigned char *)out_buf,(unsigned char *)data_char,12);
	
	strcpy(st_buffer,out_buf);
	netmask_buf = inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.net_mask);
	sprintf(data_char,"%s",netmask_buf);
	reset_value((unsigned char *)st_buffer,(unsigned char *)out_buf,(unsigned char *)data_char,13);
	
	strcpy(st_buffer,out_buf);
	gateway_buf = inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.gateway);
	sprintf(data_char,"%s",gateway_buf);//decoder_param.camera_net.gateway);
	reset_value((unsigned char *)st_buffer,(unsigned char *)out_buf,(unsigned char *)data_char,14);
	strcpy(st_buffer,out_buf);
	


	my_fd=fopen("../ipaddr.html","wb");
	
	if(my_fd==NULL)
	{
		printf("bug\n");
		exit(-1);
	}
	fprintf(my_fd,"%s",out_buf);
	fclose(my_fd);
	//system("/bin/sync");
	system("/bin/sync");
	system("/sbin/reboot");

}





void get_serveripaddr()
{
	
	



}




void get_netip(char *req, COMMAND_ARGUMENT *argm)
{
	struct in_addr ip;

	do {
		if ( (ip.s_addr = net_get_ifaddr(netname)) == -1)
			break;
		
		return;
	} while (0);
	
}

void set_netip(char *req, COMMAND_ARGUMENT *argm)
{
	struct in_addr ipaddr, sys_ip;

	do {
		if (ipv4_str_to_num(argm->value, &ipaddr) == 0)
			break;

		if ( (sys_ip.s_addr = net_get_ifaddr(netname)) == -1)
			break;

		if (sys_ip.s_addr != ipaddr.s_addr) {
			if (net_set_ifaddr(netname, ipaddr.s_addr) < 0)
				break;

		}
		
		return;
	} while (0);

}

void get_netmask(char *req, COMMAND_ARGUMENT *argm)
{
	struct in_addr ip;

	do {

		if ( (ip.s_addr = net_get_netmask(netname)) == -1)
			break;
	
		return;
	} while (0);
	
}

void set_netmask(char *req, COMMAND_ARGUMENT *argm)
{
	struct in_addr ipaddr, sys_ip;

	do {
		if (ipv4_str_to_num(argm->value, &ipaddr) == 0)
			break;


		if ( (sys_ip.s_addr = net_get_netmask(netname)) == -1)
			break;

		if (sys_ip.s_addr != ipaddr.s_addr) {
			if (net_set_netmask(netname, ipaddr.s_addr) < 0)
				break;

		}
		
		return;
	} while (0);
	

}

void get_gateway(char *req, COMMAND_ARGUMENT *argm)
{
	struct in_addr ip;
	ip.s_addr = net_get_gateway();
	
}

void set_gateway(char *req, COMMAND_ARGUMENT *argm)
{
	struct in_addr ipaddr, sys_ip;

	do {
		if (ipv4_str_to_num(argm->value, &ipaddr) == 0)
			break;

		sys_ip.s_addr = net_get_gateway();

		if (sys_ip.s_addr != ipaddr.s_addr) {
			if (net_set_gateway(ipaddr.s_addr) < 0)
				break;
		}
		
		return;
	} while (0);

}

/***************************************************************************************/
/*******************************		set rtspaddr		*******************************/
/***************************************************************************************/

/****************************************************************
@function:get_rtspaddr
@brief:get rtsp addrss
@param:NULL
@author:pengkunfan
****************************************************************/
void get_rtspaddr()
{

	FILE* h_fd;
	struct stat stat_buf;
	
	stat("../index.html",&stat_buf);
	h_fd=fopen("../index.html","rb");

	char st_buffer[1024*1024];
	memset(st_buffer,0,sizeof(st_buffer));
	
	//printf("index orign size =%d \n",stat_buf.st_size);
	
	int temp_p;//
	int p_i=0;
	FILE* my_fd;
	
	while((temp_p=fgetc(h_fd))!=EOF)
	{
		st_buffer[p_i++]=temp_p;
	}
	//printf("index read size = %d \n",p_i);
	st_buffer[p_i]='\0';
	fclose(h_fd);
	h_fd = 	NULL;
	
	char *out_buf=malloc(strlen(st_buffer)+1);
	FILE* o_fd;
	int ret;
	memset(out_buf,0,sizeof(strlen(st_buffer)));
	memset(&decoder_param,0,sizeof(decoder_param));
	
	o_fd=fopen("sys.cfg","rb");
	ret=fread(&decoder_param,1,sizeof(system_param),o_fd);
	fclose(o_fd);
	
	char data_char[1024];//{'1','2','3'};
	strcpy(data_char,"123456789");
	reset_check_selected((unsigned char *)st_buffer,(unsigned char *)out_buf,1);// 1:clean  all select

	strcpy(st_buffer,out_buf);
	//
	//printf("strlen size 1 =%d \n",strlen(st_buffer));//-18=4648-4630 4649 =+19
	if(decoder_param.p_rtspaddr.channel==1)//1~4
	{
		add_check_selected((unsigned char *)st_buffer,(unsigned char *)out_buf,48);
	}
	else if(decoder_param.p_rtspaddr.channel==2)
	{
		add_check_selected((unsigned char *)st_buffer,(unsigned char *)out_buf,49);
	}
	else if(decoder_param.p_rtspaddr.channel==3)
	{
		add_check_selected((unsigned char *)st_buffer,(unsigned char *)out_buf,50);
	}
	else if(decoder_param.p_rtspaddr.channel==4)
	{
		add_check_selected((unsigned char *)st_buffer,(unsigned char *)out_buf,51);
	}

	//
	strcpy(st_buffer,out_buf);
	/*
	printf("strlen size 2 =%d \n",strlen(st_buffer));
	FILE* Test = NULL;
	Test = fopen("../inde1.html","wb");
	fwrite(st_buffer,1,strlen(st_buffer),Test);
	fclose(Test);
	my_fd = NULL;
	*/
	//printf(", rtsp = %s  \n",decoder_param.p_rtspaddr.chrtsp[decoder_param.p_rtspaddr.channel].rtsp);
	sprintf(data_char,"%s",decoder_param.p_rtspaddr.chrtsp[decoder_param.p_rtspaddr.channel].rtsp);
	//printf("channel = %d , rtsp = %s  \n",decoder_param.p_rtspaddr.channel,data_char);
	
	//reset_value((unsigned char *)st_buffer,(unsigned char *)out_buf,(unsigned char *)data_char,18);//have add long bug
	//printf("why........\n");
	//printf("strlen size 3 =%d <br>\n",strlen(out_buf));
	//printf("end:<>%c-%c <>%c-%c\n",*(out_buf+strlen(out_buf)-13),*(out_buf+strlen(out_buf)-10),
	//*(out_buf+strlen(out_buf)-5),*(out_buf+strlen(out_buf)-2));

	//*(out_buf+strlen(out_buf)) = '\0';
	//printf("data_char = %s \n",out_buf);
	//system("/bin/chmod 777 ../*");

	my_fd = NULL;
	//system("sync");
	//system("/bin/chmod 777 ../*");
	//exit(-1);
	/*
	my_fd=fopen("../index.html","w");

	if(my_fd==NULL)
	{
		printf("bug\n");
		exit(-1);
	}
	else printf("successfull!\n");
	//printf("out_buf = %s \n",out_buf);
	//exit(-1);
	fflush(stdout);
	fwrite(out_buf,1,strlen(out_buf)-1,my_fd);
	fflush(my_fd);
	//fprintf(my_fd,"%s",out_buf);
	//fflush(my_fd);
	//system("sync");
	fclose(my_fd);*/
	/*
	Test = NULL;
	Test = fopen("../inde3.html","wb");
	fwrite(out_buf,1,strlen(out_buf),Test);
	fclose(Test);
	fflush(stdout);
	exit(1);
	
	*/
	
	if(decoder_param.p_rtspaddr.flag == 1)
	{
		system("/sbin/reboot");
		//system("/sbin/reboot");
		
	}
}


url_option url_option_table_time [] =
{
	{ "system_date",NULL,set_system_date,0 },
	{ "system_time",NULL,set_system_time,0 },
	{ "dateformat",NULL,set_dateformat,0 },
	{ "tstampformat",NULL,set_tstampformat,0 }
};

url_option url_option_table_ipaddr [] =
{
	{ "netip",NULL,set_netip,0 },
	{ "netmask",NULL,set_netmask,0 },
	{ "gateway",NULL,set_gateway,0 }
};

