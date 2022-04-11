
#--------------------------
# Variables
#--------------------------

MAKE         = make -s
#CC           = /opt/software/install-mpich/bin/mpicc
CC           = mpicc
#CC           = /home/dcamarma/mpich-install/bin/mpicc
SOURCE_PATH  = ./
#EXPAND_DIR   = $(HOME)/expand/trunk/expand
EXPAND_DIR   = /opt/xpn
#EXPAND_DIR   = /home/dcamarma/xpn-install
OBJECTS_MPISERVER      = mpiServer.o \
		         mpiServer_params.o \
		         mpiServer_comm.o \
		         mpiServer_ops.o \
		         mpiServer_workers.o \
		         mpiServer_workers_ondemand.o \
		         mpiServer_workers_pool.o \
		         mpiServer_d2xpn.o \
			 base/filesystem.o \
			 base/urlstr.o \
			 base/utils.o
#MYFLAGS               = -g  -Wall -Wextra -static -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC -D_MALLOC_ -DDEBUG
#MYFLAGS               = -g  -Wall -Wextra -static -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC -D_MALLOC_ -DDEBUG
MYFLAGS                = -g  -Wall -Wextra -static -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC  -D_MALLOC_
MYHEADERS              =  -I. -I$(EXPAND_DIR)/include 
MYLIBS                 =  -L$(EXPAND_DIR)/lib  -L${HOME}/mxml/lib  -lpthread -lxpn -lmxml -ldl


MPISERVER_EXE = mpiServer.exe
#D2XPN_EXE    = 
#XPN2D_EXE    = 
#XPN2XPN_EXE    = 

#--------------------------
# Compilation rules
#--------------------------


all:  all_object  all_exe 

#all_object:  $(OBJECTS_MPISERVER) $(OBJECTS_D2XPN) $(OBJECTS_XPN2D) $(OBJECTS_RMXPN)
all_object:  $(OBJECTS_MPISERVER)

#all_exe:  $(MPISERVER_EXE) $(D2XPN_EXE) $(XPN2D_EXE) $(XPN2XPN_EXE) $(RMXPN_EXE)
all_exe:  $(MPISERVER_EXE)

%.o: %.c
	@echo -n "                    "
	@echo -n "$< [.." 
	@$(CC) $(MYFLAGS) $(MYHEADERS) -c $< -o $@ 
	@echo "..]" 

$(MPISERVER_EXE): $(OBJECTS_MPISERVER)
	@echo -n "                    "
	@echo -n "$@ [.." 
	@$(CC) -o $(MPISERVER_EXE)  \
             $(OBJECTS_MPISERVER) \
		$(MYLIBS) > /dev/null
	@echo "..]" 
$(XPN2D_EXE): $(OBJECTS_XPN2D)
	@echo -n "                    "
	@echo -n "$@ [.." 
	@$(CC) -o $(XPN2D_EXE)  \
             $(OBJECTS_XPN2D) \
		$(MYLIBS) > /dev/null
	@echo "..]" 
$(D2XPN_EXE): $(OBJECTS_D2XPN)
	@echo -n "                    "
	@echo -n "$@ [.." 
	@$(CC) -o  $(D2XPN_EXE) \
             $(OBJECTS_D2XPN) \
		$(MYLIBS) > /dev/null
	@echo "..]" 
$(XPN2XPN_EXE): $(OBJECTS_XPN2XPN)
	@echo -n "                    "
	@echo -n "$@ [.." 
	@$(CC) -o  $(XPN2XPN_EXE) \
             $(OBJECTS_XPN2XPN) \
		$(MYLIBS) > /dev/null
	@echo "..]" 
$(RMXPN_EXE): $(OBJECTS_RMXPN)
	@echo -n "                    "
	@echo -n "$@ [.." 
	@$(CC) -o  $(RMXPN_EXE) \
             $(OBJECTS_RMXPN) \
		$(MYLIBS) > /dev/null
	@echo "..]" 


clean:
	rm -f $(OBJECTS_MPISERVER) $(OBJECTS_D2XPN) $(OBJECTS_XPN2D) $(OBJECTS_RMXPN)
	rm -f $(MPISERVER_EXE) $(D2XPN_EXE) $(XPN2D_EXE) $(RMXPN_EXE)
