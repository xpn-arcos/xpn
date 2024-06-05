#!/bin/bash
#set -x

#
#  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Elias del Pozo Puñal, Dario Muñoz Muñoz
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
  REPLICATION_LEVEL=$4
  PARTITION_NAME=$5
  STORAGE_PATH=$6
  DEPLOYMENTFILE=$7

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
    echo " * REPLICATION_LEVEL=${REPLICATION_LEVEL}"
    echo " * PARTITION_NAME=${PARTITION_NAME}"
    echo " * STORAGE_PATH=${STORAGE_PATH}"
  fi

  if [[ ! -f ${DEPLOYMENTFILE} ]]; then
    ${BASE_DIR}/mk_conf.sh --conf         ${CONF_NAME} \
                           --machinefile  ${MACHINE_FILE} \
                           --part_bsize    ${PARTITION_SIZE} \
                           --replication_level    ${REPLICATION_LEVEL} \
                           --part_name    ${PARTITION_NAME} \
                           --storage_protocol     ${SERVER_TYPE}"_server" \
                           --storage_path ${STORAGE_PATH}
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

  if command -v srun &> /dev/null
  then
    # Create dir
    srun  -n "${NODE_NUM}" -N "${NODE_NUM}" \
          -w "${HOSTFILE}" \
          mkdir -p ${XPN_STORAGE_PATH}
    if [[ ${SERVER_TYPE} == "sck" ]]; then
      srun  -n "${NODE_NUM}" -N "${NODE_NUM}"\
            -w "${HOSTFILE}" \
            --export=ALL \
            "${BASE_DIR}"/../../src/xpn_server/xpn_server -s ${SERVER_TYPE} -t pool "${ARGS}" &
    else
      srun  -n "${NODE_NUM}" -N "${NODE_NUM}" --mpi=none \
            -w "${HOSTFILE}" \
            --export=ALL \
            "${BASE_DIR}"/../../src/xpn_server/xpn_server -s ${SERVER_TYPE} "${ARGS}" &
    fi

  else
    # Create dir
    mpiexec -np       "${NODE_NUM}" \
            -hostfile "${HOSTFILE}" \
            mkdir -p ${XPN_STORAGE_PATH}

    if [[ ${SERVER_TYPE} == "sck" ]]; then
      mpiexec -np       "${NODE_NUM}" \
              -hostfile "${HOSTFILE}" \
              "${BASE_DIR}"/../../src/xpn_server/xpn_server -s ${SERVER_TYPE} -t pool "${ARGS}" &
    else
      for ((i=1; i<=$NODE_NUM; i++))
      do
          line=$(head -n $i "$HOSTFILE" | tail -n 1)
          mpiexec -np       1 \
            -host "${line}" \
            ${BASE_DIR}/../../src/xpn_server/xpn_server -s ${SERVER_TYPE} ${ARGS} &
      done
    fi
  fi

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
  if command -v srun &> /dev/null
  then
    srun -n 1 -N 1 \
            "${BASE_DIR}"/../../src/xpn_server/xpn_stop_server -s ${SERVER_TYPE} -f ${DEATH_FILE}
  else
    mpiexec -np 1 \
            "${BASE_DIR}"/../../src/xpn_server/xpn_stop_server -s ${SERVER_TYPE} -f ${DEATH_FILE}
  fi
}

terminate_xpn_server() {

  if [[ ${VERBOSE} == true ]]; then
    echo " * DEATH_FILE: ${DEATH_FILE}"
    echo " * additional daemon args: ${ARGS}"
  fi

  if [[ ${SERVER_TYPE} == "mpi" ]]; then
    mpiexec -np 1 \
            ${BASE_DIR}/../../src/mpi_server/xpn_terminate_mpi_server -f ${DEATH_FILE} -h ${HOST}
  fi
}


