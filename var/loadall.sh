
cd /home/ubuntu/var
#mylog
mydate=$(date )
sleep 1

echo 'ubuntu'|sudo -S ./set_net > /home/ubuntu/var/log/SetNet.log 2>&1 &

echo 'ubuntu'|sudo -S ./mac_op &

sleep 1

echo 'ubuntu'|sudo -S ./boa > /home/ubuntu/var/log/Boa.log 2>&1 &

#cd /home/ubuntu/var/test

echo 'ubuntu'|sudo -S ./DarkWeb > /home/ubuntu/var/log/DarkWeb.log 2>&1 &


#echo 'ubuntu'|sudo -S firefox 192.168.1.16
