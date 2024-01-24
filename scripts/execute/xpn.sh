#!/bin/bash
#set -x

#
#  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Elias del Pozo Puñal
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

  CONF_NAME=${WORKDIR}/$1
  MACHINE_FILE=$2
  PARTITION_SIZE=$3
  PARTITION_NAME=$4
  STORAGE_PATH=$5
  DEPLOYMENTFILE=$6

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

  if [[ ! -f ${DEPLOYMENTFILE} ]]; then
    "${BASE_DIR}"/mk_conf.sh --conf         "${CONF_NAME}" \
                           --machinefile  "${MACHINE_FILE}" \
                           --part_size    "${PARTITION_SIZE}" \
                           --part_name    "${PARTITION_NAME}" \
                           --storage_path "${STORAGE_PATH}"
  else
    "${BASE_DIR}"/mk_conf.sh --conf            "${CONF_NAME}" \
                           --deployment_file "${DEPLOYMENTFILE}"
  fi
}


start_xpn_servers() {

  if [[ ${VERBOSE} == true ]]; then
    echo " * rootdir: ${DIR_ROOT}"
    echo " * workdir: ${WORKDIR}"
    echo " * hostfile: ${HOSTFILE}"
    echo " * node_num: ${NODE_NUM}"
    echo " * additional daemon args: ${ARGS}"
  fi

  if [[ ! -f ${HOSTFILE} ]]; then
    echo ""
    echo " ERROR: HOSTFILE '${HOSTFILE}' does not exist"
    exit -1
  fi

  rm -f "${WORKDIR}/dns.txt"
  touch "${WORKDIR}/dns.txt"

  if [[ ${SERVER_TYPE} == "mpi" ]]; then
    mpiexec -np       "${NODE_NUM}" \
            -hostfile "${HOSTFILE}" \
            -genv LD_LIBRARY_PATH ../mxml/lib:"$LD_LIBRARY_PATH" \
            "${BASE_DIR}"/../../src/mpi_server/xpn_mpi_server -ns "${WORKDIR}"/dns.txt "${ARGS}" &
  else
    mpiexec -np       "${NODE_NUM}" \
            -hostfile "${HOSTFILE}" \
            -genv LD_LIBRARY_PATH ../mxml/lib:"$LD_LIBRARY_PATH" \
            "${BASE_DIR}"/../../src/tcp_server/xpn_tcp_server -ns "${WORKDIR}"/dns.txt "${ARGS}" -p 3456 &
  fi

  sleep 3

  if [[ ${RUN_FOREGROUND} == true ]]; then
    echo "Press 'q' to exit"
    while : ; do
      read -n 1 k <&1
      if [[ $k = q ]] ; then
        echo
        echo "Shutting down ..."
        stop_xpn_servers
        break
      else
        echo "Press 'q' to exit"
      fi
    done
  fi
}


stop_xpn_servers() {

  if [[ ${VERBOSE} == true ]]; then
    echo " * DEATH_FILE: ${DEATH_FILE}"
    echo " * additional daemon args: ${ARGS}"
  fi

  if [[ ${SERVER_TYPE} == "mpi" ]]; then
    mpiexec -np 1 \
            -genv XPN_DNS "${WORKDIR}"/dns.txt \
            -genv LD_LIBRARY_PATH ../mxml/lib:"$LD_LIBRARY_PATH" \
            "${BASE_DIR}"/../../src/mpi_server/xpn_stop_mpi_server -f "${DEATH_FILE}"
  else
    mpiexec -np 1 \
            -genv XPN_DNS "${WORKDIR}"/dns.txt \
            -genv LD_LIBRARY_PATH ../mxml/lib:"$LD_LIBRARY_PATH" \
            "${BASE_DIR}"/../../src/tcp_server/xpn_stop_tcp_server -f "${DEATH_FILE}"
  fi
}


rebuild_xpn_servers() {

  if [[ ${VERBOSE} == true ]]; then
    echo " * source partition: ${SOURCE_PATH}"
    echo " * xpn storage path: ${XPN_STORAGE_PATH}"
  fi

  # 1. Copy
  mpiexec -np       "${NODE_NUM}" \
          -hostfile "${HOSTFILE}" \
          -genv      LD_LIBRARY_PATH ../mxml/lib:"$LD_LIBRARY_PATH" \
          -genv      XPN_DNS "${WORKDIR}"/dns.txt \
          -genv      XPN_CONF /local_test/test/configuration/conf.xml \
          -genv      LD_PRELOAD src/bypass/xpn_bypass.so \
          -genv      XPN_LOCALITY 0\
          "${BASE_DIR}"/../../src/utils/xpn_rebuild "${SOURCE_PATH}" "${XPN_STORAGE_PATH}" 524288

  rm -f "${WORKDIR}"/partition_content.txt

  # 2. stop old servers
  stop_xpn_servers

  # 3. start new servers
  mk_conf_servers  "config.xml" "${HOSTFILE}" "512k" "xpn" "${XPN_STORAGE_PATH}" "${DEPLOYMENTFILE}"
  start_xpn_servers
}


