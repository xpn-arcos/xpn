
# xpn_server with mosquitto XPN


## Pre-requisites:
* In order to use mosquitto on mq_server module, you have to install the following packages:
```
sudo apt-get install mosquitto mosquitto-clients mosquitto-dev libmosquitto-dev
```

* Then, we need to stop the Mosquitto MQTT service:
```
sudo systemctl stop mosquitto
```


## Compile:

* To compile the examples you just need to execute:
```
make clean; make
```


## Execution:

### Mosquitto
* Create the mosquitto configuration file:
```
touch mosquitto.conf
```

* Insert the following parameters:
```
max_queued_messages 0
memory_limit 0
allow_anonymous true
listener 1883
max_keepalive 0
max_connections -1
```

* Start the mosquitto server:
```
mosquitto -c conf/mq.conf
```

### Expand server
* Create the local server directory:
```
mkdir -p /tmp/work/data
```

* In order to start one xpn_server, you can use:
```
../../../src/xpn_server/xpn_server -w /tmp/work/data -m 0 -t 1 -s sck
```

### Expand client
* Create the Expand configuration file:
```
touch xpn.conf
```

* Insert the following parameters:
```
[partition]
bsize = 512k
replication_level = 0
partition_name = P1
server_url = mq_server://localhost/tmp/work/data
```

* In order to start the client, just execute:
```
XPN_CONF=conf/xpn_iot.conf XPN_LOCALITY=0 XPN_CONNECTED=1 XPN_SESSION_FILE=1 XPN_MQTT=1 XPN_QOS_MQTT=0 ./test/integrity/xpn-iot/iot-xpn  Origen-Destino  ~/user-circs/bif6
```

