
# sck_server for XPN

## 1. Compile:

* To compile the sck_server just execute:
```
make clean
make
```

## 2. Execution:


### 2.A Individual sck_server

```
/home/lab/src/xpn/src/sck_server/xpn_sck_server \
        -ns /shared/sck_server.dns \
        -p 3456
```

* For example, in order to start one sck_server at host "localhost" and port "7555":
  ```
  echo localhost > machinefile
  /home/lab/src/xpn/scripts/execute/sck_server.sh /tmp/xpn machinefile 7555
  ```


### 2.B Several sck_server with MPI
```
mpirun -np 2 \
        -machinefile machinefile \
        /home/lab/src/xpn/src/sck_server/xpn_sck_server \
        -ns /shared/sck_server.dns \
        -p 3456
```


### Using xpn-docker
```
./lab.sh start 4
./lab.sh network | grep -v "Show" > machinefile
./lab.sh mpirun 2 "/home/lab/src/xpn/src/sck_server/xpn_sck_server -ns /shared/sck_server.dns -p 3456" &
./lab.sh mpirun 2 "/home/lab/src/xpn/test/integrity/xpn/run.sh"
./lab.sh stop
```

