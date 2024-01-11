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
  #include "base/ns.h"
  #include "tcp_server/tcp_server_params.h"
  #include "tcp_server/tcp_server_ops.h"
  #include "tcp_server/tcp_server_comm.h"
  #include "tcp_server/tcp_server_d2xpn.h"


  /* ... Global variables / Variables globales ......................... */

  tcp_server_param_st params;
  worker_t worker;
  int the_end = 0;
  char serv_name[HOST_NAME_MAX];


  /* ... Auxiliar Functions / Funciones Auxiliares ..................... */

void tcp_server_run(struct st_th th)
{
  debug_info("[TCP-SERVER] (ID=%d): begin to do operation '%s' OP_ID %d\n", th.id, tcp_server_op2string(th.type_op), th.type_op);

  tcp_server_do_operation( & th, & the_end);

  debug_info("[TCP-SERVER] (ID=%d) end to do operation '%s'\n", th.id, tcp_server_op2string(th.type_op));
}

void tcp_server_dispatcher(struct st_th th)
{
  int ret;
  int disconnect;
  struct st_th th_arg;

  // check params...
  if (NULL == th.params)
  {
    printf("[WORKERS ID=%d] ERROR: NULL arguments", th.id);
    return;
  }

  disconnect = 0;
  while (! disconnect)
  {
    ret = tcp_server_comm_read_operation(th.params, (int) th.sd, (char * ) & (th.type_op), 1, & (th.rank_client_id));

    if (ret < 0) {
      debug_info("[TCP-SERVER] ERROR: tcp_server_comm_readdata fail\n");
      return;
    }

    if (ret == 0) {
      debug_info("[TCP-SERVER] WARNING: tcp_server_comm_readdata broken pipe\n");
      return;
    }

    if (th.type_op == TCP_SERVER_DISCONNECT)
    {
      debug_info("[TCP-SERVER] INFO: DISCONNECT received\n");
      disconnect = 1;
      continue;
    }

    if (th.type_op == TCP_SERVER_FINALIZE)
    {
      debug_info("[TCP-SERVER] INFO: FINALIZE received\n");
      disconnect = 1;
      the_end = 1;
      continue;
    }

    // Launch worker per operation
    th_arg.params         = & params;
    th_arg.sd             = (int) th.sd;
    th_arg.function       = tcp_server_run;
    th_arg.type_op        = th.type_op;
    th_arg.rank_client_id = th.rank_client_id;
    th_arg.wait4me        = FALSE;

    tcp_server_run(th_arg) ;
  }

  debug_info("[TCP-SERVER] tcp_server_worker_run (ID=%d) close\n", th.rank_client_id);

  tcp_server_comm_close((int) th.sd);
}


/* ... Functions / Funciones ......................................... */

int tcp_server_up(void)
{
  int ret;
  struct st_th th_arg;
  int sd;

  // Feedback
  printf("\n");
  printf(" -------------------\n");
  printf(" > Starting servers... (%s)\n", serv_name);
  printf(" -------------------\n");
  printf("\n");

  signal(SIGPIPE, SIG_IGN);

  // Initialize
  debug_msg_init();
  ret = tcp_server_comm_init( & params);
  if (ret < 0) {
    printf("[TCP-SERVER] ERROR: tcp_comm initialization fails\n");
    return -1;
  }
  ret = base_workers_init(&worker, params.thread_mode);
  if (ret < 0) {
    printf("[TCP-SERVER] ERROR: workers initialization fails\n");
    return -1;
  }

  // Initialize semaphore for server disks
  /*ret = sem_init( & (params.disk_sem), 0, 1);
  if (ret < 0) {
    printf("[TCP-SERVER] ERROR: semaphore initialization fails\n");
    return -1;
  }*/

  // Initialize semaphore for clients
  /*sprintf(params.sem_name_server, "%s%d", serv_name, getpid());
  sem_t *sem_server = sem_open(params.sem_name_server, O_CREAT, 0777, 1);
  if (sem_server == 0) {
    printf("[TCP-SERVER] ERROR: semaphore open fails\n");
    return -1;
  }*/

  // Loop: receiving + processing
  the_end = 0;
  while (!the_end)
  {
    debug_info("[TCP-SERVER] tcp_server_accept_comm()\n");

    params.client = 0;
    sd = tcp_server_comm_accept(& params);
    if (sd < 0) {
      continue;
    }

    //Launch dispatcher per application
    th_arg.params = & params;
    th_arg.sd = sd;
    th_arg.function = tcp_server_dispatcher;
    th_arg.type_op = 0;
    th_arg.rank_client_id = 0;
    th_arg.wait4me = FALSE;

    base_workers_launch( & worker, & th_arg, tcp_server_dispatcher);
  }

  // Wait and finalize for all current workers
  debug_info("[TCP-SERVER] workers_destroy\n");
  base_workers_destroy( & worker);
  debug_info("[TCP-SERVER] tcp_server_comm_destroy\n");
  tcp_server_comm_destroy( & params);

  // Close semaphores
  //sem_destroy( & (params.disk_sem));
  //sem_unlink(params.sem_name_server);

  // return OK
  return 0;
}

