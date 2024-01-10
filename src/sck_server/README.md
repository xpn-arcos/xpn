
# sck_server for XPN

## Compile:

* To compile the sck_server just execute:
```
make clean
make
```

## Execution:

* In order to start one sck_server at host "localhost" and port "7555", you can use:
```
echo localhost > machinefile
./scripts/execute/sck_server.sh /tmp/xpn machinefile 7555
```


### Individual sck_server

```
/work/xpn/src/sck_server/xpn_sck_server \
        -ns /export/sck_server.dns \
        -p 3456
```


### Several sck_server with MPI
```
mpirun -np 2 \
	-machinefile machinefile \
	/work/xpn/src/sck_server/xpn_sck_server \
	-ns /export/sck_server.dns \
	-p 3456
```


### Using docker u20.sh
```
./u20.sh start 4
./u20.sh network | grep -v "Show" > machinefile
./u20.sh mpirun 2 "/work/xpn/src/sck_server/xpn_sck_server -ns /export/sck_server.dns -p 3456" &
./u20.sh mpirun 2 "/work/xpn/test/integrity/xpn/run.sh"
./u20.sh stop
```

