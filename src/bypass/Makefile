#
# This Makefile is for expand bypass (syscalls interceptions) library
#


 prefix = /home/cristhma/bin//xpn
 CC=/usr/bin/mpicc

 FLAGS=-Wall -Wextra -fpic -DLINUX -std=c11
 INCLUDE=-I../../include -I../../include/base -I../../include/xpn_client -I../../include/bypass -I../../../mxml -DNOT_TO_USE_FCNTL_H -DNOT_TO_USE_STDLIB_H -I../../mxml        -I../../bin/mosquitto/include       -I../../../mxml    -I../../../bin/mosquitto/include    -I../../../xpn/include    -I../../../xpn/include/xpn_client -I../../../xpn/include/base -I../../../../mxml -I../../../../bin/mosquitto/include -I../../../../xpn/include -I../../../../xpn/include/xpn_client -I../../../../xpn/include/base  -fPIC -O2 -g -g3 -ggdb -Wall -Wextra -std=c11
 LIBS=-L../../../xpn/src/xpn_client -L../../src/xpn_client -L../../../mxml -lxpn -lmxml -lpthread -ldl

all:
	$(CC) $(FLAGS) $(INCLUDE) -c xpn_bypass.c
	$(CC) -shared -o xpn_bypass.so xpn_bypass.o $(LIBS)

install:
	cp -f xpn_bypass.so ${prefix}/lib

clean:
	rm -f *.o
	rm -f *.so

