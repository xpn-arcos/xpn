
file(GLOB_RECURSE XPN_SIMPLE_HEADERS
    "*.hpp"
)

file(GLOB_RECURSE XPN_SIMPLE_SOURCE
    "*.cpp"
)

add_library(xpn_core OBJECT ${XPN_SIMPLE_SOURCE} ${XPN_SIMPLE_HEADERS})

target_include_directories(xpn_core PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/src/xpn_client"
)