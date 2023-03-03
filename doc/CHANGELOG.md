
# Changelog

## 2.1.0 - 2023-03-03
  * spack support added
  * Code refactoring

## 2.0.0 - 2022-12-12
  * First XPN Ad-Hoc release
  * This version provides:
    * Simplifiyed build system based on build-me script.  
    * Platforms: MPICH and Intel MPI.
    * Benchmarks tested: IOR, MdTest and IO500.
    * API: POSIX (through syscall interception library) and native XPN (similar to POSIX).
    * Main features: data locality, MPI Ad-Hoc servers, thread on-demand or thread pool for MPI servers
