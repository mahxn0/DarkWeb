./darknet detector demo cfg/voc.data tiny-yolo-voc.cfg tiny-yolo-voc_fin rtsp://admin:buaa123456@192.168.1.109:554/h264/ch1/main/av_stream latency=0 ! rtph264depay ! h264parse ! omxh264dec ! videoconvert ! appsink sync=false
