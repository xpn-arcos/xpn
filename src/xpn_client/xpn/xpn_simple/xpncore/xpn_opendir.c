
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


#include "xpn.h"
#include "xpn/xpn_simple/xpn_open.h"
#include "xpn/xpn_simple/xpn_opendir.h"
#include "xpn/xpn_simple/xpn_policy_opendir.h"


DIR *xpn_simple_opendir(const char *path)
{
  DIR *dirp = NULL;
  int res = -1;
  //char path_aux[255];
  char path_aux[PATH_MAX];

  XPN_DEBUG_BEGIN_ARGS1(path)

  if ((path == NULL)||(strlen(path)==0)||(strlen(path)>PATH_MAX))
  {
    errno = ENOENT;
    XPN_DEBUG_END_ARGS1(path)
    return NULL;
  }

  strcpy(path_aux,path);

  //if(path_aux[strlen(path_aux)-1] != '/')
  //    path_aux[strlen(path_aux)-1] = '/';

  if (path_aux[strlen(path)-1] != '/')
  {
    path_aux[strlen(path)] = '/';
    path_aux[strlen(path)+1] = '\0';
  }

  res = xpn_simple_open(path_aux, O_RDONLY, 0);
  if (res < 0)
  {
    errno = ENOENT;
    XPN_DEBUG_END_ARGS1(path)
    return NULL;
  }

  dirp = (DIR *)malloc(sizeof(DIR));
  if (dirp == NULL)
  {
    XPN_DEBUG_END_ARGS1(path)
    return NULL;
  }

  // fill the dirp info
  dirp->fd   = res;
  dirp->path = strdup(path_aux) ;

  XPN_DEBUG_END_ARGS1(path)

  return dirp;
}

struct dirent* xpn_simple_readdir(DIR *dirp)
{
  int res;
  struct dirent *dirnt = NULL;

  XPN_DEBUG_BEGIN

  if((NULL == dirp)||(dirp->fd<0)||(dirp->fd>XPN_MAX_FILE-1)){
    // set errno
    return NULL;
  }

  if(xpn_file_table[dirp->fd] == NULL){
    // xpn_err
    return NULL;
  }


  dirnt = (struct dirent *)malloc(sizeof(struct dirent));
  memset(dirnt, 0, sizeof(struct dirent));

  /*
  printf("xpn: struct dirent {             %d-%p\n",sizeof(struct dirent),dirnt);
  printf("    ino_t          d_ino;        %d+%d\n",sizeof(ino_t),(int)&dirnt->d_ino-(int)dirnt);
  printf("    off_t          d_off;        %d+%d\n",sizeof(off_t),(int)&dirnt->d_off-(int)dirnt);
  printf("    unsigned short d_reclen;     %d+%d\n",sizeof(unsigned short),(int)&dirnt->d_reclen-(int)dirnt);
  printf("    unsigned char  d_type;       %d+%d\n",sizeof(unsigned char),(int)&dirnt->d_type-(int)dirnt);
  printf("    char           d_name[256];  %d+%d\n",sizeof(struct dirent)-sizeof(ino_t)-sizeof(off_t)-sizeof(unsigned short)-sizeof(unsigned char),(int)&dirnt->d_name-(int)dirnt);
  printf("}\n");
  */

  res = XpnGetEntry(dirp->fd, dirnt);
  if(res != 0)
  {
    free(dirnt);
    XPN_DEBUG_END
    return NULL;
  }

  XPN_DEBUG_END
  return dirnt;
}

int xpn_simple_closedir(DIR *dirp)
{
  int i;

  if((NULL == dirp)||(dirp->fd<0)||(dirp->fd>XPN_MAX_FILE-1)){
    // set errno
    return -1;
  }

  if(xpn_file_table[dirp->fd] == NULL){
    // xpn_err
    return -1;
  }

  xpn_file_table[dirp->fd]->links--;

  if(xpn_file_table[dirp->fd]->links == 0)
  {
    for(i=0;i<xpn_file_table[dirp->fd]->data_vfh->n_nfih;i++)
    {
      if(xpn_file_table[dirp->fd]->data_vfh->nfih[i] != NULL)
      {
        if(xpn_file_table[dirp->fd]->data_vfh->nfih[i]->priv_fh != NULL){
          xpn_file_table[dirp->fd]->data_vfh->nfih[i]->server->ops->nfi_closedir( xpn_file_table[dirp->fd]->data_vfh->nfih[i]->server, xpn_file_table[dirp->fd]->data_vfh->nfih[i]);
        }

        free(xpn_file_table[dirp->fd]->data_vfh->nfih[i]);
      }
    }

    free(xpn_file_table[dirp->fd]->data_vfh->nfih);
    free(xpn_file_table[dirp->fd]->data_vfh);

    free(xpn_file_table[dirp->fd]->mdata->policy);
    free(xpn_file_table[dirp->fd]->mdata);

    free(xpn_file_table[dirp->fd]);
    xpn_file_table[dirp->fd] = NULL;
  }

  free(dirp->path) ;
  free(dirp);

  // set errno
  xpn_err(XPN_OK);
  return 0;
  //return -1;
}

void xpn_simple_rewinddir(__attribute__((__unused__)) DIR *dirp)
{
   // TODO
}

