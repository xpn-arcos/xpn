
# mq_server for XPN

## Pre-requisites:
* In order to use mosquitto on mq_server module, you have to install the following packages:

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

* To compile the mq_server just execute:
```
make clean
make
```

## Execution:

* In order to start one mq_server at host "localhost" and port "7555", you can use:
```
echo localhost > machinefile
./scripts/execute/mq_server.sh /tmp/xpn machinefile 7555
```

