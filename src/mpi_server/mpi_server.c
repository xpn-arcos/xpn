
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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

#define DEBUG
/* ... Include / Inclusion ........................................... */

#include "all_system.h"
#include "base/utils.h"
#include "base/workers.h"
#include "mpi_server_ops.h"
#include "mpi_server_comm.h"
#include "mpi_server_d2xpn.h"
#include "mpi_server_params.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */

char                serv_name [HOST_NAME_MAX];
mpi_server_param_st params;
worker_t            worker1, worker2;
int                 the_end = 0;


/* ... Auxiliar Functions / Funciones Auxiliares ..................... */

void mpi_server_run ( struct st_th th )
{
  debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_run] >> Begin: OP '%s'; OP_ID %d\n", th.id, mpi_server_op2string(th.type_op), th.type_op);

  mpi_server_do_operation ( &th,  &the_end );

  debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_run] << End: OP:'%s'\n", th.id, mpi_server_op2string(th.type_op));
}

void mpi_server_dispatcher ( struct st_th th )
{
  int ret;

  debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_dispatcher] >> Begin\n", th.id);

  // check params...
  if (NULL == th.params)
  {
    debug_error("[TH_ID=%d] [MPI_SERVER] [mpi_server_dispatcher] ERROR: NULL arguments\n", th.id);
    return;
  }

  struct st_th th_arg;
  int disconnect = 0;

  while (!disconnect)
  {
    debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_dispatcher] Waiting for operation\n", th.id);

    ret = mpi_server_comm_read_operation((mpi_server_param_st *) th.params, (MPI_Comm)th.sd, &(th.type_op), &(th.rank_client_id), &(th.tag_client_id));
    if (ret < 0)
    {
      debug_error("[TH_ID=%d] [MPI_SERVER] [mpi_server_dispatcher] ERROR: read operation fail\n", th.id);
      return;
    }

    debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_dispatcher] OP '%s'; OP_ID %d\n", th.id, mpi_server_op2string(th.type_op), th.type_op);

    if (th.type_op == MPI_SERVER_DISCONNECT)
    {
      debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_dispatcher] DISCONNECT received\n", th.id);

      disconnect = 1;
      continue;
    }

    if (th.type_op == MPI_SERVER_FINALIZE)
    {
      debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_dispatcher] FINALIZE received\n", th.id);

      disconnect = 1;
      the_end = 1;
      continue;
    }

    // Launch worker per operation
    th_arg.params         = &params;
    th_arg.sd             = (long) th.sd;
    th_arg.function       = mpi_server_run;
    th_arg.type_op        = th.type_op;
    th_arg.rank_client_id = th.rank_client_id;
    th_arg.tag_client_id  = th.tag_client_id;
    th_arg.wait4me        = FALSE;

    base_workers_launch ( &worker2, &th_arg, mpi_server_run );

    debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_dispatcher] Worker launched\n", th.id);
  }

  debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_dispatcher] Client %d close\n", th.id, th.rank_client_id);

  mpi_server_comm_disconnect( (MPI_Comm)(th.sd) );

  debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_dispatcher] End\n", th.id);
}


/* ... Functions / Funciones ......................................... */

//Start servers
int mpi_server_up ( void )
{
  MPI_Comm     sd;
  struct st_th th_arg;
  int          ret;

  debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_up] >> Begin\n", 0);

  printf("\n");
  printf(" ----------------\n");
  printf(" Starting servers (%s)\n", serv_name);
  printf(" ----------------\n");
  printf("\n");

  // Initialize server

  // mpi_comm initialization
  debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_up] mpi_comm initialization\n", 0);

  ret = mpi_server_comm_init(&params);
  if (ret < 0)
  {
    printf("[TH_ID=%d] [MPI_SERVER] [mpi_server_up] ERROR: mpi_comm initialization fails\n", 0);
    return -1;
  }

  // Workers initialization
  debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_up] Workers initialization\n", 0);

  ret = base_workers_init( &worker1, params.thread_mode );
  if (ret < 0)
  {
    printf("[TH_ID=%d] [MPI_SERVER] [mpi_server_up] ERROR: Workers initialization fails\n", 0);
    return -1;
  }

  ret = base_workers_init( &worker2, params.thread_mode );
  if (ret < 0)
  {
    printf("[TH_ID=%d] [MPI_SERVER] [mpi_server_up] ERROR: Workers initialization fails\n", 0);
    return -1;
  }

  // Loop: receiving + processing
  the_end = 0;
  int number_accepted = 0;
  while (!the_end)
  {
    if ((params.number_accepts != -1) && (number_accepted >= params.number_accepts))
    {
       debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_up] number_accepted >= params->number_accepts\n", 0);
       the_end = 1;
       continue;
    }

    debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_up] Waiting for accept\n", 0);

    sd = mpi_server_comm_accept(&params);
    if (sd == MPI_COMM_NULL) {
      continue;
    }
    number_accepted++;

    debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_up] Accept received\n", 0);

    //Launch dispatcher per aplication
    th_arg.params         = &params;
    th_arg.sd             = (long)sd;
    th_arg.function       = mpi_server_dispatcher;
    th_arg.type_op        = 0;
    th_arg.rank_client_id = 0;
    th_arg.tag_client_id  = 0;
    th_arg.wait4me        = FALSE;

    base_workers_launch(&worker1, &th_arg, mpi_server_dispatcher);

    // struct timespec remaining, request = { 5, 100 };
    // nanosleep(&request, &remaining);

    debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_up] Dispatcher launched\n", 0);
  }

  // Wait and finalize for all current workers
  debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_up] Workers destroy\n", 0);

  base_workers_destroy(&worker1);
  base_workers_destroy(&worker2);

  debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_up] mpi_comm destroy\n", 0);

  mpi_server_comm_destroy(&params);

  return 0 ;
}

