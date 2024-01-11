
# Changelog

## 2.2.0 - 2023-03-17
  * TCP server rebuilt from scratch
  * New system calls intercepted
  * Preload and flush data operations added
  * Minor bug fixes and improvements

## 2.1.0 - 2023-03-03
  * Spack support added
  * Code refactoring

## 2.0.0 - 2022-12-12
  * First XPN Ad-Hoc release
  * This version provides:
    * Simplifiyed build system based on build-me script.  
    * Platforms: MPICH and Intel MPI.
    * Benchmarks tested: IOR, MdTest and IO500.
    * API: POSIX (through syscall interception library) and native XPN (similar to POSIX).
    * Main features: data locality, MPI Ad-Hoc servers, thread on-demand or thread pool for MPI servers
