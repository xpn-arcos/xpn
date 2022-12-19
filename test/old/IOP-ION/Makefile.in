

 MPICC=mpicc
 EXPAND_DIR=../..
 CFLAGS=-DMPI -I$(EXPAND_DIR)/include/
#CFLAGS=-DXPN -I$(EXPAND_DIR)/include/

OBJECT=IOPMPI IONMPI

all: $(OBJECT)

%: %.c
	$(MPICC) $(CFLAGS)   -L../../../xpn/lib -L../../../mxml  -lxpn -lmxml -ldl -lpthread  -o $@ $<

clean:
	rm -f *.o $(OBJECT)

