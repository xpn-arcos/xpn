################################################################################
# Copyright 2018-2023, Barcelona Supercomputing Center (BSC), Spain            #
# Copyright 2015-2023, Johannes Gutenberg Universitaet Mainz, Germany          #
#                                                                              #
# This software was partially supported by the                                 #
# EC H2020 funded project NEXTGenIO (Project ID: 671951, www.nextgenio.eu).    #
#                                                                              #
# This software was partially supported by the                                 #
# ADA-FS project under the SPPEXA project funded by the DFG.                   #
#                                                                              #
# This file is part of GekkoFS.                                                #
#                                                                              #
# GekkoFS is free software: you can redistribute it and/or modify              #
# it under the terms of the GNU General Public License as published by         #
# the Free Software Foundation, either version 3 of the License, or            #
# (at your option) any later version.                                          #
#                                                                              #
# GekkoFS is distributed in the hope that it will be useful,                   #
# but WITHOUT ANY WARRANTY; without even the implied warranty of               #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                #
# GNU General Public License for more details.                                 #
#                                                                              #
# You should have received a copy of the GNU General Public License            #
# along with GekkoFS.  If not, see <https://www.gnu.org/licenses/>.            #
#                                                                              #
# SPDX-License-Identifier: GPL-3.0-or-later                                    #
################################################################################


find_path(Expand_INCLUDE_DIR
  NAMES xpn.h
  PREFIX xpn
)

find_library(Expand_LIBRARY
  NAMES libxpn.so
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	Expand
	DEFAULT_MSG
	Expand_INCLUDE_DIR
	Expand_LIBRARY
)

if(Expand_FOUND)
  set(Expand_LIBRARIES ${Expand_LIBRARY})
  set(Expand_INCLUDE_DIRS ${Expand_INCLUDE_DIR})


  if(NOT TARGET Expand::Expand)
	  add_library(Expand::Expand UNKNOWN IMPORTED)
	  set_target_properties(Expand::Expand PROPERTIES
		IMPORTED_LOCATION "${Expand_LIBRARY}"
		INTERFACE_INCLUDE_DIRECTORIES "${Expand_INCLUDE_DIR}"
	  )
	endif()
endif()


mark_as_advanced(
	Expand_INCLUDE_DIR
	Expand_LIBRARY
)
