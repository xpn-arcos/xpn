
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
    url = "https://github.com/xpn-arcos/xpn/archive/refs/tags/v2.1.0.tar.gz"

    # List of GitHub accounts to notify when the package is updated.
    maintainers = ["xpn-arcos", "dcamarmas", "acaldero", "fgcarbal"]

    # versions
    version('latest', branch='master', submodules=True)
    version("2.1.0", sha256="7063ae410b28abe100481cda15f80d996799f8f4b487afec9028e6774cf91f7c")

    # Variants
    variant("mpich",     default=False, description="Use MPICH")
    variant('mosquitto', default=False, description='Enables support for mosquitto.')

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

#   def setup_run_environment(self, env):
#       env.set('LD_PRELOAD', join_path(self.prefix.lib, 'xpn_bypass.so'))

