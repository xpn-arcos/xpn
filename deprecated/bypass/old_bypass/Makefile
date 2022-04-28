#
# This Makefile is for expand bypass (syscalls interceptions) library
#

#EXPAND_DIR = $(HOME)/expand-2.0
EXPAND_DIR = ..

#INCLUDE_PATH = ../include
INCLUDE_PATH = -I$(EXPAND_DIR)/include -I$(GLOBUS_LOCATION)/include/gcc32dbg/ -I$(HOME)/gsoap-linux-2.7/ -I$(EXPAND_DIR)/src/nfi/nfi_gsoap/IOService/

#LIBRARY_PATH = ../lib
LIBRARY_PATH = $(EXPAND_DIR)/lib

BYPASS_DIR=$(HOME)/bypass-2_5_6

MYFLAGS = -O2 -Wall -Wextra -fPIC -fno-exceptions -DHAVE_CONFIG_H -DLINUX -DUSE_PTHREADS -g -DDEBUG_READDIR -D__USE_LARGEFILE64
#MYFLAGS = -O2 -Wall -Wextra -fPIC -fno-exceptions -DHAVE_CONFIG_H -DLINUX -DUSE_PTHREADS -g -DDEBUG_BYPASS

#OLD_LIBRARYS = -Xlinker -Bdynamic -lnsl -ldl -lpthread -lnfs -lxpn -shared -g
LIBRARYS = -shared -g -ldl -rdynamic -L$(GLOBUS_LOCATION)/lib -lglobus_xio_gcc32dbg -lpthread

#OBJECT_FILES =  $(EXPAND_DIR)/src/base/*.o \
#		$(EXPAND_DIR)/src/nfi/common/*.o \
#		$(EXPAND_DIR)/src/nfi/nfi_local/*.o \
#		$(EXPAND_DIR)/src/nfi/nfi_nfs/*.o \
#		$(EXPAND_DIR)/src/xpn/xpncore/*.o \
#		$(EXPAND_DIR)/src/xpn/policy/*.o

OBJECT_FILES =  $(LIBRARY_PATH)/libbase_g++.a $(LIBRARY_PATH)/libnfi_g++.a $(LIBRARY_PATH)/libxpn_g++.a

#CC = g++

.PHONY: all clean clean_agent clean_bypass

all: xpn_agent.so #xpn_intercept.c

clean: clean_agent clean_bypass clean_intercept

clean_agent: clean_bypass
	rm -f xpn_agent.C
	rm -f xpn.h

clean_bypass:
	rm -f xpn_agent.o
	rm -f xpn_agent.so

clean_intercept:
	rm -f xpn_intercept.c
	rm -f xpn_intercept.o
	rm -f xpn_intercept.so

xpn_agent.C xpn.h: xpn.bypass xpn_bypass.h
	BYPASS_LIBRARY_DIR=$(BYPASS_DIR)/lib $(BYPASS_DIR)/bin/bypass -agent xpn.bypass
	@#vim -s comentar_todo.vim xpn_agent.C
	vim -s comentar___fxstat64.vim xpn_agent.C
	vim -s comentar___xstat64.vim xpn_agent.C
	vim -s comentar___lxstat64.vim xpn_agent.C
	vim -s comentar_creat.vim xpn_agent.C
	vim -s comentar_open64.vim xpn_agent.C
	@#vim -s comentar_exec.vim xpn_agent.C
	vim -s comentar_bypass_convert_stat64.vim xpn_agent.C
	vim -s comentar_readdir64.vim xpn_agent.C
	#vim -s comentar_readdir64.vim xpn_agent.C

xpn_agent.o: xpn_agent.C xpn.h
	@#g++ -Wall -Wextra -g -fPIC -fno-exceptions -I$(HOME)/bypass/include -I$(HOME)/expand-1.0/include -DHAVE_CONFIG_H -DLINUX -c -DUSE_PTHREADS xpn_agent.C -o xpn_agent.o
	@#g++ -I$(HOME)/bypass/include -I$(INCLUDE_PATH) $(MYFLAGS) -c xpn_agent.C -o xpn_agent.o
	g++ -I$(BYPASS_DIR)/include $(INCLUDE_PATH) $(MYFLAGS) -c $< -o $@

xpn_agent.so: xpn_agent.o $(OBJECT_FILES)
	@#g++ xpn_agent.o $(HOME)/expand-1.0/src/xpn/*.o $(HOME)/expand-1.0/src/xpn_normal/*.o $(HOME)/expand-1.0/src/nfs/*.o -Xlinker -Bstatic -L$(HOME)/bypass/lib -lbypass -L$(LIBRARY_PATH) -Xlinker -Bdynamic -lnsl -ldl -lpthread -lnfs -lxpn -shared -g -o xpn_agent.so
	@#g++ xpn_agent.o $(OBJECT_FILES) -L$(HOME)/bypass/lib -lbypass $(LIBRARYS) -o xpn_agent.so
	g++ $< $(OBJECT_FILES) -L$(BYPASS_DIR)/lib -lbypass $(LIBRARYS) -o $@

xpn_intercept.c: xpn.bypass
	cp $< $@
	cat $@ | sed '/^agent_/d' > tmp.$$
	mv tmp.$$ $@
	#cat $@ | sed '0,/{{/s///' > tmp.$$ # GNU sed 3.02a
	cat $@ | sed -e '1s/{{//;t' -e '1,/{{/s///' > tmp.$$ # other seds
	mv tmp.$$ $@
	#cat $@ | sed '0,/}};/s///' > tmp.$$ # GNU sed 3.02a
	cat $@ | sed -e '1s/}};//;t' -e '1,/}};/s///' > tmp.$$ # other seds
	mv tmp.$$ $@
	cat $@ | sed 's/{{/{/g' > tmp.$$
	mv tmp.$$ $@
	cat $@ | sed 's/}};/}/g' > tmp.$$
	mv tmp.$$ $@
	cat $@ | sed 's/@/#/g' > tmp.$$
	mv tmp.$$ $@

xpn_intercept.o: xpn_intercept.c
	gcc -I$(INCLUDE_PATH) $(MYFLAGS) -c $< -o $@


xpn_intercept.so: xpn_intercept.o
	g++ $< $(OBJECT_FILES) $(LIBRARYS) -o $@

