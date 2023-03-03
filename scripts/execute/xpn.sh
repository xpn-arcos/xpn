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

start_mpi_servers() {
  BASE_DIR="./scripts/execute/"

  if [[ ${VERBOSE} == true ]]; then
    echo " * rootdir: ${DIR_ROOT}"
    echo " * node_num: ${NODE_NUM}"
    echo " * additional daemon args: ${ARGS}"
  fi

  if [[ ! -f ${HOSTFILE} ]]; then
    echo ""
    echo " ERROR: HOSTFILE '${HOSTFILE}' does not exist"
    exit -1
  fi

  ${BASE_DIR}/mk_conf.sh --conf /tmp/config.xml \
                         --machinefile ${HOSTFILE} \
                         --part_size 512k \
                         --part_name xpn \
                         --storage_path /tmp

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
    echo " * rootdir: ${DIR_ROOT}"
    echo " * node_num: ${NODE_NUM}"
    echo " * additional daemon args: ${ARGS}"
  fi

  mpiexec -np 1 \
          -genv XPN_DNS /tmp/dns.txt \
          -genv LD_LIBRARY_PATH ../mxml/lib:"$LD_LIBRARY_PATH" \
          src/mpi_server/xpn_stop_mpi_server -f ${HOSTFILE}
}







rebuild_mpi_servers() {

}







usage_short() {
  echo ""
  echo " Usage: xpn.sh [-h/--help] [-a/--args <daemon_args>] [-f/--foreground <false>]"
  echo "               [-c/--config <configuration file>]"
  echo "               [-n/--numnodes <jobsize>]"
  echo "               [-l/--hostfile <host file>]"
  echo "               [-r/--rootdir <path>]"
  echo "               [-v/--verbose <false>] {start,stop}"
  echo ""
}

usage_details() {
  echo ""
  echo " This script simplifies the starting and stopping XPN ad-hoc servers."
  echo " The script looks for the 'xpn_start.cfg' file in the same directory where"
  echo " additional permanent configurations can be set."
  echo ""
  echo " positional arguments:"
  echo "     command                  Command to execute: 'start' and 'stop'"
  echo ""
  echo " optional arguments:"
  echo "     -h, --help               Shows this help message and exits"
  echo "     -a, --args <arguments>   Add various additional daemon arguments."
  echo "     -f, --foreground         Starts the script in the foreground. Daemons are stopped by pressing 'q'."
  echo "     -c, --config   <path>    Path to configuration file. By defaults looks for a 'xpn_start.cfg' in this directory."
  echo "     -n, --numnodes <n>       XPN servers are started on n nodes."
  echo "     -r, --rootdir  <path>    The rootdir path for XPN daemons."
  echo "     -l, --hostfile <path>    File with the hosts to be used to execute daemons (one per line)."
  echo "     -v, --verbose            Increase verbosity"
  echo ""
}


## default values
ACTION=""
DIR_ROOT="/tmp/"
NODE_NUM=1
ARGS=""
#FILE_CONFIG=$(dirname "$(readlink -f "$0")")/xpn_start.cfg
FILE_CONFIG=""
RUN_FOREGROUND=false
VERBOSE=false
HOSTFILE=machinefile

## get arguments
while getopts "r:n:a:c:l:fvh" opt; do
  case "${opt}" in
    r) DIR_ROOT=${OPTARG}
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
      start)    start_mpi_servers
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

