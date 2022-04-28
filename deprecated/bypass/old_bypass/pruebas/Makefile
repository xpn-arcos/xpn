# Generated automatically from Makefile.in by configure.

CC = gcc

INCLUDE_PATH = ../../include

LIBRARYS = -lxpngcc -lpthread -lnfs -ldl

MYHEADERS    = -I../../include
LIBRARY_PATH = -L../../lib
MYLIBS       = -lnfi -lxpn -lpthread
MYFLAGS      = -g -Wall -Wextra -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN

MYBINARYS =     prueba_stat.bin \
		prueba_dir.bin \
		prueba_mkdir.bin \
		prueba_isdir.bin

all:  $(MYBINARYS)

clean:
	rm -f *.o
	rm -f *.bin

%.o: %.c
	$(CC) $(MYFLAGS) $(MYHEADERS) -c $< -o $@

%.bin: %.o
	$(CC) -o $@ $(MYFLAGS) $(LIBRARY_PATH) $< $(MYLIBS)

prueba_lib_xpn:
	$(CC) -o prueba_lib_xpn.bin -DDEBUG_PRUEBA_LIB_XPN prueba.c
	$(CC) -o prueba_fork_lib_xpn.bin -DDEBUG_PRUEBA_LIB_XPN prueba_fork.c
	
prueba_xpn:
	@#$(CC) -o prueba_xpn.o -c $(DEBUG_FLAGS) $(MYFLAGS) -DDEBUG -DDEBUG_PRUEBA_XPN -I$(INCLUDE_PATH) prueba.c
	$(CC) -o prueba_xpn.o -c $(DEBUG_FLAGS) $(MYFLAGS) -DDEBUG_PRUEBA_XPN -I$(INCLUDE_PATH) prueba.c
	$(CC) -o prueba_xpn.bin prueba_xpn.o $(LIBRARY_PATH) $(LIBRARYS)

