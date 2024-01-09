
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


#include "xpn/xpn_simple/xpn_file.h"


struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];

int xpn_init_file_table()
{
  int i;

  for(i=0;i<XPN_MAX_FILE;i++){
    xpn_file_table[i] = NULL;
  }

  return 0;
}

int xpn_destroy_file_table()
{
  int i,j;

  for(i=0;i<XPN_MAX_FILE;i++)
  {
    if(xpn_file_table[i] != NULL)
    {
      for(j=0;j<xpn_file_table[i]->data_vfh->n_nfih;j++)
      {
        if(xpn_file_table[i]->data_vfh->nfih[j] != NULL)
        {
          if(xpn_file_table[i]->data_vfh->nfih[j]->priv_fh != NULL){
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

