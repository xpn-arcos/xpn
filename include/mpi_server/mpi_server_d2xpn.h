#ifndef _MPI_SERVER_D2XPN_H_
#define _MPI_SERVER_D2XPN_H_

   #include <sys/param.h>
   #include <stdio.h>
   #include <sys/file.h>
   #include <sys/fcntl.h>
   #include <unistd.h>
   #include <sys/time.h>
   #include <sys/wait.h>
   #include <sys/errno.h>

   #include "all_system.h"
   #include "base/utils.h"
   #include "mpi_server_params.h"
   #include "mpi_server_ops.h"

   #include "xpn.h"

   #define PRELOAD_SYNC  0
   #define PRELOAD_ASYNC 1

   int mpi_server_d2xpn ( mpi_server_param_st *params, char *origen, char *destino ) ;

#endif
