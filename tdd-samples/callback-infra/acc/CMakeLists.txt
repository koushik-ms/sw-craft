
####################################################
# Add Acceptance Tests file names (without .cpp extension)
####################################################
list(APPEND ACC_TESTS
    aTest
)

####################################################
# Create executable for each test
####################################################
foreach(TEST ${ACC_TESTS})
    add_executable(acc_${TEST} "${CMAKE_SOURCE_DIR}/acc/${TEST}.cpp")
    target_link_libraries(acc_${TEST} Threads::Threads)
    target_link_libraries(acc_${TEST} solution)
    add_test(acc_${TEST} acc_${TEST})
endforeach()
