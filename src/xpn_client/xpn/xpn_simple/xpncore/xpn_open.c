
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



int XpnSearchSlotFile(int pd, char * path, struct xpn_fh * vfh, struct xpn_metadata * mdata, int flags, mode_t mode) 
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
    xpn_file_table[i] -> flags = flags;
    xpn_file_table[i] -> mode = mode;
    xpn_file_table[i] -> links = 1;
    xpn_file_table[i] -> part = XpnSearchPart(pd);
    xpn_file_table[i] -> offset = 0;
    xpn_file_table[i] -> block_size = xpn_file_table[i] -> part -> block_size;
    xpn_file_table[i] -> mdata = mdata;
    xpn_file_table[i] -> data_vfh = vfh;

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
         res = -1;
    else res = i;

    XPN_DEBUG_END

    return res;
}

// Check if serv is affected by op, in other words if it has data and have to do operations in that serv
// For ops like open remove rename
int XpnCheckServAffectedByOp(struct xpn_metadata *mdata, int master_dir, int master_node, int n_serv, int serv)
{
    int current_serv, i;
    // if not have mdata is dir so all servers affected
    if (!XPN_CHECK_MAGIC_NUMBER(mdata)){
        XPN_DEBUG("Serv %d: is dir", serv);
        return 1;
    }

    // Files created for readdir in master_dir with replications
    for (i = 0; i < mdata->replication_level+1; i++) 
    {
        current_serv = (i + master_dir) % n_serv;
        if (current_serv == serv){
            XPN_DEBUG("Serv %d: file in master_dir", serv);
            return 1;
        }
    }

    // Files with metadata with replications
    for (i = 0; i < mdata->replication_level+1; i++) 
    {
        current_serv = (i + master_node) % n_serv;
        if (current_serv == serv){
            XPN_DEBUG("Serv %d: file has metadata", serv);
            return 1;
        }
    }

    // For simplicity if has malleability all servers
    if (mdata->data_nserv[1] != 0){
        XPN_DEBUG("Serv %d: file have malleability", serv);
        return 1;
    }

    // Calculate if has data in that server, the file must exist
    off_t local_offset;
    int aux_serv;
    for (i = 0; i < n_serv; i++)
    {
        off_t offset = mdata->block_size * i;
        if (offset > mdata->file_size){
            break;
        }
        for (int j = 0; j < mdata->replication_level+1; j++)
        {
            XpnCalculateBlockMdata(mdata, offset, j, &local_offset, &aux_serv);
            if (aux_serv == serv){
                XPN_DEBUG("Serv %d: file have data in serv", serv);
                return 1;
            }
        }
    }
    return 0;
}

/*****************************************************************/

