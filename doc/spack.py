# Copyright 2013-2022 Lawrence Livermore National Security, LLC and other
# Spack Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (Apache-2.0 OR MIT)

# ----------------------------------------------------------------------------
# You can save this file and test your package like this:
#     spack install xpn
#
# You can edit this file again by typing:
#     spack edit xpn
#
# See the Spack documentation for more information on packaging.
# ----------------------------------------------------------------------------

from spack.package import *


class Xpn(AutotoolsPackage):
    """XPN Parallel File System"""

    # Package's homepage here.
    homepage = "https://github.com/xpn-arcos/xpn/"
   #url = "https://github.com/xpn-arcos/xpn/archive/refs/tags/v2.0.0.tar.gz"
    url = "https://github.com/acaldero/xpn/archive/refs/tags/v2.1.tar.gz"

    # List of GitHub accounts to notify when the package is updated.
    maintainers = ["dcamarmas", "acaldero"]

   #version("2.0.0", sha256="affa2b72d2671c90aefb8cff47ff884affdd471cab0be7124c1ac99728c6afb6")
    version("2.1",   sha256="e1a732c7318a94b9b53d984e8d7bddb530c1f70565a94e111f18b3a21d9a4f21")

    # Variants
    variant("mpich", default=False, description="Use MPICH")

    # Dependencies
    depends_on("mxml")
    depends_on("mpich",    when="+mpich")
    depends_on('autoconf', type='build', when='@master')
    depends_on('automake', type='build', when='@master')
    depends_on('libtool',  type='build', when='@master')

    #def configure_args(self):
    #    args = []
    #    return args

    def install(self, spec, prefix):
        # ACLOCAL_FLAGS="-I /usr/share/aclocal/" autoreconf -v -i -s -W all

        # ./configure --prefix=$INSTALL_PATH/xpn --enable-tcp_server --enable-mpi_server="$MPICC_PATH"
        configure("--prefix=" + prefix,
                  "--enable-tcp_server",
                  "--enable-mpi_server="  + spec['mpicc_path'].prefix)

        # make clean
        # make -j 8
        make()

        # make install
        make("install")

