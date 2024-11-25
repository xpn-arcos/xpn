
add_executable(print_blocks print_blocks.cpp)
target_link_libraries(print_blocks xpn)

install(TARGETS print_blocks RUNTIME DESTINATION test/integrity/xpn_metadata)