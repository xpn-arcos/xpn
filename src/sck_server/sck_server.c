
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

#include "all_system.h"
#include "base/utils.h"
#include "base/workers.h"
#include "sck_server/sck_server_ops.h"
#include "sck_server/sck_server_comm.h"
#include "sck_server/sck_server_d2xpn.h"
#include "sck_server/sck_server_params.h"
//#include "base/ns.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ......................... */

char                serv_name [HOST_NAME_MAX];
sck_server_param_st params;
worker_t            worker1, worker2;
int                 the_end = 0;


/* ... Auxiliar Functions / Funciones Auxiliares ..................... */

void sck_server_run ( struct st_th th )
{
  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_run] >> Begin: OP '%s'; OP_ID %d\n", th.id, sck_server_op2string(th.type_op), th.type_op);

  sck_server_do_operation( &th, &the_end);

  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_run] << End: OP:'%s'\n", th.id, sck_server_op2string(th.type_op));
}

void sck_server_dispatcher ( struct st_th th )
{
  int ret;

  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_dispatcher] >> Begin\n", th.id);

  // check params...
  if (NULL == th.params) {
    debug_error("[TH_ID=%d] [SCK_SERVER] [sck_server_dispatcher] ERROR: NULL arguments\n", th.id);
    return;
  }
  
  struct st_th th_arg;
  int disconnect = 0;

  while (!disconnect)
  {
    debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_dispatcher] Waiting for operation\n", th.id);

    ret = sck_server_comm_read_operation((sck_server_param_st *) th.params, (int) th.sd, (char *)&(th.type_op), 1, &(th.rank_client_id));
    if (ret < 0)
    {
      debug_error("[TH_ID=%d] [SCK_SERVER] [sck_server_dispatcher] ERROR: read operation fail\n", th.id);
      return;
    }

    debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_dispatcher] OP '%s'; OP_ID %d\n", th.id, sck_server_op2string(th.type_op), th.type_op);

    if (ret == 0)
    {
      debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_dispatcher] Broken pipe\n", th.id);
      return;
    }

    if (th.type_op == SCK_SERVER_DISCONNECT)
    {
      debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_dispatcher] DISCONNECT received\n", th.id);

      disconnect = 1;
      continue;
    }

    if (th.type_op == SCK_SERVER_FINALIZE)
    {
      debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_dispatcher] FINALIZE received\n", th.id);

      disconnect = 1;
      the_end = 1;
      continue;
    }

    // Launch worker per operation
    th_arg.params         = & params;
    th_arg.sd             = (int) th.sd;
    th_arg.function       = sck_server_run;
    th_arg.type_op        = th.type_op;
    th_arg.rank_client_id = th.rank_client_id;
    th_arg.wait4me        = FALSE;

    //sck_server_run(th_arg); //TODO
    base_workers_launch ( &worker2, &th_arg, sck_server_run );

    debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_dispatcher] Worker launched\n", th.id);
  }

  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_dispatcher] Client %d close\n", th.id, th.rank_client_id);

  sck_server_comm_close((int) (th.sd));

  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_dispatcher] End\n", th.id);
}


/* ... Functions / Funciones ......................................... */

//Start servers
int sck_server_up ( void )
{
  int          ret;
  struct st_th th_arg;
  int          sd;

  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_up] >> Begin\n", 0);

  printf("\n");
  printf(" -------------------\n");
  printf(" Starting servers (%s)\n", serv_name);
  printf(" -------------------\n");
  printf("\n");

  //Ignore broken pipe signal
  signal(SIGPIPE, SIG_IGN);

  // Initialize server

  // socket initialization
  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_up] socket initialization\n", 0);

  ret = sck_server_comm_init(&params);
  if (ret < 0)
  {
    printf("[TH_ID=%d] [SCK_SERVER] [sck_server_up] ERROR: socket initialization fails\n", 0);
    return -1;
  }

  // Workers initialization
  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_up] Workers initialization\n", 0);

  ret = base_workers_init(&worker1, params.thread_mode);
  if (ret < 0)
  {
    printf("[TH_ID=%d] [SCK_SERVER] [sck_server_up] ERROR: Workers initialization fails\n", 0);
    return -1;
  }

  ret = base_workers_init(&worker2, params.thread_mode);
  if (ret < 0)
  {
    printf("[TH_ID=%d] [SCK_SERVER] [sck_server_up] ERROR: Workers initialization fails\n", 0);
    return -1;
  }

  // Initialize semaphore for server disks
  /*
  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_up] Semaphore for server disks initialization\n", 0);

  ret = sem_init( & (params.disk_sem), 0, 1);
  if (ret < 0)
  {
    printf("[TH_ID=%d] [SCK_SERVER] [sck_server_up] ERROR: Semaphore for server disks initialization fails\n", 0);
    return -1;
  }
  */

  // Initialize semaphore for clients
  /*
  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_up] semaphore for clients initialization\n", 0);

  sprintf(params.sem_name_server, "%s%d", serv_name, getpid());
  sem_t *sem_server = sem_open(params.sem_name_server, O_CREAT, 0777, 1);
  if (sem_server == 0)
  {
    printf("[TH_ID=%d] [SCK_SERVER] [sck_server_up] ERROR: Semaphore for clients initialization fails\n", 0);
    return -1;
  }
  */

  // Loop: receiving + processing
  the_end = 0;
  while (!the_end)
  {
    debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_up] Waiting for accept\n", 0);

    params.client = 0;

    sd = sck_server_comm_accept(& params);
    if (sd < 0) {
      continue;
    }

    debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_up] Accept received\n", 0);

    //Launch dispatcher per application
    th_arg.params         = &params;
    th_arg.sd             = sd;
    th_arg.function       = sck_server_dispatcher;
    th_arg.type_op        = 0;
    th_arg.rank_client_id = 0;
    th_arg.wait4me        = FALSE;

    base_workers_launch( &worker1, &th_arg, sck_server_dispatcher );

    debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_up] Dispatcher launched\n", 0);
  }

  // Wait and finalize for all current workers
  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_up] Workers destroy\n", 0);

  base_workers_destroy(&worker1);
  base_workers_destroy(&worker2);

  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_up] socket destroy\n", 0);

  sck_server_comm_destroy(&params);

  // Close semaphores
  /*
  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_up] Semaphore for server disks destroy\n", 0);

  sem_destroy( & (params.disk_sem));

  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_up] Semaphore for clients destroy\n", 0);

  sem_unlink(params.sem_name_server);
  */

  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_up] >> End\n", 0);

  return 0;
}

