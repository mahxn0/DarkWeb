/****************************************************************
@filename:data_process.c
@time:08-25
@author:pengkunfan
****************************************************************/
#ifndef _data_update_h_
#define _data_update_h_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "data_define.h"
#include "server_api.h"
#include <errno.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <signal.h>

 entry_buf in_buf[max_input_data];
 int cont_value;
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
#endif



