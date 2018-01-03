#include <stdio.h>
#include <signal.h>
#include "net_config.h"
#include "data_define.h"

#define IFADDRNAME "eth0"
#define SYS_CFG_FILE "/home/nvidia/var/www/cgi-bin/sys.cfg"

static void read_param();

static void read_param()
{
	FILE* p_fd;
	int ret;
	p_fd=fopen(SYS_CFG_FILE,"rb");
	
	if(p_fd==NULL)
	{
		
		p_fd=fopen(SYS_CFG_FILE,"wb");
		memset(&decoder_param,0,sizeof(decoder_param));
		decoder_param.camera_time.year=2014;
		decoder_param.camera_time.month=8;
		decoder_param.camera_time.day=1;
		decoder_param.camera_time.hour=12;
		decoder_param.camera_time.min=0;
		decoder_param.camera_time.sec=0;
#if 0
		decoder_param.camera_net.ip_add = inet_addr("192.168.0.168");
		decoder_param.camera_net.net_mask = inet_addr("255.255.255.0");
		decoder_param.camera_net.gateway = inet_addr("192.168.0.254");
		decoder_param.camera_net.nameserver = inet_addr("192.168.0.1");
#else
		strcpy(decoder_param.camera_net.ip_add , ("192.168.0.168"));
		strcpy(decoder_param.camera_net.net_mask , ("255.255.255.0"));
		strcpy(decoder_param.camera_net.gateway , ("192.168.0.1"));
		strcpy(decoder_param.camera_net.nameserver , ("192.168.0.1"));
#endif
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


void net_work_init()
{
	FILE *fp;
	int ret = -1;
	fp = fopen(SYS_CFG_FILE,"rb");
	char *nettemp;
	char *ip_buf;
	char *netmask_buf;
	char *gateway_buf;
	int i,j;
	
	if(fp == NULL)
	{
		printf("the sys cfg file open failed!\n");
		return -1;
	}
	fread(&decoder_param,1,sizeof(system_param),fp);
	fclose(fp);
	
	ip_buf = inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.ip_add);
	netmask_buf = inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.net_mask);
	gateway_buf = inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.gateway);
	printf("decoder_param.ip_add= %s,decoder_param.netmask = %s , decoder_param.gateway = %s\n",ip_buf,netmask_buf,gateway_buf);
	for(i=0;i<decoder_param.p_rtspaddr.channel_numbs;i++)
	{
		printf("rtsp%d:%s\n",i,decoder_param.p_rtspaddr.chrtsp[i].rtsp);
	}
	printf("decoder_param.p_rtspaddr.mult_flag= %d,decoder_param.p_rtspaddr.channel_numbs = %d ,decoder_param.p_rtspaddr.change_time = %d \n",\
	decoder_param.p_rtspaddr.mult_flag,decoder_param.p_rtspaddr.channel_numbs,decoder_param.p_rtspaddr.change_time);
	
	/*
	if((ret = net_set_ifaddr(IFADDRNAME, decoder_param.camera_net.ip_add))!=0)
	{
		printf("ipaddr set is failed!\n");
		return -1;
	}
	if((ret = net_set_netmask(IFADDRNAME, decoder_param.camera_net.net_mask))!=0)
	{
		printf("netmask set is failed!\n");
		return -1;
	}
	if((ret = net_add_gateway(decoder_param.camera_net.gateway))!=0)
	{
		printf("gateway set is failed!\n");
		return -1;
	}*/
}

