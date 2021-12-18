
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


   /* ... Functions / Funciones ......................................... */

   void sigint_handler ( int signal )
   {
	printf("[MAIN] Signal %d received => ending execution...", signal) ;
        the_end = 1;
   }

   void worker_function ( struct st_th th )
   {
        int op;
        struct st_mpiServer_msg head;
      
        do {
		head.type = MPISERVER_END;

		debug_info("[WORKERS] (ID=%d) mpiServer_read_operation begin...\n", th.id);
		op = mpiServer_read_operation(th.params, th.sd, &head);
			
		debug_info("[WORKERS] (ID=%d) mpiServer_op_<%d> begins\n", th.id, op);
		switch(op)
		{
			case MPISERVER_OPEN_FILE:
				mpiServer_op_open(th.params, th.sd, &head);
				break;
			case MPISERVER_CREAT_FILE:
				mpiServer_op_creat(th.params, th.sd, &head);
				break;
			case MPISERVER_READ_FILE:
				mpiServer_op_read(th.params, th.sd, &head);
				break;
			case MPISERVER_WRITE_FILE:
				mpiServer_op_write(th.params, th.sd, &head);
				break;
			case MPISERVER_CLOSE_FILE:
				mpiServer_op_close(th.params, th.sd, &head);
				break;
			case MPISERVER_RM_FILE:
				mpiServer_op_rm(th.params, th.sd, &head);
				break;
			case MPISERVER_GETATTR_FILE:
				mpiServer_op_getattr(th.params, th.sd, &head);
				break;
			case MPISERVER_SETATTR_FILE:
				mpiServer_op_setattr(th.params, th.sd, &head);
				break;
			case MPISERVER_MKDIR_DIR:
				mpiServer_op_mkdir(th.params, th.sd, &head);
				break;
			case MPISERVER_RMDIR_DIR:
				mpiServer_op_rmdir(th.params, th.sd, &head);
				break;
			case MPISERVER_PRELOAD_FILE:
				mpiServer_op_preload(th.params, th.sd, &head);
				break;
			case MPISERVER_FLUSH_FILE:
				mpiServer_op_flush(th.params, th.sd, &head);
				break;
			case MPISERVER_GETID:
				mpiServer_op_getid(th.params, th.sd, &head);
				break;
			case MPISERVER_FINALIZE:
				op = MPISERVER_FINALIZE;
				break;
			default:
				op = MPISERVER_END;
				break;
		}
		debug_info("[WORKERS] (ID=%d) mpiServer_op_<%d> ends\n", th.id, op);
				
        } while(op != MPISERVER_END);
      
        debug_info("[WORKERS] mpiServer_worker_run (ID=%d) close\n", th.id);
        mpiServer_comm_close(th.params) ;
   }


   /*
    * Main
    */

   int main ( int argc, char *argv[] )
   {
	int sd;
	int ret ;

	// Get parameters..
        ret = mpiServer_params_get(&params, argc, argv) ;
	if (ret < 0) {
	    mpiServer_params_show_usage() ;
	    exit(-1) ;
	}

	mpiServer_params_show(&params) ;

	// Initialize
	the_end = 0 ;
	signal(SIGINT, sigint_handler) ;
        mpiServer_utils_init() ;
	mpiServer_init_worker() ;
	mpiServer_comm_init(&params) ;

	// Loop: receiving + processing
	while (0 == the_end)
	{
        	debug_info("[MAIN] mpiServer_accept_comm()\n") ;
		sd = mpiServer_comm_accept(&params) ;
		if (sd == -1) {
		    continue ;
		}

        	debug_info("[MAIN] mpiServer_launch_worker()\n") ;
	        mpiServer_launch_worker(&params, sd, worker_function) ;
	}

	// Wait for all current workers
	// TODO

	// Finalize
	mpiServer_comm_destroy(&params) ;
    	xpn_destroy() ;

	// return OK 
	return 0 ;
   }


   /* ................................................................... */

