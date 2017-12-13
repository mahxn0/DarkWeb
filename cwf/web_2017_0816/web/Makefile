#SUBDIRS=$(ls -l | grep ^d | awk '{print $$9}')
SUBDIRS=data_update boot_args

.PHONY:target 
#all:target clean

target:$(SUBDIRS)

$(SUBDIRS): ECHO
	echo $@
	make -C $@ clean
	@echo "********************************************************************"
	make -C $@ 
	@echo "#####################################################################"

ECHO:
	@echo $(SUBDIRS)

