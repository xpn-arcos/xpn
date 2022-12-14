#
# This Makefile is for expand bypass (syscalls interceptions) library
#


 prefix = /home/uc3m15/uc3m15971/mn4/bin//xpn
 CC=/gpfs/apps/MN4/INTEL/2017.4/compilers_and_libraries_2017.4.196/linux/mpi/intel64/bin/mpicc

 FLAGS=-Wall -Wextra -fpic -DLINUX -std=c11
 INCLUDE=-I../../include -I../../include/base -I../../include/xpn_client -I../../../mxml -DNOT_TO_USE_FCNTL_H -DNOT_TO_USE_STDLIB_H
 LIBS=-L../../../xpn/src/xpn_client -L../../../mxml -lxpn -lmxml -lpthread -ldl

all:
	$(CC) $(FLAGS) $(INCLUDE) -c xpn_bypass.c
	$(CC) -shared -o xpn_bypass.so xpn_bypass.o $(LIBS)

install:
	cp -f xpn_bypass.so ${prefix}/lib

clean:
	rm -f *.o
	rm -f *.so

