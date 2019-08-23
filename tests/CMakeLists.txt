add_executable(static_test test.cpp)

add_executable(dynamic_test test.cpp)

target_include_directories(static_test PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(static_test json11_static)

target_include_directories(dynamic_test PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(dynamic_test json11_dynamic)

add_test(NAME static_json11 COMMAND static_test)

add_test(NAME dynamic_json11 COMMAND dynamic_test)