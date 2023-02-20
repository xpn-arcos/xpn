
#--------------------------
# Variables
#--------------------------

 MAKE                 = make -s
 CC                   = /usr/bin/mpicc
 SOURCE_PATH          = ./
 EXPAND_DIR           = /home/alejandro/bin//xpn
 OBJECTS_MPI_SERVER   = ../../src/mpi_server/mpi_server.o \
			../../src/mpi_server/mpi_server_params.o \
			../../src/mpi_server/mpi_server_comm.o \
			../../src/mpi_server/mpi_server_ops.o \
			../../src/mpi_server/mpi_server_d2xpn.o \
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
MYFLAGS              = -g  -Wall -Wextra -static -std=c11 -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -DBUILD_LITE -DHAVE_STRUCT_IOVEC  -D_MALLOC_
MYHEADERS            = -I. -I$(EXPAND_DIR)/include -I../../include -I../../include/base -I../../include/xpn_client -I../../include/mpi_server
MYLIBS               = -L../../src/xpn_client -L../../../mxml -lxpn -lmxml -ldl -lpthread

MPI_SERVER_EXE       = xpn_mpi_server
MPI_SERVER_EXE_STOP  = xpn_stop_mpi_server


#--------------------------
# Compilation rules
#--------------------------


all:  all_object  all_exe

all_object:  $(OBJECTS_MPI_SERVER)

all_exe:  $(MPI_SERVER_EXE) $(MPI_SERVER_EXE_STOP)

%.o: %.c
	$(CC) $(MYFLAGS) $(MYHEADERS) -c $< -o $@

$(MPI_SERVER_EXE): $(OBJECTS_MPI_SERVER)
	$(CC) -o $(MPI_SERVER_EXE)  $(OBJECTS_MPI_SERVER)  $(MYLIBS)

$(MPI_SERVER_EXE_STOP): $(MPI_SERVER_EXE)
	ln -s $(MPI_SERVER_EXE) $(MPI_SERVER_EXE_STOP)

install:  $(MPI_SERVER_EXE)
	cp -a                   $(MPI_SERVER_EXE) $(EXPAND_DIR)/bin
	rm -fr $(EXPAND_DIR)/bin/$(MPI_SERVER_EXE_STOP)
	ln -s $(EXPAND_DIR)/bin/$(MPI_SERVER_EXE) $(EXPAND_DIR)/bin/$(MPI_SERVER_EXE_STOP)


clean:
	rm -f $(OBJECTS_MPI_SERVER)
	rm -f $(MPI_SERVER_EXE)
	rm -f $(MPI_SERVER_EXE_STOP)

