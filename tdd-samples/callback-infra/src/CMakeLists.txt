####################################################################
# External libraries
####################################################################
find_package(Threads REQUIRED)

add_library(solution STATIC solution.cpp)
target_link_libraries(solution PUBLIC Threads::Threads)

# Some boilerplate CMakeLists.txt
# add_executable(exe-name test-file.cpp)
# target_link_libraries(exe-name PRIVATE solution)
# add_test(test-name exe-name)

####################################################################
# Add Unit Tests file names (without .cpp extension)
####################################################################
list(APPEND UNIT_TESTS
    spec
    worker_spec
    learning
)

####################################################
# Create executable for each test
####################################################
foreach(TEST ${UNIT_TESTS})
    add_executable(unit_${TEST} "${CMAKE_SOURCE_DIR}/src/test/${TEST}.cpp")
    target_include_directories(unit_${TEST} PRIVATE "${CMAKE_SOURCE_DIR}/src")
    target_link_libraries(unit_${TEST} solution)
    target_link_libraries(unit_${TEST} trompeloeil)
    add_test(unit_${TEST} unit_${TEST})
endforeach()
