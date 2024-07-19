cmake_minimum_required(VERSION 3.22)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/CMakeCommon.cmake)

project(CPPStringFormatting)

add_subdirectory(UnitTests)
add_subdirectory(Benchmark)
add_subdirectory(PerformanceTests)
