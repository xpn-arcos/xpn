#ifndef _MPISERVER_D2XPN_H_
#define _MPISERVER_D2XPN_H_

   #include <sys/param.h>
   #include <stdio.h>
   #include <sys/file.h>
   #include <sys/fcntl.h>
   #include <unistd.h>
   #include <sys/time.h>
   #include <sys/wait.h>
   #include <sys/errno.h>

   #include "all_system.h"
   #include "mpiServer_params.h"
   #include "mpiServer_utils.h"
   #include "mpiServer_ops.h"

   #include "xpn.h"

   #define PRELOAD_SYNC  0
   #define PRELOAD_ASYNC 1

   int mpiServer_d2xpn ( mpiServer_param_st *params, char *origen, char *destino, int opt ) ;

#endif
