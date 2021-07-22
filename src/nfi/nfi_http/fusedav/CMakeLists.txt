# http://www.kitware.com/products/html/BuildingExternalProjectsWithCMake2.8.html
# http://www.cmake.org/cmake/help/cmake-2-8-docs.html#section_StandardCMakeModules

ExternalProject_Add (	libneon-expand
			DOWNLOAD_COMMAND ""
			UPDATE_COMMAND ./autogen.sh
			SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libneon"
			CONFIGURE_COMMAND ./configure.gnu
			BUILD_COMMAND make
			BUILD_IN_SOURCE 1
			INSTALL_COMMAND ""
)

ExternalProject_Add (	fusedav-expand
			DEPENDS libneon-expand
			DOWNLOAD_COMMAND ""
			UPDATE_COMMAND NOCONFIGURE=1 ./bootstrap.sh
			SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
			CONFIGURE_COMMAND ./configure.gnu
			BUILD_COMMAND make
			BUILD_IN_SOURCE 1
			INSTALL_COMMAND ""
)
