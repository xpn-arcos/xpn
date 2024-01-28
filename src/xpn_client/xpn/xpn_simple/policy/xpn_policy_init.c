/*
 *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

struct conf_connect_st * XpnPartitionOpen(void)
{
    static struct conf_connect_st desc;
    char conf[PATH_MAX];

    desc.type = CONF_DEFAULT;
    switch (desc.type) {
    case CONF_FILE:
        if (param_get(XPN_CONF) != NULL) {
            strcpy(conf, param_get(XPN_CONF));
        } else {
            strcpy(conf, XPN_CONF_DEFAULT);
        }
        desc.connect_u.f = fopen(conf, "r");
        if (desc.connect_u.f == NULL) {
            fprintf(stderr, "xpn_init: Can't open %s\n", conf);
            return NULL;
        }
        break;

    case CONF_MXML:
        #ifdef ENABLE_MXML
        if (param_get(XPN_CONF) != NULL) {
            strcpy(desc.connect_u.xml.conf_n, param_get(XPN_CONF));
        } else {
            strcpy(desc.connect_u.xml.conf_n, XPN_CONF_DEFAULT);
        }

        if (param_get(XPN_PROFILE) != NULL) {
            strcpy(desc.connect_u.xml.profile_n, param_get(XPN_PROFILE));
        } else {
            strcpy(desc.connect_u.xml.profile_n, XPN_PROFILE_DEFAULT);
        }

        XPN_DEBUG("Using XPN_CONF=%s", desc.connect_u.xml.conf_n);
        XPN_DEBUG("Using XPN_PROFILE=%s", desc.connect_u.xml.profile_n);

        desc.connect_u.xml.conf_f = fopen(desc.connect_u.xml.conf_n, "r");
        if (desc.connect_u.xml.conf_f == NULL) {
            fprintf(stderr, "xpn_init: Can't open %s\n", desc.connect_u.xml.conf_n);
            return NULL;
        }

        desc.connect_u.xml.conf_tree = mxmlLoadFile(NULL, desc.connect_u.xml.conf_f, MXML_NO_CALLBACK);
        if (desc.connect_u.xml.conf_tree == NULL) {
            fclose(desc.connect_u.xml.conf_f);
            fprintf(stderr, "(1)xpn_init: %s is empty.\n", desc.connect_u.xml.conf_n);
            return NULL;
        }

        desc.connect_u.xml.conf_partition_node = desc.connect_u.xml.conf_tree;
        desc.connect_u.xml.profile_f = fopen(desc.connect_u.xml.profile_n, "r");

        #endif
        break;
    }

    return & desc;
}

void XpnPartitionClose(struct conf_connect_st * fconf)
{
    switch (fconf -> type) {
    case CONF_FILE:
        fclose(fconf -> connect_u.f);
        break;

    case CONF_MXML:
        #ifdef ENABLE_MXML
        if (fconf -> connect_u.xml.profile_f) {
            fclose(fconf -> connect_u.xml.profile_f);
        }
        if (fconf -> connect_u.xml.conf_f) {
            fclose(fconf -> connect_u.xml.conf_f);
        }
        #endif
        break;

    default:
        break;
    }
}

int XpnGetNextPartition(struct conf_connect_st * fconf, char * name)
{
    #ifdef ENABLE_MXML
    char * value;
    #endif

    switch (fconf -> type) {
    case CONF_FILE:
        if (fscanf(fconf -> connect_u.f, "%s", name) == EOF) {
            return 0;
        }
        break;

    case CONF_MXML:
        #ifdef ENABLE_MXML
        fconf -> connect_u.xml.conf_partition_node = mxmlFindElement(fconf -> connect_u.xml.conf_partition_node, fconf -> connect_u.xml.conf_tree, XML_TAG_ELEM_PARTITION, NULL, NULL, MXML_DESCEND);
        if (fconf -> connect_u.xml.conf_partition_node == NULL) {
            return 0;
        }

        value = (char * ) mxmlElementGetAttr(fconf -> connect_u.xml.conf_partition_node, XML_TAG_ATTR_NAME);
        if (value == NULL) {
            return 0;
        }

        strcpy(name, value);
        fconf -> connect_u.xml.conf_meta_node = fconf -> connect_u.xml.conf_partition_node;
        fconf -> connect_u.xml.conf_data_node = fconf -> connect_u.xml.conf_partition_node;
        #endif
        break;

    default:
        break;
    }

    return 1;
}

int XpnGetIdPartition(__attribute__((__unused__)) struct conf_connect_st * fconf, __attribute__((__unused__)) char * name) {
    static int cont = 0;

    return (cont++);
}

int XpnGetInfoPartition(struct conf_connect_st * fconf, struct xpn_partition * part) {
    #ifdef ENABLE_MXML
    char * value = NULL;
    char * value_th = NULL;
    mxml_node_t * node;
    #endif
    char type[20]; // TODO: 20??

    memset(type, 0, 20);

    switch (fconf -> type) {
    case CONF_FILE:
        if (fscanf(fconf -> connect_u.f, "%zu %d %s", & (part -> block_size), & (part -> data_nserv), type) == EOF) {
            return -1;
        }
        part -> block_size = part -> block_size * KB;

        if ((strcmp(type, "RAID0") == 0) || (strcmp(type, "NORMAL") == 0)) { // FIXME: previously was 'value', now changed to 'type' but not tested
            part -> type = POLICY_RAID0;
        } else if (strcmp(type, "RAID1") == 0) {
            part -> type = POLICY_RAID1;
        } else {
            part -> type = -1;
        }
        break;

    case CONF_MXML:
        #ifdef ENABLE_MXML
        if (fconf -> connect_u.xml.profile_f != NULL) {
            fconf -> connect_u.xml.profile_tree = mxmlLoadFile(NULL, fconf -> connect_u.xml.profile_f, MXML_NO_CALLBACK);
            //if (fconf->connect_u.xml.profile_tree == NULL){
            //fclose(fconf->connect_u.xml.profile_f);
            //fclose(fconf->connect_u.xml.conf_f);
            //fprintf(stderr,"(2)xpn_init: %s is empty.\n", fconf->connect_u.xml.profile_n);
            //return 0;
            //}

            //if (fconf->connect_u.xml.profile_tree != NULL){
            fconf -> connect_u.xml.profile_partition_node = fconf -> connect_u.xml.profile_tree;
            //}
        }

        value = (char * ) mxmlElementGetAttr(fconf -> connect_u.xml.conf_partition_node, XML_TAG_ATTR_BLOCKSIZE);

        if (value == NULL) {
            value = XML_DEFAULT_ATTR_BLOCKSIZE;
        }

        part -> block_size = getSizeFactor(value);

        value = NULL;
        value = (char * ) mxmlElementGetAttr(fconf -> connect_u.xml.conf_partition_node, XML_TAG_ATTR_TYPE);
        if (value == NULL) {
            value = XML_DEFAULT_ATTR_TYPE;
        }

        if ((strcmp(value, "RAID0") == 0) || (strcmp(value, "NORMAL") == 0)) {
            part -> type = POLICY_RAID0;
        } else if (strcmp(value, "RAID1") == 0) {
            part -> type = POLICY_RAID1;
        } else {
            part -> type = -1;
        }

        /* THREADS */
        value = NULL;
        value = (char * ) mxmlElementGetAttr(fconf -> connect_u.xml.conf_partition_node, XML_TAG_ATTR_THREADS);

        debug_info("[XPN]XML_TAG_ATTR_THREADS = %s\n", value ? value : "NULL");

        if (value == NULL) {
            value = XML_DEFAULT_ATTR_THREADS;
        }

        switch (value[0]) {
        case 'Y':
        case 'y':
            if (fconf -> connect_u.xml.conf_partition_node != NULL) {
                value_th = (char * ) mxmlElementGetAttr(fconf -> connect_u.xml.conf_partition_node, XML_TAG_ATTR_TH_THRESHOLD); // This is property 'th_limit'
                //value_th = value;
                debug_info("[XPN]XML_TAG_ATTR_TH_THRESHOLD = %s\n", value_th);
            }

            break;

        default:
            debug_info("[XPN]part->name = %s\n", part -> name);
            part -> size_threads = -1;
            debug_info("[XPN]part->size_threads: %d\n", part -> size_threads);
            break;
        }

        /*N DATASERVERS*/
        part -> data_nserv = 0;
        node = fconf -> connect_u.xml.conf_partition_node;
        if (node != NULL) {
            for (node = mxmlFindElement(node, fconf -> connect_u.xml.conf_partition_node, XML_TAG_ELEM_DATANODE, NULL, NULL, MXML_DESCEND); node != NULL; node = mxmlFindElement(node, fconf -> connect_u.xml.conf_partition_node, XML_TAG_ELEM_DATANODE, NULL, NULL, MXML_DESCEND)) {
                part -> data_nserv++;
            }
        }

        if (!part -> data_nserv) {
            fprintf(stderr, "(3)xpn_init: %s has no Data Nodes inside\n", fconf -> connect_u.xml.conf_n);
            return -1;
        }

        /*N METASERVERS*/
        part -> meta_nserv = 0;
        if (part -> data_nserv > 1) {
            node = fconf -> connect_u.xml.conf_partition_node;
            if (node != NULL) {
                for (node = mxmlFindElement(node, fconf -> connect_u.xml.conf_partition_node, XML_TAG_ELEM_METADATANODE, NULL, NULL, MXML_DESCEND); node != NULL; node = mxmlFindElement(node, fconf -> connect_u.xml.conf_partition_node, XML_TAG_ELEM_METADATANODE, NULL, NULL, MXML_DESCEND)) {
                    part -> meta_nserv++;
                }
            }
        }

        /* THREADS THRESHOLD */
        debug_info("[XPN]part->data_nserv * part->block_size: %d * %d\n", part -> data_nserv, part -> block_size);

        if (value_th == NULL) {
            part -> size_threads = part -> data_nserv * part -> block_size; // if th_limit is not set, then th_limit = num_servers * block_size
        } else {
            part -> size_threads = getSizeFactor(value_th); // else th_limit = as defined in conf file
        }

        debug_info("[XPN]part->size_threads: %d\n", part -> size_threads);

        return 1;
        #endif

    default:
        return -1;
    }

    return 1;
}

