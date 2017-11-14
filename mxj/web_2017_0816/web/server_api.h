/****************************************************************
@filename:server_api.h
@time:08-25
@author:pengkunfan
****************************************************************/
#ifndef __SERVER_API_H__
#define __SERVER_API_H__

#define TRUE		1
#define FALSE	0

#define HTTP_OPTION_CHANGED		0x0001
  
#define OPTION_OK		"OK "
#define OPTION_NG		"NG "
#define OPTION_NS		"NS "
#define OPTION_UW		"UW "
#define OPTION_UA		"UA "

#define MIN_HTTP_PORT		1
#define MAX_HTTP_PORT		65535
#define __u32 unsigned int
#define __u8 unsigned char
typedef union __NET_IPV4 {
	__u32	int32;
	__u8	str[4];
} NET_IPV4;

#include "data_define.h"
#include "net_config.h"
int hash_table_init(void);
void reply_display(int html_id);
int reset_value(unsigned char *inbuf,unsigned char *outbuf,unsigned char  *data_char,int flag);
int reset_check_selected(unsigned char *inbuf,unsigned char *outbuf,int flag);
int add_check_selected(unsigned char *inbuf,unsigned char *outbuf,int flag);
char *make_data(char *line,char stop);
char *fmake_data(FILE *f,char stop,int *len);
char u2c(char *what);
void get_data(char *word,char *line,char stop);
void unescape_url(char *url);
void plustospace(char *str);
char *get_value(char *s,int cont_value,entry_buf in_buf[]);
int  get_index(entry_buf in_buf[]);
int netsplit( char *pAddress, void *ip );
int ipv4_str_to_num(char *data, struct in_addr *ipaddr);

int GB2312ToUTF8(const unsigned char *_Gb2312, unsigned char *_Utf8, int _iBufSize);
unsigned short Utf8toUnicode ( const unsigned char  *_pucUtf8, int *_iBytes);
int UTF8ToGB2312(const unsigned char *_Utf8, unsigned char *_Gb2312, int _iBufSize);
int htoi(char s[]);
int DeBaiFenStrin2Int(unsigned char *des,unsigned char *src,int size);

int http_Paraget_cmd(char *addr, int max_size);
//set sysctl
int  set_ftpuse(char *req, COMMAND_ARGUMENT *argm);

void get_ipaddr();
void get_rtspaddr();
int sys_set_time(int hour, int min, int sec);

int video_process(entry_buf inbuf[],int cont_length);
int date_process(entry_buf in_buf[],int cont_length);
int ipaddr_process(entry_buf in_buf[],int cont_length);

int serveripaddr_process(entry_buf in_buf[],int cont_length);


int rtspaddr_process(entry_buf in_buf[],int cont_length);



#endif /* __SERVER_API_H__ */

