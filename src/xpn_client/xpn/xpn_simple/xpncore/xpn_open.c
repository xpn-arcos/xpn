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

#include "xpn/xpn_simple/xpn_open.h"

#include "xpn/xpn_simple/xpn_opendir.h"

#define MASK 0000777

extern struct xpn_filedesc * xpn_file_table[XPN_MAX_FILE];

int ino_counter = 0;



/*****************************************************************/
void XpnShowFileTable(void) 
{
    int i = 0;

    printf("<file_table %d>\n", XPN_MAX_FILE);
    while ((i < XPN_MAX_FILE) && (xpn_file_table[i] != NULL) && (xpn_file_table[i] -> path != NULL)) 
    {
        printf(" * i:%d -- path:%s\n", i, xpn_file_table[i] -> path);
        i++;
    }
    printf("</file_table>\n");
}



int XpnSearchSlotFile(int pd, char * path, struct xpn_fh * vfh, struct xpn_metadata * mdata, int mode) 
{
    int i, res;

    XPN_DEBUG_BEGIN_ARGS1(path);

    i = 0;
    while ((i < XPN_MAX_FILE - 1) && (xpn_file_table[i] != NULL)) 
    { // FIXME? Por que i<XPN_MAX_FILE-1, no deberia ser i<XPN_MAX_FILE
        i++;
    }

    if (i == XPN_MAX_FILE) 
    {
        // xpn_err() ?
        return -1;
    }

    xpn_file_table[i] = (struct xpn_filedesc * ) malloc(sizeof(struct xpn_filedesc));
    if (xpn_file_table[i] == NULL) 
    {
        return -1;
    }

    xpn_file_table[i] -> id = i;
    xpn_file_table[i] -> type = mdata -> type;
    memccpy(xpn_file_table[i] -> path, path, 0, PATH_MAX - 1);
    xpn_file_table[i] -> mode = mode;
    xpn_file_table[i] -> links = 1;
    xpn_file_table[i] -> part = XpnSearchPart(pd);
    xpn_file_table[i] -> offset = 0;
    xpn_file_table[i] -> block_size = xpn_file_table[i] -> part -> block_size;
    xpn_file_table[i] -> mdata = mdata;
    xpn_file_table[i] -> data_vfh = vfh;
    xpn_file_table[i] -> size_threads = XpnGetSizeThreads(xpn_file_table[i] -> part);

    res = i;
    XPN_DEBUG_END_ARGS1(path);

    return res;
}



int XpnSearchFile(const char * path) 
{
    int res, i = 0;

    XPN_DEBUG_BEGIN_ARGS1(path);

    while (i < XPN_MAX_FILE) 
    {
        if ((xpn_file_table[i] != NULL) && (xpn_file_table[i] -> path != NULL) && (strcmp(xpn_file_table[i] -> path, path) == 0)) 
        {
            break;
        }

        i++;
    }

    if (i == XPN_MAX_FILE) 
    {
        res = -1;
    } else 
    {
        res = i;
    }

    XPN_DEBUG_END

    return res;
}


/*****************************************************************/

