
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

     #include "xpn/xpn_simple/xpn_file.h"


  /* ... Glob. variables / Variables globales .......................... */

     struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];

     pthread_mutex_t x_d_lock = PTHREAD_MUTEX_INITIALIZER;


  /* ... Functions / Funciones ......................................... */

     int xpn_init_file_table ( void )
     {
         int i;
     
         for (i=0; i<XPN_MAX_FILE; i++)
         {
              xpn_file_table[i] = NULL;
         }
     
         return 0;
     }
     
     int xpn_destroy_file_table ( void )
     {
         int i, j;
     
         pthread_mutex_lock(&x_d_lock);
     
         for (i=0; i<XPN_MAX_FILE; i++)
         {
             if (NULL == xpn_file_table[i]) {
                 continue ;
             }
             if (NULL == xpn_file_table[i]->data_vfh) {
                 continue ;
             }
     
             for (j=0; j<xpn_file_table[i]->data_vfh->n_nfih; j++)
             {
                  if (NULL == xpn_file_table[i]->data_vfh->nfih[j]) {
                      continue ;
                  }
     
                  FREE_AND_NULL(xpn_file_table[i]->data_vfh->nfih[j]->priv_fh) ;
                  FREE_AND_NULL(xpn_file_table[i]->data_vfh->nfih[j]) ;
             }
     
             FREE_AND_NULL(xpn_file_table[i]->data_vfh->nfih) ;
             FREE_AND_NULL(xpn_file_table[i]->data_vfh) ;
             FREE_AND_NULL(xpn_file_table[i]->mdata) ;
             FREE_AND_NULL(xpn_file_table[i]) ;
         }
     
         pthread_mutex_unlock(&x_d_lock);
     
         return 0;
     }


  /* ................................................................... */

