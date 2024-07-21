# for common functions
cmake_minimum_required(VERSION 3.22)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#filter
macro(SOURCE_GROUP_BY_DIR source_files)
    set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
    set(sgbd_root_dir ${CMAKE_SOURCE_DIR})
    foreach(sgbd_file ${${source_files}})
        # 获取相对于项目根目录的路径
        file(RELATIVE_PATH sgbd_fpath ${sgbd_root_dir} ${sgbd_file})
        
        # 确保路径不包含 ".."
        string(REPLACE ".." "" sgbd_fpath ${sgbd_fpath})
        
        # 获取组名
        string(REGEX REPLACE "(.*)/.*" "\\1" sgbd_group_name ${sgbd_fpath})
        string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
        string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
        
        if(sgbd_nogroup)
            set(sgbd_group_name "\\")
        endif(sgbd_nogroup)
        
        source_group(${sgbd_group_name} FILES ${sgbd_file})
    endforeach(sgbd_file)
endmacro(SOURCE_GROUP_BY_DIR)

# Enable C++ 14
if (MSVC AND NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)    
endif()

# use unicode by default
add_definitions(-DUNICODE -D_UNICODE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