/****************************************************************
@function:rtspaddr_process
@brief:process rtspaddr
@time:08-25
@author:pengkunfan
****************************************************************/
int rtspaddr_process(entry_buf in_buf[],int cont_value)
{
	COMMAND_ARGUMENT *argm=(COMMAND_ARGUMENT*)malloc(sizeof(COMMAND_ARGUMENT));

	//argm->value=strdup(in_buf[4].value);
	
	unsigned char file_buffer[1024*512];
	unsigned char char_buf[1024*512];

	FILE* p_fd;
	//FILE* p_fp;//6
	int ret=0;
	int temp = 0;
	int i=0;
	int temp_value1 = 0;
	int temp_value2 = 0;
	int temp_value3 = 0;
	
	memset(file_buffer,0,sizeof(file_buffer));
	memset(char_buf,0,sizeof(char_buf));
	
	read_param();
	//decoder_param.p_rtspaddr.change_time  = atoi(in_buf[1].value);
	//printf("%d\n",decoder_param.p_rtspaddr.change_time);
	//decoder_param.p_rtspaddr.change_time = 1;
	
	if(decoder_param.p_rtspaddr.change_time ==0)
	{
		
		if(strcmp(in_buf[1].value,"one")==0)
		temp_value1 = 1;//decoder_param.p_rtspaddr.batch = 1;
		else if(strcmp(in_buf[1].value,"two")==0)
		temp_value1 = 2;//decoder_param.p_rtspaddr.batch = 2;
		else if(strcmp(in_buf[1].value,"thid")==0)
		temp_value1 = 3;//decoder_param.p_rtspaddr.batch = 3;
		/*
		else if(strcmp(in_buf[1].value,"four")==0)
		temp_value1 = 4;//decoder_param.p_rtspaddr.batch = 4;
		else if(strcmp(in_buf[1].value,"five")==0)
		temp_value1 = 5;//decoder_param.p_rtspaddr.batch = 5;
		else if(strcmp(in_buf[1].value,"six")==0)
		temp_value1 = 6;//decoder_param.p_rtspaddr.batch = 6;
		else if(strcmp(in_buf[1].value,"seven")==0)
		temp_value1 = 7;//decoder_param.p_rtspaddr.batch = 7;
		else if(strcmp(in_buf[1].value,"eight")==0)
		temp_value1 =8;//decoder_param.p_rtspaddr.batch = 8;
		//temp_value1 = decoder_param.p_rtspaddr.batch;
	*/
		if(temp_value1>0||temp_value1<4)
		{
			
			
		}
		if(strcmp(in_buf[2].value,"one")==0)
		temp_value2 = 1;//decoder_param.p_rtspaddr.channel = 1;
		else if(strcmp(in_buf[2].value,"two")==0)
		temp_value2 = 2;//decoder_param.p_rtspaddr.channel = 2;
		else if(strcmp(in_buf[2].value,"thid")==0)
		temp_value2 = 3;//decoder_param.p_rtspaddr.channel = 3;
		else if(strcmp(in_buf[2].value,"four")==0)
		temp_value2 = 4;//decoder_param.p_rtspaddr.channel = 4;
			
		if(temp_value2>=1 &&temp_value2 <=4)
		{
			//get_ulr_addr((char *)file_buffer,argm->value,strlen(argm->value));
			//int tmp_value = (temp_value1-1)*temp_value2+temp_value2-1;
			//printf("%d\n",tmp_value);
			//strcpy((char *)decoder_param.p_rtspaddr.chrtsp[(temp_value1-1)*temp+temp_value2-1].rtsp,(char *)file_buffer);
			//decoder_param.p_rtspaddr.mult_flag = temp;
			for(i=0; i<temp_value2; i++)
			printf("%s\n",decoder_param.p_rtspaddr.chrtsp[i].rtsp);
			for(i=0; i<temp_value2; i++)
			printf("%d;",decoder_param.p_rtspaddr.chrtsp[i].flag);
		}
		//decoder_param.p_rtspaddr.channel_numbs = (temp_value1-1)*temp+temp_value2;
	
	}

	return ret;
	
}
#if 0
int ipaddr_process(entry_buf in_buf,int cont_value)
{
	char *ip_buf;
	read_param();
	ip_buf = inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.ip_add);
	printf("%s",ip_buf);
	return 0;
}
#endif
void signal_handle()
{
	printf("capture ctr + c exit!\n");
	sleep(1);
	exit(-1);
}

