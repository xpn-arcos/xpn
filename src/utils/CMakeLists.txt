set(UTILS_LIST cp-local2xpn xpn-ls xpn-mkdir removexpn xpn-rmdir xpn-statfs cp-xpn2local xpn-cat xpncp xpnwriter)

# add the executables
foreach (util ${UTILS_LIST})
	add_executable(${util} ${util}.c)
	target_link_libraries (${util} ${CMAKE_LINK_LIBRARY_FLAG})
endforeach (util)

