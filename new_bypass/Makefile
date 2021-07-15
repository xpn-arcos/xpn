#
# This Makefile is for expand bypass (syscalls interceptions) library
#

#gcc -Wall -Wextra -fpic -DLINUX -I/opt/xpn/include/ -c xpn_bypass.c
#gcc -shared -lmxml -lpthread -o xpn_bypass.so *.o -Wl,--trace /opt/xpn/lib/libxpn.a


#gcc -shared -o xpn_bypass.so *.o -Wl,--trace /opt/xpn/lib/libxpn.a -lmxml -lpthread




#gcc -g -Wall -Wextra -DLINUX -I/opt/xpn/include/ xpn_bypass.c -L/opt/xpn/lib/ -lxpn  -lmxml -lpthread -o bypass





CC=gcc
FLAGS=-g -Wall -Werror
OBJ= queue costCalculator
LIBS= -pthread

all:  $(OBJ)
	@echo "***************************"
	@echo "Compilation successfully!"
	@echo ""

queue: queue.c
	$(CC) -c queue.c

costCalculator:	costCalculator.c
	$(CC) $(CFLAGS) $(LIBS) -o calculator  costCalculator.c queue.c

load:
	ld -o calculator queue.o

clean:
	rm -f calculator process *.o
	@echo "***************************"
	@echo "Deleted files!"
	@echo  ""

