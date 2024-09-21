
file(GLOB_RECURSE XPN_NFI_HEADERS
	"*.hpp"
)

file(GLOB_RECURSE XPN_NFI_SOURCE
	"*.cpp"
)

add_library(xpn_nfi OBJECT ${XPN_NFI_HEADERS} ${XPN_NFI_SOURCE})

target_include_directories(xpn_nfi PRIVATE
	"${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/src/xpn_client"
)