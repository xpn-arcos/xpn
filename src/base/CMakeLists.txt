file(GLOB XPN_BASE_HEADERS
        "${PROJECT_SOURCE_DIR}/include/base/*.h"
)

file(GLOB XPN_BASE_SOURCE
        "*.c"
)

add_compile_options("-DHAVE_STRING_H")
add_compile_options("-DHAVE_FCNTL_H")
add_compile_options("-DHAVE_UNISTD_H")
add_compile_options("-DHAVE_NETINET_IN_H")
add_compile_options("-DHAVE_NETINET_TCP_H")

add_library(xpn_base ${XPN_BASE_SOURCE} ${XPN_BASE_HEADERS})
target_include_directories(xpn_base PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)   

install(TARGETS xpn_base LIBRARY DESTINATION lib)