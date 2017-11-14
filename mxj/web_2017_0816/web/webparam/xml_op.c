
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "xml_op.h"
//#define CFG_FILE_PATH ("/opt/dvr_rdk/ti814x/config.xml")
//#define CFG_FILE_PATH ("/opt/dvr_rdk/ti814x/var/www/cgi-bin/version.xml")
#define CFG_FILE_PATH ("/home/ubuntu/var/www/cgi-bin/version.xml")
//#define SYS_CFG_PATH ("/opt/dvr_rdk/ti814x/var/www/cgi-bin/sys.xml")
#define SYS_CFG_PATH ("/home/ubuntu/var/www/cgi-bin/sys.xml")


static FILE *fp_cfg=NULL;
static char *file_content=NULL;
static long file_size=0;

static int   default_port   = 5010;
static int   default_width  = 736;
static int   default_height = 576;

static char *str_xml=
"<?xml version=\"1.0\"?>\r\n" 
"<udp_recv>\r\n"
"<frame_width>%d</frame_width>\r\n"
"<frame_height>%d</frame_height>\r\n"
"<port>%d</port>\r\n"
"</udp_recv>\r\n\r\n";

int create_config_file(char *file,int width,int height,int port){
	char tmp[1024*100]="";
	int ret=0;

	sprintf(tmp,str_xml,width,height,port);
	
	if(fp_cfg == NULL){
		fp_cfg=fopen(file,"wb");
		if(fp_cfg == NULL){
			return -1;
		}
	}else
		fseek(fp_cfg,0,SEEK_SET);

	ret=fwrite(tmp,1,strlen(tmp),fp_cfg);
	if(ret != strlen(tmp))
		return -2;

	if(fp_cfg){
		fclose(fp_cfg);
		fp_cfg=NULL;
	}

	return 0;
}


int tir_config_open(char *file){
	int ret=0;

	if(access(file,F_OK) < 0){/*
		ret=create_config_file(file,default_width,default_height,default_port);
		if(ret < 0)
			return -10;*/
		//MessageBox(NULL, "have no cfg file", "error", MB_OK);
		return -10;
	}
	
	fp_cfg=fopen(file,"rb+");
	if(fp_cfg == NULL){
		printf("can not open config file!please check it\n");
		return -1;
	}

	fseek(fp_cfg,0,SEEK_END);
	file_size=ftell(fp_cfg);
	fseek(fp_cfg,0,SEEK_SET);

	file_content=(char *)malloc(file_size+1);
	if(file_content == NULL){
		printf("malloc for config failed!\n");
		return -2;
	}
	
	ret=fread(file_content,1,file_size,fp_cfg);
	if(ret != file_size){
		printf("read config file failed!\n");
	}
	return 0;
}

void tir_config_close(){
	if(fp_cfg){
		fclose(fp_cfg);
		fp_cfg=NULL;
	}
	if(file_content){
		free(file_content);
		file_content=NULL;
	}
}

int tir_config_read(char *element,char *value,int *vlen){
	int offset=0;
	char tag[10][50]={0};
	int tag_index=0;
	int tag_used=0;
	int i=0;
	char *p=NULL;
	char *p_tag=NULL;
	int start_pos=0;
	int end_pos=0;

	*vlen = 0;

	if(fp_cfg == NULL) return -1;

	while(element[offset] != '\0' && tag_index < 10){
		if(element[offset] == '.'){
			tag[tag_index][i]='\0';
			tag_index++;
			i=0;
		}else{
			tag[tag_index][i]=element[offset];
			i++;
		}
		offset++;
	}
	tag_used=tag_index;

	i=0;
	offset=0;
	tag_index=0;
	p=file_content;
	while(offset < file_size && tag_index <= tag_used){
		if(p[offset] == '<'){
			if( memcmp(p+offset+1,tag[tag_index],strlen(tag[tag_index])) == 0 ){
				//printf("found %s\n",tag[tag_index]);
				if(tag_index == tag_used){
				start_pos=offset+1+strlen(tag[tag_index])+1;
				i=start_pos;
				while((p[i] != '<' || p[i+1] != '/') && i < file_size ) i++;
				end_pos=i;
				*vlen=end_pos-start_pos;
				memcpy(value,p+start_pos,*vlen);
				}

				tag_index++;
			}
		}

		offset++;
	}

	value[*vlen] = '\0';

	return 0;
}


