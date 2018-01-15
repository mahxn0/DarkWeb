#!/bin/bash

if [ $# = 1 ]
then
    echo $1
    value1="./darknet_offline detector test cfg/coco.data cfg/yolo.cfg yolo.weights "
    value2=" -out /home/ubuntu/var/www/sptp_upload/dog"
    value=$value1$1$value2
    echo $value
    echo 11111111111111111111111111111111111111111111
    cd /home/ubuntu/darknet_offline/
    $value &
    echo 22222222222222222222222222222222222222222222
else
    echo "param num is error"
fi

exit
