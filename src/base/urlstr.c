
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

      #include "base/urlstr.h"


   /* ... Functions / Funciones ......................................... */



   /* ... Functions / Funciones ......................................... */

      int getURLProtocol(char *url, char *protocol)
      {
      	int i,j;

      	if (url == NULL) {
           printf("[%s:%d] ERROR: url is NULL\n", __FILE__, __LINE__);
      	   return -1;
      	}

        // find ':'
        i = 0;
      	while ((url[i] != '\0')&&(url[i] != ':'))  {
          i++;
        }
        j = i;

      	if (url[i] != ':') {
          printf("[%s:%d] ERROR: missing ':' within the url.\n", __FILE__, __LINE__);
          printf("Usage: protocol_name://server:port//dir\n");
      	  return -1;
      	}

      	i++;
      	if (url[i] != '/') {
          printf("[%s:%d] ERROR: missing first '/' within the url.\n", __FILE__, __LINE__);
          printf("Usage: protocol_name://server:port//dir\n");
      	  return -1;
      	}

      	i++;
      	if (url[i] != '/') {
          printf("[%s:%d] ERROR: missing second '/' within the url.\n", __FILE__, __LINE__);
          printf("Usage: protocol_name://server:port//dir\n");
      	  return -1;
      	}

      	i++;
      	if (protocol != NULL) {
      	  strncpy(protocol, url, j);
      	  protocol[j] = '\0';
      	}

      	return i;
      }


      int getURLServer(char *url, char *server)
      {
      	int i,j;

      	i = getURLProtocol(url, NULL) ;
      	if (i < 0) {
      	   return -1;
      	}

      	j = i;
      	while ((url[j]!='\0')&&(url[j]!=':')&&(url[j]!='/')) {
      	   j++;
	}

      	if (server != NULL) {
     	   strncpy(server, url+i, j-i);
      	   server[j-i] = '\0';
      	}

      	return j;
      }


      int getURLPort ( char *url, char *port )
      {
      	int i, j;

      	i=getURLServer(url, NULL) ;
      	if (i < 0) {
      		return -1;
      	}

      	if (url[i] != ':') {
      		return -1;
      	}

	i++;
      	j = i;
      	while ((url[j]!='\0')&&(url[j]!='/')) {
	  j++;
	}

	if(port != NULL){
		strncpy(port, url+i, j-i);
		port[j-i] = '\0';
	}

      	return j;
      }


      int getURLDir(char *url, char *dir)
      {
      	int i;

	if ((i=getURLPort(url, NULL))<0)
	{
	     	if ((i=getURLServer(url, NULL))<0)
		{
      			if((i=getURLProtocol(url, NULL))<0)
			{
      				return -1;
      			}
      		}
      	}

      	if (dir != NULL) {
      		strncpy(dir, url+i, strlen(url)-i);
      		dir[strlen(url)-i] = '\0';
      	}

      	return strlen(url);
      }



/*
      int getURLLogin(char *url, char *login)
      {
        // TODO
      	return 0;
      }


      int getURLPasswd(char *url, char *passwd)
      {
        // TODO
      	return 0;
      }
*/


      int clear_slash(char *path)
      {
	size_t i;
	int j;
	char ant = '\0', s[PATH_MAX];

	j=0;
	for (i=0; i < strlen(path); i++)
	{
		switch(path[i])
		{
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


      int ParseURL( char *url,
		    char *protocol, char *login, char *passwd, char *server, char *port, char *dir )
      {
      	char *urlaux;

      	urlaux = url;
      	if (protocol != NULL)
	{
      		/* return the next position */
      		if(getURLProtocol(urlaux, protocol)<0){
      			return -1;
      		}
      	}

      	urlaux = url;
      	if (login != NULL)
	{
      		/* return the next position */
      		/*
      		if(getURLLogin(urlaux, login)<0){
      			//Not mandatory
      			//return -1;
      		}
      		*/
      	}

      	urlaux = url;
      	if(passwd != NULL)
	{
      		/* return the next position */
      		/*
      		if(getURLPasswd(urlaux, passwd)<0){
      			//Not mandatory
      			//return -1;
      		}
      		*/
      	}

      	urlaux = url;
      	if(server != NULL)
	{
      		/* return the next position */
      		if(getURLServer(urlaux, server)<0){
      			return -1;
      		}
      	}


      	urlaux = url;
      	if(port != NULL)
	{
      		/* return the next position */
      		if(getURLPort(urlaux, port)<0){
      			//Not mandatory
      			//return -1;
      		}
      	}

      	urlaux = url;
      	if(dir != NULL)
	{
      		/* return the next position */
      		if(getURLDir(urlaux, dir)<0){
      			return -1;
      		}
		clear_slash(dir);
      	}

      	return 0;
      }


      int getDirWithURL(char *url, char *dir)
      {
      	char dir_aux[PATH_MAX]; /* change for a const*/
      	int i,j;

      	getURLDir(url, dir_aux);

      	i = 0;
      	while((dir_aux[i] != '\0')&&(dir_aux[i] == dir[i])) {
      		i++;
	}


      	if(dir_aux[i] != '\0') {
      		return -1;
	}
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
      	if((dir[j] == '/') &&(strlen(dir+j) == 1)) {
      		dir[j-1] = '\0';
	}

      	return 0;
      }


  /* ................................................................... */

