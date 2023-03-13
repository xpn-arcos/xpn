#!/bin/bash
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

mk_conf_servers() {

  CONF_NAME=$1
  MACHINE_FILE=$2
  PARTITION_SIZE=$3
  PARTITION_NAME=$4
  STORAGE_PATH=$5

  # check params
  if [[ ${CONF_NAME} == "" ]]; then
    echo ""
    echo " ERROR: CONF_NAME is empty"
    exit -1
  fi
  if [[ ! -f ${MACHINE_FILE} ]]; then
    echo ""
    echo " ERROR: HOSTFILE '${MACHINE_FILE}' does not exist"
    exit -1
  fi 
  if [[ ${PARTITION_NAME} == "" ]]; then
    echo ""
    echo " ERROR: PARTITION_NAME is empty"
    exit -1
  fi
  if [[ ${STORAGE_PATH} == "" ]]; then
    echo ""
    echo " ERROR: STORAGE_PATH is empty"
    exit -1
  fi

  # verbose
  if [[ ${VERBOSE} == true ]]; then
    echo " * CONF_NAME=${CONF_NAME}"
    echo " * HOSTFILE=${MACHINE_FILE}"
    echo " * PARTITION_SIZE=${PARTITION_SIZE}"
    echo " * PARTITION_NAME=${PARTITION_NAME}"
    echo " * STORAGE_PATH=${STORAGE_PATH}"
  fi

  BASE_DIR="./scripts/execute/"
  ${BASE_DIR}/mk_conf.sh --conf         ${CONF_NAME} \
                         --machinefile  ${MACHINE_FILE} \
                         --part_size    ${PARTITION_SIZE} \
                         --part_name    ${PARTITION_NAME} \
                         --storage_path ${STORAGE_PATH}
}

start_mpi_servers() {
  BASE_DIR="./scripts/execute/"

  if [[ ${VERBOSE} == true ]]; then
    echo " * rootdir: ${DIR_ROOT}"
    echo " * hostfile: ${HOSTFILE}"
    echo " * node_num: ${NODE_NUM}"
    echo " * additional daemon args: ${ARGS}"
  fi

  if [[ ! -f ${HOSTFILE} ]]; then
    echo ""
    echo " ERROR: HOSTFILE '${HOSTFILE}' does not exist"
    exit -1
  fi

  rm -f /tmp/dns.txt
  touch /tmp/dns.txt

  mpiexec -np       "${NODE_NUM}" \
          -hostfile "${HOSTFILE}" \
          -genv LD_LIBRARY_PATH ../mxml/lib:"$LD_LIBRARY_PATH" \
          src/mpi_server/xpn_mpi_server -ns /tmp/dns.txt ${ARGS} &

  sleep 3

  if [[ ${RUN_FOREGROUND} == true ]]; then
    echo "Press 'q' to exit"
    while : ; do
      read -n 1 k <&1
      if [[ $k = q ]] ; then
        echo
        echo "Shutting down ..."
        stop_mpi_servers
        break
      else
        echo "Press 'q' to exit"
      fi
    done
  fi
}


stop_mpi_servers() {

  if [[ ${VERBOSE} == true ]]; then
    echo " * DEATH_FILE: ${DEATH_FILE}"
    echo " * additional daemon args: ${ARGS}"
  fi

  mpiexec -np 1 \
          -genv XPN_DNS /tmp/dns.txt \
          -genv LD_LIBRARY_PATH ../mxml/lib:"$LD_LIBRARY_PATH" \
          src/mpi_server/xpn_stop_mpi_server -f ${DEATH_FILE}
}


rebuild_mpi_servers() {

  if [[ ${VERBOSE} == true ]]; then
    echo " * source partition: ${SOURCE_PARTITION}"
    echo " * xpn storage path: ${XPN_STORAGE_PATH}"
  fi

  # 1. Get partition content
  mpiexec -np       "${NODE_NUM}" \
          -hostfile "${HOSTFILE}" \
          -genv     LD_LIBRARY_PATH ../mxml/lib:"$LD_LIBRARY_PATH" \
          -genv     XPN_DNS /tmp/dns.txt \
          -genv     XPN_CONF /local_test/test/configuration/conf.xml \
          -genv     LD_PRELOAD src/bypass/xpn_bypass.so \
          tree -fainc ${SOURCE_PARTITION} | head -n -2 | tail -n +2  | sed "s|${SOURCE_PARTITION}||g" > /tmp/partition_content.txt

  # 2. Copy
  mpiexec -np       "${NODE_NUM}" \
          -hostfile "${HOSTFILE}" \
          -genv      LD_LIBRARY_PATH ../mxml/lib:"$LD_LIBRARY_PATH" \
          -genv      XPN_DNS /tmp/dns.txt \
          -genv      XPN_CONF /local_test/test/configuration/conf.xml \
          -genv      LD_PRELOAD src/bypass/xpn_bypass.so \
          src/utils/xpn_rebuild ${SOURCE_PARTITION} ${XPN_STORAGE_PATH} 524288

  rm -f /tmp/partition_content.txt

  # 2. stop old servers
  stop_mpi_servers

  # 3. start new servers
  mk_conf_servers  "/tmp/config.xml" ${HOSTFILE} "512k" "xpn" ${XPN_STORAGE_PATH} 
  start_mpi_servers
}


