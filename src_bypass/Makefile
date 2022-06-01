#
# This Makefile is for expand bypass (syscalls interceptions) library
#

#gcc -Wall -Wextra -fpic -DLINUX -I/opt/xpn/include/ -c xpn_bypass.c
#gcc -shared -o xpn_bypass.so *.o -Wl,--trace /opt/xpn/lib/libxpn.a -lmxml -lpthread


 prefix = /home/dcamarma/xpn-install
 CC     = /home/dcamarma/mpich-install/bin/mpicc
#CC=/opt/software/install-mpich/bin/mpicc
#CC=/home/dcamarma/mpich-install/bin/mpicc
 FLAGS=-Wall -Wextra -fpic -DLINUX
 INCLUDE=-I../include -I../include/base -DNOT_TO_USE_FCNTL_H -DNOT_TO_USE_STDLIB_H
 LIBS=-lmxml -lpthread -ldl
 XPN_LIB=${prefix}/lib

all:
	$(CC) $(FLAGS) $(INCLUDE) -c xpn_bypass.c
	$(CC) -shared -o xpn_bypass.so xpn_bypass.o $(XPN_LIB)/libxpn.a $(LIBS)

install:
	cp -f xpn_bypass.o $(XPN_LIB)

clean:
	rm -f *.o
	rm -f *.so

