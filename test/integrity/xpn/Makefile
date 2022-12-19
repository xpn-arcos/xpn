
#
# Definitions
#

 MAKE         = make -s
 CC           = /opt/mpich/bin/mpicc
 MYHEADER     = -I../../../include/ -I../../../include/base -I../../../include/xpn_client/
 MYLIBPATH    = -L../../../src/base -L../../../include/xpn_client
 SOURCE_PATH  = ./
 OBJECTS      = $(SOURCE_PATH)/xpn_simple.o
 LIBRARIES    = -lpthread -lxpn_client
 MYFLAGS      = -O2 -Wall -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H


#
# Rules
#

all:  xpn_simple.o xpn_simple

xpn_simple.o:
	$(CC)  -c $(CFLAGS) $(DEBUG_FLAGS) $(MYFLAGS) $(MYHEADER) xpn_simple.c

xpn_simple:
	$(CC)  -o xpn_simple $(OBJECTS) $(MYLIBPATH) $(LIBRARIES)
	
clean:
	rm -f $(SOURCE_PATH)/*.o
	rm -f $(SOURCE_PATH)/xpn_simple 

