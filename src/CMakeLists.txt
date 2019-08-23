add_library(json11_dynamic SHARED json11.cpp)

target_include_directories(json11_dynamic PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(json11_static STATIC json11.cpp)

target_include_directories(json11_static PUBLIC ${PROJECT_SOURCE_DIR}/include)


set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


install(TARGETS json11_dynamic json11_static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
        
install(FILES ${PROJECT_SOURCE_DIR}/include/json11/json11.hpp 
        DESTINATION include/json11/json11.hpp)