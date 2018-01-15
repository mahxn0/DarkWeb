#SUBDIRS=$(ls -l | grep ^d | awk '{print $$9}')
SUBDIRS=web_2017_0816/web/cgi nopoll/nopoll-0.4.5.b375/test roll/ boa-0.94.13/src 

.PHONY:target 
#all:target clean 

target:$(SUBDIRS)

$(SUBDIRS): ECHO
	#echo $@
	#make -C $@ clean
	#@echo "********************************************************************"
	make -C $@ 
	@echo "#####################################################################"

ECHO:
	@echo $(SUBDIRS)

UPDATE:
	cp boa-0.94.13/src/boa /home/ubuntu/var/server -rf
	cp boa.conf /etc/server/server.conf -rf
	cp nopoll/nopoll-0.4.5.b375/test/nopoll-regression-listener /home/ubuntu/var/ -rf
