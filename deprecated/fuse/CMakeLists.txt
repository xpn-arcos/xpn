# add the executable
#set (LINK_LIBRARIES "${LINK_LIBRARIES} -lfuse -lrt -ldl")
#set (CMAKE_LINK_LIBRARY_FLAG "${CMAKE_LINK_LIBRARY_FLAG} -lfuse -lrt -ldl")
add_executable(fuse-expand fuse-expand.c)
#target_link_libraries (fuse-expand -lfuse -lrt -ldl)
target_link_libraries (fuse-expand "${CMAKE_LINK_LIBRARY_FLAG} -lfuse -lrt -ldl")
#target_link_libraries (fuse-expand "${CMAKE_EXE_LINKER_FLAGS} -lfuse -lrt -ldl")
#target_link_libraries (fuse-expand "${LDFLAGS}" "${LDLIBS}" "-lfuse -lrt -ldl")

