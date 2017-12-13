#include<stdlib.h>
#include<stdio.h>
#include"xml_op.h"
static int start_sig=0;
void detectkillsig()
{
	char cvip[1]={0};
	get_from_xml("rtsp.opencv",cvip);
	start_sig=atoi(cvip);
}
int main()
{
	label:while(1)
	{
		sleep(2);
		if(start_sig==0)
		{
			system("killall -9 darknet");
		}
		else 
		{
			break;
		}
	
	}
	goto label;
	return 0;
}
