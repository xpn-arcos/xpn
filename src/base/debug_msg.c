
/*
 *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

      #include "base/debug_msg.h"


   /* ... Functions / Funciones ......................................... */

      void  debug_msg_init ( void )
      {
           setbuf(stdout, NULL) ;
           setbuf(stderr, NULL) ;
      }


      int  debug_msg_printf ( int src_type, long src_pid, char *src_fname, long src_line, FILE *fd, char *msg_fmt, ... )
      {
         va_list valist ;
         int ret ;
      
         va_start(valist, msg_fmt) ;
         switch (src_type)
         {
              case  3:
                    fprintf(fd, "[%ld][%s:%4ld] [INFO] ", src_pid, src_fname, src_line) ;
                    ret = vfprintf(fd, msg_fmt, valist) ;
                    break;
      
              case  2:
                    fprintf(fd, "[%ld][%s:%4ld] [WARN] ", src_pid, src_fname, src_line) ;
                    ret = vfprintf(fd, msg_fmt, valist) ;
                    break;
      
              case  1:
                    fprintf(fd, "[%ld][%s:%4ld] [ERROR] ", src_pid, src_fname, src_line) ;
                    ret = vfprintf(fd, msg_fmt, valist) ;
                    break;
      
              default:
                    ret = vfprintf(fd, msg_fmt, valist) ;
                    break;
         }
         va_end(valist) ;
      
         // fflush(fd) ;
      
         return ret ;
      }


   /* ................................................................... */

