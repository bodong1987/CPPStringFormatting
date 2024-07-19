cmake_minimum_required(VERSION 3.10)
project(PerformanceTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include parent directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

# Recursively get source files
file(GLOB_RECURSE TEST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Sources/*.c*)
SOURCE_GROUP_BY_DIR(TEST_SOURCE_FILES)

# Define macros
add_definitions(-DUNICODE -D_UNICODE)

# Set output directories for executables and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

# Add executable
add_executable(PerformanceTests ${TEST_SOURCE_FILES})
