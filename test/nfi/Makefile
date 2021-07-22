
MYHEADER = -I../../src/misc/ -I../../include -I../../src/nfi/include/
LIBRARY_PATH = ../../lib/
SOURCE_PATH  = ./
OBJECTS = $(SOURCE_PATH)/interpreter.o
LIBRARIES = -lpthread -lnfi_nfs
MYFLAGS = -O2 -Wall -Wextra -D_REENTRANT -DPOSIX_THREADS -DLINUX


all:  interpreter.o interpreter

interpreter.o:
	$(CC)  -c $(CFLAGS) $(DEBUG_FLAGS) $(MYFLAGS) $(MYHEADER) interpreter.c

interpreter:
	$(CC)  -o interpreter $(OBJECTS) -L$(LIBRARY_PATH) $(LIBRARIES)
	
clean:
	rm -f $(SOURCE_PATH)*.o
	rm -f $(SOURCE_PATH)/interpreter 
