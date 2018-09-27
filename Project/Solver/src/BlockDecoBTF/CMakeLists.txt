cmake_minimum_required(VERSION 2.8)

project(BlockDecoBTF C)


add_library(BlockDecoBTF STATIC btf_maxtrans.c  btf_order.c  btf_strongcomp.c include/BlockDecoBTF/btf.h btf_internal.h)
add_library(BlockDecoBTF::BlockDecoBTF ALIAS BlockDecoBTF)


target_include_directories(BlockDecoBTF PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

### installation related stuff ####

 install(TARGETS BlockDecoBTF
     EXPORT BlockDecoBTFConfig
     LIBRARY DESTINATION lib COMPONENT BlockDecoBTF
     ARCHIVE DESTINATION lib COMPONENT BlockDecoBTF
     RUNTIME DESTINATION bin COMPONENT BlockDecoBTF
     INCLUDES DESTINATION include)

 install(EXPORT BlockDecoBTFConfig
     FILE BlockDecoBTFConfig.cmake
     NAMESPACE BlockDecoBTF::
     DESTINATION lib/cmake/BlockDecoBTFConfig
     COMPONENT BlockDecoBTF)


 install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
     DESTINATION "include"
     COMPONENT BlockDecoBTF
 )


