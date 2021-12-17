
#include "mpiServer_params.h"


void params_show ( mpiServer_param_st *params )
{
	printf("Usage:\n");
	printf("\t-n <string>:\t%s\n", params->name) ;
	printf("\t-p <int>:\t%d\n",    params->port) ;
	printf("\t-io <int>:\t%d\n",   params->IOsize) ;
	printf("\t-f <string>:\t%s\n", params->file) ;
	printf("\t-d <string>:\t%s\n", params->dirbase) ;
}

int params_get ( mpiServer_param_st *params, int argc, char *argv[] )
{
	// set default values
	params->argc = argc ;
	params->argv = argv ;
	params->size = 0 ;
	params->rank = 0 ;
	params->IOsize = MPISERVER_IOSIZE_DEFAULT ;
	strcpy(params->port_name, "") ;
	strcpy(params->srv_name,  "") ;
	strcpy(params->dirbase,   MPISERVER_DIRBASE_DEFAULT) ;

	// update user requests
	for (int i=0; i<argc; i++)
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
						return -1;

					default:
						break;
				}
				break;
			default:
				break;			
		}
	}

	return 0;
}

