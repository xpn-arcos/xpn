cmake_minimum_required (VERSION 2.8)
project (Expand)

# The version number.
set (Expand_VERSION_MAJOR 2)
set (Expand_VERSION_MINOR 0)

# Add revision from subversion
find_package(Subversion)
if(Subversion_FOUND)
	Subversion_WC_INFO(${CMAKE_CURRENT_SOURCE_DIR} Project)
	MESSAGE("Current revision is ${Project_WC_REVISION}")
	set(SUBVERSION_REVISION ${Project_WC_REVISION})
	#set(CPACK_PACKAGE_VERSION_PATCH "${SUBVERSION_REVISION}")
endif(Subversion_FOUND)

# check libs and packages (headers + lib)

# std C libraries
#find_package(stdlibs REQUIRED)

# libpthread with pthread.h
# http://www.cmake.org/cmake/help/cmake-2-8-docs.html#module:FindThreads
find_package(Threads REQUIRED)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/ExpandConfig.h.in"
  "${PROJECT_BINARY_DIR}/ExpandConfig.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories ("${PROJECT_BINARY_DIR}")

set (CMAKE_C_FLAGS "-O0 -g -g3 -ggdb -ggdb3 -Wall -Wextra")
#set (CMAKE_C_FLAGS "-fPIC -O0 -g -g3 -ggdb -ggdb3 -Wall -Wextra -pedantic -Werror -std=c99")

# additional compiler flags
add_definitions(-D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -DENABLE_MXML)
#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -DENABLE_MXML")

#set (CDEFS "-D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -DENABLE_MXML")
#set (CFLAGS "-O0 -g -g3 -ggdb -ggdb3 -Wall -Wextra")

include_directories("${PROJECT_SOURCE_DIR}/include /home/bberguaguerra/mxml/usr/include")

link_directories("${PROJECT_BINARY_DIR}/lib /home/bberguaguerra/mxml/usr/lib")

#set (LDFLAGS "")
#set (LDLIBS "-lxpn -lmxml")
#set (CMAKE_EXE_LINKER_FLAGS "-lxpn -lmxml")
set (CMAKE_LINK_LIBRARY_FLAG "-lxpn -lmxml")

### BEGIN OF OPTION ENABLE BLOCK. Do not remove this line. ###
### BEGIN OF NFS BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if nfs is enabled.
#
option (ENABLE_NFS "Enable NFS v2 module." OFF)
if (ENABLE_NFS)
	MESSAGE("Enabling NFS2 module")
	add_definitions(-DENABLE_NFS)
	#set (CDEFS "${CDEFS} -DENABLE_NFS")
	#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENABLE_NFS")
endif (ENABLE_NFS)
### END OF NFS BLOCK. Do not remove this line. ###

### BEGIN OF NFS3 BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if nfs3 is enabled.
#
option (ENABLE_NFS3 "Enable NFS v3 module." OFF)
if (ENABLE_NFS3)
	MESSAGE("Enabling NFS3 module")
	add_definitions(-DENABLE_NFS3)
	#set (CDEFS "${CDEFS} -DENABLE_NFS3")
	#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENABLE_NFS3")
endif (ENABLE_NFS3)
### END OF NFS3 BLOCK. Do not remove this line. ###

### BEGIN OF FTP BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if ftp is enabled.
#
option (ENABLE_FTP "Enable FTP module. CURRENTLY NOT WORKING!!!" OFF)
if (ENABLE_FTP)
	MESSAGE("Enabling FTP module. CURRENTLY NOT WORKING!!!")
	add_definitions(-DENABLE_FTP)
	#set (CDEFS "${CDEFS} -DENABLE_FTP")
	#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENABLE_FTP")
	# TODO
endif (ENABLE_FTP)
### END OF FTP BLOCK. Do not remove this line. ###

### BEGIN OF XIO BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if xio is enabled.
#
option (ENABLE_XIO "Enable Globus XIO module." OFF)
if (ENABLE_XIO)
	MESSAGE("Enabling Globus XIO module")
	add_definitions(-DENABLE_XIO)
	#set (CDEFS "${CDEFS} -DENABLE_XIO")
	#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENABLE_XIO")
	#include_directories()
	# TODO
endif (ENABLE_XIO)
### END OF XIO BLOCK. Do not remove this line. ###

