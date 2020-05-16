add_library(json11 STATIC json11.cpp)

set_property(TARGET json11 PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(json11 PUBLIC ${PROJECT_SOURCE_DIR}/include)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

install(TARGETS json11
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
        
install(FILES ${PROJECT_SOURCE_DIR}/include/json11/json11.hpp 
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/json11)