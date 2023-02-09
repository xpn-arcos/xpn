
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


   /* ... Include / Inclusion ........................................... */

      #include "xpn.h"
      #include "xpn_client/xpn/xpn_simple/xpn_simple_lib.h"


   /* ... Functions / Funciones ......................................... */


      //
      // fopen - fclose
      //

      FILE * xpn_fopen(const char *name, const char *mode)
      {
          return xpn_simple_fopen(name, mode) ;
      }

      int xpn_fclose(FILE *fp)
      {
          return xpn_simple_fclose(fp) ;
      }


      //
      // fread - fwrite
      //

      size_t xpn_fread ( void *ptr, size_t size, size_t nmemb, register FILE *stream )
      {
          return xpn_simple_fread(ptr, size, nmemb, stream) ;
      }

      size_t xpn_fwrite ( const void *ptr, size_t size, size_t nmemb, register FILE *stream )
      {
          return xpn_simple_fwrite(ptr, size, nmemb, stream) ;
      }

/*
      int xpn_fflush(FILE *stream)
      {
          return xpn_simple_fflush(stream) ;
      }
*/


      //
      // fseek - ftell
      //

/*
      int xpn_fseek(FILE *stream, long int offset, int whence)
      {
          return xpn_simple_fseek(stream, offset, whence) ;
      }

      long xpn_ftell(FILE *stream)
      {
          return xpn_simple_ftell(stream) ;
      }
*/


      //
      // flush - preload
      //

      int xpn_preload(const char *virtual_path, const char *storage_path)
      {
          return xpn_simple_preload(virtual_path, storage_path) ;
      }

      int xpn_flush(const char *virtual_path, const char *storage_path)
      {
          return xpn_simple_flush(virtual_path, storage_path) ;
      }


   /* ................................................................... */