int xpn_internal_creat(const char * path, mode_t perm, struct xpn_fh ** vfh, struct xpn_metadata ** mdata) 
{
    char abs_path[PATH_MAX], url_serv[PATH_MAX];
    struct nfi_server ** servers;
    struct xpn_metadata * mdata_aux;
    struct nfi_attr attr;
    int res, err, i, j, n, pd;
    struct xpn_fh * vfh_aux;

    XPN_DEBUG_BEGIN_ARGS1(path);

    if (path == NULL) 
    {
        xpn_err(XPNERR_PARAM);
        XPN_DEBUG_END
        return -1;
    }

    res = XpnGetAbsolutePath(path, abs_path); // this function generates the absolute path
    if (res < 0) 
    {
        xpn_err(XPNERR_PATH_NOEXIST);
        XPN_DEBUG_END_ARGS1(path);
        return -1;
    }

    pd = XpnGetPartition(abs_path); // return partition's id and remove partition name from abs_path
    if (pd < 0) 
    {
        xpn_err(XPNERR_PART_NOEXIST);
        XPN_DEBUG_END_ARGS1(path);
        return -1;
    }

    /* params:
     * flag operation, partition id, absolute path, file descriptor, pointer to server
     */
    servers = NULL;
    n = XpnGetServers(op_xpn_creat, pd, abs_path, -1, & servers, XPN_DATA_SERVER);
    if (n <= 0) 
    {
        //free(servers);
        XPN_DEBUG_END_ARGS1(path);
        return -1;
    }

    bzero( & attr, sizeof(struct nfi_attr));
    attr.at_mode = perm;

    // vfh construction
    vfh_aux = (struct xpn_fh * ) malloc(sizeof(struct xpn_fh));
    if (NULL == vfh_aux) 
    {
        xpn_err(XPNERR_NOMEMORY);
        free(servers);
        XPN_DEBUG_END_ARGS1(path);
        return -1;
    }

    vfh_aux -> n_nfih = n;
    vfh_aux -> nfih = (struct nfi_fhandle ** ) malloc(sizeof(struct nfi_fhandle * ) * n);
    if (vfh_aux -> nfih == NULL) 
    {
        xpn_err(XPNERR_NOMEMORY);
        free(servers);
        XPN_DEBUG_END_ARGS1(path);
        return -1;
    }

    for (i = 0; i < n; i++) 
    {
        vfh_aux -> nfih[i] = NULL;
    }

    mdata_aux = (struct xpn_metadata * ) malloc(sizeof(struct xpn_metadata));
    if (mdata_aux == NULL) 
    {
        free(servers);
        // also indirect pointers
        for (i = 0; i < n; i++) 
        {
            if (vfh_aux -> nfih[i] != NULL) 
            {
                if (vfh_aux -> nfih[i] -> priv_fh != NULL) 
                {
                    vfh_aux -> nfih[i] -> server -> ops -> nfi_close(vfh_aux -> nfih[i] -> server, vfh_aux -> nfih[i]);
                }
                free(vfh_aux -> nfih[i]);
            }
        }

        free(vfh_aux -> nfih);
        free(vfh_aux);
        free(mdata_aux);

        XPN_DEBUG_END_ARGS1(path);
        return -1;
    }

    XpnCreateMetadata(mdata_aux, pd, abs_path);

    for (j = 0; j < n; j++) 
    {
        i = XpnGetMetadataPos(mdata_aux, j);

        XpnGetURLServer(servers[i], abs_path, url_serv);
        vfh_aux -> nfih[i] = (struct nfi_fhandle * ) malloc(sizeof(struct nfi_fhandle));
        bzero(vfh_aux -> nfih[i], sizeof(struct nfi_fhandle));
        if (vfh_aux -> nfih[i] == NULL) 
        {
            free(servers);
            XPN_DEBUG_END_ARGS1(path);
            return -1;
        }

        // Worker
        servers[i] -> wrk -> thread = servers[i] -> xpn_thread;
        nfi_worker_do_create(servers[i] -> wrk, url_serv, & attr, vfh_aux -> nfih[i]);
    }

    // Wait
    err = 0;
    for (j = 0; j < n; j++) 
    {
        i = XpnGetMetadataPos(mdata_aux, j);
        res = nfiworker_wait(servers[i] -> wrk);
        // error checking
        if ((res < 0) && (!err)) 
        {
            err = 1;
            // erase the file create before de server number i
            /*
              for(j=0; j<i; j++){
                XpnGetURLServer(servers[j], abs_path, url_serv);
                nfi_worker_do_remove(servers[j]->wrk, url_serv);
                nfi_worker_wait(servers[j]->wrk);
              }
            }else{
              if((res>=0)&&(err)){
                XpnGetURLServer(servers[i], abs_path, url_serv);
                nfi_worker_do_remove(servers[i]->wrk, url_serv);
                nfi_worker_wait(servers[i]->wrk);
              }
            */
        }
    }

    // error checking
    if (err) 
    {
        xpn_err(XPNERR_CREATE);
        free(servers);
        for (i = 0; i < n; i++) 
        {
            if (vfh_aux -> nfih[i] != NULL) 
            {
                if (vfh_aux -> nfih[i] -> priv_fh != NULL) 
                {
                    vfh_aux -> nfih[i] -> server -> ops -> nfi_close(vfh_aux -> nfih[i] -> server, vfh_aux -> nfih[i]);
                }
                free(vfh_aux -> nfih[i]);
            }
        }

        free(vfh_aux -> nfih);
        XPN_DEBUG_END_ARGS1(path);

        return -1;
    }

    res = XpnUpdateMetadata(mdata_aux, n, servers, vfh_aux, abs_path);
    if (res < 0) 
    {
        free(servers);
        // also indirect pointers
        for (i = 0; i < n; i++) {
            if (vfh_aux -> nfih[i] != NULL) 
            {
                if (vfh_aux -> nfih[i] -> priv_fh != NULL) 
                {
                    vfh_aux -> nfih[i] -> server -> ops -> nfi_close(vfh_aux -> nfih[i] -> server, vfh_aux -> nfih[i]);
                }
                free(vfh_aux -> nfih[i]);
            }
        }

        free(vfh_aux -> nfih);
        free(vfh_aux);
        free(mdata_aux);

        XPN_DEBUG_END_ARGS1(path);
        return -1;
    }

    ( * vfh) = vfh_aux;
    ( * mdata) = mdata_aux;
    free(servers);

    xpn_err(0);
    XPN_DEBUG_END_ARGS1(path);
    return 0;
}



