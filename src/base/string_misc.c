
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


/* ............................................................... */


      //calculo del numero de servidor
      int hash(char *file,int nServ){
        int i,max;
        int unsigned num;

        num=0;
        max=strlen(file)-1;
        for(i=0;i<max;i++){
              num=(int)file[i]+num;
        }
        return (int)num%nServ;
      }
      
      
      
      int getURLProtocol(char *url, char *protocol)
      {
      
      	int i,j;
      	i = 0;
      	
      	if(url == NULL){
      		return -1;
      	}
      	
      	while((url[i] != '\0')&&(url[i] != ':'))
      		i++;
      	j = i;
      	
      	if(url[i] != ':'){
      		return -1;
      	}
      
      
      	i++;
      	if(url[i] != '/'){
      		return -1;
      	}
      
      	i++;
      	if(url[i] != '/'){
      		return -1;
      	}
      
      	i++;
      	if(protocol != NULL){
      		strncpy(protocol, url, j);
      		protocol[j] = '\0';
      	}
      	return i;
      }
      
      /*
      int getURLLogin(char *url, char *login)
      {
      	return 0;
      }
      
      int getURLPasswd(char *url, char *passwd)
      {
      	return 0;
      }
      */
      
      int getURLServer(char *url, char *server)
      {
      	int i,j;
      	
      	if((i=getURLProtocol(url, NULL))<0){
      		return -1;
      	}
      	j = i;
      	while((url[j]!='\0')&&(url[j]!=':')&&(url[j]!='/'))
      		j++;
      
      	if(server != NULL){
      		strncpy(server, url+i, j-i);
      		server[j-i] = '\0';
      	}
      	return j;
      }
      
      
      int getURLPort(char *url, char *port)
      {
      	int i,j;
      	
      	if((i=getURLServer(url, NULL))<0){
      		return -1;
      	}
      	if(url[i] != ':'){
      		return -1;
      	}
	i++;
      	j = i;
      	while((url[j]!='\0')&&(url[j]!='/'))
		j++;
	if(port != NULL){
		strncpy(port, url+i, j-i);
		port[j-i] = '\0';
	}
      	return j;
      }
      
      int getURLDir(char *url, char *dir)
      {
      	int i;
	if((i=getURLPort(url, NULL))<0){
	     	if((i=getURLServer(url, NULL))<0){
      			if((i=getURLProtocol(url, NULL))<0){
      				return -1;
      			}
      		}
      	}

      	if(dir != NULL){
      		strncpy(dir, url+i, strlen(url)-i);
      		dir[strlen(url)-i] = '\0';
      	}

      	return strlen(url);
      }
      
      /******************************************************/
     
      int clear_slash(char *path){
	size_t i;
	int j;
	char ant = '\0', s[255];
	
	j=0;
	for(i=0;i < strlen(path); i++){
		switch(path[i]){
			case '/':
				if(ant != '/'){
					ant = s[j] = '/';
					j++;
				}
				break;
			default:
				ant = s[j] = path[i];
				j++;
				
		}

		s[j] = '\0';
	}
	
	strcpy(path, s);
	return 0;
}

      int ParseURL(	char *url,
      		char *protocol,
      		char *login,
      		char *passwd,
      		char *server,
      		char *port,
      		char *dir)
      {
      	char *urlaux;
      
      	
      	urlaux = url;
      	if(protocol != NULL){
      		/* return the next position */
      		if(getURLProtocol(urlaux, protocol)<0){
      			return -1;
      		}
      	}
      
      	urlaux = url;
      	if(login != NULL){
      		/* return the next position */
      		/*
      		if(getURLLogin(urlaux, login)<0){
      			//Not mandatory
      			//return -1;
      		}
      		*/
      	}
      
      	urlaux = url;
      	if(passwd != NULL){
      		/* return the next position */
      		/*
      		if(getURLPasswd(urlaux, passwd)<0){
      			//Not mandatory
      			//return -1;
      		}
      		*/
      	}
      
      	urlaux = url;
      	if(server != NULL){
      		/* return the next position */
      		if(getURLServer(urlaux, server)<0){
      			return -1;
      		}
      	}
      	
      
      	urlaux = url;
      	if(port != NULL){
      		/* return the next position */
      		if(getURLPort(urlaux, port)<0){
      			//Not mandatory
      			//return -1;
      		}
      	}
      
      	urlaux = url;
      	if(dir != NULL){
      		/* return the next position */
      		if(getURLDir(urlaux, dir)<0){
      			return -1;
      		}
		clear_slash(dir);
      	}	
      	
      	return 0;	
      }
      
      
      
      
      
      int getNameFile(char *file, char *dir)
      {
      	int i,j;
      
      	i = strlen(dir);
      		if(i == 0){
      		file[0]='\0';
      		return 0; /* ? */
      	}
      
      	while((i>=0)&&(dir[i] == '/'))
      		i--;
      	if(i == 0){
      		file[0]='\0';
      		return 0; /* ? */
      	}
      
      	/* */
      	j = i;
      	while((j>=0)&&(dir[j] != '/'))
      		j--;
      	
      	if(dir[j] == '/'){
      		j++;
      	}
      
      	strncpy(file, dir+j, i-j);
      	
      	dir[j] = '\0';
      	file[i-j] = '\0';
      	
      	return 0;
      }
      
      
      int getNamePart(char *part, char *dir){
      	int i,j;
      	
      	//Printf("original dir = %s\n", dir);
      	if(dir[0] != '/')
      		return -1;	
      	i =1;
      	while((dir[i] != '\0')&&(dir[i] != '/'))
      		i++;
      	strncpy(part, dir+1, i-1);
      	part[i-1] = '\0';
      	j = 0;
      	while(dir[i] != '\0'){		
      		dir[j] = dir[i];
      		j++;
      		i++;
      	}
      	
      	dir[j] = '\0';
      	return 0;
      }
      
      
      int getDirWithURL(char *url, char *dir)
      {
      	char dir_aux[255]; /* change for a const*/
      	int i,j;
      	
      	getURLDir(url, dir_aux);
      	i = 0;
      	while((dir_aux[i] != '\0')&&(dir_aux[i] == dir[i]))
      		i++;
      	
      	
      	if(dir_aux[i] != '\0')
      		return -1;
      	if(dir_aux[i] == dir[i]){
      		//dir[0] = '\0'; /* or '/'  */
      		dir[0] = '/';
      		dir[1] = '\0';
      		return 0;
      	}
      	j = 0;
      	while(dir[j+i]!='\0'){
      		dir[j] = dir[j+i];
      		j++;
      	}
      	dir[j] = '\0';
      	if((dir[j] == '/') &&(strlen(dir+j) == 1))
      		dir[j-1] = '\0';
      	return 0;
      }



