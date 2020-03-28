# Integration tests use private include files.
include_directories("${CMAKE_SOURCE_DIR}/src")

####################################################
# Create executable for each test
####################################################
add_executable(int_main main.cpp)
target_link_libraries(int_main Threads::Threads)
target_link_libraries(int_main solution)
add_test(int_main int_main)
