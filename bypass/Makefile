#
# This Makefile is for expand bypass (syscalls interceptions) library
#

#gcc -Wall -Wextra -fpic -DLINUX -I/opt/xpn/include/ -c xpn_bypass.c
#gcc -shared -o xpn_bypass.so *.o -Wl,--trace /opt/xpn/lib/libxpn.a -lmxml -lpthread


CC=/opt/software/install-mpich/bin/mpicc
#CC=/home/dcamarma/mpich-install/bin/mpicc
FLAGS=-Wall -Wextra -fpic -DLINUX
#INCLUDE=-I/home/dcamarma/xpn-install/include/
INCLUDE=-I/opt/xpn/include/
#XPN=/home/dcamarma/xpn-install/lib/libxpn.a
XPN=/opt/xpn/lib/libxpn.a
LIBS= -lmxml -lpthread -ldl
OBJ= xpn_bypass.o

all:
	$(CC) $(FLAGS) $(INCLUDE) -c xpn_bypass.c
	$(CC) -shared -o xpn_bypass.so $(OBJ) $(XPN) $(LIBS)

clean:
	rm -f *.o
	rm -f *.so


