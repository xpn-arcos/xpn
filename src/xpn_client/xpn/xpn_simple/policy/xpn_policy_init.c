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


#include "xpn/xpn_simple/xpn_policy_init.h"


extern struct xpn_partition xpn_parttable[XPN_MAX_PART];


char * param_get(char * key)
{
    char * ret = NULL;

    /* get value */
    if (NULL != key) {
        ret = getenv(key);
    }

    /* return final value */
    return ret;
}

int XpnConfGetValueRept ( struct conf_file_data *conf_data, char *key, char *value, int partition, int rept )
{
    char key_buf[KB];
    char value_buf[KB];
    int i;
    int part_index = -1;
    int internal_rept = 0;

    for (i = 0; i < conf_data->lines_n; i++)
    {
        sscanf(conf_data->lines[i], "%s = %s", key_buf, value_buf);
        if (strcmp(key_buf, XPN_CONF_TAG_PARTITION) == 0)
        {
            part_index++;
        }
        if (strcmp(key_buf, key) == 0)
        {
            if (partition == part_index)
            {
                if (rept == internal_rept)
                {
                    strcpy(value, value_buf);
                    return 0;
                }

                internal_rept++;
            }
        }
    }
    return -1;
}

int XpnConfGetServer(struct conf_file_data *conf_data, char *value, int partition, int server)
{   
    if (partition >= conf_data->partition_n)
        return -1;
    
    char key_buf[KB];
    int server_index = 0;
    for (int i = 0; i < partition; i++)
    {   
        server_index += conf_data->server_n[i];
    }

    server_index += server;
    server_index = conf_data->server_url_index[server_index];
    sscanf(conf_data->lines[server_index], "%s = %s", key_buf, value);
    if (strcmp(key_buf, XPN_CONF_TAG_SERVER_URL) != 0)
    {
        return -1;
    }
    return 0;
}

int XpnConfGetValue(struct conf_file_data *conf_data, char *key, char *value, int partition)
{
    return XpnConfGetValueRept(conf_data, key, value, partition, 0);
}

int XpnConfGetNumPartitions(struct conf_file_data *conf_data)
{   
    return conf_data->partition_n;
}

int XpnConfGetNumServers(struct conf_file_data *conf_data, int partition_index)
{      
    if (partition_index >= conf_data->partition_n)
        return -1;

    return conf_data->server_n[partition_index];
}

