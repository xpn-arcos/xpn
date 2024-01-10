
#--------------------------
# Variables
#--------------------------

 MAKE                 = make -s
 CC                   = /usr/bin/mpicc
 SOURCE_PATH          = ./
 EXPAND_DIR           = /home/acaldero/bin/xpn
 OBJECTS_TCP_SERVER   = ../../src/tcp_server/tcp_server.o \
			../../src/tcp_server/tcp_server_params.o \
			../../src/tcp_server/tcp_server_comm.o \
			../../src/tcp_server/tcp_server_ops.o \
			../../src/tcp_server/tcp_server_d2xpn.o \
			../../src/base/filesystem.o \
			../../src/base/urlstr.o \
			../../src/base/string_misc.o \
			../../src/base/debug_msg.o \
			../../src/base/utils.o \
			../../src/base/time_misc.o \
			../../src/base/ns.o \
			../../src/base/workers.o \
			../../src/base/workers_ondemand.o \
			../../src/base/workers_pool.o
MYFLAGS              = -g  -Wall -Wextra -static -std=c11 -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC  -D_MALLOC_
MYHEADERS            = -I. -I$(EXPAND_DIR)/include -I../../include -I../../include/base -I../../include/xpn_client -I../../include/tcp_server
MYLIBS               =  -L../../src/xpn_client -lxpn  -L../../mxml        -L../../bin/mosquitto/lib        -L../../xpn/lib -L../../../mxml     -L../../../bin/mosquitto/lib     -L../../../xpn/lib -L../../../../mxml  -L../../../../bin/mosquitto/lib  -L../../../../xpn/lib  -lmosquitto -lmxml -ldl -lpthread  -lmxml

TCP_SERVER_EXE       = xpn_tcp_server
TCP_SERVER_EXE_STOP  = xpn_stop_tcp_server


#--------------------------
# Compilation rules
#--------------------------


all:  all_object  all_exe

all_object:  $(OBJECTS_TCP_SERVER)

all_exe:  $(TCP_SERVER_EXE) $(TCP_SERVER_EXE_STOP)

%.o: %.c
	$(CC) $(MYFLAGS) $(MYHEADERS) -c $< -o $@

$(TCP_SERVER_EXE): $(OBJECTS_TCP_SERVER)
	$(CC) -o $(TCP_SERVER_EXE)  $(OBJECTS_TCP_SERVER)  $(MYLIBS)

$(TCP_SERVER_EXE_STOP): $(TCP_SERVER_EXE)
	ln -s $(TCP_SERVER_EXE) $(TCP_SERVER_EXE_STOP)

install:  $(TCP_SERVER_EXE)
	cp -a                   $(TCP_SERVER_EXE) $(EXPAND_DIR)/bin
	rm -fr $(EXPAND_DIR)/bin/$(TCP_SERVER_EXE_STOP) && ln -s $(EXPAND_DIR)/bin/$(TCP_SERVER_EXE) $(EXPAND_DIR)/bin/$(TCP_SERVER_EXE_STOP)


clean:
	rm -f $(OBJECTS_TCP_SERVER)
	rm -f $(TCP_SERVER_EXE)
	rm -f $(TCP_SERVER_EXE_STOP)