int xpn_internal_open(const char * path, struct xpn_fh * vfh, struct xpn_metadata * mdata, int mode) 
{
    char abs_path[PATH_MAX];
    struct nfi_server ** servers;
    int n, pd, i;
    int res = -1;

    XPN_DEBUG_BEGIN_ARGS1(path);

    res = XpnGetAbsolutePath(path, abs_path); // this function generates the absolute path
    if (res < 0) 
    {
        xpn_err(XPNERR_PATH_NOEXIST);
        XPN_DEBUG_END_ARGS1(path);
        return res;
    }

    pd = XpnGetPartition(abs_path); // returns partition id and remove partition name from abs_path
    if (pd < 0) 
    {
        xpn_err(XPNERR_PART_NOEXIST);
        XPN_DEBUG_END_ARGS1(path);
        return pd;
    }

    res = XpnSearchFile(abs_path);
    if (res < 0) 
    {
        // FIXME: CAUTION, this caused the call to fail some changes before, although now it seems to work.
        /* params:
         * flag operation, partition id, absolute path, file descriptor, pointer to server
         */
        servers = NULL;
        n = XpnGetServers(op_xpn_open, pd, abs_path, -1, & servers, XPN_DATA_SERVER);
        if (n <= 0) 
        {
            //free(servers);
            XPN_DEBUG_END_ARGS1(path);
            return res;
        }

        if (mdata == NULL) {
            mdata = (struct xpn_metadata * ) malloc(sizeof(struct xpn_metadata));
            if (mdata == NULL) 
            {
                free(servers);
                XPN_DEBUG_END_ARGS1(path);
                return res;
            }
            if (vfh == NULL) {
                vfh = (struct xpn_fh * ) malloc(sizeof(struct xpn_fh));
                if (vfh == NULL) 
                {
                    free(servers);
                    XPN_DEBUG_END_ARGS1(path);
                    return res;
                }

                vfh -> n_nfih = n;
                vfh -> nfih = (struct nfi_fhandle ** ) malloc(sizeof(struct nfi_fhandle * ) * n);
                for (i = 0; i < n; i++) 
                {
                    vfh -> nfih[i] = NULL;
                }
            }
            // read the metadata
            memset(mdata, 0, sizeof(struct xpn_metadata));
            res = XpnReadMetadata(mdata, n, servers, vfh, abs_path, pd);
            if (res < 0) 
            {
                free(servers);
                free(vfh -> nfih);
                free(vfh);
                free(mdata);
                XPN_DEBUG_END_ARGS1(path);
                return res;
            }
        }

        free(servers);
        res = XpnSearchSlotFile(pd, abs_path, vfh, mdata, mode);
    } else 
    {
        xpn_file_table[res] -> links++;
    }

    XPN_DEBUG_END_ARGS1(path);
    return res;
}



int xpn_internal_resize(__attribute__((__unused__)) char * path, __attribute__((__unused__)) struct xpn_fh ** vfh, __attribute__((__unused__)) int size) 
{
    // TODO
    return 0;
}



int xpn_internal_fresize(__attribute__((__unused__)) int fd, __attribute__((__unused__)) int size) 
{
    // TODO
    return 0;
}



