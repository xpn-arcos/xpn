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
  echo "                  [-w/--workdir  <path to workdir>]"
  echo "                  [-d/--datadir  <path to datadir>]"
  echo "                  [-m/--mountdir <path to mountdir>]"
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
  echo "     -h, --help                      Shows this help message and exits."
  echo "     -l, --hosts     <arguments>     A comma separated list of valid job hostnames that can be used to start the adhoc servers."
  echo "     -w, --workdir   <path>          The default working directory for adhoc servers."
  echo "     -d, --datadir   <path>          The default data directory for adhoc servers."
  echo "     -m, --mountdir  <path>          The default mount directory for adhoc servers."
  echo "     -v, --verbose                   Increase verbosity"
  echo ""
}

get_opts() {
   # Taken the general idea from https://stackoverflow.com/questions/70951038/how-to-use-getopt-long-option-in-bash-script
   mkconf_name=$(basename "$0")
   mkconf_short_opt=l:w:d:m:vh
   mkconf_long_opt=hosts:,workdir:,datadir:,mountdir:,verbose,help
   TEMP=$(getopt -o $mkconf_short_opt --long $mkconf_long_opt --name "$mkconf_name" -- "$@")
   eval set -- "${TEMP}"

   while :; do
      case "${1}" in
         -e | --hosts            ) HOSTLIST=$2;                 shift 2 ;;
         -r | --workdir          ) WORKDIR=$2;                  shift 2 ;;
         -w | --datadir          ) DATADIR=$2;                  shift 2 ;;
         -s | --mountdir         ) MOUNTDIR=$2;                 shift 2 ;;
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
HOSTLIST="localhost"
WORKDIR="/tmp/expand"
DATADIR="/tmp/expand/data"
MOUNTDIR="/tmp/expand/mnt"
DEPLOYMENTFILE=""
NHOST=0
VERBOSE=false

## get arguments
BASE_DIR=$(dirname "$(readlink -f "$0")")/
get_opts $@

# run 
case "${ACTION}" in
      start)    # HOSTLIST -> HOSTFILE
                rm -fr /tmp/hostfile.txt
                touch  /tmp/hostfile.txt
                for i in $(echo ${HOSTLIST} | tr "," "\n")
                do
                	NHOST=$((NHOST+1))
                  echo $i >> /tmp/hostfile.txt
                done

								# xpn ...
                ./xpn -n $NHOST -l /tmp/hostfile.txt -x ${DATADIR} start
                ;;

      stop)     ./xpn -l /tmp/hostfile.txt stop
								rm -f /tmp/hostfile.txt
                ;;

      expand)   
      shrink)   # HOSTLIST -> HOSTFILE
                rm -fr /tmp/hostfile_rebuild.txt
                touch  /tmp/hostfile_rebuild.txt
                for i in $(echo ${HOSTLIST} | tr "," "\n")
                do
                	NHOST=$((NHOST+1))
                  echo $i >> /tmp/hostfile_rebuild.txt
                done

                # rebuild from old to new...
                ./xpn -n $NHOST -l /tmp/hostfile_rebuild.txt -s ${MOUNTDIR} -x ${DATADIR} rebuild

                mv /tmp/hostfile_rebuild.txt /tmp/hostfile.txt
                ;;
                
      *)        echo ""
                echo " ERROR: ACTION '${ACTION}' not supported"
                usage_short
                exit 1
                ;;
esac

