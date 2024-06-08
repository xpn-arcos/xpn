
# Copyright 2013-2022 Lawrence Livermore National Security, LLC and other
# Spack Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (Apache-2.0 OR MIT)


from spack.package import *


class Xpn(AutotoolsPackage):
    """The Expand Parallel File System (XPN)"""

    # Package's homepage here.
    homepage = "https://github.com/xpn-arcos/xpn/"
    git = "https://github.com/xpn-arcos/xpn.git"
    url = "https://github.com/xpn-arcos/xpn/archive/refs/tags/v3.0.0.tar.gz"

    # List of GitHub accounts to notify when the package is updated.
    maintainers = ["xpn-arcos", "dcamarmas", "acaldero", "fgcarbal"]

    # versions
    version('latest', branch='master', submodules=True)
    version("3.0.0",  sha256="867ad73bae2fa4dc223e5acc1d27c6bf3ed2883e")

    # Variants
    variant("mpich",     default=False, description="Use MPICH")
    variant('mosquitto', default=False, description='Enables support for mosquitto.')

    # Dependencies
    depends_on("mpich",                  when="+mpich")
    depends_on('autoconf', type='build', when='@master')
    depends_on('automake', type='build', when='@master')
    depends_on('libtool',  type='build', when='@master')

    def install(self, spec, prefix):
        configure("--prefix=" + prefix,
                  "--enable-sck_server",
                  "--enable-mpi_server=" + spec['mpich'].prefix + "/bin/mpicc")
        make()
        make("install")

#   def setup_run_environment(self, env):
#       env.set('LD_PRELOAD', join_path(self.prefix.lib, 'xpn_bypass.so'))

