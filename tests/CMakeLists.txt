add_executable(json_test test.cpp)

target_include_directories(json_test PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(json_test json11)

add_test(NAME json11 COMMAND json_test)