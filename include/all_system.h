
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
 *
 *  This file is part of Expand.
 *
 *  Expand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Expand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef _ALL_H_SYSTEM_H
#define _ALL_H_SYSTEM_H

  /* ... Include / Inclusion ........................................... */

  //Get config*.h
  #if defined(HAVE_CONFIG_H)
    #include "config.h"
  #endif

  //Get "base_debug.h"
  #include "base_debug.h"

  //Include common headers
  #ifndef NOT_TO_USE_STDLIB_H
    #include <stdlib.h>
  #endif

  #include <stdio.h>
  #include <errno.h>
  #include <stdarg.h>
  #include <stdint.h>
  #include <math.h>
  #include <ctype.h>

  #include <sys/types.h>
  #include <sys/stat.h>
  
  #include <semaphore.h>


  //Include detected headers
  #if defined(HAVE_SYS_PARAM_H)
    #include <sys/param.h>
  #endif

  #if defined(HAVE_DIRENT_H)
    #include <dirent.h>
  #endif

  #if defined(HAVE_STRINGS_H)
    #include <strings.h>
  #endif

  #if defined(HAVE_STRING_H)
    #include <string.h>
  #endif

  #if defined(HAVE_PTHREAD_H)
    #include <pthread.h>
  #endif

  #if defined(HAVE_NETINET_TCP_H)
    #include <netinet/tcp.h>
  #endif

  #if defined(HAVE_NETINET_IN_H)
    #include <netinet/in.h>
    #include <netdb.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
  #endif

  #if defined(HAVE_UNISTD_H)
    #include <unistd.h>
  #endif

  #if defined(HAVE_SYS_TIME_H)
    #include <sys/time.h>
  #endif

  #if defined(HAVE_RPC_RPC_H)
    #include <rpc/rpc.h>
  #endif

  #if defined(HAVE_RPC_CLNT_H)
    #include <rpc/clnt.h>
  #endif

  #if defined(HAVE_RPC_TYPES_H)
    #include <rpc/types.h>
  #endif

  #if defined(HAVE_FCNTL_H)
    #ifndef NOT_TO_USE_FCNTL_H
      #include <fcntl.h>
    #endif
  #endif

  #if defined(HAVE_MPI_H)
    #include <mpi.h>
  #endif

  #if defined(HAVE_MOSQUITTO_H)
    #include <mosquitto.h>
  #endif


  /* ... Const / Const ................................................. */

  //Common sizes
  #ifndef KB
    #define KB  (1024)
  #endif

  #ifndef MB
    #define MB  (KB*KB)
  #endif

  #ifndef GB
    #define GB  (KB*KB*KB)
  #endif

  #ifndef TRUE
    #define TRUE 1
  #endif

  #ifndef FALSE
    #define FALSE 0
  #endif

  #ifndef LARGEFILE_SOURCE
    #define LARGEFILE_SOURCE 1
  #endif

  //Other definitions
  #if !defined(NULL_DEVICE_PATH)
    #define NULL_DEVICE_PATH  "/dev/null"
  #endif

  #if !defined(PATH_MAX)
    #define PATH_MAX  1024
  #endif

  #if !defined(MAX_BUFFER_SIZE)
    #define MAX_BUFFER_SIZE (1*MB)
  #endif

  #define PROTOCOL_MAXLEN 20

  #if !defined(HAVE_FCNTL_H)
    #define O_ACCMODE          0003
    #define O_RDONLY             00
    #define O_WRONLY             01
    #define O_RDWR               02
    #define O_CREAT            0100 /* not fcntl */
    #define O_EXCL             0200 /* not fcntl */
    #define O_NOCTTY           0400 /* not fcntl */
    #define O_TRUNC           01000 /* not fcntl */
    #define O_APPEND          02000
    #define O_NONBLOCK        04000
    #define O_NDELAY        O_NONBLOCK
    #define O_SYNC           010000
    #define O_FSYNC          O_SYNC
    #define O_ASYNC          020000
  #endif

  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */
  

  /* ................................................................... */

#endif /* _ALL_H_SYSTEM_H */
