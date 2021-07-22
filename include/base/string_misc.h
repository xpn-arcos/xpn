

 #ifndef _STRING_MISC_H_
 #define _STRING_MISC_H_

   /* ... Include / Inclusion ........................................... */

      #include "all_system.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Functions / Funciones ......................................... */

      int hash(char *file,int nServ);

      int ParseURL(   char *url,  
      		      char *protocol, 
		      char *login, 
		      char *passwd, 
		      char *server,  
		      char *port,  
		      char *dir);

      /* get the last name of a path and erase the file name */
      int getNameFile(char *file, char *dir);

      /* get the first name of the path and erase the part name */
      int getNamePart(char *part, char *dir);

      /* erase the initial dir used in the url */
      int getDirWithURL(char *url, char *dir);


      /**
        * Return the string length of 'str'.
        *
        *   @param str the string.
        *   @return the string length.
        *
        */
      long   STRING_MISC_StrLen ( /*IN*/  char  *str ) ;

      /**
        * Return true if and only if the strings 'str1' and 'str2' are equals.
        *
        *   @param str1 the first string.
        *   @param str2 the second string.
        *   @return true (1) iff are equals or false (0) in other case.
        *
        */
      int8_t STRING_MISC_Equal  ( /*IN*/  char  *str1,
		                  /*IN*/  char  *str2 ) ;

      /**
        * Return a string clone of 'str'.
        *
        *   @param str the string.
        *   @return a clone of 'str'.
        *
        */
      char  *STRING_MISC_StrDup ( /*IN*/ char  *str ) ;

       /**
         *
         *     Como 'strlen' pero permite usar
         *  un string con formato.
         *
         *     Like 'strlen' but also accept
         *  a string with format.
         *
         *     @params string format.
         *     @params format params.
         *     @return string legth.
         *
         */
       int   STRING_MISC_StrLenF
       (
         /*IN*/  char     *format,
         /*IN*/  va_list   argl
       ) ;

      /**
       *
       *     Actua igual que 'vsprintf', pero pide memoria
       *  dinamica para el string donde se imprime.
       *  Retorna NULL si no pudo.
       *
       *     Like vsprintf, but request dynamic memory
       *  to write string elements.
       *
       *     @params string format.
       *     @params format params.
       *     @return string pointer or NULL if error.
       *
       */
      char *STRING_MISC_Dvsprintf
      (
        /*IN*/  char     *format,
        /*IN*/  va_list   argl
      ) ;

      /**
        *
        *     Igual que la anterior, solo varia los argumentos
        *
        *     Like before, but with open arguments.
        *
        *     @params string format.
        *     @return string pointer or NULL if error.
        *
        */
      char *STRING_MISC_Dsprintf
      (
        /*IN*/  char   *format,
        ...
      ) ;


  /* .................................................................... */

   int getDir(char *);
   int multiline(char *text);
   int getFirstDir(char *dir, char *path);
   int getSizeFactor(char *name);
   
#ifdef  __cplusplus
    }
 #endif

 #endif /* _STRING_MISC_H */

