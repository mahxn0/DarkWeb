#arm-none-linux-gnueabi-gcc -o set_net main.c xml_op.c xml_op.h
gcc -o set_net main.c xml_op.c xml_op.h
cp set_net /home/ubuntu/var/ 
echo "cp set_net /home/ubuntu/var/" 
chmod 777 /home/ubuntu/var/set_net
