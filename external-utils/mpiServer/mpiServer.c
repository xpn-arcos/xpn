
/*
 *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 *
 *  This file is part of mpiServer.
 *
 *  mpiServer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  mpiServer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with mpiServer.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


/* defines usados
 * _LARGEFILE64_: para soporte de ficheros mayores de 4GB
 * _MPI_: para lanzar los servidores como procesos MPI
 * DEBUG: imprimo depuracion
 * _LOG_: imprimo errores
 * _COMPRESS_: uso del sistema de compresion lzf
 */

/* VARIABLES DE ENTORNO:
 * MPISERVER_DNS: indica donde se encuentra el sistema traductor de
 * <id> <hostname> <port>
 */


   /* ... Include / Inclusion ........................................... */

   #include "all_system.h"
   #include "mpiServer_params.h"
   #include "mpiServer_utils.h"
   #include "mpiServer_ops.h"
   #include "mpiServer_workers.h"
   #include "mpiServer_comm.h"
   #include "mpiServer_d2xpn.h"


   /* ... Global variables / Variables globales ......................... */

   int the_end = 0;
   mpiServer_param_st params;

   char  *MPISERVER_ALIAS_NAME_STRING;
   char  *MPISERVER_FILE_STRING;
   char  *MPISERVER_DIRBASE_STRING;


   /* ... Functions / Funciones ......................................... */

void show_usage()
{
	printf("Usage:\n");
	printf("\t-n <string>: name of the server\n") ;
	printf("\t-p <int>: port number\n") ;
	printf("\t-io <int>: IOsize\n") ;
	printf("\t-f <string>: name of the DNS file\n") ; 
	printf("\t-d <string>: dir base\n") ;
}

void sigint_handler ( int signal )
{
	printf("[MAIN] Signal %d received => ending execution...", signal) ;
        the_end = 1;
}


   /*
    * Main
    */

int main ( int argc, char *argv[] )
{
	int sd;
	int ret ;

	// Initializing...
	setbuf(stdout,NULL) ;
	setbuf(stderr,NULL) ;
	signal(SIGINT, sigint_handler) ;

	// Get parameters..
        ret = params_get(&params, argc, argv) ;
	if (ret < 0) {
	    show_usage() ;
	    exit(-1) ;
	}

	//MPISERVER_ALIAS_NAME_STRING = params.name ;
	//MPISERVER_FILE_STRING       = params.file ;
	MPISERVER_DIRBASE_STRING    = params.dirbase ;
	MPISERVER_IOSIZE_INT        = params.IOsize * KB ;

	params_show(&params) ;

	// Initialize
	mpiServer_comm_init(&params) ;
	mpiServer_init_worker() ;

	the_end = 0 ;
	while (0 == the_end)
	{
        	debug_info("[MAIN] mpiServer_accept_comm()\n") ;
		sd = mpiServer_comm_accept(&params) ;
		if (sd == -1){
		    continue ;
		}

        	debug_info("[MAIN] mpiServer_launch_worker()\n") ;
		mpiServer_launch_worker(&params, sd) ;
	}

	// Finalize
	mpiServer_comm_destroy(&params) ;
    	xpn_destroy() ;

	// return OK 
	return 0 ;
}

   /* ................................................................... */

