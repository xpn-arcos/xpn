
#--------------------------
# Variables
#--------------------------

 MAKE         = make -s
 CC           = /home/dcamarma/mpich-install/bin/mpicc
 EXPAND_DIR   = /home/dcamarma/xpn-install
 OBJECTS_TCPSERVER  = tcpServer.o \
		      tcpServer_utils.o \
		      tcpServer_params.o \
		      tcpServer_comm.o \
		      tcpServer_ops.o \
		      tcpServer_workers.o \
		      tcpServer_d2xpn.o
#OBJECTS_XPN2D         = xpn2d_lock.o
#OBJECTS_D2XPN         = d2xpn_lock.o
#OBJECTS_XPN2XPN       = xpn2xpn_lock.o
#MYFLAGS               = -g  -Wall -Wextra -static -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC -D_MALLOC_ -DDBG_OPS -DDBG_WORKERS -DDBG_COMM
#MYFLAGS               = -g  -Wall -Wextra -static -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC -D_MALLOC_ -DDBG_OPS -DDBG_WORKERS
MYFLAGS                = -g  -Wall -Wextra -static -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC  -D_MALLOC_
MYHEADERS              =  -I$(EXPAND_DIR)/include
MYLIBS                 =  -L$(EXPAND_DIR)/lib  -L${HOME}/mxml/lib  -lpthread -lxpn -lmxml -ldl


TCPSERVER_EXE = tcpServer.exe
#D2XPN_EXE    = d2xpn_lock.exe
#XPN2D_EXE    = xpn2d_lock.exe
#XPN2XPN_EXE  = xpn2xpn_lock.exe

#D2XPN_EXE    =
#XPN2D_EXE    =
#XPN2XPN_EXE    =
#--------------------------
# Compilation rules
#--------------------------


all:  all_object  all_exe

#all_object:  $(OBJECTS_TCPSERVER) $(OBJECTS_D2XPN) $(OBJECTS_XPN2D) $(OBJECTS_RMXPN)
all_object:  $(OBJECTS_TCPSERVER)

#all_exe:  $(TCPSERVER_EXE) $(D2XPN_EXE) $(XPN2D_EXE) $(XPN2XPN_EXE) $(RMXPN_EXE)
all_exe:  $(TCPSERVER_EXE)

install: $(TCPSERVER_EXE)
	@cp -a $(TCPSERVER_EXE) $(EXPAND_DIR)/bin

%.o: %.c
	$(CC) $(MYFLAGS) $(MYHEADERS) -c $< -o $@

$(TCPSERVER_EXE): $(OBJECTS_TCPSERVER)
	$(CC) -o $(TCPSERVER_EXE)  $(OBJECTS_TCPSERVER)  $(MYLIBS) 

$(XPN2D_EXE): $(OBJECTS_XPN2D)
	@$(CC) -o $(XPN2D_EXE)  $(OBJECTS_XPN2D) $(MYLIBS) 
$(D2XPN_EXE): $(OBJECTS_D2XPN)
	@$(CC) -o  $(D2XPN_EXE) $(OBJECTS_D2XPN) $(MYLIBS) 
$(XPN2XPN_EXE): $(OBJECTS_XPN2XPN)
	@$(CC) -o  $(XPN2XPN_EXE) $(OBJECTS_XPN2XPN)  $(MYLIBS) 
$(RMXPN_EXE): $(OBJECTS_RMXPN)
	@$(CC) -o  $(RMXPN_EXE) $(OBJECTS_RMXPN)  $(MYLIBS) 

clean:
	rm -f $(OBJECTS_TCPSERVER) $(OBJECTS_D2XPN) $(OBJECTS_XPN2D) $(OBJECTS_RMXPN)
	rm -f $(TCPSERVER_EXE) $(D2XPN_EXE) $(XPN2D_EXE) $(RMXPN_EXE)

