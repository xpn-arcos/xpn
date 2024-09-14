set_source_files_properties("xpn_bypass.c" PROPERTIES LANGUAGE C)
add_library(xpn_bypass SHARED "xpn_bypass.c")
set_target_properties(xpn_bypass PROPERTIES LINKER_LANGUAGE C)
set_target_properties(xpn_bypass PROPERTIES PREFIX "")

target_link_libraries(xpn_bypass PRIVATE -Wl,--whole-archive xpn_base -Wl,--no-whole-archive xpn pthread dl )

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