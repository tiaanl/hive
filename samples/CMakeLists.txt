
set(DatabaseSample_SOURCE_FILES
    Database/DatabaseSample.cpp
    )

add_executable(DatabaseSample ${DatabaseSample_SOURCE_FILES})
target_link_libraries(DatabaseSample PUBLIC hive)
set_target_properties(DatabaseSample PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    FOLDER hive/samples
    )
