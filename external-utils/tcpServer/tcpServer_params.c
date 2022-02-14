
/*
 *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 *
 *  This file is part of tcpServer.
 *
 *  tcpServer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  tcpServer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with tcpServer.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


   /* ... Include / Inclusion ........................................... */

      #include "tcpServer_params.h"


   /* ... Functions / Funciones ......................................... */

      void params_show_values ( struct tcpServer_param_st *params )
      {
         printf("Usage:\n");
         printf("\t-n <string>:\t%s\n",params->name);
         printf("\t-p <int>:\t%d\n",params->port);
         printf("\t-io <int>:\t%d\n",params->IOsize);
         printf("\t-f <string>:\t%s\n",params->file);
         printf("\t-d <string>:\t%s\n",params->dirbase);

         if(params->thread_mode == TH_POOL)
            printf("\t-pt:\t\tThread Pool Activated\n") ;
         if(params->thread_mode == TH_OP)
            printf("\t-pt:\t\tThread Pool Deactivated (Using Thread per Client)\n") ;
      }

      void params_show_usage ( void )
      {
         printf("Usage:\n");
         printf("\t-n <string>: name of the server\n");
         printf("\t-p <int>: port number\n");
         printf("\t-io <int>: IOsize\n");
         printf("\t-f <string>: name of the DNS file\n");
         printf("\t-d <string>: dir base\n");
         printf("\t-pt: use thread pool\n") ;
      }

      int params_get ( int argc, char *argv[], struct tcpServer_param_st *params )
      {
         int i;

         params->port   = TCPSERVER_PORT_DEFAULT;
         params->IOsize    = TCPSERVER_IOSIZE_DEFAULT;
         gethostname(params->name, 255);
         strcpy(params->file, TCPSERVER_FILE_DEFAULT);
         strcpy(params->dirbase, TCPSERVER_DIRBASE_DEFAULT);
         params->thread_mode = TH_OP ;

         for (i=0;i<argc;i++)
         {
            switch(argv[i][0])
            {
               case '-':
                  switch(argv[i][1])
                  {
                     case 'p':
                        if ((strlen(argv[i]) == 3) && (argv[i][2] == 't')){
                           params->thread_mode = TH_POOL;
                        }
                        else if (strlen(argv[i]) == 2){
                           params->port = atoi(argv[i+1]);
                        }
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

         TCPSERVER_ALIAS_NAME_STRING = params->name;
         TCPSERVER_FILE_STRING = params->file;
         TCPSERVER_DIRBASE_STRING = params->dirbase;
         TCPSERVER_IOSIZE_INT = params->IOsize * KB;

         return 0;
      }


   /* ................................................................... */

