#arm-none-linux-gnueabi-gcc -o set_net main.c xml_op.c xml_op.h
gcc -o KillDark main.c xml_op.c xml_op.h
cp KillDark /home/ubuntu/var/ 
echo "cp KillDark /home/ubuntu/var/" 
chmod 777 /home/ubuntu/var/KillDark
