cmake_minimum_required(VERSION 3.5)
project(DcoCppCe CXX)

add_library(DcoCppCe INTERFACE)
add_library(DcoCppCe::DcoCppCe ALIAS DcoCppCe)

target_include_directories(DcoCppCe INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

### installation related stuff ####

 install(TARGETS DcoCppCe
     EXPORT DcoCppCeConfig
     LIBRARY DESTINATION lib COMPONENT DcoCppCe
     ARCHIVE DESTINATION lib COMPONENT DcoCppCe
     RUNTIME DESTINATION bin COMPONENT DcoCppCe
     INCLUDES DESTINATION include)

 install(EXPORT DcoCppCeConfig
     FILE DcoCppCeConfig.cmake
     NAMESPACE DcoCppCe::
     DESTINATION lib/cmake/DcoCppCeConfig
     COMPONENT DcoCppCe)


 install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
     DESTINATION "include"
     COMPONENT DcoCppCe
 )
 