int XpnGetNumServersPartition(struct conf_connect_st * fconf, struct xpn_partition * part, int type) {
    switch (fconf -> type) {
    case CONF_FILE:
    case CONF_MXML:
    default:
        switch (type) {
        case XPN_DATA_SERVER:
            return part -> data_nserv;
        case XPN_META_SERVER:
            return part -> meta_nserv;
        }
        break;
    }

    return -1;
}

int XpnGetServer(struct conf_connect_st * fconf, __attribute__((__unused__)) struct xpn_partition * part, struct nfi_server * serv, int type) {
    int ret;
    char prt[PROTOCOL_MAXLEN];
    char * url;

    url = serv -> url;
    switch (fconf -> type)
    {
    case CONF_FILE:
        if ((ret = fscanf(fconf -> connect_u.f, "%s", url)) == EOF) {
            return 0;
        }
        break;

    case CONF_MXML:
        #ifdef ENABLE_MXML
        switch (type)
        {
        case XPN_DATA_SERVER:
            fconf -> connect_u.xml.conf_data_node = mxmlFindElement(fconf -> connect_u.xml.conf_data_node, fconf -> connect_u.xml.conf_partition_node, XML_TAG_ELEM_DATANODE, NULL, NULL, MXML_DESCEND);
            if (fconf -> connect_u.xml.conf_data_node == NULL) {
                return 0;
            }

            url = (char * ) mxmlElementGetAttr(fconf -> connect_u.xml.conf_data_node, XML_TAG_ATTR_URL);
            break;

        case XPN_META_SERVER:
            fconf -> connect_u.xml.conf_meta_node = mxmlFindElement(fconf -> connect_u.xml.conf_meta_node, fconf -> connect_u.xml.conf_partition_node, XML_TAG_ELEM_METADATANODE, NULL, NULL, MXML_DESCEND);
            if (fconf -> connect_u.xml.conf_meta_node == NULL) {
                return 0;
            }

            url = (char * ) mxmlElementGetAttr(fconf -> connect_u.xml.conf_meta_node, XML_TAG_ATTR_URL);
            break;
        }

        if (url == NULL) {
            return 0;
        }
        #endif
        break;

    default:
        fprintf(stderr, "[XPN] Conf type %d not recognized\n", fconf -> type);
        break;
    }

    serv -> block_size = part -> block_size; // Reference of the partition blocksize
    
    XPN_DEBUG("url=%s", url);
    
    ret = ParseURL(url, prt, NULL, NULL, NULL, NULL, NULL);
    if (ret < 0) {
        xpn_err(XPNERR_INVALURL);
        return -1;
    }

    //TODO: locality
    /*
    char serv_name[PATH_MAX];
    char cli_name[HOST_NAME_MAX];

    // Get server name
    ret = ParseURL(url, prt, NULL, NULL, serv_name, NULL, NULL);
    if (ret < 0) {
        xpn_err(XPNERR_INVALURL);
        return -1;
    }

    // If the protocol is not file, then check if i am in the same node in order to change the protocol to file to exploit the locality
    if (strcmp(prt, "file") != 0)
    {
        gethostname(cli_name, HOST_NAME_MAX);

        if (strcmp(cli_name, serv_name) == 0)
        {
            strcpy(prt, "file");
        }
    }
    */
    //TODO END

    // crear conexion
    if (strcmp(prt, "file") == 0) {
        //printf("[XPN]nfi_local_init: %s\n",url);
        ret = nfi_local_init(url, serv, NULL);
        if (ret < 0) {
            xpn_err(XPNERR_INITSERV);
            return -1;
        }
    }

    #ifdef ENABLE_NFS
    else if ((strcmp(prt, "nfs") == 0) || (strcmp(prt, "nfs2") == 0)) {
        //printf("[XPN]nfi_nfs_init: %s\n",url);
        ret = nfi_nfs_init(url, serv, NULL);
        if (ret < 0) {
            xpn_err(XPNERR_INITSERV);
            return -1;
        }
    }
    #endif

    #ifdef ENABLE_NFS3
    else if (strcmp(prt, "nfs3") == 0) {
        //printf("[XPN]nfi_nfs3_init: %s\n",url);
        ret = nfi_nfs3_init(url, serv, NULL);
        if (ret < 0) {
            xpn_err(XPNERR_INITSERV);
            return -1;
        }
    }
    #endif

    #ifdef ENABLE_MPI_SERVER
    else if (strcmp(prt, "mpi_server") == 0) {
        //printf("[XPN]nfi_mpi_server_init: %s\n",url);
        ret = nfi_mpi_server_init(url, serv, NULL);
        if (ret < 0) {
            xpn_err(XPNERR_INITSERV);
            return -1;
        }
    }
    #endif

    #ifdef ENABLE_SCK_SERVER
    else if (strcmp(prt, "sck_server") == 0) {
        //printf("[XPN]nfi_sck_server_init: %s\n",url);
        ret = nfi_sck_server_init(url, serv, NULL);
        if (ret < 0) {
            xpn_err(XPNERR_INITSERV);
            return -1;
        }
    }
    #endif

    #ifdef ENABLE_TCP_SERVER
    else if (strcmp(prt, "tcp_server") == 0) {
        //printf("[XPN]nfi_tcp_server_init: %s\n",url);
        ret = nfi_tcp_server_init(url, serv, NULL);
        if (ret < 0) {
            xpn_err(XPNERR_INITSERV);
            return -1;
        }
    }
    #endif

    else {
        printf("[XPN] Protocol '%s' not recognized\n", prt);
        xpn_err(XPNERR_INVALURL);
        return -1;
    }

    // Default Value
    return 1;
}

int XpnGetPartition(char * path) /* return partition's id */ {
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

struct xpn_partition * XpnSearchPart(int pd) {
    int i = 0;

    while ((i < XPN_MAX_PART) && (xpn_parttable[i].id != pd)) {
        i++;
    }

    if (i == XPN_MAX_PART) {
        return NULL;
    }

    return & (xpn_parttable[i]);
}

void XpnShowPartitionTable(void) {
    int i = 0;

    while ((i < XPN_MAX_PART) && (xpn_parttable[i].name != NULL) && (strcmp("", xpn_parttable[i].name) != 0)) {
        printf("[XPN]xpn_parttable[%d].name = %s\n", i, xpn_parttable[i].name);
        i++;
    }
}
