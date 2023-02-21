# Copyright 2013-2022 Lawrence Livermore National Security, LLC and other
# Spack Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (Apache-2.0 OR MIT)


from spack.package import *


class Xpn(AutotoolsPackage):
    """The Expand Parallel File System (XPN)"""

    # Package's homepage here.
    homepage = "https://github.com/xpn-arcos/xpn/"
   #url = "https://github.com/xpn-arcos/xpn/archive/refs/tags/v2.1.0.tar.gz"
    url = "https://github.com/acaldero/xpn/archive/refs/tags/v2.1.tar.gz"

    # List of GitHub accounts to notify when the package is updated.
    maintainers = ["dcamarmas", "acaldero", "fgcarbal"]

    version("2.1", sha256="25a15aa2b2a7bb57f457f171500d3fa1b72752e02e7daa420a8f8033518023f5")

    # Variants
    variant("mpich", default=False, description="Use MPICH")

    # Dependencies
    depends_on("mxml")
    depends_on("mpich",                  when="+mpich")
    depends_on('autoconf', type='build', when='@master')
    depends_on('automake', type='build', when='@master')
    depends_on('libtool',  type='build', when='@master')


    def install(self, spec, prefix):
        ## ./configure --prefix=$INSTALL_PATH/xpn --enable-tcp_server --enable-mpi_server="$MPICC_PATH"
        configure("--prefix=" + prefix,
                  "--enable-tcp_server",
                  "--enable-mpi_server")

        ## make -j
        make()

        # make install
        make("install")

