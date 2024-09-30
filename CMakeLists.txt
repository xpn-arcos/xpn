cmake_minimum_required(VERSION 3.16)

project(XPN VERSION 3.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options("-D_GNU_SOURCE" "-DMPICH_SKIP_MPICXX" "-D_REENTRANT")
add_compile_options("-fPIC" "-O2" "-g" "-g3" "-ggdb" "-Wall" "-Wextra")

option(ENABLE_SCK_SERVER "Enable the sck_server module" ON)
if(ENABLE_SCK_SERVER)
    message(STATUS "ENABLE_SCK_SERVER : true")
    add_compile_options("-DENABLE_SCK_SERVER")
else()
    message(STATUS "ENABLE_SCK_SERVER : false")
endif(ENABLE_SCK_SERVER)
unset(ENABLE_SCK_SERVER CACHE)

option(ENABLE_MPI_SERVER "Enable the mpi_server module" ON)
if(ENABLE_MPI_SERVER)
    message(STATUS "ENABLE_MPI_SERVER : true")
    add_compile_options("-DENABLE_MPI_SERVER")
else()
    message(STATUS "ENABLE_MPI_SERVER : false")
endif(ENABLE_MPI_SERVER)
unset(ENABLE_MPI_SERVER CACHE)

link_libraries("pthread" "dl")

add_subdirectory(src/base_c)
add_subdirectory(src/base_cpp)
add_subdirectory(src/bypass)
add_subdirectory(src/xpn_client)
add_subdirectory(src/xpn_server)
# add_subdirectory(src/utils)


option(BUILD_TESTS "Build the test" OFF)
if(BUILD_TESTS)
    message(STATUS "BUILD_TESTS : true")
    add_subdirectory(test)
else()
    message(STATUS "BUILD_TESTS : false")
endif(BUILD_TESTS)
unset(BUILD_TESTS CACHE)


install(CODE "
    message(STATUS \"Create_symlink: from \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib to \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64\")
    execute_process(COMMAND ln -s -f -T \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64)
")