int tcp_server_down( void )
{
  int ret, sd, data;
  char srv_name[1024];
  char server_name[1024];
  char port_number[1024];
  FILE *file;

  // Feedback
  printf("\n");
  printf(" -------------------\n");
  printf(" Stopping servers... (%s)\n", serv_name);
  printf(" -------------------\n");
  printf("\n");

  // Initialize
  debug_msg_init();
  ret = tcp_server_comm_init(&params);
  if (ret < 0)
  {
  printf("[TCP-SERVER] ERROR: tcp_comm initialization fails\n");
  return -1;
  }
  ret = base_workers_init(&worker, params.thread_mode);
  if (ret < 0)
  {
  printf("[TCP-SERVER] ERROR: workers initialization fails\n");
  return -1;
  }

  // Open host file
  file = fopen(params.dns_file, "r");
  if (file == NULL)
  {
  printf("[TCP-SERVER] ERROR: invalid file %s\n", params.shutdown_file);
  return -1;
  }

  while (fscanf(file, "%s %s %s", srv_name, server_name, port_number) != EOF)
  {
  // Lookup port name
  ret = ns_lookup("tcp_server", srv_name, server_name, port_number);
  if (ret < 0)
  {
    printf("[TCP-SERVER] ERROR: server %s %s %s not found\n", srv_name, server_name, port_number);
    continue;
  }

  // Connect with server
  sd = tcp_server_comm_connect(&params, server_name, atoi(port_number));
  if (sd < 0)
  {
    printf("[TCP-SERVER] ERROR: connect to %s failed\n", server_name);
    continue;
  }

  // Send shutdown request
  data = TCP_SERVER_FINALIZE;
  ret = tcp_server_comm_write_data(&params, sd, (char *)&data, sizeof(int), 0); // 0: rank_client_id
  if (ret < 0)
  {
    printf("[TCP-SERVER] ERROR: write SERVER_FINALIZE to %s failed\n", srv_name);
    return -1;
  }

  // Close
  tcp_server_comm_close(sd);
  }

  // Close host file
  fclose(file);

  // Wait and finalize for all current workers
  debug_info("[TCP-SERVER] base_workers_destroy\n");
  base_workers_destroy(&worker);
  debug_info("[TCP-SERVER] tcp_server_comm_destroy\n");
  tcp_server_comm_destroy(&params);

  return 0;
}


/*
 * Main
 */

int main(int argc, char * argv[])
{
  int    ret = -1;
  char * exec_name = NULL;

  // Initializing...
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  // Welcome...
  printf("\n");
  printf(" tcp_server\n");
  printf(" ----------\n");
  printf("\n");
  printf(" > Begin.\n");
  printf("\n");

  // Get arguments..
  ret = tcp_server_params_get(&params, argc, argv);
  if (ret < 0)
  {
    tcp_server_params_show_usage();
    return -1;
  }

  // Show configuration...
  exec_name = basename(argv[0]);
  printf("   * action=%s\n", exec_name);
  gethostname(serv_name, HOST_NAME_MAX);
  printf("   * host=%s\n",   serv_name);
  tcp_server_params_show(&params);

  // Do associate action...
  if (strcasecmp(exec_name, "xpn_stop_tcp_server") == 0) {
    ret = tcp_server_down();
  }
  else {
    ret = tcp_server_up();
  }

  return ret;
}


/* ................................................................... */

