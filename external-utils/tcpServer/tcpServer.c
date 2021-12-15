/* defines usados
 * _LARGEFILE64_: para soporte de ficheros mayores de 4GB
 * _MPI_: para lanzar los servidores como procesos MPI
 * DBG_MAIN: imprimo depuracion
 * _LOG_: imprimo errores
 * _COMPRESS_: uso del sistema de compresion lzf
 */

/* VARIABLES DE ENTORNO:
 * MYSERVER_DNS: indica donde se encuentra el sistema traductor de 
 * <id> <hostname> <port>
 */

#include "tcpServer.h"
#include "tcpServer_ops.h"
#include "tcpServer_workers.h"
#include "tcpServer_comm.h"
#include "tcpServer_d2xpn.h"
#include "tcpServer_params.h"
///////////////////////////////



struct tcpServer_param_st tcpServer_params;

/* GLOBAL VARIABLES */
char *MYSERVER_ALIAS_NAME_STRING;
char *MYSERVER_FILE_STRING;
char *MYSERVER_DIRBASE_STRING;


/* INTERNAL CONST & STRUCTS */
extern int errno;
//pthread_t th;


void show_values(struct tcpServer_param_st *params){
	printf("Usage:\n");
	printf("\t-n <string>:\t%s\n",params->name);
	printf("\t-p <int>:\t%d\n",params->port);
	printf("\t-io <int>:\t%d\n",params->IOsize);
	printf("\t-f <string>:\t%s\n",params->file);	
	printf("\t-d <string>:\t%s\n",params->dirbase);
}


void show_usage(){
	printf("Usage:\n");
	printf("\t-n <string>: name of the server\n");
	printf("\t-p <int>: port number\n");
	printf("\t-io <int>: IOsize\n");
	printf("\t-f <string>: name of the DNS file\n");	
	printf("\t-d <string>: dir base\n");
}


int get_params(int argc, char *argv[], struct tcpServer_param_st *params){
	int i;

	params->port 	= MYSERVER_PORT_DEFAULT;	
	params->IOsize 	= MYSERVER_IOSIZE_DEFAULT;	
	gethostname(params->name, 255);
	strcpy(params->file, MYSERVER_FILE_DEFAULT);
	strcpy(params->dirbase, MYSERVER_DIRBASE_DEFAULT);
	
	for(i=0;i<argc;i++){
		switch(argv[i][0]){
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

	MYSERVER_ALIAS_NAME_STRING = params->name;
	MYSERVER_FILE_STRING = params->file;
	MYSERVER_DIRBASE_STRING = params->dirbase;
	MYSERVER_IOSIZE_INT = params->IOsize * KB;

	
	return 0;
}



int main(int argc, char *argv[]){
	int sd;
	int cont;

	setbuf(stdout,NULL);	
	setbuf(stderr,NULL);

	if(get_params(argc,argv, &tcpServer_params) == -1){
		show_usage();
		exit(-1);
	}
	show_values(&tcpServer_params);
	tcpServer_comm_init(tcpServer_params.name,
		       	   tcpServer_params.port,
			   tcpServer_params.file);

	//tcpServer_init_worker(&th);
	tcpServer_init_worker();

	cont = 0;

	while (1)
	{
	#ifdef DBG_MAIN
        	printf("tcpServer_accept_comm()\n");
	#endif
		sd = tcpServer_accept_comm();
	#ifdef DBG_MAIN
        	printf("tcpServer_launch_worker()\n");
	#endif
		if(sd == -1){
			break;
		}
		//tcpServer_launch_worker(sd, &th);
		tcpServer_launch_worker(sd);
	}

	tcpServer_close_comm();
    	xpn_destroy();
	exit(0);
}
