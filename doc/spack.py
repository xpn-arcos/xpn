# Copyright 2013-2022 Lawrence Livermore National Security, LLC and other
# Spack Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (Apache-2.0 OR MIT)


from spack.package import *


class Xpn(AutotoolsPackage):
    """The Expand Parallel File System (XPN)"""

    # Package's homepage here.
    homepage = "https://github.com/xpn-arcos/xpn/"
    url = "https://github.com/dcamarmas/xpn/archive/refs/tags/v2.1.0.tar.gz"

    # List of GitHub accounts to notify when the package is updated.
    maintainers = ["xpn-arcos", "dcamarmas", "acaldero", "fgcarbal"]

    version("2.1.0", sha256="a44de0e8b75de694c959cbc84d1593a07aa7cd343f7172cc39ddfd6526ed5e1d")

    # Variants
    variant("mpich", default=False, description="Use MPICH")

    # Dependencies
    depends_on("mxml")
    depends_on("mpich",                  when="+mpich")
    depends_on('autoconf', type='build', when='@master')
    depends_on('automake', type='build', when='@master')
    depends_on('libtool',  type='build', when='@master')

    def install(self, spec, prefix):
        configure("--prefix=" + prefix,
                  "--enable-tcp_server",
                  "--enable-mpi_server=" + spec['mpich'].prefix + "/bin/mpicc")
        make()
        make("install")