int xpn_internal_open(const char * path, struct xpn_fh * vfh, struct xpn_metadata * mdata, int flags, mode_t mode) 
{
    char abs_path[PATH_MAX];
    char url_serv[PATH_MAX];
    struct nfi_server *servers;
    int n, pd, i, j, master_node, master_dir;
    int res = -1, err;

    XPN_DEBUG_BEGIN_CUSTOM("%s, %d, %d", path, flags, mode);

    res = XpnGetAbsolutePath(path, abs_path); // this function generates the absolute path
    if (res < 0) 
    {
        errno = ENOENT;
        XPN_DEBUG_END_ARGS1(path);
        return res;
    }

    pd = XpnGetPartition(abs_path); // returns partition id and remove partition name from abs_path
    if (pd < 0) 
    {
        errno = ENOENT;
        XPN_DEBUG_END_ARGS1(path);
        return pd;
    }

    servers = NULL;
    n = XpnGetServers(pd, -1, &servers);
    if (n <= 0) 
    {
        XPN_DEBUG_END_ARGS1(path);
        return res;
    }
    // Metadata
    if (mdata == NULL) {
        mdata = (struct xpn_metadata * ) malloc(sizeof(struct xpn_metadata));
        if (mdata == NULL) 
        {
            goto error_xpn_internal_open;
        }
        memset(mdata, 0, sizeof(*mdata));
    }
    if ((O_DIRECTORY != (flags & O_DIRECTORY)))
    {
        // read metadata only in files
        res = XpnReadMetadata(mdata, n, servers, abs_path, XpnSearchPart(pd)->replication_level);
        if (res < 0 && O_CREAT != (flags & O_CREAT)){
            goto error_xpn_internal_open;
        }
        // create metadata if not exits
        if (!XPN_CHECK_MAGIC_NUMBER(mdata)){
            XpnCreateMetadata(mdata, pd, abs_path);
        }
    }

    if (vfh == NULL) {
        vfh = (struct xpn_fh * ) malloc(sizeof(struct xpn_fh));
        if (vfh == NULL) 
        {
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

    // Open file only in master server
    master_node = hash(abs_path, n, 1);
    master_dir = hash(abs_path, n, 0);

    // if create it has to create in the servers
    if (O_CREAT == (flags & O_CREAT)){
        for (int i = 0; i < n; i++)
        {
            if (XpnCheckServAffectedByOp(mdata, master_dir, master_node, n, i) == 1){
                vfh -> nfih[i] = (struct nfi_fhandle *) malloc(sizeof(struct nfi_fhandle));
                if(vfh -> nfih[i] == NULL)
                {
                    res = -1;
                    goto error_xpn_internal_open;
                }
                servers[i].wrk->thread = servers[i].xpn_thread;
                XpnGetURLServer(&servers[i], abs_path, url_serv);
                nfi_worker_do_open(servers[i].wrk, url_serv, flags, mode, vfh->nfih[i]);
            }
        }

        err = 0;
        for (int i = 0; i < n; i++)
        {
            if (XpnCheckServAffectedByOp(mdata, master_dir, master_node, n, i) == 1){
                res = nfiworker_wait(servers[i].wrk);
                if (res < 0)
                {
                    err = 1;
                }
            }
        }
        if (err == 1)
        {
            res = -1;
            goto error_xpn_internal_open;
        }
    }else{
        // else only open in one
        vfh -> nfih[master_dir] = (struct nfi_fhandle *) malloc(sizeof(struct nfi_fhandle));
        if(vfh -> nfih[master_dir] == NULL)
        {
            res = -1;
            goto error_xpn_internal_open;
        }
                
        servers[master_dir].wrk->thread = servers[master_dir].xpn_thread;
        
        XpnGetURLServer(&servers[master_dir], abs_path, url_serv);
        XPN_DEBUG("Open in %d serv", master_dir);
        if (O_DIRECTORY == (flags & O_DIRECTORY))
            nfi_worker_do_opendir(servers[master_dir].wrk, url_serv, vfh->nfih[master_dir]);
        else
            nfi_worker_do_open(servers[master_dir].wrk, url_serv, flags, mode, vfh->nfih[master_dir]);
        res = nfiworker_wait(servers[master_dir].wrk);
        if (res < 0) {
            goto error_xpn_internal_open;
        }
    }

    // Metadata
    if (O_CREAT == (flags & O_CREAT) && O_DIRECTORY != (flags & O_DIRECTORY))
    {   
        res = XpnUpdateMetadata(mdata, n, servers, abs_path, XpnSearchPart(pd)->replication_level, 0);
        if (res < 0){
            goto error_xpn_internal_open;
        }
    }

    res = XpnSearchSlotFile(pd, abs_path, vfh, mdata, flags, mode);
    
    XPN_DEBUG_END_ARGS1(path);
    return res;

error_xpn_internal_open:
    if (vfh != NULL){
        if (vfh -> nfih != NULL){
            for (j=0; j<n; j++){ 
                FREE_AND_NULL(vfh -> nfih[j]);
            }
        }
        FREE_AND_NULL(vfh -> nfih);
    }
    FREE_AND_NULL(vfh);
    FREE_AND_NULL(mdata);
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
    struct nfi_server *servers;
    struct xpn_metadata mdata = {0};
    int master_node, master_dir;

    if (path == NULL) 
    {
        errno = EINVAL;
        return -1;
    }

    res = XpnGetAbsolutePath(path, abs_path); // esta funcion genera el path absoluto
    if (res < 0) 
    {
        errno = ENOENT;
        return -1;
    }

    pd = XpnGetPartition(abs_path); // return partition's id
    if (pd < 0) 
    {
        errno = ENOENT;
        return -1;
    }

    servers = NULL;
    n = XpnGetServers(pd, -1, &servers);
    if (n <= 0) 
    {
        return -1;
    }

    XpnReadMetadata(&mdata, n, servers, abs_path, XpnSearchPart(pd)->replication_level);
    master_node = hash((char *)abs_path, n, 1);
    master_dir = hash((char *)abs_path, n, 0);

    for (i = 0; i < n; i++)
    {
        if (XpnCheckServAffectedByOp(&mdata, master_dir, master_node, n, i) == 1){
            if (master_node == i){
                servers[i].wrk->arg.is_master_node = 1;
            }else{
                servers[i].wrk->arg.is_master_node = 0;
            }
            servers[i].wrk->arg.master_node = master_node;
            XpnGetURLServer(&servers[i], abs_path, url_serv);
            nfi_worker_do_remove(servers[i].wrk, url_serv);
        }
    }

    err = 0;
    for (i = 0; i < n; i++)
    {
        if (XpnCheckServAffectedByOp(&mdata, master_dir, master_node, n, i) == 1){
            res = nfiworker_wait(servers[i].wrk);
            if (res < 0)
            {
                err = 1;
            }
        }
    }
    
    if (err == 1){
        return -1;
    }

    return 0;
}


int xpn_simple_creat(const char * path, mode_t perm) 
{
    int res;

    XPN_DEBUG_BEGIN_ARGS1(path);

    res = xpn_simple_open(path, O_WRONLY|O_CREAT|O_TRUNC, perm);
    if (res < 0) 
    {
        XPN_DEBUG_END_ARGS1(path)
        return res;
    }

    XPN_DEBUG_END_ARGS1(path)
    return res;
}


int xpn_simple_open(const char * path, int flags, mode_t mode) 
{
    struct xpn_fh * vfh;
    struct xpn_metadata * mdata;
    int res = -1;

    XPN_DEBUG_BEGIN_ARGS1(path);

    if ((path == NULL) || (strlen(path) > PATH_MAX)) 
    {
        XPN_DEBUG_END_ARGS1(path)
        errno = EINVAL;
        return res;
    }

    vfh = NULL;
    mdata = NULL;

    res = xpn_internal_open(path, vfh, mdata, flags, mode);

    XPN_DEBUG_END_ARGS1(path);

    return res;
}


int xpn_simple_close(int fd) 
{
    int i;

    XPN_DEBUG_BEGIN_CUSTOM("%d", fd)

    if ((fd < 0) || (fd > XPN_MAX_FILE - 1)) 
    {
        errno = EBADF;
        XPN_DEBUG_END_CUSTOM("%d", fd)
        return -1;
    }

    if (xpn_file_table[fd] == NULL) 
    {
        errno = EBADF;
        XPN_DEBUG_END_CUSTOM("%d", fd)
        return -1;
    }

    xpn_file_table[fd] -> links--;
    if (xpn_file_table[fd] -> links == 0) 
    {
        for (i = 0; i < xpn_file_table[fd] -> data_vfh -> n_nfih; i++) 
        {
            if (xpn_file_table[fd] -> data_vfh -> nfih[i] != NULL) 
            {
                if(xpn_file_table[fd]->data_vfh->nfih[i]->priv_fh != NULL){
                    xpn_file_table[fd]->data_vfh->nfih[i]->server->ops->nfi_close( xpn_file_table[fd]->data_vfh->nfih[i]->server, xpn_file_table[fd]->data_vfh->nfih[i]);
                }
                free(xpn_file_table[fd] -> data_vfh -> nfih[i]);
            }
        }

        free(xpn_file_table[fd] -> data_vfh -> nfih);
        free(xpn_file_table[fd] -> data_vfh);
        free(xpn_file_table[fd] -> mdata);
        free(xpn_file_table[fd]);
        xpn_file_table[fd] = NULL;
    }

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
    struct nfi_server *servers;
    struct xpn_metadata mdata = {0};
    int res, err, i, n, pd, newpd;
    int master_dir, master_node;

    XPN_DEBUG_BEGIN_CUSTOM("(%s %s)", path, newpath);

    if (path == NULL) 
    {
        errno = EINVAL;
        XPN_DEBUG_END;
        return -1;
    }

    if (newpath == NULL) 
    {
        errno = EINVAL;
        XPN_DEBUG_END;
        return -1;
    }

    res = XpnGetAbsolutePath(path, abs_path); // esta funcion genera el path absoluto
    if (res < 0) 
    {
        errno = ENOENT;
        XPN_DEBUG_END;
        return -1;
    }

    res = XpnGetAbsolutePath(newpath, newabs_path); // esta funcion genera el path absoluto
    if (res < 0) 
    {
        errno = ENOENT;
        XPN_DEBUG_END;
        return -1;
    }

    pd = XpnGetPartition(abs_path); // return partition's id
    if (pd < 0)
    {
        errno = ENOENT;
        XPN_DEBUG_END;
        return -1;
    }

    newpd = XpnGetPartition(newabs_path); // return partition's id
    if (newpd < 0) 
    {
        errno = ENOENT;
        XPN_DEBUG_END;
        return -1;
    }

    if (pd != newpd) 
    {
        XPN_DEBUG_END;
        return -1;
    }

    servers = NULL;
    n = XpnGetServers(pd, -1, &servers);
    if (n <= 0) {
        XPN_DEBUG_END;
        return -1;
    }

    XpnReadMetadata(&mdata, n, servers, abs_path, XpnSearchPart(pd)->replication_level);
    master_dir = hash((char *)abs_path, n, 0);
    master_node = hash((char *)abs_path, n, 1);

    for (i = 0; i < n; i++)
    {
        if (XpnCheckServAffectedByOp(&mdata, master_dir, master_node, n, i) == 1){
            XpnGetURLServer(&servers[i], abs_path, url_serv);
            XpnGetURLServer(&servers[i], newabs_path, newurl_serv);
            nfi_worker_do_rename(servers[i].wrk, url_serv, newurl_serv);
        }
    }

    err = 0;
    for (i = 0; i < n; i++)
    {
        if (XpnCheckServAffectedByOp(&mdata, master_dir, master_node, n, i) == 1){
            res = nfiworker_wait(servers[i].wrk);
            if (res < 0)
            {
                err = 1;
            }
        }
    }

    if (err == 1){
        return -1;
    }    

    //Check magic number if is dir not have it so no update metadata
    if (XPN_CHECK_MAGIC_NUMBER(&mdata)){
        XpnUpdateMetadata(&mdata, n, servers, newabs_path, XpnSearchPart(pd)->replication_level, 0);
    }

    XPN_DEBUG_END;
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
        errno = EINVAL;
        XPN_DEBUG_END_ARGS1(path)
        return -1;
    }

    if (sb == NULL) 
    {
        errno = EINVAL;
        XPN_DEBUG_END_ARGS1(path)
        return -1;
    }

    res = XpnGetAbsolutePath(path, abs_path); // this function generates the absolute path
    if (res < 0) 
    {
        errno = ENOENT;
        XPN_DEBUG_END_ARGS1(path)
        return res;
    }

    res = XpnGetAtribPath(abs_path, sb);
    if (res < 0) 
    {
        errno = ENOENT;
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
