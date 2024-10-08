
file(GLOB_RECURSE XPN_SERVER_HEADERS 
    "*.hpp"
)

file(GLOB_RECURSE XPN_SERVER_SOURCE 
    "*.cpp"
)

add_executable(xpn_server ${XPN_SERVER_SOURCE})

target_link_libraries(xpn_server PRIVATE xpn_base_c xpn_base_cpp)

target_include_directories(xpn_server PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/src/xpn_client"
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

add_custom_command(
    OUTPUT xpn_server_stats
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            xpn_server
            xpn_server_stats
    DEPENDS xpn_server
    COMMENT "Create_symlink xpn_server -> xpn_server_stats"
)

add_custom_target(create_symlinks ALL
    DEPENDS
        xpn_stop_server
        xpn_terminate_server
        xpn_server_spawn
        xpn_server_stats
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
install(CODE "
    message(STATUS \"Create_symlink: from \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_server to \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_server_stats\")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_server \$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/xpn_server_stats)
")
