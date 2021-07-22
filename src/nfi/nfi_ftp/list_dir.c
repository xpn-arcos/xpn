

  /*
   * Includes
   */

  #include "nfi/nfi_ftp/list_dir.h"
  //#include "ftp.h"


  /*
   * Functions
   */

  int LIST_DIR_init 
  (
     /* INOUT */ struct list_dir *lista 
  ) 
  {
	  /* check params */
	  if (NULL == lista) {
		  printf("¡list_dir es NULL!\n") ;
		  return -1 ;
	  }

	  /* set default values */
	  lista->first = NULL ;
	  lista->last  = NULL ;

	  /* return ok */
	  return (1) ;
  }

  int LIST_DIR_insert 
  ( 
     /* INOUT */ struct list_dir *lista, 
     /* IN    */ char *data 
  ) 
  {
	  struct element_dir * new_elto ;


	  /* check params */
	  if (NULL == lista) {
		  printf("¡list_dir es NULL!\n") ;
		  return -1 ;
	  }

	  /* 1) new element */
	  new_elto = (struct element_dir *) malloc(sizeof(struct element_dir));
	  if (NULL == new_elto) {
		  printf("¡No hay suficiente memoria ;)\n") ;
		  return -1 ;
	  }

	  /* 2) setup default values */
	  
	  //miro a ver si en el último elemento quedan cosas por copiar
	  if ((lista->last!=NULL) && (strstr(lista->last->data,END_CHAR)!=NULL))
	  {
	  	//if last one has *, means that is incomplete
	  	lista->last->data[strlen(lista->last->data)-1]='\0';
		strncat(lista->last->data,data,strlen(data));
	  }
	  else
	  {
	  	strcpy(new_elto->data,data);
	  	new_elto->next = NULL;

	  	/* 3) try to insert into the list */
	  	if (lista->first!=NULL)
	  	{
			lista->last->next = new_elto;
			lista->last       = new_elto;
	  	}
	  	else 
	  	{
			lista->first = new_elto;
			lista->last  = new_elto;
	  	}
	  }
	  /* return ok */
	  return (1) ;
   }

   int LIST_DIR_finalize 
   (
     /* INOUT */ struct list_dir *lista 
   ) 
   {
	  struct element_dir * elto_to_del ;
	  struct element_dir * next_to_del ;


	  /* check params */
	  if (NULL == lista) {
		  printf("¡list_dir es NULL!\n") ;
		  return -1 ;
	  }

	  /* easy case */
	  if (NULL == lista->first) {
		  return 1 ;
	  }

	  /* free each element */
	  elto_to_del = lista->first ;
	  while (elto_to_del != NULL) 
	  {
	       //printf("Borro Elemento del list \n");
	       next_to_del = elto_to_del->next ;
	       free(elto_to_del) ;
	       elto_to_del = next_to_del ;
	  }

	  /* set default values */
	  lista->first = NULL ;
	  lista->last  = NULL ;

	  /* return ok */
	  return (1) ;
   }

   int LIST_DIR_list
   (
     /* IN */ struct list_dir *lista 
   ) 
   {
	  struct element_dir * elto ;
	  
	  /* check params */
	  if (NULL == lista) {
		  printf("¡list_dir es NULL!\n") ;
		  return -1 ;
	  }

	  elto=lista->first;
	
		while(elto)
		{
		//printf("-- %s\n",elto->data);
		elto= elto->next;
		}

	  /* return ok */
	  return (1) ;
   }
   
   int LIST_DIR_delete_mark
   (
     /* INOUT */ struct list_dir *lista 
   ) 
   {
	/* check params */
	  if (NULL == lista) {
		  printf("¡list_dir es NULL!\n") ;
		  return -1 ;
	  }
	/* Delete * to the end of data*/
	lista->last->data[strlen(lista->last->data)-1] = '\0';
	
	/* return ok */
	return (1) ;
   }
   
   

