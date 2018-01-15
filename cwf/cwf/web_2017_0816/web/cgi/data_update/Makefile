#for makefile web module by pkf
.PHONY: clean all

INSTALLDIR = $(EXEC_DIR)

GCC_FLAGS = -Wstrict-prototypes -Wpointer-arith -Wcast-align -Wcast-qual\
  -Wtraditional\
  -Wshadow\
  -Wconversion\
  -Waggregate-return\
  -Wmissing-prototypes\
  -Wnested-externs\
  -Wall \
  -Wundef -Wwrite-strings -Wredundant-decls -Winline \
  -mshort-calls 

#BASE_INSTALL_DIR    := /opt/arm-2009q1
#BUILD_TOOL_DIR      := $(BASE_INSTALL_DIR)
#BUILD_TOOL_PREFIX   := $(BUILD_TOOL_DIR)/bin/arm-none-linux-gnueabi-


srcdir = .



LDFLAGS = -g -lpthread   
LIBS = 
CFLAGS = -O3 -pipe -Wall   

#LDFLAGS += -g -L/opt/dvsdk/DVRRDK_02.80.00.10/ti_tools/linux_devkit/arm-none-linux-gnueabi/usr/lib  -lxml2 -lz
#CFLAGS += -I /opt/dvsdk/DVRRDK_02.80.00.10/ti_tools/linux_devkit/arm-none-linux-gnueabi/usr/include/libxml2/ 
LDFLAGS += -g -L/usr/lib  -lxml2 -lz
CFLAGS += -I /usr/include/libxml2/ 





CC = $(BUILD_TOOL_PREFIX)gcc 


SOURCES = data_process.c data_update.c server_api.c net_config.c charset.c xml_op.c


OBJS =$(SOURCES:.c=.o)

all:data_update.cgi fsupdate


data_update.cgi:	$(OBJS) $(LIBS)
	$(CC)  -o $@ $^ $(LDFLAGS) $(CFLAGS)
	
clean:
	rm -f $(OBJS) data_update.cgi

fsupdate:
	@echo "--------------------------------------------------------------"
	@echo "|           !!!!!!!!!!!!!!<file update sucess>!!!!!!!!!!!!!  |"
	@echo "--------------------------------------------------------------"
	
	cp ./*.cgi   /home/nvidia/var/www/cgi-bin/

