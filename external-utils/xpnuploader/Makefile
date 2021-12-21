GSOAP=@GSOAP_DIR@/soapcpp2
WSDL2H=@GSOAP_DIR@/wsdl2h
SOAPH=@SOAPH_DIR@/stdsoap2.h
SOAPC=@SOAPC_DIR@/stdsoap2.c

OUTPUT_DIR=project

CC=mpicc
CWFLAGS=-g -Wall -Wextra
CIFLAGS=-I@SOAPH_DIR@/ -I$(OUTPUT_DIR) -I/usr/local/ssl/include
CLFLAGS=-L/usr/local/ssl/lib
SOAP_FLAGS=-DWITH_GZIP -DWITH_OPENSSL -D_POSIX_THREADS
CFLAGS=$(CWFLAGS) $(CIFLAGS) $(CLFLAGS) $(SOAP_FLAGS)

LIBS=$(CLFLAGS) -lpthread -lm -lz -lssl -lcrypto -ldl

AR=ar

all: XpnUploaderClient XpnUploaderServer

xpnuploader.h: xpnuploader.wsdl typemap.dat
	$(WSDL2H) -c -t typemap.dat -o $@ $<

$(OUTPUT_DIR)/soapC.c $(OUTPUT_DIR)/soapClient.c $(OUTPUT_DIR)/soapServer.c $(OUTPUT_DIR)/xpnuploader.nsmap: xpnuploader.h
	mkdir -p $(OUTPUT_DIR)
	$(GSOAP) -c -d $(OUTPUT_DIR) $<

$(OUTPUT_DIR)/stdsoap2.o: $(SOAPC)
	$(CC) $(CFLAGS)   -c -o $@ $<

libXpnUploaderClient.o: libXpnUploaderClient.c libXpnUploaderClient.h $(OUTPUT_DIR)/xpnuploader.nsmap
#	$(CC) $(CFLAGS) -c $< -o $@

libXpnUploaderServer.o: libXpnUploaderServer.c libXpnUploaderServer.h $(OUTPUT_DIR)/xpnuploader.nsmap
#	$(CC) $(CFLAGS) -c $< -o $@

libXpnUploaderClient.a: libXpnUploaderClient.o $(OUTPUT_DIR)/soapC.o $(OUTPUT_DIR)/soapClient.o $(OUTPUT_DIR)/stdsoap2.o
	$(AR) rcs $@ $^

libXpnUploaderServer.a: libXpnUploaderServer.o $(OUTPUT_DIR)/soapC.o $(OUTPUT_DIR)/soapServer.o $(OUTPUT_DIR)/stdsoap2.o
	$(AR) rcs $@ $^

XpnUploaderClient.o: XpnUploaderClient.c libXpnUploaderClient.a
#	$(CC) $(CFLAGS) -c $< -o $@

XpnUploaderServer.o: XpnUploaderServer.c libXpnUploaderServer.a
#	$(CC) $(CFLAGS) -c $< -o $@

XpnUploaderClient: XpnUploaderClient.o libXpnUploaderClient.a
	$(CC) -o $@ $^ $(LIBS)
#	$(CC) -o $@ $< -L. -lXpnUploaderClient $(LIBS)

XpnUploaderServer: XpnUploaderServer.o libXpnUploaderServer.a th-lock.o
	$(CC) -o $@ $^ $(LIBS)
#	$(CC) -o $@ $< -L. -lXpnUploaderServer $(LIBS) -static

clean:
	rm -f XpnUploaderClient XpnUploaderClient.o XpnUploaderServer XpnUploaderServer.o libXpnUploaderClient.a libXpnUploaderClient.o libXpnUploaderServer.a libXpnUploaderServer.o xpnuploader.h th-lock.o
	rm -rf $(OUTPUT_DIR)
