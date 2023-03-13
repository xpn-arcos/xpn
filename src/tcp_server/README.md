
# tcp_server for XPN


## Compile:

* To compile the tcp_server just execute:
```
make clean
make
```

## Execution:

* In order to start one tcp_server at host "localhost" and port "555", you can use:
```
echo localhost > machinefile
./scripts/execute/tcp_server.sh /tmp/xpn machinefile 555
```

