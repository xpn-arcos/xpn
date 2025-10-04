
# Execution Guide for Expand with XPN Proxy Deployment

* This guide provides the instructions for deploying and executing Expand across the following four different configurations:
   |                       | App XPN   | App POSIX |
   |-----------------------|-----------|-----------|
   | **Without XPN Proxy** |    1      |    2      |
   | **With    XPN Proxy** |    3      |    4      |

* They correspond to:
  1. [Expand Server + Native Expand Client Application (with ```xpn_*``` functions)](#-case-1-expand-server--native-expand-client)
  2. [Expand Server + POSIX Client Application with bypass to Expand](#-case-2-expand-server--posix-expand-client-with-bypass-to-expand)
  3. [Expand Server + Expand Proxy Server + Native Expand Client (with ```xpn_*``` functions)](#-case-3-expand-server--expand-proxy-server--native-expand-client)
  4. [Expand Server + Expand Proxy Server + POSIX Client Application with bypass to Expand](#-case-4-expand-server--expand-proxy-server--expand-client-with-bypass-to-expand)


</br>

## 🧩 Prerequisites

Before starting, ensure at least the following key requirements:

- Required binaries are installed (compiled and available).</br>
  For example, if the source code of Expand is available at ```$HOME/xpn```:
  ```bash
  cd $HOME/xpn/
  ./scripts/compile/build-me-platform.sh generic
  ```
- Environment variables like `$HOMEDIRXPN` and `$DIRTOCONF` are properly set. </br>
  For example, if the source code of Expand is available at ```$HOME/xpn```:
  ```bash
  export HOMEDIRXPN=$HOME/xpn/
  export  DIRTOCONF=$HOME/xpn/test/integrity/xpn-iot/conf/
  ```



## 🧪 Case 1: Expand Server + Native Expand Client

### 1. 🗄️ Launch The Expand Server

* For example: 
  ```bash
  mkdir -p /tmp/work/data
  $HOMEDIRXPN/src/xpn_server/xpn_server -w /tmp/work/data -s sck -t 1 -i 4 
  ```

### 2. 🚀 Run The Expand Client Application (without Proxy)

* For example: 
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


## 🧪 Case 2: Expand Server + POSIX Expand Client with bypass to Expand

### 1. 🗄️ Launch Expand Server

* For example: 
  ```bash
  mkdir -p /tmp/work/data
  $HOMEDIRXPN/src/xpn_server/xpn_server -w /tmp/work/data -s sck -t 1 -i 4
  ```

### 2. 🚀 Run Expand Client with Bypass (without Proxy)

* For example: 
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


## 🧪 Case 3: Expand Server + Expand Proxy Server + Native Expand Client

### 1. 🗄️ Launch The Expand Server

* For example: 
  ```bash
  mkdir -p /tmp/work/data
  $HOMEDIRXPN/src/xpn_server/xpn_server -w /tmp/work/data -s sck -t 1 -i 4
  ```

### 2.🔗 Launch The Expand Proxy Server

* For example: 
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

### 3. 🚀 Run The Expand Client Application (```xpn_*``` functions)

* For example: 
  ```bash
  cd $HOMEDIRXPN/test/performance/xpn-proxy
  make -j
  XPN_PROXY_ADDR=127.0.0.1:5555 ./xpn-create-dirs-test
  ```


## 🧪 Case 4: Expand Server + Expand Proxy Server + Expand Client with bypass to Expand

### 1. 🗄️ Launch Expand Server

* For example: 
  ```bash
  mkdir -p /tmp/work/data
  $HOMEDIRXPN/src/xpn_server/xpn_server -w /tmp/work/data -s sck -t 1 -i 4
  ```

### 2. 🔗 Launch Expand Proxy Server

* For example: 
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

### 3. 🚀 Run Expand Client with bypass and Proxy

* For example: 
  ```bash
  cd $HOMEDIRXPN/test/performance/xpn-proxy_posix
  make -j
  LD_PRELOAD=$HOMEDIRXPN/src/bypass/xpn_bypass_proxy.so \
    XPN_PROXY_ADDR=127.0.0.1:5555 ./posix-create-dirs-test
  ```

