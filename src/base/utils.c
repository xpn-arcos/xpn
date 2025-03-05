
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

   #include "utils.h"


/* ... Functions / Funciones ......................................... */

   long utils_get_time ( void )
   {
      struct timeval timenow;

      // get timestamp
      gettimeofday(&timenow, NULL);

      // return timestamp
      return (long)timenow.tv_sec * 1000 + (long)timenow.tv_usec / 1000;
   }

   int utils_str2int ( char *str_value, int default_value )
   {
       char *endptr;
       int   int_value;

       int_value = (int) strtol (str_value, &endptr, 10);
       if ( (endptr == str_value) || (*endptr != '\0') )
       {
             int_value = default_value ;
             printf("[utils_str2int] warning: value '%s' is not a number\n", str_value);
       }

       return int_value ;
   }

   int utils_getenv_int ( char *env_name, int default_value )
   {
      int env_int = default_value ;

      // env_name == NULL -> return default value
      if (NULL == env_name) {
          return default_value ;
      }

      // getenv(...) == NULL -> return default value
      char *env_value = getenv(env_name);
      if (NULL == env_value) {
          return default_value ;
      }

      // strtol fails -> return default value
      char *end ;
      env_int = strtol(env_value, &end, 10);
      if (*end != '\0') {
          printf("[utils_getenv_int] warning: environmental variable '%s' value '%s' is not a number\n", env_name, env_value);
          return default_value ;
      }

      return env_int;
   }


/* ................................................................... */

