
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


/*
 * Includes
 */
#include "mpiServer.h"
#include "mpiServer_ops.h"
#include "mpiServer_workers.h"
#include "mpiServer_comm.h"
#include "mpiServer_d2xpn.h"
#include "mpiServer_params.h"


/* 
 * GLOBAL VARIABLES 
 */
struct mpiServer_param_st mpiServer_params;

char  *MPISERVER_ALIAS_NAME_STRING;
char  *MPISERVER_FILE_STRING;
char  *MPISERVER_DIRBASE_STRING;


/*
 * Auxiliar functions
 */

void show_values ( struct mpiServer_param_st *params )
{
	printf("Usage:\n");
	printf("\t-n <string>:\t%s\n", params->name) ;
	printf("\t-p <int>:\t%d\n",    params->port) ;
	printf("\t-io <int>:\t%d\n",   params->IOsize) ;
	printf("\t-f <string>:\t%s\n", params->file) ;
	printf("\t-d <string>:\t%s\n", params->dirbase) ;
}


void show_usage()
{
	printf("Usage:\n");
	printf("\t-n <string>: name of the server\n") ;
	printf("\t-p <int>: port number\n") ;
	printf("\t-io <int>: IOsize\n") ;
	printf("\t-f <string>: name of the DNS file\n") ; 
	printf("\t-d <string>: dir base\n") ;
}


int get_params ( int argc, char *argv[], struct mpiServer_param_st *params )
{
	int i;

	// set default values
	gethostname(params->name, 255);
	params->port 	      = MPISERVER_PORT_DEFAULT;
	params->IOsize 	      = MPISERVER_IOSIZE_DEFAULT;
	strcpy(params->file,    MPISERVER_FILE_DEFAULT);
	strcpy(params->dirbase, MPISERVER_DIRBASE_DEFAULT);

	// update user requests
	for(i=0;i<argc;i++)
	{
		switch(argv[i][0])
		{
			case '-':
				switch(argv[i][1]){
					case 'p':
						params->port = atoi(argv[i+1]);
						i++;
						break;
					case 'i':
						params->IOsize = atoi(argv[i+1]);
						i++;
						break;
					case 'n':
						strcpy(params->name, argv[i+1]);
						i++;
						break;					
					case 'f':
						strcpy(params->file, argv[i+1]);
						i++;
						break;					
					case 'd':
						strcpy(params->dirbase, argv[i+1]);
						i++;
						break;					
					case 'h':
						show_usage();
						exit(0);					

					default:
						break;
				}
				break;
			default:
				break;			
		}
	}

	// copy parameters into the global variables
	MPISERVER_ALIAS_NAME_STRING = params->name ;
	MPISERVER_FILE_STRING       = params->file ;
	MPISERVER_DIRBASE_STRING    = params->dirbase ;
	MPISERVER_IOSIZE_INT        = params->IOsize * KB ;

	return 0;
}


void sigint_handler ( int signal )
{
	printf("Signal %d received !!", signal) ;

	exit(0) ;
}


int main(int argc, char *argv[])
{
	int sd;
	int the_end;

	// Initializing...
	setbuf(stdout,NULL);
	setbuf(stderr,NULL);

	signal(SIGINT, sigint_handler);

	// Get parameters..
	if(get_params(argc,argv, &mpiServer_params) == -1){
		show_usage();
		exit(-1);
	}
	show_values(&mpiServer_params);
	mpiServer_comm_init(mpiServer_params.name,
		       	    mpiServer_params.port,
			    mpiServer_params.file);

	//mpiServer_init_worker(&th);
	mpiServer_init_worker();

	the_end = 0;
	while (0 == the_end)
	{
        	debug_printf("[MAIN] mpiServer_accept_comm()\n");
		sd = mpiServer_accept_comm();
        	debug_printf("[MAIN] mpiServer_launch_worker()\n");
		if(sd == -1){
			break;
		}
		//mpiServer_launch_worker(sd, &th);
		mpiServer_launch_worker(sd);
	}

	mpiServer_close_comm();
    	xpn_destroy();
	exit(0);
}

