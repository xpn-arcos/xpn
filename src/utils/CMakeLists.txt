set(UTILS_LIST 
	xpn_expand
	xpn_flush
	xpn_preload
	xpn_rebuild
	xpn_rebuild_active_reader
	xpn_rebuild_active_writer
	xpn_shrink
	xpn_get_block_locality
	xpn_tree
)

# add the executables
foreach (util ${UTILS_LIST})
	set_source_files_properties(${util} PROPERTIES LANGUAGE CXX)
	add_executable(${util} ${util}.c)
	target_link_libraries(${util} PRIVATE xpn_base xpn xpn_core xpn_nfi)
	install(TARGETS ${util} RUNTIME DESTINATION bin)
endforeach (util)

