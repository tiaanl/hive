cmake_minimum_required(VERSION 3.14)

project(hive)

add_subdirectory(../nucleus nucleus)

add_subdirectory(samples)

set(hive_HEADER_FILES
    include/hive/Cache.h
    include/hive/Converter.h
    include/hive/ResourceLocator.h
    include/hive/ResourceManager.h
    include/hive/TypeId.h
    )

set(hive_SOURCE_FILES
    )

set(hive_TEST_FILES
    tests/CacheTests.cpp
    tests/ResourceManagerTests.cpp
    )

add_library(hive INTERFACE)
target_include_directories(hive INTERFACE include)
target_link_libraries(hive INTERFACE nucleus)
#set_target_properties(hive PROPERTIES
#    CXX_STANDARD 14
#    CXX_STANDARD_REQUIRED ON
#    CXX_EXTENSIONS OFF
#    FOLDER hive
#    )

add_executable(hiveTests ${hive_TEST_FILES})
target_link_libraries(hiveTests hive testing)
set_target_properties(hiveTests PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    FOLDER hive
    )
