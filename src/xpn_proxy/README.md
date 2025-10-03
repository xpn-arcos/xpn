# XPN Proxy Deployment and Execution Guide

This guide provides detailed instructions for deploying and executing the XPN Proxy system across four different configurations. It covers four scenarios:

1. Expand Server + Expand Client Application  
2. Expand Server + Expand Proxy Server + Expand Client (xpn_ functions)  
3. Expand Server + Client Application with Bypass Expand  
4. Expand Server + Expand Proxy Server + Client Application with Bypass Expand 

---

## 🧩 Prerequisites

Before starting, ensure the following:

- Environment variables like `$HOMEDIRXPN` and `$DIRTOCONF` are properly set.
- Required binaries are compiled and available.
- You have permission to create directories and execute binaries.
- Create a working directory for the server before each launch:

```bash
mkdir -p /tmp/work/data
```

---

## 🧪 Case 1: Expand Server + Expand Client Application

### Architecture Diagram

graph TD
    C[Expand Client Application] --> S[Expand Server]

### 🖥️ Launch Expand Server

```bash
mkdir -p /tmp/work/data
cd $HOMEDIRXPN
XPN_SCK_IPV=4 ./src/xpn_server/xpn_server -w /tmp/work/data -s sck -t 1 -i 4
```

### 🚀 Run Expand Client Application (No Proxy)

```bash
cd $HOMEDIRXPN/test/performance/xpn
make -j
XPN_CONF=$DIRTOCONF/xpn_sck.conf \
XPN_MQTT=0 \
XPN_LOCALITY=0 \
XPN_CONNECTED=1 \
XPN_SESSION_FILE=1 \
XPN_SCK_IPV=4 \
./create-dirs-test
```

## 🧪 Case 2: Expand Server + Expand Proxy Server + Expand Client (xpn_ functions)

### Architecture Diagram

graph TD
    C[Expand Client (xpn_ functions)] --> P[XPN Proxy Server]
    P --> S[Expand Server]

### 🖥️ Launch Expand Server

```bash
mkdir -p /tmp/work/data
cd $HOMEDIRXPN
XPN_SCK_IPV=4 ./src/xpn_server/xpn_server -w /tmp/work/data -s sck -t 1 -i 4
```

### 🔗 Launch Expand Proxy Server

```bash
cd $HOMEDIRXPN/src/xpn_proxy
XPN_CONF=$DIRTOCONF/xpn_sck.conf \
XPN_MQTT=0 \
XPN_LOCALITY=0 \
XPN_CONNECTED=1 \
XPN_SESSION_FILE=1 \
XPN_PROXY_IPV=4 \
XPN_PROXY_PORT=5555 \
XPN_SCK_IPV=4 \
./xpn_proxy_server
```

### 🚀 Run Expand Client Application (xpn_ functions)

```bash
cd $HOMEDIRXPN/test/performance/xpn-proxy
make
XPN_PROXY_ADDR=127.0.0.1:5555 ./xpn-create-dirs-test
```

## 🧪 Case 3: Expand Server + Expand Client + Bypass Expand

### Architecture Diagram

graph TD
    C[Client with LD_PRELOAD (Bypass)] --> S[Expand Server]


### 🖥️ Launch Expand Server

```bash
mkdir -p /tmp/work/data
cd $HOMEDIRXPN
XPN_SCK_IPV=4 ./src/xpn_server/xpn_server -w /tmp/work/data -s sck -t 1 -i 4
```

### 🚀 Run Expand Client with Bypass (No Proxy)

```bash
cd $HOMEDIRXPN/test/performance/xpn-proxy_posix
make -j
LD_PRELOAD=$HOMEDIRXPN/src/bypass/xpn_bypass.so \
XPN_CONF=$DIRTOCONF/xpn_sck.conf \
XPN_MQTT=0 \
XPN_LOCALITY=0 \
XPN_CONNECTED=1 \
XPN_SESSION_FILE=1 \
XPN_SCK_IPV=4 \
./posix-create-dirs-test
```

## 🧪 Case 4: Expand Server + Expand Proxy Server + Expand Client + Bypass Expand

### Architecture Diagram

graph TD
    C[Client with LD_PRELOAD (Bypass)] --> P[XPN Proxy Server]
    P --> S[Expand Server]


### 🖥️ Launch Expand Server

```bash
mkdir -p /tmp/work/data
cd $HOMEDIRXPN
XPN_SCK_IPV=4 ./src/xpn_server/xpn_server -w /tmp/work/data -s sck -t 1 -i 4
```

### 🔗 Launch Expand Proxy Server

```bash
cd $HOMEDIRXPN/src/xpn_proxy
XPN_CONF=$DIRTOCONF/xpn_sck.conf \
XPN_MQTT=0 \
XPN_LOCALITY=0 \
XPN_CONNECTED=1 \
XPN_SESSION_FILE=1 \
XPN_PROXY_IPV=4 \
XPN_PROXY_PORT=5555 \
XPN_SCK_IPV=4 \
./xpn_proxy_server
```

### 🚀 Run Expand Client with Bypass and Proxy

```bash
cd $HOMEDIRXPN/test/performance/xpn-proxy_posix
make -j
LD_PRELOAD=$HOMEDIRXPN/src/bypass/xpn_bypass_proxy.so \
XPN_PROXY_ADDR=127.0.0.1:5555 \
./posix-create-dirs-test
```