
set(TEST_LIST 
	server-write-read
	write-server-read
    rnd-write-read-cmp
)

# add the executables
foreach (test ${TEST_LIST})
    add_executable(${test} ${test}.c)
	set_target_properties(${test} PROPERTIES LINKER_LANGUAGE C)
    target_link_libraries(${test} xpn)
    install(TARGETS ${test} RUNTIME DESTINATION test/performance/xpn-fault-tolerant)
endforeach (test)
