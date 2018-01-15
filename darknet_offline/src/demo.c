#include "network.h"
#include "detection_layer.h"
#include "region_layer.h"
#include "cost_layer.h"
#include "utils.h"
#include "parser.h"
#include "box.h"
#include "image.h"
#include "demo.h"
#include <sys/time.h>

#define DEMO 1
#define MAXLINE 300
#ifdef OPENCV

static char **demo_names;
static image **demo_alphabet;
static int demo_classes;

static float **probs;
static box *boxes;
static network *net;
static image buff [3];
static image buff_letter[3];
static int buff_index = 0;
static CvCapture * cap;
static CvCapture * cap1=NULL;
static CvCapture * cap2=NULL;
static CvCapture * cap3=NULL;
static CvCapture * cap4=NULL;
static IplImage  * ipl;
static float fps = 0;
static float demo_thresh = 0;
static float demo_hier = .5;
static int running = 0;

static int demo_frame = 3;
static int demo_detections = 0;
static float **predictions;
static int demo_index = 0;
static int demo_done = 0;
static int det_sig=0;
static int a=0,b=0,c=0,d=0;
static float *avg;
double demo_time;

int ipcam_flags=0;
static unsigned char* rtspname=NULL;
typedef struct       //socket通信传输的结构体
{
	char rtsp_url[256];
	int num;
}rtsp_info;

rtsp_info online_rtsp_info;

//添加线程锁，用来同步线程
static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

void *detect_in_thread(void *ptr)
{
    running = 1;
    float nms = .4;

    layer l = net->layers[net->n-1];
    float *X = buff_letter[(buff_index+2)%3].data;
    float *prediction = network_predict(net, X);

    memcpy(predictions[demo_index], prediction, l.outputs*sizeof(float));
    mean_arrays(predictions, demo_frame, l.outputs, avg);
    l.output = avg;
    if(l.type == DETECTION){
        get_detection_boxes(l, 1, 1, demo_thresh, probs, boxes, 0);
    } else if (l.type == REGION){
        get_region_boxes(l, buff[0].w, buff[0].h, net->w, net->h, demo_thresh, probs, boxes, 0, 0, 0, demo_hier, 1);
    } else {
        error("Last layer must produce detections\n");
    }
    if (nms > 0) do_nms_obj(boxes, probs, l.w*l.h*l.n, l.classes, nms);

    //printf("\033[2J");
    //printf("\033[1;1H");
    //printf("\nFPS:%.1f\n",fps);
    //printf("Objects:\n\n");
    image display = buff[(buff_index+2) % 3];
    draw_detections(display, demo_detections, demo_thresh, boxes, probs, 0, demo_names, demo_alphabet, demo_classes);

    demo_index = (demo_index + 1)%demo_frame;
    running = 0;
    return 0;
    
}

void *fetch_in_thread(void *ptr)
{
    int status = fill_image_from_stream(cap, buff[buff_index]);
    letterbox_image_into(buff[buff_index], net->w, net->h, buff_letter[buff_index]);
    if(status == 0) demo_done = 1;
    return 0;
    
}

void *display_in_thread(void *ptr)
{
    show_image_cv(buff[(buff_index + 1)%3], "Demo", ipl);
    int c = cvWaitKey(1);
    if (c != -1) c = c%256;
    if (c == 27) {
        demo_done = 1;
        return 0;
    } else if (c == 82) {
        demo_thresh += .02;
    } else if (c == 84) {
        demo_thresh -= .02;
        if(demo_thresh <= .02) demo_thresh = .02;
    } else if (c == 83) {
        demo_hier += .02;
    } else if (c == 81) {
        demo_hier -= .02;
        if(demo_hier <= .0) demo_hier = .0;
    }
    return 0;
        
}

void *display_loop(void *ptr)
{
    while(1){
        display_in_thread(0);
    }
}