int XpnConfLoad ( struct conf_file_data *conf_data )
{
    char conf[KB];
    char key_buf[KB];
    char value_buf[KB];
    FILE *fd;
    int res = 0;

    //Init in NULL pointers
    conf_data->data = NULL;
    conf_data->lines = NULL;
    conf_data->server_n = NULL;
    conf_data->server_url_index = NULL;

    if (param_get(XPN_CONF) != NULL)
    {
        strcpy(conf, param_get(XPN_CONF));
    }
    else
    {
        strcpy(conf, XPN_CONF_DEFAULT);
    }

    fd = fopen(conf, "r");
    if (fd == NULL)
    {
        fprintf(stderr, "XpnLoadConf: Can't open %s %s\n", conf, strerror(errno));
        goto cleanup_error_XpnConfLoad;
    }
    fseek(fd, 0L, SEEK_END);
    size_t file_size = ftell(fd);
    rewind(fd);

    if (file_size > 10*MB)
    {
        fprintf(stderr, "XpnLoadConf: Error conf file bigger than 10MB, size %ldB\n", file_size);
        goto cleanup_error_XpnConfLoad;
    }

    // Alocate and read all file
    conf_data->data = malloc(sizeof(char) * file_size);
    if (conf_data->data == NULL)
    {
        fprintf(stderr, "XpnLoadConf: Fail malloc %s %s\n", conf, strerror(errno));
        goto cleanup_error_XpnConfLoad;
    }

    res = fread(conf_data->data, file_size * sizeof(char), 1, fd);
    if (res != 1)
    {
        fprintf(stderr, "XpnLoadConf: Fail fread %s %s\n", conf, strerror(errno));
        goto cleanup_error_XpnConfLoad;
    }

    fclose(fd);

    // Count the lines
    conf_data->lines_n = 0;
    for (size_t i = 0; i < file_size; i++)
    {
        if (conf_data->data[i] == '\n')
            if (i+1 < file_size)
                conf_data->lines_n++;
    }
    conf_data->lines_n++;

    if (conf_data->lines_n < 1)
    {
        goto cleanup_error_XpnConfLoad;
    }

    // Allocate and pointer to lines
    conf_data->lines = malloc(conf_data->lines_n*sizeof(char *));
    if (conf_data->lines == NULL)
    {
        fprintf(stderr, "XpnLoadConf: Fail malloc %s %s\n", conf, strerror(errno));
        goto cleanup_error_XpnConfLoad;
    }

    conf_data->lines[0] = conf_data->data;
    int line_index = 1;
    for (size_t i = 1; i < file_size; i++)
    {
        if (conf_data->data[i] == '\n')
        {
            conf_data->data[i] = '\0';
            if (i+1 < file_size)
            {
                conf_data->lines[line_index] = &conf_data->data[i+1];
                line_index++;
            }
        }
    }

    // Count partitions
    conf_data->partition_n = 0;
    for (int i = 0; i < conf_data->lines_n; i++)
    {
        sscanf(conf_data->lines[i], "%s = %s", key_buf, value_buf);
        if (strcmp(key_buf, XPN_CONF_TAG_PARTITION) == 0)
        {
            conf_data->partition_n++;
        }
    }

    // Count servers
    conf_data->server_n = malloc(conf_data->partition_n * sizeof(int));
    if (conf_data->server_n == NULL)
    {
        fprintf(stderr, "XpnLoadConf: Fail malloc %s %s\n", conf, strerror(errno));
        goto cleanup_error_XpnConfLoad;
    }

    memset(conf_data->server_n, 0, conf_data->partition_n * sizeof(int));

    int current_partition = -1;
    for (int i = 0; i < conf_data->lines_n; i++)
    {
        sscanf(conf_data->lines[i], "%s = %s", key_buf, value_buf);
        if (strcmp(key_buf, XPN_CONF_TAG_PARTITION) == 0)
        {
            current_partition++;
        }
	else if (strcmp(key_buf, XPN_CONF_TAG_SERVER_URL) == 0)
        {
            conf_data->server_n[current_partition]++;
        }
    }

    //Store the server_url index
    int total_servers = 0;
    for (int i = 0; i < conf_data->partition_n; i++)
    {
        total_servers += conf_data->server_n[i];
    }

    conf_data->server_url_index = malloc(total_servers * sizeof(int));
    if (conf_data->server_url_index == NULL)
    {
        fprintf(stderr, "XpnLoadConf: Fail malloc %s %s\n", conf, strerror(errno));
        goto cleanup_error_XpnConfLoad;
    }

    int server_url_index = 0;
    for (int i = 0; i < conf_data->lines_n; i++)
    {
        sscanf(conf_data->lines[i], "%s = %s", key_buf, value_buf);
        if (strcmp(key_buf, XPN_CONF_TAG_SERVER_URL) == 0)
        {
            conf_data->server_url_index[server_url_index] = i;
            server_url_index++;
        }
    }

    return 0;

cleanup_error_XpnConfLoad:
    XpnConfFree(conf_data);
    return -1;
}

void XpnConfFree(struct conf_file_data *conf_data)
{
    FREE_AND_NULL(conf_data->data);
    FREE_AND_NULL(conf_data->lines);
    FREE_AND_NULL(conf_data->server_n);
    FREE_AND_NULL(conf_data->server_url_index);
}


int XpnInitServer(struct conf_file_data *conf_data, struct xpn_partition * part, struct nfi_server * serv, int server_num)
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
