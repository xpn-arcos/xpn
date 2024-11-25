
set(TEST_LIST 
	open-read-close
	open-write-close
)

# add the executables
foreach (test ${TEST_LIST})
    add_executable(bypass_c_${test} ${test}.c)
	set_target_properties(bypass_c_${test} PROPERTIES LINKER_LANGUAGE C)
    target_link_libraries(bypass_c_${test} xpn)
    set_target_properties(bypass_c_${test} PROPERTIES OUTPUT_NAME ${test}) 
    install(TARGETS bypass_c_${test} RUNTIME DESTINATION test/integrity/bypass_c)
endforeach (test)
