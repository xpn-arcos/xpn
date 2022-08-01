
#--------------------------
# Variables
#--------------------------

 MAKE         = make -s
 CC           = /home/dcamarma/mpich-install//bin/mpicc
 SOURCE_PATH  = ./
 EXPAND_DIR   = /home/dcamarma/bin//xpn
 OBJECTS_MPISERVER     = mpiServer.o \
		         mpiServer_params.o \
		         mpiServer_comm.o \
		         mpiServer_ops.o \
		         mpiServer_workers.o \
		         mpiServer_workers_ondemand.o \
		         mpiServer_workers_pool.o \
		         mpiServer_d2xpn.o \
			 base/filesystem.o \
			 base/urlstr.o \
			 base/string_misc.o \
			 base/debug_msg.o \
			 base/utils.o
#MYFLAGS               = -g  -Wall -Wextra -static -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC -D_MALLOC_ -DDEBUG
MYFLAGS                = -g  -Wall -Wextra -static -std=c11 -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC  -D_MALLOC_
MYHEADERS              =  -I. -I$(EXPAND_DIR)/include -I../../../xpn/include
MYLIBS                 =  -L../../../xpn/lib -L../../../mxml -lpthread -lxpn -lmxml -ldl

MPISERVER_EXE = mpiServer.exe


#--------------------------
# Compilation rules
#--------------------------


all:  all_object  all_exe

all_object:  $(OBJECTS_MPISERVER)

all_exe:  $(MPISERVER_EXE)

install:  $(MPISERVER_EXE)
	@cp -a $(MPISERVER_EXE) $(EXPAND_DIR)/bin

%.o: %.c
	$(CC) $(MYFLAGS) $(MYHEADERS) -c $< -o $@

$(MPISERVER_EXE): $(OBJECTS_MPISERVER)
	$(CC) -o $(MPISERVER_EXE)  $(OBJECTS_MPISERVER)  $(MYLIBS)


clean:
	rm -f $(OBJECTS_MPISERVER)
	rm -f $(MPISERVER_EXE)

