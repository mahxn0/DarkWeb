target = roll_2

.PHONY:clean release


#BASE_INSTALL_DIR    := /opt/arm-2009q1
#BUILD_TOOL_DIR      := $(BASE_INSTALL_DIR)
#BUILD_TOOL_PREFIX   := $(BUILD_TOOL_DIR)/bin/arm-none-linux-gnueabi-

Cc = $(BUILD_TOOL_PREFIX)gcc

Cflags = -lpthread
# -Wall -O3
Incs =$(wildcard *.h)
Srcs =$(wildcard *.c)
objects =$(Srcs:%.c=%.o) 

all:release fsupdate

release:$(target)

fsupdate:
#	cp $(target) /home/tirvideo/lyb/armfs/opt/dvr_rdk/ti814x/var/www/cgi-bin/
	cp $(target) /home/nvidia/var/www/cgi-bin/

$(target):$(objects)
	$(Cc) $^ -o $@ $(Incs) $(Cflags) 
	
$(objects):%.o:%.c $(Incs) $(Cflags) 
	$(Cc) -c $< -o $@

clean:
	-rm *.o $(target)
#	-rm $@