void get_handle_data()
{
	int cont_length =0;
	int i=0;
	int k=0;
	printf("Content type: text/html\n\n");
	
	
	if (strcmp(getenv("REQUEST_METHOD"),"POST")==0)
	{
		
		char *str_len=NULL;
	
		cont_length=atoi((str_len=getenv("CONTENT_LENGTH")));//CONTENT_LENGTH
		
		
		for (i=0;i<cont_length&&(!feof(stdin));i++)
		{
			cont_value=i;
			in_buf[i].value=fmake_data(stdin,'&',&cont_length);
			plustospace(in_buf[i].value);
			unescape_url(in_buf[i].value);//NULL
			in_buf[i].name=make_data(in_buf[i].value,'=');

		}
		//printf("why %d\n",atoi(in_buf[0].value));
		k = atoi(in_buf[0].value);

		if(k==1)
		{	
			rtspaddr_process(in_buf,cont_value);
		}
		else if(k==2)
		{
			ip_addr();
		}
		else if(k==3)
		{
			enc_param();
		}

		else if(k==4)
		{
			//ipaddr_process(in_buf,cont_value);
			client_param();
		}
		else if(k==5)
		{
			//ipaddr_process(in_buf,cont_value);
			get_v_param();
		}

		else if(k==6)
		{
			get_audio_param();
		}
		else if(k == 50)
		{
			get_osd_param();
		}
		else if(k== 51)
		{
			get_arithmetic_param();
		}
		else if(k== 52)
		{
			get_display_param();
		}
		else if (k==55)
		{
			server_ipaddr();
		}
	}
	
	
	//rtspaddr_process(in_buf,cont_value);
}

void replace_param()
{
	FILE *fp;
	int ret = -1;
	fp = fopen(SYS_CFG_FILE,"rb");
	char *nettemp;
	char *ip_buf;
	char *netmask_buf;
	char *gateway_buf;
	int i,j;
	int cont_length = 0;
	int cont_value =0;
	
	if(fp == NULL)
	{
		printf("the sys cfg file open failed!\n");
		return -1;
	}
	fread(&decoder_param,1,sizeof(system_param),fp);
	fclose(fp);
	//
	 printf("Content type: text/html\n\n");
	//printf("argc = %d",argc);
    //printf("%s", asctime(timeinfo));
	
	//
	//printf("decoder_param.ip_add= %s,decoder_param.netmask = %s , decoder_param.gateway = %s\n",ip_buf,netmask_buf,gateway_buf);
	for(i=0;i<decoder_param.p_rtspaddr.channel_numbs;i++)
	{
		//printf("rtsp%d:%s\n",i,decoder_param.p_rtspaddr.chrtsp[i].rtsp);
	}
}

void ip_addr()
{
	char *ip_buf;
	char *net_mask;
	char *gateway;
	//read_param();
	
	//ip_buf = inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.ip_add);
	//net_mask = inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.net_mask);
	//gateway = inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.gateway);
#if 0
	printf("%s\n",inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.ip_add));
	printf("%s\n",inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.net_mask));
	printf("%s\n",inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.gateway));
#else

#if 1
		get_from_xml("network.ip",decoder_param.camera_net.ip_add);
		get_from_xml("network.netmask",decoder_param.camera_net.net_mask);
		get_from_xml("network.gateway",decoder_param.camera_net.gateway);
	
#endif

	printf("%s\n",(decoder_param.camera_net.ip_add));
	printf("%s\n",(decoder_param.camera_net.net_mask));
	printf("%s\n",(decoder_param.camera_net.gateway));
#endif
}

void server_ipaddr()
{

#if 1
		get_from_xml("server.ip",decoder_param.serverip);
		get_from_xml("server.netmask",decoder_param.serverport);
	

	printf("%s\n",(decoder_param.serverip));
	printf("%s\n",(decoder_param.serverport));

#endif
}



void enc_param()
{
	char *ip_buf;
	char *net_mask;
	char *gateway;
	//read_param();
	
	//ip_buf = inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.ip_add);
	//net_mask = inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.net_mask);
	//gateway = inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.gateway);
	char value[32] = {0};

	memset(value, 0 ,sizeof(value));
	get_from_xml("enc.bitrate",value);
	decoder_param.en_param.bit_rate = atoi(value);

	memset(value, 0 ,sizeof(value));
	get_from_xml("enc.resolution",value);
	decoder_param.en_param.resolution = atoi(value);

	memset(value, 0 ,sizeof(value));
	get_from_xml("enc.fps",value);
	decoder_param.en_param.fps = atoi(value);
	
	memset(value, 0 ,sizeof(value));
	get_from_xml("enc.bitctrl",value);
	decoder_param.en_param.bit_ctrl = atoi(value);

	
	printf("%d\n", decoder_param.en_param.bit_rate);
	if(0 == decoder_param.en_param.resolution)
	{
		printf("1080p\n");
	}
	else if(1 == decoder_param.en_param.resolution)
	{
		printf("720p\n");
	}
	else if(2 == decoder_param.en_param.resolution)
	{
		printf("1080i\n");
	}
	else if(3 == decoder_param.en_param.resolution)
	{
		printf("480p\n");
	}

	if(0 == decoder_param.en_param.fps)
	{
		printf("60fps\n");
	}
	else if(1 == decoder_param.en_param.fps)
	{
		printf("30fps\n");
	}
	else if(2 == decoder_param.en_param.fps)
	{
		printf("25fps\n");
	}

	if(0 == decoder_param.en_param.bit_ctrl)
	{
		printf("CBR\n");
	}
	else if(1 == decoder_param.en_param.bit_ctrl)
	{
		printf("VBR\n");
	}
	
}

