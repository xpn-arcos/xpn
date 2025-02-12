#!/bin/bash
# shellcheck disable=all
#set -x

# 
#  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
#  
#  This file is part of Expand.
#  
#  Expand is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#  
#  Expand is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public License
#  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
#  


# Header
echo ""
echo " platform"
echo " --------"
echo ""

BASE_PATH=$(dirname $0)
PLATFORM_NAME=$1
PLATFORMS_AVAILABLE=$(ls -1 ${BASE_PATH}/platform | tr -d '\n' | sed 's/.sh/ | /g' | sed 's/| $//g')

# Check arguments
if [ "$#" != 1 ]; then
    echo ""
    echo " Usage:"
    echo " $0 <platform>"
    echo " Where:"
    echo " * platform = $PLATFORMS_AVAILABLE"
    echo ""
    exit
fi

# Do request
echo " Begin."

if [ ! -f ${BASE_PATH}/platform/$1.sh ]; then
     echo ""
     echo " ERROR: unknown platform '"$1"' :-("
     echo ""
     echo " Available platforms are:"
     echo " $PLATFORMS_AVAILABLE"
     echo ""
     exit
else
     ${BASE_PATH}/platform/$1.sh
fi

# Stop
echo " End."
