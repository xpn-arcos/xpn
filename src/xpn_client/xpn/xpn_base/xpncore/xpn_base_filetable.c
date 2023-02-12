
  /*
   *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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


#include "xpn/xpn_base/xpn_base_filetable.h"


struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];

int xpn_init_file_table ( void )
{
  int i;

  for (i=0;i<XPN_MAX_FILE;i++) {
     xpn_file_table[i] = NULL;
  }

  return 0;
}

int xpn_destroy_file_table()
{
  int i, j;

  for (i=0;i<XPN_MAX_FILE;i++)
  {
    if (xpn_file_table[i] != NULL)
    {
      for (j=0;j<xpn_file_table[i]->data_vfh->n_nfih;j++)
      {
        if (xpn_file_table[i]->data_vfh->nfih[j] != NULL)
        {
          if (xpn_file_table[i]->data_vfh->nfih[j]->priv_fh != NULL){
            free(xpn_file_table[i]->data_vfh->nfih[j]->priv_fh);
          }
          free(xpn_file_table[i]->data_vfh->nfih[j]);
        }
      }

      free(xpn_file_table[i]->data_vfh->nfih);
      free(xpn_file_table[i]->data_vfh);
      free(xpn_file_table[i]->mdata->policy);
      free(xpn_file_table[i]->mdata);
      free(xpn_file_table[i]);

      xpn_file_table[i] = NULL;
    }
  }

  return 0;
}


/*****************************************************************/

void XpnShowFileTable()
{
  int i=0;

  while ((i<XPN_MAX_FILE) && (xpn_file_table[i] != NULL) && (xpn_file_table[i]->path != NULL))
  {
    XPN_DEBUG("%d %s", i, xpn_file_table[i]->path)
    i++;
  }
}

int XpnSearchSlotFile(int pd, char *path, struct xpn_fh *vfh, struct xpn_metadata *mdata, int mode)
{
  int i, res;

  XPN_DEBUG_BEGIN_ARGS1(path)

  i=0;
  while((i<XPN_MAX_FILE-1)&&(xpn_file_table[i] != NULL)){ // FIXME? Por que i<XPN_MAX_FILE-1, no deberia ser i<XPN_MAX_FILE
    i++;
  }

  if(i == XPN_MAX_FILE){
    // xpn_err() ? 
    return -1;
  }

  xpn_file_table[i]= (struct xpn_filedesc *)malloc(sizeof(struct xpn_filedesc));

  if(xpn_file_table[i] == NULL){
    // xpn_err() ? 
    return -1;
  }

  xpn_file_table[i]->id           = i;
  xpn_file_table[i]->type         = mdata->type;
  strcpy(xpn_file_table[i]->path, path);
  xpn_file_table[i]->mode         = mode;
  xpn_file_table[i]->links        = 1;
  xpn_file_table[i]->part         = XpnSearchPart(pd);
  xpn_file_table[i]->offset       = 0;
  xpn_file_table[i]->block_size   = xpn_file_table[i]->part->block_size;
  xpn_file_table[i]->mdata        = mdata;
  xpn_file_table[i]->data_vfh     = vfh;
  xpn_file_table[i]->size_threads = XpnGetSizeThreads(xpn_file_table[i]->part);

  res = i;
  XPN_DEBUG_END_ARGS1(path)

  return res;
}

int XpnSearchFile(const char *path)
{
  int res, i=0;

  XPN_DEBUG_BEGIN_ARGS1(path)

  while (i<XPN_MAX_FILE) 
  {
    if ( (xpn_file_table[i] != NULL) && (xpn_file_table[i]->path != NULL) && (strcmp(xpn_file_table[i]->path, path) == 0) ){
      break;
    }

    i++;
  }

  if(i == XPN_MAX_FILE){
    res = -1;
  } 
  else {
    res = i;
  }

  XPN_DEBUG_END

  return res;
}

