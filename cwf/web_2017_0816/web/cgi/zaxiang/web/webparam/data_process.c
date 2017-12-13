/****************************************************************
@filename:data_process.c
@time:08-25
@author:pengkunfan
****************************************************************/
#ifndef davince_ipncam
#define davince_ipncam
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "data_define.h"
//#include "data_update.h"
//#include "server_api.h"
#include <sys/stat.h>
#include <unistd.h>


#define R_DB(fmt, args...)	fprintf(stderr, fmt, ##args)
extern url_option url_option_table_time [];
extern url_option url_option_table_ipaddr [];
#endif

//static void write_param(system_param my_param);
static void read_param();

static void read_param()
{
	FILE* p_fd;
	int ret;
	p_fd=fopen("sys.cfg","rb");
	
	if(p_fd==NULL)
	{
		
		p_fd=fopen("sys.cfg","wb");
		memset(&decoder_param,0,sizeof(decoder_param));
		decoder_param.camera_time.year=2014;
		decoder_param.camera_time.month=8;
		decoder_param.camera_time.day=1;
		decoder_param.camera_time.hour=12;
		decoder_param.camera_time.min=0;
		decoder_param.camera_time.sec=0;
	
	
		
		strcpy(decoder_param.camera_net.ip_add , ("192.168.0.168"));
		strcpy(decoder_param.camera_net.net_mask , ("255.255.255.0"));
		strcpy(decoder_param.camera_net.gateway , ("192.168.0.254"));
		strcpy(decoder_param.camera_net.nameserver , ("192.168.0.1"));
		
		
		strcpy((char *)decoder_param.p_rtspaddr.chrtsp[0].rtsp,"rtsp://192.168.0.102/1.264");
		strcpy((char *)decoder_param.p_rtspaddr.chrtsp[1].rtsp,"rtsp://192.168.0.102/1.264");
		strcpy((char *)decoder_param.p_rtspaddr.chrtsp[2].rtsp,"rtsp://192.168.0.102/1.264");
		strcpy((char *)decoder_param.p_rtspaddr.chrtsp[3].rtsp,"rtsp://192.168.0.102/1.264");
		strcpy((char *)decoder_param.p_rtspaddr.chrtsp[4].rtsp,"rtsp://192.168.0.102/1.264");
		
		decoder_param.p_rtspaddr.channel_numbs = 1;
		decoder_param.p_rtspaddr.change_time = 0;
		decoder_param.p_rtspaddr.flag = 0;
	
		ret=fwrite((void *)&decoder_param,1,sizeof(decoder_param),p_fd);
		
		
	}
	else 
	{
		fread(&decoder_param,1,sizeof(system_param),p_fd);
	}
	
		
}


void get_data(char *word,char *line,char stop)//cut word
{
	int x=0,y=0;
	for (x=0;((line[x])&&(line[x]!=stop));x++)
	word[x]=line[x];
	word[x]='\0';
	if (line[x])++x;
	while((line[y++]=line[x++]));
}

char *make_data(char *line,char stop)//cut word '='
{
	int x=0,y;
	char *word=(char *)malloc(sizeof(char)*(strlen(line)));
	for(x=0;((line[x])&&(line[x]!=stop));x++)
	{
		
		word[x]=line[x];
	}
	word[x]='\0';
	if (line[x])++x;
	y=0;
	while((line[y++]=line[x++]));
	return word;//=name
}

char *fmake_data(FILE *f,char stop,int *len)//get name =value
{
	int wsize;
	char *word;
	int ll;
	ll=0;
	wsize=102400;//100kb;
	word=(char *)malloc(sizeof(char)*(wsize+1));
	while(1)
	{
		word[ll]=(char)fgetc(f);//!EOF
		if(ll==wsize)
		{
			word[ll+1]='\0';
			wsize+=102400;
			word=(char*)realloc(word,sizeof(char)*(wsize+1));
		}
		 
		if (word[ll]==stop||feof(f))//feof=0
		{
			if(word[ll]!=stop)ll++;
			word[ll]='\0';
			return word;
		}
		++ll;
	}
}	
char u2c(char *what)//url encode
{
	register char digit;
	digit=(what[0]>='A'?((what[0]&0xdf)-'A')+10:(what[0]-'0'));
	digit*=16;//<-4
	digit+=(what[0]>='A'?((what[1]&0xdf)-'A')+10:(what[1]-'0'));//(&)-32,-48
	return(digit);
}
void unescape_url(char *url)//url encode//tempery not have 
{
	
	//printf("my1:%s\n",url);
	/*
	for (x=0,y=0;strlen(p_url);++x,++y)//url[y]
	{
		if (p_url[x]=p_url[y]=='%')
		{
			printf("oooooooooox=%d\n",x);
			p_url[x]=x2c(&p_url[y+1]);
			y+=2;
		}
		printf("%c-%d",p_url[y],y);//and hack!!!
	}*/
	//printf("my2:%s\n",url);
	//url[x]='\0';
	
}
void plustospace(char *str)//+ to ''
{
	register int x;
	for (x=0;str[x];x++)
	if(str[x]=='+')str[x]=' ';
}
char *get_value(char *s,int cont_value,entry_buf in_buf[])//get form's  one value
{
	int x,length,i=0;
	char *buf = NULL;
	for (x=0;x<=cont_value;x++)//num_value static
	{
		if (strcmp(in_buf[x].name,s)==0)//entries static
		{	
			if ((length=strlen(in_buf[x].value))==0)return NULL;
			if ((buf=(char *)malloc(length))==NULL)
			{
				printf("not enough memory to allocate buffer");
				exit(0);
			}
			memset(buf,0,sizeof(buf));
			while((in_buf[x].value[i])!='\0')
			{
				buf[i]=in_buf[x].value[i];
				i++;
			}
			
			buf[i]='\0';
			
		}
	}
	return buf;
}
int  get_index(entry_buf in_buf[])
{
	int ret=0;
	
	
	if(strcmp(in_buf[0].name,"date_h")==0)
	{
		in_buf[0].index=4;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"camera_ipaddr")==0)
	{
		in_buf[0].index=5;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"Batch_level")==0)
	{
		in_buf[0].index=6;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"camera_osdsetting_str")==0)
	{
		in_buf[0].index=50;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"arithmeticpara1")==0)
	{
		in_buf[0].index=51;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"rtspurl")==0)
	{
		in_buf[0].index=52;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"server_ipaddr_name")==0)
	{
		in_buf[0].index=55;
		ret=1;
	}
	else {in_buf[0].index=0;ret=0;printf("%s",in_buf[0].name);}
	return ret;
}

int netsplit( char *pAddress, void *ip )
{
	unsigned int ret;
	NET_IPV4 *ipaddr = (NET_IPV4 *)ip;

	if ((ret = atoi(pAddress + 9)) > 255)
		return FALSE;
	ipaddr->str[3] = ret;

	*( pAddress + 9 ) = '\x0';
	if ((ret = atoi(pAddress + 6)) > 255)
		return FALSE;
	ipaddr->str[2] = ret;

	*( pAddress + 6 ) = '\x0';
	if ((ret = atoi(pAddress + 3)) > 255)
		return FALSE;
	ipaddr->str[1] = ret;

	*( pAddress + 3 ) = '\x0';
	if ((ret = atoi(pAddress + 0)) > 255)
		return FALSE;
	ipaddr->str[0] = ret;

	return TRUE;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int ipv4_str_to_num(char *data, struct in_addr *ipaddr)
{
	if ( strchr(data, '.') == NULL )
		return netsplit(data, ipaddr);
	return inet_aton(data, ipaddr);
}
