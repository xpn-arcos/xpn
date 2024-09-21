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

set_source_files_properties(${XPN_BASE_SOURCE} PROPERTIES LANGUAGE C)
add_library(xpn_base_c OBJECT ${XPN_BASE_SOURCE} ${XPN_BASE_HEADERS})
set_target_properties(xpn_base_c PROPERTIES LINKER_LANGUAGE C)
target_include_directories(xpn_base_c PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)   

# install(TARGETS xpn_base_c LIBRARY DESTINATION lib)