int tir_config_write(char *element,char *value,int vlen){
	int offset=0;
	char tag[10][50]={0};
	int tag_index=0;
	int tag_used=0;
	int i=0;
	char *p=NULL;
	char *p_tag=NULL;
	int start_pos=0;
	int end_pos=0;
	char *new_buf=NULL;

	if(fp_cfg == NULL) return -1;

	new_buf=(char *)malloc(file_size+vlen+1);
	if(new_buf == NULL){
		printf("malloc for writing config failed!\n");
		return -1;
	}
	memset(new_buf, 0, file_size+vlen+1);
	
	while(element[offset] != '\0' && tag_index < 10){
		if(element[offset] == '.'){
			tag[tag_index][i]='\0';
			tag_index++;
			i=0;
		}else{
			tag[tag_index][i]=element[offset];
			i++;
		}
		offset++;
	}
	tag_used=tag_index;

	i=0;
	offset=0;
	tag_index=0;
	p=file_content;
	while(offset < file_size && tag_index <= tag_used){
		if(p[offset] == '<'){
			if( memcmp(p+offset+1,tag[tag_index],strlen(tag[tag_index])) == 0 ){
				//printf("found %s\n",tag[tag_index]);
				if(tag_index == tag_used){
					start_pos=offset+1+strlen(tag[tag_index])+1;
					i=start_pos;
					while((p[i] != '<' || p[i+1] != '/') && i < file_size ) i++;
					end_pos=i;
					memcpy(new_buf,p,start_pos);
					memcpy(new_buf+start_pos,value,vlen);
					memcpy(new_buf+start_pos+vlen,p+end_pos,file_size-end_pos);

					if(file_content){
						free(file_content);
						file_content=new_buf;
						file_size=file_size+vlen-(end_pos-start_pos);
					}
				}

				tag_index++;
			}
		}

		offset++;
	}

	return 0;
}

int tir_flush_last_config_to_file(){
	/*FILE *fp_new=NULL;
	int ret=0;

	fp_new = fopen("new.cfg","wb");
	if(fp_new == NULL){
		printf("rewrite config file failed!\n");
		return -1;
	}
	
	ret=fwrite(file_content,1,file_size,fp_new);
	if(ret != file_size){
		return -2;
	}

	if(fp_new){fclose(fp_new);}*/

	int ret=0;

	if(fp_cfg == NULL || file_content == NULL) return -1;

	fseek(fp_cfg,0,SEEK_SET);
	ret=fwrite(file_content,1,file_size,fp_cfg);
	if(ret != file_size)
		return -2;

	return 0;
}

void set_bitrate(int bitrate)
{
	

	char *netMem[] = {"bitrate","mac","gateway","netmask",NULL};
	char tbuf[300];
	int ret;
	char value[32] = {0};

	printf("write now ....\n");
	ret = tir_config_open(CFG_FILE_PATH);
	if(ret < 0) return;

	sprintf(tbuf, "codec.ch0.%s",netMem[0]);
	sprintf(value, "%d", bitrate);
	
	tir_config_write(tbuf, value, strlen(value));

	tir_flush_last_config_to_file();
	tir_config_close();	
}

int get_bitrate()
{
	char tbuf[300] = {0};
	char str[200] = {0};
	int len;
	int bitrate = 0; 
	if( tir_config_open(CFG_FILE_PATH) < 0)
	{
        printf("cannot open cfg file!\n");
    }
	
	sprintf(tbuf, "codec.ch0.bitrate");
	if(tir_config_read(tbuf, str, &len) == 0)
	{
			str[len] = 0;
			printf("str = %s\n",str);
    }
	
	bitrate = atoi(str);
	tir_config_close(); 

	return bitrate;
}


