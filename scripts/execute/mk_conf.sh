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


#
# Default values
#
CONFNAME="$HOME/tmp/config.xml"
MACHINEFILE="$HOME/tmp/machinefile"
XPN_PARTITION_BSIZE="512k"
XPN_PARTITION_TYPE="NORMAL"
XPN_PARTITION_NAME="xpn"
XPN_STORAGE_PATH="/tmp"
XPN_STORAGE_PROTOCOL="mpi_server"


#
# Auxiliar functions
#

intro() {
   echo ""
   echo " mk_conf 1.1"
   echo " -----------"
   echo ""
}

usage() {
   echo "Usage: $0 --conf              ~/tmp/config.xml \\"
   echo "          --machinefile       ~/tmp/machinefile \\"
   echo "          [--part_bsize       <64|512k|1m|...>] \\"
   echo "          [--part_type        NORMAL] \\"
   echo "          [--part_name        <partition name>] \\"
   echo "          [--storage_path     <server local storage path>] \\"
   echo "          [--storage_protocol <mpi_server|tcp_server>]"
   echo ""
}

info() {
   echo " * configuration file name: "${CONFNAME}
   echo " * machinefile:      "${MACHINEFILE}
   echo " * partition bsize:  "${XPN_PARTITION_BSIZE}
   echo " * partition type:   "${XPN_PARTITION_TYPE}
   echo " * partition name:   "${XPN_PARTITION_NAME}
   echo " * storage path:     "${XPN_STORAGE_PATH}
   echo " * storage protocol: "${XPN_STORAGE_PROTOCOL}
   echo ""
}

get_opts() {
   # Taken the general idea from https://stackoverflow.com/questions/70951038/how-to-use-getopt-long-option-in-bash-script
   mkconf_name=$(basename "$0")
   mkconf_short_opt=c:,m:,s:,t:,n,p:,x:,h
   mkconf_long_opt=conf:,machinefile:,part_bsize:,part_type:,part_name:,storage_path:,storage_protocol:,help
   TEMP=$(getopt -o $mkconf_short_opt --long $mkconf_long_opt --name "$mkconf_name" -- "$@")
   eval set -- "${TEMP}"

   while :; do
      case "${1}" in
         -c | --conf             ) CONFNAME=$2;             shift 2 ;;
         -m | --machinefile      ) MACHINEFILE=$2;          shift 2 ;;
         -s | --part_bsize       ) XPN_PARTITION_BSIZE=$2;  shift 2 ;;
         -t | --part_type        ) XPN_PARTITION_TYPE=$2;   shift 2 ;;
         -n | --part_name        ) XPN_PARTITION_NAME=$2;   shift 2 ;;
         -p | --storage_path     ) XPN_STORAGE_PATH=$2;     shift 2 ;;
         -x | --storage_protocol ) XPN_STORAGE_PROTOCOL=$2; shift 2 ;;
         -h | --help             ) intro; usage;  exit 0 ;;
         --                      ) shift;         break  ;;
         *                       ) intro; echo " > ERROR: parsing arguments found an error :-/"; usage; exit 1 ;;
      esac
   done
}

check_opts() {
   if [ ! -f $MACHINEFILE ]; then
      echo " > ERROR: machinefile ${MACHINEFILE} does not exits :-/"
      exit 1
   fi

   # TODO: more checks around partition bsize, type, etc.
}

mk_conf_file() {
   echo "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"    > ${CONFNAME}
   echo "<xpn_conf>"                                        >> ${CONFNAME}

   XPN_PARTITION="<partition "
   XPN_PARTITION=${XPN_PARTITION}" bsize='"${XPN_PARTITION_BSIZE}"' "
   XPN_PARTITION=${XPN_PARTITION}"  type='"${XPN_PARTITION_TYPE}"' "
   XPN_PARTITION=${XPN_PARTITION}"  name='"${XPN_PARTITION_NAME}"' "
   XPN_PARTITION=${XPN_PARTITION}" >"
   echo "  ${XPN_PARTITION}"    >> ${CONFNAME}

   ITER=1
   while read line
   do
      XPN_DATA_NODE="<data_node "
      XPN_DATA_NODE=${XPN_DATA_NODE}" url=\"${XPN_STORAGE_PROTOCOL}://${line}/${XPN_STORAGE_PATH}\" "
      XPN_DATA_NODE=${XPN_DATA_NODE}" id=\"id${ITER}\" "
      XPN_DATA_NODE=${XPN_DATA_NODE}" />"
      echo "    ${XPN_DATA_NODE}"  >> ${CONFNAME}
      ITER=$((${ITER}+1))
   done < <(grep . ${MACHINEFILE})

   echo "  </partition>"    >> ${CONFNAME}
   echo "</xpn_conf>"       >> ${CONFNAME}
}


#
# Main
#

# Check arguments, and print it
get_opts $@
intro
check_opts
info

# Make XPN configuration file
mkdir -p $(dirname "${CONFNAME}")
mk_conf_file
echo " Done."

