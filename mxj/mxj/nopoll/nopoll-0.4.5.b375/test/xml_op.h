#ifndef __TIR_CONFIG_H__
#define __TIR_CONFIG_H__

#ifndef MAX_PATH
#define MAX_PATH (260)
#endif



int tir_config_open(char *file);
int tir_config_read(char *element,char *value,int *vlen);
int tir_config_write(char *element,char *value,int vlen);
int tir_flush_last_config_to_file(void);
void tir_config_close(void);
void save_to_xml(char *element,char *arg_value);
int get_from_xml(char *element,char *arg_value);



#endif

