
set_source_files_properties("xpn_bypass.c" PROPERTIES LANGUAGE C)

add_library(xpn_bypass SHARED "xpn_bypass.c")

set_target_properties(xpn_bypass PROPERTIES LINKER_LANGUAGE C)
set_target_properties(xpn_bypass PROPERTIES PREFIX "")

target_link_libraries(xpn_bypass PRIVATE xpn pthread dl )

target_include_directories(xpn_bypass PRIVATE 
    "${PROJECT_SOURCE_DIR}/src"
)

install(TARGETS xpn_bypass LIBRARY DESTINATION lib)