### BEGIN OF GSOAP BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if gsoap is enabled.
#
option (ENABLE_GSOAP "Enable Gsoap module." OFF)
if (ENABLE_GSOAP)
	MESSAGE("Enabling Gsoap module")
	# TODO: System instropection: path, installed by package/source code, headers
	add_definitions(-DENABLE_GSOAP)
	#set (CDEFS "${CDEFS} -DENABLE_GSOAP")
	#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENABLE_GSOAP")
	#set (LDFLAGS "${LDFLAGS} -L/usr/local/ssl/lib -L${PROJECT_SOURCE_DIR}/src/nfi/nfi_gsoap/PosixIOService")
	#set (LDLIBS "${LDLIBS} -lIOServiceClient -lpthread -lm -lgsoapssl -lssl -lcrypto -lz -ldl")
	#set (CMAKE_LIBRARY_PATH_FLAG ${CMAKE_LIBRARY_PATH_FLAG} -L/usr/local/ssl/lib -L${PROJECT_SOURCE_DIR}/src/nfi/nfi_gsoap/PosixIOService)
	link_directories(/usr/local/ssl/lib ${PROJECT_SOURCE_DIR}/src/nfi/nfi_gsoap/PosixIOService)
	set (CMAKE_LINK_LIBRARY_FLAG "${CMAKE_LINK_LIBRARY_FLAG} -lIOServiceClient -lpthread -lm -lgsoapssl -lssl -lcrypto -lz -ldl")
	#set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -lIOServiceClient -lpthread -lm -lgsoapssl -lssl -lcrypto -lz -ldl)
	#add_dependencies(xpn IOServiceClient)
	set (EXPAND_DEPS "IOServiceClient")
	add_subdirectory (src/nfi/nfi_gsoap/PosixIOService)
endif (ENABLE_GSOAP)
### END OF GSOAP BLOCK. Do not remove this line. ###

### BEGIN OF GRIDFTP BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if gridftp is enabled.
#
option (ENABLE_GRIDFTP "Enable Globus GridFTP module." OFF)
if (ENABLE_GRIDFTP)
	MESSAGE("Enabling Globus GridFTP module")
	add_definitions(-DENABLE_GRIDFTP)
	#set (CDEFS "${CDEFS} -DENABLE_GRIDFTP")
	#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENABLE_GRIDFTP")
	# TODO
endif (ENABLE_GRIDFTP)
### END OF GRIDFTP BLOCK. Do not remove this line. ###

### BEGIN OF TCPSERVER BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if gridftp is enabled.
#
option (ENABLE_TCPSERVER "Enable MyServer module." OFF)
if (ENABLE_TCPSERVER)
	MESSAGE("Enabling MyServer module")
	add_definitions(-DENABLE_TCPSERVER)
	#set (CDEFS "${CDEFS} -DENABLE_TCPSERVER")
	#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENABLE_TCPSERVER")
	# TODO
endif (ENABLE_TCPSERVER)
### END OF TCPSERVER BLOCK. Do not remove this line. ###

### BEGIN OF MPISERVER BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if gridftp is enabled.
#
option (ENABLE_MPISERVER "Enable MyServer module." OFF)
if (ENABLE_MPISERVER)
	MESSAGE("Enabling MyServer module")
	add_definitions(-DENABLE_MPISERVER)
	#set (CDEFS "${CDEFS} -DENABLE_MPISERVER")
	#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENABLE_MPISERVER")
	# TODO
endif (ENABLE_MPISERVER)
### END OF MPISERVER BLOCK. Do not remove this line. ###

### BEGIN OF IOPROXY_RPC BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if ioproxy-rpc is enabled.
#
option (ENABLE_IOPROXY_RPC "Enable ioproxy-rpc module." OFF)
if (ENABLE_IOPROXY_RPC)
	MESSAGE("Enabling ioproxy-rpc module")
	add_definitions(-DENABLE_IOPROXY_RPC)
	#set (CDEFS "${CDEFS} -DENABLE_IOPROXY_RPC")
	#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENABLE_IOPROXY_RPC")
	# TODO
endif (ENABLE_IOPROXY_RPC)
### END OF IOPROXY_RPC BLOCK. Do not remove this line. ###

