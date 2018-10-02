cmake_minimum_required(VERSION 3.5)

project(Eso CXX)

find_package(Eigen3 REQUIRED)
find_package(Yasp REQUIRED)
find_package(DcoCppCe REQUIRED)



add_library(Eso INTERFACE)
add_library(Eso::Eso ALIAS Eso)

target_link_libraries(Eso INTERFACE Eigen3::Eigen Yasp::Yasp DcoCppCe::DcoCppCe)

target_include_directories(Eso INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)


### installation related stuff ####

 install(TARGETS Eso
     EXPORT EsoConfig
     LIBRARY DESTINATION lib COMPONENT Eso
     ARCHIVE DESTINATION lib COMPONENT Eso
     RUNTIME DESTINATION bin COMPONENT Eso
     INCLUDES DESTINATION include)

 install(EXPORT EsoConfig
     FILE EsoConfig.cmake
     NAMESPACE Eso::
     DESTINATION lib/cmake/EsoConfig
     COMPONENT Eso)


 install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
     DESTINATION "include"
     COMPONENT Eso
 )
 

enable_testing()
add_subdirectory(test)
