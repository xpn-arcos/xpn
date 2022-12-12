
/*
 *  Copyright 2020-2021 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

      #include "tcp_server_utils.h"


   /* ... Functions / Funciones ......................................... */

      void tcp_server_utils_init ( void )
      {
	  setbuf(stdout,NULL) ;
	  setbuf(stderr,NULL) ;
      }

      int tcp_server_utils_printf ( int src_type, char *src_fname, long src_line, FILE *fd, char *msg_fmt, ... )
      {
         va_list valist ;
         int ret ;
      
         va_start(valist, msg_fmt) ;
         switch (src_type)
         {
              case  3:
                    fprintf(fd, "[%s:%4ld] [INFO] ", src_fname, src_line) ;
                    ret = vfprintf(fd, msg_fmt, valist) ;
           	    break;
      
              case  2:
                    fprintf(fd, "[%s:%4ld] [WARN] ", src_fname, src_line) ;
                    ret = vfprintf(fd, msg_fmt, valist) ;
           	    break;
      
              case  1:
                    fprintf(fd, "[%s:%4ld] [ERROR] ", src_fname, src_line) ;
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
      
      long tcp_server_utils_get_time ( void )
      {
          struct timeval timenow ;
      
          // get timestamp
          gettimeofday(&timenow, NULL) ;
      
          // return timestamp
          return (long)timenow.tv_sec * 1000 + (long)timenow.tv_usec / 1000 ;
      }


   /* ................................................................... */

