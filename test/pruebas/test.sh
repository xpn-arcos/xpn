set -x

PATOS="1"
#LBUF="2097152 1048576 524288 262144 131072 65536 32768"
LBUF="1048576"
DIR="/export/home/pato11-1/proyectos/xpn/expand-2.0/test/pruebas/"
for J in $PATOS; do
	for S in $LBUF; do
		mpirun -np $J -machinefile $DIR/machines  -nolocal $DIR/IONW /PNFS $S
	done
	for S in $LBUF; do
		mpirun -np $J -machinefile $DIR/machines  -nolocal $DIR/IONR /PNFS $S
	done
done
