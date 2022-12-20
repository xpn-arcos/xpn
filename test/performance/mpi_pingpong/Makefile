#---------------------- ping' MAKEFILE -------------------- 
 

#--------------------------
# Sistema base
#-------------------------- 

CC             = /opt/mpich/bin/mpicc
LIB_PATH       = -L/usr/lib
INCLUDE_PATH   = -I./
SOURCE_PATH    = .
CFLAGS         = -Wall -O2
LIBLINK        = -lpthread



#--------------------------
# Objetos
#-------------------------- 

OBJETOS   = \
            $(SOURCE_PATH)/ping.o


 
#--------------------------
# Reglas de compilacion ...
#-------------------------- 

all:    init    ping
refast: clean   ping
fast:   ping

ping: $(OBJETOS)
	@echo "   Building ping :"
	@echo "     ping ..."
	@$(CC)  $(CFLAGS) -o ping  $(OBJETOS) $(LIBLINK)


init:
	@echo "" 
	@echo "    ping  " 
	@echo "   ------"
	@echo "" 
	@echo "   Options:"
	@echo "     "$(CFLAGS)
	@echo "   Directory:"
	@echo "     "$(INCLUDE_PATH)
	@echo "   Compiling:"

%.o: %.c
	@echo "     $< ..." 
	@$(CC)  $(CFLAGS) $(INCLUDE_PATH) -c $< -o $@ 


clean: 
	@echo "   Removing trash..."
	@rm -fr $(OBJETOS) ./ping
	@echo "   Done!"



# ---------------------- ping' MAKEFILE -------------------- 

