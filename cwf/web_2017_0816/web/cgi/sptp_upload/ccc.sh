#!/bin/bash

if [ $# = 1 ]
then
    #echo 222222222222222222226666666
    /home/ubuntu/darknet_offline/runpic_det.sh $1 &
    #echo 3333333333333333333333333333
else
    echo "param num is error"
fi

exit 1
