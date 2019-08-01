cmake_minimum_required(VERSION 3.14)

project(hive)

add_subdirectory(../canvas canvas)

set(hive_HEADER_FILES
    include/hive/Resource.h
    include/hive/ResourceCache.h
    include/hive/ResourceLocator.h
    include/hive/ResourceManager.h
    include/hive/Resources/File.h
    include/hive/Resources/Image.h
    include/hive/Resources/Texture.h
    include/hive/ResourceProcessor.h
    include/hive/Utils.h
    include/hive/Cache.h
    )

set(hive_SOURCE_FILES
    src/Utils.cpp
    )

set(hive_TEST_FILES
    tests/ResourceManagerTests.cpp
    tests/ResourceTests.cpp
    )

add_library(hive ${hive_HEADER_FILES} ${hive_SOURCE_FILES})
target_include_directories(hive PUBLIC include)
target_link_libraries(hive PUBLIC canvas glad nucleus glfw)
set_target_properties(hive PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    FOLDER hive
    )

add_executable(hiveTests ${hive_TEST_FILES})
target_link_libraries(hiveTests hive testing)
set_target_properties(hiveTests PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    FOLDER hive
    )
