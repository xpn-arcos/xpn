cmake_minimum_required(VERSION 3.16)

project(XPN VERSION 3.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options("-D_GNU_SOURCE" "-DMPICH_SKIP_MPICXX" "-D_REENTRANT")
add_compile_options("-fPIC" "-O2" "-g" "-g3" "-ggdb" "-Wall" "-Wextra" "-fpermissive")

option(ENABLE_SCK_SERVER "Enable the sck_server module" ON)
if(ENABLE_SCK_SERVER)
    add_compile_options("-DENABLE_SCK_SERVER")
endif(ENABLE_SCK_SERVER)
unset(ENABLE_SCK_SERVER CACHE)

option(ENABLE_MPI_SERVER "Enable the mpi_server module" ON)
if(ENABLE_MPI_SERVER)
    add_compile_options("-DENABLE_MPI_SERVER")
endif(ENABLE_MPI_SERVER)
unset(ENABLE_MPI_SERVER CACHE)



add_subdirectory(src/base)
add_subdirectory(src/xpn_client)
add_subdirectory(src/xpn_server)
add_subdirectory(src/bypass)
add_subdirectory(src/utils)


install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h")

install(CODE "execute_process(COMMAND ln -s -f \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64)")
