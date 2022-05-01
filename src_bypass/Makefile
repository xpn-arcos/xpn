#
# This Makefile is for expand bypass (syscalls interceptions) library
#

#gcc -Wall -Wextra -fpic -DLINUX -I/opt/xpn/include/ -c xpn_bypass.c
#gcc -shared -o xpn_bypass.so *.o -Wl,--trace /opt/xpn/lib/libxpn.a -lmxml -lpthread


 CC=/opt/software/install-mpich/bin/mpicc
#CC=/home/dcamarma/mpich-install/bin/mpicc
FLAGS=-Wall -Wextra -fpic -DLINUX
INCLUDE=-I../include -I../include/base -DNOT_TO_USE_FCNTL_H -DNOT_TO_USE_STDLIB_H
LIBS=-lmxml -lpthread -ldl
OBJ=xpn_bypass.o
XPN_LIB=/opt/xpn/lib
LIBXPN=$(XPN_LIB)/libxpn.a

all:
	$(CC) $(FLAGS) $(INCLUDE) -c xpn_bypass.c
	$(CC) -shared -o xpn_bypass.so $(OBJ) $(LIBXPN) $(LIBS)

install:
	cp -f $(OBJ) $(XPN_LIB)

clean:
	rm -f *.o
	rm -f *.so

