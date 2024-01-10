
# tcp_server for XPN

## Pre-requisites:
* In order to use mosquitto on tcp_server module, you have to install the following packages:

```
sudo apt-get install mosquitto mosquitto-clients mosquitto-dev libmosquitto-dev
```

* Create the mosquitto config file:

```
touch /etc/mosquitto/conf.d/mosquitto.conf
```

* Insert the following parameters:

```
port 2555
max_queued_messages 0
memory_limit 0
```

* Then, we need to start the Mosquitto MQTT service:

```
sudo systemctl restart mosquitto
```

## Compile:

* To compile the tcp_server just execute:
```
make clean
make
```

## Execution:

* In order to start one tcp_server at host "localhost" and port "7555", you can use:
```
echo localhost > machinefile
./scripts/execute/tcp_server.sh /tmp/xpn machinefile 7555
```


### Individual tcp_server

```
/work/xpn/src/tcp_server/xpn_tcp_server \
        -ns /export/tcp_server.dns \
        -p 3456
```


### Several tcp_server with MPI
```
mpirun -np 2 \
	-machinefile machinefile \
	/work/xpn/src/tcp_server/xpn_tcp_server \
	-ns /export/tcp_server.dns \
	-p 3456
```


### Using docker u20.sh
```
./u20.sh start 4
./u20.sh network | grep -v "Show" > machinefile
./u20.sh mpirun 2 "/work/xpn/src/tcp_server/xpn_tcp_server -ns /export/tcp_server.dns -p 3456" &
./u20.sh mpirun 2 "/work/xpn/test/integrity/xpn/run.sh"
./u20.sh stop
```

