#!/bin/bash
#@auther:maxiangjun
#@date:2018-1-5
#func:Lock GPU and MEM clocks to max MHZ
# Lock GPU and MEM clocks to max MHz
#
# - benchmarking shows there is no need to override the MEM clock
#
echo
echo Locking GPU clock to max MHz for debugging porpoises...
 
# set to max
cat /sys/kernel/debug/clock/override.gbus/max > /sys/kernel/debug/clock/override.gbus/rate
cat /sys/kernel/debug/clock/override.emc/max  > /sys/kernel/debug/clock/override.emc/rate
 
# enable override
echo 1 > /sys/kernel/debug/clock/override.gbus/state
echo 1 > /sys/kernel/debug/clock/override.emc/state
 
echo
echo "                                    __               "
echo "                                _.-~  )  GPU:" $[ $(</sys/kernel/debug/clock/gbus/rate) / 1000000] MHz
echo "                     _..--~~~~,'   ,-/     _         "
echo "                  .-'. . . .'   ,-','    ,' )        "
echo "                ,'. . . _   ,--~,-'__..-'  ,'        "
echo "              ,'. . .  (@)' ---~~~~      ,'          "
echo "             /. . . . '~~             ,-'            "
echo "            /. . . . .             ,-'               "
echo "           ; . . . .  - .        ,'                  "
echo "          : . . . .       _     /                    "
echo "         . . . . .          \`-.:                    "
echo "        . . . ./  - .          )                     "
echo "       .  . . |  _____..---.._/ _____                "
echo " ~---~~~~----~~~~             ~~                     "
echo
