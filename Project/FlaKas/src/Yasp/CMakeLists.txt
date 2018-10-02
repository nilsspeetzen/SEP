cmake_minimum_required(VERSION 3.5)
project(Yasp CXX)



add_library(Yasp STATIC src/Yasp.cpp include/Yasp/Yasp.hpp)
add_library(Yasp::Yasp ALIAS Yasp)

target_include_directories(Yasp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
    
    
### installation related stuff ####

 install(TARGETS Yasp
     EXPORT YaspConfig
     LIBRARY DESTINATION lib COMPONENT Yasp
     ARCHIVE DESTINATION lib COMPONENT Yasp
     RUNTIME DESTINATION bin COMPONENT Yasp
     INCLUDES DESTINATION include)

 install(EXPORT YaspConfig
     FILE YaspConfig.cmake
     NAMESPACE Yasp::
     DESTINATION lib/cmake/YaspConfig
     COMPONENT Yasp)


 install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
     DESTINATION "include"
     COMPONENT Yasp
 )

