
set(UTILS_LIST_CPP
	xpn_flush
	xpn_preload
	xpn_expand
	xpn_shrink
	# xpn_rebuild_active_reader # TODO: fix with metadata
	xpn_server_stats
)

# add the executables cpp
foreach (util ${UTILS_LIST_CPP})
	add_executable(${util} ${util}.cpp)
	target_link_libraries(${util} PRIVATE xpn)
	install(TARGETS ${util} RUNTIME DESTINATION bin)
endforeach (util)


set(UTILS_LIST_C
	xpn_get_block_locality
	xpn_tree
)

# add the executables c
foreach (util ${UTILS_LIST_C})
	add_executable(${util} ${util}.c)
	target_link_libraries(${util} PRIVATE xpn)
	install(TARGETS ${util} RUNTIME DESTINATION bin)
endforeach (util)