### BEGIN OF HTTP BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if http is enabled.
#
option (ENABLE_HTTP "Enable http module." OFF)
if (ENABLE_HTTP)
	MESSAGE("Enabling http module")
	add_definitions(-DENABLE_HTTP)
	#set (CDEFS  "${CDEFS}  -DENABLE_HTTP")
	#set (CFLAGS "${CFLAGS} -Infi/nfi_http/fusedav/libneon/src")
	#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENABLE_HTTP -Infi/nfi_http/fusedav/libneon/src -Infi/nfi_http/fusedav/libneon/src")
	#ADD_DEFINITIONS (-DENABLE_HTTP -Infi/nfi_http/fusedav/libneon/src -Infi/nfi_http/fusedav/libneon/src)
	#set (COMPILE_DEFINITIONS "-DENABLE_HTTP -Infi/nfi_http/fusedav/libneon/src -Infi/nfi_http/fusedav/libneon/src")
	#set_property (GLOBAL PROPERTY COMPILE_DEFINITIONS "-DENABLE_HTTP -Infi/nfi_http/fusedav/libneon/src -Infi/nfi_http/fusedav/libneon/src")
	#set_property (DIRECTORY . PROPERTY COMPILE_DEFINITIONS "-DENABLE_HTTP -Infi/nfi_http/fusedav/libneon/src -Infi/nfi_http/fusedav/libneon/src")
	#set (LDFLAGS "${LDFLAGS} -L${PROJECT_SOURCE_DIR}/src/nfi/nfi_http/fusedav/src -L${PROJECT_SOURCE_DIR}/src/nfi/nfi_http/fusedav/libneon/src/.libs")
	#set (LDLIBS "${LDLIBS} -lfusedav -lneon -lexpat -lgssapi_krb5 -lssl")
	include_directories (${PROJECT_BINARY_DIR}/src/nfi/nfi_http/fusedav/libneon/src ${PROJECT_BINARY_DIR}/src/nfi/nfi_http/fusedav/src)
	link_directories(${PROJECT_SOURCE_DIR}/src/nfi/nfi_http/fusedav/src ${PROJECT_SOURCE_DIR}/src/nfi/nfi_http/fusedav/libneon/src/.libs)
	set (CMAKE_LINK_LIBRARY_FLAG "${CMAKE_LINK_LIBRARY_FLAG} -lfusedav -lneon -lexpat -lgssapi_krb5 -lssl")
	#set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${PROJECT_SOURCE_DIR}/src/nfi/nfi_http/fusedav/src -L${PROJECT_SOURCE_DIR}/src/nfi/nfi_http/fusedav/libneon/src/.libs -lfusedav -lneon -lexpat -lgssapi_krb5 -lssl")
	include (ExternalProject)
	
	set (EXPAND_DEPS "${EXPAND_DEPS} fusedav-expand fusedav")
	#add_dependencies(xpn fusedav-expand)
	#add_dependencies(xpn fusedav)
	#add_subdirectory (src/nfi/nfi_http)
#	add_custom_command	(OUTPUT ${PROJECT_SOURCE_DIR}/src/nfi/nfi_http/fusedav/libneon/libneon.a
#				 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/nfi/nfi_http/fusedav/libneon/
#				 COMMAND ./autogen.sh
#				 COMMAND bash ./configure.gnu
#				 COMMAND make
#				)
##	add_custom_command	(OUTPUT ${PROJECT_SOURCE_DIR}/src/nfi/nfi_http/fusedav/fusedav.a
##				 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/nfi/nfi_http/fusedav/
##				 COMMAND NOCONFIGURE=1 ./bootstrap.sh
##				 DEPENDS ${PROJECT_SOURCE_DIR}/src/nfi/nfi_http/fusedav/libneon/libneon.a
##				)
#	add_custom_target	(fusedav
#				 NOCONFIGURE=1 ./bootstrap.sh
#				 COMMAND bash ./configure.gnu
#				 COMMAND make
#				 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/nfi/nfi_http/fusedav/
#				 #DEPENDS ${PROJECT_SOURCE_DIR}/src/nfi/nfi_http/fusedav/libneon/libneon.a
#				)
	add_subdirectory (src/nfi/nfi_http/fusedav)
endif (ENABLE_HTTP)
### END OF HTTP BLOCK. Do not remove this line. ###
### END OF OPTION ENABLE BLOCK. Do not remove this line. ###


#set_property (TARGET xpn PROPERTY COMPILE_FLAGS "${CDEFS} ${CFLAGS}")
#set_property (TARGET xpn PROPERTY LINK_FLAGS "${LDFLAGS} ${LDLIBS}")

#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CDEFS} ${CFLAGS}")
#set (CMAKE_EXE_LINKER_FLAGS "${LDLIBS}")

add_subdirectory (src)
add_dependencies(xpn "${EXPAND_DEPS}")

# Use:
# cmake -D ENABLE_FUSE=[1|ON|True] .
option (ENABLE_FUSE "Enable FUSE module." OFF)
if (ENABLE_FUSE)
	MESSAGE("Enabling FUSE module")
	add_subdirectory (fuse)
endif (ENABLE_FUSE)

add_subdirectory (utils)
#add_subdirectory (tests)

#include(FeatureSummary)
#PRINT_ENABLED_FEATURES()
