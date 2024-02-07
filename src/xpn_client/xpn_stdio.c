
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


   /* ... Include / Inclusion ........................................... */

      #include "xpn.h"
      #include "xpn_client/xpn/xpn_simple/xpn_simple_lib.h"
      #include "xpn_api_mutex.h"


   /* ... Functions / Funciones ......................................... */


    //
    // fopen - fclose
    //

    FILE * xpn_fopen ( const char *name, const char *mode )
    {
      FILE *ret = NULL ;

      XPN_API_LOCK() ;
      ret = xpn_simple_fopen(name, mode) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_fclose ( FILE *fp )
    {
      int ret ;

      XPN_API_LOCK() ;
      ret = xpn_simple_fclose(fp) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }


    //
    // fread - fwrite
    //

    size_t xpn_fread ( void *ptr, size_t size, size_t nmemb, register FILE *stream )
    {
      size_t ret ;

      XPN_API_LOCK() ;
      ret = xpn_simple_fread(ptr, size, nmemb, stream);
      XPN_API_UNLOCK() ;

      return ret ;
    }

    size_t xpn_fwrite ( const void *ptr, size_t size, size_t nmemb, register FILE *stream )
    {
      size_t ret ;

      XPN_API_LOCK() ;
      ret = xpn_simple_fwrite(ptr, size, nmemb, stream);
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_fflush ( FILE *stream )
    {
      int ret ;

      XPN_API_LOCK() ;
      ret = xpn_simple_fflush(stream);
      XPN_API_UNLOCK() ;

      return ret ;
    }


    //
    // fseek - ftell
    //

    int xpn_fseek ( FILE *stream, long int offset, int whence )
    {
      int ret ;

      XPN_API_LOCK() ;
      ret = xpn_simple_fseek(stream, offset, whence);
      XPN_API_UNLOCK() ;

      return ret ;
    }

    long xpn_ftell ( FILE *stream )
    {
      long ret ;

      XPN_API_LOCK() ;
      ret = xpn_simple_ftell(stream);
      XPN_API_UNLOCK() ;

      return ret ;
    }

    void xpn_rewind ( FILE *stream )
    {
      XPN_API_LOCK() ;
      xpn_simple_rewind(stream);
      XPN_API_UNLOCK() ;
    }


    //
    // flush - preload
    //

    int xpn_preload ( const char *virtual_path, const char *storage_path )
    {
      int ret ;

      XPN_API_LOCK() ;
      ret = xpn_simple_preload(virtual_path, storage_path);
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_flush ( const char *virtual_path, const char *storage_path )
    {
      int ret ;

      XPN_API_LOCK() ;
      ret = xpn_simple_flush(virtual_path, storage_path);
      XPN_API_UNLOCK() ;

      return ret ;
    }


    //
    // fgetc - fgets - getc
    //

    int xpn_fgetc ( FILE *stream )
    {
      int ret ;

      XPN_API_LOCK() ;
      ret = xpn_simple_fgetc(stream);
      XPN_API_UNLOCK() ;

      return ret ;
    }

    char * xpn_fgets ( char *s, int tam, FILE *stream )
    {
      char * ret ;

      XPN_API_LOCK() ;
      ret = xpn_simple_fgets(s, tam, stream);
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_getc ( FILE *stream )
    {
      int ret ;

      XPN_API_LOCK() ;
      ret = xpn_simple_getc(stream);
      XPN_API_UNLOCK() ;

      return ret ;
    }


    //
    // fileno - ferror
    //

    int xpn_fileno ( FILE *stream )
    {
      int ret ;

      XPN_API_LOCK() ;
      ret = xpn_simple_fileno(stream);
      XPN_API_UNLOCK() ;

      return ret ;
    }


    int xpn_ferror ( FILE *stream )
    {
      int ret ;

      XPN_API_LOCK() ;
      ret = xpn_simple_ferror(stream);
      XPN_API_UNLOCK() ;

      return ret ;
    }


   /* ................................................................... */


