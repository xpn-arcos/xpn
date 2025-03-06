
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

   #include "base/ns.h"


/* ... Functions / Funciones ......................................... */

void ns_get_hostname (char *srv_name)
{
  debug_info("[NS] [ns_get_hostname] >> Begin\n");

  gethostname(srv_name, HOST_NAME_MAX); // get hostname

  debug_info("[NS] [ns_get_hostname] >> End\n");
}

char *ns_get_host_ip (void)
{
  char   *ip;
  char   srv_name[HOST_NAME_MAX];
  struct hostent *srv_entry;

  debug_info("[NS] [ns_get_host_ip] >> Begin\n");

  gethostname(srv_name, HOST_NAME_MAX);                           // get hostname
  srv_entry = gethostbyname(srv_name);                            // find host information
  ip = inet_ntoa(*((struct in_addr *)srv_entry->h_addr_list[0])); // Convert into IP string

  debug_info("[NS] [ns_get_host_ip] srv_name: %s IP: %s\n", srv_name, ip);

  debug_info("[NS] [ns_get_host_ip] >> End\n");

  return ip;
}

int ns_publish (char *dns_file, char *protocol, char *param_srv_name, char *srv_ip, char *port_name)
{
  int ret;
  FILE *dns_fd;

  debug_info("[NS] [ns_publish] >> Begin\n");

  dns_fd = fopen(dns_file, "a+");
  if (NULL == dns_fd)
  {
    perror("[NS] [ns_publish] ERROR: fopen on DNS File");
    return -1;
  }

  ret = fprintf(dns_fd, "%s:%s %s:%s %s\n", protocol, param_srv_name, protocol, srv_ip, port_name);
  if (ret < 0)
  {
    fclose(dns_fd);
    perror("[NS] [ns_publish] ERROR: fprintf on DNS File");
    return -1;
  }

  fclose(dns_fd);

  debug_info("[NS] [ns_publish] >> End\n");
  return 0;
}

int ns_unpublish (char *dns_file, char *protocol, char *param_srv_name)
{
  FILE *dns_fd;
  FILE *new_dns_fd;
  char new_dns_file[PATH_MAX];
  int found = 0;
  char aux_name[1024];
  char aux_name_2[1024];
  char port_name[HOST_NAME_MAX];

  debug_info("[NS] [ns_unpublish] >> Begin\n");
  int res = 0;

  // open files
  dns_fd = fopen(dns_file, "r");
  if (NULL == dns_fd)
  {
    perror("[NS] [ns_unpublish] ERROR: DNS File");
    return -1;
  }

  sprintf(new_dns_file, "%saux%d", dns_file, rand());

  new_dns_fd = fopen(new_dns_file, "w");
  if (NULL == new_dns_fd)
  {
    fclose(dns_fd);
    perror("[NS] [ns_unpublish] ERROR: New DNS File");
    return -1;
  }

  char aux_srv_name[2 * HOST_NAME_MAX];
  sprintf(aux_srv_name, "%s:%s", protocol, param_srv_name);

  // copy filtering...
  while (fscanf(dns_fd, "%1000s %1000s %100s", aux_name, aux_name_2, port_name) != EOF)
  {
    if (strcmp(aux_name, aux_srv_name) == 0)
    {
      // Not copy the line
      found = 1;
    }
    else
    {
      // Copy the line
      fprintf(new_dns_fd, "%s %s %s\n", aux_name, aux_name_2, port_name);
    }
  }

  if (0 == found) {
    printf("Warning: Server %s not found\n", aux_name);
  }

  // close files
  fclose(new_dns_fd);
  fclose(dns_fd);

  unlink(dns_file);
  res = rename(new_dns_file, dns_file);
  if (res != 0){
    debug_error("Error: in rename %s\n",strerror(errno));
  }
  
  debug_info("[NS] [ns_unpublish] >> End\n");

  return res;
}

int ns_lookup (char *protocol, char *param_srv_name, char *srv_ip, char *port_name)
{
  int  found = 0;
  char aux_srv_name[1024];
  char prot_srv_name[1024];
  char aux_protocol[1024];
  char dns_file[PATH_MAX];
  FILE *dns_fd;

  debug_info("[NS] [ns_lookup] >> Begin\n");

  // try to get the ns_file_name
  char *dns_file_env = getenv("XPN_DNS");
  if (dns_file_env == NULL)
  {
    if (strcmp(protocol, "mpi_server") == 0) {
      strcpy(dns_file, MPI_SERVER_DNS_FILE_DEFAULT);
    }
    else if (strcmp(protocol, "sck_server") == 0) {
      strcpy(dns_file, SCK_SERVER_DNS_FILE_DEFAULT);
    }
    else if (strcmp(protocol, "mq_server") == 0) {
      strcpy(dns_file, MQ_SERVER_DNS_FILE_DEFAULT);
    }
    else {
      printf("Unrecognized protocol '%s' !!\n", protocol) ;
    }
  }
  else {
    strcpy(dns_file, dns_file_env);
  }

  // try to open the ns_file_fd
  dns_fd = fopen(dns_file, "r");
  if (dns_fd == NULL)
  {
    return -1;
  }

  sprintf(prot_srv_name, "%s:%s", protocol, param_srv_name);

  while (fscanf(dns_fd, "%1000s %[^:]:%100s %100s", aux_srv_name, aux_protocol, srv_ip, port_name) != EOF)
  {
    debug_info("[NS] [ns_lookup] %s %s:%s %s\n\n", aux_srv_name, aux_protocol, srv_ip, port_name);

    if (strcmp(aux_srv_name, prot_srv_name) == 0 || strcmp(srv_ip, param_srv_name) == 0)
    {
      found = 1;
      break;
    }
  }

  fclose(dns_fd);

  if (found == 0)
  {
    return -1;
  }

  debug_info("[NS] [ns_lookup] >> End\n");

  return 0;
}


/* ................................................................... */