int xpn_internal_remove(const char * path) 
{
    char abs_path[PATH_MAX], url_serv[PATH_MAX];
    int res, err, i, n, pd;
    struct nfi_server ** servers;

    if (path == NULL) 
    {
        xpn_err(XPNERR_PARAM);
        return -1;
    }

    res = XpnGetAbsolutePath(path, abs_path); // esta funcion genera el path absoluto
    if (res < 0) 
    {
        xpn_err(XPNERR_PATH_NOEXIST);
        return -1;
    }

    pd = XpnGetPartition(abs_path); // return partition's id
    if (pd < 0) 
    {
        xpn_err(XPNERR_PART_NOEXIST);
        return -1;
    }

    /* params:
     * flag operation, partition id, absolute path, file descriptor, pointer to server
     */
    servers = NULL;
    n = XpnGetServers(op_xpn_remove, pd, abs_path, -1, & servers, XPN_DATA_SERVER);
    if (n <= 0) 
    {
        // free(servers);
        return -1;
    }

    //Master node
    int master_node = hash((char *)path, n);
    XpnGetURLServer(servers[master_node], abs_path, url_serv);

    // Worker
    servers[master_node] -> wrk -> thread = servers[master_node] -> xpn_thread;
    servers[master_node] -> wrk -> arg.master_node = master_node;
    servers[master_node] -> wrk -> arg.is_master_node = 1;

    nfi_worker_do_remove(servers[master_node] -> wrk, url_serv);

    res = nfiworker_wait(servers[master_node] -> wrk);
    if (res < 0)
    {
        free(servers);
        return res;
    }

    // Rest of nodes...
    for (i = 0; i < n; i++) 
    {
        if (i == master_node)
        {
            continue;
        }

        XpnGetURLServer(servers[i], abs_path, url_serv);

        // Worker
        servers[i] -> wrk -> thread = servers[i] -> xpn_thread;
        servers[i] -> wrk -> arg.master_node = master_node;
        servers[i] -> wrk -> arg.is_master_node = 0;

        nfi_worker_do_remove(servers[i] -> wrk, url_serv);
    }

    // Wait
    err = 0;
    for (i = 0; i < n; i++) 
    {
        if (i == master_node)
        {
            continue;
        }

        res = nfiworker_wait(servers[i] -> wrk);
        // error checking
        if ((res < 0) && (!err)) {
            err = 1;
        }
    }

    // error checking
    if (err) 
    {
        xpn_err(XPNERR_REMOVE);
        free(servers);
        return -1;
    }

    free(servers);
    xpn_err(XPN_OK);
    return 0;
}



/************************* TODO ****************************************/

int xpn_simple_preload(const char * virtual_path, const char * storage_path) 
{
    char abs_path[PATH_MAX], url_serv[PATH_MAX];
    struct nfi_server ** servers;
    struct xpn_metadata * mdata;
    int res, i, n, pd;

    XPN_DEBUG_BEGIN_ARGS2(virtual_path, storage_path)

    if (virtual_path == NULL) 
    {
        xpn_err(XPNERR_PARAM);
        XPN_DEBUG_END
        return -1;
    }

    if (storage_path == NULL) 
    {
        xpn_err(XPNERR_PARAM);
        XPN_DEBUG_END
        return -1;
    }

    res = XpnGetAbsolutePath(virtual_path, abs_path); // esta funcion genera el path absoluto
    if (res < 0) 
    {
        xpn_err(XPNERR_PATH_NOEXIST);
        XPN_DEBUG_END
        return -1;
    }

    pd = XpnGetPartition(abs_path); // return partition's id
    if (pd < 0) 
    {
        xpn_err(XPNERR_PART_NOEXIST);
        XPN_DEBUG_END
        return -1;
    }

    /* params:
     * flag operation, partition id, absolute path, file descriptor, pointer to server
     */
    servers = NULL;
    n = XpnGetServers(op_xpn_flush, pd, abs_path, -1, & servers, XPN_DATA_SERVER);
    if (n <= 0) 
    {
        //free(servers);
        XPN_DEBUG_END
        return -1;
    }

    mdata = (struct xpn_metadata * ) malloc(sizeof(struct xpn_metadata));
    if (mdata == NULL) 
    {
        free(servers);
        free(mdata);

        XPN_DEBUG_END
        return -1;
    }

    XpnCreateMetadata(mdata, pd, abs_path);

    i = XpnGetMetadataPos(mdata, -1); //TODO: pasar por parametro

    for (int j = 0; j < n; ++j) 
    {
        XpnGetURLServer(servers[j], abs_path, url_serv);

        // Worker
        servers[i] -> wrk -> thread = servers[i] -> xpn_thread;
        nfi_worker_do_preload(servers[j] -> wrk, url_serv, (char * ) url_serv, (char * ) storage_path, 1);
    }

    // Wait
    for (int j = 0; j < n; ++j) 
    {
        res = nfiworker_wait(servers[i] -> wrk);
    }

    free(servers);
    free(mdata);

    // error checking
    if (res) 
    {
        xpn_err(XPNERR_PARAM);
        XPN_DEBUG_END
        return -1;
    }

    XPN_DEBUG_END
    return 0;
}