int mpi_server_down ( int argc, char *argv[] )
{
  int  ret, buf, res = 0;
  char port_name[MPI_MAX_PORT_NAME];
  char srv_name[1024] ;
  char dns_name[2048] ;
  MPI_Comm server;
  FILE *file;

  debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_down] >> Begin\n", 0);

  printf("\n");
  printf(" ----------------\n");
  printf(" Stopping servers (%s)\n", serv_name);
  printf(" ----------------\n");
  printf("\n");

  MPI_Init(&argc, &argv);
  MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

  // Open host file
  debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_down] Open host file %s\n", 0, params.shutdown_file);

  file = fopen(params.shutdown_file, "r");
  if (file == NULL) 
  {
    printf("[MPI-SERVER] ERROR: invalid file %s\n", params.shutdown_file);
    return -1;
  }

  while (fscanf(file, "%[^\n] ", srv_name) != EOF)
  {
    int version_len;
    char version[MPI_MAX_LIBRARY_VERSION_STRING];
    MPI_Get_library_version(version, &version_len);

    debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_down] MPI Version: %s\n", 0, version);

    if (strncasecmp(version, "Open MPI", strlen("Open MPI")) != 0)
    {
       // Lookup port name
      char aux_srv_ip[1024];

      debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_down] ns_lookup server %s\n", 0, srv_name);

      ret = ns_lookup("mpi_server", srv_name, aux_srv_ip, port_name);
      if (ret < 0)
      {
        printf("[TH_ID=%d] [MPI_SERVER] [mpi_server_down] INFO: server %s not found\n", 0, srv_name);
        continue;
      }
    }
    else
    {
      // Lookup port name on nameserver
      debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_down] MPI_Lookup_name server %s\n", 0, srv_name);

      ret = MPI_Lookup_name(srv_name, MPI_INFO_NULL, port_name);
      if (MPI_SUCCESS != ret)
      {
        printf("[TH_ID=%d] [MPI_SERVER] [mpi_server_down] INFO: server %s not found\n", 0, srv_name);
        continue;
      }
    }

    // Connect with servers
    debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_down] Connect with port %s\n", 0, port_name);
    MPI_Info info;
    MPI_Info_create( &info );
    int errclass, resultlen;
    char err_buffer[MPI_MAX_ERROR_STRING];
    MPI_Info_set(info, "timeout", "1");
    ret = MPI_Comm_connect( port_name, info, 0, MPI_COMM_SELF, &server );
    
    MPI_Error_class(ret,&errclass);
    MPI_Error_string(ret,err_buffer,&resultlen);
    printf("%s", err_buffer);
    MPI_Info_free(&info);
    if (MPI_ERR_PORT == errclass){
      printf("[MPI-SERVER] Info: Server %s down\n", srv_name) ;
      continue;
    }else if (MPI_SUCCESS != errclass) {
      printf("[MPI-SERVER] ERROR: MPI_Comm_connect fails\n") ;
      continue;
    }
    ret = mpi_server_comm_write_operation_finalize(server, MPI_SERVER_FINALIZE);
    if (ret < 0)
    {
      printf("[TH_ID=%d] [MPI_SERVER] [mpi_server_down] ERROR: Send finalize operation\n", 0);
      continue;
    }
    mpi_server_comm_disconnect(server);
  }
  // Close host file
  fclose(file);

  MPI_Finalize();
  debug_info("[TH_ID=%d] [MPI_SERVER] [mpi_server_down] >> End\n", 0);

  return 0;
}
int mpi_server_terminate ( int argc, char *argv[] )
{
  int  ret, res = 0;
  char port_name[MPI_MAX_PORT_NAME];
  MPI_Comm server;
  
  printf("\n");
  printf(" ----------------\n");
  printf(" Stopping server (%s)\n", serv_name);
  printf(" ----------------\n");
  printf("\n");

  MPI_Init(&argc, &argv);
  MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

  int version_len;
  char version[MPI_MAX_LIBRARY_VERSION_STRING];
  MPI_Get_library_version(version, &version_len);

  if (strncasecmp(version, "Open MPI", strlen("Open MPI")) != 0)
  {
    // Lookup port name
    char aux_srv_ip[1024];
    ret = ns_lookup("mpi_server", params.srv_name, aux_srv_ip, port_name);
    if (ret < 0)
    {
      printf("[MPI-SERVER] INFO: server %s not found\n", params.srv_name);
      // continue;
    }
  }
  else
  {
    // Lookup port name on nameserver
    ret = MPI_Lookup_name(params.srv_name, MPI_INFO_NULL, port_name) ;
    if (MPI_SUCCESS != ret) {
      printf("[MPI-SERVER] INFO: server %s not found\n", params.srv_name) ;
      // continue;
    }
  }

  debug_info("[MPI-SERVER] Connect to server %s\n", params.srv_name) ;
  // Connect with servers
  MPI_Info info;
  MPI_Info_create( &info );
  int errclass, resultlen;
  char err_buffer[MPI_MAX_ERROR_STRING];
  MPI_Info_set(info, "timeout", "1");
  ret = MPI_Comm_connect( port_name, info, 0, MPI_COMM_SELF, &server );
  
  MPI_Error_class(ret,&errclass);
  MPI_Error_string(ret,err_buffer,&resultlen);
  debug_info("%s", err_buffer);
  MPI_Info_free(&info);
  if (MPI_ERR_PORT == errclass){
    printf("[MPI-SERVER] Info: Server %s down\n", params.srv_name) ;
  }
  if (MPI_SUCCESS != errclass) {
    printf("[MPI-SERVER] ERROR: MPI_Comm_connect fails\n") ;
  }
  
  debug_info("[MPI-SERVER] SEND: MPI_SERVER_FINALIZE to server %s\n", params.srv_name) ;
  ret = mpi_server_comm_write_operation_finalize(server, MPI_SERVER_FINALIZE);
  if (ret < 0)
  {
    printf("[TH_ID=%d] [MPI_SERVER] [mpi_server_down] ERROR: Send finalize operation\n", 0);
  }

  debug_info("[MPI-SERVER] SEND finish: MPI_SERVER_FINALIZE to server %s\n", params.srv_name) ;
  MPI_Comm_disconnect( &server ); 
  
  MPI_Finalize();

  return 0;
}

