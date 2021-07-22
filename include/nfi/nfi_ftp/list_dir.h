
#ifndef _LIST_DIR_H_
#define _LIST_DIR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Char to mark end of incomplete record of list
#define END_CHAR "*" 


  /*
   * Includes
   */

//element_dirs de la estructura que almacena el list del fichero

 #ifdef  __cplusplus
    extern "C" {
 #endif

struct element_dir{
   char data[256];
   struct element_dir *next;
};
 
  /*
   * New types
   */
  struct list_dir
  {
  	int dir_pos;
  	struct element_dir * first ;
     	struct element_dir * last ;
  } ;


  /*
   * Functions
   */
  int LIST_DIR_init 
  (
     /* INOUT */ struct list_dir *lista 
  ) ;

  int LIST_DIR_insert 
  ( 
     /* INOUT */ struct list_dir *lista, 
     /* IN    */ char *data 
  ) ;

  int LIST_DIR_finalize 
  (
     /* INOUT */ struct list_dir *lista 
  ) ;

  int LIST_DIR_list
   (
     /* IN */ struct list_dir *lista 
   );
  int LIST_DIR_delete_mark
   (
     /* INOUT */ struct list_dir *lista 
   );

 #ifdef  __cplusplus
        }
 #endif
   
#endif