int xpn_simple_flush(const char * virtual_path, const char * storage_path) 
{
    char abs_path[PATH_MAX], url_serv[PATH_MAX];
    struct nfi_server ** servers;
    struct xpn_metadata * mdata;
    int res, i, n, pd;

    if (virtual_path == NULL) 
    {
        xpn_err(XPNERR_PARAM);
        return -1;
    }

    if (storage_path == NULL) 
    {
        xpn_err(XPNERR_PARAM);
        return -1;
    }

    res = XpnGetAbsolutePath(virtual_path, abs_path); // esta funcion genera el path absoluto
    if (res < 0) 
    {
        xpn_err(XPNERR_PATH_NOEXIST);
        return -1;
    }

    pd = XpnGetPartition(abs_path); // return partition's id
    if (pd < 0) 
    {
        xpn_err(XPNERR_PART_NOEXIST);
        return -1;
    }

    /* params:
     * flag operation, partition id, absolute path, file descriptor, pointer to server
     */
    servers = NULL;
    n = XpnGetServers(op_xpn_flush, pd, abs_path, -1, & servers, XPN_DATA_SERVER);
    if (n <= 0) 
    {
        //free(servers);
        return -1;
    }

    mdata = (struct xpn_metadata * ) malloc(sizeof(struct xpn_metadata));
    if (mdata == NULL) 
    {
        free(servers);
        free(mdata);

        return -1;
    }

    XpnCreateMetadata(mdata, pd, abs_path);

    i = XpnGetMetadataPos(mdata, -1);
    // TODO: i variable is not used, next line avoid compilation warning
    i = i;

    for (int j = 0; j < n; ++j) 
    {
        XpnGetURLServer(servers[j], abs_path, url_serv);

        // Worker
        servers[i] -> wrk -> thread = servers[i] -> xpn_thread;
        nfi_worker_do_flush(servers[j] -> wrk, url_serv, (char * ) url_serv, (char * ) storage_path, 1);
    }

    // Wait
    for (int j = 0; j < n; ++j) 
    {
        res = nfiworker_wait(servers[i] -> wrk);
    }

    free(servers);
    free(mdata);

    // error checking
    /*if(err){
      xpn_err(XPNERR_PARAM);
      return -1;
    }*/

    return 0;
}



/************************* </TODO> ****************************************/

int xpn_simple_creat(const char * path, mode_t perm) 
{
    struct xpn_fh * vfh;
    struct xpn_metadata * mdata;
    int res;

    XPN_DEBUG_BEGIN_ARGS1(path);

    if ((path == NULL) || (strlen(path) > PATH_MAX)) 
    {
        res = -1;
        XPN_DEBUG_END_ARGS1(path)
        return res;
    }

    vfh = NULL;
    mdata = NULL;
    /*
    res = xpn_internal_resize(path, &vfh, 0);
    if(res<0){
      res = xpn_internal_creat(path, perm, &vfh, &mdata);
      if (res <0){
        XPN_DEBUG_END_ARGS1(path)
        return res;
      }
    }
    */
    xpn_internal_remove(path);

    res = xpn_internal_creat(path, perm, & vfh, & mdata);
    if (res < 0) 
    {
        XPN_DEBUG_END_ARGS1(path)
        return res;
    }

    res = xpn_internal_open(path, vfh, mdata, perm);
    if (res < 0) 
    {
        XPN_DEBUG_END_ARGS1(path)
        return res;
    }

    xpn_err(XPN_OK);
    XPN_DEBUG_END_ARGS1(path)
    return res;
}



int xpn_simple_open(const char * path, int flags, mode_t mode) 
{
    struct xpn_fh * vfh;
    struct xpn_metadata * mdata;
    int md;
    int res = -1;

    XPN_DEBUG_BEGIN_ARGS1(path);

    if ((path == NULL) || (strlen(path) > PATH_MAX)) 
    {
        XPN_DEBUG_END_ARGS1(path)
        return res;
    }

    if ((flags & O_DIRECTORY) > 0) 
    {
        struct stat sb;
        xpn_simple_stat(path, &sb);
        if ((sb.st_mode & S_IFMT) != S_IFDIR) 
        {
            errno = ENOTDIR;
            return -1;
        }
    }

    if ((flags & O_CREAT) > 0) 
    {
        if (mode > 0177777) {
            XPN_DEBUG_END_ARGS1(path)
            return res;
        }
    }

    vfh = NULL;
    mdata = NULL;

    if ((O_WRONLY == (flags & O_WRONLY)) || (O_RDWR == (flags & O_RDWR))) 
    {
        //Check if the file exists
        struct stat sb;
        int exists = xpn_simple_stat(path, & sb);

        // O_TRUNC
        if ((exists >= 0) && (O_TRUNC == (flags & O_TRUNC)))
        {
            // Remove the existing file
            res = xpn_internal_remove(path);
            // res = xpn_internal_resize(path, 0)

            // Create the new file
            //mode = mode & MASK;
            res = xpn_internal_creat(path, mode, & vfh, & mdata);
            if (res < 0) {
                XPN_DEBUG_END_ARGS1(path)
                return res;
            }
        }

        // O_CREAT
        if ((exists < 0) && ((O_CREAT == (flags & O_CREAT)) && (mode != 0))) 
        {
            // Create the new file
            //mode = mode & MASK;
            res = xpn_internal_creat(path, mode, & vfh, & mdata);
            if (res < 0) {
                XPN_DEBUG_END_ARGS1(path)
                return res;
            }
        }
    }

    md = (flags & O_RDWR) | (flags & O_RDONLY) | (flags & O_WRONLY);

    res = xpn_internal_open(path, vfh, mdata, md);
    /*if (res >= 0) {
        xpn_file_table[res]->st = sb ;
    }*/

    xpn_err(XPN_OK);

    XPN_DEBUG_END_ARGS1(path);

    return res;
}