preload_xpn() {

  if [[ ${VERBOSE} == true ]]; then
    echo " * source partition: ${SOURCE_PATH}"
    echo " * xpn storage path: ${XPN_STORAGE_PATH}"
  fi

  # 1. Copy
  mpiexec -np       "${NODE_NUM}" \
          -hostfile "${HOSTFILE}" \
          "${BASE_DIR}"/../../src/utils/xpn_preload "${SOURCE_PATH}" "${XPN_STORAGE_PATH}" 524288
}

flush_xpn() {

  if [[ ${VERBOSE} == true ]]; then
    echo " * xpn storage path: ${XPN_STORAGE_PATH}"
    echo " * destination path: ${DEST_PATH}"
  fi

  # 1. Copy
  mpiexec -np       "${NODE_NUM}" \
          -hostfile "${HOSTFILE}" \
          "${BASE_DIR}"/../../src/utils/xpn_flush "${XPN_STORAGE_PATH}" "${DEST_PATH}" 524288
}


usage_short() {
  echo ""
  echo " Usage: xpn.sh [-h/--help] [-a/--args <daemon_args>] [-f/--foreground <false>]"
  echo "               [-c/--config <configuration file>]"
  echo "               [-m/--deployment_file <deployment file>]"
  echo "               [-n/--numnodes <jobsize>]"
  echo "               [-l/--hostfile  <host file>]"
  echo "               [-d/--deathfile <host file>]"
  echo "               [-r/--rootdir <path>]"
  echo "               [-w/--workdir <path>]"
  echo "               [-s/--source_path <path>]"
  echo "               [-t/--destination_path <path>]"
  echo "               [-x/--xpn_storage_path <path>]"
  echo "               [-v/--verbose <false>] {start,stop,rebuild,preload,flush}"
  echo ""
}


usage_details() {
  echo ""
  echo " This script simplifies the starting and stopping XPN ad-hoc servers."
  echo " The script looks for the 'xpn_start.cfg' file in the same directory where"
  echo " additional permanent configurations can be set."
  echo ""
  echo " positional arguments:"
  echo "     command                  Command to execute: 'start', 'stop', 'rebuild', 'preload' and 'flush'"
  echo ""
  echo " optional arguments:"
  echo "     -h, --help                          Shows this help message and exits"
  echo "     -a, --args <arguments>              Add various additional daemon arguments."
  echo "     -f, --foreground                    Starts the script in the foreground. Daemons are stopped by pressing 'q'."
  echo "     -c, --config   <path>               Path to configuration file."
  echo "     -m, --deployment_file   <path>      Path to deployment file."
  echo "     -n, --numnodes <n>                  XPN servers are started on n nodes."
  echo "     -r, --rootdir  <path>               The rootdir path for XPN daemons."
  echo "     -w, --workdir  <path>               The working directory path for XPN temporal files."
  echo "     -s, --source_path <path>            Origin XPN partition for the rebuild process or path for preload"
  echo "     -t, --destination_path <path>       Path for flush"
  echo "     -x, --xpn_storage_path <path>       The XPN local storage path"  
  echo "     -l, --hostfile  <path>              File with the hosts to be used to execute daemons (one per line)."
  echo "     -d, --deathfile <path>              File with the hosts to be used to stop    daemons (one per line)."
  echo "     -v, --verbose                       Increase verbosity"
  echo ""
}


## default values
ACTION=""
WORKDIR="/tmp/"
DIR_ROOT="/tmp/"
XPN_STORAGE_PATH="/tmp/"
SOURCE_PATH="xpn"
DEST_PATH="xpn"
NODE_NUM=1
ARGS=""
FILE_CONFIG=""
DEPLOYMENTFILE=""
RUN_FOREGROUND=false
VERBOSE=false
HOSTFILE="machinefile"
DEATH_FILE="machinefile"
SERVER_TYPE="mpi"


## get arguments
while getopts "r:w:s:t:x:d:n:a:c:m:l:fvh" opt; do
  case "${opt}" in
    r) DIR_ROOT=${OPTARG}
       ;;
    w) WORKDIR=${OPTARG}
       ;;
    s) SOURCE_PATH=${OPTARG}
       ;;
    t) DEST_PATH=${OPTARG}
       ;;
    x) XPN_STORAGE_PATH=${OPTARG}
       ;;
    n) NODE_NUM=${OPTARG}
       ;;
    a) ARGS=${OPTARG}
       ;;
    c) FILE_CONFIG=${OPTARG}
       ;;
    m) DEPLOYMENTFILE=${OPTARG}
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

BASE_DIR=$(dirname "$(readlink -f "$0")")/

shift $((OPTIND - 1))
ACTION=$*

# load xpn_start.cfg
if [ -f "$FILE_CONFIG" ]; then
  source "$FILE_CONFIG"
fi

# run 
case "${ACTION}" in
      start)    mk_conf_servers  "config.xml" "${HOSTFILE}" "512k" "xpn" "${XPN_STORAGE_PATH}" "${DEPLOYMENTFILE}"
                start_xpn_servers
                ;;
      stop)     stop_xpn_servers
                ;;
      rebuild)  rebuild_xpn_servers
                ;;
      preload)  preload_xpn
                ;;
      flush)    flush_xpn
                ;;
      *)        echo ""
                echo " ERROR: ACTION '${ACTION}' not supported"
                usage_short
                exit 1
                ;;
esac

