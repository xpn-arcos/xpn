
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

      #include "mpiServer_params.h"


   /* ... Functions / Funciones ......................................... */

      void mpiServer_params_show ( mpiServer_param_st *params )
      {
      	   printf("Current configuration:\n");
      	   printf("\t-io <int>:\t%d\n",     params->IOsize) ;
      	   printf("\t-d <string>:\t%s\n",   params->dirbase) ;

            if(params->thread_mode == TH_POOL)
               printf("\t-p:\tThread Pool\n") ;
            if(params->thread_mode == TH_OP)
               printf("\t-p:\tThread per client\n") ;
      }
      
      void mpiServer_params_show_usage ( void )
      {
      	   printf("Usage:\n");
      	   printf("\t-io <int>: IOsize\n") ;
      	   printf("\t-d <string>: name of the base directory\n") ;
            printf("\t-p: thread Pool\n") ;
      }
      
      int mpiServer_params_get ( mpiServer_param_st *params, int argc, char *argv[] )
      {
           DEBUG_BEGIN() ;

      	// set default values
      	   params->argc = argc ;
      	   params->argv = argv ;
      	   params->size = 0 ;
      	   params->rank = 0 ;
      	   params->IOsize = MPISERVER_IOSIZE_DEFAULT ;
            params->thread_mode = TH_OP ;
      	   strcpy(params->port_name, "") ;
      	   strcpy(params->srv_name,  "") ;
      	   strcpy(params->dirbase,   MPISERVER_DIRBASE_DEFAULT) ;
      
      	// update user requests
      	   for (int i=0; i<argc; i++)
      	   {
      		switch (argv[i][0])
      		{
      			case '-':
      				switch (argv[i][1])
				{
      					/*
      					case 'p':
      						params->port = atoi(argv[i+1]);
      						i++;
      						break;
      					*/
      					case 'i':
      						params->IOsize = atoi(argv[i+1]);
      						i++;
      						break;
      					/*
      					case 'n':
      						strcpy(params->name, argv[i+1]);
      						i++;
      						break;					
      					case 'f':
      						strcpy(params->file, argv[i+1]);
      						i++;
      						break;					
      					*/
      					case 'd':
      						strcpy(params->dirbase, argv[i+1]);
      						i++;
      						break;
                     case 'p':
                        params->thread_mode = TH_POOL;
                        i++;
                        break;				
      					case 'h':
      						return -1;
      
      					default:
      						break;
      				}
      				break;

      			default:
      				break;			
      		}
      	   }

      	// return OK
           DEBUG_END() ;
      	   return 1;
      }


   /* ................................................................... */

