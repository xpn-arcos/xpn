# Expand 2.0.0
Expand: XPN Parallel File System

*Licence*: GNU GENERAL PUBLIC LICENSE Version 3</br>
*Authors*: Felix Garcia Carballeira, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Alejandro Calderon Mateos, Diego Camarmas Alonso, David Garcia Fernandez


## 1. Dependencies

C develop tools and minixml (http://www.minixml.org):

```
aptitude install autoconf automake sysutils gcc g++ make libmxml-dev
```


### Globus

* For Globus support:
```
wget http://www.globus.org/ftppub/gt5/5.2/5.2.2/installers/repo/globus-repository-5.2-stable-squeeze_0.0.3_all.deb
dpkg -i globus-repository-5.2-stable-squeeze_0.0.3_all.deb
apt-get update
```

* For just building Expand with Globus support:
```
apt-get -y --force-yes install libglobus-ftp-client-dev libglobus-xio-dev libglobus-xio-gridftp-driver-dev
```

* For running Globus:
```
#tasksel install globus-gridftp
apt-get -y --force-yes install globus-gridftp-server-progs globus-gass-copy-progs globus-common-progs globus-gsi-cert-utils-progs globus-proxy-utils
```

* Other useful Globus' stuff:
```
apt-get -y install globus-gss-assist-progs globus-simple-ca
```


### HTTP + WebDAV

* For HTTP and WebDAV support (src/nfi/nfi_http/):
```
aptitude install libtool libexpat1-dev libfuse-dev libattr1-dev lynx libkrb5-dev libssl-dev
```

To disable SSl for HTTP, then the following actions must be taken:
  - Uninstall the following packages:
    - libkrb5-dev
    - libssl-dev
  - In 'src/nfi/nfi_http/fusedav/src/fusedav.c:1541' comment the following fragment of code:
~~~~
    if (!ne_has_support(NE_FEATURE_SSL) ||
        !ne_has_support(NE_FEATURE_TS_SSL) ||
        !ne_has_support(NE_FEATURE_LFS)) {
        fprintf(stderr, "fusedav requires libneon built with SSL, SSL thread safety and LFS enabled.\n");
        goto finish;
    }
~~~~    
  - In 'src/nfi/nfi_http/fusedav/libneon/configure.gnu' replace "--enable-threadsafe-ssl=posix --with-ssl=openssl" with "--without-gssapi".
  - In 'src/nfi/nfi_http/fusedav/configure.gnu' remove  "-lgssapi_krb5 -lssl" from the "LIBS" parameter.
  - In 'configure.ac' remove "-lgssapi_krb5 -lssl" from the "LIBS" parameter in the HTTP block.
  - In 'utils/Makefile.am' uncomment the line "AM_LDFLAGS = -static".


* For write access to an HTTP server by using WebDAV:
```
aptitude install apache2
a2enmod dav
a2enmod dav_fs
```

Add the following content inside the 'Location' directive in /etc/apache2/sites-available/default:
	# WebDAV support
	# http://httpd.apache.org/docs/2.0/mod/mod_dav.html
	#DavLockDB /var/lock/apache2/DAVLock

	# Note Alias goes to our DocumentRoot.
	Alias /webdav /tmp

	# But we apply different settings
	<Location /webdav>
		DAV On
		AuthType Basic
		AuthName DAV
	</Location>
	# End of WebDAV support

* For gSOAP (http://gsoap2.sourceforge.net) support (src/nfi/nfi_gsoap/):
```
aptitude install gsoap zlib1g-dev
#aptitude install flex bison
```

* For FUSE (http://fuse.sourceforge.net) support (fuse/):
```
aptitude install libfuse-dev
```

* For test/xpn2/:
```
aptitude install libmpich1.0-dev
```

* For test/autotest/ltp-full-20080731:
```
aptitude install flex linux-libc-dev
```


## 2. Build

See INSTALL for details. Briefly:
1. Only the first time:
```
./autogen.sh
```  
2. Each time you compile:
```
./configure [<options>]
make -j
```
3. Optional:
```
doxygen doc/doxygen-XPN.cfg
make check
make install
```

For example:
```
./autogen.sh
./configure --prefix=/opt/xpn --enable-nfs3 --enable-tcpserver --enable-mpiserver=/opt/software/install-mpich/bin
make -j
doxygen doc/doxygen-XPN.cfg
make check
make install
```


## 3. Execution

- mpiServer:
```
  mpiexec -np <# of processes> -nameserver ${HYDRA_HOSTNAME} -genv LD_PRELOAD=<bypass path> ./<program path>
```


## 4. Source structure

Expand 2.0 sources has four main levels:
- xpni:
  eXPaNd Interface provides many enhanced functionality like Fault-Tolerance support, locking, etc.
- xpn:
  eXPaNd implements a Parallel File System using NFI interfaces.
- nfi:
  Network File Interface provides access to several storage protocols (local and network protocols) like NFS, FTP, etc.
- base:
  Base interface provides independent access to operating system resources. 
  Portability is provided in this layer.

