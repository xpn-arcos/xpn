
#--------------------------
# Variables
#--------------------------

 MAKE         = make -s
 CC           = /usr/bin/mpicc
 EXPAND_DIR   = /root/bin//xpn
 OBJECTS_TCP_SERVER  = tcp_server.o \
		       tcp_server_utils.o \
		       tcp_server_params.o \
		       tcp_server_comm.o \
		       tcp_server_ops.o \
		       tcp_server_workers.o \
		       tcp_server_d2xpn.o
#OBJECTS_XPN2D         = xpn2d_lock.o
#OBJECTS_D2XPN         = d2xpn_lock.o
#OBJECTS_XPN2XPN       = xpn2xpn_lock.o
#MYFLAGS               = -g  -Wall -Wextra -static -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC -D_MALLOC_ -DDBG_OPS -DDBG_WORKERS -DDBG_COMM
#MYFLAGS               = -g  -Wall -Wextra -static -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC -D_MALLOC_ -DDBG_OPS -DDBG_WORKERS
MYFLAGS                = -g  -Wall -Wextra -static -std=c11 -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC  -D_MALLOC_
MYHEADERS              = -I. -I$(EXPAND_DIR)/include -I../../include -I../../include/base -I../../include/xpn_client -I../../include/tcp_server
MYLIBS                 =  -L../../src/xpn_client -L../../../mxml -lpthread -lxpn -lmxml -ldl

TCP_SERVER_EXE      = tcp_server
TCP_SERVER_EXE_STOP = xpn_stop_tcp_server
#D2XPN_EXE          = d2xpn_lock
#XPN2D_EXE          = xpn2d_lock
#XPN2XPN_EXE        = xpn2xpn_lock


#--------------------------
# Compilation rules
#--------------------------


all:  all_object  all_exe

#all_object:  $(OBJECTS_TCP_SERVER) $(OBJECTS_D2XPN) $(OBJECTS_XPN2D) $(OBJECTS_RMXPN)
all_object:  $(OBJECTS_TCP_SERVER)

#all_exe:  $(TCP_SERVER_EXE) $(D2XPN_EXE) $(XPN2D_EXE) $(XPN2XPN_EXE) $(RMXPN_EXE)
all_exe:  $(TCP_SERVER_EXE)

install: $(TCP_SERVER_EXE)
	cp -a                   $(TCP_SERVER_EXE) $(EXPAND_DIR)/bin
	rm -fr $(EXPAND_DIR)/bin/$(TCP_SERVER_EXE_STOP)
	ln -s $(EXPAND_DIR)/bin/$(TCP_SERVER_EXE) $(EXPAND_DIR)/bin/$(TCP_SERVER_EXE_STOP)

%.o: %.c
	$(CC) $(MYFLAGS) $(MYHEADERS) -c $< -o $@

$(TCP_SERVER_EXE): $(OBJECTS_TCP_SERVER)
	$(CC) -o $(TCP_SERVER_EXE)  $(OBJECTS_TCP_SERVER)  $(MYLIBS) 

$(XPN2D_EXE): $(OBJECTS_XPN2D)
	@$(CC) -o $(XPN2D_EXE)  $(OBJECTS_XPN2D) $(MYLIBS) 
$(D2XPN_EXE): $(OBJECTS_D2XPN)
	@$(CC) -o  $(D2XPN_EXE) $(OBJECTS_D2XPN) $(MYLIBS) 
$(XPN2XPN_EXE): $(OBJECTS_XPN2XPN)
	@$(CC) -o  $(XPN2XPN_EXE) $(OBJECTS_XPN2XPN)  $(MYLIBS) 
$(RMXPN_EXE): $(OBJECTS_RMXPN)
	@$(CC) -o  $(RMXPN_EXE) $(OBJECTS_RMXPN)  $(MYLIBS) 

clean:
	rm -f $(OBJECTS_TCP_SERVER) $(OBJECTS_D2XPN) $(OBJECTS_XPN2D) $(OBJECTS_RMXPN)
	rm -f $(TCP_SERVER_EXE) $(D2XPN_EXE) $(XPN2D_EXE) $(RMXPN_EXE)