int xpn_simple_close(int fd) 
{
    int res, i;

    XPN_DEBUG_BEGIN_CUSTOM("%d", fd)

    if ((fd < 0) || (fd > XPN_MAX_FILE - 1)) 
    {
        xpn_err(EBADF);
        XPN_DEBUG_END_CUSTOM("%d", fd)
        return -1;
    }

    if (xpn_file_table[fd] == NULL) 
    {
        xpn_err(EBADF);
        XPN_DEBUG_END_CUSTOM("%d", fd)
        return -1;
    }

    /*
    int n_threads = 0;
    struct nfi_server **servers;
    servers = NULL;
    int n = XpnGetServers(op_xpn_close, xpn_file_table[fd]->part->id, NULL, -1, &servers, XPN_DATA_SERVER);
    if(n<=0){
      free(servers);
      res = -1;
      return res;
    }
    */

    xpn_file_table[fd] -> links--;
    if (xpn_file_table[fd] -> links == 0) 
    {
        for (i = 0; i < xpn_file_table[fd] -> data_vfh -> n_nfih; i++) 
        {
            if (xpn_file_table[fd] -> data_vfh -> nfih[i] != NULL) 
            {
                if (xpn_file_table[fd] -> data_vfh -> nfih[i] -> priv_fh != NULL) 
                {
                    //n_threads++;

                    // Worker
                    //servers[i]->wrk->thread = servers[i]->xpn_thread;
                    //nfi_worker_do_close(servers[i]->wrk, xpn_file_table[fd]->data_vfh->nfih[i]);

                    //TODO: old
                    xpn_file_table[fd] -> data_vfh -> nfih[i] -> server -> ops -> nfi_close(xpn_file_table[fd] -> data_vfh -> nfih[i] -> server, xpn_file_table[fd] -> data_vfh -> nfih[i]);
                }
                //TODO: old
                free(xpn_file_table[fd] -> data_vfh -> nfih[i]);
            }
        }

        // Wait
        /*int err = 0;
        for(int j=0;j<n_threads;j++)
        {
          res = nfi_worker_wait(servers[j]->wrk);
          if((res<0)&&(!err)){
            err = 1;
          }
        }*/

        /*for(i=0;i<xpn_file_table[fd]->data_vfh->n_nfih;i++)
        {
          if(xpn_file_table[fd]->data_vfh->nfih[i] != NULL)
          {
            if(xpn_file_table[fd]->data_vfh->nfih[i]->priv_fh != NULL){
              free(xpn_file_table[fd]->data_vfh->nfih[i]);
            }
          }
        }*/

        //free(servers);
        free(xpn_file_table[fd] -> data_vfh -> nfih);
        free(xpn_file_table[fd] -> data_vfh);
        free(xpn_file_table[fd] -> mdata -> policy);
        free(xpn_file_table[fd] -> mdata);
        free(xpn_file_table[fd]);
        xpn_file_table[fd] = NULL;

        // error checking
        /*if(err){
          return -1;
        } */
    }

    xpn_err(XPN_OK);
    XPN_DEBUG_END_CUSTOM("%d", fd)
    return 0;
}



int xpn_simple_unlink(const char * path) 
{
    int res;

    XPN_DEBUG_BEGIN_ARGS1(path);

    res = xpn_internal_remove(path);

    XPN_DEBUG_END_ARGS1(path)
    return res;
}



