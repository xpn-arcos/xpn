
#include "mpiServer_params.h"


void params_show ( struct mpiServer_param_st *params )
{
	printf("Usage:\n");
	printf("\t-n <string>:\t%s\n", params->name) ;
	printf("\t-p <int>:\t%d\n",    params->port) ;
	printf("\t-io <int>:\t%d\n",   params->IOsize) ;
	printf("\t-f <string>:\t%s\n", params->file) ;
	printf("\t-d <string>:\t%s\n", params->dirbase) ;
}

int params_get ( int argc, char *argv[], struct mpiServer_param_st *params )
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

