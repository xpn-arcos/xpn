

   /* ... Include / Inclusion ........................................... */

      #include "base/urlstr.h"


   /* ... Functions / Funciones ......................................... */

      static
      int    URLSTR_Match_protocol    ( char    **protocol,
                                           char    **str )
      {
        static char *URLSTR_protocols[] = 
               {
                 "http",
                 "ftp",
                 "file",
                 "news",
                 "nntp",
                 "pop3",
                 "mailto",
                 "gopher",
                 "telnet",
                 "finger",
                 "tcpServer",
                 "mpiServer",
                 NULL
               } ;
        int i, ret ;

        for (i=0; URLSTR_protocols[i] != NULL; i++)
            {
              ret = strncmp( (*str),
                             URLSTR_protocols[i],
                             strlen(URLSTR_protocols[i]) ) ;
              if (ret == 0)
                 {
                   if (strcmp("news",URLSTR_protocols[i]) == 0)
                            (*protocol) = strdup("nntp") ;
                       else (*protocol) = strdup(URLSTR_protocols[i]) ;

                   (*str) = (*str) + strlen(URLSTR_protocols[i]) ;
                   if ( (**str) == ':' )
                        (*str) ++ ;
                   if ( (**str) == '/' )
                        (*str) ++ ;
                   if ( (**str) == '/' )
                        (*str) ++ ;
                   return (1) ;
                 }
            }
        (*protocol) = strdup("http") ;
        return (1) ;
      }

      static
      int    URLSTR_Match_user     (  char    **user,
                                         char    **str )
      {
        char *pch ;

        pch = strchr((*str),'@') ;
        if (pch == NULL)
             return (1) ;

        pch[0] = '\0' ;
        (*user) = strdup((*str)) ;
        (*str) = pch + 1 ;
        return (1) ;
      }

      static
      int    URLSTR_Match_machine  (  char    **machine,
                                         char     *protocol,
                                         char    **str )
      {
        char *pch1, *pch2 ;

        (*machine) = NULL ;

        /* 
         *  file
         */
        if ( !strcmp(protocol,"file") )
           {
             int ret ;

             (*machine) = strdup("localhost") ;
             ret = strncmp((*str),"localhost",strlen("localhost")) ;
             if (ret == 0)
                 (*str) = (*str) + strlen("localhost") ;
             return (1) ;
           }

        /* 
         *  http || ftp 
         */
        pch1 = strchr((*str),'/') ;
        pch2 = strchr((*str),':') ;
        if (pch2 == NULL)
           {
             if (pch1 == NULL)
                {
                  (*machine) = strdup((*str)) ;
                  (*str) = (*str) + strlen((*str)) ;
                  return (1) ;
                }
             if (pch1 != NULL)
                {
                  pch1[0] = '\0' ;
                  (*machine) = strdup((*str)) ;
                  (*str) = pch1 ;
                  pch1[0] = '/' ;
                  return (1) ;
                }
           }
        if (pch2 != NULL)
           {
             pch2[0] = '\0' ;
             (*machine) = strdup((*str)) ;
             pch2[0] = ':' ;
             (*str) = pch2 ;
             return (1) ;
           }
        return (1) ;
      }

      static
      int    URLSTR_Match_port     (  int      *port,
                                         char     *protocol,
                                         char     **str )
      {
        char *pch1, *pch2 ;

        /* ... default port ... */
        (*port) = htons(80) ;
        if (!strcmp(protocol,"http"))
            (*port) = htons(80) ;
        if (!strcmp(protocol,"ftp"))
            (*port) = htons(21) ;
        if (!strcmp(protocol,"file"))
            (*port) = htons(0) ;
        if (!strcmp(protocol,"nntp"))
            (*port) = htons(119) ;
        if (!strcmp(protocol,"news"))
            (*port) = htons(119) ;
        if (!strcmp(protocol,"pop3"))
            (*port) = htons(110) ;
        if (!strcmp(protocol,"finger"))
            (*port) = htons(79) ;

        /* ... scanning port ... */
        /* 
           file
        */
        if (!strcmp(protocol,"file"))
            return (1) ;

        /* 
           http || ftp 
        */
        pch1 = strchr((*str),':') ;
        if (pch1 == NULL)
           {
             return (1) ;
           }
        pch1 ++ ; /* skip ':' */
        pch2 = strchr(pch1,'/') ;
        if (pch2 == NULL)
           {
             (*port) = htons(atoi(pch1)) ;
             (*str)  = (*str) + strlen((*str)) ;
             return (1) ;
           }
        if (pch2 != NULL)
           {
             pch2[0] = '\0' ;
             (*port) = htons(atoi(pch1)) ;
             pch2[0] = '/' ;
             (*str)  = pch2 ;
             return (1) ;
           }
        return (1) ;
      }

      static
      int    URLSTR_Match_file     (  char    **file,
                                         char    **str )
      {
        char *pch1 ;

        pch1 = strchr((*str),'#') ;
        if (pch1 != NULL)
           {
             pch1[0] = '\0' ;
             (*file) = strdup((*str)) ;
             (*str)  = (*str) + strlen(pch1) ;
             pch1[0] = '#' ;
             return (1) ;
           }

        pch1 = strchr((*str),'?') ;
        if (pch1 != NULL)
           {
             pch1[0] = '\0' ;
             (*file) = strdup((*str)) ;
             pch1[0] = '?' ;
             (*str)  = pch1 ;
             return (1) ;
           }

        /* ... all is file ... */
        if ((*str)[0] != '\0')
           {
             (*file) = strdup((*str)) ;
             (*str)  = (*str) + strlen((*str)) ;
             return (1) ;
           }

        (*file) = strdup("/") ;
        return (1) ;
      }

      static
      int    URLSTR_Match_relative (  char     **relative,
                                         char     **str )
      {
        char *pch1, *pch2 ;

        (*relative) = NULL ;
        pch1 = strchr((*str),'#') ;
        if (pch1 == NULL)
             return (1) ;

        pch1 ++ ; /* skip '#' */
        pch2 = strchr(pch1,'?') ;
        if (pch2 == NULL)
           {
             (*relative) = strdup((*str)) ;
             (*str)      = (*str) + strlen((*str)) ;
             return (1) ;
           }

        if (pch2 != NULL)
           {
             pch2[0]     = '\0' ;
             (*relative) = strdup(pch1) ;
             (*str)      = pch2 ;
             pch2[0]     = '?' ;
             return (1) ;
           }

        return (1) ;
      }

      static
      int    URLSTR_Match_params   (  char    **params,
                                         char    **str )
      {
        char *pch1 ;

        (*params) = NULL ;
        pch1 = strchr((*str),'?') ;
        if (pch1 == NULL)
            return (1) ;

        pch1 ++ ; /* skip '?' */
        (*params) = strdup(pch1) ;
        (*str)    = (*str) + strlen((*str)) ;
        return (1) ;
      }

    /**
     * Given a URL in 'str', this function split in the
     * common components: protocol, user, machine, etc.
     * @param str the URL as string.
     * @param protocol the string where this component will be placed.
     * @param user the string where this component will be placed.
     * @param machine the string where this component will be placed.
     * @param port the string where this component will be placed.
     * @param file the string where this component will be placed.
     * @param relative the string where this component will be placed.
     * @param params the string where this component will be placed.
     * @return true (1) if parsing was madden or error (-1) if
               any error is found.
     */
     int    URLSTR_Match_url      (  char    **protocol,
                                         char    **user,
                                         char    **machine,
                                         int      *port,
                                         char    **file,
                                         char    **relative,
                                         char    **params,
                                         char    **str )
     {
        int ok ;

        ok = URLSTR_Match_protocol(protocol,str) ;
        if (0 == ok) return (0) ;
        ok = URLSTR_Match_user(user,str) ;
        if (0 == ok) return (0) ;
        ok = URLSTR_Match_machine(machine,*protocol,str) ;
        if (0 == ok) return (0) ;
        ok = URLSTR_Match_port(port,*protocol,str) ;
        if (0 == ok) return (0) ;
        ok = URLSTR_Match_file(file,str) ;
        if (0 == ok) return (0) ;
        ok = URLSTR_Match_relative(relative,str) ;
        if (0 == ok) return (0) ;
        ok = URLSTR_Match_params(params,str) ;
        if (0 == ok) return (0) ;
        return (1) ;
     }

    /**
     * Given a URL in 'urlstr', this function split in the
     * common components: protocol, user, machine, etc.
     * @param urlstr the URL as string.
     * @param protocol the string where this component will be placed.
     * @param user the string where this component will be placed.
     * @param machine the string where this component will be placed.
     * @param port the string where this component will be placed.
     * @param file the string where this component will be placed.
     * @param relative the string where this component will be placed.
     * @param params the string where this component will be placed.
     * @return true (1) if parsing was madden or error (-1) if
               any error is found.
     */
     int    URLSTR_ParseURL
     (
          /*IN */ char  *urlstr,
          /*OUT*/ char **protocol,
          /*OUT*/ char **user,
          /*OUT*/ char **machine,
          /*OUT*/ int   *port,
          /*OUT*/ char **file,
          /*OUT*/ char **relative,
          /*OUT*/ char **params
     )
     {
        char     *pch, *fch ;
        int   ok ;

        fch = pch = strdup(urlstr) ;
        if (NULL == pch) 
            return (-1);

        ok  = URLSTR_Match_url(protocol,
                               user,
                               machine,
                               port,
                               file,
                               relative,
                               params,
                               &pch) ;
        free(fch) ;
        return ok ;
     }


  /* ...................................................................... */

