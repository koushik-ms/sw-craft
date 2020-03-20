project(callback-infra-int)

cmake_minimum_required(VERSION 2.8)

enable_testing()

####################################################
# Set the C++ Standard to be used (Default to C++11)
####################################################
set(CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

####################################################################
# Set Compiler flags
####################################################################
IF (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic -std=c++${CXX_STANDARD}")
ELSEIF (MSVC)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
ENDIF()

include_directories("${CMAKE_SOURCE_DIR}/../include")

####################################################
# Include relevant subdirectories
####################################################
# at some point...

####################################################
# Add Integration Tests file names (without .cpp extension)
####################################################
list(APPEND INT_TESTS
    main
)

####################################################
# Create executable for each test
####################################################
foreach(TEST ${INT_TESTS})
    add_executable(int_${TEST} "${CMAKE_SOURCE_DIR}/${TEST}.cpp")
    # At some point this will depend on and link with a library that has the solution (production code)
    add_test(int_${TEST} int_${TEST})
endforeach()
