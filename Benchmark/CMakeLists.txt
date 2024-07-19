cmake_minimum_required(VERSION 3.10)
project(Benchmark)

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

# Add Celero subdirectory
add_subdirectory(External/Celero)
include_directories(External/Celero/include)

# Set project output directory
set(OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)

# Set output directories for executables and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

# Add executable
add_executable(Benchmark ${TEST_SOURCE_FILES})

# Set Celero library output directory
set(CELERO_OUTPUT_DIR ${CMAKE_BINARY_DIR}/Benchmark/External/Celero/$<CONFIG>)

# Add Celero output directory to link search paths
target_link_directories(Benchmark PRIVATE ${CELERO_OUTPUT_DIR})

# Select library name based on build type
set(CELERO_LIB "$<IF:$<CONFIG:Debug>,celero-d,celero>")

# Link Celero library
target_link_libraries(Benchmark ${CELERO_LIB})

