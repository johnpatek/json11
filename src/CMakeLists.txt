add_library(json11 STATIC json11.cpp)

target_include_directories(json11 PUBLIC ${PROJECT_SOURCE_DIR}/include)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

if(WIN32)
    set(INSTALL_DIRECTORY "C:\\Program Files\\json11")
else()
    set(INSTALL_DIRECTORY /usr)
endif()

set(CMAKE_INSTALL_PREFIX ${INSTALL_DIRECTORY})

install(TARGETS json11
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
        
install(FILES ${PROJECT_SOURCE_DIR}/include/json11/json11.hpp 
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/json11)