int getFirstDir(char *dir, char *path)
{
        int i, j;

        //printf("++ path = %s ++\n", path);

        i = 0;
        while((path[i] != '\0')&&(path[i] != '/')){
                i++;
        }
        strncpy(dir, path , i);

        dir[i] = '\0';

        //printf("++ dir = %s ++\n",dir);


        while((path[i] != '\0')&&(path[i] == '/')){
                i++;
        }

        j = 0;
        while(path[i+j] != '\0'){

                path[j] = path[i+j];
                j++;
        }

        path[j] = '\0';
        //printf("++ dir = %s path = %s ++\n",dir, path);
        return j;
}
                                    
int getSizeFactor(char *name){

        switch(name[strlen(name)-1] ){
                case 'K':
                case 'k':
                        return atoi(name)*KB;
                case 'M':
                case 'm':
                        return atoi(name)*MB;
                case 'G':
                case 'g':
                        return atoi(name)*GB;
                case 'B':
                case 'b':
                        switch(name[strlen(name)-2]){
                                case 'K':
                                case 'k':
                                        return atoi(name)*KB;
                                case 'M':
                                case 'm':
                                        return atoi(name)*MB;
                                case 'G':
                                case 'g':
                                        return atoi(name)*GB;
                                default:
                                        return 1;
                        }
                default:
                        return 1;
        }
}


  /* ...................................................................... */

