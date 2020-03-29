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