void *detect_loop(void *ptr)
{
    while(1){
        detect_in_thread(0);
    }
}
////tcp监听线程
//void mlisten_thread(void *ptr)
//{
//    //定义socket 监听端口等
//    int    listenfd, connfd;
//    struct sockaddr_in     servaddr;
//    char    serverbuff[300];
//    int     n;    
//	//socket listen message from web
//	  if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
//	{
// 	  printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
//   	  exit(0);
//        }
//  	    memset(&servaddr, 0, sizeof(servaddr));
//    	 servaddr.sin_family = AF_INET;
//         //servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//     	if( inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0)
//	 {
//     		exit(0);
//    	 }	
//    	servaddr.sin_port = htons(6668);
//
//   	 if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
//	{
//    		printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
//    		exit(0);
//    	}
//
//   	 if( listen(listenfd, 10) == -1)
//	{
//    		printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
//    		exit(0);
//    	}
//	
//    	printf("======waiting for client's request======\n");
//	// int a=0,b=0,c=0;
//    // int change_sig[4]={0};
//	while(1)
//	{
//	    	if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1)//阻塞线程，监听端口信息
//		    {
//	        	printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
//	        	continue;
//    		}
//		memset(serverbuff,0,sizeof(serverbuff));
//	    n = recv(connfd, serverbuff, MAXLINE, 0);
//	   	memcpy(&online_rtsp_info, serverbuff , sizeof(rtsp_info));
//	   	printf("rtsp_url=%s  num=%d\n", online_rtsp_info.rtsp_url , online_rtsp_info.num);
//        ipcam_flags=0;
//        rtspname=NULL;
//        ipcam_flags=online_rtsp_info.num;
//		rtspname=online_rtsp_info.rtsp_url;
//        det_sig++;
//        switch(ipcam_flags){
//            case 1:
//                a=1;
//                b=0;
//                c=0;
//                d=0;
//                break;
//            case 2:
//                a=0;
//                b=1;
//                c=0;
//                d=0;
//                break;
//            case 3:
//                a=0;
//                b=0;
//                c=1;
//                d=0;
//                break;
//            case 4:
//                a=0;
//                b=0;
//                c=0;
//                d=1;
//                break;
//        }
//
//	 	close(connfd);
//   	}   
//}
//udp socket
void *mlisten_thread(void *ptr)
{
	   int sock;  
        //创建服务器socket  
        if((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0){  
            perror("socket error");  
            exit(EXIT_FAILURE);  
        }  
        //设置服务器socket参数  
        struct sockaddr_in servaddr;  
        memset(&servaddr, 0, sizeof(servaddr));  
        //设置socket的协议、端口以及地址  
        servaddr.sin_family = AF_INET;  
        servaddr.sin_port = htons(6668);  
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
      
        //将socket绑定地址  
        if(bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){  
            perror("bind error");  
            exit(EXIT_FAILURE);  
        }  
        server(sock); 
        close(sock);
        
}
void server(int sock){  
    //1、建立接收缓存字节数组 
    printf("进入server\n");
    char recvbuf[300] = {0};  
    //2、定义socket  
    struct sockaddr_in peeraddr;  
    socklen_t peerlen;  
    int n;  
    while(1){  
        //  
        peerlen = sizeof(peeraddr);  
        //3、清空接收缓存数组  
        memset(recvbuf, 0, sizeof(recvbuf));  
        //4、开始接收数据  
        n = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&peeraddr, &peerlen);  //没有数据就阻塞
        //5、判断是否接收完整数据  
        if(n == -1){  
                perror("recvfrom");   
                exit(EXIT_FAILURE);   
        }else if(n > 0){  
            //6、成功接收数据，将缓存数据写入到标准输出，打印到控制台  
            printf("%s\n",recvbuf); 
            memcpy(&online_rtsp_info, recvbuf , sizeof(rtsp_info));
            //  printf("rtsp_url=%s  num=%d\n", online_rtsp_info.rtsp_url , online_rtsp_info.num);
            ipcam_flags=0;
       rtspname=NULL;
       ipcam_flags=online_rtsp_info.num;
	   rtspname=online_rtsp_info.rtsp_url;
       printf("rtspname:%s\n",rtspname);
       printf("ipcam_flags:%d\n",ipcam_flags);
       det_sig++;
       switch(ipcam_flags){
           case 1:
               a=1;
               b=0;
               c=0;
               d=0;
               break;
           case 2:
               a=0;
               b=1;
               c=0;
               d=0;
               break;
           case 3:
               a=0;
               b=0;
               c=1;
               d=0;
               break;
           case 4:
               a=0;
               b=0;
               c=0;
               d=1;
               break;
       }
            //7、想客户端回送接收到的数据(回显客户端输入的数据)  
            sendto(sock, recvbuf, n, 0, (struct sockaddr *)&peeraddr, peerlen);  
             
        }  
    }  
    //关闭socket  
     
}  
//轮巡检测
void *mrotate_detect(void *ptr)
{
    pthread_mutex_lock(&mutex);
    pthread_t detect_thread;
    pthread_t fetch_thread;
    srand(2222222);
    if(!cap)
    {
        cap=cvCaptureFromFile(rtspname);
    }
    buff[0] = get_image_from_stream(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);
    ipl = cvCreateImage(cvSize(buff[0].w,buff[0].h), IPL_DEPTH_8U, buff[0].c);
    int count = 0;
    demo_time = what_time_is_it_now();
    printf("开始检测...");
    while(!demo_done){
        buff_index = (buff_index + 1) %3;
        if(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
        if(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
        fps = 1./(what_time_is_it_now() - demo_time);
        demo_time = what_time_is_it_now();
//        display_in_thread(0);
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        ++count;
        //轮训版本1
       // if(det_sig!=0 && (a||b||c||d==1))
       //  {
       //      printf("正在切换到相机%d，请等待...",ipcam_flags+1);
       //      cvReleaseCapture(&cap);
       //      cap=cvCaptureFromFile(rtspname);
       //      a=b=c=d=0;
       //  }
        //轮训版本2
        printf("det_sig=%d\n",det_sig);
        if((det_sig!=1)&&(a||b||c||d)==1)
        {
            switch(ipcam_flags)
            {
                case 1:
                    //cvReleaseCapture(&cap);
                    if(cap1==NULL)
                    {
                        printf("正在打开相机1");
                        cap1=cvCaptureFromFile(rtspname);
                        printf("相机1已打开");
                        cap=cap1;
                    }
                    else{
                        printf("相机1已打开");
                        cap=cap1;
                    }
                    a=b=c=d=0;
                    break;
                case 2:
                    //cvReleaseCapture(&cap);
                    if(cap2==NULL)
                    {
                        printf("正在打开相机2");
                        cap2=cvCaptureFromFile(rtspname);
                        printf("相机2已打开");
                        cap=cap2;
                    }
                    else{
                        printf("相机2已打开");
                        cap=cap2;
                    }
                    a=b=c=d=0;
                    break;
                case 3:

                    //cvReleaseCapture(&cap);
                    if(cap3==NULL)
                    {
                        printf("正在打开相机3");
                        cap3=cvCaptureFromFile(rtspname);
                        printf("相机3已打开");
                        cap=cap3;
                    }
                    else{
                        printf("相机3已打开");
                        cap=cap3;
                    }
                    a=b=c=d=0;
                    break;
                case 4:
                    //cvReleaseCapture(&cap);
                    if(cap4==NULL)
                    {
                        printf("正在打开相机4");
                        cap4=cvCaptureFromFile(rtspname);
                        printf("相机4已打开");
                        cap=cap4;
                    }
                    else{
                        printf("相机4已打开");
                        cap=cap4;
                    }
                    a=b=c=d=0;
                    break;
                default:
                    break;
            }
        }
    
    }
    //
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}

//加载权重初始化网络
void minit_loadnetwork(char *cfgfile, char *weightfile, float thresh,  char **names, int classes, int delay,int avg_frames, float hier)
{
    demo_frame = avg_frames;
    predictions = calloc(demo_frame, sizeof(float*));
    image **alphabet = load_alphabet();
    demo_names = names;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;
    demo_hier = hier;
    printf("Demo\n");
    net = load_network(cfgfile, weightfile, 0);
    set_batch_network(net, 1);
    layer l = net->layers[net->n-1];
    demo_detections = l.n*l.w*l.h;
    int j;
    avg = (float *) calloc(l.outputs, sizeof(float));
    for(j = 0; j < demo_frame; ++j) predictions[j] = (float *) calloc(l.outputs, sizeof(float));

    boxes = (box *)calloc(l.w*l.h*l.n, sizeof(box));
    probs = (float **)calloc(l.w*l.h*l.n, sizeof(float *));
    for(j = 0; j < l.w*l.h*l.n; ++j) probs[j] = (float *)calloc(l.classes+1, sizeof(float));
    //cvNamedWindow("Demo", CV_WINDOW_NORMAL); 
    //cvMoveWindow("Demo", 0, 0);
    //cvResizeWindow("Demo", 1352, 1013);
    //cvWaitKey(100);
    pthread_t socket_thread;
    pthread_t rotate_thread;
    pthread_create(&socket_thread,NULL,mlisten_thread,NULL);
    while(1)
    {
        printf("等待客户端信号......\n");
        if(ipcam_flags==0)
        {
            continue;
        }
        else{
            printf("已接收到信号......\n");
            pthread_create(&rotate_thread,NULL,mrotate_detect,NULL);
            pthread_join(rotate_thread,0);
        }
     }
     pthread_join(socket_thread,0);
}
void demo(char *cfgfile, char *weightfile, float thresh, int cam_index, const char *filename, char **names, int classes, int delay, char *prefix, int avg_frames, float hier, int w, int h, int frames, int fullscreen)
{
    demo_frame = avg_frames;
    predictions = calloc(demo_frame, sizeof(float*));
    image **alphabet = load_alphabet();
    demo_names = names;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;
    demo_hier = hier;
    printf("Demo\n");
    net = load_network(cfgfile, weightfile, 0);
    set_batch_network(net, 1);
    pthread_t detect_thread;
    pthread_t fetch_thread;

    srand(2222222);

    if(filename){
        printf("video file: %s\n", filename);
        cap = cvCaptureFromFile(filename);
    }else{
        cap = cvCaptureFromCAM(cam_index);

        if(w){
            cvSetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH, w);
        }
        if(h){
            cvSetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT, h);
        }
        if(frames){
            cvSetCaptureProperty(cap, CV_CAP_PROP_FPS, frames);
        }
    }

    if(!cap) error("Couldn't connect to webcam.\n");

    layer l = net->layers[net->n-1];
    demo_detections = l.n*l.w*l.h;
    int j;

    avg = (float *) calloc(l.outputs, sizeof(float));
    for(j = 0; j < demo_frame; ++j) predictions[j] = (float *) calloc(l.outputs, sizeof(float));

    boxes = (box *)calloc(l.w*l.h*l.n, sizeof(box));
    probs = (float **)calloc(l.w*l.h*l.n, sizeof(float *));
    for(j = 0; j < l.w*l.h*l.n; ++j) probs[j] = (float *)calloc(l.classes+1, sizeof(float));

    buff[0] = get_image_from_stream(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);
    ipl = cvCreateImage(cvSize(buff[0].w,buff[0].h), IPL_DEPTH_8U, buff[0].c);

    int count = 0;
    if(!prefix){
        cvNamedWindow("Demo", CV_WINDOW_NORMAL); 
        if(fullscreen){
            cvSetWindowProperty("Demo", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
        } else {
            cvMoveWindow("Demo", 0, 0);
            cvResizeWindow("Demo", 1352, 1013);
        }
    }

    demo_time = what_time_is_it_now();

    while(!demo_done){
        buff_index = (buff_index + 1) %3;
        if(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
        if(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
        if(!prefix){
            fps = 1./(what_time_is_it_now() - demo_time);
            demo_time = what_time_is_it_now();
            display_in_thread(0);
        }else{
            char name[256];
            sprintf(name, "%s_%08d", prefix, count);
            save_image(buff[(buff_index + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
 //      	if(count%2==0)
	    pthread_join(detect_thread, 0);
        ++count;
    }
}

void demo_compare(char *cfg1, char *weight1, char *cfg2, char *weight2, float thresh, int cam_index, const char *filename, char **names, int classes, int delay, char *prefix, int avg_frames, float hier, int w, int h, int frames, int fullscreen)
{
    demo_frame = avg_frames;
    predictions = calloc(demo_frame, sizeof(float*));
    image **alphabet = load_alphabet();
    demo_names = names;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;
    demo_hier = hier;
    printf("Demo\n");
    net = load_network(cfg1, weight1, 0);
    set_batch_network(net, 1);
    pthread_t detect_thread;
    pthread_t fetch_thread;

    srand(2222222);

    if(filename){
        printf("video file: %s\n", filename);
        cap = cvCaptureFromFile(filename);
    }else{
        cap = cvCaptureFromCAM(cam_index);

        if(w){
            cvSetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH, w);
        }
        if(h){
            cvSetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT, h);
        }
        if(frames){
            cvSetCaptureProperty(cap, CV_CAP_PROP_FPS, frames);
        }
    }

    if(!cap) error("Couldn't connect to webcam.\n");

    layer l = net->layers[net->n-1];
    demo_detections = l.n*l.w*l.h;
    int j;

    avg = (float *) calloc(l.outputs, sizeof(float));
    for(j = 0; j < demo_frame; ++j) predictions[j] = (float *) calloc(l.outputs, sizeof(float));

    boxes = (box *)calloc(l.w*l.h*l.n, sizeof(box));
    probs = (float **)calloc(l.w*l.h*l.n, sizeof(float *));
    for(j = 0; j < l.w*l.h*l.n; ++j) probs[j] = (float *)calloc(l.classes+1, sizeof(float));

    buff[0] = get_image_from_stream(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);
    ipl = cvCreateImage(cvSize(buff[0].w,buff[0].h), IPL_DEPTH_8U, buff[0].c);

    int count = 0;
    if(!prefix){
        cvNamedWindow("Demo", CV_WINDOW_NORMAL); 
        if(fullscreen){
            cvSetWindowProperty("Demo", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
        } else {
            cvMoveWindow("Demo", 0, 0);
            cvResizeWindow("Demo", 1352, 1013);
        }
    }

    demo_time = what_time_is_it_now();

    while(!demo_done){
        buff_index = (buff_index + 1) %3;
        if(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
        if(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
        if(!prefix){
            fps = 1./(what_time_is_it_now() - demo_time);
            demo_time = what_time_is_it_now();
            display_in_thread(0);
        }else{
            char name[256];
            sprintf(name, "%s_%08d", prefix, count);
            save_image(buff[(buff_index + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        ++count;
    }
}
#else
void demo(char *cfgfile, char *weightfile, float thresh, int cam_index, const char *filename, char **names, int classes, int delay, char *prefix, int avg, float hier, int w, int h, int frames, int fullscreen)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#endif

