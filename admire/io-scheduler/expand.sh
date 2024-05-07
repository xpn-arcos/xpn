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


usage_short() {
  echo ""
  echo " Usage: expand.sh [-h/--help] "
  echo "                  [-l/--hosts    <host1,host2,...>] "
  echo "                  [-d/--datadir  <path to datadir>]"
  echo "                  [-s/--sharedfir <path to sharedfir>]"
  echo "                  [-r/--replication_level <replication level>]"
  echo "                  [-v/--verbose <false>] {start,stop,expand,shrink}"
  echo ""
}


usage_details() {
  echo ""
  echo " This script simplifies the starting and stopping XPN ad-hoc servers."
  echo ""
  echo " positional arguments:"
  echo "     command                  Command to execute: 'start', 'stop', 'expand' and 'shrink'"
  echo ""
  echo " optional arguments:"
  echo "     -h, --help                               Shows this help message and exits."
  echo "     -l, --hosts              <arguments>     A comma separated list of valid job hostnames that can be used to start the adhoc servers."
  echo "     -d, --datadir            <path>          The default data directory for adhoc servers."
  echo "     -s, --shareddir          <path>          The default shared directory for adhoc servers."
  echo "     -r, --replication_level  <path>          The replication level to use."
  echo "     -v, --verbose                            Increase verbosity"
  echo ""
}

get_opts() {
   # Taken the general idea from https://stackoverflow.com/questions/70951038/how-to-use-getopt-long-option-in-bash-script
   mkconf_name=$(basename "$0")
   mkconf_short_opt=l:d:s:r:vh
   mkconf_long_opt=hosts:,datadir:,shareddir:,replication_level:,verbose,help
   TEMP=$(getopt -o $mkconf_short_opt --long $mkconf_long_opt --name "$mkconf_name" -- "$@")
   eval set -- "${TEMP}"

   while :; do
      case "${1}" in
         -l | --hosts            ) HOSTLIST=$2;                 shift 2 ;;
         -d | --datadir          ) DATADIR=$2;                  shift 2 ;;
         -s | --shareddir        ) SHAREDDIR=$2;                shift 2 ;;
         -r | --replication_level) REPLICATION_LEVEL=$2;        shift 2 ;;
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
HOSTLIST=$(hostname -I)
DATADIR="/tmp/expand/data"
SHAREDDIR=""
DEPLOYMENTFILE=""
NHOST=0
REPLICATION_LEVEL=0
VERBOSE=false

## get arguments
BASE_DIR=$(dirname "$(readlink -f "$0")")/
XPN_SH=${BASE_DIR}../../scripts/execute/xpn.sh
get_opts $@

if [ -z "$SHAREDDIR" ]; then
  echo "Error: shareddir not especified"
  usage_short
  exit 1
fi

HOSTFILE=${SHAREDDIR}/hostfile.txt
HOSTFILE_REBUILD=${SHAREDDIR}/hostfile_rebuild.txt
HOSTFILE_START=${SHAREDDIR}/hostfile_start.txt
HOSTFILE_STOP=${SHAREDDIR}/hostfile_stop.txt
HOSTFILE_REBUILD_JOIN=${SHAREDDIR}/hostfile_rebuild_join.txt

if [[ ${VERBOSE} == true ]]; then
  echo "HOSTS=${HOSTLIST}"
  echo "DATADIR=${DATADIR}"
  echo "SHAREDDIR=${SHAREDDIR}"
  echo "REPLICATION_LEVEL=${REPLICATION_LEVEL}"
  start_script=$(date +%s%3N)
