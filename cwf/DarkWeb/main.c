#include<stdio.h>
#include<stdlib.h>
#include"xml_op.h"
#include<pthread.h>
static int start_sig=0;
void gotodetect()
{
	char rtspname[300]={0};
	char cmd[600]={0};
	//从网页获取rtsp的通道
	get_from_xml("rtsp.rtsp_url",rtspname);
	//sprintf(rtspname,"rtspsrc location=\\%s\\ latency=10 ! rtph264depay ! h264parse ! omxh264dec ! videoconvert ! appsink",rtspname);
	system("./date.sh\n");
	sprintf(cmd,"./darknet detector demo voc.data tiny-yolo-voc.cfg tiny-yolo-voc.weights \"rtspsrc location=\\\"%s\\\"  latency=10 ! rtph264depay ! h264parse ! omxh264dec ! videoconvert ! appsink\"",rtspname);
	printf("%s\n",cmd);
	system(cmd);
}
void getdetectsig()
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
		getdetectsig();
		if(start_sig==1)
		{
			printf("Detect start!!!\n");
			gotodetect();
		}
		else
		{
			system("killall -9 darknet");
			break;
		}
	}
	system("killall -9 darknet");
	printf("Detect stop!!!\n");
	goto label;
	return 0;

}
