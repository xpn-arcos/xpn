
add_subdirectory(nfi)
add_subdirectory(xpn)

file(GLOB XPN_HEADERS
	"${PROJECT_SOURCE_DIR}/include/xpn_client/*.h"
)
file(GLOB XPN_SOURCE
	"*.c"
)

set_source_files_properties(${XPN_SOURCE} PROPERTIES LANGUAGE CXX)
add_library(xpn ${XPN_HEADERS} ${XPN_SOURCE})
add_library(xpn_shared SHARED $<TARGET_OBJECTS:xpn>)
set_target_properties(xpn_shared PROPERTIES OUTPUT_NAME xpn)
target_link_libraries(xpn PRIVATE xpn_base xpn_core xpn_nfi)
set_target_properties(xpn PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(xpn PUBLIC
	"${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/include/xpn_client"
    "${PROJECT_SOURCE_DIR}/include/xpn_client/xpn/xpn_simple"
    "${PROJECT_SOURCE_DIR}/include/xpn_client/nfi"
    "${PROJECT_SOURCE_DIR}/include/xpn_client/nfi/nfi_local"
    "${PROJECT_SOURCE_DIR}/include/xpn_client/nfi/nfi_xpn_server"
	)


install(TARGETS xpn LIBRARY DESTINATION lib)
install(TARGETS xpn_shared LIBRARY DESTINATION lib)