MPI="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16"

for I in $MPI; do
	mpirun -np $I -machinefile /export/home/pato11-1/proyectos/xpn/expand-2.0/test/pruebas//machines -nolocal /export/home/pato11-1/proyectos/xpn/expand-2.0/test/pruebas//IOPW /PNFS 2097152
done
