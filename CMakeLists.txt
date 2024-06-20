cmake_minimum_required(VERSION 3.22)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/CMakeCommon.cmake)

project(CppFormatLibrary)

include_directories(		
		${CMAKE_CURRENT_SOURCE_DIR}/Format
	)

file( GLOB_RECURSE ROOT_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Format/*.h* ${CMAKE_CURRENT_SOURCE_DIR}/Format/*.inl )

SOURCE_GROUP_BY_DIR(ROOT_HEADER_FILES)

file( GLOB_RECURSE TEST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/UnitTests/Sources/*.c* )
SOURCE_GROUP_BY_DIR(TEST_SOURCE_FILES)

# Add subdirectories
add_subdirectory(UnitTests/External/googletest)

add_definitions(-DUNICODE -D_UNICODE)

add_executable(UnitTests ${ROOT_HEADER_FILES} ${TEST_SOURCE_FILES})
target_link_libraries(UnitTests gtest gtest_main)


