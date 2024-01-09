#
# This Makefile is for expand bypass (syscalls interceptions) library
#


 prefix = /local_test/bin//xpn
 CC=/local_test/bin/mpich/bin/mpicc

 FLAGS=-Wall -Wextra -fpic -DLINUX -std=c11
 INCLUDE=-I../../include -I../../include/base -I../../include/xpn_client -I../../include/bypass -I../../../mxml -DNOT_TO_USE_FCNTL_H -DNOT_TO_USE_STDLIB_H
 LIBS=-L../../../xpn/src/xpn_client -L../../src/xpn_client -L../../../mxml -lxpn -lmxml -lpthread -ldl

all:
	$(CC) $(FLAGS) $(INCLUDE) -c xpn_bypass.c
	$(CC) -shared -o xpn_bypass.so xpn_bypass.o $(LIBS)

install:
	cp -f xpn_bypass.so ${prefix}/lib

clean:
	rm -f *.o
	rm -f *.so

