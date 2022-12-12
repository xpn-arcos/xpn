EXPAND_DIR=../../..

MYFLAGS      = -g  -Wall -Wextra -static -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -DBUILD_LITE -DHAVE_STRUCT_IOVEC
MYHEADERS    =  -I$(EXPAND_DIR)/include 
MYLIBS       =  -L$(EXPAND_DIR)/lib 

EXECS=bmp_est

all: $(EXECS)

Bitmap.o: Bitmap.c Bitmap.h
	gcc $(MYFLAGS) $(MYHEADERS) -c $<

bmp_est.o: bmp_est.c Bitmap.o
	gcc $(MYFLAGS) $(MYHEADERS) -c $<

bmp_est: bmp_est.o Bitmap.o $(EXPAND_DIR)/lib/libxpn.a
	gcc -o $@ $^ -lpthread $(MYLIBS)

clean:
	rm -f *.o
	rm -f bmp_est
