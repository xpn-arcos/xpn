#!/bin/bash



start_mpi_servers() {
    if [[ ${VERBOSE} == true ]]; then
        echo "### rootdir: ${ROOTDIR}"
        echo "### node_num: ${NODE_NUM}"
        echo "### additional daemon args: ${ARGS}"
    fi

}

stop_mpi_servers() {

}


usage_short() {
    echo "
usage: xpn.sh [-h/--help] [-a/--args <daemon_args>] [-f/--foreground <false>]
        [-c/--config <configuration file>] 
        [-n/--numnodes <jobsize>]   
        [-l/--hostfile <host file>] 
        [-r/--rootdir <path>] 
        [-v/--verbose <false>] {start,stop}
    "
}


help_msg() {
    usage_short
    echo "
    This script simplifies the starting and stopping XPN ad-hoc servers. 
    The script looks for the 'xpn.cfg' file in the same directory where
    additional permanent configurations can be set.

    positional arguments:
            command                  Command to execute: 'start' and 'stop'

    optional arguments:
            -h, --help               Shows this help message and exits
            -a, --args <daemon_arguments>
                                     Add various additional daemon arguments.
            -f, --foreground         Starts the script in the foreground. Daemons are stopped by pressing 'q'.
            -c, --config   <path>    Path to configuration file. By defaults looks for a 'xpn.cfg' in this directory.
            -n, --numnodes <n>       XPN servers are started on n nodes.
            -r, --rootdir  <path>    The rootdir path for GekkoFS daemons.
            -v, --verbose            Increase verbosity
            "
}


# Try to get xpn.cfg from arguments
CONFIGPATH=""
argv=("$@")

for i in "${argv[@]}"; do
    if [[ "${argv[i]}" == "-c" || "${argv[i]}" == "--config" ]]; then
        CONFIGPATH=$(readlink -mn "${argv[i+1]}")
        break
    fi
done

# get default xpn.cfg if wasn't given as arguments
SCRIPTDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"

if [[ -z ${CONFIGPATH} ]]; then
      CONFIGPATH="${SCRIPTDIR}/xpn.cfg"
fi
if [[ ! -f ${CONFIGPATH} ]]; then
    >&2 echo ">> No xpn.cfg file found at '${CONFIGPATH}'."
    exit 1
fi

# load xpn.cfg
source "$CONFIGPATH"

# more variables (default values)
VERBOSE=false
NODE_NUM=1
ROOTDIR=${DAEMON_ROOTDIR}
HOSTSFILE=${HOSTFILE_PATH}
ARGS=${DAEMON_ARGS}
RUN_FOREGROUND=false

# parse input
POSITIONAL=()
while [[ $# -gt 0 ]]; do
    key="$1"

    case ${key} in
    -r | --rootdir)
        ROOTDIR=$2
        shift # past argument
        shift # past value
        ;;
    -n | --numnodes)
        NODE_NUM=$2
        shift # past argument
        shift # past value
        ;;
    -a | --args)
        ARGS="${ARGS} $2"
        shift # past argument
        shift # past value
        ;;
    -f | --foreground)
        RUN_FOREGROUND=true
        shift # past argument
        ;;
    -c | --config)
            # skip. was handled above
            shift # past argument
            shift # past value
            ;;
    -h | --help)
        help_msg
        exit
        ;;
    -v | --verbose)
        VERBOSE=true
        shift # past argument
        ;;
    *) # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift              # past argument
        ;;
    esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

# check arguments
if [[ -z ${1+x} ]]; then
    echo "ERROR: Positional arguments missing."
    usage_short
    exit 1
fi
command="${1}"

if [[ ${command} != *"start"* ]] && [[ ${command} != *"stop"* ]]; then
    echo "ERROR: command ${command} not supported"
    usage_short
    exit 1
fi

# Run 
if [[ ${command} == "start" ]]; then
    start_mpi_servers
elif [[ ${command} == "stop" ]]; then
    stop_mpi_servers
fi
if [[ ${VERBOSE} == true ]]; then
    echo "Nothing left to do. Exiting :)"
fi

