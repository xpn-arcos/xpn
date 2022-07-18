#
# This Makefile is for expand bypass (syscalls interceptions) library
#


 prefix = /opt/xpn/xpn
 CC=/opt/software/install-mpich//bin/mpicc

 FLAGS=-Wall -Wextra -fpic -DLINUX
 INCLUDE=-I../include -I../include/base -I../../mxml -DNOT_TO_USE_FCNTL_H -DNOT_TO_USE_STDLIB_H
 LIBS=-lxpn -lmxml -lpthread -ldl

 XPN_LIB=${prefix}/lib
 MXML_LIB=../../mxml


all:
	$(CC) $(FLAGS) $(INCLUDE) -c xpn_bypass.c
	$(CC) -shared -o xpn_bypass.so xpn_bypass.o -L$(XPN_LIB) -L$(MXML_LIB) $(LIBS)

install:
	cp -f xpn_bypass.o $(XPN_LIB)

clean:
	rm -f *.o
	rm -f *.so

