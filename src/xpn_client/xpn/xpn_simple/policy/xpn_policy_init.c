
/*
 *  Copyright 2000-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Dario Muñoz Muñoz
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

     #include "xpn/xpn_simple/xpn_policy_init.h"


  /* ... Global variables / Variables globales ......................... */

     extern struct xpn_partition xpn_parttable[XPN_MAX_PART];


  /* ... Functions / Funciones ......................................... */

int XpnInitServer ( struct conf_file_data *conf_data, struct xpn_partition * part, struct nfi_server * serv, int server_num )
{
    int ret;
    char prt[PROTOCOL_MAXLEN];
    char url_buf[KB];

    ret = XpnConfGetServer(conf_data, url_buf, part->id, server_num);
    if (ret != 0)
        return -1;

    serv -> block_size = part -> block_size; // Reference of the partition blocksize
    XPN_DEBUG("url=%s", url_buf);

    ret = ParseURL(url_buf, prt, NULL, NULL, NULL, NULL, NULL);
    if (ret < 0) {
        errno = EINVAL;
        return -1;
    }

    // crear conexion
    if (strcmp(prt, "file") == 0) {
        //printf("[XPN]nfi_local_init: %s\n",url);
        ret = nfi_local_init(url_buf, serv, NULL);
        if (ret < 0) {
            errno = ESRCH;
            return -1;
        }
    }
    #ifdef ENABLE_MPI_SERVER
    else if (strcmp(prt, "mpi_server") == 0) {
        ret = nfi_xpn_server_init(url_buf, serv, XPN_SERVER_TYPE_MPI);
        if (ret < 0) {
            errno = ESRCH;
            return -1;
        }
    }
    #endif

    #ifdef ENABLE_SCK_SERVER
    else if ( (strcmp(prt, "sck_server") == 0) || (strcmp(prt, "mq_server") == 0) ) {
        ret = nfi_xpn_server_init(url_buf, serv, XPN_SERVER_TYPE_SCK);
        if (ret < 0) {
            errno = ESRCH;
            return -1;
        }
    }
    #endif

    #ifdef ENABLE_NFS
    else if ((strcmp(prt, "nfs") == 0) || (strcmp(prt, "nfs2") == 0)) {
        //printf("[XPN]nfi_nfs_init: %s\n",url);
        ret = nfi_nfs_init(url_buf, serv, NULL);
        if (ret < 0) {
            errno = ESRCH;
            return -1;
        }
    }
    #endif

    #ifdef ENABLE_NFS3
    else if (strcmp(prt, "nfs3") == 0) {
        //printf("[XPN]nfi_nfs3_init: %s\n",url);
        ret = nfi_nfs3_init(url_buf, serv, NULL);
        if (ret < 0) {
            errno = ESRCH;
            return -1;
        }
    }
    #endif

    else {
        printf("[XPN] Protocol '%s' not recognized\n", prt);
        errno = EINVAL;
        return -1;
    }

    // Default Value
    return 1;
}

/* return partition's id */
int XpnGetPartition(char * path)
{
    int i;
    char part[PATH_MAX];

    getNamePart(part, path);

    i = 0;
    while ((i < XPN_MAX_PART) && (strcmp(part, xpn_parttable[i].name) != 0)) {
        i++;
    }

    if (i == XPN_MAX_PART) {
        fprintf(stderr, "Error: Partition '%s' does not exist in the conf file.\n", part);
        return -1;
    }

    return xpn_parttable[i].id;
}

struct xpn_partition * XpnSearchPart(int pd)
{
    int i = 0;

    while ((i < XPN_MAX_PART) && (xpn_parttable[i].id != pd)) {
        i++;
    }

    if (i == XPN_MAX_PART) {
        return NULL;
    }

    return & (xpn_parttable[i]);
}

void XpnShowPartitionTable(void)
{
    int i = 0;

    while ((i < XPN_MAX_PART) && (xpn_parttable[i].name != NULL) && (strcmp("", xpn_parttable[i].name) != 0)) {
        printf("[XPN]xpn_parttable[%d].name = %s\n", i, xpn_parttable[i].name);
        i++;
    }
}


  /* ...................................................................... */

