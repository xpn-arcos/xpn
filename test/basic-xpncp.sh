#!/bin/bash
set -e

export TRASH_OFF=YES

FILES="$@"

if [ $# -le 0 ]
then
	#echo "Usage: $0 <file> [<file> ...]"
	#exit 1
	FILES="/home/bbergua/grande"
fi

#POLICIES="RAID0"
POLICIES="RAID0 RAID1"
#PROTOCOLS="file"
#PROTOCOLS="nfs3"
#PROTOCOLS="http"
#PROTOCOLS="gridftp"
PROTOCOLS="file nfs3 http gridftp"
#NUM_SERVERS="1 2 3 4"
NUM_SERVERS="1 2 4 8 16"
#COMMANDS="xpncp_m"
COMMANDS="xpncp xpncp_th xpncp_f xpncp_f_th xpncp_m"

HOST=`hostname -f`
#MACHINES=`cat $PBS_NODEFILE | uniq | grep -v $HOST`
#XPN_PART_DIR="/tmp/expand"
XPN_PART_DIR="/scratch/$USER"

ulimit -c unlimited

for ns in $NUM_SERVERS
do
	for ns2 in `seq $ns`
	do
		echo "mkdir -p ${XPN_PART_DIR}/expand${ns2}"
		mkdir -p ${XPN_PART_DIR}/expand${ns2}
	done

	HOSTS=`cat $PBS_NODEFILE | uniq | grep -v $HOST | head -$ns`

	for h in $HOSTS
	do
		echo "ssh -o StrictHostKeyChecking=no $h mkdir -p ${XPN_PART_DIR}/expand${ns} ; chmod 777 ${XPN_PART_DIR}/expand${ns}"
		ssh -o StrictHostKeyChecking=no $h "/bin/bash -c \"mkdir -p ${XPN_PART_DIR}/expand${ns} ; chmod 777 ${XPN_PART_DIR}/expand${ns}\""
	done
done

for po in $POLICIES
do
	for pr in $PROTOCOLS
	do
		for ns in $NUM_SERVERS
		do
			XPN_CONF_FILE="${XPN_PART_DIR}/xpn.conf.${po}.${pr}.${ns}"

			echo "Generating $XPN_CONF_FILE ..."

			rm -f $XPN_CONF_FILE

			echo -e "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" >> $XPN_CONF_FILE
			echo -e "<xpn_conf>" >> $XPN_CONF_FILE
			echo -e "\t<partition name=\"XPN\" type=\"${po}\" bsize=\"64k\" >" >> $XPN_CONF_FILE

			if [ "$pr" == "file" ]
			then
				for ns2 in `seq $ns`
				do
					#mkdir -p ${XPN_PART_DIR}/expand${ns2}

					echo -e "\t\t<data_node id=\"\" url=\"${pr}://${XPN_PART_DIR}/expand${ns2}\"/>" >> $XPN_CONF_FILE
				done
			else
				HOSTS=`cat $PBS_NODEFILE | uniq | grep -v $HOST | head -$ns`

				for h in $HOSTS
				do
					#echo "ssh -o StrictHostKeyChecking=no $h mkdir -p ${XPN_PART_DIR}/expand${ns} ; chmod 777 ${XPN_PART_DIR}/expand${ns}"
					#ssh -o StrictHostKeyChecking=no $h "mkdir -p ${XPN_PART_DIR}/expand${ns} ; chmod 777 ${XPN_PART_DIR}/expand${ns}"

					echo -e "\t\t<data_node id=\"\" url=\"${pr}://${h}${XPN_PART_DIR}/expand${ns}\"/>" >> $XPN_CONF_FILE
				done
			fi

			echo -e "\t</partition>" >> $XPN_CONF_FILE
			echo -e "</xpn_conf>" >> $XPN_CONF_FILE
		done
	done
done

for fl in $FILES
do
	for po in $POLICIES
	do
		for pr in $PROTOCOLS
		do
			for ns in $NUM_SERVERS
			do
				XPN_CONF_FILE="${XPN_PART_DIR}/xpn.conf.${po}.${pr}.${ns}"
				echo "XPN_CONF=\"$XPN_CONF_FILE\""

				for co in $COMMANDS
				do
					rm -f ${XPN_PART_DIR}/expand*/*
					rm -f ${XPN_PART_DIR}/outfile[25]

					echo -en "Testing $fl $po $pr $ns $co ...\t"

					echo -n "l->l "
					XPN_CONF="$XPN_CONF_FILE" $HOME/expand/trunk/expand/utils/$co -S $fl ${XPN_PART_DIR}/outfile2

					echo -n "l->r "
					XPN_CONF="$XPN_CONF_FILE" $HOME/expand/trunk/expand/utils/$co -S ${XPN_PART_DIR}/outfile2 xpn://XPN/outfile3

					echo -n "r->r "
					XPN_CONF="$XPN_CONF_FILE" $HOME/expand/trunk/expand/utils/$co -S xpn://XPN/outfile3 xpn://XPN/outfile4

					echo -n "r->l "
					XPN_CONF="$XPN_CONF_FILE" $HOME/expand/trunk/expand/utils/$co -S xpn://XPN/outfile4 ${XPN_PART_DIR}/outfile5

					echo -n "cmp "
					if cmp $fl ${XPN_PART_DIR}/outfile5
					then
						echo -e "\033[1;32mOK\033[0m"
					else
						echo -e "\033[1;31mFAIL\033[0m"
					fi

					rm -f ${XPN_PART_DIR}/expand*/*
					rm -f ${XPN_PART_DIR}/outfile[25]
				done
			done
		done
	done
done

echo "rm -rf ${XPN_PART_DIR}/expand*"
rm -rf ${XPN_PART_DIR}/expand*

HOSTS=`cat $PBS_NODEFILE | uniq | grep -v $HOST`

for h in $HOSTS
do
	echo "ssh -o StrictHostKeyChecking=no $h rm -rf ${XPN_PART_DIR}/expand*/"
	ssh -o StrictHostKeyChecking=no $h "rm -rf ${XPN_PART_DIR}/expand*/"
done
