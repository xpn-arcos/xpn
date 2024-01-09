
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

      #include "base/string_misc.h"


   /* ... Functions / Funciones ......................................... */


    /**
     * Return the string length of 'str'.
     * @param str the string.
     * @return the string length.
     */
     long   STRING_MISC_StrLen ( /*IN*/  char  *str )
     {
         /* check params */
         if (str == NULL)
             return 0L ;

         return (long)strlen(str) ;
     }

    /**
     * Return true if and only if the strings 'str1' and 'str2' are equals.
     * @param str1 the first string.
     * @param str2 the second string.
     * @return true (1) iff are equals or false (0) in other case.
     */
     int8_t STRING_MISC_Equal ( /*IN*/  char  *str1,
                                /*IN*/  char  *str2 )
     {
         /* easy cases */
         if (str1 == str2)
               return (1) ;
         if ( (str1 == NULL) || (str2 == NULL) )
               return (0) ;

         /* use strcmp */
         if ( (strcmp(str1,str2) == 0) )
               return (1) ;

         return (0) ;
     }

    /**
     * Return a string clone of 'str'.
     * @param str the string.
     * @return a clone of 'str'.
     */
     char  *STRING_MISC_StrDup ( /*IN*/ char  *str )
     {
         int  lenstr ;
         char *pch ;

         /* check params */
         if (NULL == str)
             return NULL;

         /* alloc memory for the clone... */
         lenstr = STRING_MISC_StrLen(str) + 1 ;
         pch = (char *)malloc(lenstr) ;
         if (NULL == pch)
             return NULL;

         /* copy contents */
         memmove(pch,str,lenstr) ;
         return pch ;
       }

       /**
         *
         *     Como 'strlen' pero permite usar
         *  un string con formato.
         *
         *     Like 'strlen' but also accept
         *  a string with format.
         *
         *     @param  format string format.
         *     @param  argl   format params.
         *     @return string legth.
         *
         */
       int   STRING_MISC_StrLenF
       (
         /*IN*/  char     *format,
         /*IN*/  va_list   argl
       )
       {
                int       ineed ;
         static FILE     *nullfd = NULL ;

         if (nullfd == NULL)
            {
              nullfd = fopen (NULL_DEVICE_PATH,"w") ;
              /*
               *   we will lost one file descriptor,
               *   because we never "fclose(nullfd)".
               */
            }

         if (strchr(format,'%') != NULL)
            {
              ineed = vfprintf(nullfd,format,argl) ;
            }
       else /* only a string, not format */
            {
              ineed = strlen(format) ;
            }

        return ineed ;
      }

      /**
       *
       *     Actua igual que 'vsprintf', pero pide memoria
       *  dinamica para el string donde se imprime.
       *  Retorna NULL si no pudo.
       *
       *     Like vsprintf, but request dynamic memory
       *  to write string elements.
       *
       *     @param  format string format.
       *     @param  argl   format params.
       *     @return string pointer or NULL if error.
       *
       */
      char *STRING_MISC_Dvsprintf
      (
        /*IN*/  char     *format,
        /*IN*/  va_list   argl
      )
      {
        char     *baux ;
        int       ineed ;

        ineed = STRING_MISC_StrLenF(format,argl) ;
        baux  = (char *)malloc(ineed+1) ;
        if (NULL == baux)
            return NULL ;

        vsprintf(baux,format,argl) ;

        return baux ;
      }

      /**
        *
        *     Igual que la anterior, solo varia los argumentos
        *
        *     Like before, but with open arguments.
        *
        *     @param  format string with message format.
        *     @return string pointer or NULL if error.
        *
        */
      char *STRING_MISC_Dsprintf
      (
        /*IN*/  char   *format,
        ...
      )
      {
        char   *saux ;
        va_list varg ;

        va_start(varg,format) ;
        saux = STRING_MISC_Dvsprintf(format,varg) ;
        va_end(varg) ;

        return saux ;
      }


  /* ...................................................................... */