void client_param()
{
	char *ip_buf;
	char *net_mask;
	char *gateway;
	//read_param();

	char value[32] = {0};

	memset(value, 0 ,sizeof(value));
	get_from_xml("client.talk_back_type", value);
	decoder_param.client_type = atoi(value);

	if(0 == decoder_param.client_type)
	{
		printf("pc\n");
	}
	else if(1 == decoder_param.client_type)
	{
		printf("decode\n");
	}
	else if(2 == decoder_param.client_type)
	{
		printf("android\n");
	}
	
}

void get_audio_param()
{

	//read_param();

	char value[32] = {0};

	memset(value, 0 ,sizeof(value));
	get_from_xml("audio.pb_volume",value);
	decoder_param.pb_volume = atoi(value);

	memset(value, 0 ,sizeof(value));
	get_from_xml("audio.r_volume",value);
	decoder_param.r_volume = atoi(value);


	
	printf("%d\n", decoder_param.pb_volume);
	printf("%d\n", decoder_param.r_volume);
}


void get_display_param()
{

	//read_param();
	char value[32] = {0};


	memset(value, 0 ,sizeof(value));
	get_from_xml("rtsp.opencv",value);
	decoder_param.start_opencv= atoi(value);



	get_from_xml("rtsp.rtsp_url",decoder_param.rtsp_url);

	printf("%d\n", decoder_param.rtsp_url);
	printf("%d\n", decoder_param.start_opencv);	
}

void get_arithmetic_param()
{

	//read_param();

	char value[32] = {0};

	memset(value, 0 ,sizeof(value));
	get_from_xml("arithmetic.arithmeticpara1",value);
	decoder_param.arithmeticpara1 = atoi(value);

	memset(value, 0 ,sizeof(value));
	get_from_xml("arithmetic.arithmeticpara2",value);
	decoder_param.arithmeticpara2 = atoi(value);

	memset(value, 0 ,sizeof(value));
	get_from_xml("arithmetic.arithmeticpara3",value);
	decoder_param.arithmeticpara3 = atoi(value);

	memset(value, 0 ,sizeof(value));
	get_from_xml("arithmetic.arithmetic_recvideo",value);
	decoder_param.arithmetic_recvideo = atoi(value);
	
	
	printf("%d\n", decoder_param.arithmeticpara1);
	printf("%d\n", decoder_param.arithmeticpara2);
	printf("%d\n", decoder_param.arithmeticpara3);
	printf("%d\n", decoder_param.arithmetic_recvideo);
}


