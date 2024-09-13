add_library(xpn_bypass "xpn_bypass.c")

target_compile_options(xpn_bypass PRIVATE
    "-DNOT_TO_USE_FCNTL_H"
    "-DNOT_TO_USE_STDLIB_H"
)

target_include_directories(xpn_bypass PRIVATE 
    "${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/include/bypass"
	"${PROJECT_SOURCE_DIR}/include/xpn_client"
)

install(TARGETS xpn_bypass LIBRARY DESTINATION lib)