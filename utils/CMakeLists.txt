set(UTILS_LIST d2xpn lsxpn mkdirxpn removexpn rmdirxpn statfsxpn xpn2d xpncat xpncp xpncp_f xpnrm xpnwriter)

# add the executables
foreach (util ${UTILS_LIST})
	add_executable(${util} ${util}.c)
	target_link_libraries (${util} ${CMAKE_LINK_LIBRARY_FLAG})
endforeach (util)

