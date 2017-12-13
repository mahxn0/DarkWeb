
/****************************************************************
@filename:data_define.h
@time:08-25
@author:pengkunfan
****************************************************************/
#ifndef _data_header
#define _data_header
#include <arpa/inet.h>
#include <net/if.h>
#define max_input_data 1024*512
#define max_batch 8
#define max_channel_id 32
typedef struct __COMMAND_ARGUMENT {
    char *name;
    char *value;
    int flags;
} COMMAND_ARGUMENT;
typedef struct _entry_buf
{
	char *name;
	char *value;
	int entry_falg;
	int index;
}entry_buf;

typedef struct _url_option
{
	char *name;
	char *value;
	void			(*handler)(char *, COMMAND_ARGUMENT *);
	int url_flag;
}url_option;

#if 0
typedef struct _ipaddr
{
	in_addr_t ip_add;
	in_addr_t net_mask;
	in_addr_t gateway;
	in_addr_t nameserver;
}ipaddr;
#else
typedef struct _ipaddr
{
	char ip_add[32];
	char net_mask[32];
	char gateway[32];
	char nameserver[32];
}ipaddr;
#endif


typedef struct _datatime
{
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
	int msec;
	int usec;

}datetime;

typedef struct _rtspch
{
	
	unsigned char rtsp[200];
	unsigned char reserve[10];
	int flag;
}rtspch;

typedef struct _rtspaddr
{
	int batch;
	int channel;
	//unsigned char rtsp[200];
	int bc[max_batch][max_channel_id];
	rtspch chrtsp[max_channel_id];
	unsigned char reserve[100];
	int mult_flag;
	int flag;
	int change_time;
	int channel_numbs;
}rtspaddr;
typedef struct _encode_param
{
	int bit_rate;
	int resolution;
	int fps;
	int bit_ctrl;
}encode_param;

typedef struct _system_param
{
	ipaddr camera_net;
	datetime camera_time;
	rtspaddr p_rtspaddr;
	encode_param en_param;
	int client_type;
	int pb_volume;//play back volume
	int r_volume;//record volume
	unsigned char reserve[20];
	unsigned char osd_str[100];
	int osd_size;
	int osd_color;
	int osd_x;
	int osd_y;
	unsigned char device_addr[100];
	int osd_time;
	int arithmeticpara1;
	int arithmeticpara2;
	int arithmeticpara3;
	int arithmetic_recvideo;
	unsigned char rtsp_url[500];
	int start_opencv;
	char serverip[32];
	int serverport;
}system_param;

#define __u32 unsigned int
#define __u8 unsigned char
typedef union __NET_IPV4 {
	__u32	int32;
	__u8	str[4];
} NET_IPV4;
#define TRUE		1
#define FALSE	0

system_param decoder_param;
 entry_buf in_buf[max_input_data];
 int cont_value;

int GB2312ToUTF8(const unsigned char *_Gb2312, unsigned char *_Utf8, int _iBufSize);
unsigned short Utf8toUnicode ( const unsigned char  *_pucUtf8, int *_iBytes);
int UTF8ToGB2312(const unsigned char *_Utf8, unsigned char *_Gb2312, int _iBufSize);

#define index_repose_defult 0
#define index_repose_date 4
#define index_repose_ipaddr 5
#define index_repose_rtspaddr 6

#define index_repose_serverip 55


#define sucess_ok 0

#else

#endif



