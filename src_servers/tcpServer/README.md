
 tcpServer
 ---------

## Compile:
   * To compile tcpServer:
```
make clean
make
```

## Execution:

   * To start one tcpServer:
```
export XPN_CONF=${HOME}/conf/xpn.conf.xml
export XPN_PROFILE=${HOME}/conf/xpn.profile.xml
export MYSERVER_FILE=${HOME}/conf/xpn.dns
export MYSERVER_HOME=${HOME}/expand/src_servers/tcpServer

# argument: node id.
I=$1

${MYSERVER_HOME}/tcpServer.exe -n node$I -p 9999 -f ${MYSERVER_FILE}
```

