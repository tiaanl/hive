cmake_minimum_required(VERSION 3.14)

if (TARGET hive)
    return()
endif ()

project(hive)

add_subdirectory(../nucleus nucleus)

set(hive_HEADER_FILES
    include/hive/Cache.h
    include/hive/Converter.h
    include/hive/PhysicalResourceLocator.h
    include/hive/ResourceLocator.h
    include/hive/ResourceManager.h
    )

set(hive_SOURCE_FILES
    src/Converter.cpp
    src/PhysicalResourceLocator.cpp
    src/ResourceLocator.cpp
    src/ResourceManager.cpp
    )

set(hive_TESTS_SOURCE_FILES
    tests/CacheTests.cpp
    tests/ResourceManagerTests.cpp
    )

nucleus_add_library(hive ${hive_HEADER_FILES} ${hive_SOURCE_FILES})
target_link_libraries(hive PUBLIC nucleus)

nucleus_add_executable(hiveTests ${hive_TESTS_SOURCE_FILES})
target_link_libraries(hiveTests hive testing)

add_subdirectory(samples)