// Stop servers
int sck_server_down( void ) //TODO
{
  int  ret, sd, data;
  char srv_name[1024];
  char server_name[1024];
  char port_number[1024];
  FILE *file;

  debug_info("[TH_ID=%d] [SCK_SERVER] [sck_server_down] >> Begin\n", 0);

  printf("\n");
  printf(" -------------------\n");
  printf(" Stopping servers... (%s)\n", serv_name);
  printf(" -------------------\n");
  printf("\n");

  // Initialize
  debug_msg_init();
  ret = sck_server_comm_init(&params);
  if (ret < 0)
  {
      printf("[SCK_SERVER] ERROR: sck_comm initialization fails\n");
      return -1;
  }

  ret = base_workers_init(&worker1, params.thread_mode);
  if (ret < 0)
  {
      printf("[SCK_SERVER] ERROR: workers1 initialization fails\n");
      return -1;
  }

  ret = base_workers_init(&worker1, params.thread_mode);
  if (ret < 0)
  {
      printf("[SCK_SERVER] ERROR: workers2 initialization fails\n");
      return -1;
  }


  // Open host file
  file = fopen(params.dns_file, "r");
  if (file == NULL)
  {
      printf("[SCK_SERVER] ERROR: invalid file %s\n", params.shutdown_file);
      return -1;
  }

  while (fscanf(file, "%s %s %s", srv_name, server_name, port_number) != EOF)
  {
	  // Lookup port name
	  ret = ns_lookup("sck_server", srv_name, server_name, port_number);
	  if (ret < 0)
	  {
	    printf("[SCK_SERVER] ERROR: server %s %s %s not found\n", srv_name, server_name, port_number);
	    continue;
	  }

	  // Connect with server
	  sd = sck_server_comm_connect(&params, server_name, atoi(port_number));
	  if (sd < 0)
	  {
	    printf("[SCK_SERVER] ERROR: connect to %s failed\n", server_name);
	    continue;
	  }

	  // Send shutdown request
	  data = SCK_SERVER_FINALIZE;
	  ret = sck_server_comm_write_data(&params, sd, (char *)&data, sizeof(int), 0); // 0: rank_client_id  (TODO!)
	  if (ret < 0)
	  {
	    printf("[SCK_SERVER] ERROR: write SERVER_FINALIZE to %s failed\n", srv_name);
	    return -1;
	  }

	  // Close
	  sck_server_comm_close(sd);
  }

  // Close host file
  fclose(file);

  // Wait and finalize for all current workers
  debug_info("[SCK_SERVER] base_workers_destroy\n");
  base_workers_destroy(&worker1);
  base_workers_destroy(&worker2);

  debug_info("[SCK_SERVER] sck_server_comm_destroy\n");
  sck_server_comm_destroy(&params);

  return 0;
}

//Main
int main( int argc, char *argv[] )
{
  int    ret = -1;
  char * exec_name = NULL;

  // Initializing...
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  // Welcome...
  printf("\n");
  printf(" sck_server\n");
  printf(" ----------\n");
  printf("\n");
  printf(" Begin.\n");
  printf("\n");

  // Get arguments..
  debug_info("[TH_ID=%d] [SCK_SERVER] [main] Get server params\n", 0);

  ret = sck_server_params_get(&params, argc, argv);
  if (ret < 0)
  {
    sck_server_params_show_usage();
    return -1;
  }

  // Show configuration...
  exec_name = basename(argv[0]);
  printf(" * action=%s\n", exec_name);
  gethostname(serv_name, HOST_NAME_MAX);
  printf(" * host=%s\n",   serv_name);
  sck_server_params_show(&params);

  // Do associate action...
  if (strcasecmp(exec_name, "xpn_stop_sck_server") == 0)
  {
    debug_info("[TH_ID=%d] [SCK_SERVER] [main] Down servers\n", 0);

    ret = sck_server_down();
  }
  else
  {
    debug_info("[TH_ID=%d] [SCK_SERVER] [main] Up servers\n", 0);

    ret = sck_server_up();
  }

  return ret;
}


/* ................................................................... */