void get_osd_param()
{

	//read_param();
#if 1
	char value[100] = {0};

	

	memset(value, 0 ,sizeof(value));
	get_from_xml("osd.str",value);
	//decoder_param.osd_str = atoi(value);
	strcpy(decoder_param.osd_str,value);

	memset(value, 0 ,sizeof(value));
	get_from_xml("osd.size",value);
	decoder_param.osd_size = atoi(value);

	memset(value, 0 ,sizeof(value));
	get_from_xml("osd.color",value);
	decoder_param.osd_color = atoi(value);

	memset(value, 0 ,sizeof(value));
	get_from_xml("osd.x",value);
	decoder_param.osd_x = atoi(value);

	memset(value, 0 ,sizeof(value));
	get_from_xml("osd.y",value);
	decoder_param.osd_y = atoi(value);

	memset(value, 0 ,sizeof(value));
	get_from_xml("osd.device_addr",value);
	//decoder_param.device_addr= atoi(value);
	strcpy(decoder_param.device_addr,value);

	memset(value, 0 ,sizeof(value));
	get_from_xml("osd.time",value);
	decoder_param.osd_time= atoi(value);
	


	

	printf("%s\n", decoder_param.osd_str);
	printf("%d\n", decoder_param.osd_size);
	//printf("%d\n", decoder_param.osd_color);
	printf("%d\n", decoder_param.osd_x);
	printf("%d\n", decoder_param.osd_y);

	printf("%s\n", decoder_param.device_addr);
	printf("%d\n", decoder_param.osd_time);	
	

	
	#endif
}


	
void get_v_param()
{
	char buf[100] = {0};
	unsigned char mac[8] = {0};
	int i = 0;
	
	get_kernel_version(buf);
	printf("%s\n", buf);

	memset(buf, 0, 100);
	get_fs_version(buf);
	printf("%s\n", buf);

	memset(buf, 0, 100);
	get_hw_version(buf);
	printf("%s\n", buf);
	
#if 1
		
		memset(mac, 0, 8);
		net_get_hwaddr("eth0", mac);
		for(i = 0; i< 6;i++)
		{
			if(5 == i)
			{
				printf("%02X",mac[i]);
			}
			else
			{
				printf("%02X:",mac[i]);
			}
		}
		printf("\n");
#endif

    memset(buf, 0, 100);
	get_devId_version(buf);
	printf("%s\n", buf);

}



int tolower(int c) 
{ 
    if (c >= 'A' && c <= 'Z') 
    { 
        return c + 'a' - 'A'; 
    } 
    else 
    { 
        return c; 
    } 
} 

//将十六进制的字符串转换成整数 
int htoi(char s[]) 
{ 
    int i; 
    int n = 0; 
    if(s[0] == '0' && (s[1]=='x' || s[1]=='X')) //判断是否有前导0x或者0X
    { 
        i = 2; 
    } 
    else 
    { 
        i = 0; 
    } 
    for(; (s[i] >= '0' && s[i] <= '9') 
|| (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <= 'Z');++i) 
    {   
        if(tolower(s[i]) > '9') 
        { 
            n = 16 * n + (10 + tolower(s[i]) - 'a'); 
        } 
        else 
        { 
            n = 16 * n + (tolower(s[i]) - '0'); 
        } 
    } 
    return n; 
} 

int DeBaiFenStrin2Int(unsigned char *des,unsigned char *src,int size)
{
	
	unsigned char *pSrc = src;
	unsigned char *pDes = des;
	unsigned char outPut[100];
	unsigned char inPut[100];
	memset(outPut,0,sizeof(outPut));
	memset(inPut,0,sizeof(inPut));
	
	int bufSize = size;
	int i;
	while(bufSize>0)
	{
		if(*pSrc=='%')
		{
			pSrc++;
		}
		else
		{
			*pDes++=*pSrc++;
		}
		bufSize--;
	}

	printf("%s:pDes =%s\n",__func__,des);
	pDes = des;
	unsigned char *outDes=(unsigned char*)malloc(size);
	memset(outDes,0,size);
	
	
	unsigned char *pOut=outDes;
	unsigned char in[3];
	memset(in,0,sizeof(in));

	bufSize =strlen(pDes)/2;
	printf("%s:size = %d,pdes =%s,outPut =%s\n",__func__,bufSize,pDes,pOut);
	
	while(bufSize>0)
	{
		in[0]=*pDes++;
		in[1]=*pDes++;
		in[2]='\0';
		*pOut++=htoi(in);
		printf("v:%s,%0x,%d\n",in,*(pOut-1),htoi(in));
		memset(in,0,sizeof(in));
		bufSize--;
	}
	//memcpy(des,outDes,size/2);
	memset(des,0,strlen(des));
	UTF8ToGB2312(outDes, des, strlen(outDes));
	
	for(i=0;i<strlen(des);i++)
	{
		printf("%0x ",*(des+i));	
	}
	printf("\nlen =%d\n",strlen(des));

	return 0;
}



int main(int argc,char *argv[])
{
	get_handle_data();
	return 0;
}