int get_kernel_version(char *strArg)
{
	char tbuf[300] = {0};
	char str[200] = {0};
	int len;
	
	if( tir_config_open(CFG_FILE_PATH) < 0)
	{
       // printf("cannot open cfg file!\n");
    }
	
	sprintf(tbuf, "kernel");
	if(tir_config_read(tbuf, str, &len) == 0)
	{
			str[len] = 0;
			//printf("str = %s\n",str);
    }
	
	tir_config_close(); 

	strcpy(strArg, str);
	return 0;
}
int get_fs_version(char *strArg)
{
	char tbuf[300] = {0};
	char str[200] = {0};
	int len;
	
	if( tir_config_open(CFG_FILE_PATH) < 0)
	{
        printf("cannot open cfg file!\n");
    }
	
	sprintf(tbuf, "fs");
	if(tir_config_read(tbuf, str, &len) == 0)
	{
			str[len] = 0;
			//printf("str = %s\n",str);
    }
	
	tir_config_close(); 

	strcpy(strArg, str);
	return 0;
}
int get_hw_version(char *strArg)
{
	char tbuf[300] = {0};
	char str[200] = {0};
	int len;
	
	if( tir_config_open(CFG_FILE_PATH) < 0)
	{
        printf("cannot open cfg file!\n");
    }
	
	sprintf(tbuf, "hw");
	if(tir_config_read(tbuf, str, &len) == 0)
	{
			str[len] = 0;
			//printf("str = %s\n",str);
    }
	
	tir_config_close(); 

	strcpy(strArg, str);
	return 0;
}
int get_devId_version(char *strArg)
{
	char tbuf[300] = {0};
	char str[200] = {0};
	int len;
	
	if( tir_config_open("/home/ubuntu/var/ip.xml") < 0)
	{
        printf("cannot open cfg file!\n");
    }
	
	sprintf(tbuf, "serialNo");
	if(tir_config_read(tbuf, str, &len) == 0)
	{
			str[len] = 0;
			//printf("str = %s\n",str);
    }
	
	tir_config_close(); 

	strcpy(strArg, str);
	return 0;
}



#if 0
int wwwmain_bak(int argc,char **argv)
{
	printf("write now...\n");
	set_bitrate(16000);
	printf("read now ....\n");
	printf("read birRate:%d ....\n", get_bitrate());
	return 0;
}

int test_main(int argc,char **argv)
{
	char buf[100] = {0};
	 get_kernel_version(buf);
	 printf("result:%s\n",buf);
	return 0;
}
#endif

void save_to_xml(char *element,char *arg_value)
{
	

	char *netMem[] = {"bitrate","mac","gateway","netmask",NULL};
	char tbuf[300];
	int ret;
	//char value[32] = {0};

	//printf("write now ....\n");
	ret = tir_config_open(SYS_CFG_PATH);
	if(ret < 0) return;

	//sprintf(tbuf, element);
	//sprintf(value, "%s", ip);
	
	tir_config_write(element, arg_value, strlen(arg_value));

	tir_flush_last_config_to_file();
	tir_config_close();	
	system("sync");
}


int get_from_xml(char *element,char *arg_value)
{
	char tbuf[300] = {0};
	char str[200] = {0};
	int len;
	
	if( tir_config_open(SYS_CFG_PATH) < 0)
	{
        printf("cannot open cfg file!\n");
    }
	
	//sprintf(tbuf, "network.ip");
	if(tir_config_read(element, str, &len) == 0)
	{
			str[len] = 0;
			//printf("str = %s\n",str);
    }
	
	tir_config_close(); 

	strcpy(arg_value, str);
	return 0;
}




