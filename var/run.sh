#!/bin/bash
#cd /home/ubuntu/darknet/

#./cpu.sh

#sleep 1

#./tk1_gpu_max_clock.sh

#sleep 1

./darknet detector demo voc.data tiny-yolo-voc.cfg tiny-yolo-voc.weights  "rtspsrc location=\"rtsp://admin:12345@192.168.0.2:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif\" latency=10 ! rtph264depay ! h264parse ! omxh264dec ! videoconvert ! appsink"
