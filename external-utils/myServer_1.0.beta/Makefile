
#--------------------------
# Variables
#--------------------------

MAKE         = make -s
CC           = gcc
SOURCE_PATH  = ./
#EXPAND_DIR   = $(HOME)/expand/trunk/expand
EXPAND_DIR   = /opt/xpn
OBJECTS_MYSERVER      = myServer.o \
		       myServer_comm.o \
		       myServer_ops.o \
		       myServer_workers.o \
		       myServer_d2xpn.o
#OBJECTS_XPN2D         = xpn2d_lock.o
#OBJECTS_D2XPN         = d2xpn_lock.o
#OBJECTS_RMXPN         = rmxpn.o
#OBJECTS_XPN2XPN         = xpn2xpn_lock.o
#MYFLAGS      = -g  -Wall -Wextra -static -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC -D_MALLOC_ -DDBG_OPS -DDBG_WORKERS -DDBG_COMM
#MYFLAGS      = -g  -Wall -Wextra -static -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC -D_MALLOC_ -DDBG_OPS -DDBG_WORKERS 
MYFLAGS      = -g  -Wall -Wextra -static -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC  -D_MALLOC_
MYHEADERS    =  -I$(EXPAND_DIR)/include 
MYLIBS       =  -L$(EXPAND_DIR)/lib  -L${HOME}/mxml/lib  -lpthread -lxpn -lmxml


MYSERVER_EXE = myServer.exe
#D2XPN_EXE    = d2xpn_lock.exe
#XPN2D_EXE    = xpn2d_lock.exe
#XPN2XPN_EXE    = xpn2xpn_lock.exe
#RMXPN_EXE          = rmxpn.exe

#D2XPN_EXE    = 
#XPN2D_EXE    = 
#XPN2XPN_EXE    = 
#--------------------------
# Compilation rules
#--------------------------


all:  all_object  all_exe 

#all_object:  $(OBJECTS_MYSERVER) $(OBJECTS_D2XPN) $(OBJECTS_XPN2D) $(OBJECTS_RMXPN)
all_object:  $(OBJECTS_MYSERVER)

#all_exe:  $(MYSERVER_EXE) $(D2XPN_EXE) $(XPN2D_EXE) $(XPN2XPN_EXE) $(RMXPN_EXE)
all_exe:  $(MYSERVER_EXE)

%.o: %.c
	@echo -n "                    "
	@echo -n "$< [.." 
	@$(CC) $(MYFLAGS) $(MYHEADERS) -c $< -o $@ 
	@echo "..]" 

$(MYSERVER_EXE): $(OBJECTS_MYSERVER)
	@echo -n "                    "
	@echo -n "$@ [.." 
	@$(CC) -o $(MYSERVER_EXE)  \
             $(OBJECTS_MYSERVER) \
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
	rm -f $(OBJECTS_MYSERVER) $(OBJECTS_D2XPN) $(OBJECTS_XPN2D) $(OBJECTS_RMXPN)
	rm -f $(MYSERVER_EXE) $(D2XPN_EXE) $(XPN2D_EXE) $(RMXPN_EXE)


