cmake_minimum_required(VERSION 3.19)

if (TARGET hive)
    return()
endif ()

project(hive)

add_subdirectory(../nucleus nucleus)

set(HEADER_FILES
    include/hive/importer/importer.h
    include/hive/locator/in_memory_locator.h
    include/hive/locator/locator.h
    include/hive/locator/physical_file_locator.h
    include/hive/resource_manager.h
    include/hive/typed_resource.h
    include/hive/typed_resource_cache.h
    )

set(SOURCE_FILES
    src/locator/in_memory_locator.cpp
    src/locator/physical_file_locator.cpp
    src/resource_manager.cpp
    )

set(TEST_FILES
    tests/resource_manager_tests.cpp
    tests/typed_resource_cache_tests.cpp
    tests/typed_resource_tests.cpp
    )

nucleus_add_library(hive ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(hive PUBLIC nucleus)

nucleus_add_executable(hive_tests ${TEST_FILES})
target_link_libraries(hive_tests PRIVATE hive tests_main)

add_subdirectory(examples)
