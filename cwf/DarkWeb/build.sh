#arm-none-linux-gnueabi-gcc -o set_net main.c xml_op.c xml_op.h
gcc -o DarkWeb main.c xml_op.c xml_op.h
cp DarkWeb /home/ubuntu/var/ 
echo "cp DarkWeb /home/ubuntu/var/" 
chmod 777 /home/ubuntu/var/DarkWeb
