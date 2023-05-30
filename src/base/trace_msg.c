
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

/**
 * @file trace_msg.c
 * @brief File to 'TODO'.
 *
 * File to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

/************************************************
 *  ... Includes
 ***********************************************/
#include "base/trace_msg.h"


/************************************************
 *  ... Globla var.
 ***********************************************/
int (*TRACE_MSG_PrintMsg[TRACE_PRINTER_SIZE])(const char *, va_list);

#ifdef HAVE_PROM_H
   // Inspired by https://github.com/digitalocean/prometheus-client-c/blob/master/example/bar.c
   prom_counter_t    *bar_counter ;
   prom_gauge_t      *bar_gauge ;
   prom_histogram_t  *test_histogram;
   struct MHD_Daemon *daemon ;
#endif


/************************************************
 *  ... Functions
 ***********************************************/

// Initialize and Finalize

int base_trace_msg_init ( void )
{
   int i;

   // init trace_printer
   for (i = 0; i < TRACE_PRINTER_SIZE; i++)
   {
      TRACE_MSG_PrintMsg[i] = NULL;
   }

   return 1;
}

int trace_msg_finalize ( void )
{
   // finalize trace_printer
   return 1;
}


int base_trace_prom_init ( void )
{
   // initialize prometheus eltos
#ifdef HAVE_PROM_H
   bar_counter = NULL ;
   bar_gauge   = NULL ;

   prom_collector_registry_default_init();

   bar_counter = prom_collector_registry_must_register_metric(prom_counter_new(counter_name, "counter for bar", 0, NULL)) ;
   bar_gauge   = prom_collector_registry_must_register_metric(  prom_gauge_new("bar_gauge",   "gauge for bar",   1, (const char *[]) { "label" })) ;

   test_histogram = prom_collector_registry_must_register_metric(
		       prom_histogram_new("test_histogram",
		    		          "histogram under test",
				          prom_histogram_buckets_linear(5.0, 5.0, 2),
				          0,
				          NULL)
                    ) ;

   promhttp_set_active_collector_registry(NULL) ;


   int r ;
   double hist_value;
   const char *labels[] = { "one", "two", "three", "four", "five" };
   r = 0;
   for (int i = 1; i <= 100; i++)
   {
        if (i % 2 == 0)
             hist_value = 3.0;
        else hist_value = 7.0;

        r = prom_histogram_observe(test_histogram, hist_value, NULL);
        if (r) return -1;

        for (int x = 0; x < 5; x++)
	{
          r = foo(i,  labels[x]);
          if (r) return -1;
          r = bar(i+x, labels[x]);
          if (r) return -1;
        }
   }



   daemon = promhttp_start_daemon(MHD_USE_SELECT_INTERNALLY, 8000, NULL, NULL) ;
   if (daemon == NULL) {
       return -1;
   }
#endif

   return 1;
}

int base_trace_prom_finalize ( void )
{
   // finalize prometheus eltos
#ifdef HAVE_PROM_H
   printf("\nShutting down trace_msg module...\n");
   fflush(stdout);

   prom_collector_registry_destroy(PROM_COLLECTOR_REGISTRY_DEFAULT);

   MHD_stop_daemon(daemon);
#endif

   return 1;
}


// Set collector

int base_trace_msg_set_printer ( int index, int (*printer)(const char *, va_list) )
{
   if (index < 0) {
      return -1;
   }
   if (index > TRACE_PRINTER_SIZE) {
      return -1;
   }

   TRACE_MSG_PrintMsg[index] = (int (*)(const char *, va_list))printer;

   return 1;
}


// Set data to collector

int base_trace_msg_bar ( int v, const char *label )
{
  int r ;

  r = 0 ;
  v = v ; // ToBeRemoved
  label = label ; // ToBeRemoved

#ifdef HAVE_PROM_H
  r = prom_counter_inc(bar_counter, NULL) ;
  if (r) {
      return r ;
  }

  r = prom_gauge_add(bar_gauge, v, (const char *[]) { label }) ;
#endif

  return r ;
}

void trace_msg_do_print ( char *fto, ... )
{
   if (NULL == TRACE_MSG_PrintMsg)
   {
      return;
   }

   va_list vl;

   va_start(vl, fto);
   (*TRACE_MSG_PrintMsg)(fto, vl);
   va_end(vl);
}

void base_trace_msg_variable_printf(int line, char *name, long pid, int type, char *fto, va_list vl)
{
   if (NULL == TRACE_MSG_PrintMsg)
   {
      return;
   }

   char *msg;

   msg = base_string_misc_dynamic_vsprintf(fto, vl);
   trace_msg_do_print("trace(%i,\"%s\",%li,%i,\"%s\").", line, name, pid, type, msg);
   free(msg);
}

void base_trace_msg_printf(int line, char *name, long pid, int type, char *fto, ...)
{
   if (NULL == TRACE_MSG_PrintMsg)
   {
      return;
   }

   va_list vl;

   va_start(vl, fto);
   base_trace_msg_variable_printf(line, name, pid, type, fto, vl);
   va_end(vl);
}


/* ................................................................... */