rebuild_xpn_servers() {
  if [[ ${VERBOSE} == true ]]; then
    echo " * xpn storage path: ${XPN_STORAGE_PATH}"
    echo " * xpn old hostfile: ${DEATH_FILE}"
    echo " * xpn new hostfile: ${REBUILD_FILE}"
    echo " * xpn replication_level: ${XPN_REPLICATION_LEVEL}"
  fi

  NODE_NUM_SUM=$(($(cat ${DEATH_FILE} | wc -l) + $(cat ${REBUILD_FILE} | wc -l)))
  # 1. Copy
  if command -v srun &> /dev/null
  then
    # Create dir
    srun  -n "${NODE_NUM}" -N "${NODE_NUM}" \
          -w "${HOSTFILE}" \
          mkdir -p ${XPN_STORAGE_PATH}
    hosts=$(cat ${DEATH_FILE} ${REBUILD_FILE} | sort | paste -sd "," -)
    srun  -n "${NODE_NUM_SUM}" \
          -w "${hosts}" \
          "${BASE_DIR}"/../../src/utils/xpn_rebuild_active_writer "${XPN_STORAGE_PATH}" "${DEATH_FILE}" "${REBUILD_FILE}" 524288 "${XPN_REPLICATION_LEVEL}" 
  else
    # Create dir
    mpiexec -np       "${NODE_NUM}" \
            -hostfile "${HOSTFILE}" \
            mkdir -p ${XPN_STORAGE_PATH}
    hosts=$(cat ${DEATH_FILE} ${REBUILD_FILE} | sort | uniq -c | awk '{print $2":"$1}' | paste -sd "," -)
    mpiexec -l -np       "${NODE_NUM_SUM}" \
            -host "${hosts}" \
            "${BASE_DIR}"/../../src/utils/xpn_rebuild_active_writer "${XPN_STORAGE_PATH}" "${DEATH_FILE}" "${REBUILD_FILE}" 524288 "${XPN_REPLICATION_LEVEL}" 
  fi
}


preload_xpn() {

  if [[ ${VERBOSE} == true ]]; then
    echo " * source partition: ${SOURCE_PATH}"
    echo " * xpn storage path: ${XPN_STORAGE_PATH}"
  fi

  # 1. Copy
  if command -v srun &> /dev/null
  then
    srun  -n "${NODE_NUM}" -N "${NODE_NUM}" \
          -w "${HOSTFILE}" \
          "${BASE_DIR}"/../../src/utils/xpn_preload "${SOURCE_PATH}" "${XPN_STORAGE_PATH}" 524288 "${XPN_REPLICATION_LEVEL}"
  else
    mpiexec -np       "${NODE_NUM}" \
            -hostfile "${HOSTFILE}" \
            "${BASE_DIR}"/../../src/utils/xpn_preload "${SOURCE_PATH}" "${XPN_STORAGE_PATH}" 524288 "${XPN_REPLICATION_LEVEL}"
  fi
}

flush_xpn() {

  if [[ ${VERBOSE} == true ]]; then
    echo " * xpn storage path: ${XPN_STORAGE_PATH}"
    echo " * destination path: ${DEST_PATH}"
  fi

  # 1. Copy
  if command -v srun &> /dev/null
  then
    srun  -n "${NODE_NUM}" -N "${NODE_NUM}" \
          -w "${HOSTFILE}" \
          "${BASE_DIR}"/../../src/utils/xpn_flush "${XPN_STORAGE_PATH}" "${DEST_PATH}" 524288 "${XPN_REPLICATION_LEVEL}"
  else
    mpiexec -np       "${NODE_NUM}" \
            -hostfile "${HOSTFILE}" \
            "${BASE_DIR}"/../../src/utils/xpn_flush "${XPN_STORAGE_PATH}" "${DEST_PATH}" 524288 "${XPN_REPLICATION_LEVEL}"
  fi
}


usage_short() {
  echo ""
  echo " Usage: xpn.sh [-h/--help]"
  echo "               [-e/--execute <daemon type>] [-a/--args <daemon_args>] [-f/--foreground <false>]"
  echo "               [-c/--config <configuration file>]"
  echo "               [-p/--replication_level <replication_level>]"
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
  echo "     -e, --execute <arguments>           Server type: mpi, sck or tcp."
  echo "     -a, --args <arguments>              Add various additional daemon arguments."
  echo "     -f, --foreground                    Starts the script in the foreground. Daemons are stopped by pressing 'q'."
  echo "     -c, --config   <path>               Path to configuration file."
  echo "     -p, --replication_level   <n>       Replication level n."
  echo "     -m, --deployment_file   <path>      Path to deployment file."
  echo "     -n, --numnodes <n>                  XPN servers are started on n nodes."
  echo "     -r, --rootdir  <path>               The rootdir path for XPN daemons."
  echo "     -w, --workdir  <path>               The working directory path for XPN temporal files."
  echo "     -s, --source_path <path>            Origin XPN partition for the rebuild process or path for preload"
  echo "     -t, --destination_path <path>       Path for flush"
  echo "     -x, --xpn_storage_path <path>       The XPN local storage path"  
  echo "     -l, --hostfile  <path>              File with the hosts to be used to execute daemons (one per line)."
  echo "     -d, --deathfile <path>              File with the hosts to be used to stop    daemons (one per line)."
  echo "     -b, --rebuildfile <path>              File with the hosts to be used to stop    daemons (one per line)."
  echo "     -k, --host <host>                   Ip of the host to be used to terminate    daemons (one per line)."
  echo "     -v, --verbose                       Increase verbosity"
  echo ""
}

