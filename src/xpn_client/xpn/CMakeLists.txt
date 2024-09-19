file(GLOB_RECURSE XPN_SIMPLE_HEADERS
    # "${PROJECT_SOURCE_DIR}/include/xpn_client/xpn/**.h"
    "*.hpp"
)

file(GLOB_RECURSE XPN_SIMPLE_SOURCE
    "*.cpp"
)

add_library(xpn_core ${XPN_SIMPLE_SOURCE} ${XPN_SIMPLE_HEADERS})
target_compile_options(xpn_core PRIVATE 
    "-DHAVE_STRING_H"
    "-DHAVE_FCNTL_H"
    "-DHAVE_UNISTD_H"
    "-DHAVE_NETINET_IN_H"
    "-DHAVE_NETINET_TCP_H"
)
target_include_directories(xpn_core PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/src/xpn_client"
    "${PROJECT_SOURCE_DIR}/src/xpn_client/xpn"
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/xpn_client"
    "${PROJECT_SOURCE_DIR}/include/xpn_client/nfi"
    "${PROJECT_SOURCE_DIR}/include/xpn_client/nfi/nfi_local"
    "${PROJECT_SOURCE_DIR}/include/xpn_client/nfi/nfi_xpn_server"
    "${PROJECT_SOURCE_DIR}/include/xpn_client/xpn/xpn_simple"
)