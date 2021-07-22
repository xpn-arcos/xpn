# Usar mpich-1.2.4 sin el parche de xpn.
# Ejecutar con:
# 	mpirun -np 2 -machinefile machines ./IONMPI /PNFS/

OBJECT=IOPMPI IONMPI

FLAGS=-DXPN $(MYHEADERS) $(MYLIBS)
#FLAGS=-DMPI

#EXPAND_DIR=../..
EXPAND_DIR=$(HOME)/expand-2.0

MYHEADERS=-I$(EXPAND_DIR)/include/ -I/export/home/pato11-1/globus-4.0.1/include/gcc32dbg/

MYLIBS= -L$(EXPAND_DIR)/lib/ -lxpn_gcc -L/export/home/pato11-1/globus-4.0.1/lib/ -lglobus_xio_gcc32dbg -lpthread

all: $(OBJECT)

%: %.c
	mpicc -cc=gcc $(FLAGS) -o $@ $<

clean:
	rm -f *.o $(OBJECT)