//Main
int main ( int argc, char *argv[] )
{
  int    ret = -1;
  char * exec_name = NULL;

  // Initializing...
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  // Welcome...
  printf("\n");
  printf(" mpi_server\n");
  printf(" ----------\n");
  printf("\n");
  printf(" Begin.\n");
  printf("\n");

  // Get arguments..
  debug_info("[TH_ID=%d] [MPI_SERVER] [main] Get server params\n", 0);

  ret = mpi_server_params_get(&params, argc, argv);
  if (ret < 0)
  {
    mpi_server_params_show_usage();
    return -1;
  }

  // Show configuration...
  exec_name = basename(argv[0]);
  printf(" * action=%s\n", exec_name);
  gethostname(serv_name, HOST_NAME_MAX);
  printf(" * host=%s\n",   serv_name);
  mpi_server_params_show(&params);

  // Do associate action...
  if (strcasecmp(exec_name, "xpn_stop_mpi_server") == 0)
  {
    debug_info("[TH_ID=%d] [MPI_SERVER] [main] Down servers\n", 0);

    ret = mpi_server_down (argc, argv);
  }else if (strcasecmp(exec_name, "xpn_terminate_mpi_server") == 0) {
    debug_info("[TH_ID=%d] [MPI_SERVER] [main] Terminate server\n", 0);

    ret = mpi_server_terminate (argc, argv);
  }
  else
  {
    debug_info("[TH_ID=%d] [MPI_SERVER] [main] Up servers\n", 0);

    ret = mpi_server_up ();
  }

  return ret;
}


/* ................................................................... */
