add_custom_command	(OUTPUT posix.h
			 COMMAND wsdl2h -c -t typemap.dat -o posix.h posix.wsdl
			 DEPENDS posix.wsdl typemap.dat)

add_custom_command	(OUTPUT project/posix.nsmap project/soapC.c project/soapClient.c project/soapServer.c
			 COMMAND mkdir -p project
			 COMMAND soapcpp2 -I ${GSOAP_IMPORT_DIR} -c -d project posix.h
			 DEPENDS posix.h)

set			(CMAKE_C_FLAGS "-Iproject -I/usr/include/gsoap -I/usr/include/openssl -I/usr/local/ssl/include -DWITH_GZIP -DWITH_OPENSSL -D_POSIX_THREADS -D_FILE_OFFSET_BITS=64")

add_library		(IOServiceClient STATIC libIOServiceClient.c project/soapC.c project/soapClient.c)

add_library		(IOServiceServer STATIC libIOServiceServer.c project/soapC.c project/soapServer.c th-lock.c)

add_executable		(client IOServiceClient.c)
target_link_libraries	(client -static -L. -L/usr/local/ssl/lib -lIOServiceClient -lpthread -lm -lgsoapssl -lssl -lcrypto -lz -ldl)
SET_TARGET_PROPERTIES	(client PROPERTIES OUTPUT_NAME "IOServiceClient")

add_executable		(server IOServiceServer.c)
target_link_libraries	(server -static -L. -L/usr/local/ssl/lib -lIOServiceServer -lpthread -lm -lgsoapssl -lssl -lcrypto -lz -ldl)
SET_TARGET_PROPERTIES	(server PROPERTIES OUTPUT_NAME "IOServiceServer")

add_executable		(serverq IOServiceServerRequestsQueue.c)
target_link_libraries	(serverq -static -L. -L/usr/local/ssl/lib -lIOServiceServer -lpthread -lm -lgsoapssl -lssl -lcrypto -lz -ldl)
SET_TARGET_PROPERTIES	(serverq PROPERTIES OUTPUT_NAME "IOServiceServerRequestsQueue")

