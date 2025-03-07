
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

      #include "base/path_misc.h"


   /* ... Functions / Funciones ......................................... */

      int hash ( const char *path, int nServ, int isfile )
      {
        int i,max;
        int unsigned num;
        char *aux_file;
        char file[PATH_MAX];
        strncpy(file, path, PATH_MAX-1);

        // Get file name
        if (isfile == 1){
              aux_file = basename(file);
        }else{
              aux_file = dirname(file);
              aux_file = basename(aux_file);
        }
        num = 0;
        max = strlen(aux_file);
        for (i = 0; i < max; i++) {
              num += (int)aux_file[i];
        }
        return (int)num % nServ;
     }


      int getFirstDir ( char *dir, char *path )
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


      long getSizeFactor ( char *name )
      {
        switch (name[strlen(name)-1])
	{
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
                 switch (name[strlen(name)-2])
		 {
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
                 return atoi(name);
        }
      }


      int getNameFile ( char *file, char *dir )
      {
      	int i,j;

      	i = strlen(dir);
    	if (i == 0) {
      		file[0]='\0';
      		return 0; /* ? */
      	}

      	while ((i>=0)&&(dir[i] == '/')) {
      		i--;
	}
      	if (i == 0) {
      		file[0]='\0';
      		return 0; /* ? */
      	}

      	/* */
      	j = i;
      	while ((j>=0)&&(dir[j] != '/')) {
      		j--;
	}

      	if (dir[j] == '/') {
      		j++;
      	}

      	strncpy(file, dir+j, i-j);

      	dir[j] = '\0';
      	file[i-j] = '\0';

      	return 0;
      }


      int getNamePart ( char *part, char *dir )
      {
      	int i,j;

      	// Printf("original dir = %s\n", dir);
      	if (dir[0] != '/') {
      	    return -1;
	}

      	i = 1;
      	while((dir[i] != '\0')&&(dir[i] != '/')) {
      		i++;
	}

      	strncpy(part, dir+1, i-1);
      	part[i-1] = '\0';
      	j = 0;
      	while (dir[i] != '\0') {
      		dir[j] = dir[i];
      		j++;
      		i++;
      	}

      	dir[j] = '\0';
      	return 0;
      }


   /* ................................................................... */

