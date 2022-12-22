#!/bin/bash
# shellcheck disable=all
#set -x

# 
#  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

# Check arguments
if [ "$#" != 1 ]; then
    echo ""
    echo " Usage:"
    echo " $0 <platform>"
    echo " Where:"
    echo " * platform = mn4 | cte-arm-fuji | cte-arm-mpich | picasso | tucan | lab21 | generic"
    echo ""
    exit
fi

# Do request
echo " Begin."
BASE_PATH=$(dirname $0)

case $1 in
   "mn4")
     $BASE_PATH/platform/mn4.sh
     ;;
   "cte-arm-fuji")
     $BASE_PATH/platform/cte-arm-fuji.sh
     ;;
    "cte-arm-mpich")
     $BASE_PATH/platform/cte-arm-mpich.sh
     ;;
   "picasso")
     $BASE_PATH/platform/picasso.sh
     ;;
   "lab21")
     $BASE_PATH/platform/lab21.sh
     ;;
    "tucan")
     $BASE_PATH/platform/tucan.sh
     ;;
    "generic")
     $BASE_PATH/platform/generic.sh
     ;;

   *)
     echo ""
     echo " ERROR: unknown platform '"$1"' :-("
     echo ""
     echo " Available platforms are:"
     echo " mn4 | cte-arm-fuji | cte-arm-mpich | picasso | tucan | lab21 | generic"
     echo ""
     ;;
esac

# Stop
echo " End."

