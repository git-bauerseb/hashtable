cmake_minimum_required(VERSION 3.17)
project(hashtable C)

set(CMAKE_C_STANDARD 99)

# Library
add_library(hashtable SHARED hashtable.c hashtable.h)

# Executable
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(test ${CMAKE_CURRENT_SOURCE_DIR}/test/test.c)

target_link_libraries(test hashtable)