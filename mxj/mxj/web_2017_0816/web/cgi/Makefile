#SUBDIRS=$(ls -l | grep ^d | awk '{print $$9}')
SUBDIRS=data_update boot_args jc_sptp_upload sptp_upload

.PHONY:target 
all:target

target:$(SUBDIRS)

$(SUBDIRS): ECHO
	echo $@
	make -C $@ clean
	@echo "********************************************************************"
	make -C $@ 
	@echo "#####################################################################"

ECHO:
	@echo $(SUBDIRS)

