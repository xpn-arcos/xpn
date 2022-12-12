#!/bin/sh
set -x

DIR="/export/home/pato11-1/proyectos/xpn/expand-2.0/test/pruebas/"
LBUF="2097152 1048576 524288 262144 131072 65536 32768 16384 8192 4096 2048 1024 512 256 128"
#TYPE="file file.hilos udp udp.hilos tcp tcp.hilos"
PATOS="1 2 4 8 16"

TYPE="udp udp.hilos tcp tcp.hilos"
SIZE="1 8 16 64"

for X in $TYPE; do
       for Y in $SIZE; do
		cp $DIR/conf/xpn.$Y.$X.conf $DIR/conf/xpn.conf
		$DIR/sync.sh
		$DIR/delete.sh
		$DIR/sync.sh

		for I in $PATOS; do
			for J in $LBUF; do
				$DIR/sync.sh
				$DIR/lanza.sh IONW /PNFS $J $I xpn.$Y.$X.conf
				#sleep 5
			done
			$DIR/sync.sh 
			#sleep 10
			for J in $LBUF; do
				$DIR/sync.sh
				$DIR/lanza.sh IONR /PNFS $J $I xpn.$Y.$X.conf
				#sleep 5
			done
			$DIR/delete.sh
		done
	
		$DIR/sync.sh
		$DIR/delete.sh
		$DIR/sync.sh
	done
done

$DIR/umountdirs.sh
$DIR/mountdirstcp.sh
$DIR/sync.sh
$DIR/delete.sh
$DIR/sync.sh

TYPE="file file.hilos"

for X in $TYPE; do
       for Y in $SIZE; do
		cp $DIR/conf/xpn.$Y.$X.conf $DIR/conf/xpn.conf
		$DIR/sync.sh
		$DIR/delete.sh
		$DIR/sync.sh

		for I in $PATOS; do
			for J in $LBUF; do
				$DIR/sync.sh
				$DIR/lanza.sh IONW /PNFS $J $I xpn.$Y.$X.conf
				#sleep 5
			done
			$DIR/sync.sh 
			#sleep 10
			for J in $LBUF; do
				$DIR/sync.sh
				$DIR/lanza.sh IONR /PNFS $J $I xpn.$Y.$X.conf
				#sleep 5
			done
			$DIR/delete.sh
		done
	
		$DIR/sync.sh
		$DIR/delete.sh
		$DIR/sync.sh
	done
done

$DIR/umountdirs.sh
$DIR/mountdirsudp.sh
$DIR/sync.sh
$DIR/delete.sh
$DIR/sync.sh

for X in $TYPE; do
       for Y in $SIZE; do
		cp $DIR/conf/xpn.3.$Y.$X.conf $DIR/conf/xpn.conf
		$DIR/sync.sh
		$DIR/delete.sh
		$DIR/sync.sh

		for I in $PATOS; do
			for J in $LBUF; do
				$DIR/sync.sh
				$DIR/lanza.sh IONW /PNFS $J $I xpn.3.$Y.$X.conf
				#sleep 5
			done
			$DIR/sync.sh 
			#sleep 10
			for J in $LBUF; do
				$DIR/sync.sh
				$DIR/lanza.sh IONR /PNFS $J $I xpn.3.$Y.$X.conf
				#sleep 5
			done
			$DIR/delete.sh
		done
	
		$DIR/sync.sh
		$DIR/delete.sh
		$DIR/sync.sh
	done
done


$DIR/umountdirs.sh
$DIR/mountdirstcp.sh
$DIR/sync.sh
$DIR/delete.sh
$DIR/sync.sh

for I in $PATOS; do
	for J in $LBUF; do
		$DIR/sync.sh
		$DIR/lanza1.sh IONsW /export/temp/pato23/ $J $I tcp
		#sleep 5
	done
	$DIR/sync.sh 
	#sleep 10
	for J in $LBUF; do
		$DIR/sync.sh
		$DIR/lanza1.sh IONsR /export/temp/pato23/ $J $I tcp
		#sleep 5
	done
	$DIR/delete.sh
done


$DIR/umountdirs.sh
$DIR/mountdirsudp.sh
$DIR/sync.sh
$DIR/delete.sh
$DIR/sync.sh

for I in $PATOS; do
	for J in $LBUF; do
		$DIR/sync.sh
		$DIR/lanza1.sh IONsW /export/temp/pato23/ $J $I udp
		#sleep 5
	done
	$DIR/sync.sh 
	#sleep 10
	for J in $LBUF; do
		$DIR/sync.sh
		$DIR/lanza1.sh IONsR /export/temp/pato23/ $J $I udp
		#sleep 5
	done
	$DIR/delete.sh
done