get_opts() {
   # Taken the general idea from https://stackoverflow.com/questions/70951038/how-to-use-getopt-long-option-in-bash-script
   mkconf_name=$(basename "$0")
   mkconf_short_opt=e:r:w:s:t:x:d:k:p:n:a:c:m:l:b:fvh
   mkconf_long_opt=execute:,rootdir:,workdir:,source_path:,destination_path:,xpn_storage_path:,numnodes:,args:,config:,deployment_file:,foreground_file,hostfile:,deathfile:,rebuildfile:,host:,replication_level:,verbose,help
   TEMP=$(getopt -o $mkconf_short_opt --long $mkconf_long_opt --name "$mkconf_name" -- "$@")
   eval set -- "${TEMP}"

   while :; do
      case "${1}" in
         -e | --execute          ) SERVER_TYPE=$2;              shift 2 ;;
         -r | --rootdir          ) DIR_ROOT=$2;                 shift 2 ;;
         -w | --workdir          ) WORKDIR=$2;                  shift 2 ;;
         -s | --source_path      ) SOURCE_PATH=$2;              shift 2 ;;
         -t | --destination_path ) DEST_PATH=$2;                shift 2 ;;
         -x | --xpn_storage_path ) XPN_STORAGE_PATH=$2;         shift 2 ;;
         -n | --numnodes         ) NODE_NUM=$2;                 shift 2 ;;
         -a | --args             ) ARGS=$2;                     shift 2 ;;
         -c | --config           ) FILE_CONFIG=$2;              shift 2 ;;
         -m | --deployment_file  ) DEPLOYMENTFILE=$2;           shift 2 ;;
         -f | --foreground_file  ) RUN_FOREGROUND=true;         shift 1 ;;
         -l | --hostfile         ) HOSTFILE=$2;                 shift 2 ;;
         -d | --deathfile        ) DEATH_FILE=$2;               shift 2 ;;
         -b | --rebuildfile      ) REBUILD_FILE=$2;               shift 2 ;;
         -k | --host             ) HOST=$2;                     shift 2 ;;
         -p | --replication_level) XPN_REPLICATION_LEVEL=$2;    shift 2 ;;
         -v | --verbose          ) VERBOSE=true;                shift 1 ;;
         -h | --help             ) usage_short; usage_details;  exit 0 ;;
         --                      ) shift;         break  ;;
         *                       ) intro; echo " > ERROR: parsing arguments found an error :-/"; usage; exit 1 ;;
      esac
   done

   ACTION=$*
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
REBUILD_FILE=""
HOST=""
SERVER_TYPE="mpi"
XPN_REPLICATION_LEVEL=0


## get arguments
BASE_DIR=$(dirname "$(readlink -f "$0")")/
get_opts $@

# load xpn_start.cfg
if [ -f "$FILE_CONFIG" ]; then
  source "$FILE_CONFIG"
fi

# run 
case "${ACTION}" in
      mk_conf)  mk_conf_servers  "config.xml" ${HOSTFILE} "512k" ${XPN_REPLICATION_LEVEL} "xpn" ${XPN_STORAGE_PATH} ${DEPLOYMENTFILE}
                ;;
      start)    mk_conf_servers  "config.xml" ${HOSTFILE} "512k" ${XPN_REPLICATION_LEVEL} "xpn" ${XPN_STORAGE_PATH} ${DEPLOYMENTFILE}
                start_xpn_servers
                ;;
      stop)     stop_xpn_servers
                ;;
      terminate)terminate_xpn_server
                ;;
      rebuild)  mk_conf_servers  "config.xml" ${REBUILD_FILE} "512k" ${XPN_REPLICATION_LEVEL} "xpn" ${XPN_STORAGE_PATH} ${DEPLOYMENTFILE}
                rebuild_xpn_servers
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

