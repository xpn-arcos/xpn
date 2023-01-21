#!/bin/sh

BASE_PATH=$(dirname "$0")
if [ $(echo "$BASE_PATH" | head -c 1) != "/" ]
then
	BASE_PATH="$PWD/$BASE_PATH"
fi

usage () {
	echo "$0 <xpn_conf> <xpn_file_in> <xpn_file_out> <app> [options]"
}

if [ $# -lt 4 ]
then
	usage
	exit
fi

XPN_CONF="$1"
XPN_IN="$2"
XPN_OUT="$3"
shift 3

export XPN_CONF

"$BASE_PATH/xpncp" "$XPN_IN" in ; $@ ; "$BASE_PATH/xpncp" out "$XPN_OUT"

