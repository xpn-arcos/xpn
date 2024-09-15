file(GLOB XPN_NFI_HEADERS
	"${PROJECT_SOURCE_DIR}/include/xpn_client/nfi/*.h"
	"${PROJECT_SOURCE_DIR}/include/xpn_client/nfi/nfi_local/*.h"
	"${PROJECT_SOURCE_DIR}/include/xpn_client/nfi/nfi_xpn_server/*.h"
	"${PROJECT_SOURCE_DIR}/include/xpn_client/nfi/nfi_sck_server/*.h"
	"${PROJECT_SOURCE_DIR}/include/xpn_client/nfi/nfi_mpi_server/*.h"
)

file(GLOB XPN_NFI_SOURCE
	"common/*.c"
	"nfi_local/*.c"
	"nfi_xpn_server/*.c"
	"nfi_sck_server/*.c"
	"nfi_mpi_server/*.c"
)
set_source_files_properties(${XPN_NFI_SOURCE} PROPERTIES LANGUAGE CXX)
add_library(xpn_nfi ${XPN_NFI_HEADERS} ${XPN_NFI_SOURCE})
set_target_properties(xpn_nfi PROPERTIES LINKER_LANGUAGE CXX)

target_compile_options(xpn_nfi PRIVATE 
"-DHAVE_STRING_H"
"-DHAVE_FCNTL_H"
"-DHAVE_UNISTD_H"
"-DHAVE_NETINET_IN_H"
"-DHAVE_NETINET_TCP_H"
)

target_include_directories(xpn_nfi PRIVATE
	"${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/xpn_client"
    "${PROJECT_SOURCE_DIR}/include/xpn_client/xpn/xpn_simple"
    "${PROJECT_SOURCE_DIR}/include/xpn_client/nfi"
    "${PROJECT_SOURCE_DIR}/include/xpn_client/nfi/nfi_local"
    "${PROJECT_SOURCE_DIR}/include/xpn_client/nfi/nfi_xpn_server"
    "${PROJECT_SOURCE_DIR}/include/xpn_client/nfi/nfi_sck_server"
    "${PROJECT_SOURCE_DIR}/include/xpn_client/nfi/nfi_mpi_server"
)