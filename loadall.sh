#!/bin/sh
#@auther:maxiangjun
#@data:2018/1/11-17:53
#@function:init加载检测程序和服务器
VAR=/home/ubuntu/var
FILE1=/home/ubuntu/var/log/runrotate.log
FILE2=/home/ubuntu/var/log/nopoll.log
cd $VAR
#设置网络IP
date
echo 'ubuntu'|sudo -S ./cpu.sh
sleep 1
echo 'ubuntu'|sudo -S ./tk1_gpu_max_clock.sh
sleep 1
echo 'ubuntu'|sudo -S ./set_net > /home/ubuntu/var/log/SetNet.log 2>&1 &
echo "@网络设置成功"
echo 'ubuntu'|sudo -S ./server > /home/ubuntu/var/log/server.log 2>&1 &
echo "@后端服务器已启动"
echo 'ubuntu'|sudo -S ./nopoll-regression-listener > /home/ubuntu/var/log/nopoll.log 2>&1 &
echo "@后台发送程序已启动"
while true
do
        FILESIZE1=`ls -l $FILE1 | awk '{print $2}'`
#	FILESIZE2=`ls -l $FILE2 | awk '{print $3}'`
	if [ $FILESIZE1 -gt 262144 ] 
	then
	echo :> $FILE1
	fi
	FILESIZE2=`ls -l $FILE2 | awk '{print $3}'`
	if [ $FILESIZE2 -gt 1048 ]
	then 
	echo :> $FILE2
	fi
	count=`ps -ef | grep DetectDemo_Vid | grep -v "grep" | wc -l`
        echo $count
        if [ $count -eq 0 ]
	then
        #将标准输出和错误输出重定向到log文件
        echo "没有找到检测进程，准备重启..."
	#count1=`ps -ef | grep roll | grep -v "grep" | wc -l`
	#	if [ $count1 -eq 1 ]
	#	then
	#	echo 'ubuntu'|sudo -S killall -9 roll 
	#	fi
        echo 'ubutnu' |sudo -S ./runrotate.sh > /home/ubuntu/var/log/runrotate.log 2>&1 & 
	echo 'ubuntu' |sudo -S ./run_picdet.sh > /home/ubuntu/var/log/picdet.log 2>&1 &
	sleep 10
	count1=`ps -ef | grep roll | grep -v "grep" | wc -l`
               if [ $count1 -eq 1 ]
               then
               echo "轮训程序启动成功" 
	       else 
	       echo 'ubuntu'|sudo -S ./roll > /home/ubuntu/var/log/roll.log 2>&1 
	       echo "没有找到轮训进程，准备重启..."
               fi
	#echo 'ubuntu'|sudo -S ./roll > /home/ubuntu/var/log/roll.log 2>&1 &
	else
		echo "检测程序启动成功"
	fi
done
        




