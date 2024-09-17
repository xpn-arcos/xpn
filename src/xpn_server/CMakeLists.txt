file(GLOB XPN_SERVER_HEADERS 
    "${PROJECT_SOURCE_DIR}/include/xpn_server/*.h"
    "${PROJECT_SOURCE_DIR}/include/xpn_server/*.hpp"
    "${PROJECT_SOURCE_DIR}/include/xpn_server/mpi_server/*.h"
    "${PROJECT_SOURCE_DIR}/include/xpn_server/mpi_server/*.hpp"
    "${PROJECT_SOURCE_DIR}/include/xpn_server/sck_server/*.h"
)
file(GLOB XPN_SERVER_SOURCE 
"*.c"
"*.cpp"
"mpi_server/*.c"
"mpi_server/*.cpp"
"sck_server/*.c"
)

set_source_files_properties(${XPN_SERVER_SOURCE} PROPERTIES LANGUAGE CXX)
add_executable(xpn_server ${XPN_SERVER_SOURCE})

set_target_properties(xpn_server PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(xpn_server PRIVATE xpn_base xpn_base_cpp)

target_compile_options(xpn_server PRIVATE 
"-DHAVE_STRING_H"
"-DHAVE_FCNTL_H"
"-DHAVE_UNISTD_H"
"-DHAVE_NETINET_IN_H"
"-DHAVE_NETINET_TCP_H"
)

target_include_directories(xpn_server PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/include/xpn_server"
	"${PROJECT_SOURCE_DIR}/include/xpn_server/mpi_server"
	"${PROJECT_SOURCE_DIR}/include/xpn_server/sck_server"
	"${PROJECT_SOURCE_DIR}/include/xpn_client/xpn/xpn_simple"
	"${PROJECT_SOURCE_DIR}/include/xpn_client/nfi"
)

add_custom_command(
    OUTPUT xpn_stop_server
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            xpn_server
            xpn_stop_server
    DEPENDS xpn_server
    COMMENT "Create_symlink xpn_server -> xpn_stop_server"
)

add_custom_command(
    OUTPUT xpn_terminate_server
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            xpn_server
            xpn_terminate_server
    DEPENDS xpn_server
    COMMENT "Create_symlink xpn_server -> xpn_terminate_server"
)

add_custom_command(
    OUTPUT xpn_server_spawn
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            xpn_server
            xpn_server_spawn
    DEPENDS xpn_server
    COMMENT "Create_symlink xpn_server -> xpn_server_spawn"
)

add_custom_target(create_symlinks ALL
    DEPENDS
        xpn_stop_server
        xpn_terminate_server
        xpn_server_spawn
)

install(TARGETS xpn_server RUNTIME DESTINATION bin)

install(CODE "
    message(STATUS \"Create_symlink: from \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_server to \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_stop_server\")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_server \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_stop_server)
")
install(CODE "
    message(STATUS \"Create_symlink: from \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_server to \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_terminate_server\")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_server \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_terminate_server)
")
install(CODE "
    message(STATUS \"Create_symlink: from \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_server to \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_server_spawn\")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_server \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_server_spawn)
")
