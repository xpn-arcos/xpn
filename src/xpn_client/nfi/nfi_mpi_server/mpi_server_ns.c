
  /*
   *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

  #include "mpi_server_ns.h"


  /* ... Functions / Funciones ......................................... */

  int mpi_server_dns_publish ( char * param_srv_name, char * dns_file, char * port_name )
  {
    char serv_name [HOST_NAME_MAX];
    gethostname(serv_name, HOST_NAME_MAX);
    sprintf(param_srv_name, "mpi_server.%s", serv_name) ;

    FILE * dns_fd = fopen(dns_file, "a");
    if (dns_fd == NULL)
    {
      perror("DNS File:");
      return -1;
    }

    fprintf(dns_fd, "%s %s\n", param_srv_name, port_name);

    fclose(dns_fd);

    return 0;
  }


  int mpi_server_dns_unpublish ( char * dns_file )
  {
    char serv_name    [HOST_NAME_MAX];
    char aux_srv_name [2*HOST_NAME_MAX];

    gethostname(serv_name, HOST_NAME_MAX);
    sprintf(aux_srv_name, "mpi_server.%s", serv_name) ;

    FILE * dns_fd = fopen(dns_file, "r");
    if (dns_fd == NULL)
    {
      perror("DNS File:");
      return -1;
    }

    char new_dns_file [PATH_MAX];
    strcpy(new_dns_file, "xpn_dns_XXXXXX");
    int new_dns_poxis_fd = mkstemp(new_dns_file);

    FILE * new_dns_fd = fdopen(new_dns_poxis_fd, "w");
    if (new_dns_fd == NULL)
    {
      fclose(dns_fd) ;
      perror("New DNS File:");
      return -1;
    }

    int found = 0;
    char aux_name[1024];
    char port_name [HOST_NAME_MAX];
    while(fscanf(dns_fd, "%s %s", aux_name, port_name) != EOF)
    {
      if (strcmp(aux_name, aux_srv_name) == 0)
      { //Not copy the line
        found = 1;
      }
      else
      { //Copy the line
        fprintf(new_dns_fd, "%s %s\n", aux_name, port_name);
      }
    }

    if (found == 0) {
        printf("Warning: Server %s not found\n", aux_name);
    }

    fclose(new_dns_fd);
    fclose(dns_fd);

    unlink(dns_file);
    rename(new_dns_file, dns_file);

    return 0;
  }


  int mpi_server_dns_lookup ( char * param_srv_name, char * port_name )
  {
    int    found = 0 ;
    char   srv_name[1024] ;
    char   aux_name[1024] ;
    char * dns_file ;
    FILE * dns_fd ;

    // try to get the ns_file_name
    dns_file = getenv("XPN_DNS");
    if (dns_file == NULL)
    {
      printf("DNS File not defined (XPN_DNS is empty)\n");
      return -1;
    }

    // try to open the ns_file_fd
    dns_fd = fopen(dns_file, "r");
    if (dns_fd == NULL)
    {
      perror("DNS File:");
      return -1;
    }

    sprintf(srv_name, "mpi_server.%s", param_srv_name);
    while(fscanf(dns_fd, "%s %s", aux_name, port_name) != EOF)
    {
      if (strcmp(aux_name, srv_name) == 0)
      {
        found = 1;
        break;
      }
    }

    if (found == 0)
    {
      fclose(dns_fd);
      return -1;
    }
  
    fclose(dns_fd);
    return 0;
  }


  /* ................................................................... */

