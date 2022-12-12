#!/bin/sh
# shellcheck disable=all
#set -x


#
# Default values
#
CONFNAME="$HOME/tmp/config.xml"
MACHINEFILE="$HOME/tmp/machinefile"
XPN_PARTITION_BSIZE="512k"
XPN_PARTITION_TYPE="NORMAL"
XPN_PARTITION_NAME="xpn"
XPN_STORAGE_PATH="/tmp"

#
# Auxiliar functions
#

intro() {
   echo ""
   echo " mk_conf 1.0"
   echo " -----------"
   echo ""
}

usage() {
   echo "Usage: $0 --conf ~/tmp/config.xml \\"
   echo "          --machinefile ~/tmp/machinefile \\"
   echo "          [--part_size <64|512k|1m|...>] \\"
   echo "          [--part_type NORMAL] \\"
   echo "          [--part_name <partition name>] \\"
   echo "          [--storage_path <server local storage path>]"
   echo ""
}

info() {
   echo " * configuration file name: "$CONFNAME
   echo " * machinefile:    "$MACHINEFILE
   echo " * partition size: "$XPN_PARTITION_BSIZE
   echo " * partition type: "$XPN_PARTITION_TYPE
   echo " * partition name: "$XPN_PARTITION_NAME
   echo " * storage path:   "$XPN_STORAGE_PATH
   echo ""
}

get_opts() {
   # Taken the general idea from https://stackoverflow.com/questions/70951038/how-to-use-getopt-long-option-in-bash-script
   mkconf_name=$(basename "$0")
   mkconf_short_opt=c:,m:,s:,t:,n,p:
   mkconf_long_opt=conf:,machinefile:,part_size:,part_type:,part_name:,storage_path:,help
   TEMP=$(getopt -o $mkconf_short_opt --long $mkconf_long_opt --name "$mkconf_name" -- "$@")
   eval set -- "${TEMP}"

   while :; do
      case "${1}" in
         -c | --conf         ) CONFNAME=$2;             shift 2 ;;
         -m | --machinefile  ) MACHINEFILE=$2;          shift 2 ;;
         -s | --part_size    ) XPN_PARTITION_BSIZE=$2;  shift 2 ;;
         -t | --part_type    ) XPN_PARTITION_TYPE=$2;   shift 2 ;;
         -n | --part_name    ) XPN_PARTITION_NAME=$2;   shift 2 ;;
         -p | --storage_path ) XPN_STORAGE_PATH=$2;     shift 2 ;;
         --help              ) intro; usage;  exit 0 ;;
         --                  ) shift;         break ;;
         *                   ) intro; echo " > ERROR: parsing arguments found an error :-/"; usage; exit 1 ;;
      esac
   done
}

check_opts() {
   if [ ! -f $MACHINEFILE ]; then
      echo " > ERROR: machinefile $MACHINEFILE does not exits :-/"
      exit 1
   fi

   # TODO: more checks around partition size, type, etc.
}

mk_conf_file() {
   echo "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"    > ${CONFNAME}
   echo "<xpn_conf>"                                        >> ${CONFNAME}

   XPN_PARTITION="<partition "
   XPN_PARTITION=$XPN_PARTITION" bsize='"${XPN_PARTITION_BSIZE}"' "
   XPN_PARTITION=$XPN_PARTITION" type='"${XPN_PARTITION_TYPE}"' "
   XPN_PARTITION=$XPN_PARTITION" name='"${XPN_PARTITION_NAME}"' "
   XPN_PARTITION=$XPN_PARTITION" >"
   echo "  $XPN_PARTITION"  >> ${CONFNAME}

   ITER=1
   while IFS= read -r line
   do
      XPN_DATA_NODE="<data_node "
      XPN_DATA_NODE=$XPN_DATA_NODE" url=\"mpiServer://$line/$XPN_STORAGE_PATH\" "
      XPN_DATA_NODE=$XPN_DATA_NODE" id=\"id${ITER}\" "
      XPN_DATA_NODE=$XPN_DATA_NODE" />"
      echo "    ${XPN_DATA_NODE}"  >> ${CONFNAME}
      ITER=$((${ITER}+1))
   done < ${MACHINEFILE}

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
mkdir -p $(dirname "$CONFNAME")
mk_conf_file
echo " Done."

