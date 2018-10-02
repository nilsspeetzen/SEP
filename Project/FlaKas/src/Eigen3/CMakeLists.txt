cmake_minimum_required(VERSION 3.5)
project(Eigen3 CXX)

add_library(Eigen3 INTERFACE)
add_library(Eigen3::Eigen ALIAS Eigen3)

target_include_directories(Eigen3 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/eigen3>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/eigen3/unsupported>
    $<INSTALL_INTERFACE:include>)

### installation related stuff ####

 install(TARGETS Eigen3
     EXPORT Eigen3Config
     LIBRARY DESTINATION lib COMPONENT Eigen3
     ARCHIVE DESTINATION lib COMPONENT Eigen3
     RUNTIME DESTINATION bin COMPONENT Eigen3
     INCLUDES DESTINATION include)

 install(EXPORT Eigen3Config
     FILE Eigen3Config.cmake
     NAMESPACE Eigen3::
     DESTINATION lib/cmake/Eigen3Config
     COMPONENT Eigen3)


 install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/eigen3/Eigen"
     DESTINATION "include/Eigen"
     COMPONENT Eigen3
 )
 
 install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/eigen3/unsupported/Eigen"
     DESTINATION "include/Eigen"
     COMPONENT Eigen3
 )
 
 
 
