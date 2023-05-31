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
 * @file urlstr.c
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
#include "base/urlstr.h"

/************************************************
 *  ... Functions
 ***********************************************/

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param url 'TODO'.
 * @param protocol 'TODO'.
 * @return 'TODO'.
 */
int urlstr_get_url_protocol(char *url, char *protocol)
{
	int i, j;

	if (url == NULL)
	{
		printf("[%s:%d] ERROR: url is NULL\n", __FILE__, __LINE__);
		return -1;
	}

	// find ':'
	i = 0;
	while ((url[i] != '\0') && (url[i] != ':'))
	{
		i++;
	}
	j = i;

	if (url[i] != ':')
	{
		printf("[%s:%d] ERROR: missing ':' within the url.\n", __FILE__, __LINE__);
		printf("Usage: protocol_name://server:port//dir\n");
		return -1;
	}

	i++;
	if (url[i] != '/')
	{
		printf("[%s:%d] ERROR: missing first '/' within the url.\n", __FILE__, __LINE__);
		printf("Usage: protocol_name://server:port//dir\n");
		return -1;
	}

	i++;
	if (url[i] != '/')
	{
		printf("[%s:%d] ERROR: missing second '/' within the url.\n", __FILE__, __LINE__);
		printf("Usage: protocol_name://server:port//dir\n");
		return -1;
	}

	i++;
	if (protocol != NULL)
	{
		strncpy(protocol, url, j);
		protocol[j] = '\0';
	}

	return i;
}

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param url 'TODO'.
 * @param server 'TODO'.
 * @return 'TODO'.
 */
int urlstr_get_url_server(char *url, char *server)
{
	int i, j;

	i = urlstr_get_url_protocol(url, NULL);
	if (i < 0)
	{
		return -1;
	}

	j = i;
	while ((url[j] != '\0') && (url[j] != ':') && (url[j] != '/'))
	{
		j++;
	}

	if (server != NULL)
	{
		strncpy(server, url + i, j - i);
		server[j - i] = '\0';
	}

	return j;
}

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param url 'TODO'.
 * @param port 'TODO'.
 * @return 'TODO'.
 */
int urlstr_get_url_port(char *url, char *port)
{
	int i, j;

	i = urlstr_get_url_server(url, NULL);
	if (i < 0)
	{
		return -1;
	}

	if (url[i] != ':')
	{
		return -1;
	}

	i++;
	j = i;
	while ((url[j] != '\0') && (url[j] != '/'))
	{
		j++;
	}

	if (port != NULL)
	{
		strncpy(port, url + i, j - i);
		port[j - i] = '\0';
	}

	return j;
}

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param url 'TODO'.
 * @param dir 'TODO'.
 * @return 'TODO'.
 */
int urlstr_get_url_dir(char *url, char *dir)
{
	int i;

	if ((i = urlstr_get_url_port(url, NULL)) < 0)
	{
		if ((i = urlstr_get_url_server(url, NULL)) < 0)
		{
			if ((i = urlstr_get_url_protocol(url, NULL)) < 0)
			{
				return -1;
			}
		}
	}

	if (dir != NULL)
	{
		strncpy(dir, url + i, strlen(url) - i);
		dir[strlen(url) - i] = '\0';
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

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path 'TODO'.
 * @return 'TODO'.
 */
int urlstr_clear_slash(char *path)
{
	size_t i;
	int j;
	char ant = '\0', s[PATH_MAX];

	j = 0;
	for (i = 0; i < strlen(path); i++)
	{
		switch (path[i])
		{
		case '/':
			if (ant != '/')
			{
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

int base_urlstr_parse_url(char *url,
			 char *protocol, char *login, char *passwd, char *server, char *port, char *dir)
{
	char *urlaux;

	urlaux = url;
	if (protocol != NULL)
	{
		/* return the next position */
		if (urlstr_get_url_protocol(urlaux, protocol) < 0)
		{
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
	if (passwd != NULL)
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
	if (server != NULL)
	{
		/* return the next position */
		if (getURLServer(urlaux, server) < 0)
		{
			return -1;
		}
	}

	urlaux = url;
	if (port != NULL)
	{
		/* return the next position */
		if (urlstr_get_url_port(urlaux, port) < 0)
		{
			// Not mandatory
			// return -1;
		}
	}

	urlaux = url;
	if (dir != NULL)
	{
		/* return the next position */
		if (urlstr_get_url_dir(urlaux, dir) < 0)
		{
			return -1;
		}
		urlstr_clear_slash(dir);
	}

	return 0;
}

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param url 'TODO'.
 * @param dir 'TODO'.
 * @return 'TODO'.
 */
int getDirWithURL(char *url, char *dir)
{
	char dir_aux[PATH_MAX]; /* change for a const*/
	int i, j;

	urlstr_get_url_dir(url, dir_aux);

	i = 0;
	while ((dir_aux[i] != '\0') && (dir_aux[i] == dir[i]))
	{
		i++;
	}

	if (dir_aux[i] != '\0')
	{
		return -1;
	}
	if (dir_aux[i] == dir[i])
	{
		// dir[0] = '\0'; /* or '/'  */
		dir[0] = '/';
		dir[1] = '\0';
		return 0;
	}

	j = 0;
	while (dir[j + i] != '\0')
	{
		dir[j] = dir[j + i];
		j++;
	}

	dir[j] = '\0';
	if ((dir[j] == '/') && (strlen(dir + j) == 1))
	{
		dir[j - 1] = '\0';
	}

	return 0;
}

/* ................................................................... */
