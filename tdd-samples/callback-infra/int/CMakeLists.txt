# Integration tests use private include files.
include_directories("${CMAKE_SOURCE_DIR}/src")

####################################################
# Create executable for each test
####################################################
add_executable(int_main main.cpp)
target_link_libraries(int_main solution)
target_link_libraries(int_main trompeloeil)
add_test(int_main int_main)