fi
# run 
case "${ACTION}" in
      start)    rm -f  ${HOSTFILE}
                touch  ${HOSTFILE}
                rm -f  ${SHAREDDIR}/dns.txt
                touch  ${SHAREDDIR}/dns.txt
                # HOSTLIST -> HOSTFILE
                for i in $(echo ${HOSTLIST} | tr "," "\n")
                do
                  NHOST=$((NHOST+1))
                  echo $i >> ${HOSTFILE}
                done
                
                # xpn ...
                ${XPN_SH} --numnodes $NHOST --hostfile ${HOSTFILE} --xpn_storage_path ${DATADIR} --workdir ${SHAREDDIR} --replication_level ${REPLICATION_LEVEL} start
                ;;

      stop)     ${XPN_SH} --deathfile ${HOSTFILE} --workdir ${SHAREDDIR} stop
                rm -f ${HOSTFILE}
                ;;

      expand_v2 | shrink_v2)   
                rm -f  ${HOSTFILE_REBUILD}
                touch  ${HOSTFILE_REBUILD}
                # HOSTLIST -> HOSTFILE
                for i in $(echo ${HOSTLIST} | tr "," "\n")
                do
                  NHOST=$((NHOST+1))
                  echo $i >> ${HOSTFILE_REBUILD}
                done
                NHOST_OLD=$(cat ${HOSTFILE} | wc -l)

                diff ${HOSTFILE} ${HOSTFILE_REBUILD} | grep "^>" | cut -d' ' -f2 > ${HOSTFILE_START}
                diff ${HOSTFILE} ${HOSTFILE_REBUILD} | grep "^<" | cut -d' ' -f2 > ${HOSTFILE_STOP}
                NHOST_START=$(cat ${HOSTFILE_START} | wc -l)
                NHOST_STOP=$(cat ${HOSTFILE_STOP} | wc -l)

                if [ $NHOST_START -eq 0 ] && [ $NHOST_STOP -eq 0 ]; then
                    echo "The hosts provided are the same"
                    exit 0
                fi

                if [[ ${VERBOSE} == true ]]; then
                  start_stop=$(date +%s%3N)
                fi

                if [ $NHOST_START -gt 0 ]; then
                  ${XPN_SH} --numnodes ${NHOST_START} --hostfile ${HOSTFILE_START} --xpn_storage_path ${DATADIR} --workdir ${SHAREDDIR} --replication_level ${REPLICATION_LEVEL} start
                fi
                
                if [ $NHOST_STOP -gt 0 ]; then
                  ${XPN_SH} --deathfile ${HOSTFILE_STOP} --workdir ${SHAREDDIR} stop
                fi

                if [[ ${VERBOSE} == true ]]; then
                  end_stop=$(date +%s%3N)
                  seconds=$(((end_stop - start_stop) / 1000))
                  milliseconds=$(((end_stop - start_stop) % 1000))
                  echo "Time to run start-stop: $seconds.$milliseconds sec"
                fi
                
                cat ${HOSTFILE} ${HOSTFILE_REBUILD} | sort | uniq > ${HOSTFILE_REBUILD_JOIN}
                NHOST_REBUILD_JOIN=$(cat ${HOSTFILE_REBUILD_JOIN} | wc -l)

                if [[ ${VERBOSE} == true ]]; then
                  start_rebuild=$(date +%s%3N)
                fi

                ${XPN_SH} --numnodes ${NHOST_REBUILD_JOIN} --hostfile ${HOSTFILE_REBUILD_JOIN} --workdir ${SHAREDDIR} --xpn_storage_path ${DATADIR} --replication_level ${REPLICATION_LEVEL} --deathfile ${HOSTFILE} --rebuildfile ${HOSTFILE_REBUILD} rebuild

                if [[ ${VERBOSE} == true ]]; then
                  end_rebuild=$(date +%s%3N)
                  seconds=$(((end_rebuild - start_rebuild) / 1000))
                  milliseconds=$(((end_rebuild - start_rebuild) % 1000))
                  echo "Time to run rebuild: $seconds.$milliseconds sec"
                fi

                mv ${HOSTFILE_REBUILD} ${HOSTFILE}
                rm ${HOSTFILE_START}
                rm ${HOSTFILE_STOP}
                rm ${HOSTFILE_REBUILD_JOIN}
                ;;
      expand_v1 | shrink_v1)   
                rm -f  ${HOSTFILE_REBUILD}
                touch  ${HOSTFILE_REBUILD}
                # HOSTLIST -> HOSTFILE
                for i in $(echo ${HOSTLIST} | tr "," "\n")
                do
                  NHOST=$((NHOST+1))
                  echo $i >> ${HOSTFILE_REBUILD}
                done
                NHOST_OLD=$(cat ${HOSTFILE} | wc -l)

                diff ${HOSTFILE} ${HOSTFILE_REBUILD} | grep "^>" | cut -d' ' -f2 > ${HOSTFILE_START}
                diff ${HOSTFILE} ${HOSTFILE_REBUILD} | grep "^<" | cut -d' ' -f2 > ${HOSTFILE_STOP}
                NHOST_START=$(cat ${HOSTFILE_START} | wc -l)
                NHOST_STOP=$(cat ${HOSTFILE_STOP} | wc -l)

                if [ $NHOST_START -eq 0 ] && [ $NHOST_STOP -eq 0 ]; then
                    echo "The hosts provided are the same"
                    exit 0
                fi
                # stop
                if [[ ${VERBOSE} == true ]]; then
                  start_stop=$(date +%s%3N)
                fi

                ${XPN_SH} --deathfile ${HOSTFILE} --workdir ${SHAREDDIR} stop
                
                if [[ ${VERBOSE} == true ]]; then
                  end_stop=$(date +%s%3N)
                  seconds=$(((end_stop - start_stop) / 1000))
                  milliseconds=$(((end_stop - start_stop) % 1000))
                  echo "Time to run stop: $seconds.$milliseconds sec"
                fi
                
                cat ${HOSTFILE} ${HOSTFILE_REBUILD} | sort | uniq > ${HOSTFILE_REBUILD_JOIN}
                NHOST_REBUILD_JOIN=$(cat ${HOSTFILE_REBUILD_JOIN} | wc -l)

                if [[ ${VERBOSE} == true ]]; then
                  start_rebuild=$(date +%s%3N)
                fi

                ${XPN_SH} --numnodes ${NHOST_REBUILD_JOIN} --hostfile ${HOSTFILE_REBUILD_JOIN} --workdir ${SHAREDDIR} --xpn_storage_path ${DATADIR} --replication_level ${REPLICATION_LEVEL} --deathfile ${HOSTFILE} --rebuildfile ${HOSTFILE_REBUILD} rebuild

                if [[ ${VERBOSE} == true ]]; then
                  end_rebuild=$(date +%s%3N)
                  seconds=$(((end_rebuild - start_rebuild) / 1000))
                  milliseconds=$(((end_rebuild - start_rebuild) % 1000))
                  echo "Time to run rebuild: $seconds.$milliseconds sec"
                fi

                # start
                if [[ ${VERBOSE} == true ]]; then
                  start_start=$(date +%s%3N)
                fi

                mv ${HOSTFILE_REBUILD} ${HOSTFILE}
                ${XPN_SH} --numnodes $NHOST --hostfile ${HOSTFILE} --xpn_storage_path ${DATADIR} --workdir ${SHAREDDIR} --replication_level ${REPLICATION_LEVEL} start
                
                if [[ ${VERBOSE} == true ]]; then
                  end_start=$(date +%s%3N)
                  seconds=$(((end_start - start_start) / 1000))
                  milliseconds=$(((end_start - start_start) % 1000))
                  echo "Time to run start: $seconds.$milliseconds sec"
                fi

                rm ${HOSTFILE_START}
                rm ${HOSTFILE_STOP}
                rm ${HOSTFILE_REBUILD_JOIN}
                ;;
      expand | shrink)   
                rm -f  ${HOSTFILE_REBUILD}
                touch  ${HOSTFILE_REBUILD}
                # HOSTLIST -> HOSTFILE
                for i in $(echo ${HOSTLIST} | tr "," "\n")
                do
                  NHOST=$((NHOST+1))
                  echo $i >> ${HOSTFILE_REBUILD}
                done
                NHOST_OLD=$(cat ${HOSTFILE} | wc -l)
                # stop
                if [[ ${VERBOSE} == true ]]; then
                  start_stop=$(date +%s%3N)
                fi

                ${XPN_SH} --deathfile ${HOSTFILE} --workdir ${SHAREDDIR} stop
                
                if [[ ${VERBOSE} == true ]]; then
                  end_stop=$(date +%s%3N)
                  seconds=$(((end_stop - start_stop) / 1000))
                  milliseconds=$(((end_stop - start_stop) % 1000))
                  echo "Time to run stop: $seconds.$milliseconds sec"
                fi
                # flush
                if [[ ${VERBOSE} == true ]]; then
                  start_flush=$(date +%s%3N)
                fi

                mkdir -p ${SHAREDDIR}/tmp_shrink
                ${XPN_SH} --numnodes $NHOST_OLD --hostfile ${HOSTFILE} --xpn_storage_path ${DATADIR} --destination_path ${SHAREDDIR}/tmp_shrink --replication_level ${REPLICATION_LEVEL} flush
                
                if [[ ${VERBOSE} == true ]]; then
                  end_flush=$(date +%s%3N)
                  seconds=$(((end_flush - start_flush) / 1000))
                  milliseconds=$(((end_flush - start_flush) % 1000))
                  echo "Time to run flush: $seconds.$milliseconds sec"
                fi
                # start
                if [[ ${VERBOSE} == true ]]; then
                  start_start=$(date +%s%3N)
                fi

                mv ${HOSTFILE_REBUILD} ${HOSTFILE}
                ${XPN_SH} --numnodes $NHOST --hostfile ${HOSTFILE} --xpn_storage_path ${DATADIR} --workdir ${SHAREDDIR} --replication_level ${REPLICATION_LEVEL} start
                
                if [[ ${VERBOSE} == true ]]; then
                  end_start=$(date +%s%3N)
                  seconds=$(((end_start - start_start) / 1000))
                  milliseconds=$(((end_start - start_start) % 1000))
                  echo "Time to run start: $seconds.$milliseconds sec"
                fi
                # preload
                if [[ ${VERBOSE} == true ]]; then
                  start_preload=$(date +%s%3N)
                fi

                ${XPN_SH} --numnodes $NHOST --hostfile ${HOSTFILE} --source_path ${SHAREDDIR}/tmp_shrink --xpn_storage_path ${DATADIR} --replication_level ${REPLICATION_LEVEL} preload
                rm -r ${SHAREDDIR}/tmp_shrink
                
                if [[ ${VERBOSE} == true ]]; then
                  end_preload=$(date +%s%3N)
                  seconds=$(((end_preload - start_preload) / 1000))
                  milliseconds=$(((end_preload - start_preload) % 1000))
                  echo "Time to run preload: $seconds.$milliseconds sec"
                fi
                ;;
      *)        echo ""
                echo " ERROR: ACTION '${ACTION}' not supported"
                usage_short
                exit 1
                ;;
esac

if [[ ${VERBOSE} == true ]]; then
  end_script=$(date +%s%3N)
  seconds=$(((end_script - start_script) / 1000))
  milliseconds=$(((end_script - start_script) % 1000))
  echo "Time to run script: $seconds.$milliseconds sec"
fi