int xpn_simple_rename(const char * path, const char * newpath) 
{
    char abs_path[PATH_MAX], url_serv[PATH_MAX];
    char newabs_path[PATH_MAX], newurl_serv[PATH_MAX];
    struct nfi_server ** servers;
    struct xpn_metadata * mdata_aux;
    int res, err, i, n, pd, newpd;
    struct xpn_fh * vfh_aux;

    if (path == NULL) 
    {
        xpn_err(XPNERR_PARAM);
        return -1;
    }

    if (newpath == NULL) 
    {
        xpn_err(XPNERR_PARAM);
        return -1;
    }

    res = XpnGetAbsolutePath(path, abs_path); // esta funcion genera el path absoluto
    if (res < 0) 
    {
        xpn_err(XPNERR_PATH_NOEXIST);
        return -1;
    }

    res = XpnGetAbsolutePath(newpath, newabs_path); // esta funcion genera el path absoluto
    if (res < 0) 
    {
        xpn_err(XPNERR_PATH_NOEXIST);
        return -1;
    }

    pd = XpnGetPartition(abs_path); // return partition's id
    if (pd < 0)
    {
        xpn_err(XPNERR_PART_NOEXIST);
        return -1;
    }

    newpd = XpnGetPartition(newabs_path); // return partition's id
    if (newpd < 0) 
    {
        xpn_err(XPNERR_PART_NOEXIST);
        return -1;
    }

    if (pd != newpd) 
    {
        return -1;
    }

    /* params:
     * flag operation, partition id, absolute path, file descriptor, pointer to server
     */
    servers = NULL;
    n = XpnGetServers(op_xpn_rename, pd, abs_path, -1, & servers, XPN_DATA_SERVER);
    if (n <= 0) {
        //free(servers);
        return -1;
    }

    vfh_aux = (struct xpn_fh * ) malloc(sizeof(struct xpn_fh));

    // construccion del vfh
    if (vfh_aux == NULL) 
    {
        xpn_err(XPNERR_NOMEMORY);
        free(servers);
        return -1;
    }

    vfh_aux -> n_nfih = n;
    vfh_aux -> nfih = (struct nfi_fhandle ** ) malloc(sizeof(struct nfi_fhandle * ) * n);
    if (vfh_aux -> nfih == NULL) 
    {
        xpn_err(XPNERR_NOMEMORY);
        free(servers);
        return -1;
    }

    for (i = 0; i < n; i++) 
    {
        vfh_aux -> nfih[i] = NULL;
    }

    mdata_aux = (struct xpn_metadata * ) malloc(sizeof(struct xpn_metadata));
    if (mdata_aux == NULL) 
    {
        free(servers);
        free(vfh_aux -> nfih);
        free(vfh_aux);
        return -1;
    }

    memset(mdata_aux, 0, sizeof(struct xpn_metadata));

    res = XpnReadMetadata(mdata_aux, n, servers, vfh_aux, abs_path, pd);
    if (res < 0) 
    {
        free(servers);
        // tambien los punteros indirectos
        for (i = 0; i < n; i++) 
        {
            if (vfh_aux -> nfih[i] != NULL) 
            {
                if (vfh_aux -> nfih[i] -> priv_fh != NULL) 
                {
                    vfh_aux -> nfih[i] -> server -> ops -> nfi_close(vfh_aux -> nfih[i] -> server, vfh_aux -> nfih[i]);
                }

                free(vfh_aux -> nfih[i]);
            }
        }
        free(vfh_aux -> nfih);
        free(vfh_aux);
        free(mdata_aux);

        return -1;
    }

    res = XpnUpdateMetadata(mdata_aux, n, servers, vfh_aux, newabs_path);
    if (res < 0) 
    {
        free(servers);
        // tambien los punteros indirectos
        for (i = 0; i < n; i++) 
        {
            if (vfh_aux -> nfih[i] != NULL) 
            {
                if (vfh_aux -> nfih[i] -> priv_fh != NULL) 
                {
                    vfh_aux -> nfih[i] -> server -> ops -> nfi_close(vfh_aux -> nfih[i] -> server, vfh_aux -> nfih[i]);
                }

                free(vfh_aux -> nfih[i]);
            }
        }
        free(vfh_aux -> nfih);
        free(vfh_aux);
        free(mdata_aux);

        return -1;
    }

    for (i = 0; i < n; i++) 
    {
        XpnGetURLServer(servers[i], abs_path, url_serv);
        XpnGetURLServer(servers[i], newabs_path, newurl_serv);

        // Worker
        servers[i] -> wrk -> thread = servers[i] -> xpn_thread;
        nfi_worker_do_rename(servers[i] -> wrk, url_serv, newurl_serv);
    }

    err = 0;
    for (i = 0; i < n; i++) 
    {
        res = nfiworker_wait(servers[i] -> wrk);
        if ((!err) && (res < 0)) 
        {
            err = 1;
        }
    }

    free(servers);
    // tambien los punteros indirectos
    for (i = 0; i < n; i++) 
    {
        if (vfh_aux -> nfih[i] != NULL) 
        {
            if (vfh_aux -> nfih[i] -> priv_fh != NULL) 
            {
                vfh_aux -> nfih[i] -> server -> ops -> nfi_close(vfh_aux -> nfih[i] -> server, vfh_aux -> nfih[i]);
            }
            free(vfh_aux -> nfih[i]);
        }
    }

    free(vfh_aux -> nfih);
    free(vfh_aux);
    free(mdata_aux);

    // error checking
    if (err) 
    {
        res = XpnUpdateMetadata(mdata_aux, n, servers, vfh_aux, newabs_path);
        if (res < 0) 
        {
            free(servers);
            // tambien los punteros indirectos
            for (i = 0; i < n; i++) 
            {
                if (vfh_aux -> nfih[i] != NULL) 
                {
                    if (vfh_aux -> nfih[i] -> priv_fh != NULL) 
                    {
                        vfh_aux -> nfih[i] -> server -> ops -> nfi_close(vfh_aux -> nfih[i] -> server, vfh_aux -> nfih[i]);
                    }

                    free(vfh_aux -> nfih[i]);
                }
            }
            free(vfh_aux -> nfih);
            free(vfh_aux);
            free(mdata_aux);

            return -1;
        }
    }

    return 0;
}



