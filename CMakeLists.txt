cmake_minimum_required(VERSION 3.14)

project(hive)

add_subdirectory(../canvas canvas)

set(hive_HEADER_FILES
    include/hive/ResourceManager.h
    include/hive/ResourceCache.h
    include/hive/TextureResource.h
    include/hive/TextureCache.h
    )

set(hive_SOURCE_FILES
    src/ResourceManager.cpp
    src/TextureCache.cpp
    )

add_library(hive ${hive_HEADER_FILES} ${hive_SOURCE_FILES})
target_include_directories(hive PUBLIC include)
target_link_libraries(hive PUBLIC canvas)
