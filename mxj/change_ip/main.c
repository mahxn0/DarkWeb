
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include "data_define.h"


#define	  CFG_FILE "/opt/dvr_rdk/ti814x/var/www/cgi-bin/sys.cfg"
int set_net()
{

	//FILE *fp;
	unsigned int i=0;
	unsigned char cmd[100] = {0};
	unsigned  char ip[20] = {0};
	unsigned  char netmask[20] = {0};
	unsigned  char gw[20] = {0};
#if 0
	fp = fopen(CFG_FILE,"rb");
	if(fp == NULL)
	{	
		printf("rtsp cfg file not exit!\n");
		return -1;
	}
	fread(&decoder_param,1,sizeof(system_param),fp);
	fclose(fp);

	printf("ip:%s\n",inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.ip_add));
	printf("netmask:%s\n",inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.net_mask));
	printf("gateway:%s\n",inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.gateway));
	printf("bitrate:%d\n",decoder_param.en_param.bit_rate);


	//net_set_ifaddr("eth0", decoder_param.camera_net.ip_add);

	sprintf(ip, "%s", inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.ip_add));
	sprintf(netmask, "%s", inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.net_mask));
	sprintf(gw, "%s", inet_ntoa(*(struct in_addr *)&decoder_param.camera_net.gateway));
#else
	

		get_from_xml("network.ip",ip);
		get_from_xml("network.netmask",netmask);
		get_from_xml("network.gateway",gw);
	

#endif
	system("./date.sh");	
	printf("111ip:%s\n", ip);
	printf("222netmask:%s\n", netmask);
	printf("222gw:%s\n", gw);
	
	memset(cmd, 0 ,sizeof(cmd));
	sprintf(cmd, "ifconfig eth0 %s netmask %s",ip, netmask);
	printf("cmd:%s\n", cmd);
	system(cmd);

	memset(cmd, 0 ,sizeof(cmd));
	sprintf(cmd, "route add default gw %s",gw);
	system(cmd);

}





int main(int argc, char* argv[])
{
	set_net();
	return 0;
}



