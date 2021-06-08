cmake_minimum_required(VERSION 3.14)

if (TARGET hive)
    return()
endif ()

project(hive)

add_subdirectory(../nucleus nucleus)

set(hive_HEADER_FILES
    include/hive/importer.h
    include/hive/in_memory_locator.h
    include/hive/locator.h
    include/hive/physical_file_locator.h
    include/hive/typed_resource.h
    include/hive/typed_resource_cache.h
    )

set(hive_SOURCE_FILES
    src/physical_file_locator.cpp
    src/in_memory_locator.cpp
    )

set(hive_TESTS_SOURCE_FILES
    tests/typed_resource_cache_tests.cpp
    tests/typed_resource_tests.cpp
    )

nucleus_add_library(hive ${hive_HEADER_FILES} ${hive_SOURCE_FILES})
target_link_libraries(hive PUBLIC nucleus)

nucleus_add_executable(hiveTests ${hive_TESTS_SOURCE_FILES})
target_link_libraries(hiveTests PRIVATE hive tests_main)

add_subdirectory(examples)
