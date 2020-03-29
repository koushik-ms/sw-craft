project(callback-infra)
# project(callback-infra C CXX)

cmake_minimum_required(VERSION 2.8)

enable_testing()

####################################################
# Set the C++ Standard to be used (Default to C++17)
####################################################
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

####################################################################
# External libraries
####################################################################

####################################################################
# Set Compiler flags
####################################################################
IF (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic -std=c++${CXX_STANDARD}")
ELSEIF (MSVC)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
ENDIF()

include_directories("${CMAKE_SOURCE_DIR}/include")

####################################################
# Include relevant subdirectories
####################################################
add_subdirectory(src)
add_subdirectory(int)
add_subdirectory(acc)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
