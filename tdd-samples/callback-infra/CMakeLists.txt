project(callback-infra)

cmake_minimum_required(VERSION 2.8)

enable_testing()

####################################################
# Set the C++ Standard to be used (Default to C++11)
####################################################
set(CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED ON)

####################################################################
# Set Compiler flags
####################################################################
IF (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++${CXX_STANDARD}")
ELSEIF (MSVC)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
ENDIF()

include_directories("${CMAKE_SOURCE_DIR}/include")

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
    # At some point this will depend on and link with a library that has the solution (production code)
    add_test(acc_${TEST} acc_${TEST})
endforeach()
