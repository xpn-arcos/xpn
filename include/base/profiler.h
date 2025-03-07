
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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


#ifndef _XPN_PROFILER_H
#define _XPN_PROFILER_H

  /* ... Include / Inclusion ........................................... */

  #include <stdio.h>
  #include <errno.h>
  #include "time_misc.h"
  #include "filesystem.h"

  extern int xpn_profiler_fd; // In src/xpn/xpn_simple/xpncore/xpn_init.c
  extern int xpn_profiler; // In src/xpn/xpn_simple/xpncore/xpn_init.c
  

  /* ... Const / Const ................................................. */
  
  #define XPN_PROFILER_HEADER "{\"otherData\": {},\"traceEvents\":[{}\n"
  #define XPN_PROFILER_FOOTER "]}\n"


  #define XPN_PROFILER_BEGIN(name_file) \
    if (xpn_profiler) { \
      char buf[PATH_MAX]; \
      sprintf(buf, "%s%ld.json", name_file, pthread_self()); \
      xpn_profiler_fd = filesystem_open2(buf, O_WRONLY | O_CREAT, S_IRWXU); \
      if (xpn_profiler_fd > 0) { \
        filesystem_write(xpn_profiler_fd, (void *)XPN_PROFILER_HEADER, strlen(XPN_PROFILER_HEADER)); \
      } \
    }

  #define XPN_PROFILER_END() \
    if (xpn_profiler) { \
      filesystem_write(xpn_profiler_fd, (void *)XPN_PROFILER_FOOTER, strlen(XPN_PROFILER_FOOTER)); \
      filesystem_close(xpn_profiler_fd); \
    }

  #define XPN_PROFILER_WRITE_CUSTOM(start_time, elapsed_time, format, ...) \
    if (xpn_profiler) { \
      if (xpn_profiler_fd >= 0) { \
        char xpn_profiler_aux_buff[PATH_MAX*2]; \
        char xpn_profiler_aux_buff2[PATH_MAX]; \
        snprintf(xpn_profiler_aux_buff2, PATH_MAX, format, ## __VA_ARGS__); \
        snprintf(xpn_profiler_aux_buff, PATH_MAX*2, ",{" \
        "\"cat\":\"function\"," \
        "\"dur\":%ld," \
        "\"name\":\"%s(%s)\"," \
        "\"ph\":\"X\"," \
        "\"pid\":%d," \
        "\"tid\":%ld," \
        "\"ts\":%ld" \
        "}\n", \
        elapsed_time, __func__, xpn_profiler_aux_buff2, getpid(), pthread_self(), start_time); \
        filesystem_write(xpn_profiler_fd, xpn_profiler_aux_buff, strlen(xpn_profiler_aux_buff)); \
      } \
    }

  #define XPN_PROFILER_NAME_BEGIN(name) \
    struct timeval name##_start_timer; \
    if (xpn_profiler) { \
      TIME_MISC_Timer(&name##_start_timer); \
    }

  #define XPN_PROFILER_NAME_END_CUSTOM(name, format, ...) \
    if (xpn_profiler) { \
      struct timeval name##_end_timer; \
      struct timeval name##_dif_timer; \
      TIME_MISC_Timer(&name##_end_timer); \
      TIME_MISC_DiffTime(&name##_start_timer,&name##_end_timer,&name##_dif_timer); \
      long name##_start_us = (long) TIME_MISC_TimevaltoMicroLong(&name##_start_timer); \
      long name##_dif_us = (long) TIME_MISC_TimevaltoMicroLong(&name##_dif_timer); \
      XPN_PROFILER_WRITE_CUSTOM(name##_start_us, name##_dif_us, format, ## __VA_ARGS__) \
    }

  #define XPN_PROFILER_DEFAULT_BEGIN() XPN_PROFILER_NAME_BEGIN(default);
  #define XPN_PROFILER_DEFAULT_END() XPN_PROFILER_NAME_END_CUSTOM(default, " ");
  #define XPN_PROFILER_DEFAULT_END_CUSTOM(format, ...) XPN_PROFILER_NAME_END_CUSTOM(default, format, ## __VA_ARGS__);


  /* ................................................................... */

#endif