int xpn_simple_fstat(int fd, struct stat * sb) 
{
    int res;

    XPN_DEBUG_BEGIN_CUSTOM("%d", fd)

    if (fd < 0) 
    {
        errno = EBADF;
        XPN_DEBUG_END_CUSTOM("%d", fd)
        return -1;
    }

    res = XpnGetAtribFd(fd, sb);

    XPN_DEBUG_END_CUSTOM("%d", fd)

    return res;
}



int xpn_simple_stat(const char * path, struct stat * sb)
{
    char abs_path[PATH_MAX];
    int res = -1;

    XPN_DEBUG_BEGIN_ARGS1(path);

    if ((path == NULL) || (strlen(path) == 0))
    {
        errno = ENOENT;
        XPN_DEBUG_END_ARGS1(path)
        return -1;
    }

    if (sb == NULL) 
    {
        errno = ENOENT;
        XPN_DEBUG_END_ARGS1(path)
        return -1;
    }

    res = XpnGetAbsolutePath(path, abs_path); // this function generates the absolute path
    if (res < 0) 
    {
        xpn_err(XPNERR_PATH_NOEXIST);
        XPN_DEBUG_END_ARGS1(path)
        return res;
    }

    res = XpnGetAtribPath(abs_path, sb);
    if (res < 0) 
    {
        xpn_err(XPNERR_PATH_NOEXIST); //TODO: review error code
        XPN_DEBUG_END_ARGS1(path)
        return res;
    }

    XPN_DEBUG_END_ARGS1(path)
    return res;
}



int xpn_simple_chown(__attribute__((__unused__)) const char * path, __attribute__((__unused__)) uid_t owner, __attribute__((__unused__)) gid_t group) 
{
    // TODO
    return 0;
}



int xpn_simple_fchown(int __attribute__((__unused__)) fd, __attribute__((__unused__)) uid_t owner, __attribute__((__unused__)) gid_t group) 
{
    // TODO
    return 0;
}



int xpn_simple_chmod(__attribute__((__unused__)) const char * path, __attribute__((__unused__)) mode_t mode) 
{
    // TODO
    return 0;
}



int xpn_simple_fchmod(__attribute__((__unused__)) int fd, __attribute__((__unused__)) mode_t mode) 
{
    // TODO
    return 0;
}



int xpn_simple_truncate(__attribute__((__unused__)) const char * path, __attribute__((__unused__)) off_t length) 
{
    // TODO
    return 0;
}



int xpn_simple_ftruncate(__attribute__((__unused__)) int fd, __attribute__((__unused__)) off_t length) 
{
    // TODO
    return 0;
}



int xpn_simple_dup(int fd) {
    int i;

    if ((fd > XPN_MAX_FILE - 1) || (fd < 0)) 
    {
        return -1;
    }

    if (xpn_file_table[fd] == NULL) 
    {
        return -1;
    }

    i = 0;
    while ((i < XPN_MAX_FILE - 1) && (xpn_file_table[i] != NULL)) 
    {
        i++;
    }
    if (i == XPN_MAX_FILE) 
    {
        // xpn_err() ?
        return -1;
    }
    xpn_file_table[i] = xpn_file_table[fd];
    xpn_file_table[fd] -> links++;

    return i;
}



int xpn_simple_dup2(int fd, int fd2) {
    if ((fd > XPN_MAX_FILE - 1) || (fd < 0)) 
    {
        return -1;
    }
    if (xpn_file_table[fd] == NULL) 
    {
        return -1;
    }
    if ((fd2 > XPN_MAX_FILE - 1) || (fd2 < 0)) 
    {
        return -1;
    }
    if (xpn_file_table[fd2] != NULL) 
    {
        return -1;
    }

    xpn_file_table[fd2] = xpn_file_table[fd];
    xpn_file_table[fd] -> links++;

    return 0;
}