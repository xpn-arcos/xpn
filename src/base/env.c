
/*
 *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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

#include "base/env.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */

int priv_xpn_session_dir = -1;
int priv_xpn_session_file = -1;

/* ... Functions / Funciones ......................................... */

int xpn_session_dir( void )
{
  if (priv_xpn_session_dir != -1){
    return priv_xpn_session_dir;
  }
  
  char * env_prefix = getenv("XPN_SESSION_DIR");
  if (env_prefix != NULL)
  {
    priv_xpn_session_dir = atoi(env_prefix);
  }else{
    // Default to 1
    priv_xpn_session_dir = 1;
  }

  return priv_xpn_session_dir;
}

int xpn_session_file( void )
{
  if (priv_xpn_session_file != -1){
    return priv_xpn_session_file;
  }
  
  char * env_prefix = getenv("XPN_SESSION_FILE");
  if (env_prefix != NULL)
  {
    priv_xpn_session_file = atoi(env_prefix);
  }else{
    // Default to 0
    priv_xpn_session_file = 0;
  }

  return priv_xpn_session_file;
}
