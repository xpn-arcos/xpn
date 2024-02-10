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
   echo " mk_conf 1.2"
   echo " -----------"
   echo ""
}

usage() {
   echo " Usage: $0 --conf              ~/tmp/config.xml \\"
   echo "           --machinefile       ~/tmp/machinefile \\"
   echo "           [--part_size        <64|512k|1m|...>] \\"
   echo "           [--part_type        NORMAL] \\"
   echo "           [--part_name        <partition name>] \\"
   echo "           [--storage_path     <server local storage path>] \\"
   echo "           [--storage_protocol <mpi_server|tcp_server>]"
   echo "           [--deployment_file  ~/tmp/deploymentfile] \\"
   echo ""
   echo " Deployment file has this format in general:"
   echo "   [ partition; block_size; part_type; protocol; host; path ]+"
   echo ""
   echo " Example:"
   echo "   particion; block_size; part_type; protocolo ; host     ; path"
   echo "   xpn1     ; 512k      ; NORMAL   ; mpi_server; compute-1; /tmp/"
   echo "            ;           ;          ; tcp_server; compute-2; /tmp/"
   echo "   xpn2     ; 1024k     ; NORMAL   ; mpi_server; compute-3; /tmp/"
   echo "            ;           ;          ; tcp_server; compute-4; /tmp/"
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
   echo " * deployment_file:  "${DEPLOYMENTFILE}
   echo ""
}

get_opts() {
   # Taken the general idea from https://stackoverflow.com/questions/70951038/how-to-use-getopt-long-option-in-bash-script
   mkconf_name=$(basename "$0")
   mkconf_short_opt=c:,m:,s:,t:,n,p:,x:,d:,h
   mkconf_long_opt=conf:,machinefile:,part_size:,part_type:,part_name:,storage_path:,storage_protocol:,deployment_file:,help
   TEMP=$(getopt -o $mkconf_short_opt --long $mkconf_long_opt --name "$mkconf_name" -- "$@")
   eval set -- "${TEMP}"

   while :; do
      case "${1}" in
         -c | --conf             ) CONFNAME=$2;             shift 2 ;;
         -m | --machinefile      ) MACHINEFILE=$2;          shift 2 ;;
         -s | --part_size        ) XPN_PARTITION_BSIZE=$2;  shift 2 ;;
         -t | --part_type        ) XPN_PARTITION_TYPE=$2;   shift 2 ;;
         -n | --part_name        ) XPN_PARTITION_NAME=$2;   shift 2 ;;
         -p | --storage_path     ) XPN_STORAGE_PATH=$2;     shift 2 ;;
         -x | --storage_protocol ) XPN_STORAGE_PROTOCOL=$2; shift 2 ;;
         -d | --deployment_file  ) DEPLOYMENTFILE=$2;       shift 2 ;;
         -h | --help             ) intro; usage;  exit 0 ;;
         --                      ) shift;         break  ;;
         *                       ) intro; echo " > ERROR: parsing arguments found an error :-/"; usage; exit 1 ;;
      esac
   done
}

check_opts() {

   if [ -f "$DEPLOYMENTFILE" ]; then
      return
   fi

   if [ ! -f "$MACHINEFILE" ]; then
      echo " > ERROR: machinefile ${MACHINEFILE} does not exits :-/"
      exit 1
   fi

   # TODO: more checks around partition block size, type, etc.
}

mk_conf_file_from_args() {
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

mk_conf_file_from_deploy() {

cat > mk_conf.awk <<EOF
#
# XPN GPL3
#
function ltrim(s) { sub(/^[ \t\r\n]+/, "", s); return s }
function rtrim(s) { sub(/[ \t\r\n]+$/, "", s); return s }
function trim(s)  { return rtrim(ltrim(s)); }

BEGIN {
         FS=";";

         print("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>");
         print("<xpn_conf>");

         partition="xpn";
         block_size="512k";
         part_type="NORMAL";
         protocol="mpi_server";
         host="localhost";
         path="/tmp";
      }
//    {
         if (NR != 1)
         {
            old_partition = partition;

            if (trim(\$1) != "") partition  = trim(\$1);
            if (trim(\$2) != "") block_size = trim(\$2);
            if (trim(\$3) != "") part_type  = trim(\$3);
            if (trim(\$4) != "") protocol   = trim(\$4);
            if (trim(\$5) != "") host       = trim(\$5);
            if (trim(\$6) != "") path       = trim(\$6);

            if ( (NR != 2) && (old_partition != partition) )
            {
                  print("  </partition>");
            }

            if ( (NR == 2) || (old_partition != partition) )
            {
                  print "  <partition bsize=\"" block_size "\" type=\"" part_type "\" name=\"" partition "\">";
            } 

                  print "    <data_node url=\"" protocol "://" host "/" path "\" id=\"id" NR "\"/>";
         } 
      } 
END   {
         print("</xpn_conf>");
}
EOF

awk -f mk_conf.awk ${DEPLOYMENTFILE} > ${CONFNAME}

rm mk_conf.awk
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

# make from deployment file
if [ -f "$DEPLOYMENTFILE" ]; then
  mk_conf_file_from_deploy
else
  mk_conf_file_from_args
fi
echo " Done."
