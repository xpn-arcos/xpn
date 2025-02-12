
# FUSE for XPN (XPN-FUSE)

## Pre-requisites:
* In order to use FUSE, you have to install the following packages:

```
sudo apt-get install fuse libfuse-dev
```

## Compile:
* To compile please use:

```
make
```

* Internal steps are close to:
```
gcc -Wall -Wextra `pkg-config fuse --cflags --libs` fuse-expand.c -o fuse-expand
gcc -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=22 -c -o fuse-expand.o fuse-expand.c
gcc -o fuse-expand fuse-expand.o /usr/lib/libfuse.so.2.4.2 -lpthread -Wl,--rpath -Wl,/usr/lib
```

## Execute:
* To execute XPN-FUSE please use:

```
./fuse-expand /storage/fuse -d -s -o xpnpart=/<partition_name> -o big_writes -o allow_other -o no_remote_lock -o intr -o uid=1001 -o gid=100
```

