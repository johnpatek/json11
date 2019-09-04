add_executable(static-test test.cpp)

add_executable(dynamic-test test.cpp)

target_include_directories(static-test PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(static-test json11-static)

target_include_directories(dynamic-test PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(dynamic-test json11-dynamic)

add_test(NAME static-json11 COMMAND static-test)

add_test(NAME dynamic-json11 COMMAND dynamic-test)