#arm-none-linux-gnueabi-gcc -o set_net main.c xml_op.c xml_op.h
gcc -o DarkWeb main.c xml_op.c xml_op.h
cp DarkWeb /home/nvidia/var/ 
echo "cp DarkWeb /home/nvidia/var/" 
chmod 777 /home/nvidia/var/DarkWeb
