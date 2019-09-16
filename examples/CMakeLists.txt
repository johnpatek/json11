add_executable(client client.cpp)

target_link_libraries(client json11)

target_include_directories(client PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(server server.cpp)

target_include_directories(server PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(server json11)
