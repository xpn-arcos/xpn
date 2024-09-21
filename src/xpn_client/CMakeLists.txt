
add_subdirectory(nfi)
add_subdirectory(xpn)

file(GLOB XPN_HEADERS
	"*.h"
)
file(GLOB XPN_SOURCE
	"*.cpp"
)

add_library(xpn ${XPN_HEADERS} ${XPN_SOURCE})
target_link_libraries(xpn xpn_base_c xpn_base_cpp xpn_core xpn_nfi)
add_library(xpn_shared SHARED $<TARGET_OBJECTS:xpn>)
set_target_properties(xpn_shared PROPERTIES OUTPUT_NAME xpn)
target_link_libraries(xpn_shared PRIVATE xpn_base_c xpn_base_cpp xpn_core xpn_nfi)

target_include_directories(xpn PUBLIC
	"${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/src/xpn_client"
	)


install(TARGETS xpn LIBRARY DESTINATION lib)
install(TARGETS xpn_shared LIBRARY DESTINATION lib)
install(FILES "xpn.h" DESTINATION include)