file(GLOB XPN_BASE_C_HEADERS
        "*.h"
)

file(GLOB XPN_BASE_C_SOURCE
        "*.c"
)

add_library(xpn_base_c OBJECT ${XPN_BASE_C_SOURCE} ${XPN_BASE_C_HEADERS})
target_include_directories(xpn_base_c PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)