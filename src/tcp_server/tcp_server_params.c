
  /*
   *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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


   /* ... Include / Inclusion ........................................... */

      #include "tcp_server_params.h"


   /* ... Functions / Funciones ......................................... */

      void params_show_values ( struct tcp_server_param_st *params )
      {
         printf("Usage:\n");
         printf("\t-n <string>:\t%s\n",params->name);
         printf("\t-p <int>:\t%d\n",params->port);
         printf("\t-io <int>:\t%d\n",params->IOsize);
         printf("\t-f <string>:\t%s\n",params->file);
         printf("\t-d <string>:\t%s\n",params->dirbase);

         if(params->thread_mode == TH_POOL)
            printf("\t-tp:\t\tThread Pool Activated\n") ;
         if(params->thread_mode == TH_OP)
            printf("\t-tp:\t\tThread Pool Deactivated (Using Thread per Client)\n") ;
      }

      void params_show_usage ( void )
      {
         printf("Usage:\n");
         printf("\t-n <string>: name of the server\n");
         printf("\t-p <int>: port number\n");
         printf("\t-io <int>: IOsize\n");
         printf("\t-f <string>: name of the DNS file\n");
         printf("\t-d <string>: dir base\n");
         printf("\t-tp: use thread pool\n") ;
      }

      int params_get ( int argc, char *argv[], struct tcp_server_param_st *params )
      {
         int i;

         params->port   = TCP_SERVER_PORT_DEFAULT;
         params->IOsize    = TCP_SERVER_IOSIZE_DEFAULT;
         gethostname(params->name, 255);
         strcpy(params->file, TCP_SERVER_FILE_DEFAULT);
         strcpy(params->dirbase, TCP_SERVER_DIRBASE_DEFAULT);
         params->thread_mode = TH_OP ;

         for (i=0;i<argc;i++)
         {
            switch(argv[i][0])
            {
               case '-':
                  switch(argv[i][1])
                  {
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
                     case 't':
                       if ((strlen(argv[i]) == 3) && (argv[i][2] == 'p')){
                          params->thread_mode = TH_POOL;
                       }
                       i++;
                       break; 
                     case 'h':
                        params_show_usage();
                        exit(0);

                     default:
                        break;
                  }
                  break;
               default:
                  break;
            }
         }

         TCP_SERVER_ALIAS_NAME_STRING = params->name;
         TCP_SERVER_FILE_STRING = params->file;
         TCP_SERVER_DIRBASE_STRING = params->dirbase;
         TCP_SERVER_IOSIZE_INT = params->IOsize * KB;

         return 0;
      }


   /* ................................................................... */