usage_short() {
  echo ""
  echo " Usage: xpn.sh [-h/--help] [-a/--args <daemon_args>] [-f/--foreground <false>]"
  echo "               [-c/--config <configuration file>]"
  echo "               [-n/--numnodes <jobsize>]"
  echo "               [-l/--hostfile  <host file>]"
  echo "               [-d/--deathfile <host file>]"
  echo "               [-r/--rootdir <path>]"
  echo "               [-s/--source_partition <xpn_partition>]"
  echo "               [-x/--xpn_storage_path <path>]"
  echo "               [-v/--verbose <false>] {start,stop,rebuild}"
  echo ""
}

usage_details() {
  echo ""
  echo " This script simplifies the starting and stopping XPN ad-hoc servers."
  echo " The script looks for the 'xpn_start.cfg' file in the same directory where"
  echo " additional permanent configurations can be set."
  echo ""
  echo " positional arguments:"
  echo "     command                  Command to execute: 'start', 'stop' and 'rebuild'"
  echo ""
  echo " optional arguments:"
  echo "     -h, --help                          Shows this help message and exits"
  echo "     -a, --args <arguments>              Add various additional daemon arguments."
  echo "     -f, --foreground                    Starts the script in the foreground. Daemons are stopped by pressing 'q'."
  echo "     -c, --config   <path>               Path to configuration file. By defaults looks for a 'xpn_start.cfg' in this directory."
  echo "     -n, --numnodes <n>                  XPN servers are started on n nodes."
  echo "     -r, --rootdir  <path>               The rootdir path for XPN daemons."
  echo "     -s, --source_partition <partition>  Origin XPN partition (for the rebuild process)"
  echo "     -x, --xpn_storage_path <path>       The XPN local storage path"  
  echo "     -l, --hostfile  <path>              File with the hosts to be used to execute daemons (one per line)."
  echo "     -d, --deathfile <path>              File with the hosts to be used to stop    daemons (one per line)."
  echo "     -v, --verbose                       Increase verbosity"
  echo ""
}


## default values
ACTION=""
DIR_ROOT="/tmp/"
XPN_STORAGE_PATH="/tmp/"
SOURCE_PARTITION="xpn"
NODE_NUM=1
ARGS=""
#FILE_CONFIG=$(dirname "$(readlink -f "$0")")/xpn_start.cfg
FILE_CONFIG=""
RUN_FOREGROUND=false
VERBOSE=false
HOSTFILE="machinefile"
DEATH_FILE="machinefile"

## get arguments
while getopts "r:s:x:d:n:a:c:l:fvh" opt; do
  case "${opt}" in
    r) DIR_ROOT=${OPTARG}
       ;;
    s) SOURCE_PARTITION=${OPTARG}
       ;;
    x) XPN_STORAGE_PATH=${OPTARG}
       ;;
    n) NODE_NUM=${OPTARG}
       ;;
    a) ARGS=${OPTARG}
       ;;
    c) FILE_CONFIG=${OPTARG}
       ;;
    f) RUN_FOREGROUND=true
       ;;
    l) HOSTFILE=${OPTARG}
       ;;
    d) DEATH_FILE=${OPTARG}
       ;;
    v) VERBOSE=true
       ;;
    h) usage_short
       usage_details
       exit
       ;;
  esac
done

shift $((OPTIND - 1))
ACTION=$*

# load xpn_start.cfg
if [ -f "$FILE_CONFIG" ]; then
  source "$FILE_CONFIG"
fi

# run 
case "${ACTION}" in
      start)    mk_conf_servers  "/tmp/config.xml" ${HOSTFILE} "512k" "xpn" ${XPN_STORAGE_PATH}
                start_mpi_servers
                ;;
      stop)     stop_mpi_servers
                ;;
      rebuild)  rebuild_mpi_servers
                ;;
      *)        echo ""
                echo " ERROR: ACTION '${ACTION}' not supported"
                usage_short
                exit 1
                ;;
esac

